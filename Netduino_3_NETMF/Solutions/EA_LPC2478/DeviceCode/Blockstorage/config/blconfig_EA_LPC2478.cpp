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
#include <Drivers\BlockStorage\Flash\SST39WF_16\SST39WF_16.h>

//--// 
   /****
    * NOTICE:   Sector = min writeable unit = 2 bytes ; Block = min erasable unit = 4KBytes
    *           SST docs call these a Word and a Sector respectively, while they define Block as 64KBytes.
    *           We will refer to 64KBytes Blocks as SST Blocks.
    */


//SST39VF320 flash information
#define     FLASH_MANUFACTURER_CODE                     0x00BF
#define     FLASH_DEVICE_CODE                           0x235B

#define     FLASH_BLOCK_COUNT                           1024
#define     FLASH_BYTES_PER_SECTOR                      2
#define     FLASH_BYTES_PER_BLOCK                       0x1000

#define     FLASH_SECTOR_WRITE_TYPICAL_TIME_USEC        10
#define     FLASH_BLOCK_ERASE_ACTUAL_TIME_USEC          25000
#define     FLASH_SECTOR_WRITE_MAX_TIME_USEC          	10          //WORD as per SST docs, that is a SECTOR
#define     FLASH_BLOCK_ERASE_MAX_TIME_USEC        	25000       //SECTOR as per SST docs, that is a BLOCK


// can be platform dependant for both the chip select and the the sector map
#define SST39VF320_16_CHIP_SELECT                   		0
#define SST39VF320_16_WAIT_STATES                   		0
#define SST39VF320_16_RELEASE_COUNTS                            0
#define SST39VF320_16_BIT_WIDTH                     		16
#define SST39VF320_16_BASE_ADDRESS                  		FLASH_MEMORY_Base
#define SST39VF320_16_SIZE_IN_BYTES                 		FLASH_MEMORY_Size
#define SST39VF320_16__WP_GPIO_PIN                  		GPIO_PIN_NONE
#define SST39VF320_16__WP_ACTIVE                    		FALSE

#define SST39VF320_16_SECTOR_SIZE                   		2   //(in bytes. SST calls this a Word.)
#define SST39VF320_16_BLOCK_SIZE                    		2*1024*2    //(in bytes, 2KWord, where Word = 16bit. SST docs call this a Sector)
#define SST39VF320_16_SECTORS_PER_BLOCK                         SST39VF320_16_BLOCK_SIZE/SST39VF320_16_SECTOR_SIZE

// BlockDeviceInformation

#define SST39VF320_16__IS_REMOVABLE                 		FALSE
#define SST39VF320_16__SUPPORTS_XIP                 		TRUE
#define SST39VF320_16__WRITE_PROTECTED                          FALSE
#define SST39VF320_16__NUM_REGIONS                              1


const BlockRange g_SST39VF320_16_BlockRange[] =
{
    { BlockRange::BLOCKTYPE_CODE       ,   0, 383 },   // TinyCLR runtime

    { BlockRange::BLOCKTYPE_DEPLOYMENT , 384, 570 },   // TinyCLR runtime
    { BlockRange::BLOCKTYPE_DEPLOYMENT , 571, 721 },   // TinyCLR runtime
    { BlockRange::BLOCKTYPE_DEPLOYMENT , 722, 870 },   // TinyCLR runtime
    { BlockRange::BLOCKTYPE_FILESYSTEM , 871, 990 },   // TinyCLR runtime

    { BlockRange::BLOCKTYPE_CONFIG     , 991, 999 },    // g_ConfigurationSector 
    { BlockRange::BLOCKTYPE_STORAGE_A  , 1000, 1011 },    
    { BlockRange::BLOCKTYPE_STORAGE_B  , 1012, 1023 } 

};

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata = "g_SST39VF320_16_Config"
#endif


//--//

const BlockRegionInfo  g_SST39VF320_16_BlkRegion[SST39VF320_16__NUM_REGIONS] = 
{
    SST39VF320_16_BASE_ADDRESS,      // ByteAddress   Start;              // Starting Sector address
    FLASH_BLOCK_COUNT,               // UINT32        NumBlocks;          // total number of blocks in this region
    FLASH_BYTES_PER_BLOCK,           // UINT32        BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)

    ARRAYSIZE_CONST_EXPR(g_SST39VF320_16_BlockRange),
    g_SST39VF320_16_BlockRange,
};

    
BlockDeviceInfo g_SST39VF320_16_DeviceInfo=
{
        {  
            SST39VF320_16__IS_REMOVABLE,                    // BOOL Removable;
            SST39VF320_16__SUPPORTS_XIP,                    // BOOL SupportsXIP;
            SST39VF320_16__WRITE_PROTECTED,
        },

	    FLASH_SECTOR_WRITE_TYPICAL_TIME_USEC,           // UINT32 Duration_Max_WordWrite_uSec;
	    FLASH_BLOCK_ERASE_ACTUAL_TIME_USEC,             // UINT32 Duration_Max_SectorErase_uSec;
	    FLASH_BYTES_PER_SECTOR,                         // UINT32 BytesPerSector; // Bytes Per Sector

	    FLASH_MEMORY_Size,                              // UINT32 Size;

          SST39VF320_16__NUM_REGIONS,                       // UINT32 NumRegions;
          g_SST39VF320_16_BlkRegion,                        //const BlockRegionInfo* pRegions;
};


struct MEMORY_MAPPED_NOR_BLOCK_CONFIG g_SST39VF320_16_BS_Config =
{
   { // BLOCK_CONFIG
        {
            SST39VF320_16__WP_GPIO_PIN,                    // GPIO_PIN             Pin;
            SST39VF320_16__WP_ACTIVE,                      // BOOL                 ActiveState;
        },
       &g_SST39VF320_16_DeviceInfo,                        // BlockDeviceinfo
    },

    {
        SST39VF320_16_CHIP_SELECT,                     // UINT8  CPU_MEMORY_CONFIG::ChipSelect;
        FALSE,                                         // UINT8  CPU_MEMORY_CONFIG::ReadOnly;
        SST39VF320_16_WAIT_STATES,                     // UINT32 CPU_MEMORY_CONFIG::WaitStates;
        SST39VF320_16_RELEASE_COUNTS,                  // UINT32 CPU_MEMORY_CONFIG::ReleaseCounts;
        SST39VF320_16_BIT_WIDTH,                       // UINT32 CPU_MEMORY_CONFIG::BitWidth;
        SST39VF320_16_BASE_ADDRESS,                    // UINT32 CPU_MEMORY_CONFIG::BaseAddress;
        SST39VF320_16_SIZE_IN_BYTES,                   // UINT32 CPU_MEMORY_CONFIG::SizeInBytes;
        0,0,0,
    },
    
    0,                                      // UINT32  ChipProtection;
    FLASH_MANUFACTURER_CODE,                // FLASH_WORD ManufacturerCode;
    FLASH_DEVICE_CODE,                      // FLASH_WORD DeviceCode;
};
    
#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata
#endif

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata = "g_SST39VF320_16_BS"
#endif

struct BlockStorageDevice g_SST39VF320_16_BS;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata
#endif
