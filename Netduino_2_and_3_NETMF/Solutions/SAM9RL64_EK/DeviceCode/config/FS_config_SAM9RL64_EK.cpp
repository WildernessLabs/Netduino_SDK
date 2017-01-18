////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// This file is part of the Microsoft .NET Micro Framework Porting Kit Code Samples and is unsupported. 
// Copyright (C) Microsoft Corporation. All rights reserved. Use of this sample source code is subject to 
// the terms of the Microsoft license agreement under which you licensed this sample source code. 
// 
// THIS SAMPLE CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// 
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "..\..\..\..\DeviceCode\Drivers\FS\FAT\FAT_FS.h"


#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata = "g_AvailableFSInterfaces"
#endif


extern FILESYSTEM_DRIVER_INTERFACE g_FAT32_FILE_SYSTEM_DriverInterface;
extern STREAM_DRIVER_INTERFACE g_FAT32_STREAM_DriverInterface;
extern BlockStorageDevice g_MT29F2G08AAC_BS;
extern struct IBlockStorageDevice g_MT29F2G08AAC_BS_DeviceTable; 
extern struct BLOCK_CONFIG        MT29F2G08AAC_BS_Config;

FileSystemVolume g_SAM9RL64_EK_NAND_FS;

void FS_AddVolumes()
{
	FileSystemVolume* pFSVolume;
	FAT_LogicDisk* pLogicDisk;
//	g_MT29F2G08AAC_BS.m_BSD = &g_MT29F2G08AAC_BS_DeviceTable;
//	g_MT29F2G08AAC_BS.m_context = &MT29F2G08AAC_BS_Config;
    FileSystemVolumeList::AddVolume( &g_SAM9RL64_EK_NAND_FS, "ROOT", 0, 0, &g_FAT32_STREAM_DriverInterface, &g_FAT32_FILE_SYSTEM_DriverInterface, &g_MT29F2G08AAC_BS, 0, FALSE );
	
	pFSVolume = FileSystemVolumeList::FindVolume("ROOT", 4);
	if (pFSVolume)
	{
	   pLogicDisk = FAT_LogicDisk::Initialize(&(pFSVolume->m_volumeId));
	   if (pLogicDisk== NULL)
	   {
	       pFSVolume->Format("", FORMAT_PARAMETER_FORCE_FAT32);  
		 
	   }
	   else
	   {
			pLogicDisk->Uninitialize();
		
	   }
	}
}

void FS_MountRemovableVolumes()
{
}

FILESYSTEM_INTERFACES g_AvailableFSInterfaces[] =
{
    { &g_FAT32_FILE_SYSTEM_DriverInterface, &g_FAT32_STREAM_DriverInterface },
};

const size_t g_InstalledFSCount = 1;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata
#endif
