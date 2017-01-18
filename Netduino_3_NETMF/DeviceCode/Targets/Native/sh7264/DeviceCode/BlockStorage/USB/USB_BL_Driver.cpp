
/*
 *    Copyright (C) 2010 Renesas Electronics America Inc. All rights reserved.
 */
 
#include <tinyhal.h>
#include "USB_BL.h"
#include "USB_Host.h"

//--//

extern USB_CONFIGURATION_INFO g_SH7264_USB_Info;

extern struct USB_BL_CONFIGURATION g_USB_BL_Configuration;
extern struct USB_BLOCK_CONFIG g_SH7264_USB_BL_Config;

extern FILESYSTEM_DRIVER_INTERFACE g_FAT32_FILE_SYSTEM_DriverInterface;
extern STREAM_DRIVER_INTERFACE g_FAT32_STREAM_DriverInterface;
extern struct BlockStorageDevice g_USB_Device;

extern FileSystemVolume g_SH7264_USB_FS;

static BYTE s_sectorBuff[526];

//--//


#define BIT_EDIANW(data)            ((UINT16)data & 0x00FF) << 8 | (UINT16)data >> 8    
#define BIT_EDIANL(data)            ((UINT32)data & 0x000000FF) << 24 | ((UINT32)data & 0x0000FF00) << 8 | ((UINT32)data & 0x00FF0000) >> 8 | ((UINT32)data & 0xFF000000) >> 24 

void    USB_Convert_Endian_FAT_MBR(UINT32 buf)
{
    *(UINT16*)(buf+510) = BIT_EDIANW(*(UINT16*)(buf+510));//ofs:510. 
}

void    USB_Convert_Endian_FAT_DBR(UINT32 buf)
{
    *(UINT16*)(buf+14) = BIT_EDIANW(*(UINT16*)(buf+14));//ofs:14. 
    *(UINT16*)(buf+22) = BIT_EDIANW(*(UINT16*)(buf+22));//ofs:22. 
    *(UINT16*)(buf+24) = BIT_EDIANW(*(UINT16*)(buf+24));//ofs:22. 
    *(UINT16*)(buf+26) = BIT_EDIANW(*(UINT16*)(buf+26));//ofs:22. 
    *(UINT32*)(buf+28) = BIT_EDIANL(*(UINT32*)(buf+28));//ofs:28. 
    *(UINT32*)(buf+32) = BIT_EDIANL(*(UINT32*)(buf+32));//ofs:32. 
    *(UINT32*)(buf+36) = BIT_EDIANL(*(UINT32*)(buf+36));//ofs:36. 
    *(UINT16*)(buf+40) = BIT_EDIANW(*(UINT16*)(buf+40));//ofs:40. 
    *(UINT16*)(buf+42) = BIT_EDIANW(*(UINT16*)(buf+42));//ofs:42. 
    *(UINT32*)(buf+44) = BIT_EDIANL(*(UINT32*)(buf+44));//ofs:44. 
    *(UINT16*)(buf+48) = BIT_EDIANW(*(UINT16*)(buf+48));//ofs:48. 
    *(UINT16*)(buf+50) = BIT_EDIANW(*(UINT16*)(buf+50));//ofs:50. 
    *(UINT16*)(buf+510) = BIT_EDIANW(*(UINT16*)(buf+510));//ofs:510. 
}

void    USB_Convert_Endian_FAT_FSINFO(UINT32 buf)
{
    *(UINT32*)(buf) = BIT_EDIANL(*(UINT32*)(buf));//ofs:0. 
    *(UINT32*)(buf+484) = BIT_EDIANL(*(UINT32*)(buf+484));//ofs:484. 
    *(UINT32*)(buf+488) = BIT_EDIANL(*(UINT32*)(buf+488));//ofs:488. 
    *(UINT32*)(buf+492) = BIT_EDIANL(*(UINT32*)(buf+492));//ofs:492.    
    *(UINT32*)(buf+508) = BIT_EDIANL(*(UINT32*)(buf+508));//ofs:508. 
}

BOOL USB_BS_Driver::CPU_USB_ChipInitialize(void *context)
{
    BLOCK_CONFIG *config = (BLOCK_CONFIG*)context;

    if(!config || !config->BlockDeviceInformation)
    {
        return FALSE;
    }
        
        SH7264_USB_BL_Driver::USB_IRQ_Disable();
        SH7264_USB_BL_Driver::USB_Initialize();
        SH7264_USB_BL_Driver::USB_IRQ_Enable();

    return TRUE;
}

BOOL USB_BS_Driver::CPU_USB_Global_ISR(void *Param)
{
    BOOL isMBR = FALSE;
    BOOL isGood = FALSE;
    GLOBAL_LOCK(irq);
    if(!((BlockDeviceInfo* )g_SH7264_USB_BL_Config.BlockDeviceInformation))
    {
      return FALSE;
    }
    
    SH7264_USB_BL_Driver::USB_Global_ISR();
    
    if(g_SH7264_USB_Info.initialized == 1)
    {       
        BlockDeviceInfo* pDevInfo = (BlockDeviceInfo* )g_SH7264_USB_BL_Config.BlockDeviceInformation;
        if(g_SH7264_USB_Info.device_speed > 0)
        {
            UINT32 SectorsPerBlock, BytesPerSector, baseAddress, offset, size, MemCapacity;

            baseAddress = 0;
            offset = 0;
            MemCapacity = 0xFFFFFFFF;//pDevInfo->Size;
            while(TRUE)
            {
                if (SH7264_USB_BL_Driver::USB_Read_Sectors(baseAddress, 1, (UINT32)s_sectorBuff)==TRUE)
                {               
                    if ((s_sectorBuff[0] == 0xEB) || (s_sectorBuff[2] == 0x90))
                    {
                        isGood = TRUE;
                        break;
                    }
                    offset = (s_sectorBuff[454] | s_sectorBuff[455] << 8 | s_sectorBuff[456] << 16 | s_sectorBuff[457] << 24);
                    size = (s_sectorBuff[458] | s_sectorBuff[459] << 8 | s_sectorBuff[460] << 16 | s_sectorBuff[461] << 24)*512;
    
                    if (((s_sectorBuff[510]==0x55)&&(s_sectorBuff[511]==0xaa)) &&
                          ((s_sectorBuff[446]==0x00)||(s_sectorBuff[446]==0x80)) &&
                          (offset > 0) && (size >0))
                         
                    {   
                        if(offset + size > MemCapacity)
                        {
                            //hal_printf("USB Flash: Invalid boot record\r\n");   
                            //return FALSE;
                            break;
                        }
                
                        baseAddress += offset;
                        isMBR = TRUE;
                    }
                    else
                    {
                        //hal_printf("USB Flash: Invalid boot record\r\n");   
                        //return FALSE;
                        break;
                    }               
                }
                else
                {
                    //hal_printf("USB Flash: Initialization failed\r\n"); 
                    //return FALSE;               
                    break;
                }
            }
            SectorsPerBlock     = s_sectorBuff[0xD];//sertor per clustor
            BytesPerSector      = s_sectorBuff[0xB] | s_sectorBuff[0xC]<< 8;// data bytes per sector is always 512
            if (isGood)
            {
                if (isMBR) {
                    MemCapacity = (offset * BytesPerSector) + size;
                }
                else {
                    MemCapacity =   (s_sectorBuff[0x13] | s_sectorBuff[0x14] << 8)* BytesPerSector;;
                    if (MemCapacity == 0)
                    {
                        MemCapacity         = (s_sectorBuff[0x20] | s_sectorBuff[0x21] << 8 | s_sectorBuff[0x22] << 16 | s_sectorBuff[0x23] << 24) //Sector per 1 partition
                                              * BytesPerSector;
                    }   
                }
                pDevInfo->BytesPerSector = BytesPerSector;
                pDevInfo->Size           = MemCapacity;
        
                BlockRegionInfo* pRegions = (BlockRegionInfo*)&pDevInfo->Regions[0];

                pRegions[0].BytesPerBlock = SectorsPerBlock * pDevInfo->BytesPerSector;
                pRegions[0].NumBlocks     = (MemCapacity) / pRegions[0].BytesPerBlock;    
                if ((MemCapacity % pRegions[0].BytesPerBlock) > 0) pRegions[0].NumBlocks++;
                    pRegions->Start = 0;
    
                BlockRange* pRanges   = (BlockRange*)&pRegions[0].BlockRanges[0];
    
                pRanges[0].StartBlock = 0;
                pRanges[0].EndBlock   = pRegions[0].NumBlocks-1;
            }
            else
            {
                hal_printf("USB Flash: Invalid device\r\n");   
            }               
        }
        FS_Initialize();
        FileSystemVolumeList::Initialize();
        FS_AddVolumes();
        FileSystemVolumeList::InitializeVolumes();
    }
    else {
        FileSystemVolumeList::RemoveVolume( &g_SH7264_USB_FS, FALSE );
    }
    
    return TRUE;
}

BOOL USB_BS_Driver::CPU_USB_ChipUnInitialize(void *context)
{    
    return TRUE;
}

BOOL USB_BS_Driver::CPU_USB_ReadProductID(void *context, BYTE *ManufacturerCode, BYTE *OEMID, BYTE *ProductName)
{
        if (g_SH7264_USB_Info.initialized)
        {
        *ManufacturerCode = g_SH7264_USB_Info.disk.descriptor.iManufacturer;
    
        *(UINT16 *)OEMID = g_SH7264_USB_Info.disk.descriptor.idVendor;
    
        memcpy(&ProductName, &g_SH7264_USB_Info.disk.descriptor.ProductName[0], 30);
    }

    return TRUE;
}

const BlockDeviceInfo *USB_BS_Driver::CPU_USB_GetDeviceInfo(void *context)
{
    BLOCK_CONFIG *config = (BLOCK_CONFIG*)context;

    return config->BlockDeviceInformation;
}

BOOL USB_BS_Driver::CPU_USB_ReadSector(SectorAddress sectorAddress, UINT32 Offset, UINT32 NumBytes, BYTE* pSectorBuff, UINT32 BytesPerSector)
{
    NumBytes = (NumBytes + Offset > BytesPerSector ? BytesPerSector - Offset : NumBytes);

        if(g_SH7264_USB_Info.device_speed > 0)
        {
            if(g_SH7264_USB_Info.initialized == 1)
            {
                SH7264_USB_BL_Driver::USB_Read_Sectors((UINT32)sectorAddress, 1, (UINT32)s_sectorBuff);
                if(g_SH7264_USB_Info.error != 0)
                {
                    return FALSE;
                }
            }
            else
            {
                return FALSE;
            }
        }
        else
        {
                return FALSE;
        }

        memcpy(pSectorBuff, s_sectorBuff + Offset, NumBytes);
        
        return TRUE;
}

BOOL USB_BS_Driver::CPU_USB_Read(void *context, ByteAddress phyAddress, UINT32 NumBytes, BYTE *pSectorBuff)
{
    NATIVE_PROFILE_HAL_DRIVERS_FLASH();
    UINT32 RangeIndex;
    UINT32 RegionIndex;
    UINT32 BytesPerSector;

    BLOCK_CONFIG* pConfig = (BLOCK_CONFIG*)context;
    
    if(pConfig->BlockDeviceInformation->FindRegionFromAddress(phyAddress, RegionIndex, RangeIndex))
    {
        ByteAddress StartSector = pConfig->BlockDeviceInformation->PhysicalToSectorAddress( &pConfig->BlockDeviceInformation->Regions[RegionIndex], phyAddress);

        BytesPerSector = pConfig->BlockDeviceInformation->BytesPerSector;

        CHIP_WORD *pBuf = (CHIP_WORD*)pSectorBuff;

        UINT32 offset = phyAddress - (StartSector * pConfig->BlockDeviceInformation->BytesPerSector);

        UINT32 bytes  = (NumBytes + offset > BytesPerSector ? BytesPerSector - offset : NumBytes);
                
        while(NumBytes > 0)
        {
            if(!CPU_USB_ReadSector(StartSector, offset, bytes, pBuf, BytesPerSector))
            {
                return FALSE;
            }
            
            offset    = 0;
            pBuf      = (CHIP_WORD*)((UINT32)pBuf + bytes);
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

BOOL USB_BS_Driver::CPU_USB_Write(void *context, ByteAddress phyAddr, UINT32 NumBytes, BYTE *pSectorBuff, BOOL ReadModifyWrite )
{
    NATIVE_PROFILE_PAL_FLASH();

    return CPU_USB_WriteX( context, phyAddr, NumBytes, pSectorBuff, ReadModifyWrite, TRUE );
}

BOOL USB_BS_Driver::CPU_USB_Memset(void *context, ByteAddress phyAddr, UINT8 Data, UINT32 NumBytes )
{
    NATIVE_PROFILE_PAL_FLASH();

    return CPU_USB_WriteX( context, phyAddr, NumBytes, &Data, TRUE, FALSE );
}

BOOL USB_BS_Driver::CPU_USB_WriteX(void *context, ByteAddress phyAddr, UINT32 NumBytes, BYTE *pSectorBuff, BOOL ReadModifyWrite, BOOL fIncrementDataPtr )
{
    NATIVE_PROFILE_PAL_FLASH();

    UINT32 RangeIndex;
    UINT32 RegionIndex;
    UINT32 BytesPerSector;
    UINT32 offset;
    UINT32 bytes;
   // BYTE response;

    BLOCK_CONFIG* pConfig = (BLOCK_CONFIG*)context;

    CHIP_WORD *pData, *pWrite;

    // find the corresponding region     
    if(!pConfig->BlockDeviceInformation->FindRegionFromAddress(phyAddr, RegionIndex, RangeIndex))
        return FALSE;

    ByteAddress StartSector = pConfig->BlockDeviceInformation->PhysicalToSectorAddress( &pConfig->BlockDeviceInformation->Regions[RegionIndex], phyAddr);

    pData = (CHIP_WORD*)pSectorBuff;
    BytesPerSector = pConfig->BlockDeviceInformation->BytesPerSector;

    offset = phyAddr - (StartSector * BytesPerSector);

    bytes = (NumBytes + offset > BytesPerSector ? BytesPerSector - offset : NumBytes);

    while(NumBytes > 0)
    {
        // if we are using memset, or if the bytes written are less than the BytesPerSector then do read/modify/write
        if(!fIncrementDataPtr || (bytes != BytesPerSector))
        {   
            if(bytes != BytesPerSector)
            {
                if(!CPU_USB_ReadSector(StartSector, 0, BytesPerSector, s_sectorBuff, BytesPerSector))
                {
                    return FALSE;
                }

            }
            
            pWrite = (CHIP_WORD*)&s_sectorBuff[0];

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

                if(g_SH7264_USB_Info.device_speed > 0)
                {
                    if(g_SH7264_USB_Info.initialized == 1)
                    {
                        SH7264_USB_BL_Driver::USB_Write_Sectors((UINT32)StartSector, 1, (UINT32)pWrite);    
                        
                        if(g_SH7264_USB_Info.error != 0)
                        {
                                return FALSE;
                        }
                    }
                    else
                    {
                        return FALSE;
                    }
                }
                else
                {
                        return FALSE;
                }

        if(fIncrementDataPtr) pData = (CHIP_WORD*)((UINT32)pData + bytes);

        NumBytes   -= bytes;
        offset      = 0;
        StartSector++;
        bytes = __min(BytesPerSector, NumBytes);        
    }

    return TRUE;

}

BOOL USB_BS_Driver::CPU_USB_GetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return TRUE;
}

BOOL USB_BS_Driver::CPU_USB_SetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return TRUE;
}

BOOL USB_BS_Driver::CPU_USB_IsBlockErased(void *context, ByteAddress phyAddress, UINT32 BlockLength)
{

    NATIVE_PROFILE_HAL_DRIVERS_FLASH();

    UINT32 RegionIndex;
    UINT32 RangeIndex;
    UINT32 SectorsPerBlock;
    UINT32 BytesPerSector;

    BLOCK_CONFIG* pConfig = (BLOCK_CONFIG*)context;

    // this is static buffer, as the driver is current tailor for SD, a page size is 2048 bytes.
    BYTE *pSectorBuff = s_sectorBuff;


    BYTE state_After_Erase = 0xff;//g_SH7264_USB_BL_Configuration.State_After_Erase ? 0xFF : 0x00;

    if(!pConfig->BlockDeviceInformation->FindRegionFromAddress(phyAddress, RegionIndex, RangeIndex))
        return FALSE;

    ByteAddress StartSector = pConfig->BlockDeviceInformation->PhysicalToSectorAddress( &pConfig->BlockDeviceInformation->Regions[RegionIndex], phyAddress);

    const BlockRegionInfo* pRegion = &pConfig->BlockDeviceInformation->Regions[RegionIndex];

    // as the input arg Sector may not be the startSector address of a block,
    // we need to recalculate it.
    BytesPerSector  = pConfig->BlockDeviceInformation->BytesPerSector;
    SectorsPerBlock = (pRegion->BytesPerBlock / BytesPerSector);

    StartSector = (StartSector / SectorsPerBlock) * SectorsPerBlock;
    
    for(UINT32 i = 0; i < SectorsPerBlock; i++)
    {
        CPU_USB_Read(context, StartSector, BytesPerSector, pSectorBuff);
        for(UINT32 j = 0; j < BytesPerSector; j++)
        {
            if(pSectorBuff[j] != state_After_Erase)
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}


BOOL USB_BS_Driver::CPU_USB_EraseSectors(SectorAddress Address, INT32 SectorCount)
{
    return (SH7264_USB_BL_Driver::USB_Erase_Sectors(Address, SectorCount));
}

BOOL USB_BS_Driver::CPU_USB_EraseBlock(void *context, ByteAddress phyAddr)
{
    NATIVE_PROFILE_HAL_DRIVERS_FLASH();

    UINT32 RangeIndex;
    UINT32 RegionIndex;

    BLOCK_CONFIG* pConfig = (BLOCK_CONFIG*)context;

    if(!pConfig->BlockDeviceInformation->FindRegionFromAddress(phyAddr, RegionIndex, RangeIndex))
        return FALSE;

    const BlockRegionInfo* pRegion = &pConfig->BlockDeviceInformation->Regions[RegionIndex];

    ByteAddress StartSector = pConfig->BlockDeviceInformation->PhysicalToSectorAddress( pRegion, phyAddr );

    UINT32 SectorsPerBlock = pRegion->BytesPerBlock / pConfig->BlockDeviceInformation->BytesPerSector;

    SectorAddress SectorAddress = (StartSector / SectorsPerBlock) * SectorsPerBlock;

    CPU_USB_EraseSectors(SectorAddress, SectorsPerBlock);

    return TRUE;

}

void USB_BS_Driver::CPU_USB_SetPowerState(void *context, UINT32 State)
{
    // our flash driver is always Power ON
    return ;
}

UINT32 USB_BS_Driver::CPU_USB_MaxSectorWrite_uSec(void *context)
{
    NATIVE_PROFILE_PAL_FLASH();

    BLOCK_CONFIG *config = (BLOCK_CONFIG*)context;

    return config->BlockDeviceInformation->MaxSectorWrite_uSec;
}

UINT32 USB_BS_Driver::CPU_USB_MaxBlockErase_uSec(void *context)
{
    NATIVE_PROFILE_PAL_FLASH();

    BLOCK_CONFIG *config = (BLOCK_CONFIG*)context;

    return config->BlockDeviceInformation->MaxBlockErase_uSec;
}

struct IBlockStorageDevice g_SH7264_USB_BL_DeviceTable = 
{                          
    &USB_BS_Driver::CPU_USB_ChipInitialize,
    &USB_BS_Driver::CPU_USB_ChipUnInitialize,
    &USB_BS_Driver::CPU_USB_GetDeviceInfo,
    &USB_BS_Driver::CPU_USB_Read,
    &USB_BS_Driver::CPU_USB_Write,
    &USB_BS_Driver::CPU_USB_Memset,
    &USB_BS_Driver::CPU_USB_GetSectorMetadata,
    &USB_BS_Driver::CPU_USB_SetSectorMetadata,
    &USB_BS_Driver::CPU_USB_IsBlockErased,
    &USB_BS_Driver::CPU_USB_EraseBlock,
    &USB_BS_Driver::CPU_USB_SetPowerState,
    &USB_BS_Driver::CPU_USB_MaxSectorWrite_uSec,
    &USB_BS_Driver::CPU_USB_MaxBlockErase_uSec,
};