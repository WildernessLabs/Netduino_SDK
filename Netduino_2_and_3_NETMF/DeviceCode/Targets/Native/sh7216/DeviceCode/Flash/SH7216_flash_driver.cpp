////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <machine.h>
#include <smachine.h>
#include "../include/iodefine.h"
#include "SH7216_flash.h"


#ifdef FLASH_DRIVER_TESTING
//#define FLASH_DELAYS()    HAL_Time_Sleep_MicroSeconds(10); Events_WaitForEventsInternal(0,1)
#define FLASH_DELAYS()  HAL_Time_Sleep_MicroSeconds_InterruptEnabled(1)
#else
#define FLASH_DELAYS()    
#endif

static int imask_value =0;



//--//
BOOL  SH7216_INTERNAL_FLASH_Driver::ChipInitialize( void* context )
{
    unsigned long int   * src;
    unsigned long int   * dst;
    unsigned short int  i;

    NATIVE_PROFILE_HAL_DRIVERS_FLASH();

    MEMORY_MAPPED_NOR_BLOCK_CONFIG* config = (MEMORY_MAPPED_NOR_BLOCK_CONFIG*)context;
    
    // first setup the memory for wait states, read only, etc.
    CPU_EBIU_ConfigMemoryBlock( config->Memory );

    ChipReadOnly( config, FALSE, FLASH_PROTECTION_KEY );
    
    /* Flash Pin Monitor Register */
    /* Check to see if the FWE pin is set correctly */
    if((FPMON & 0x80) != 0x80)
    {
        /* Cannot perform Flash Erase/Programming while FWE is low */
        return(1);
    }
 
    /* Disable Standby for flash sequencer peripheral (enable clocking of this peripheral) */
    STBCR3 = STBCR3 & ~0x01;        // Clear bit 0
    
    /* Flash access error interrupt enable register */
    /* Disable output of flash interface error (FIFE) interrupt requests. */
    /* (FLD) */
    FAEINT = 0x00;

    /*----------------------------------*/
    /* Transfer Firmware to the FCU RAM */
    /*----------------------------------*/
    /* To use FCU commands, the FCU firmware must be stored in the FCU RAM. */
    /* Before writing data to the FCU RAM, clear FENTRYR to H'0000 to stop the FCU. */
    if(FENTRYR != 0x0000)
    {
        /* Disable the FCU form accepting commands */
        /* Clear both the FENTRY0(ROM) and FENTRYD(FLD) bits */
        FENTRYR = 0xAA00;                   // Clear FENTRY0 & FENTRYD to 0
        FENTRYR;    // Dummy Read is needed to enter Read Mode
        nop();      // To keep compiler optimization form re-ordering
    }
    
    /* Specify FCU RAM access enabled state */
    FCURAME = 0xC401;

#if 0
    /* Setup DMAC to copy FCU firmware to the FCU RAM. */
    STBCR2 = STBCR2 & ~0x20;            // (bit 5) Disable DMAC Standby bit (ie, Enable it)
    DMAC.DMAOR.BIT.DME = 0x1;           
    DMAC0.SAR = (void *)0x00402000;     
    DMAC0.DAR = (void *)0x80ff8000;     
    DMAC0.DMATCR = 0x200;               
    DMAC0.CHCR.LONG = 0x80005438;
    /* ---- Start the DMAC ---- */
    DMAC0.CHCR.BIT.DE = 0x1;
    while(DMAC0.CHCR.BIT.TE == 0x0)
    {
        /* ---- wait ---- */
    }
#else

    // Copies the FCU firmware to the FCU RAM.
    // Source: H'00402000 to H'00403FFF (FCU firmware area)
    // Destination: H'80FF8000 to H'80FF9FFF (FCU RAM area)
    src = (unsigned long int *)0x00402000;
    dst = (unsigned long int *)0x80FF8000;
    for( i=0; i<(0x2000/4); i++)
    {
        *dst = *src;
        src++;
        dst++;
    }
#endif

    Action_FLDAreaAccess(0xF, 0xF);

    ChipReadOnly(config, TRUE, FLASH_PROTECTION_KEY );
    // the flash are now write protected

    return TRUE;
}

void SH7216_INTERNAL_FLASH_Driver::Action_FLDAreaAccess(UINT8 read_mask, UINT8 write_mask)
{
    /* Set Read access for the Data Flash blocks */
    EEPRE0 = 0x2D00 | (unsigned short)read_mask;

    /* Set Erase/Program access for the Data Flash blocks */
    EEPWE0 = 0x1E00 | write_mask;

}

BOOL  SH7216_INTERNAL_FLASH_Driver::ChipUnInitialize( void* context )
{
    return TRUE;
}


const BlockDeviceInfo*  SH7216_INTERNAL_FLASH_Driver::GetDeviceInfo( void* context )
{
    return ((MEMORY_MAPPED_NOR_BLOCK_CONFIG*)context)->BlockConfig.BlockDeviceInformation;
}


BOOL  SH7216_INTERNAL_FLASH_Driver::ChipReadOnly(void* context, BOOL On, UINT32 ProtectionKey )
{
    NATIVE_PROFILE_HAL_DRIVERS_FLASH();
    
    MEMORY_MAPPED_NOR_BLOCK_CONFIG* config = (MEMORY_MAPPED_NOR_BLOCK_CONFIG*)context;

    if(ProtectionKey != FLASH_PROTECTION_KEY) { ASSERT(0); return FALSE; }

    config->ChipProtection = On ? 0 : ProtectionKey;

    CPU_EBIU_Memory_ReadOnly( config->Memory, On );

    if(config->BlockConfig.WriteProtectionPin.Pin != GPIO_PIN_NONE)
    {
        CPU_GPIO_SetPinState( config->BlockConfig.WriteProtectionPin.Pin, On ? config->BlockConfig.WriteProtectionPin.ActiveState: !config->BlockConfig.WriteProtectionPin.ActiveState );

    }

    return TRUE;
}

BOOL  SH7216_INTERNAL_FLASH_Driver::ReadProductID(void* context, FLASH_WORD& ManufacturerCode, FLASH_WORD& DeviceCode )
{
    NATIVE_PROFILE_HAL_DRIVERS_FLASH();
    
    ChipReadOnly( context, FALSE, FLASH_PROTECTION_KEY );

    Action_ReadID( context, ManufacturerCode, DeviceCode );

    ChipReadOnly( context, TRUE, FLASH_PROTECTION_KEY );

    return TRUE;
}


BOOL  SH7216_INTERNAL_FLASH_Driver::IsBlockErased( void* context, ByteAddress address, UINT32 blockLength )
{
    NATIVE_PROFILE_HAL_DRIVERS_FLASH();

    address = CPU_GetUncachableAddress(address);

    volatile CHIP_WORD * ChipAddress = (volatile CHIP_WORD *) address;

    CHIP_WORD * EndAddress = (CHIP_WORD*)(address + blockLength);
    
    while(ChipAddress < EndAddress)
    {
        if( (*ChipAddress ) != (CHIP_WORD)0xFFFFFFFF)
        {
            return FALSE;   
        }
        ChipAddress ++;
    }
    return TRUE;
}

BOOL  SH7216_INTERNAL_FLASH_Driver::EraseBlock( void* context, ByteAddress address )
{
    NATIVE_PROFILE_HAL_DRIVERS_FLASH();

    BOOL result;
    CHIP_WORD  ChipAddress;
    
    UINT32 iRegion, iRange;

    MEMORY_MAPPED_NOR_BLOCK_CONFIG* config = (MEMORY_MAPPED_NOR_BLOCK_CONFIG*)context;
    const BlockDeviceInfo *    deviceInfo = config->BlockConfig.BlockDeviceInformation;

    if (deviceInfo->Attribute.WriteProtected) return FALSE;
    
    if (!deviceInfo->FindRegionFromAddress(address, iRegion, iRange)) return FALSE;

    address -= (address % deviceInfo->Regions[iRegion].BytesPerBlock);

    ChipAddress = (CHIP_WORD ) address;
   
    ChipReadOnly(context, FALSE, FLASH_PROTECTION_KEY);

    FLASH_BEGIN_PROGRAMMING_FAST();
    
    result =  Action_EraseSector( context, ChipAddress );

    FLASH_END_PROGRAMMING_FAST( "SH7216 INTERNAL FLASH EraseSector", ChipAddress);       


    ChipReadOnly(context, TRUE, FLASH_PROTECTION_KEY);

    return result;
}


BOOL  SH7216_INTERNAL_FLASH_Driver::Read(void* context, ByteAddress address, UINT32 numBytes, BYTE * pSectorBuff)
{
    NATIVE_PROFILE_HAL_DRIVERS_FLASH();

    CHIP_WORD* ChipAddress, *EndAddress;


#if defined(_DEBUG)    
    UINT32 iRegion, iRange;

    MEMORY_MAPPED_NOR_BLOCK_CONFIG* config = (MEMORY_MAPPED_NOR_BLOCK_CONFIG*)context;
    const BlockDeviceInfo *    deviceInfo = config->BlockConfig.BlockDeviceInformation;
    if(!deviceInfo->FindRegionFromAddress(address, iRegion, iRange)) return FALSE;
#endif

    address = CPU_GetUncachableAddress(address);

    ChipAddress = (CHIP_WORD *) address;
    EndAddress  = (CHIP_WORD *)(address + numBytes);

    CHIP_WORD *pBuf = (CHIP_WORD *)pSectorBuff;

    while(ChipAddress < EndAddress)
    {
        *pBuf++ = *ChipAddress++;
    }

    return TRUE;
}

BOOL  SH7216_INTERNAL_FLASH_Driver::Write(void* context, ByteAddress address, UINT32 numBytes,BYTE * pSectorBuff, BOOL ReadModifyWrite)
{
    BOOL result = TRUE;
    NATIVE_PROFILE_PAL_FLASH();
    FLASH_BEGIN_PROGRAMMING_FAST();
    result = WriteX(context, address, numBytes, pSectorBuff, ReadModifyWrite, TRUE);
    FLASH_END_PROGRAMMING_FAST( "SH7216 INTERNAL FLASH WriteWord", address );    

    return result;
}

BOOL  SH7216_INTERNAL_FLASH_Driver::Memset(void* context, ByteAddress address, UINT8 data, UINT32 numBytes)
{
   BOOL result = TRUE;
   NATIVE_PROFILE_PAL_FLASH();

    CHIP_WORD chipData;

    memset(&chipData, data, sizeof(CHIP_WORD));

    FLASH_BEGIN_PROGRAMMING_FAST();
    result = WriteX(context, address, numBytes, (BYTE*)&chipData, TRUE, FALSE);
    FLASH_END_PROGRAMMING_FAST( "SH7216 INTERNAL FLASH WriteWord", address );    
    
    return result;
}


BOOL  SH7216_INTERNAL_FLASH_Driver::GetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return TRUE;
}

BOOL  SH7216_INTERNAL_FLASH_Driver::SetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return TRUE;
}


void   SH7216_INTERNAL_FLASH_Driver::SetPowerState(void* context, UINT32 State )
{
    // our flash driver is always ON
    return ;
}


UINT32  SH7216_INTERNAL_FLASH_Driver::MaxSectorWrite_uSec( void* context )
{
    NATIVE_PROFILE_PAL_FLASH();

    MEMORY_MAPPED_NOR_BLOCK_CONFIG* config = (MEMORY_MAPPED_NOR_BLOCK_CONFIG*)context;
    
    return config->BlockConfig.BlockDeviceInformation->MaxSectorWrite_uSec;
}


UINT32  SH7216_INTERNAL_FLASH_Driver::MaxBlockErase_uSec( void* context )
{
    NATIVE_PROFILE_PAL_FLASH();

    MEMORY_MAPPED_NOR_BLOCK_CONFIG* config = (MEMORY_MAPPED_NOR_BLOCK_CONFIG*)context;

    return config->BlockConfig.BlockDeviceInformation->MaxBlockErase_uSec;
}

//--//


void  SH7216_INTERNAL_FLASH_Driver::Action_ReadID( void* context, FLASH_WORD& ManufacturerCode, FLASH_WORD& DeviceCode )
{
    NATIVE_PROFILE_HAL_DRIVERS_FLASH();
    

    ///////////////////////////////////
    //
    //
    FLASH_BEGIN_PROGRAMMING_FAST();
 
    ManufacturerCode = 0x0001;
    DeviceCode       = 0x227E;  
 
    
    FLASH_END_PROGRAMMING_FAST( "SH7216 INTERNAL FLASH ReadProductID", BaseAddress );
    //
    //
    ///////////////////////////////////
}


#pragma section FRAM

BOOL SH7216_INTERNAL_FLASH_Driver::Action_EnterPEMode(volatile CHIP_WORD BaseAddress)
{
    volatile unsigned char *flash_addr;
    BOOL result = TRUE;
    
    /* FENTRYR must be 0x0000 before bit FENTRY0 or FENTRYD can be set to 1 */
    if( FENTRYR != 0x0000)
    {
        FENTRYR = 0xAA00;
        FENTRYR;    // Dummy Read is needed to enter Read Mode
        nop();      // To keep compiler optimization form re-ordering
    }

    if( BaseAddress <= (CHIP_WORD)0x000FFFFF )
    {
        /* ROM */
        /* Disable Interrupts because the interrupt vector table will not be accessible */
        imask_value = get_imask();
        set_imask(0xF);

        FENTRYR = 0xAA01;   // Set FENTRY0 bit(Bit 0) and FKEY (B8-15 = 0xAA)
        flash_addr = (unsigned char *)ROM_PE_ADDR;
    }
    else 
    {
        /* FLD */
        FENTRYR = 0xAA80;   // Set FENTRYD bit(Bit 7) and FKEY (B8-15 = 0xAA)
        flash_addr = (unsigned char *)FLD_ADDRESS;

        // First clear the status of the FCU before doing FLD programming.
        // This is to clear out any previous errors that may have occured.
        // For example, if you attempt to read the Data Flash area 
        // before you make it readable using FlashFLDAreaAccess().
        /* Check if ILGERR (bit 6) is set */
        if( FSTATR0 & 0x40 )
        {
            if(FASTAT != 0x10)
            {
                FASTAT = 0x10;
            }
        }
        *flash_addr = 0x50;
    }

    /* Send the 'Normal mode transition' (0xFF) command 
       by writing to address 0x80800000 (or 0x80100000) */
    *flash_addr = 0xFF;

    /* Check 'ILGLERR' (bit 6) in 'Flash Status Register 0' (FSTATR0) */
    if( FSTATR0 & 0x40 )
    {
        /* Restore the imask value (ROM PE Mode only) */
        if( BaseAddress <= (CHIP_WORD )0x000FFFFF )
        {
            set_imask(imask_value); // Restore the previous imask
        }

        FENTRYR = 0xAA00;
        FENTRYR;    // Dummy Read is needed to enter Read Mode
        nop();      // To keep compiler optimization form re-ordering
        result = FALSE; // ERROR
    }

    return result;

}


void SH7216_INTERNAL_FLASH_Driver::Action_ExitPEMode(volatile CHIP_WORD BaseAddress)
{
    volatile unsigned char *flash_addr;

    // Go back to normal 'ROM/FLD' Read mode

    /* Check the FRDY bit in FSTATR0 */
    if( FSTATR0 & 0x80 )    // FRDY = 1?
    {
        /* Check if any errors exist */
        /* Check bits ILGLERR, PRGERR, and ERSERR (bits 6,5,4) in FSTATR0 */
        if( (FSTATR0 & 0xE0) == 0x00 )
        {
            // All good. Nothing left to check
        }
        else
        {
            
            if( (FSTATR0 & 0x40) )  // ILGLERR == 1?
            {
                if(FASTAT != 0x10)
                {
                       FASTAT = 0x10;
                }
            }
             
            /* Issue a status register clear command to clear all error bits */
            if( BaseAddress <= (CHIP_WORD )0x000FFFFF)
            {
                /* ROM */
                flash_addr = (unsigned char *)ROM_PE_ADDR;
                *flash_addr = 0x50;
            }
            else // ( current_mode == FLD_PE_MODE )
            {
                /* FLD */
                flash_addr = (unsigned char *)FLD_ADDRESS;
                *flash_addr = 0x50;
            }
        }
    }
    else
    {
        //  Wait for FRDY to be set?
    }

    /* Leave Program/Erase Mode */
    FENTRYR = 0xAA00;                   // Clear FENTRY0 & FENTRYD to 0

    /* When returning from Program/Erase Mode to Rom/FLD Read mode, you must do a dummy
       read of the 'FENTRYR' register after writing to it in order to guarantee the value
       gets transferred from the register buffer to the actual FCU internal register.
       Because 'FENTRYR' was defined as 'volatile', it will not get optimized out.*/
    FENTRYR;    // Dummy read after value written
    nop();      // This NOP is inserted in order to keep the SH compiler from re-ordering the 'FENTRYR;' instruction

    /* Restore the imask value (ROM PE Mode only) */
    if(BaseAddress <= (CHIP_WORD )0x000FFFFF)
    {
        set_imask(imask_value); // Restore the previous imask
    }

}

BOOL  SH7216_INTERNAL_FLASH_Driver::Action_WriteFlash( void* context, volatile CHIP_WORD Address, CHIP_WORD Data, UINT16 numBytesWrite)
{
    UINT8 n = 0;
    BOOL result = TRUE;
    
    if(Address < (CHIP_WORD)ROM_PE_ADDR)
    {
        numBytesWrite = numBytesWrite >> 1;
        *(volatile unsigned char *)FLD_ADDRESS = 0xE8;
        *(volatile unsigned char *)FLD_ADDRESS = (unsigned char)numBytesWrite;  //0x40 for 128 bytes or 0x04 for 8 bytes
        while(n++ < numBytesWrite)
        {
            *(volatile unsigned short *)Address = *(unsigned short *)Data;
            //Address += 2;
            Data += 2;
        }

        /* Write the final FCU command for programming */
        *(volatile unsigned char *)(FLD_ADDRESS) = 0xD0;

        
    }
    else
    {
        /* Write the FCU Program command */
        *(volatile unsigned char *)Address = 0xE8;  
        *(volatile unsigned char *)Address = 0x80;

        /* Write 256 bytes into flash, 16-bits at a time */
        for(n = 0; n < 128; n++)
        {
            *(volatile unsigned short *)Address = *(unsigned short *)Data;
            Data += 2;
        }

        /* Write the final FCU command for programming */
        *(volatile unsigned char *)Address = 0xD0;

    }

    /* Wait until the operation is complete */
    // The operations is complete with the FRDY (bit 7) in register
    // FSTATR goes back high.
    while( (FSTATR0 & 0x80) == 0x00 )
    {
        /* Waiting for operation to complete */
    }

    // Check if the program operation was successful by checking 
    // bit 'PRGERR' (bit4) and 'ILGLERR' (bit 6) of register 'FSTATR0'
    if((FSTATR0 & 0x50) != 0x00)
    {
        /* Did not program correctly */
        result = FALSE;
    }

    return result;

}

BOOL  SH7216_INTERNAL_FLASH_Driver::WriteX(void* context, ByteAddress address, UINT32 numBytes, BYTE * pSectorBuff, BOOL ReadModifyWrite, BOOL fIncrementDataPtr)
{

    MEMORY_MAPPED_NOR_BLOCK_CONFIG* config = (MEMORY_MAPPED_NOR_BLOCK_CONFIG*)context;
    const BlockDeviceInfo *    deviceInfo = config->BlockConfig.BlockDeviceInformation;
    BYTE  DataBuff[300];
    UINT16 numBytesWrite, i, j=0;
    CHIP_WORD ChipAddress, PEAddress;
    CHIP_WORD StartAddress;
    CHIP_WORD EndAddress, pData;
    BOOL result = TRUE;

    if (deviceInfo->Attribute.WriteProtected) return FALSE;

    /* Hack to avoid reprogramming the same area initially by TinyCLR for BLOCKTYPE_STORAGE_A */
    /* keep this address as start of BLOCKTYPE_STORAGE_A */
    if(address == 0x80104000 && numBytes == 0xc)
    {
        numBytes = 8;
    }

    address = CPU_GetUncachableAddress(address);
    StartAddress = (CHIP_WORD)address;
    ChipAddress = StartAddress;
    EndAddress  = (CHIP_WORD )(address + numBytes); 
    pData       = (CHIP_WORD )DataBuff;
    /* ROM area or FLD area */
    if(StartAddress >= (CHIP_WORD)FLD_ADDRESS)
    {
        /* Check for an address on a 8 byte page boundary */
        if((unsigned short)StartAddress & (unsigned short)0x07)
        {
            ChipAddress = StartAddress & 0xFFFFFFF8;
        }
           
    }
    else
    {
        /* Check for an address on a 256 byte page */
        if( (unsigned short)ChipAddress & (unsigned short)0xFF)
        {
            ChipAddress = StartAddress & 0xFFFFFF00;
        }
    }

    PEAddress = ChipAddress;
    
    if(ChipAddress <= (CHIP_WORD)0x000FFFFF)
    {
        /* Transpose the flash address to the flash peripheral address */
        ChipAddress = ChipAddress + ROM_PE_ADDR;
        StartAddress = StartAddress + ROM_PE_ADDR;
        EndAddress  = (CHIP_WORD )(StartAddress + numBytes); 
    }

    ChipReadOnly(config, FALSE, FLASH_PROTECTION_KEY);

    if(ChipAddress >= (CHIP_WORD)ROM_PE_ADDR)
    {
        numBytesWrite = 256;
    }
    else
    {
        if( (numBytes >= 128) && (((unsigned char)ChipAddress & 0x7F) == 0x00))
        {
            numBytesWrite = 128;
        }
        else
        {
            numBytesWrite = 8;
        }
    }

    // Can write either 8 or 128 bytes at a time (128 bytes is faster though)
    // But, we can only write 128 bytes if we are on a 128 byte boundary.
    while(ChipAddress < EndAddress)
    {
        // Aligning the data
        for(i = 0; i < numBytesWrite; i++)
        {
            if((ChipAddress+i) >= StartAddress && (ChipAddress+i) < EndAddress)
            {
                DataBuff[i] = pSectorBuff[j];
                j++;
            }
            else
            {
                DataBuff[i] = *(BYTE *)(PEAddress +i);
            }

        }
        
        /* Enter Program Erase Normal Mode for ROM Operations */
        if(Action_EnterPEMode(PEAddress) != TRUE)
        {
            return FALSE;
        }

        /**********************************/
        /* Cancel the ROM Protect feature */
        /* NOTE: If you are using block locking feature to protect your 
        data, remove the following line */
        FPROTR = 0x5501;

        /* Call the Programming function */
        result = Action_WriteFlash(context, ChipAddress, pData, numBytesWrite);
            
        Action_ExitPEMode(PEAddress);

        if(result != TRUE)
        {  /* Stop Programming */
            break;
        }
            
        // Now do the next set of numBytesWrite
        ChipAddress += numBytesWrite;
        PEAddress += numBytesWrite;
    }
    ChipReadOnly(config, TRUE, FLASH_PROTECTION_KEY);

    return result;
}


BOOL  SH7216_INTERNAL_FLASH_Driver::Action_EraseSector( void* context, volatile CHIP_WORD BaseAddress )
{

    volatile unsigned char *erase_addr;
    
    BOOL success = TRUE;
    
   /* Enter Program Erase Normal Mode for ROM/FLD Operations */
    if(Action_EnterPEMode(BaseAddress) != TRUE)
    {
        return FALSE;
    }

    /**********************************/
    /* Cancel the ROM Protect feature */
    /* NOTE: If you are using block locking feature to protect your 
       data, remove the following line */
    FPROTR = 0x5501;

    /* Send the FCU Command */
    if(BaseAddress <= (CHIP_WORD)0x000FFFFF)
    {
        erase_addr = (unsigned char *)(BaseAddress | ROM_PE_ADDR);
    }
    else
    {
        erase_addr = (unsigned char *)BaseAddress;
    }

    *erase_addr = 0x20;
    *erase_addr = 0xD0;

    /* Wait until the operation is complete */
    // The operations is complete with the FRDY (bit 7) in register
    // FSTATR goes back high.
    while( (FSTATR0 & 0x80) == 0x00 )
    {
        /* Waiting for operation to complete */
    }

    // Check if erase operation was successful by checking 
    // bit 'ERSERR' (bit5) and 'ILGLERR' (bit 6) of register 'FSTATR0'
    if((FSTATR0 & 0x60) != 0x00)
    {
        success = FALSE;
    }
 
    /* Leave Program/Erase Mode */
    Action_ExitPEMode(BaseAddress);
    
    return success;        
}

#pragma section /* RAM SECTION ENDS */

struct IBlockStorageDevice SH7216_INTERNAL_FLASH_DeviceTable = 
{
    &SH7216_INTERNAL_FLASH_Driver::ChipInitialize,
    &SH7216_INTERNAL_FLASH_Driver::ChipUnInitialize,
    &SH7216_INTERNAL_FLASH_Driver::GetDeviceInfo,
    &SH7216_INTERNAL_FLASH_Driver::Read,
    &SH7216_INTERNAL_FLASH_Driver::Write,
    &SH7216_INTERNAL_FLASH_Driver::Memset,
    &SH7216_INTERNAL_FLASH_Driver::GetSectorMetadata,
    &SH7216_INTERNAL_FLASH_Driver::SetSectorMetadata,
    &SH7216_INTERNAL_FLASH_Driver::IsBlockErased,
    &SH7216_INTERNAL_FLASH_Driver::EraseBlock,
    &SH7216_INTERNAL_FLASH_Driver::SetPowerState,
    &SH7216_INTERNAL_FLASH_Driver::MaxSectorWrite_uSec,
    &SH7216_INTERNAL_FLASH_Driver::MaxBlockErase_uSec,     
};


