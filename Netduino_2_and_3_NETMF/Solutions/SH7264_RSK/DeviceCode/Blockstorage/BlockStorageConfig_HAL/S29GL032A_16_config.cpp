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
#include <Drivers\BlockStorage\Flash\AM29DL_16\AM29DL_16.h>


//--//

#define FLASH_MANUFACTURER_CODE                0x0001
#define FLASH_DEVICE_CODE                       0x2223//0x227E
#define FLASH_BLOCK_COUNT                       512
#define FLASH_BLOCK_ERASE_TYPICAL_TIME_USEC   700000
#define FLASH_SECTOR_WRITE_TYPICAL_TIME_USEC 210
#define FLASH_BLOCK_ERASE_MAX_TIME_USEC       12000000
#define FLASH_SECTOR_WRITE_MAX_TIME_USEC      220

//--//

#define S29GL032A_16__CHIP_SELECT      0
#define S29GL032A_16__WAIT_STATES      0 
#define S29GL032A_16__RELEASE_COUNTS   0
#define S29GL032A_16__BIT_WIDTH        32 
#define S29GL032A_16__BASE_ADDRESS     FLASH_MEMORY_Base
#define S29GL032A_16__SIZE_IN_BYTES    FLASH_MEMORY_Size
#define S29GL032A_16__WP_GPIO_PIN      GPIO_PIN_NONE
#define S29GL032A_16__WP_ACTIVE        FALSE


// -- // device information

// BlockDeviceInformation

#define S29GL032A_16__IS_REMOVABLE        FALSE
#define S29GL032A_16__SUPPORTS_XIP        TRUE
#define S29GL032A_16__WRITE_PROTECTED     FALSE

#define S29GL032A_16__NUM_REGIONS         1
//--//

#ifndef SH7264_SERIAL_FLASH //Boot from Nor flash
const BlockRange g_S29GL032A_16_BlockRange1[] =
{
    { BlockRange::BLOCKTYPE_BOOTSTRAP       , 0,  0 },       // SA0    - Bootloader - Reserve 128K for Compressed Flashlite/TinyBooter (0-7FFF)
    { BlockRange::BLOCKTYPE_CODE            , 1,  30 },       // SA1-30 - ER_FLASH (20020000-203DFFFF)
    { BlockRange::BLOCKTYPE_CONFIG          , 31, 31 },       // SA31-- ER_Config(203E0000)
    { BlockRange::BLOCKTYPE_STORAGE_A       , 32, 32 },       // SA32- - Storage A(20400000)
    { BlockRange::BLOCKTYPE_STORAGE_B       , 33, 33 },       // SA33- - Storage B(20420000)
    { BlockRange::BLOCKTYPE_SIMPLE_A        , 34, 34 },       // for cert storage
    { BlockRange::BLOCKTYPE_SIMPLE_B        , 35, 35 },       // for cert storage
    { BlockRange::BLOCKTYPE_DEPLOYMENT      , 36, 40 },
    { BlockRange::BLOCKTYPE_DEPLOYMENT      , 41, 50 },
    { BlockRange::BLOCKTYPE_DEPLOYMENT      , 51, 60 },
    { BlockRange::BLOCKTYPE_RESERVED       , 61, 511 },       // SA48-62- DEPLOYMENT()

};
#else//Boot from Serial flash
const BlockRange g_S29GL032A_16_BlockRange1[] =
{
    { BlockRange::BLOCKTYPE_FILESYSTEM       ,  0,  511},
};
#endif

//--//

#define BYTES_PER_SECTOR            2
    
#define REGION1_XIP_ADDRESS         S29GL032A_16__BASE_ADDRESS
#define REGION1_BYTES_PER_BLK       0x20000
#define REGION1_SECTORS_PER_BLOCK   REGION1_BYTES_PER_BLK/BYTES_PER_SECTOR

#define REGION1_NUM_OF_BLOCKS       512

#define REGION1_SECTOR_START        0


const BlockRegionInfo  g_S29GL032A_16_BlkRegion[S29GL032A_16__NUM_REGIONS] = 
{

    REGION1_XIP_ADDRESS,            // ByteAddress  Start;              // Starting Sector address
    REGION1_NUM_OF_BLOCKS,          // UINT32       NumBlocks;          // total number of blocks in this region
    REGION1_BYTES_PER_BLK,          // UINT32       BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)

    ARRAYSIZE_CONST_EXPR(g_S29GL032A_16_BlockRange1),
    g_S29GL032A_16_BlockRange1

};

//--//


BlockDeviceInfo g_S29GL032A_16_DeviceInfo=
{
    {
        S29GL032A_16__IS_REMOVABLE,          // BOOL Removable;
        S29GL032A_16__SUPPORTS_XIP,          // BOOL SupportsXIP;
        S29GL032A_16__WRITE_PROTECTED        // BOOL WRiteProtected
    },
    FLASH_SECTOR_WRITE_MAX_TIME_USEC,        // UINT32 Duration_Max_WordWrite_uSec;
    FLASH_BLOCK_ERASE_MAX_TIME_USEC,         // UINT32 Duration_Max_SectorErase_uSec;
    BYTES_PER_SECTOR,                        // UINT32 BytesPerSector; // Bytes Per Sector
    FLASH_MEMORY_Size,                       // UINT32 Size;
    S29GL032A_16__NUM_REGIONS,               // UINT32 NumRegions;
    g_S29GL032A_16_BlkRegion,            // const BlockRegionInfo* pRegions;
};


//struct MEMORY_MAPPED_NOR_BLOCK_CONFIG g_S29GL032A_16_BS_Config =
struct MEMORY_MAPPED_NOR_BLOCK_CONFIG g_BS_Config_SH7264_RSK =
{
    { // BLOCK_CONFIG
        {
            S29GL032A_16__WP_GPIO_PIN,      // GPIO_PIN             Pin;
            S29GL032A_16__WP_ACTIVE,        // BOOL                 ActiveState;
        },

        &g_S29GL032A_16_DeviceInfo,    // BlockDeviceinfo
    },

    { // CPU_MEMORY_CONFIG
        S29GL032A_16__CHIP_SELECT,          // UINT8                CPU_MEMORY_CONFIG::ChipSelect;
        TRUE,                               // UINT8                CPU_MEMORY_CONFIG::ReadOnly;
        S29GL032A_16__WAIT_STATES,          // UINT32               CPU_MEMORY_CONFIG::WaitStates;
        S29GL032A_16__RELEASE_COUNTS,       // UINT32               CPU_MEMORY_CONFIG::ReleaseCounts;
        S29GL032A_16__BIT_WIDTH,            // UINT32               CPU_MEMORY_CONFIG::BitWidth;
        S29GL032A_16__BASE_ADDRESS,         // UINT32               CPU_MEMORY_CONFIG::BaseAddress;
        S29GL032A_16__SIZE_IN_BYTES,        // UINT32               CPU_MEMORY_CONFIG::SizeInBytes;
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
#pragma arm section rodata = "g_SH7264_BS";//"g_S29GL032A_16_BS"
#endif

struct BlockStorageDevice g_SH7264_BS;//g_S29GL032A_16_BS;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata 
#endif 

//--//

