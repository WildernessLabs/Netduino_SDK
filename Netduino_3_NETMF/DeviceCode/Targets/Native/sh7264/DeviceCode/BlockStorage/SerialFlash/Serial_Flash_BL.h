/*
 *    Copyright (C) 2010 Renesas Electronics America Inc. All rights reserved.
 */
 
#include <tinyhal.h>

//--//
#ifndef _DRIVERS_FLASH_SF_BL_H_
#define _DRIVERS_FLASH_SF_BL_H_ 1

typedef BYTE SERIAL_WORD;

#define SF_PAGE_SIZE        256         
#define SF_SECTOR_SIZE      0x10000     

//======== Serial Flash Commands ==========//
#define SFLASH_READ_ID          0x9F
#define SFLASH_CHIP_ERASE       0xc7
#define SFLASH_SECTOR_ERASE     0xd8
#define SFLASH_BYTE_PROGRAM     0x02
#define SFLASH_BYTE_READ        0x03
#define SFLASH_WRITE_ENABLE     0x06
#define SFLASH_WRITE_DISABLE    0x04
#define SFLASH_READ_STATUS      0x05
#define SFLASH_WRITE_STATUS     0x01
#define UNPROTECT_STATUS        0x00
#define PROTECT_STATUS          0x3C

//--//

struct SF_BS_Driver
{
    public:
    static BOOL InitializeDevice(void *context);
    static BOOL UnInitializeDevice(void *context);
    static const BlockDeviceInfo *GetDeviceInfo(void *context);
    static BOOL Read(void *context, ByteAddress Address, UINT32 NumBytes, BYTE *pSectorBuff);
    static BOOL Write(void *context, ByteAddress Address, UINT32 NumBytes, BYTE *pSectorBuff, BOOL ReadModifyWrite);
    static BOOL Memset(void *context, ByteAddress Address, UINT8 Data, UINT32 NumBytes);
    static BOOL GetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata);
    static BOOL SetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata);
    static BOOL IsBlockErased(void *context, ByteAddress BlockStartAddress, UINT32 BlockLength);
    static BOOL EraseBlock(void *context, ByteAddress Sector);
    static void SetPowerState(void *context, UINT32 State);
    static UINT32 MaxSectorWrite_uSec(void *context);
    static UINT32 MaxBlockErase_uSec(void *context);
    static BOOL ChipReadOnly(void *context, BOOL On, UINT32 ProtectionKey);
    static BOOL ReadProductID(void *context, SERIAL_WORD& ManufacturerCode, SERIAL_WORD& DeviceCode);
    static BOOL ReadSector(SectorAddress Address, UINT32 Offset, UINT32 NumBytes, BYTE *pSectorBuff, UINT32 BytesPerSector);
    static BOOL WriteX(void *context, ByteAddress Address, UINT32 NumBytes, BYTE *pSectorBuff, BOOL ReadModifyWrite, BOOL fIncrementDataPtr);
    private:

        static void SF_Chip_ID(unsigned char * id);
        static void SF_Chip_Erase(void);
        static void SF_Sector_Erase(unsigned long addr);
        static void SF_Byte_Program(unsigned int addr, unsigned char *buf, int size);
        static void SF_Byte_Read(unsigned int addr, unsigned char *buf, int size);
};

struct SERIALFLASH_BLOCK_CONFIG
{
    GPIO_FLAG WriteProtectionPin;
    BlockDeviceInfo *BlockDeviceInformation;
};

struct MEMORY_MAPPED_SERIAL_BLOCK_CONFIG
{
    SERIALFLASH_BLOCK_CONFIG            BlockConfig;
    CPU_MEMORY_CONFIG       Memory;
    UINT32                  ChipProtection;
    UINT32                  ManufacturerCode;
    UINT32                  DeviceCode;
};

#endif // _DRIVERS_FLASH_SF_BL_H_
