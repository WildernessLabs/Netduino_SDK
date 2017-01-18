////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "..\K9F2G_8.h"
#include <Drivers\BlockStorage\WearLeveling\BS_WearLeveling.h>

//--//

#define FLASH_MANUFACTURER_CODE                 0xEC
#define FLASH_DEVICE_CODE                       0xDA
#define FLASH_BASE_ADDRESS                      0x00000000
#define FLASH_SIZE                              0x10000000
#define FLASH_BLOCK_COUNT                       2048
#define FLASH_SECTOR_PER_BLOCK                  64
#define FLASH_SECTOR_SIZE                       2048
#define FLASH_BLOCK_SIZE                        (FLASH_SECTOR_PER_BLOCK*FLASH_SECTOR_SIZE)
#define FLASH_BLOCK_ERASE_TYPICAL_TIME_USEC     1500000
#define FLASH_SECTOR_WRITE_TYPICAL_TIME_USEC    200
#define FLASH_BLOCK_ERASE_MAX_TIME_USEC         2000000
#define FLASH_SECTOR_WRITE_MAX_TIME_USEC        700


#define K9F2G_8__SIZE_IN_BYTES                  (FLASH_BLOCK_SIZE*FLASH_BLOCK_COUNT)
#define K9F2G_8__WP_GPIO_PIN                    GPIO_PIN_NONE
#define K9F2G_8__WP_ACTIVE                      FALSE


// BlockDeviceInformation

#define K9F2G_8__IS_REMOVABLE                   FALSE
#define K9F2G_8__SUPPORTS_XIP                   FALSE
#define K9F2G_8__WRITE_PROTECTED                FALSE
#define K9F2G_8__NUM_REGIONS                    1


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



//--//


const BlockRange g_K9F2G_8_BlockStatus[] =
{
    { BlockRange::BLOCKTYPE_BOOTSTRAP ,  0,  1 },
    { BlockRange::BLOCKTYPE_CONFIG    ,  2,  2 },
    { BlockRange::BLOCKTYPE_CODE      ,  3, 22 },

    { BlockRange::BLOCKTYPE_DEPLOYMENT, 23, 30 },
    { BlockRange::BLOCKTYPE_DEPLOYMENT, 31, 39 },
    { BlockRange::BLOCKTYPE_DEPLOYMENT, 40, 46 },
    { BlockRange::BLOCKTYPE_SIMPLE_A  , 47, 47 },
    { BlockRange::BLOCKTYPE_SIMPLE_B  , 48, 48 },
    { BlockRange::BLOCKTYPE_STORAGE_A , 49, 49 },
    { BlockRange::BLOCKTYPE_STORAGE_B , 50, 50 },
    
    { BlockRange::BLOCKTYPE_FILESYSTEM, 51, FLASH_BLOCK_COUNT - 1 }
};

const BlockRegionInfo  g_K9F2G_8_BlkRegion[K9F2G_8__NUM_REGIONS] = 
{
    FLASH_BASE_ADDRESS,             // ByteAddress  Start;              // Starting Sector address
    FLASH_BLOCK_COUNT,              // UINT32       NumBlocks;          // total number of blocks in this region
    FLASH_BLOCK_SIZE,               // UINT32       BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)

    ARRAYSIZE_CONST_EXPR(g_K9F2G_8_BlockStatus),
    g_K9F2G_8_BlockStatus, 
};

//--//

#undef MEMORY_USAGE_GCC_SPECIAL_CODE 

//--//


const BlockDeviceInfo  g_K9F2G_8_DeviceInfo=
{
    {  
        K9F2G_8__IS_REMOVABLE,
        K9F2G_8__SUPPORTS_XIP,
        K9F2G_8__WRITE_PROTECTED,
    },
    FLASH_SECTOR_WRITE_MAX_TIME_USEC,           // UINT32 Duration_Max_WordWrite_uSec;
    FLASH_BLOCK_ERASE_MAX_TIME_USEC,            // UINT32 Duration_Max_SectorErase_uSec;
    FLASH_SECTOR_SIZE,                          // UINT32 BytesPerSector;

    FLASH_SIZE,                                 // UINT32 Size;
    
    ARRAYSIZE_CONST_EXPR(g_K9F2G_8_BlkRegion),  // UINT32 NumRegions;
    g_K9F2G_8_BlkRegion,                        // const BlockRegionInfo* pRegions;
};


struct BLOCK_CONFIG g_K9F2G_8_BS_Config =
{
    {
        K9F2G_8__WP_GPIO_PIN,               // GPIO_PIN             Pin;
        K9F2G_8__WP_ACTIVE,                 // BOOL                 ActiveState;
    },

    &g_K9F2G_8_DeviceInfo,             // BlockDeviceinfo
};

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata = "g_K9F2G_8_BS_WL"
#endif

extern struct IBlockStorageDevice g_K9F2G08_BS_DeviceTable;

struct BS_WearLeveling_Config g_K9F2G_8_WearLeveling_Config =
{
    &g_K9F2G_8_BS_Config,

    &g_K9F2G08_BS_DeviceTable,

    0,
    FLASH_BLOCK_SIZE,
    FLASH_BASE_ADDRESS,

    NULL,
};

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata
#endif


//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata = "g_K9F2G_8_BS"
#endif

struct BlockStorageDevice g_K9F2G_8_BS;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata 
#endif 

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata = "g_K9F2G_8_BS_Driver"
#endif

struct K9F2G_8_BS_Driver g_K9F2G_8_BS_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata 
#endif 

//--//

