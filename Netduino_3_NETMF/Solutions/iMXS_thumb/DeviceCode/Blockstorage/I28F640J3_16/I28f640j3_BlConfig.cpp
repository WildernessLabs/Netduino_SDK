////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#include <Drivers\BlockStorage\Flash\I28F_16\I28F_16.h>

//--//


#define FLASH_MANUFACTURER_CODE              0x0089
#define FLASH_DEVICE_CODE                    0x0017
#define FLASH_BLOCK_COUNT                    64
#define FLASH_BYTES_PER_SECTOR               2
#define FLASH_BYTES_PER_BLOCK                0x20000
#define FLASH_BLOCK_ERASE_TYPICAL_TIME_USEC  1000000
#define FLASH_SECTOR_WRITE_TYPICAL_TIME_USEC 210
#define FLASH_BLOCK_ERASE_MAX_TIME_USEC      4000000
#define FLASH_SECTOR_WRITE_MAX_TIME_USEC     630
//
// The real max time should be 4,000,000 uSec, but the chip has independent partitions, it's not critical to avoid radio collisions.
//
#define FLASH_BLOCK_ERASE_ACTUAL_TIME_USEC   400000
//--//

// EBIU Information

#define I28F640J3_16__CHIP_SELECT       0
#define I28F640J3_16__WAIT_STATES       7                    /* New freescale board uses 75 nS parts */
#define I28F640J3_16__RELEASE_COUNTS    0
#define I28F640J3_16__BIT_WIDTH         (16 | (1 << 16))     /* Data is on D15-D0 on the freescale boards  */
#define I28F640J3_16__BASE_ADDRESS      FLASH_MEMORY_Base
#define I28F640J3_16__SIZE_IN_BYTES     8*1024*1024
#define I28F640J3_16__WP_GPIO_PIN       GPIO_PIN_NONE
#define I28F640J3_16__WP_ACTIVE         FALSE



//--//

#define I28F640J3_16__FLASH_BOTTOM_PARAMETER 1
#undef  I28F640J3_16__FLASH_TOP_PARAMETER



// BlockDeviceInformation

#define I28F640J3_16__IS_REMOVABLE     FALSE
#define I28F640J3_16__SUPPORTS_XIP     TRUE
#define I28F640J3_16__WRITE_PROTECTED  FALSE
#define I28F640J3_16__NUM_REGIONS      1


//--//
#ifdef MEMORY_BLOCKTYPE_SPECIAL
#undef MEMORY_BLOCKTYPE_SPECIAL
#endif

#ifdef MEMORY_BLOCKTYPE_SPECIAL2
#undef MEMORY_BLOCKTYPE_SPECIAL2
#endif

#ifdef MEMORY_BLOCKTYPE_SPECIAL3
#undef MEMORY_BLOCKTYPE_SPECIAL3
#endif

#if defined(MEMORY_BLOCKTYPE_GCC_SPECIAL_CODE)
#undef MEMORY_BLOCKTYPE_GCC_SPECIAL_CODE 
#endif

#ifdef __GNUC__
#define MEMORY_BLOCKTYPE_GCC_SPECIAL_CODE       BlockRange::BLOCKTYPE_CODE
#define MEMORY_BLOCKTYPE_GCC_BOOTSTRAP_CODE     BlockRange::BLOCKTYPE_BOOTSTRAP
#else
#define MEMORY_BLOCKTYPE_GCC_SPECIAL_CODE       BlockRange::BLOCKTYPE_DEPLOYMENT
#define MEMORY_BLOCKTYPE_GCC_BOOTSTRAP_CODE     BlockRange::BLOCKTYPE_CODE
#endif


//--//

#if defined(BUILD_RTM)
        #define MEMORY_BLOCKTYPE_SPECIAL  BlockRange::BLOCKTYPE_DEPLOYMENT
#else
        #define MEMORY_BLOCKTYPE_SPECIAL  BlockRange::BLOCKTYPE_FILESYSTEM
#endif

//--//


const BlockRange g_I28F640J3_16_BlockRange[] =
{
    { BlockRange::BLOCKTYPE_BOOTSTRAP    ,  0,  0 },   // TinyBooter
    { MEMORY_BLOCKTYPE_GCC_BOOTSTRAP_CODE,  1,  1 },   // TinyCLR runtime
    { BlockRange::BLOCKTYPE_CODE         ,  2, 13 },   // TinyCLR runtime

    { MEMORY_BLOCKTYPE_GCC_SPECIAL_CODE, 14, 18 },   // TinyCLR runtime 
    { BlockRange::BLOCKTYPE_DEPLOYMENT , 19, 30 },   // TinyCLR runtime
    { BlockRange::BLOCKTYPE_DEPLOYMENT , 31, 40 },   // TinyCLR runtime
    { BlockRange::BLOCKTYPE_DEPLOYMENT , 41, 50 },   // TinyCLR runtime

    { MEMORY_BLOCKTYPE_SPECIAL         , 51, 60 },        
    { BlockRange::BLOCKTYPE_STORAGE_A  , 61, 61 },    
    { BlockRange::BLOCKTYPE_STORAGE_B  , 62, 62 }, 
    { BlockRange::BLOCKTYPE_CONFIG     , 63, 63 }    // g_ConfigurationSector
};


const BlockRegionInfo  g_I28F640_J3_16_BlkRegion[I28F640J3_16__NUM_REGIONS] = 
{
    {
        I28F640J3_16__BASE_ADDRESS,         // ByteAddress   Start;              // Starting Sector address
        FLASH_BLOCK_COUNT,                  // UINT32        NumBlocks;          // total number of blocks in this region
        FLASH_BYTES_PER_BLOCK,              // UINT32        BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)

        ARRAYSIZE_CONST_EXPR(g_I28F640J3_16_BlockRange),
        g_I28F640J3_16_BlockRange,
    },
};

//--//

#undef MEMORY_BLOCKTYPE_GCC_SPECIAL_CODE 

//--//


BlockDeviceInfo g_I28F640J3_16_DeviceInfo=
{
    {  
        I28F640J3_16__IS_REMOVABLE,         // BOOL Removable;
        I28F640J3_16__SUPPORTS_XIP,         // BOOL SupportsXIP;
        I28F640J3_16__WRITE_PROTECTED,
    },
    FLASH_SECTOR_WRITE_TYPICAL_TIME_USEC,   // UINT32 Duration_Max_WordWrite_uSec;
    FLASH_BLOCK_ERASE_ACTUAL_TIME_USEC,     // UINT32 Duration_Max_SectorErase_uSec;
    FLASH_BYTES_PER_SECTOR,                 // UINT32 BytesPerSector;     

    FLASH_MEMORY_Size,                      // UINT32 Size;

    I28F640J3_16__NUM_REGIONS,              // UINT32 NumRegions;
    g_I28F640_J3_16_BlkRegion,              //const BlockRegionInfo* pRegions;
};


struct MEMORY_MAPPED_NOR_BLOCK_CONFIG g_I28F640J3_16_BS_Config =
{
    { // BLOCK_CONFIG
        {
            I28F640J3_16__WP_GPIO_PIN,      // GPIO_PIN             Pin;
            I28F640J3_16__WP_ACTIVE,        // BOOL                 ActiveState;
        },

        &g_I28F640J3_16_DeviceInfo,         // BlockDeviceinfo
    },

    { // CPU_MEMORY_CONFIG
        I28F640J3_16__CHIP_SELECT,          // UINT8  CPU_MEMORY_CONFIG::ChipSelect;
        TRUE,                               // UINT8  CPU_MEMORY_CONFIG::ReadOnly;
        I28F640J3_16__WAIT_STATES,          // UINT32 CPU_MEMORY_CONFIG::WaitStates;
        I28F640J3_16__RELEASE_COUNTS,       // UINT32 CPU_MEMORY_CONFIG::ReleaseCounts;
        I28F640J3_16__BIT_WIDTH,            // UINT32 CPU_MEMORY_CONFIG::BitWidth;
        I28F640J3_16__BASE_ADDRESS,         // UINT32 CPU_MEMORY_CONFIG::BaseAddress;
        I28F640J3_16__SIZE_IN_BYTES,        // UINT32 CPU_MEMORY_CONFIG::SizeInBytes;
        0,                                  // UINT8                CPU_MEMORY_CONFIG::XREADYEnable 
        0,                                  // UINT8                CPU_MEMORY_CONFIG::ByteSignalsForRead 
        0,                                  // UINT8                CPU_MEMORY_CONFIG::ExternalBufferEnable
    },

    0,                                      // UINT32  ChipProtection;
    FLASH_MANUFACTURER_CODE,                // FLASH_WORD ManufacturerCode;
    FLASH_DEVICE_CODE,                      // FLASH_WORD DeviceCode;
};

//--//


#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata = "g_I28F640_16_BS"
#endif

struct BlockStorageDevice g_I28F640_16_BS;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata 
#endif 

//--//

