////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata = "g_AvailableFSInterfaces"
#endif

extern FILESYSTEM_DRIVER_INTERFACE g_FAT32_FILE_SYSTEM_DriverInterface;
extern STREAM_DRIVER_INTERFACE g_FAT32_STREAM_DriverInterface;

extern struct BlockStorageDevice  g_SD_BS;
extern struct IBlockStorageDevice g_SD_BS_DeviceTable;
extern struct BLOCK_CONFIG        g_SD_BS_Config;

void SD_InsertEjectIsr( GPIO_PIN Pin, BOOL PinState, void* Param );

void FS_AddVolumes()
{
    // enable power to the SD card slot
	CPU_GPIO_EnableOutputPin( 0x11, FALSE ); // PB1: /PWR_CTRL_MICROSD

    // wire up our SD card detect interrupt handler (with glitch filter)
    CPU_GPIO_EnableInputPin( 0x1B, TRUE, SD_InsertEjectIsr, GPIO_INT_EDGE_BOTH, RESISTOR_PULLUP );
}

void FS_MountRemovableVolumes()
{
    // if the SD card is inserted, try to mount it.
    if ( !CPU_GPIO_GetPinState( 0x15 ) )
    {
        if (BlockStorageList::AddDevice( &g_SD_BS, &g_SD_BS_DeviceTable, &g_SD_BS_Config, TRUE ));
        {
            FS_MountVolume( "SD", 0, 0, &g_SD_BS );		
        }
    }
}

FILESYSTEM_INTERFACES g_AvailableFSInterfaces[] =
{
    { &g_FAT32_FILE_SYSTEM_DriverInterface, &g_FAT32_STREAM_DriverInterface },
};

void SD_InsertEjectIsr( GPIO_PIN Pin, BOOL PinState, void* Param )
{
    // if the SD card was inserted, try to mount it; if it was ejected, try to unmount it.
    if ( !PinState ) 
    {
        // card was inserted
        if (BlockStorageList::AddDevice( &g_SD_BS, &g_SD_BS_DeviceTable, &g_SD_BS_Config, TRUE ));
        {
            FS_MountVolume( "SD", 0, 0, &g_SD_BS );
        }
    }
    else
    {
        // card was ejected
        FS_UnmountVolume( &g_SD_BS );
        BlockStorageList::RemoveDevice( &g_SD_BS, TRUE );
    }
}

const size_t g_InstalledFSCount = 1;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata
#endif
