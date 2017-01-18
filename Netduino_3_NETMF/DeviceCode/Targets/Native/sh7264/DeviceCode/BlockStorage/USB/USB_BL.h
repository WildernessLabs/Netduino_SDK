/*
 *    Copyright (C) 2010 Renesas Electronics America Inc. All rights reserved.
 */
 
#include <tinyhal.h>

//--//
#ifndef _DRIVERS_FLASH_USB_BL_H_
#define _DRIVERS_FLASH_USB_BL_H_ 1

typedef BYTE CHIP_WORD;

struct USB_BLOCK_CONFIG
{
    GPIO_FLAG WriteProtectionPin;
    BlockDeviceInfo *BlockDeviceInformation;
};

//--//

struct USB_BS_Driver
{
    static BOOL CPU_USB_ChipInitialize(void *context);
    static BOOL CPU_USB_ChipUnInitialize(void *context);
    static const BlockDeviceInfo *CPU_USB_GetDeviceInfo(void *context);
    static BOOL CPU_USB_Read(void *context, ByteAddress Address, UINT32 NumBytes, BYTE *pSectorBuff);
    static BOOL CPU_USB_Write(void *context, ByteAddress Address, UINT32 NumBytes, BYTE *pSectorBuff, BOOL ReadModifyWrite);
    static BOOL CPU_USB_Memset(void *context, ByteAddress Address, UINT8 Data, UINT32 NumBytes);
    static BOOL CPU_USB_GetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata);
    static BOOL CPU_USB_SetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata);
    static BOOL CPU_USB_IsBlockErased(void *context, ByteAddress BlockStartAddress, UINT32 BlockLength);
    static BOOL CPU_USB_EraseBlock(void *context, ByteAddress Sector);
    static void CPU_USB_SetPowerState(void *context, UINT32 State);
    static UINT32 CPU_USB_MaxSectorWrite_uSec(void *context);
    static UINT32 CPU_USB_MaxBlockErase_uSec(void *context);
    static BOOL CPU_USB_ChipReadOnly(void *context, BOOL On, UINT32 ProtectionKey);
    static BOOL CPU_USB_ReadProductID(void *context, BYTE *ManufacturerCode, BYTE *OEMID, BYTE *ProductName);
    static BOOL CPU_USB_EraseSectors(SectorAddress Address, INT32 SectorCount);
    static BOOL CPU_USB_ReadSector(SectorAddress Address, UINT32 Offset, UINT32 NumBytes, BYTE *pSectorBuff, UINT32 BytesPerSector);
    static BOOL CPU_USB_WriteX(void *context, ByteAddress Address, UINT32 NumBytes, BYTE *pSectorBuff, BOOL ReadModifyWrite, BOOL fIncrementDataPtr);
        static BOOL CPU_USB_Global_ISR(void* Param);
};
struct USB_BL_CONFIGURATION
{
    SPI_CONFIGURATION   SPI;
    GPIO_PIN            InsertIsrPin;
    GPIO_PIN            EjectIsrPin;
    BOOL                State_After_Erase;
    BOOL                Low_Voltage_Flag;
    BlockStorageDevice* Device;
};

#endif // _DRIVERS_FLASH_USB_BL_H_
