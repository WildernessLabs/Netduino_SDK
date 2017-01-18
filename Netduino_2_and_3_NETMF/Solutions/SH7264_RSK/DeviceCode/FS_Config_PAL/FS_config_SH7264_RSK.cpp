////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata = "g_AvailableFSInterfaces"
#endif

extern FILESYSTEM_DRIVER_INTERFACE g_FAT32_FILE_SYSTEM_DriverInterface;
extern STREAM_DRIVER_INTERFACE g_FAT32_STREAM_DriverInterface;
extern struct BlockStorageDevice g_SH7264_USB_Device;
FileSystemVolume g_SH7264_USB_FS;


#ifdef SH7264_SERIAL_FLASH
extern struct BlockStorageDevice  g_SH7264_BS;
FileSystemVolume g_SH7264_NOR_FS;

#else
extern struct BlockStorageDevice g_SerialFlash_Device;
FileSystemVolume g_SH7264_SERIAL_FS;

#endif



void FS_AddVolumes()
{
#ifdef SH7264_SERIAL_FLASH
    FileSystemVolumeList::AddVolume( &g_SH7264_NOR_FS, "ROOT", 0, 0, &g_FAT32_STREAM_DriverInterface, &g_FAT32_FILE_SYSTEM_DriverInterface, &g_SH7264_BS, 1, FALSE );      
#else
    FileSystemVolumeList::AddVolume( &g_SH7264_SERIAL_FS, "ROOT", 0, 0, &g_FAT32_STREAM_DriverInterface, &g_FAT32_FILE_SYSTEM_DriverInterface, &g_SerialFlash_Device, 1, FALSE );      
#endif
    FileSystemVolumeList::AddVolume( &g_SH7264_USB_FS, "U", 0, 0, &g_FAT32_STREAM_DriverInterface, &g_FAT32_FILE_SYSTEM_DriverInterface, &g_SH7264_USB_Device, 1, FALSE );      
}

void FS_MountRemovableVolumes()
{
}

FILESYSTEM_INTERFACES g_AvailableFSInterfaces[] =
{
    { &g_FAT32_FILE_SYSTEM_DriverInterface, &g_FAT32_STREAM_DriverInterface },
};

const size_t g_InstalledFSCount = ARRAYSIZE(g_AvailableFSInterfaces);

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata
#endif
