////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

//--//


#define USE_WEAR_LEVELING

extern struct BlockStorageDevice     g_K9F2G_8_BS;

#ifdef USE_WEAR_LEVELING
extern struct IBlockStorageDevice    g_BS_WearLeveling_DeviceTable;
extern struct BS_WearLeveling_Config g_K9F2G_8_WearLeveling_Config;
#else
extern struct IBlockStorageDevice    g_K9F2G08_BS_DeviceTable; 
extern struct BLOCK_CONFIG           g_K9F2G_8_BS_Config;
#endif

//--//

void BlockStorage_AddDevices()
{
#ifdef USE_WEAR_LEVELING
    BlockStorageList::AddDevice( &g_K9F2G_8_BS, &g_BS_WearLeveling_DeviceTable, &g_K9F2G_8_WearLeveling_Config, FALSE );
#else
    BlockStorageList::AddDevice( &g_K9F2G_8_BS, &g_K9F2G08_BS_DeviceTable, &g_K9F2G_8_BS_Config, FALSE );
#endif
//    BlockStorageList::AddDevice( &g_SAM9_SD_BS, &g_SAM9_SD_BS_DeviceTable, &g_SAM9_SD_BS_Config, FALSE );
}

//--//

