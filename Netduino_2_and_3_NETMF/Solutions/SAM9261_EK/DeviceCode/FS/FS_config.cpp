////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <Drivers\fs\fat\fat_fs.h>

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata = "g_AvailableFSInterfaces"
#endif

extern FILESYSTEM_DRIVER_INTERFACE g_FAT32_FILE_SYSTEM_DriverInterface;
extern STREAM_DRIVER_INTERFACE g_FAT32_STREAM_DriverInterface;

FILESYSTEM_DRIVER_INTERFACE g_SAM9_FAT32_FILE_SYSTEM_DriverInterface = 
{    
    &FAT_FS_Driver::FindOpen,
    &FAT_FS_Driver::FindNext,
    &FAT_FS_Driver::FindClose,

    &FAT_FS_Driver::GetFileInfo,

    &FAT_FS_Driver::CreateDirectory,
    &FAT_FS_Driver::Move,
    &FAT_FS_Driver::Delete,

    &FAT_FS_Driver::GetAttributes,
    &FAT_FS_Driver::SetAttributes,    

    &FAT_FS_Driver::Format,
    &FAT_FS_Driver::IsLoadableMedia, 
    &FAT_FS_Driver::GetSizeInfo,
    &FAT_FS_Driver::FlushAll,
    &FAT_FS_Driver::GetVolumeLabel,

    "FAT",
    FS_DRIVER_ATTRIBUTE__FORMAT_REQUIRES_ERASE,
};

extern BlockStorageDevice g_K9F2G_8_BS;
extern BlockStorageDevice g_SAM9_SD_BS;

FileSystemVolume g_SAM9261_EK_NAND_FS;
FileSystemVolume g_SAM9261_EK_SD_FS;

void FS_AddVolumes()
{
    FileSystemVolumeList::AddVolume( &g_SAM9261_EK_NAND_FS, "ROOT", 0, 0, &g_FAT32_STREAM_DriverInterface, &g_FAT32_FILE_SYSTEM_DriverInterface, &g_K9F2G_8_BS, 0, FALSE );
}

void FS_MountRemovableVolumes()
{
    //FileSystemVolumeList::AddVolume( &g_SAM9261_EK_SD_FS  , "SD1" , 1, 0, &g_FAT32_STREAM_DriverInterface, &g_FAT16_FILE_SYSTEM_DriverInterface, &g_SAM9_SD_BS, 1, FALSE );
}

FILESYSTEM_INTERFACES g_AvailableFSInterfaces[] =
{
    { &g_SAM9_FAT32_FILE_SYSTEM_DriverInterface, &g_FAT32_STREAM_DriverInterface },
    //{ &g_FAT16_FILE_SYSTEM_DriverInterface, &g_FAT32_STREAM_DriverInterface },
};

const size_t g_InstalledFSCount = ARRAYSIZE(g_AvailableFSInterfaces);

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata
#endif
