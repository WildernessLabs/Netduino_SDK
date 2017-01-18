/*
 *    Copyright (C) 2010 Renesas Electronics America Inc. All rights reserved.
 */
 
#include <tinyhal.h>
#include "Serial_Flash_BL.h"
#include "RSPI.h"

//--//

extern struct MEMORY_MAPPED_SERIAL_BLOCK_CONFIG g_SerialFlash_BL_Config;

extern FILESYSTEM_DRIVER_INTERFACE g_FAT32_FILE_SYSTEM_DriverInterface;
extern STREAM_DRIVER_INTERFACE g_FAT32_STREAM_DriverInterface;
extern struct BlockStorageDevice g_SerialFlash_Device;

extern FileSystemVolume g_SerialFlash_FS;

static BYTE s_sectorBuff[SF_PAGE_SIZE];

//--//


BOOL SF_BS_Driver::InitializeDevice(void *context)
{
    BLOCK_CONFIG *config = (BLOCK_CONFIG*)context;

    if(!config || !config->BlockDeviceInformation)
    {
        return FALSE;
    }
        
    RSPI_Driver::RSPI_Initialize();
    return TRUE;
}

BOOL SF_BS_Driver::UnInitializeDevice(void *context)
{    
    return TRUE;
}

BOOL SF_BS_Driver::ReadProductID(void *context, SERIAL_WORD& ManufacturerCode, SERIAL_WORD& DeviceCode)
{
    unsigned char       id[20]; 
    
    SF_Chip_ID(id);
    
  ManufacturerCode = id[0];

  DeviceCode = id[1];

    return TRUE;
}

const BlockDeviceInfo *SF_BS_Driver::GetDeviceInfo(void *context)
{
    BLOCK_CONFIG *config = (BLOCK_CONFIG*)context;

    return config->BlockDeviceInformation;
}

BOOL SF_BS_Driver::ReadSector(SectorAddress sectorAddress, UINT32 Offset, UINT32 NumBytes, BYTE* pSectorBuff, UINT32 BytesPerSector)
{
    NumBytes = (NumBytes + Offset > BytesPerSector ? BytesPerSector - Offset : NumBytes);

    sectorAddress *= BytesPerSector;
        
    SF_Byte_Read(sectorAddress, s_sectorBuff, BytesPerSector);
        
    memcpy(pSectorBuff, s_sectorBuff + Offset, NumBytes);
        
    return TRUE;
}

BOOL SF_BS_Driver::Read(void *context, ByteAddress phyAddress, UINT32 NumBytes, BYTE *pSectorBuff)
{
    NATIVE_PROFILE_HAL_DRIVERS_FLASH();
    UINT32 RangeIndex;
    UINT32 RegionIndex;
    UINT32 BytesPerSector;

    MEMORY_MAPPED_SERIAL_BLOCK_CONFIG* pConfig = (MEMORY_MAPPED_SERIAL_BLOCK_CONFIG*)context;
    const BlockDeviceInfo *    deviceInfo = pConfig->BlockConfig.BlockDeviceInformation;
    
    if(deviceInfo->FindRegionFromAddress(phyAddress, RegionIndex, RangeIndex))
    {
        BytesPerSector = deviceInfo->BytesPerSector;
        SERIAL_WORD *pBuf = (SERIAL_WORD*)pSectorBuff;
        BlockRegionInfo* pRegion = (BlockRegionInfo*)&deviceInfo->Regions[RegionIndex];
        
        ByteAddress StartSector = phyAddress / deviceInfo->BytesPerSector;

        UINT32 offset = phyAddress - (StartSector * deviceInfo->BytesPerSector);

        UINT32 bytes  = (NumBytes + offset > BytesPerSector ? BytesPerSector - offset : NumBytes);
                
        while(NumBytes > 0)
        {
            if(!ReadSector(StartSector, offset, bytes, pBuf, BytesPerSector))
            {
                return FALSE;
            }
            
            offset    = 0;
            pBuf      = (SERIAL_WORD*)((UINT32)pBuf + bytes);
            NumBytes -= bytes;
            StartSector++;

            bytes = __min(BytesPerSector, NumBytes);
        }
        
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL SF_BS_Driver::Write(void *context, ByteAddress Address, UINT32 NumBytes, BYTE *pSectorBuff, BOOL ReadModifyWrite )
{
    NATIVE_PROFILE_PAL_FLASH();

    BYTE * pData;
    BYTE * pBuf = NULL;
    MEMORY_MAPPED_SERIAL_BLOCK_CONFIG* config = (MEMORY_MAPPED_SERIAL_BLOCK_CONFIG*)context;
    const BlockDeviceInfo * deviceInfo = config->BlockConfig.BlockDeviceInformation;
    
    UINT32 region, range;

    if(ReadModifyWrite) 
    {
        BOOL fRet = TRUE;
        
        if(!deviceInfo->FindRegionFromAddress(Address, region, range)) return FALSE;

        UINT32      bytesPerBlock   = deviceInfo->Regions[region].BytesPerBlock;
        UINT32      regionEnd       = deviceInfo->Regions[region].Start + deviceInfo->Regions[region].Size();
        UINT32      offset          = Address % bytesPerBlock;
        ByteAddress addr            = Address;
        ByteAddress addrEnd         = Address + NumBytes;
        UINT32      index           = 0;

        pBuf = (BYTE*)private_malloc(bytesPerBlock);

        if(pBuf == NULL)
        {
 
            return FALSE;
        }

        while(fRet && addr < addrEnd)
        {
            ByteAddress sectAddr = (addr - offset);
             
            if(offset == 0 && NumBytes >= bytesPerBlock)
            {
                pData = &pSectorBuff[index];
            }
            else
            {
                int bytes = __min(bytesPerBlock - offset, NumBytes); 
                
                //memcpy( &pBuf[0]     , (void*)sectAddr    , bytesPerBlock );
                Read(context, sectAddr, bytesPerBlock, (BYTE *)&pBuf[0]);
                memcpy( &pBuf[offset], &pSectorBuff[index], bytes         );

                pData = pBuf;
            }

            if(!EraseBlock( context, sectAddr ))
{
                fRet = FALSE;
                break;
            }

            fRet = WriteX(context, sectAddr, bytesPerBlock, pData, ReadModifyWrite, TRUE);

            NumBytes -= bytesPerBlock - offset;
            addr     += bytesPerBlock - offset;
            index    += bytesPerBlock - offset;
            offset    = 0;

            if(NumBytes > 0 && addr >= regionEnd)
            {
                region++;

                if(region >= deviceInfo->NumRegions)
                {
                    fRet = FALSE;
                }
                else
                {
                    regionEnd       = deviceInfo->Regions[region].Start + deviceInfo->Regions[region].Size();
                    bytesPerBlock   = deviceInfo->Regions[region].BytesPerBlock;

                    private_free(pBuf);

                    pBuf = (BYTE*)private_malloc(bytesPerBlock);

                    if(pBuf == NULL)
                    {
                        fRet = FALSE;
                    }
                }
            }
                
        }

        if(pBuf != NULL)
        {
            private_free(pBuf);
        }

        return fRet;            
    }
    else
    {
        return WriteX(context, Address, NumBytes, pSectorBuff, ReadModifyWrite, TRUE);
    }
}

BOOL SF_BS_Driver::Memset(void *context, ByteAddress phyAddr, UINT8 Data, UINT32 NumBytes )
{
    NATIVE_PROFILE_PAL_FLASH();

    return WriteX( context, phyAddr, NumBytes, &Data, TRUE, FALSE );
}

BOOL SF_BS_Driver::WriteX(void *context, ByteAddress phyAddr, UINT32 NumBytes, BYTE *pSectorBuff, BOOL ReadModifyWrite, BOOL fIncrementDataPtr )
{
    NATIVE_PROFILE_PAL_FLASH();

    UINT32 RangeIndex;
    UINT32 RegionIndex;
    UINT32 BytesPerSector;
    UINT32 offset;
    UINT32 bytes;

    BLOCK_CONFIG* pConfig = (BLOCK_CONFIG*)context;

    SERIAL_WORD *pData, *pWrite;

    // find the corresponding region     
    if(!pConfig->BlockDeviceInformation->FindRegionFromAddress(phyAddr, RegionIndex, RangeIndex))
        return FALSE;

    pData = (SERIAL_WORD*)pSectorBuff;
    BytesPerSector = pConfig->BlockDeviceInformation->BytesPerSector;

    const BlockDeviceInfo *    deviceInfo = pConfig->BlockDeviceInformation;        
        BlockRegionInfo* pRegion = (BlockRegionInfo*)&deviceInfo->Regions[RegionIndex];
    
    ByteAddress StartSector = phyAddr / BytesPerSector;
    
    offset = phyAddr - (StartSector * BytesPerSector);

    bytes = (NumBytes + offset > BytesPerSector ? BytesPerSector - offset : NumBytes);

    while(NumBytes > 0)
    {
        // if we are using memset, or if the bytes written are less than the BytesPerSector then do read/modify/write
        if(!fIncrementDataPtr || (bytes != BytesPerSector))
        {   
            if(bytes != BytesPerSector)
            {
                if(!ReadSector(StartSector, 0, BytesPerSector, s_sectorBuff, BytesPerSector))
                {
                    return FALSE;
                }

            }
            
            pWrite = (SERIAL_WORD*)&s_sectorBuff[0];

            if(fIncrementDataPtr)
            {
                memcpy(&pWrite[offset], pData, bytes);
            }
            else
            {
                memset(&pWrite[offset], *pData, bytes);
            }
        }
        else
        {
            pWrite = pData;
        }

        SF_Byte_Program((UINT32)(StartSector*BytesPerSector), (unsigned char *)pWrite, BytesPerSector);
                        
        if(fIncrementDataPtr) pData = (SERIAL_WORD*)((UINT32)pData + bytes);

        NumBytes   -= bytes;
        offset      = 0;
        StartSector++ ;
        bytes = __min(BytesPerSector, NumBytes);        
    }

    return TRUE;

}

BOOL SF_BS_Driver::GetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return TRUE;
}

BOOL SF_BS_Driver::SetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return TRUE;
}

BOOL SF_BS_Driver::IsBlockErased(void *context, ByteAddress phyAddress, UINT32 BlockLength)
{

    NATIVE_PROFILE_HAL_DRIVERS_FLASH();

    UINT32 RegionIndex;
    UINT32 RangeIndex;
    UINT32 SectorsCount;
    UINT32 BytesPerSector;
    UINT32 BytesCount;

    BLOCK_CONFIG* pConfig = (BLOCK_CONFIG*)context;

    // this is static buffer, as the driver is current tailor for SD, a page size is 2048 bytes.
    BYTE *pSectorBuff = s_sectorBuff;


    BYTE state_After_Erase = 0xff;//g_SerialFlash_BL_Configuration.State_After_Erase ? 0xFF : 0x00;

    if(!pConfig->BlockDeviceInformation->FindRegionFromAddress(phyAddress, RegionIndex, RangeIndex))
        return FALSE;

    const BlockRegionInfo* pRegion = &pConfig->BlockDeviceInformation->Regions[RegionIndex];

    // as the input arg Sector may not be the startSector address of a block,
    // we need to recalculate it.
    BytesPerSector  = pConfig->BlockDeviceInformation->BytesPerSector;

    SectorsCount = BlockLength / BytesPerSector;
    
    if ((BlockLength % BytesPerSector) > 0) SectorsCount++;

    ByteAddress StartSector = phyAddress / BytesPerSector;
    
    UINT32 offset = phyAddress - (StartSector * BytesPerSector);
    
    UINT32 bytes  = (BlockLength + offset > BytesPerSector ? BytesPerSector - offset : BlockLength);
    
    BytesCount = 0;
    for(UINT32 i = 0; i < SectorsCount; i++)
    {
        Read(context, phyAddress, bytes, pSectorBuff);
        for(UINT32 j = 0; j < bytes; j++)
        {
            BytesCount++;
            if (BytesCount > BlockLength)
            {
                return TRUE;
            }
            if(pSectorBuff[j] != state_After_Erase)
            {
                return FALSE;
            }
        }
        bytes = BytesPerSector;
        offset = 0;
        phyAddress += BytesPerSector;
    }
    return TRUE;
}


BOOL SF_BS_Driver::EraseBlock(void *context, ByteAddress phyAddr)
{
    NATIVE_PROFILE_HAL_DRIVERS_FLASH();

    UINT32 iRegion, iRange, BytesPerSector, i, j, k;
    
    unsigned char * Buff;
    
    Buff = (unsigned char *)private_malloc(0x10000);

    MEMORY_MAPPED_SERIAL_BLOCK_CONFIG* config = (MEMORY_MAPPED_SERIAL_BLOCK_CONFIG*)context;
    const BlockDeviceInfo *    deviceInfo = config->BlockConfig.BlockDeviceInformation;

    if (deviceInfo->Attribute.WriteProtected) return FALSE;

    if (!deviceInfo->FindRegionFromAddress(phyAddr, iRegion, iRange)) return FALSE;

    const BlockRegionInfo* pRegion = &deviceInfo->Regions[iRegion];

    BytesPerSector = deviceInfo->BytesPerSector;

    SF_Sector_Erase(/*i * SF_SECTOR_SIZE*/phyAddr);

    return TRUE;
}

void SF_BS_Driver::SetPowerState(void *context, UINT32 State)
{
    // our flash driver is always Power ON
    return ;
}

UINT32 SF_BS_Driver::MaxSectorWrite_uSec(void *context)
{
    NATIVE_PROFILE_PAL_FLASH();

    BLOCK_CONFIG *config = (BLOCK_CONFIG*)context;

    return config->BlockDeviceInformation->MaxSectorWrite_uSec;
}

UINT32 SF_BS_Driver::MaxBlockErase_uSec(void *context)
{
    NATIVE_PROFILE_PAL_FLASH();

    BLOCK_CONFIG *config = (BLOCK_CONFIG*)context;

    return config->BlockDeviceInformation->MaxBlockErase_uSec;
}

void SF_BS_Driver::SF_Chip_ID(unsigned char * id)
{
    RSPI_Driver::Chip_ID(id);
}

void SF_BS_Driver::SF_Sector_Erase(unsigned long addr)
{
    RSPI_Driver::Protect_Ctrl(0);       //Turn off write-proctection on SerialFlash 
    RSPI_Driver::Sector_Erase(addr);
    RSPI_Driver::Protect_Ctrl(1);       //Turn on write-proctection on SerialFlash      
}

void SF_BS_Driver::SF_Byte_Program(unsigned int addr, unsigned char *buf, int size)
{
    RSPI_Driver::Protect_Ctrl(0);       //Turn off write-proctection on SerialFlash
    RSPI_Driver::Byte_Program(addr, buf, size);
    RSPI_Driver::Protect_Ctrl(1);       //Turn on write-proctection on SerialFlash
    
}

void SF_BS_Driver::SF_Byte_Read(unsigned int addr, unsigned char *buf, int size)
{
    RSPI_Driver::Byte_Read(addr, buf, size);        
}

struct IBlockStorageDevice g_SerialFlash_BL_DeviceTable = 
{                          
    &SF_BS_Driver::InitializeDevice,
    &SF_BS_Driver::UnInitializeDevice,
    &SF_BS_Driver::GetDeviceInfo,
    &SF_BS_Driver::Read,
    &SF_BS_Driver::Write,
    &SF_BS_Driver::Memset,
    &SF_BS_Driver::GetSectorMetadata,
    &SF_BS_Driver::SetSectorMetadata,
    &SF_BS_Driver::IsBlockErased,
    &SF_BS_Driver::EraseBlock,
    &SF_BS_Driver::SetPowerState,
    &SF_BS_Driver::MaxSectorWrite_uSec,
    &SF_BS_Driver::MaxBlockErase_uSec,
};