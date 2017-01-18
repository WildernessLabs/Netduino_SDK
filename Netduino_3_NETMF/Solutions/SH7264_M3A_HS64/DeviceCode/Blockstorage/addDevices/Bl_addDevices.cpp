////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

extern struct BlockStorageDevice  g_SH7264_BS;
extern struct IBlockStorageDevice g_AM29DL_16_BS_DeviceTable;
extern struct BLOCK_CONFIG        g_BS_Config_SH7264_M3A_HS64;

extern struct BlockStorageDevice  g_SerialFlash_Device;
extern struct IBlockStorageDevice g_SerialFlash_BL_DeviceTable;
extern struct BLOCK_CONFIG        g_SerialFlash_BL_Config;

extern struct BlockStorageDevice  g_SH7264_USB_Device;
extern struct IBlockStorageDevice g_SH7264_USB_BL_DeviceTable;
extern struct BLOCK_CONFIG        g_SH7264_USB_BL_Config;

void BlockStorage_AddDevices()
{   

    BlockStorageList::AddDevice( &g_SH7264_BS, &g_AM29DL_16_BS_DeviceTable, &g_BS_Config_SH7264_M3A_HS64, FALSE);
    BlockStorageList::AddDevice( &g_SerialFlash_Device, &g_SerialFlash_BL_DeviceTable, &g_SerialFlash_BL_Config, FALSE );
    BlockStorageList::AddDevice( &g_SH7264_USB_Device, &g_SH7264_USB_BL_DeviceTable, &g_SH7264_USB_BL_Config, FALSE );      
}

