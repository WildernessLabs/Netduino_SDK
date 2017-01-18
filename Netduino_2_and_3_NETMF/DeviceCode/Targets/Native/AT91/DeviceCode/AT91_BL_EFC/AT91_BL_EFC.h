////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _AT91_BS_EFC_H_1
#define _AT91_BS_EFC_H_1   1

//--//


struct AT91_BL_EFC {

    static const UINT32 c_Base_0 = AT91C_BASE_EFC0;
    static const UINT32 c_Base_1 = AT91C_BASE_EFC1;


    /****/ volatile  UINT32     EFC_FMR;   // MC Flash Mode Register
    // Flash Ready
    static const    UINT32 MC_FRDY         = (0x1 <<  0);  
    // Lock Error
    static const    UINT32 MC_LOCKE        = (0x1 <<  2);  
    // Programming Error
    static const    UINT32 MC_PROGE        = (0x1 <<  3);  
    //  No Erase Before Programming
    static const    UINT32 MC_NEBP         = (0x1 <<  7);  
    //  Flash Wait State
    static const    UINT32 MC_FWS          = (0x3 <<  8);  
    //  1 cycle for Read, 2 for Write operations
    static const    UINT32 MC_FWS_0FWS     = (0x0 <<  8);  
    //  2 cycles for Read, 3 for Write operations
    static const    UINT32 MC_FWS_1FWS     = (0x1 <<  8);  
    //  3 cycles for Read, 4 for Write operations
    static const    UINT32 MC_FWS_2FWS     = (0x2 <<  8);  
    //  4 cycles for Read, 4 for Write operations
    static const    UINT32 MC_FWS_3FWS     = (0x3 <<  8);  
    //  Flash Microsecond Cycle Number
    static const    UINT32 MC_FMCN         = (0xFF << 16); 

    /****/ volatile  UINT32     EFC_FCR;   // MC Flash Command Register
    //  Flash Command
    static const    UINT32 MC_FCMD                 = (0xF <<  0);   
    //  Starts the programming of th epage specified by PAGEN.
    static const    UINT32 MC_FCMD_START_PROG      = (0x1);         
    //  Starts a lock sequence of the sector defined by the bits 4 to 7 of the field PAGEN.
    static const    UINT32 MC_FCMD_LOCK            = (0x2);         
    //  The lock sequence automatically happens after the programming sequence is completed.
    static const    UINT32 MC_FCMD_PROG_AND_LOCK   = (0x3);         
    //  Starts an unlock sequence of the sector defined by the bits 4 to 7 of the field PAGEN.
    static const    UINT32 MC_FCMD_UNLOCK          = (0x4);         
    //  Starts the erase of the entire flash.If at least a page is locked, the command is cancelled.
    static const    UINT32 MC_FCMD_ERASE_ALL       = (0x8);         
    //  Set General Purpose NVM bits.
    static const    UINT32 MC_FCMD_SET_GP_NVM      = (0xB);         
    //  Clear General Purpose NVM bits.
    static const    UINT32 MC_FCMD_CLR_GP_NVM      = (0xD);         
    //  Set Security Bit.
    static const    UINT32 MC_FCMD_SET_SECURITY    = (0xF);         
    //  Page Number
    static const    UINT32 MC_PAGEN                = (0x3FF <<  8); 
    //  Writing Protect Key
    static const    UINT32 MC_KEY                  = ((UINT32)0xFF  << 24); 
    static const    UINT32 MC_KEY_VALUE            = ((UINT32)0x5A  << 24); 

    /****/ volatile  UINT32     EFC_FSR;   // MC Flash Status Register
    //  Security Bit Status
    static const    UINT32 MC_SECURITY     = (0x1 <<  4); 
    //  Sector 0 Lock Status
    static const    UINT32 MC_GPNVM0       = (0x1 <<  8); 
    //  Sector 1 Lock Status
    static const    UINT32 MC_GPNVM1       = (0x1 <<  9); 
    //  Sector 2 Lock Status
    static const    UINT32 MC_GPNVM2       = (0x1 << 10); 
    //  Sector 3 Lock Status
    static const    UINT32 MC_GPNVM3       = (0x1 << 11); 
    //  Sector 4 Lock Status
    static const    UINT32 MC_GPNVM4       = (0x1 << 12); 
    //  Sector 5 Lock Status
    static const    UINT32 MC_GPNVM5       = (0x1 << 13); 
    //  Sector 6 Lock Status
    static const    UINT32 MC_GPNVM6       = (0x1 << 14); 
    //  Sector 7 Lock Status
    static const    UINT32 MC_GPNVM7       = (0x1 << 15); 
    //  Sector 0 Lock Status
    static const    UINT32 MC_LOCKS0       = (0x1 << 16); 
    //  Sector 1 Lock Status
    static const    UINT32 MC_LOCKS1       = (0x1 << 17); 
    //  Sector 2 Lock Status
    static const    UINT32 MC_LOCKS2       = (0x1 << 18); 
    //  Sector 3 Lock Status
    static const    UINT32 MC_LOCKS3       = (0x1 << 19); 
    //  Sector 4 Lock Status
    static const    UINT32 MC_LOCKS4       = (0x1 << 20); 
    //  Sector 5 Lock Status
    static const    UINT32 MC_LOCKS5       = (0x1 << 21); 
    //  Sector 6 Lock Status
    static const    UINT32 MC_LOCKS6       = (0x1 << 22); 
    //  Sector 7 Lock Status
    static const    UINT32 MC_LOCKS7       = (0x1 << 23); 
    //  Sector 8 Lock Status
    static const    UINT32 MC_LOCKS8       = (0x1 << 24); 
    //  Sector 9 Lock Status
    static const    UINT32 MC_LOCKS9       = (0x1 << 25); 
    //  Sector 10 Lock Status
    static const    UINT32 MC_LOCKS10      = (0x1 << 26); 
    //  Sector 11 Lock Status
    static const    UINT32 MC_LOCKS11      = (0x1 << 27); 
    //  Sector 12 Lock Status
    static const    UINT32 MC_LOCKS12      = (0x1 << 28); 
    //  Sector 13 Lock Status
    static const    UINT32 MC_LOCKS13      = (0x1 << 29); 
    //  Sector 14 Lock Status
    static const    UINT32 MC_LOCKS14      = (0x1 << 30); 
    //  Sector 15 Lock Status
    static const    UINT32 MC_LOCKS15      = ((UINT32)0x1 << 31); 


    /****/ volatile  UINT32     EFC_VR;    // MC Flash Version Register
    //  EFC version number
    static const    UINT32 EFC_VERSION     = (0xFFF <<  0); 
    //  EFC MFN
    static const    UINT32 EFC_MFN         = (0x7 << 16);   

    static AT91_BL_EFC   & BL_EFC( int sel )  { if( sel == 0) return *(AT91_BL_EFC   *)(size_t)c_Base_0;
                                                  else      return *(AT91_BL_EFC   *)(size_t)c_Base_1;                        }

};


//--//


struct SAM7X_BS_Driver
{
    typedef UINT32 CHIP_WORD;

    //--// The following values will be modified

    static const CHIP_WORD MANUFACTURER_CODE            = 0xffff;
    static const CHIP_WORD DEVICE_CODE                      = 0xffff;

    static const size_t BLOCK_ERASE_TYPICAL_TIME_USEC   = 96000;
    static const size_t SECTOR_WRITE_TYPICAL_TIME_USEC  = 48000;

    static const size_t BLOCK_ERASE_MAX_TIME_USEC       = 96000;
    static const size_t SECTOR_WRITE_MAX_TIME_USEC       = 48000; 

    //--//

    static BOOL ChipInitialize( void* context );

    static BOOL ChipUnInitialize( void* context );

    static const BlockDeviceInfo* GetDeviceInfo( void* context);

    static BOOL Read( void* context, ByteAddress Address, UINT32 NumBytes, BYTE * pSectorBuff );
    
    static BOOL Write( void* context, ByteAddress Address, UINT32 NumBytes, BYTE * pSectorBuff, BOOL ReadModifyWrite );

    static BOOL Memset( void* context, ByteAddress Address, UINT8 Data, UINT32 NumBytes );
    
    static BOOL GetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata);

    static BOOL SetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata);

    static BOOL IsBlockErased( void* context, ByteAddress Sector, UINT32 BlockLength );

    static BOOL EraseBlock( void* context, ByteAddress Sector );

    static void SetPowerState( void* context, UINT32 State );

    static UINT32 MaxSectorWrite_uSec( void* context );

    static UINT32 MaxBlockErase_uSec( void* context );

    static BOOL ChipReadOnly( void* context, BOOL On, UINT32 ProtectionKey );

private:
    static BOOL WriteX( void* context, ByteAddress Address, UINT32 NumBytes, BYTE * pSectorBuff, BOOL ReadModifyWrite, BOOL fIncrementDataPtr );
};

//--//

#endif //_AT91_BS_EFC_H_1

