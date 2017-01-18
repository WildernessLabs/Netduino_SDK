////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata = "g_AvailableFSInterfaces"
#endif

extern FILESYSTEM_DRIVER_INTERFACE g_FAT32_FILE_SYSTEM_DriverInterface;
extern STREAM_DRIVER_INTERFACE     g_FAT32_STREAM_DriverInterface;
extern BlockStorageDevice g_SST39VF320_16_BS;

FileSystemVolume g_EA_LPC2478_FS;

void FS_AddVolumes()
{
    FileSystemVolumeList::AddVolume( &g_EA_LPC2478_FS, "ROOT", 0, 0, &g_FAT32_STREAM_DriverInterface, &g_FAT32_FILE_SYSTEM_DriverInterface, &g_SST39VF320_16_BS, 0, FALSE );
}

void FS_MountRemovableVolumes()
{
}

FILESYSTEM_INTERFACES g_AvailableFSInterfaces[] =
{
    { &g_FAT32_FILE_SYSTEM_DriverInterface  , &g_FAT32_STREAM_DriverInterface },
};

const size_t g_InstalledFSCount = ARRAYSIZE(g_AvailableFSInterfaces);

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata
#endif
