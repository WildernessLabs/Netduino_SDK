////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "..\K9F2G_8.h"

//--//

extern K9F2G_8_BS_Driver g_K9F2G_8_BS_Driver;

//--//

BOOL K9F2G_8_BS_Driver::WaitReady()
{
    UINT32 Status;
    
    // Loop uintl Ready/Busy Pin returns to High state
    while (!CPU_GPIO_GetPinState(AT91_NAND_RB));

    
    do
    {
        WriteCommand(COMMAND_STATUS);

        Status = ReadData8();
    }
    while(((Status & STATUS_READY) != STATUS_READY) && ((Status & STATUS_ERROR) != 0));

    return (Status & STATUS_ERROR) == 0;
}


BOOL K9F2G_8_BS_Driver::ChipInitialize( void* context )
{
    AT91_SMC &smctrl = AT91::SMCTRL();

    AT91_MATRIX &matrix = AT91::MATRIX();


    GLOBAL_LOCK(irq);

    matrix.MATRIX_EBICSA |= AT91_MATRIX::MATRIX_CS3A_SM;

    // Configures the EBI for NandFlash access at 48MHz
    smctrl.SMC_SETUP3 = 0x00010001;
    smctrl.SMC_PULSE3 = 0x03030303;
    smctrl.SMC_CYCLE3 = 0x00050005;
    smctrl.SMC_CTRL3  =  AT91_SMC::SMC_READMODE
                       | AT91_SMC::SMC_WRITEMODE
                       | AT91_SMC::SMC_NWAITM_NWAIT_DISABLE
                       | (0x2 << 16)  | AT91_SMC::SMC_TDFEN
                       | (0x2 << 28) | AT91_SMC::SMC_PMEN
#if USE_16_BIT_TRANSFERS
                       | AT91_SMC::SMC_DBW_WIDTH_SIXTEEN_BITS;
#else
                       | AT91_SMC::SMC_DBW_WIDTH_EIGTH_BITS;
#endif

    CPU_GPIO_DisablePin(AT91_NAND_RE, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1);
    CPU_GPIO_DisablePin(AT91_NAND_WE, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1);
    CPU_GPIO_EnableInputPin(AT91_NAND_RB, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP);
    CPU_GPIO_EnableOutputPin(AT91_NAND_CE, TRUE);

    EnableCE();
#if USE_16_BIT_TRANSFERS
    WriteCommand16(COMMAND_RESET);
#else 
    WriteCommand(COMMAND_RESET);
#endif
    if(!WaitReady())
    {
        DisableCE();
        ASSERT(FALSE);
        return FALSE;
    }
    
    DisableCE();

    
    FLASH_WORD ManufacturerCode = 0;
    FLASH_WORD DeviceCode       = 0;

    ReadProductID(context, ManufacturerCode, DeviceCode );
   
    if (0xEC != ManufacturerCode)
    {
        debug_printf( "Flash_ChipInitialize: ManufacturerCode failure!\r\n" );
        return FALSE;
    }

    if (0xDA != DeviceCode)
    {
        debug_printf( "Flash_ChipInitialize: DeviceCode failure!\r\n" );
        return FALSE;		
    }

    return TRUE;

}

BOOL K9F2G_8_BS_Driver::ChipUnInitialize( void* context )
{
    return TRUE;
}

BOOL K9F2G_8_BS_Driver::ReadProductID( void* context, FLASH_WORD& ManufacturerCode, FLASH_WORD& DeviceCode )
{
    EnableCE();

#if USE_16_BIT_TRANSFERS
    WriteCommand16(COMMAND_READID);
#else
    WriteCommand(COMMAND_READID);
#endif
    WriteAddress(0);

#if USE_16_BIT_TRANSFERS
    ManufacturerCode = (FLASH_WORD)ReadData16();
    DeviceCode       = (FLASH_WORD)ReadData16();
#else
    ManufacturerCode = (FLASH_WORD)ReadData8();
    DeviceCode       = (FLASH_WORD)ReadData8();
#endif
   
    DisableCE();

    return TRUE;
}


const BlockDeviceInfo* K9F2G_8_BS_Driver::GetDeviceInfo( void* context)
{
    BLOCK_CONFIG* config = (BLOCK_CONFIG* )context;
    
    return config->BlockDeviceInformation;    
}

BOOL K9F2G_8_BS_Driver::Read( void* context, ByteAddress byteAddr, UINT32 NumBytes, BYTE * pSectorBuff )
{
    NATIVE_PROFILE_HAL_DRIVERS_FLASH();
    UINT32 RegionIndex, RangeIndex;
    UINT32 BytesToRead;

    BLOCK_CONFIG* pConfig = (BLOCK_CONFIG*)context;

    if (pSectorBuff == NULL) return FALSE;
    
    if (pConfig->BlockDeviceInformation->FindRegionFromAddress( byteAddr, RegionIndex, RangeIndex))
    {
        const BlockRegionInfo* pRegion      = &pConfig->BlockDeviceInformation->Regions[RegionIndex];
        UINT32                 BytesPerPage = pConfig->BlockDeviceInformation->BytesPerSector * c_SectorsPerPage;
        ByteAddress            PageStart    = pConfig->BlockDeviceInformation->PhysicalToSectorAddress( pRegion, byteAddr ) / c_SectorsPerPage;
        INT32                  offset       = ((byteAddr - pRegion->Start) % BytesPerPage); 

#if USE_16_BIT_TRANSFERS
         offset >>= 1; // 16-bit addressing
#endif

        CHIP_WORD *pBuf = (CHIP_WORD *)pSectorBuff;

        ASSERT(PageStart < (pConfig->BlockDeviceInformation->Size / BytesPerPage));

        BytesToRead = (NumBytes + offset > BytesPerPage ? BytesPerPage - offset : NumBytes);

        EnableCE();

        while(NumBytes > 0)
        {
            WriteCommand(COMMAND_READ_1);

            WriteAddress((offset >> 0) & 0xFF);
            WriteAddress((offset >> 8) & 0xFF);
            WriteAddress((PageStart >>  0) & 0xFF);
            WriteAddress((PageStart >>  8) & 0xFF);
            WriteAddress((PageStart >> 16) & 0xFF);

            WriteCommand(COMMAND_READ_2);

            // Loop uintl Ready/Busy Pin returns to High state
            while (!CPU_GPIO_GetPinState(AT91_NAND_RB));

            // Read loop
            for (UINT32 j = 0; j < BytesToRead; j+=sizeof(CHIP_WORD))
            {
#if USE_16_BIT_TRANSFERS
                *pBuf++ = ReadData16();
#else
                *pBuf++ = ReadData8();
#endif
            }

            offset = 0;

            NumBytes -= BytesToRead;

            BytesToRead = __min(NumBytes, BytesPerPage);

            PageStart++;

        }

        DisableCE();

        return TRUE;
    }
    else
    {
        ASSERT(FALSE);
        return FALSE;
    }

}


BOOL K9F2G_8_BS_Driver::Write( void* context, ByteAddress byteAddr, UINT32 NumBytes, BYTE * pSectorBuff, BOOL ReadModifyWrite )
{
    NATIVE_PROFILE_PAL_FLASH();
    return WriteX( context, byteAddr, NumBytes, pSectorBuff, ReadModifyWrite, TRUE );
}

BOOL K9F2G_8_BS_Driver::Memset( void* context, ByteAddress byteAddr, UINT8 Data, UINT32 NumBytes )
{
    NATIVE_PROFILE_PAL_FLASH();

    CHIP_WORD chipData;

    memset(&chipData, Data, sizeof(CHIP_WORD));

    return WriteX( context, byteAddr, NumBytes, (BYTE*)&chipData, TRUE, FALSE );
}

BOOL K9F2G_8_BS_Driver::WriteX( void* context, ByteAddress byteAddr, UINT32 NumBytes, BYTE * pSectorBuff, BOOL ReadModifyWrite, BOOL fIncrementDataPtr )
{
    NATIVE_PROFILE_PAL_FLASH();

    UINT32                 RegionIndex, RangeIndex;
    UINT32                 BytesToWrite;
    const BlockRegionInfo* pRegion;
    CHIP_WORD*             pData;
    BLOCK_CONFIG*          pConfig      = (BLOCK_CONFIG*)context;
    UINT32                 BytesPerPage = pConfig->BlockDeviceInformation->BytesPerSector * c_SectorsPerPage;

    // not supported
    ASSERT(!ReadModifyWrite);

    if (pSectorBuff == NULL) return FALSE;
    
    if (!pConfig->BlockDeviceInformation->FindRegionFromAddress( byteAddr, RegionIndex, RangeIndex))
    {
        return FALSE;
    }

    pRegion = &pConfig->BlockDeviceInformation->Regions[RegionIndex];

    ASSERT(0 == ((byteAddr - pRegion->Start) % BytesPerPage));

    pData = (CHIP_WORD *)pSectorBuff;

    SectorAddress PageStart = pConfig->BlockDeviceInformation->PhysicalToSectorAddress( pRegion, byteAddr ) / c_SectorsPerPage;

    INT32 offset = ((byteAddr - pRegion->Start) % BytesPerPage);

#if USE_16_BIT_TRANSFERS
    offset >>= 1; //16-bit transfers
#endif

    BytesToWrite = (NumBytes + offset > BytesPerPage ? BytesPerPage - offset : NumBytes);

    ASSERT(PageStart < (pConfig->BlockDeviceInformation->Size / BytesPerPage));

    CHIP_WORD* pWrite = (CHIP_WORD*)pData;
    
    EnableCE();

    while(NumBytes > 0)
    {
        BOOL fIncr = fIncrementDataPtr; 
        BOOL fRMW = ReadModifyWrite;

        WriteCommand(COMMAND_WRITE_1);

        WriteAddress((offset >> 0) & 0xFF);
        WriteAddress((offset >> 8) & 0xFF);
        WriteAddress((PageStart >>  0) & 0xFF);
        WriteAddress((PageStart >>  8) & 0xFF);
        WriteAddress((PageStart >> 16) & 0xFF);

        // Write loop
        for (UINT32 j = 0; j < (fRMW ? BytesPerPage : BytesToWrite); j+=sizeof(CHIP_WORD))
        {
#if USE_16_BIT_TRANSFERS
            WriteData16(*pWrite);
#else
            WriteData8(*pWrite);
#endif

            if(fIncr) pWrite+=sizeof(CHIP_WORD);
        }

        WriteCommand(COMMAND_WRITE_2);

        if(!WaitReady())
        {
            DisableCE();
            ASSERT(FALSE);
            return FALSE;
        }

        NumBytes -= BytesToWrite;

        BytesToWrite = __min(NumBytes, BytesPerPage);

        PageStart++;
    }

    DisableCE();

    return TRUE;

}


BOOL K9F2G_8_BS_Driver::GetSectorMetadata(void* context, ByteAddress byteAddress, SectorMetadata* pSectorMetadata)
{
    if (pSectorMetadata == NULL) return FALSE;

    BLOCK_CONFIG* pConfig = (BLOCK_CONFIG*)context;
    UINT32 BytesPerPage   = pConfig->BlockDeviceInformation->BytesPerSector * c_SectorsPerPage;
    UINT32 offset         = BytesPerPage;
    UINT32 BytesToRead    = sizeof(SectorMetadata);
    CHIP_WORD* pData      = (CHIP_WORD*)pSectorMetadata;
    UINT32 RegionIndex, RangeIndex;
    const BlockRegionInfo* pRegion;
    SectorAddress PageStart;

    ASSERT(BytesToRead == c_SizeOfSectorMetadata);

    if (!pConfig->BlockDeviceInformation->FindRegionFromAddress( byteAddress, RegionIndex, RangeIndex))
    {
        return FALSE;
    }

    pRegion = &pConfig->BlockDeviceInformation->Regions[RegionIndex];

    PageStart = pConfig->BlockDeviceInformation->PhysicalToSectorAddress( pRegion, byteAddress );

    ASSERT(0 == ((byteAddress - pRegion->Start) % BytesPerPage));

    offset += c_SizeOfSectorMetadata * (PageStart % c_SectorsPerPage);

#if USE_16_BIT_TRANSFERS
    offset >>= 1; // 16 bit addressing
#endif

    PageStart /= c_SectorsPerPage;

    ASSERT(PageStart < (pConfig->BlockDeviceInformation->Size / BytesPerPage));
    
    EnableCE();

    WriteCommand(COMMAND_READ_1);

    WriteAddress((offset >> 0) & 0xFF);
    WriteAddress((offset >> 8) & 0xFF);
    WriteAddress((PageStart >>  0) & 0xFF);
    WriteAddress((PageStart >>  8) & 0xFF);
    WriteAddress((PageStart >> 16) & 0xFF);

    WriteCommand(COMMAND_READ_2);

    // Loop uintl Ready/Busy Pin returns to High state
    while (!CPU_GPIO_GetPinState(AT91_NAND_RB));

    // Read loop
    for(; BytesToRead > 0; BytesToRead -= sizeof(CHIP_WORD))
    {
#if USE_16_BIT_TRANSFERS
        *pData++ = ReadData16();
#else
        *pData++ = ReadData8();
#endif
    }

    DisableCE();

    return TRUE;
}

BOOL K9F2G_8_BS_Driver::SetSectorMetadata(void* context, ByteAddress byteAddress, SectorMetadata* pSectorMetadata)
{
    BLOCK_CONFIG* pConfig = (BLOCK_CONFIG*)context;
    UINT32 BytesPerPage   = pConfig->BlockDeviceInformation->BytesPerSector * c_SectorsPerPage;
    UINT32 offset         = BytesPerPage;
    UINT32 BytesToWrite   = sizeof(SectorMetadata);
    CHIP_WORD* pData      = (CHIP_WORD*)pSectorMetadata;
    UINT32 RegionIndex, RangeIndex;
    const BlockRegionInfo* pRegion;
    SectorAddress PageStart;

    ASSERT(BytesToWrite == c_SizeOfSectorMetadata);

    if (!pConfig->BlockDeviceInformation->FindRegionFromAddress( byteAddress, RegionIndex, RangeIndex))
    {
        return FALSE;
    }

    pRegion = &pConfig->BlockDeviceInformation->Regions[RegionIndex];

    PageStart = pConfig->BlockDeviceInformation->PhysicalToSectorAddress( pRegion, byteAddress );

    ASSERT(0 == ((byteAddress - pRegion->Start) % BytesPerPage));

    offset += c_SizeOfSectorMetadata * (PageStart % c_SectorsPerPage);

#if USE_16_BIT_TRANSFERS
    offset >>= 1; // 16-bit addressing
#endif

    PageStart /= c_SectorsPerPage;

    ASSERT(PageStart < (pConfig->BlockDeviceInformation->Size / BytesPerPage));
    
    EnableCE();

    WriteCommand(COMMAND_WRITE_1);

    WriteAddress((offset >> 0) & 0xFF);
    WriteAddress((offset >> 8) & 0xFF);
    WriteAddress((PageStart >>  0) & 0xFF);
    WriteAddress((PageStart >>  8) & 0xFF);
    WriteAddress((PageStart >> 16) & 0xFF);

    // Write loop
    for(; BytesToWrite > 0; BytesToWrite -= sizeof(CHIP_WORD))
    {
#if USE_16_BIT_TRANSFERS
        WriteData16(*pData++);
#else
        WriteData8(*pData++);
#endif
    }

    WriteCommand(COMMAND_WRITE_2);

    if(!WaitReady())
    {
        DisableCE();
        ASSERT(FALSE);
        return FALSE;
    }

    DisableCE();

    return TRUE;
}


BOOL K9F2G_8_BS_Driver::IsBlockErased( void* context, ByteAddress BlockStartAddress, UINT32 BlockLength  )
{

    NATIVE_PROFILE_HAL_DRIVERS_FLASH();

    UINT32 RegionIndex, RangeIndex;

    BLOCK_CONFIG* pConfig = (BLOCK_CONFIG*)context;

    if (!pConfig->BlockDeviceInformation->FindRegionFromAddress( BlockStartAddress, RegionIndex, RangeIndex))
    {
        return FALSE;
    }

    const BlockRegionInfo* pRegion = &pConfig->BlockDeviceInformation->Regions[RegionIndex];

    ByteAddress   PageStart     = pConfig->BlockDeviceInformation->PhysicalToSectorAddress( pRegion, BlockStartAddress ) / c_SectorsPerPage;
    UINT32        BytesPerPage  = pConfig->BlockDeviceInformation->BytesPerSector * c_SectorsPerPage;
    UINT32        PagesPerBlock = pRegion->BytesPerBlock / BytesPerPage;
    BOOL          fErased       = TRUE;

    ASSERT(0 == ((BlockStartAddress - pRegion->Start) % pRegion->BytesPerBlock));

    EnableCE();

    for(UINT32 i=0; i<PagesPerBlock && fErased; i++)
    {
        WriteCommand(COMMAND_READ_1);

        WriteAddress(0);
        WriteAddress(0);
        WriteAddress((PageStart >>  0) & 0xFF);
        WriteAddress((PageStart >>  8) & 0xFF);
        WriteAddress((PageStart >> 16) & 0xFF);

        WriteCommand(COMMAND_READ_2);

        // Loop uintl Ready/Busy Pin returns to High state
        while (!CPU_GPIO_GetPinState(AT91_NAND_RB));
        
        // Read loop - 64 bytes for sector metadata
        for (UINT32 j = 0; j < BytesPerPage + (c_SizeOfSectorMetadata*c_SectorsPerPage); j+=sizeof(CHIP_WORD))
        {
#if USE_16_BIT_TRANSFERS
            if(ReadData16() != 0xFFFF)
#else
            if(ReadData8() != 0xFF)
#endif
            {
                fErased = FALSE;
                break;
            }
        }

        PageStart++;

    }

    DisableCE();

    return fErased;

}

BOOL K9F2G_8_BS_Driver::EraseBlock( void* context, ByteAddress byteAddress )
{
    NATIVE_PROFILE_HAL_DRIVERS_FLASH();

    UINT32 RegionIndex, RangeIndex;

    BLOCK_CONFIG* pConfig = (BLOCK_CONFIG*)context;

    if (!pConfig->BlockDeviceInformation->FindRegionFromAddress( byteAddress, RegionIndex, RangeIndex))
    {
        return FALSE;
    }

    const BlockRegionInfo* pRegion       = &pConfig->BlockDeviceInformation->Regions[RegionIndex];
    UINT32                 PagesPerBlock = pRegion->BytesPerBlock / (c_SectorsPerPage * pConfig->BlockDeviceInformation->BytesPerSector);
    UINT32                 PageStart     = pConfig->BlockDeviceInformation->PhysicalToSectorAddress( pRegion, byteAddress ) / c_SectorsPerPage;
    SectorAddress          BlockStart    = PageStart - (PageStart % PagesPerBlock);

    
    ASSERT(0 == ((byteAddress - pRegion->Start) % pRegion->BytesPerBlock));

    EnableCE();

    WriteCommand(COMMAND_ERASE_1);

    WriteAddress((BlockStart >>  0) & 0xFF);
    WriteAddress((BlockStart >>  8) & 0xFF);
    WriteAddress((BlockStart >> 16) & 0xFF);

    WriteCommand(COMMAND_ERASE_2);

    if(!WaitReady())
    {
        DisableCE();
        ASSERT(FALSE);
        return FALSE;
    }

    DisableCE();

    return TRUE;

}

void K9F2G_8_BS_Driver::SetPowerState( void* context, UINT32 State )
{
    // our flash driver is always Power ON
    return ;
}


UINT32 K9F2G_8_BS_Driver::MaxSectorWrite_uSec( void* context )
{
    NATIVE_PROFILE_PAL_FLASH();

    BLOCK_CONFIG* config = (BLOCK_CONFIG* )context;
    
    return config->BlockDeviceInformation->MaxSectorWrite_uSec;
}

UINT32 K9F2G_8_BS_Driver::MaxBlockErase_uSec( void* context )
{
    NATIVE_PROFILE_PAL_FLASH();
    
    BLOCK_CONFIG* config = (BLOCK_CONFIG* )context;

    return config->BlockDeviceInformation->MaxBlockErase_uSec;

}

struct IBlockStorageDevice g_K9F2G08_BS_DeviceTable = 
{                          
    &K9F2G_8_BS_Driver::ChipInitialize,
    &K9F2G_8_BS_Driver::ChipUnInitialize,
    &K9F2G_8_BS_Driver::GetDeviceInfo,
    &K9F2G_8_BS_Driver::Read,
    &K9F2G_8_BS_Driver::Write,
    &K9F2G_8_BS_Driver::Memset,
    &K9F2G_8_BS_Driver::GetSectorMetadata,
    &K9F2G_8_BS_Driver::SetSectorMetadata,
    &K9F2G_8_BS_Driver::IsBlockErased,
    &K9F2G_8_BS_Driver::EraseBlock,
    &K9F2G_8_BS_Driver::SetPowerState,
    &K9F2G_8_BS_Driver::MaxSectorWrite_uSec,
    &K9F2G_8_BS_Driver::MaxBlockErase_uSec,    
};

