////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "tinyhal.h"

//--//

#ifndef _Sh7216_INTERNAL_FLASH
#define _Sh7216_INTERNAL_FLASH 1

/******************************************************************************
Macro definitions
******************************************************************************/
/****** MCU Clock Registers *****/
#define FRQCR   (*(volatile unsigned short *)0xFFFE0010)        /* Frequency control register */

/****** Module Standby Registers *****/
#define STBCR2  (*(unsigned char *)0xFFFE0018)      /* Standby control register 2 */
#define STBCR3  (*(unsigned char *)0xFFFE0408)      /* Standby Control Register 3 */

/****** Flash Control Registers *****/
#define FPMON   (*(volatile unsigned char *)0xFFFFA800)     /* Flash pin monitor register */
#define FMODR   (*(volatile unsigned char *)0xFFFFA802)     /* Flash mode register */
#define FASTAT  (*(volatile unsigned char *)0xFFFFA810)     /* Flash access status register */
#define FAEINT  (*(volatile unsigned char *)0xFFFFA811)     /* Flash access error interrupt enable register */
#define ROMMAT  (*(volatile unsigned short *)0xFFFFA820)        /* ROM MAT select register */
#define FCURAME (*(volatile unsigned short *)0xFFFFA854)    /* FCU RAM enable register */
#define FSTATR0 (*(volatile unsigned char *)0xFFFFA900)     /* Flash status register 0 */
#define FSTATR1 (*(volatile unsigned char *)0xFFFFA901)     /* Flash status register 1 */
#define FENTRYR (*(volatile unsigned short *)0xFFFFA902)    /* Flash P/E mode entry register */
#define FPROTR  (*(volatile unsigned short *)0xFFFFA904)    /* Flash protect register */
#define FRESETR (*(volatile unsigned short *)0xFFFFA906)    /* Flash reset register */
#define FCMDR   (*(volatile unsigned short *)0xFFFFA90A)    /* FCU command register */
#define FCPSR   (*(volatile unsigned short *)0xFFFFA918)    /* FCU processing switch register */
#define FPESTAT (*(volatile unsigned short *)0xFFFFA91C)    /* Flash P/E status register */
#define RCCR    (*(volatile unsigned long *)0xFFFC1400)     /* ROM cache control register */

/* FLD only registers */
#define FMODR       (*(volatile unsigned char *)0xFFFFA802)     /* Flash mode register */
#define EEPRE0      (*(volatile unsigned short *)0xFFFFA840)    /* FLD read enable register 0 */
#define EEPWE0      (*(volatile unsigned short *)0xFFFFA850)    /* FLD program/erase enable register 0 */
#define EEPBCCNT    (*(volatile unsigned short *)0xFFFFA91A)    /* FLD blank check control register */
#define EEPBCSTAT   (*(volatile unsigned short *)0xFFFFA91E)    /* FLD blank check status register */


#define FLD_ADDRESS 0x80100000
#define ROM_PE_ADDR 0x80800000

#define READ_MODE 0
#define ROM_PE_MODE 1
#define FLD_PE_MODE 2

#define BLANK_CHECK_8_BYTE       0  /* 'size' parameter for FlashFLDBlankCheck */
#define BLANK_CHECK_ENTIRE_BLOCK 1  /* 'size' parameter for FlashFLDBlankCheck */


//--//

struct SH7216_INTERNAL_FLASH_Driver
{
    typedef UINT32 CHIP_WORD;

    static BOOL ChipInitialize( void* context );

    static BOOL ChipUnInitialize( void* context );

    static const BlockDeviceInfo* GetDeviceInfo( void* context );

    static BOOL Read( void* context, ByteAddress Address, UINT32 NumBytes, BYTE * pSectorBuff );

    static BOOL Write( void* context, ByteAddress Address, UINT32 NumBytes, BYTE * pSectorBuff, BOOL ReadModifyWrite );

    static BOOL Memset( void* context, ByteAddress Address, UINT8 Data, UINT32 NumBytes );

    static BOOL GetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata);

    static BOOL SetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata);

    static BOOL IsBlockErased( void* context, ByteAddress address, UINT32 blockLength );

    static BOOL EraseBlock( void* context, ByteAddress Sector );

    static void SetPowerState( void* context, UINT32 State );

    static UINT32 MaxSectorWrite_uSec( void* context );

    static UINT32 MaxBlockErase_uSec( void* context );

//--//
    static BOOL ChipReadOnly( void* context, BOOL On, UINT32 ProtectionKey );

    static BOOL ReadProductID( void* context, FLASH_WORD& ManufacturerCode, FLASH_WORD& DeviceCode );

private:
    static BOOL WriteX( void* context, ByteAddress StartSector, UINT32 NumSectors,BYTE * pSectorBuff, BOOL ReadModifyWrite, BOOL fIncrementDataPtr );
    static void Action_ReadID     ( void* context, FLASH_WORD& ManufacturerCode, FLASH_WORD& DeviceCode );
    static BOOL Action_EraseSector( void* context, volatile CHIP_WORD SectorStart                      );
    static BOOL Action_WriteFlash( void* context, volatile CHIP_WORD Address, CHIP_WORD Data, UINT16 numBytesWrite);
    static void Action_FLDAreaAccess(UINT8 read_mask, UINT8 write_mask                                  );
    static BOOL Action_EnterPEMode  (volatile CHIP_WORD BaseAddress                                 );
    static void Action_ExitPEMode   (volatile CHIP_WORD BaseAddress                                 );
};

//--//

#endif // _Sh7216_INTERNAL_FLASH

