//-----------------------------------------------------------------------------
// Software that is described herein is for illustrative purposes only  
// which provides customers with programming information regarding the  
// products. This software is supplied "AS IS" without any warranties.  
// NXP Semiconductors assumes no responsibility or liability for the 
// use of the software, conveys no license or title under any patent, 
// copyright, or mask work right to the product. NXP Semiconductors 
// reserves the right to make changes in the software without 
// notification. NXP Semiconductors also make no representation or 
// warranty that such application will be suitable for the specified 
// use without further testing or modification. 
//-----------------------------------------------------------------------------

#include <tinyhal.h>
#include <Drivers\BlockStorage\Flash\AM29DL_32\AM29DL_32.h>

//--//

#define FLASH_MANUFACTURER_CODE              0x00010001
#define FLASH_DEVICE_CODE                    0x22DA22DA
#define FLASH_BLOCK_COUNT                    19
#define FLASH_BLOCK_ERASE_TYPICAL_TIME_USEC  700000
#define FLASH_SECTOR_WRITE_TYPICAL_TIME_USEC 210
#define FLASH_BLOCK_ERASE_MAX_TIME_USEC      12000000
#define FLASH_SECTOR_WRITE_MAX_TIME_USEC     220

//--//

#define AM29LV800BT_32__CHIP_SELECT      0
#define AM29LV800BT_32__WAIT_STATES      0 
#define AM29LV800BT_32__RELEASE_COUNTS   0
#define AM29LV800BT_32__BIT_WIDTH        32 
#define AM29LV800BT_32__BASE_ADDRESS     FLASH_MEMORY_Base
#define AM29LV800BT_32__SIZE_IN_BYTES    FLASH_MEMORY_Size
#define AM29LV800BT_32__WP_GPIO_PIN      GPIO_PIN_NONE
#define AM29LV800BT_32__WP_ACTIVE        FALSE


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

#define AM29LV800BT_32__IS_REMOVABLE        FALSE
#define AM29LV800BT_32__SUPPORTS_XIP        TRUE
#define AM29LV800BT_32__WRITE_PROTECTED     FALSE

#define AM29LV800BT_32__NUM_REGIONS         4




//--//
const BlockRange g_AM29LV800BT_32_BlockRange1[] =
{
    //
    // Start Address of block, byte length, (TRUE - used, FALSE - unused)
    //
    { BlockRange::BLOCKTYPE_BOOTSTRAP       ,  0,  0 },       // SA0  - Bootloader - Reserve 128K for Compressed Flashlite/TinyBooter
    { MEMORY_BLOCKTYPE_GCC_SPECIAL_BOOTSTRAP,  1,  1 },       // SA1  - Bootloader GCC - Reserve 128K for Compressed Flashlite/TinyBooter
    { BlockRange::BLOCKTYPE_CODE            ,  2,  7 },       // SA2  - ER_FLASH
    { MEMORY_BLOCKTYPE_GCC_SPECIAL_CODE     ,  8,  9 },       // SA8  - DEPLOYMENT
    { BlockRange::BLOCKTYPE_DEPLOYMENT      , 10, 14 },       // SA10 - DEPLOYMENT
};

const BlockRange g_AM29LV800BT_32_BlockRange2[] =
{
    { BlockRange::BLOCKTYPE_DEPLOYMENT      ,  0,  0 }        // SA15 - DEPLOYMENT
};

const BlockRange g_AM29LV800BT_32_BlockRange3[] =
{
    { BlockRange::BLOCKTYPE_STORAGE_A       ,  0,  0 },       // SA16 - Storage A
    { BlockRange::BLOCKTYPE_STORAGE_B       ,  1,  1 },       // SA17 - Storage B
};

const BlockRange g_AM29LV800BT_32_BlockRange4[] =
{
    { BlockRange::BLOCKTYPE_CONFIG          ,  0,  0 },       // SA18 - Config
};                                                                           

//--//

#undef MEMORY_BLOCKTYPE_GCC_SPECIAL_BOOTSTRAP
#undef MEMORY_BLOCKTYPE_GCC_SPECIAL_CODE

//--//

#define BYTES_PER_SECTOR            4
    
#define REGION1_XIP_ADDRESS         AM29LV800BT_32__BASE_ADDRESS
#define REGION1_BYTES_PER_BLK       0x20000
#define REGION1_SECTORS_PER_BLOCK   REGION1_BYTES_PER_BLK/BYTES_PER_SECTOR
#define REGION1_NUM_OF_BLOCKS       15
#define REGION1_SECTOR_START        0

#define REGION2_XIP_ADDRESS         REGION1_XIP_ADDRESS + REGION1_NUM_OF_BLOCKS* REGION1_BYTES_PER_BLK
#define REGION2_BYTES_PER_BLK       0x10000
#define REGION2_SECTORS_PER_BLOCK   REGION2_BYTES_PER_BLK/BYTES_PER_SECTOR
#define REGION2_NUM_OF_BLOCKS       1
#define REGION2_SECTOR_START        REGION1_SECTOR_START + REGION1_NUM_OF_BLOCKS*REGION1_SECTORS_PER_BLOCK

#define REGION3_XIP_ADDRESS         REGION2_XIP_ADDRESS + REGION2_NUM_OF_BLOCKS* REGION2_BYTES_PER_BLK
#define REGION3_BYTES_PER_BLK       0x04000
#define REGION3_SECTORS_PER_BLOCK   REGION3_BYTES_PER_BLK/BYTES_PER_SECTOR
#define REGION3_NUM_OF_BLOCKS       2
#define REGION3_SECTOR_START        REGION2_SECTOR_START + REGION2_NUM_OF_BLOCKS* REGION2_SECTORS_PER_BLOCK

#define REGION4_XIP_ADDRESS         REGION3_XIP_ADDRESS + REGION3_NUM_OF_BLOCKS* REGION3_BYTES_PER_BLK
#define REGION4_BYTES_PER_BLK       0x08000
#define REGION4_SECTORS_PER_BLOCK   REGION4_BYTES_PER_BLK/BYTES_PER_SECTOR
#define REGION4_NUM_OF_BLOCKS       1
#define REGION4_SECTOR_START        REGION3_SECTOR_START + (UINT32)REGION3_NUM_OF_BLOCKS* (UINT32)REGION3_SECTORS_PER_BLOCK




const BlockRegionInfo  g_AM29LV800BT_32_BlkRegion[AM29LV800BT_32__NUM_REGIONS] = 
{
    {
        REGION1_XIP_ADDRESS,            // ByteAddress  Start;              // Starting Sector address
        REGION1_NUM_OF_BLOCKS,          // UINT32       NumBlocks;          // total number of blocks in this region
        REGION1_BYTES_PER_BLK,          // UINT32       BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)

        ARRAYSIZE_CONST_EXPR(g_AM29LV800BT_32_BlockRange1),
        g_AM29LV800BT_32_BlockRange1
    },
    
    {
        REGION2_XIP_ADDRESS,            // SectorAddress   Start;              // Starting Sector address
        REGION2_NUM_OF_BLOCKS,          // UINT32          NumBlocks;          // total number of blocks in this region
        REGION2_BYTES_PER_BLK,          // UINT32          BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)

        ARRAYSIZE_CONST_EXPR(g_AM29LV800BT_32_BlockRange2),
        g_AM29LV800BT_32_BlockRange2
    },

    {
        REGION3_XIP_ADDRESS,            // SectorAddress   Start;              // Starting Sector address
        REGION3_NUM_OF_BLOCKS,          // UINT32          NumBlocks;          // total number of blocks in this region
        REGION3_BYTES_PER_BLK,          // UINT32          BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)

        ARRAYSIZE_CONST_EXPR(g_AM29LV800BT_32_BlockRange3),
        g_AM29LV800BT_32_BlockRange3
    },

    {
        REGION4_XIP_ADDRESS,            // SectorAddress   Start;              // Starting Sector address
        REGION4_NUM_OF_BLOCKS,          // UINT32          NumBlocks;          // total number of blocks in this region
        REGION4_BYTES_PER_BLK,          // UINT32          BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)

        ARRAYSIZE_CONST_EXPR(g_AM29LV800BT_32_BlockRange4),
        g_AM29LV800BT_32_BlockRange4
    },

};

//--//


BlockDeviceInfo g_AM29LV800BT_32_DeviceInfo=
{
    {
        AM29LV800BT_32__IS_REMOVABLE,           // BOOL Removable;
        AM29LV800BT_32__SUPPORTS_XIP,           // BOOL SupportsXIP;
        AM29LV800BT_32__WRITE_PROTECTED         // BOOL WRiteProtected
    },
    FLASH_SECTOR_WRITE_MAX_TIME_USEC,           // UINT32 Duration_Max_WordWrite_uSec;
    FLASH_BLOCK_ERASE_MAX_TIME_USEC,            // UINT32 Duration_Max_SectorErase_uSec;
    BYTES_PER_SECTOR,                           // UINT32 BytesPerSector; // Bytes Per Sector

    FLASH_MEMORY_Size,                          // UINT32 Size;

    AM29LV800BT_32__NUM_REGIONS,                // UINT32 NumRegions;
    g_AM29LV800BT_32_BlkRegion,                 // const BlockRegionInfo* pRegions;
};


struct MEMORY_MAPPED_NOR_BLOCK_CONFIG g_AM29LV800BT_32_BS_Config =
{
    { // BLOCK_CONFIG
        {
            AM29LV800BT_32__WP_GPIO_PIN,    // GPIO_PIN             Pin;
            AM29LV800BT_32__WP_ACTIVE,      // BOOL                 ActiveState;
        },

        &g_AM29LV800BT_32_DeviceInfo,       // BlockDeviceinfo
    },

    { // CPU_MEMORY_CONFIG
        AM29LV800BT_32__CHIP_SELECT,        // UINT8                CPU_MEMORY_CONFIG::ChipSelect;
        TRUE,                               // UINT8                CPU_MEMORY_CONFIG::ReadOnly;
        AM29LV800BT_32__WAIT_STATES,        // UINT32               CPU_MEMORY_CONFIG::WaitStates;
        AM29LV800BT_32__RELEASE_COUNTS,     // UINT32               CPU_MEMORY_CONFIG::ReleaseCounts;
        AM29LV800BT_32__BIT_WIDTH,          // UINT32               CPU_MEMORY_CONFIG::BitWidth;
        AM29LV800BT_32__BASE_ADDRESS,       // UINT32               CPU_MEMORY_CONFIG::BaseAddress;
        AM29LV800BT_32__SIZE_IN_BYTES,      // UINT32               CPU_MEMORY_CONFIG::SizeInBytes;
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
#pragma arm section rodata = "g_AM29LV800BT_32_BS"
#endif

struct BlockStorageDevice g_AM29LV800BT_32_BS;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata 
#endif 

//--//

