////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// This file is part of the Microsoft .NET Micro Framerwork Porting Kit Code Samples and is unsupported. 
// Copyright (C) Microsoft Corporation. All rights reserved. Use of this sample source code is subject to 
// the terms of the Microsoft license agreement under which you licensed this sample source code. 
// 
// THIS SAMPLE CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// 
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

extern struct BlockStorageDevice  g_MT29F2G08AAC_BS;
extern struct IBlockStorageDevice    g_BS_WearLeveling_DeviceTable;
extern struct BS_WearLeveling_Config        g_MT29F2G08AAC_8_WearLeveling_Config;

void BlockStorage_AddDevices()
{
	BlockStorageList::AddDevice(&g_MT29F2G08AAC_BS, &g_BS_WearLeveling_DeviceTable, &g_MT29F2G08AAC_8_WearLeveling_Config, FALSE);
}

