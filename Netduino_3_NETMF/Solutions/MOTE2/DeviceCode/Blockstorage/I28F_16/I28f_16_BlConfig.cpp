////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#include <Drivers\BlockStorage\Flash\I28F_16\I28F_16.h>

//--//


#define    FLASH_MANUFACTURER_CODE                  0x0089
#define    FLASH_DEVICE_CODE                        0x8810
#define    FLASH_PARTITION_COUNT                    16
#define    FLASH_BLOCK_COUNT_1                      4
#define    FLASH_BLOCK_COUNT_2                      255
#define    FLASH_BYTES_PER_BLOCK_1                  0x08000
#define    FLASH_BYTES_PER_BLOCK_2                  0x20000
#define    FLASH_BYTES_PER_SECTOR                   2
#define    FLASH_BLOCK_ERASE_TYPICAL_TIME_USEC      1200000
#define    FLASH_SECTOR_WRITE_TYPICAL_TIME_USEC     90
#define    FLASH_BLOCK_ERASE_MAX_TIME_USEC          4000000
#define    FLASH_SECTOR_WRITE_MAX_TIME_USEC         180
//
// The real max time should be 4,000,000 uSec, but the chip has independent partitions, it's not critical to avoid radio collisions.
//
#define    FLASH_BLOCK_ERASE_ACTUAL_TIME_USEC       400000



// EBIU Information

//--//

#define PXA271_16__FLASH_BOTTOM_PARAMETER 1
//#undef  PXA271_16__FLASH_TOP_PARAMETER
#define PXA271_16__CHIP_SELECT                       0
#define PXA271_16__WAIT_STATES                       10
#define PXA271_16__RELEASE_COUNTS                    0
#define PXA271_16__BIT_WIDTH                         16                 
#define PXA271_16__BASE_ADDRESS                      0
#define PXA271_16__SIZE_IN_BYTES                     32*1024*1024
#define PXA271_16__WP_GPIO_PIN                       GPIO_PIN_NONE
#define PXA271_16__WP_ACTIVE                         FALSE
//    const UINT32 PartionStartSectors[PARTITION_COUNT] = { 0, 19, 35, 51, 67, 83, 99, 115, 131, 147, 163, 179, 195, 211, 227, 243 };


//--//

// BlockDeviceInformation

#define PXA271_16__IS_REMOVABLE     FALSE
#define PXA271_16__SUPPORTS_XIP     TRUE
#define PXA271_16__WRITE_PROTECTED  FALSE
#define PXA271_16__NUM_REGIONS      2


//--//

#ifdef MEMORY_BLOCKTYPE_SPECIAL
#undef MEMORY_BLOCKTYPE_SPECIAL
#endif


// Deployment area was cut down to 4 Meg because it takes about 0.2 seconds per Meg during deployment to test
// for blank Flash sectors
const BlockRange g_PXA271_16_BlockRange1[] =
{
    { BlockRange::BLOCKTYPE_BOOTSTRAP ,   0,   3 },         // BootLoader Vector Table  0x8000size  
};


const BlockRange g_PXA271_16_BlockRange2[] =
{
    { BlockRange::BLOCKTYPE_BOOTSTRAP,    0,   6 },         // Reserved - SRAM is remapped over the first 10_0000 bytes of flash (include first block range)
    { BlockRange::BLOCKTYPE_BOOTSTRAP ,   7,   9 },         // BootLoader (GCC) 
    { BlockRange::BLOCKTYPE_CODE      ,  10,  16 },         // TinyCLR runtime
    { BlockRange::BLOCKTYPE_DEPLOYMENT,  17,  20 },         // VS Deployment
    { BlockRange::BLOCKTYPE_DEPLOYMENT,  21,  30 },         // VS Deployment
    { BlockRange::BLOCKTYPE_DEPLOYMENT,  31,  40 },         // VS Deployment
    { BlockRange::BLOCKTYPE_DEPLOYMENT,  41,  50 },         // VS Deployment
    { BlockRange::BLOCKTYPE_DEPLOYMENT,  51,  60 },         // VS Deployment
    { BlockRange::BLOCKTYPE_DEPLOYMENT,  61,  70 },         // VS Deployment
    { BlockRange::BLOCKTYPE_DEPLOYMENT,  71,  77 },         // VS Deployment
    { BlockRange::BLOCKTYPE_FILESYSTEM,  78, 251 },         // File System
    { BlockRange::BLOCKTYPE_STORAGE_A , 252, 252 },         // File System
    { BlockRange::BLOCKTYPE_STORAGE_B , 253, 253 },         // File System
    { BlockRange::BLOCKTYPE_CONFIG    , 254, 254 },         // g_ConfigurationSector  
};
//--//


const BlockRegionInfo  g_I28F_16_BlkRegion[PXA271_16__NUM_REGIONS] = 
{
    {
        PXA271_16__BASE_ADDRESS,                    // ByteAddress   Start
        FLASH_BLOCK_COUNT_1,                        // UINT32        NumBlocks;          // total number of blocks in this region
        FLASH_BYTES_PER_BLOCK_1,                    // UINT32        BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)

        ARRAYSIZE_CONST_EXPR(g_PXA271_16_BlockRange1),
        g_PXA271_16_BlockRange1
    },

    {
        PXA271_16__BASE_ADDRESS + (FLASH_BLOCK_COUNT_1 * FLASH_BYTES_PER_BLOCK_1),      // SectorAddress   Start;              // Starting address
        FLASH_BLOCK_COUNT_2,                                                            // UINT32          NumBlocks;          // total number of blocks in this region
        FLASH_BYTES_PER_BLOCK_2,                                                        // UINT32          BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)

        ARRAYSIZE_CONST_EXPR(g_PXA271_16_BlockRange2),
        g_PXA271_16_BlockRange2
    },

    
};

//--//

BlockDeviceInfo g_PXA271_16_DeviceInfo=
{
    {
        PXA271_16__IS_REMOVABLE,                // BOOL Removable;
        PXA271_16__SUPPORTS_XIP,                // BOOL SupportsXIP;
        PXA271_16__WRITE_PROTECTED,             // BOOL Write protected   
    },
    FLASH_SECTOR_WRITE_TYPICAL_TIME_USEC,       // UINT32               Duration_Max_WordWrite_uSec;
    FLASH_BLOCK_ERASE_ACTUAL_TIME_USEC,         // UINT32               Duration_Max_SectorErase_uSec;
    FLASH_BYTES_PER_SECTOR,                     // BytesPerSector; // Bytes Per Sector

    FLASH_MEMORY_Size,                          // UINT32 Size;

    PXA271_16__NUM_REGIONS,                     // UINT32 NumRegions;
    g_I28F_16_BlkRegion,                        // const BlockRegionInfo* pRegions;
};


struct MEMORY_MAPPED_NOR_BLOCK_CONFIG g_PXA271_16_BS_Config =
{
    { // BLOCK_CONFIG
        {
            PXA271_16__WP_GPIO_PIN,         // GPIO_PIN             Pin;
            PXA271_16__WP_ACTIVE,           // BOOL                 ActiveState;
        },

        &g_PXA271_16_DeviceInfo,            // BlockDeviceinfo
    },

    { // CPU_MEMORY_CONFIG
        PXA271_16__CHIP_SELECT,             // UINT8                CPU_MEMORY_CONFIG::ChipSelect;
        TRUE,                               // UINT8                CPU_MEMORY_CONFIG::ReadOnly;
        PXA271_16__WAIT_STATES,             // UINT32               CPU_MEMORY_CONFIG::WaitStates;
        PXA271_16__RELEASE_COUNTS,          // UINT32               CPU_MEMORY_CONFIG::ReleaseCounts;
        PXA271_16__BIT_WIDTH,               // UINT32               CPU_MEMORY_CONFIG::BitWidth;
        PXA271_16__BASE_ADDRESS,            // UINT32               CPU_MEMORY_CONFIG::BaseAddress;
        PXA271_16__SIZE_IN_BYTES,           // UINT32               CPU_MEMORY_CONFIG::SizeInBytes;
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
#pragma arm section rodata = "g_PXA271_16_BS"
#endif

struct BlockStorageDevice g_PXA271_16_BS; 

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata 
#endif 

//--//

