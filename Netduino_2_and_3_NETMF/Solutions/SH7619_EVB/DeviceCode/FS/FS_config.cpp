////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata = "g_AvailableFSInterfaces"
#endif

extern FILESYSTEM_DRIVER_INTERFACE g_FAT32_FILE_SYSTEM_DriverInterface;
extern STREAM_DRIVER_INTERFACE g_FAT32_STREAM_DriverInterface;

#if defined(SH7619_EVB_8M_FLASH)
extern BlockStorageDevice g_S29GL064A_16_BS;
#else
extern BlockStorageDevice g_S29GL032A_16_BS;
#endif

FileSystemVolume g_SH7619_EVB_NOR_FS;

void FS_AddVolumes()
{
#if defined(SH7619_EVB_8M_FLASH)
    FileSystemVolumeList::AddVolume( &g_SH7619_EVB_NOR_FS, "ROOT", 0, 0, &g_FAT32_STREAM_DriverInterface, &g_FAT32_FILE_SYSTEM_DriverInterface, &g_S29GL064A_16_BS, 0, FALSE );
#else
    FileSystemVolumeList::AddVolume( &g_SH7619_EVB_NOR_FS, "ROOT", 0, 0, &g_FAT32_STREAM_DriverInterface, &g_FAT32_FILE_SYSTEM_DriverInterface, &g_S29GL032A_16_BS, 0, FALSE );
#endif
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
