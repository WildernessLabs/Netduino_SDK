////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#if defined(SH7619_EVB_8M_FLASH)
extern struct BlockStorageDevice  g_S29GL064A_16_BS;
extern struct IBlockStorageDevice g_AM29DL_16_BS_DeviceTable;
extern struct BLOCK_CONFIG        g_S29GL064A_16_BS_Config;
void BlockStorage_AddDevices()
{   
     BlockStorageList::AddDevice(&g_S29GL064A_16_BS, &g_AM29DL_16_BS_DeviceTable, &g_S29GL064A_16_BS_Config, FALSE);
}

#else
extern struct BlockStorageDevice  g_S29GL032A_16_BS;
extern struct IBlockStorageDevice g_AM29DL_16_BS_DeviceTable;
extern struct BLOCK_CONFIG        g_S29GL032A_16_BS_Config;


void BlockStorage_AddDevices()
{   
     BlockStorageList::AddDevice(&g_S29GL032A_16_BS, &g_AM29DL_16_BS_DeviceTable, &g_S29GL032A_16_BS_Config, FALSE);
}
#endif

