////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>


//--//

#define FLASH_MANUFACTURER_CODE                0x0001  
#define FLASH_DEVICE_CODE                       0x227E 
#define FLASH_BLOCK_COUNT                       24
#define FLASH_BLOCK_ERASE_TYPICAL_TIME_USEC   700000
#define FLASH_SECTOR_WRITE_TYPICAL_TIME_USEC 210
#define FLASH_BLOCK_ERASE_MAX_TIME_USEC       12000000
#define FLASH_SECTOR_WRITE_MAX_TIME_USEC      220

//--//

#define INTERNALFLASH__CHIP_SELECT      0
#define INTERNALFLASH__WAIT_STATES      0 
#define INTERNALFLASH__RELEASE_COUNTS   0
#define INTERNALFLASH__BIT_WIDTH        8 
#define INTERNALFLASH__BASE_ADDRESS     FLASH_MEMORY_Base
#define INTERNALFLASH__SIZE_IN_BYTES    FLASH_MEMORY_Size
#define INTERNALFLASH__WP_GPIO_PIN      GPIO_PIN_NONE
#define INTERNALFLASH__WP_ACTIVE        FALSE


#if defined(MEMORY_BLOCKTYPE_GCC_SPECIAL_BOOTSTRAP)
#undef MEMORY_BLOCKTYPE_GCC_SPECIAL_BOOTSTRAP
#endif

#if defined(MEMORY_BLOCKTYPE_GCC_SPECIAL_CODE)
#undef MEMORY_BLOCKTYPE_GCC_SPECIAL_CODE
#endif

#ifdef __GNUC__
#define MEMORY_BLOCKTYPE_GCC_SPECIAL_BOOTSTRAP BlockRange::BLOCKTYPE_BOOTSTRAP
#define MEMORY_BLOCKTYPE_GCC_SPECIAL_CODE      BlockRange::BLOCKTYPE_CODE
#else
#define MEMORY_BLOCKTYPE_GCC_SPECIAL_BOOTSTRAP BlockRange::BLOCKTYPE_CODE
#define MEMORY_BLOCKTYPE_GCC_SPECIAL_CODE      BlockRange::BLOCKTYPE_DEPLOYMENT
#endif


// -- // device information

// BlockDeviceInformation

#define INTERNALFLASH__IS_REMOVABLE        FALSE
#define INTERNALFLASH__SUPPORTS_XIP        TRUE
#define INTERNALFLASH__WRITE_PROTECTED     FALSE

#define INTERNALFLASH__NUM_REGIONS         4




//--//
const BlockRange g_INTERNALFLASH_BlockRange1[] =
{
    { BlockRange::BLOCKTYPE_BOOTSTRAP       ,  0,  0 },   /* 8KB  (1*8): 00000000 - 00001FFF */
    { BlockRange::BLOCKTYPE_CODE            ,  1, 7 },    /* 56KB (7*8): 00002000 - 0000FFFF */
};

const BlockRange g_INTERNALFLASH_BlockRange2[] =
{
    { BlockRange::BLOCKTYPE_CODE          , 0,  8 },     /* 128KB  (2*64): 00010000 - 0002FFFF */
//    { BlockRange::BLOCKTYPE_CODE       , 2,  4 },      /* 192KB  (3*64): 00030000 - 0005FFFF */
//    { BlockRange::BLOCKTYPE_CODE       , 5,  8 },      /* 192KB  (4*64): 00060000 - 0009FFFF */
};

const BlockRange g_INTERNALFLASH_BlockRange3[] =
{
    { BlockRange::BLOCKTYPE_CODE       , 0,  0 },		/* 128KB  (1*128): 000A0000 - 000BFFFF */   
    { BlockRange::BLOCKTYPE_DEPLOYMENT  , 1,  2 },      /* 256KB  (2*128): 000C0000 - 000FFFFF */
};

const BlockRange g_INTERNALFLASH_BlockRange4[] =
{
    { BlockRange::BLOCKTYPE_CONFIG          , 0,  1 },    /* 16KB  (2*8): 80100000 - 80103FFF */ 
    { BlockRange::BLOCKTYPE_STORAGE_A       , 2,  2 },	  /* 8KB  (1*8): 80104000 - 80105FFF */
	{ BlockRange::BLOCKTYPE_STORAGE_B       , 3,  3 },    /* 8KB  (1*8): 80106000 - 80107FFF */
};

//--//

#undef MEMORY_BLOCKTYPE_GCC_SPECIAL_BOOTSTRAP
#undef MEMORY_BLOCKTYPE_GCC_SPECIAL_CODE

//--//

#define BYTES_PER_SECTOR            2
    
#define REGION1_XIP_ADDRESS         INTERNALFLASH__BASE_ADDRESS
#define REGION1_BYTES_PER_BLK       0x2000
//#define REGION1_SECTORS_PER_BLOCK   REGION1_BYTES_PER_BLK/BYTES_PER_SECTOR
#define REGION1_SECTORS_PER_BLOCK   1
#define REGION1_NUM_OF_BLOCKS       8
#define REGION1_SECTOR_START        0

#define REGION2_XIP_ADDRESS         REGION1_XIP_ADDRESS + REGION1_NUM_OF_BLOCKS* REGION1_BYTES_PER_BLK
#define REGION2_BYTES_PER_BLK       0x10000
//#define REGION2_SECTORS_PER_BLOCK   REGION2_BYTES_PER_BLK/BYTES_PER_SECTOR
#define REGION2_SECTORS_PER_BLOCK   1
#define REGION2_NUM_OF_BLOCKS       9
#define REGION2_SECTOR_START        REGION1_SECTOR_START + REGION1_NUM_OF_BLOCKS*REGION1_SECTORS_PER_BLOCK

#define REGION3_XIP_ADDRESS         REGION2_XIP_ADDRESS + REGION2_NUM_OF_BLOCKS* REGION2_BYTES_PER_BLK
#define REGION3_BYTES_PER_BLK       0x20000
//#define REGION3_SECTORS_PER_BLOCK   REGION3_BYTES_PER_BLK/BYTES_PER_SECTOR
#define REGION3_SECTORS_PER_BLOCK   1
#define REGION3_NUM_OF_BLOCKS       3
#define REGION3_SECTOR_START        REGION2_SECTOR_START + REGION2_NUM_OF_BLOCKS*REGION2_SECTORS_PER_BLOCK


#define REGION4_XIP_ADDRESS         0x80100000
#define REGION4_BYTES_PER_BLK       0x2000
//#define REGION4_SECTORS_PER_BLOCK   REGION4_BYTES_PER_BLK/BYTES_PER_SECTOR
#define REGION4_SECTORS_PER_BLOCK   1
#define REGION4_NUM_OF_BLOCKS       4
#define REGION4_SECTOR_START        REGION3_SECTOR_START + REGION3_NUM_OF_BLOCKS*REGION3_SECTORS_PER_BLOCK

const BlockRegionInfo  g_INTERNALFLASH_BlkRegion[INTERNALFLASH__NUM_REGIONS] = 
{
    {
        REGION1_XIP_ADDRESS,            // ByteAddress  Start;              // Starting Sector address
        REGION1_NUM_OF_BLOCKS,          // UINT32       NumBlocks;          // total number of blocks in this region
        REGION1_BYTES_PER_BLK,          // UINT32       BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)

        ARRAYSIZE_CONST_EXPR(g_INTERNALFLASH_BlockRange1),
        g_INTERNALFLASH_BlockRange1
    },
    
    {
        REGION2_XIP_ADDRESS,            // SectorAddress   Start;              // Starting Sector address
        REGION2_NUM_OF_BLOCKS,          // UINT32          NumBlocks;          // total number of blocks in this region
        REGION2_BYTES_PER_BLK,          // UINT32          BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)

        ARRAYSIZE_CONST_EXPR(g_INTERNALFLASH_BlockRange2),
        g_INTERNALFLASH_BlockRange2
    },

	{
        REGION3_XIP_ADDRESS,            // SectorAddress   Start;              // Starting Sector address
        REGION3_NUM_OF_BLOCKS,          // UINT32          NumBlocks;          // total number of blocks in this region
        REGION3_BYTES_PER_BLK,          // UINT32          BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)

        ARRAYSIZE_CONST_EXPR(g_INTERNALFLASH_BlockRange3),
        g_INTERNALFLASH_BlockRange3
    },

	{
        REGION4_XIP_ADDRESS,            // SectorAddress   Start;              // Starting Sector address
        REGION4_NUM_OF_BLOCKS,          // UINT32          NumBlocks;          // total number of blocks in this region
        REGION4_BYTES_PER_BLK,          // UINT32          BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)

        ARRAYSIZE_CONST_EXPR(g_INTERNALFLASH_BlockRange4),
        g_INTERNALFLASH_BlockRange4
    },

};

//--//


BlockDeviceInfo g_INTERNALFLASH_DeviceInfo=
{
    {
        INTERNALFLASH__IS_REMOVABLE,          // BOOL Removable;
        INTERNALFLASH__SUPPORTS_XIP,          // BOOL SupportsXIP;
        INTERNALFLASH__WRITE_PROTECTED        // BOOL WRiteProtected
    },
    FLASH_SECTOR_WRITE_MAX_TIME_USEC,        // UINT32 Duration_Max_WordWrite_uSec;
    FLASH_BLOCK_ERASE_MAX_TIME_USEC,         // UINT32 Duration_Max_SectorErase_uSec;
    BYTES_PER_SECTOR,                        // UINT32 BytesPerSector; // Bytes Per Sector

    FLASH_MEMORY_Size,                       // UINT32 Size;

    INTERNALFLASH__NUM_REGIONS,               // UINT32 NumRegions;
    g_INTERNALFLASH_BlkRegion,            // const BlockRegionInfo* pRegions;
};


struct MEMORY_MAPPED_NOR_BLOCK_CONFIG g_INTERNALFLASH_Config =
{
    { // BLOCK_CONFIG
        {
            INTERNALFLASH__WP_GPIO_PIN,      // GPIO_PIN             Pin;
            INTERNALFLASH__WP_ACTIVE,        // BOOL                 ActiveState;
        },

        &g_INTERNALFLASH_DeviceInfo,    // BlockDeviceinfo
    },

    { // CPU_MEMORY_CONFIG
        INTERNALFLASH__CHIP_SELECT,          // UINT8                CPU_MEMORY_CONFIG::ChipSelect;
        TRUE,                               // UINT8                CPU_MEMORY_CONFIG::ReadOnly;
        INTERNALFLASH__WAIT_STATES,          // UINT32               CPU_MEMORY_CONFIG::WaitStates;
        INTERNALFLASH__RELEASE_COUNTS,       // UINT32               CPU_MEMORY_CONFIG::ReleaseCounts;
        INTERNALFLASH__BIT_WIDTH,            // UINT32               CPU_MEMORY_CONFIG::BitWidth;
        INTERNALFLASH__BASE_ADDRESS,         // UINT32               CPU_MEMORY_CONFIG::BaseAddress;
        INTERNALFLASH__SIZE_IN_BYTES,        // UINT32               CPU_MEMORY_CONFIG::SizeInBytes;
        0,                                  // UINT8                CPU_MEMORY_CONFIG::XREADYEnable 
        0,                                  // UINT8                CPU_MEMORY_CONFIG::ByteSignalsForRead 
        0,                                  // UINT8                CPU_MEMORY_CONFIG::ExternalBufferEnable
    },

    0,                                      // UINT32               ChipProtection;
    FLASH_MANUFACTURER_CODE,                // FLASH_WORD           ManufacturerCode;
    FLASH_DEVICE_CODE,                      // FLASH_WORD           DeviceCode;
};

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata = "g_INTERNALFLASH"
#endif

struct BlockStorageDevice g_INTERNALFLASH;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata 
#endif 

//--//

