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

#ifndef _USB_BL_CONFIG_
#define _USB_BL_CONFIG_

#include <tinyhal.h>
#include "..\..\..\..\..\DeviceCode\Targets\Native\SH7264\DeviceCode\BlockStorage\USB\USB_BL.h"

//--//


#define USB_MANUFACTURER_CODE                 0xEC
#define USB_DEVICE_CODE                       0xDA
#define USB_BLOCK_COUNT                       0x80000//30699//cluster num
#define USB_SECTOR_PER_BLOCK                  8
#define USB_SECTOR_SIZE                       512
#define USB_BLOCK_SIZE                        USB_SECTOR_PER_BLOCK*USB_SECTOR_SIZE
#define USB_BLOCK_ERASE_TYPICAL_TIME_USEC     1500000
#define USB_SECTOR_WRITE_TYPICAL_TIME_USEC    200
#define USB_BLOCK_ERASE_MAX_TIME_USEC         2000000
#define USB_SECTOR_WRITE_MAX_TIME_USEC        700

#define USB_SIZE_IN_BYTES                   USB_BLOCK_SIZE*USB_BLOCK_COUNT
#define USB_WP_GPIO_PIN                     GPIO_PIN_NONE
#define USB_WP_ACTIVE                       FALSE


// BlockDeviceInformation

#define USB_REMOVEABLE                      TRUE
#define USB_SUPPORTXIP                      FALSE
#define USB_WRITEPROTECTED                  FALSE
#define USB_NUMREGIONS                      1


#define USB_LOW_VOLTAGE_FLAG      FALSE

const BlockRange g_USB_BlockRange[] =
{
    { BlockRange::BLOCKTYPE_FILESYSTEM    ,  0,  USB_BLOCK_COUNT -1 }
};

const BlockRegionInfo  g_USB_BlkRegion[USB_NUMREGIONS] = 
{
    0,                      // SectorAddress        Start;      // Starting Sector address
    USB_BLOCK_COUNT,                // UINT32           NumBlocks;      // total number of blocks in this region
    USB_BLOCK_SIZE,             // UINT16           BytesPerBlock;  // Number of Bytes in each block of this region 
    ARRAYSIZE_CONST_EXPR(g_USB_BlockRange),
    g_USB_BlockRange,
};

//--//

BlockDeviceInfo  g_USB_DeviceInfo=
{
    {  
        USB_REMOVEABLE,
        USB_SUPPORTXIP,
        USB_WRITEPROTECTED,
    },    
    USB_SECTOR_WRITE_MAX_TIME_USEC,     // UINT32 Duration_Max_WordWrite_uSec;
    USB_BLOCK_ERASE_MAX_TIME_USEC,      // UINT32 Duration_Max_SectorErase_uSec;
    USB_SECTOR_SIZE,                        // UINT32 BytesPerSector;
    2*1024*1024*1024,                      // UINT32 Size;
    USB_NUMREGIONS,                     // UINT32 NumRegions;
    g_USB_BlkRegion,                        // const BlockRegionInfo* pRegions;
};

struct BLOCK_CONFIG g_SH7264_USB_BL_Config =
{
    {
        USB_WP_GPIO_PIN,            // GPIO_PIN             Pin;
        USB_WP_ACTIVE,              // BOOL                 ActiveState;
    },
    &g_USB_DeviceInfo,             // BlockDeviceinfo
};

struct BlockStorageDevice g_SH7264_USB_Device;

struct USB_BL_CONFIGURATION g_USB_BL_Configuration =
{
    {// SPI_CONFIGURATION
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    },
    0,
    0,
    TRUE,                   // State_After_Erase will be filled at initialization
    USB_LOW_VOLTAGE_FLAG,
    &g_SH7264_USB_Device,
};

#endif