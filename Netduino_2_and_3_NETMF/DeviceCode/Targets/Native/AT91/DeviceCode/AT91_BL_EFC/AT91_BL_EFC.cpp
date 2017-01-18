////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "AT91_BL_EFC.h"

#pragma arm section code = "SectionForFlashOperations"
#define PAGES_IN_REGION (AT91C_IFLASH_LOCK_REGION_SIZE / AT91C_IFLASH_PAGE_SIZE)

//--//

BOOL __section(SectionForFlashOperations) SAM7X_BS_Driver::ChipInitialize( void* context )
{   
    return TRUE;
}


BOOL __section(SectionForFlashOperations) SAM7X_BS_Driver::ChipUnInitialize( void* context )
{

    // UnInitialize EFC
    return TRUE;
}


BOOL __section(SectionForFlashOperations) SAM7X_BS_Driver::ChipReadOnly(void * context, BOOL On, UINT32 ProtectionKey )
{
    // Internal setting for WriteProtection
    AT91_BL_EFC &efc0 = AT91_BL_EFC::BL_EFC(0);
    AT91_BL_EFC &efc1 = AT91_BL_EFC::BL_EFC(1);;
    int i;
    unsigned char command; 

    // Set Lock/Unlock command
    command = (On == TRUE) ? AT91_BL_EFC::MC_FCMD_LOCK : AT91_BL_EFC::MC_FCMD_UNLOCK;

    // Set EFC Mode Register - number of cycles during 1 microsecond
    efc0.EFC_FMR = (efc0.EFC_FMR & ~AT91_BL_EFC::MC_FMCN) | ((SYSTEM_CYCLE_CLOCK_HZ / 1000000) << 16);
    efc1.EFC_FMR = (efc1.EFC_FMR & ~AT91_BL_EFC::MC_FMCN) | ((SYSTEM_CYCLE_CLOCK_HZ / 1000000) << 16);

    {
        GLOBAL_LOCK(irq);

        // Lock regions of EFC0 controller (1st FLASH block)
        // Lock regions of EFC1 controller (2nd FLASH block)
        for(i=0; i<16; i++ )
        {
            efc0.EFC_FCR = (AT91_BL_EFC::MC_KEY_VALUE | ((i*PAGES_IN_REGION) << 8) | command);
            efc1.EFC_FCR = (AT91_BL_EFC::MC_KEY_VALUE | ((i*PAGES_IN_REGION) << 8) | command);

            // Wait EFC ready 
            while(  ((efc0.EFC_FSR & AT91_BL_EFC::MC_FRDY) != AT91_BL_EFC::MC_FRDY)  && 
                    ((efc1.EFC_FSR & AT91_BL_EFC::MC_FRDY) != AT91_BL_EFC::MC_FRDY)  );
        }
    }

    return TRUE;
}




const BlockDeviceInfo* __section(SectionForFlashOperations) SAM7X_BS_Driver::GetDeviceInfo( void* context )
{

    BLOCK_CONFIG* config = (BLOCK_CONFIG *)context;
    return config->BlockDeviceInformation;
}

BOOL __section(SectionForFlashOperations) SAM7X_BS_Driver::Read( void* context, ByteAddress Address, UINT32 NumBytes, BYTE * pSectorBuff )
{
    Address = CPU_GetUncachableAddress(Address);
    
    CHIP_WORD* chipAddress = (CHIP_WORD *)Address;
    CHIP_WORD* endAddress  = (CHIP_WORD *)(Address + NumBytes);
    CHIP_WORD *pBuf        = (CHIP_WORD *)pSectorBuff;


    while(chipAddress < endAddress)
    {
        *pBuf++ = *chipAddress++;
    }

    return TRUE;
}

BOOL __section(SectionForFlashOperations) SAM7X_BS_Driver::Write( void* context, ByteAddress address, UINT32 numBytes, BYTE * pSectorBuff, BOOL ReadModifyWrite )
{

    BYTE * pData;
    BYTE * pBuf = NULL;
    MEMORY_MAPPED_NOR_BLOCK_CONFIG* config = (MEMORY_MAPPED_NOR_BLOCK_CONFIG*)context;
    const BlockDeviceInfo * deviceInfo = config->BlockConfig.BlockDeviceInformation;
    
    UINT32 region, range;

    if(ReadModifyWrite) 
    {
        BOOL fRet = TRUE;
        
        if(!deviceInfo->FindRegionFromAddress(address, region, range)) return FALSE;

        UINT32      bytesPerBlock   = deviceInfo->Regions[region].BytesPerBlock;
        UINT32      regionEnd       = deviceInfo->Regions[region].Start + deviceInfo->Regions[region].Size();
        UINT32      offset          = address % bytesPerBlock;
        ByteAddress addr            = address;
        ByteAddress addrEnd         = address + numBytes;
        UINT32      index           = 0;

        pBuf = (BYTE*)private_malloc(bytesPerBlock);

        if(pBuf == NULL)
        {
 
            return FALSE;
        }

        while(fRet && addr < addrEnd)
        {
            ByteAddress sectAddr = (addr - offset);
             
            if(offset == 0 && numBytes >= bytesPerBlock)
            {
                pData = &pSectorBuff[index];
            }
            else
            {
                int bytes = __min(bytesPerBlock - offset, numBytes); 
                
                memcpy( &pBuf[0]     ,    (void*)sectAddr, bytesPerBlock );
                memcpy( &pBuf[offset], &pSectorBuff[index], bytes );

                pData = pBuf;
            }

            if(!EraseBlock( context, sectAddr ))
            {
                fRet = FALSE;
                break;
            }

            fRet = WriteX(context, sectAddr, bytesPerBlock, pData, ReadModifyWrite, TRUE);

            numBytes -= bytesPerBlock - offset;
            addr     += bytesPerBlock - offset;
            index    += bytesPerBlock - offset;
            offset    = 0;

            if(numBytes > 0 && addr >= regionEnd)
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
        return WriteX( context, address, numBytes, pSectorBuff, ReadModifyWrite, TRUE );
    }
}

BOOL __section(SectionForFlashOperations) SAM7X_BS_Driver::WriteX( void* context, ByteAddress address, UINT32 numBytes, BYTE * pSectorBuff, BOOL ReadModifyWrite, BOOL fIncrementDataPtr )
{
    MEMORY_MAPPED_NOR_BLOCK_CONFIG* config = (MEMORY_MAPPED_NOR_BLOCK_CONFIG*)context;
    const BlockDeviceInfo *    deviceInfo = config->BlockConfig.BlockDeviceInformation;

    UINT8  pageBuffer[AT91C_IFLASH_PAGE_SIZE];
    BYTE*  localpData = pSectorBuff;


    UINT32 baseAddress;
    UINT32 endAddress;
    UINT32 pageNumber;
    UINT32 offset;
    UINT32 writeSize;
    UINT32 pageAddress;
    UINT32 iRegion, iRange;
    INT32  NumOfBytes = (INT32)numBytes;
    
    CHIP_WORD *chipAddress;

    if (deviceInfo->Attribute.WriteProtected) return FALSE;

    if (!deviceInfo->FindRegionFromAddress(address, iRegion, iRange)) return FALSE;

    address     = CPU_GetUncachableAddress(address);
    baseAddress = CPU_GetUncachableAddress(deviceInfo->Regions[iRegion].Start);
    endAddress  = (baseAddress + deviceInfo->Regions[iRegion].Size());
    chipAddress = (CHIP_WORD *)address;
    

    if(( address < baseAddress) || (address >= endAddress)) return FALSE;

    pageNumber  = ((address - baseAddress) / AT91C_IFLASH_PAGE_SIZE);
    offset      = ((address - baseAddress) % AT91C_IFLASH_PAGE_SIZE);
    pageAddress = pageNumber * AT91C_IFLASH_PAGE_SIZE + baseAddress;

    ChipReadOnly( context, FALSE, FLASH_PROTECTION_KEY );

    while(NumOfBytes > 0)
    {
        writeSize = __min(AT91C_IFLASH_PAGE_SIZE - offset, NumOfBytes);

        // 1st or 2nd FLASH controller 
        AT91_BL_EFC &efc = AT91_BL_EFC::BL_EFC(( pageNumber < 1024 ) ? 0:1);

        CHIP_WORD * tmpWrData =(CHIP_WORD*) localpData;
        for (UINT32 i=0; i <writeSize; i+=sizeof(CHIP_WORD))
        {
            CHIP_WORD Data;
            Data = *tmpWrData;
            if(0 != (Data & ~(*chipAddress)))
            {
                debug_printf( "Erase failure: 0x%08x=0x%08x, writeTo(0x%08x)\r\n", (size_t)chipAddress, *chipAddress, Data );
                ASSERT(0);
                return FALSE;
            }
            chipAddress ++;
            tmpWrData ++;

        }
        // get the whole page first
        memcpy(pageBuffer, (void *) pageAddress, AT91C_IFLASH_PAGE_SIZE);
        memcpy(pageBuffer + offset, localpData, writeSize);

        // --//
        // Set EFC Mode Register - number of cycles during 1.5 microsecond
        {
            GLOBAL_LOCK(irq);

            efc.EFC_FMR = AT91_BL_EFC::MC_NEBP | (efc.EFC_FMR & ~(AT91_BL_EFC::MC_FMCN)) | (((SYSTEM_CYCLE_CLOCK_HZ / 2000000) * 3) << 16);      

            volatile CHIP_WORD *pageAddr = (CHIP_WORD *)pageAddress;
            CHIP_WORD * pageBuf = (CHIP_WORD*)&pageBuffer[0];
            for (UINT32  i=0; i< AT91C_IFLASH_PAGE_SIZE; i+= sizeof(CHIP_WORD))
            {
                *pageAddr++ = *pageBuf++;
            }
            //memcpy((void *) pageAddress, pageBuffer, AT91C_IFLASH_PAGE_SIZE);

            // perform write command
            efc.EFC_FCR  = ( AT91_BL_EFC::MC_KEY_VALUE  | ((pageNumber % 1024) << 8) | AT91_BL_EFC::MC_FCMD_START_PROG);

                // wait for FLASH ready
            while( (efc.EFC_FSR & AT91_BL_EFC::MC_FRDY) != AT91_BL_EFC::MC_FRDY );

        }
        NumOfBytes -= writeSize;
        if(fIncrementDataPtr) localpData += writeSize;
        pageAddress += AT91C_IFLASH_PAGE_SIZE;
        pageNumber++;
        offset = 0;
        
    }

    ChipReadOnly( context, TRUE, FLASH_PROTECTION_KEY );
    return TRUE;

}

BOOL __section(SectionForFlashOperations) SAM7X_BS_Driver::Memset(void* context, ByteAddress Address, UINT8 Data, UINT32 NumBytes)
{
    CHIP_WORD chipData;

    memset(&chipData, Data, sizeof(CHIP_WORD));
    
    return WriteX(context, Address, NumBytes, (BYTE*)&chipData, TRUE, FALSE);
}

BOOL __section(SectionForFlashOperations) SAM7X_BS_Driver::GetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return FALSE;
}

BOOL __section(SectionForFlashOperations) SAM7X_BS_Driver::SetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return FALSE;
}

BOOL __section(SectionForFlashOperations) SAM7X_BS_Driver::IsBlockErased( void* context, ByteAddress Address, UINT32 BlockLength )
{
    MEMORY_MAPPED_NOR_BLOCK_CONFIG* config = (MEMORY_MAPPED_NOR_BLOCK_CONFIG*)context;
    const BlockDeviceInfo *     deviceInfo = config->BlockConfig.BlockDeviceInformation;

    UINT32 iRegion, iRange;
    CHIP_WORD *chipAddress, *endAddress;

    // 1st or 2nd FLASH controller 
    if (!deviceInfo->FindRegionFromAddress( Address, iRegion, iRange )) return FALSE;

    Address = CPU_GetUncachableAddress(Address);

    chipAddress = (CHIP_WORD *) Address;
    endAddress  = (CHIP_WORD *)(Address + BlockLength);
    
    while(chipAddress < endAddress)
    {
        if(*chipAddress++ != 0xFFFFFFFF)
        {
            return FALSE;
        }
    }

    return TRUE;

}

// erase one  page
BOOL __section(SectionForFlashOperations) SAM7X_BS_Driver::EraseBlock( void* context, ByteAddress address )
{

    MEMORY_MAPPED_NOR_BLOCK_CONFIG* config = (MEMORY_MAPPED_NOR_BLOCK_CONFIG*)context;
    const BlockDeviceInfo *     deviceInfo = config->BlockConfig.BlockDeviceInformation;

    UINT32 iRegion, iRange;
    UINT32 pageNumber=0, baseAddress;
   
    // 1st or 2nd FLASH controller 
    if (!deviceInfo->FindRegionFromAddress( address, iRegion, iRange )) return FALSE;

    if (IsBlockErased( context, address, deviceInfo->Regions[iRegion].BytesPerBlock )) return TRUE;

    address     = CPU_GetUncachableAddress(address);
    baseAddress = CPU_GetCachableAddress(deviceInfo->Regions[iRegion].Start);

    // the pageNumber is the start of page of "Sector", even Sector is not the start sectoraddres of the block
    pageNumber  = ((address - baseAddress) / AT91C_IFLASH_PAGE_SIZE);

    AT91_BL_EFC &efc = AT91_BL_EFC::BL_EFC(( pageNumber < 1024 ) ? 0:1);


    ChipReadOnly( context, FALSE, FLASH_PROTECTION_KEY );
    
    // Set EFC Mode Register - number of cycles during 1.5 microsecond
    efc.EFC_FMR = (efc.EFC_FMR & ~(AT91_BL_EFC::MC_FMCN | AT91_BL_EFC::MC_NEBP)) | (((SYSTEM_CYCLE_CLOCK_HZ / 2000000) * 3) << 16);
    pageNumber = pageNumber % 1024;

    {
        GLOBAL_LOCK(irq);
        // Erase Block (write 0xFFFFFFFF)
        // perform write/erase command
        for (UINT32 i=0; i<AT91C_IFLASH_PAGE_PER_BLOCK; i++)
        {
            efc.EFC_FCR  = (AT91_BL_EFC::MC_KEY_VALUE  | (pageNumber << 8) | AT91_BL_EFC::MC_FCMD_START_PROG);
        
        // wait for FLASH ready
            while( (efc.EFC_FSR & AT91_BL_EFC::MC_FRDY) != AT91_BL_EFC::MC_FRDY );
            pageNumber ++;
        }
    }

    ChipReadOnly( context, TRUE, FLASH_PROTECTION_KEY );

    return TRUE;
}

void  __section(SectionForFlashOperations) SAM7X_BS_Driver::SetPowerState( void* context, UINT32 State )
{

    return ;
}

UINT32 __section(SectionForFlashOperations) SAM7X_BS_Driver::MaxSectorWrite_uSec( void* context )
{
    NATIVE_PROFILE_PAL_FLASH();
    MEMORY_MAPPED_NOR_BLOCK_CONFIG* config = (MEMORY_MAPPED_NOR_BLOCK_CONFIG*)context;

    return config->BlockConfig.BlockDeviceInformation->MaxSectorWrite_uSec;
}


UINT32 __section(SectionForFlashOperations) SAM7X_BS_Driver::MaxBlockErase_uSec( void* context )
{
    NATIVE_PROFILE_PAL_FLASH();
    MEMORY_MAPPED_NOR_BLOCK_CONFIG* config = (MEMORY_MAPPED_NOR_BLOCK_CONFIG*)context;

    return config->BlockConfig.BlockDeviceInformation->MaxBlockErase_uSec;
}

//--//

#pragma arm section code
struct IBlockStorageDevice g_SAM7X_BS_DeviceTable = 
{
    &SAM7X_BS_Driver::ChipInitialize,
    &SAM7X_BS_Driver::ChipUnInitialize,
    &SAM7X_BS_Driver::GetDeviceInfo,
    &SAM7X_BS_Driver::Read,
    &SAM7X_BS_Driver::Write,
    &SAM7X_BS_Driver::Memset,
    &SAM7X_BS_Driver::GetSectorMetadata,
    &SAM7X_BS_Driver::SetSectorMetadata,
    &SAM7X_BS_Driver::IsBlockErased,
    &SAM7X_BS_Driver::EraseBlock,
    &SAM7X_BS_Driver::SetPowerState,
    &SAM7X_BS_Driver::MaxSectorWrite_uSec,
    &SAM7X_BS_Driver::MaxBlockErase_uSec,    
};


