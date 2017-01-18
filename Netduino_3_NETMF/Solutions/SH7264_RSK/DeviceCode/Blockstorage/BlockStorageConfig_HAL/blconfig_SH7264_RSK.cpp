////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#define FLASH_MANUFACTURER_CODE              0x0089
#define FLASH_DEVICE_CODE                    0x0017
#define FLASH_BLOCK_COUNT                    64
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

#define BLCONFIG_SH7264_M3A_HS64_CHIP_SELECT       0
#define BLCONFIG_SH7264_M3A_HS64_WAIT_STATES       7                    /* New freescale board uses 75 nS parts */
#define BLCONFIG_SH7264_M3A_HS64_RELEASE_COUNTS    0
#define BLCONFIG_SH7264_M3A_HS64_BIT_WIDTH         (16 | (1 << 16))     /* Data is on D15-D0 on the freescale boards  */
#define BLCONFIG_SH7264_M3A_HS64_BASE_ADDRESS      FLASH_MEMORY_Base
#define BLCONFIG_SH7264_M3A_HS64_SIZE_IN_BYTES     8*1024*1024
#define BLCONFIG_SH7264_M3A_HS64_BYTES_PER_SECTOR  2
#define BLCONFIG_SH7264_M3A_HS64_BYTES_PER_BLOCK   0x20000
#define BLCONFIG_SH7264_M3A_HS64_WP_GPIO_PIN       GPIO_PIN_NONE
#define BLCONFIG_SH7264_M3A_HS64_WP_ACTIVE         FALSE



//--//

#define BLCONFIG_SH7264_M3A_HS64_FLASH_BOTTOM_PARAMETER 1
#undef  BLCONFIG_SH7264_M3A_HS64_FLASH_TOP_PARAMETER



// BlockDeviceInformation

#define BLCONFIG_SH7264_M3A_HS64_REMOVEABLE     FALSE
#define BLCONFIG_SH7264_M3A_HS64_SUPPORTXIP     TRUE
#define BLCONFIG_SH7264_M3A_HS64_WRITEPROTECTED FALSE
#define BLCONFIG_SH7264_M3A_HS64_NUMREGIONS     1


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
#define MEMORY_BLOCKTYPE_GCC_SPECIAL_BOOTSTRAP  BlockRange::BLOCKTYPE_BOOTSTRAP
#else
#define MEMORY_BLOCKTYPE_GCC_SPECIAL_CODE       BlockRange::BLOCKTYPE_DEPLOYMENT
#define MEMORY_BLOCKTYPE_GCC_SPECIAL_BOOTSTRAP  BlockRange::BLOCKTYPE_CODE
#endif




//--//


const BlockRange g_BLCONFIG_SH7264_M3A_HS64_BlockRange[] =
{
    { BlockRange::BLOCKTYPE_BOOTSTRAP       ,  0,  0 },     // Bootloader
    { MEMORY_BLOCKTYPE_GCC_SPECIAL_BOOTSTRAP,  1,  1 },     // Bootloader
    { BlockRange::BLOCKTYPE_CODE            ,  2,  35 },     // TinyCLR runtime           
    { MEMORY_BLOCKTYPE_GCC_SPECIAL_CODE     ,  36, 38 },     // TinyCLR runtime 
    { BlockRange::BLOCKTYPE_DEPLOYMENT      , 38, 58 },     // VS Deployment
    { BlockRange::BLOCKTYPE_SIMPLE_A        , 59, 59 },     // Simple native storage (A)
    { BlockRange::BLOCKTYPE_SIMPLE_B        , 60, 60 },     // Simple native storage (B)
    { BlockRange::BLOCKTYPE_STORAGE_A       , 61, 61 },     // data storage - Extended Weak References  
    { BlockRange::BLOCKTYPE_STORAGE_B       , 62, 62 },     // data storage - Extended Weak References
    { BlockRange::BLOCKTYPE_CONFIG          , 63, 63 },     // Device config data
};


const BlockRegionInfo  g_BlkRegion_SH7264_M3A_HS64[BLCONFIG_SH7264_M3A_HS64_NUMREGIONS] = 
{
    BLCONFIG_SH7264_M3A_HS64_BASE_ADDRESS,     // ByteAddress     Address;         
    FLASH_BLOCK_COUNT,              // UINT32          NumBlocks;          // total number of blocks in this region
    BLCONFIG_SH7264_M3A_HS64_BYTES_PER_BLOCK,  // UINT32          BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)

    ARRAYSIZE_CONST_EXPR(g_BLCONFIG_SH7264_M3A_HS64_BlockRange),
    g_BLCONFIG_SH7264_M3A_HS64_BlockRange,
};

//--//

#undef MEMORY_BLOCKTYPE_GCC_SPECIAL_CODE 

//--//


BlockDeviceInfo g_BLCONFIG_SH7264_M3A_HS64_DeviceInfo=
{
    {  
        BLCONFIG_SH7264_M3A_HS64_REMOVEABLE,           // BOOL Removable;
        BLCONFIG_SH7264_M3A_HS64_SUPPORTXIP,           // BOOL SupportsXIP;
        BLCONFIG_SH7264_M3A_HS64_WRITEPROTECTED,
    },
    FLASH_SECTOR_WRITE_TYPICAL_TIME_USEC,   // UINT32 Duration_Max_WordWrite_uSec;
    FLASH_BLOCK_ERASE_ACTUAL_TIME_USEC,     // UINT32 Duration_Max_SectorErase_uSec;
    BLCONFIG_SH7264_M3A_HS64_BYTES_PER_SECTOR,         // BytesPerSector; // Bytes Per Sector

    FLASH_MEMORY_Size,                      // UINT32 Size;

    BLCONFIG_SH7264_M3A_HS64_NUMREGIONS,               // UINT32 NumRegions;
    g_BlkRegion_SH7264_M3A_HS64,                      // const BlockRegionInfo* pRegions;
};


struct MEMORY_MAPPED_NOR_BLOCK_CONFIG g_BS_Config_SH7264_M3A_HS64 =
{
    { // BLOCK_CONFIG
        {
            BLCONFIG_SH7264_M3A_HS64_WP_GPIO_PIN,      // GPIO_PIN             Pin;
            BLCONFIG_SH7264_M3A_HS64_WP_ACTIVE,        // BOOL                 ActiveState;
        },

        &g_BLCONFIG_SH7264_M3A_HS64_DeviceInfo,        // BlockDeviceinfo
    },

    { // CPU_MEMORY_CONFIG
        BLCONFIG_SH7264_M3A_HS64_CHIP_SELECT,          // UINT8  CPU_MEMORY_CONFIG::ChipSelect;
        TRUE,                               // UINT8  CPU_MEMORY_CONFIG::ReadOnly;
        BLCONFIG_SH7264_M3A_HS64_WAIT_STATES,          // UINT32 CPU_MEMORY_CONFIG::WaitStates;
        BLCONFIG_SH7264_M3A_HS64_RELEASE_COUNTS,       // UINT32 CPU_MEMORY_CONFIG::ReleaseCounts;
        BLCONFIG_SH7264_M3A_HS64_BIT_WIDTH,            // UINT32 CPU_MEMORY_CONFIG::BitWidth;
        BLCONFIG_SH7264_M3A_HS64_BASE_ADDRESS,         // UINT32 CPU_MEMORY_CONFIG::BaseAddress;
        BLCONFIG_SH7264_M3A_HS64_SIZE_IN_BYTES,        // UINT32 CPU_MEMORY_CONFIG::SizeInBytes;
        0,                                  // UINT8                CPU_MEMORY_CONFIG::XREADYEnable 
        0,                                  // UINT8                CPU_MEMORY_CONFIG::ByteSignalsForRead 
        0,                                  // UINT8                CPU_MEMORY_CONFIG::ExternalBufferEnable
    },

    0,                                      // UINT32  ChipProtection;
    FLASH_MANUFACTURER_CODE,                // FLASH_WORD ManufacturerCode;
    FLASH_DEVICE_CODE,                      // FLASH_WORD DeviceCode;
};


#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata = "g_SH7264_M3A_HS64_BS"
#endif

struct BlockStorageDevice g_SH7264_BS;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata 
#endif 


//--//

