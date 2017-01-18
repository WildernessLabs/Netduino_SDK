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
#include "..\MT29F2G08AAC.h"

#include <Drivers\BlockStorage\WearLeveling\BS_WearLeveling.h>
//--//

//#define TINCLR_TEST         1

#define FLASH_MANUFACTURER_CODE                 0x2C
#define FLASH_DEVICE_CODE                       0xDA

#define FLASH_BLOCK_COUNT                       2048

#define FLASH_BASE_ADDRESS                      0x00000000
#define FLASH_SECTOR_PER_BLOCK                  64
#define FLASH_SECTOR_SIZE                       2048
#define FLASH_BLOCK_SIZE                        FLASH_SECTOR_PER_BLOCK*FLASH_SECTOR_SIZE
#define FLASH_BLOCK_ERASE_TYPICAL_TIME_USEC     2000000
#define FLASH_SECTOR_WRITE_TYPICAL_TIME_USEC    300
#define FLASH_BLOCK_ERASE_MAX_TIME_USEC         3000000
#define FLASH_SECTOR_WRITE_MAX_TIME_USEC        700

#define FLASH_SIZE								0x10000000
#define MT29F2G08AAC_SIZE_IN_BYTES                   FLASH_BLOCK_SIZE*FLASH_BLOCK_COUNT
#define MT29F2G08AAC_WP_GPIO_PIN                     GPIO_PIN_NONE
#define MT29F2G08AAC_WP_ACTIVE                       FALSE


// BlockDeviceInformation

#define MT29F2G08AAC_REMOVEABLE                      FALSE
#define MT29F2G08AAC_SUPPORTXIP                      FALSE
#define MT29F2G08AAC_WRITEPROTECTED                  FALSE
#define MT29F2G08AAC_NUMREGIONS                      1


//--//
#ifdef MEMORY_USAGE_SPECIAL
#undef MEMORY_USAGE_SPECIAL
#endif

#ifdef MEMORY_USAGE_SPECIAL2
#undef MEMORY_USAGE_SPECIAL2
#endif

#ifdef MEMORY_USAGE_SPECIAL3
#undef MEMORY_USAGE_SPECIAL3
#endif

#if defined(MEMORY_USAGE_GCC_SPECIAL_CODE)
#undef MEMORY_USAGE_GCC_SPECIAL_CODE 
#endif

#ifdef __GNUC__
#define MEMORY_USAGE_GCC_SPECIAL_BOOTSTRAP  BlockStatus::USAGE_BOOTSTRAP
#define MEMORY_USAGE_GCC_SPECIAL_CONFIG     BlockStatus::USAGE_CONFIG

#else
#define MEMORY_USAGE_GCC_SPECIAL_BOOTSTRAP  BlockStatus::USAGE_CONFIG
#define MEMORY_USAGE_GCC_SPECIAL_CONFIG     BlockStatus::USAGE_CODE
#endif





//--//
const BlockRange g_MT29F2G08AAC_BlockStatus[] =
{
	{ BlockRange::BLOCKTYPE_BOOTSTRAP ,  0,  1 },
    { BlockRange::BLOCKTYPE_CONFIG    ,  2,  2 },
    { BlockRange::BLOCKTYPE_CODE      ,  3, 24 },

    { BlockRange::BLOCKTYPE_DEPLOYMENT, 25, 29 },
    { BlockRange::BLOCKTYPE_DEPLOYMENT, 30, 34 },
    { BlockRange::BLOCKTYPE_DEPLOYMENT, 35, 39 },
    { BlockRange::BLOCKTYPE_DEPLOYMENT, 40, 48 },
    { BlockRange::BLOCKTYPE_STORAGE_A , 49, 49 },
    { BlockRange::BLOCKTYPE_STORAGE_B , 50, 50 },
    
    { BlockRange::BLOCKTYPE_FILESYSTEM, 51, FLASH_BLOCK_COUNT - 1 }
};

const BlockRegionInfo  g_MT29F2G08AAC_BlkRegion[MT29F2G08AAC_NUMREGIONS] = 
{
    FLASH_BASE_ADDRESS,             // ByteAddress  Start;              // Starting Sector address
    FLASH_BLOCK_COUNT,              // UINT32       NumBlocks;          // total number of blocks in this region
    FLASH_BLOCK_SIZE,               // UINT32       BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)
	
	ARRAYSIZE_CONST_EXPR(g_MT29F2G08AAC_BlockStatus),
    g_MT29F2G08AAC_BlockStatus, 
};

//--//

#undef MEMORY_USAGE_GCC_SPECIAL_CODE 

//--//


BlockDeviceInfo  g_MT29F2G08AAC_DeviceInfo=
{
    {  
        MT29F2G08AAC_REMOVEABLE,
        MT29F2G08AAC_SUPPORTXIP,
        MT29F2G08AAC_WRITEPROTECTED,
    },
    
    FLASH_SECTOR_WRITE_MAX_TIME_USEC,       // UINT32 Duration_Max_WordWrite_uSec;
    FLASH_BLOCK_ERASE_MAX_TIME_USEC,        // UINT32 Duration_Max_SectorErase_uSec;
	FLASH_SECTOR_SIZE,
	FLASH_SIZE,
	ARRAYSIZE_CONST_EXPR(g_MT29F2G08AAC_BlkRegion),  // UINT32 NumRegions;
    g_MT29F2G08AAC_BlkRegion,                    //const BlockRegionInfo* pRegions;
};


struct BLOCK_CONFIG MT29F2G08AAC_BS_Config =
{
    {
        MT29F2G08AAC_WP_GPIO_PIN,            // GPIO_PIN             Pin;
        MT29F2G08AAC_WP_ACTIVE,              // BOOL                 ActiveState;
    },

     &g_MT29F2G08AAC_DeviceInfo,             // BlockDeviceinfo
     

};


extern struct IBlockStorageDevice g_MT29F2G08AAC_BS_DeviceTable;

struct BS_WearLeveling_Config g_MT29F2G08AAC_8_WearLeveling_Config =
{
    &MT29F2G08AAC_BS_Config,

    &g_MT29F2G08AAC_BS_DeviceTable,

    0,
    FLASH_BLOCK_SIZE,

    NULL,
};
//--//


#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata = "g_MT29F2G08AAC_BS"
#endif

struct BlockStorageDevice g_MT29F2G08AAC_BS;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata 
#endif 

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata = "g_MT29F2G08AAC_BS_Driver"
#endif

struct MT29F2G08AAC_BS_Driver g_MT29F2G08AAC_BS_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata 
#endif 

//--//

