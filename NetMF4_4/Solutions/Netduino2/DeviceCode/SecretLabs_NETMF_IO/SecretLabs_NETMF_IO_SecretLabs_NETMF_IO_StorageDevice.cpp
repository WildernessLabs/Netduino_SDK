//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------


#include "SecretLabs_NETMF_IO.h"
#include "SecretLabs_NETMF_IO_SecretLabs_NETMF_IO_StorageDevice.h"

using namespace SecretLabs::NETMF::IO;

extern struct BlockStorageDevice  g_SD_BS;
extern struct IBlockStorageDevice g_SD_BS_DeviceTable;
extern struct BLOCK_CONFIG        g_SD_BS_Config;
extern SD_BL_CONFIGURATION        g_SD_BL_Config;


void SD_InsertEjectIsr( GPIO_PIN Pin, BOOL PinState, void* Param );

void StorageDevice::SDSPI_Mount( LPCSTR param0, UINT32 param1, UINT32 param2, UINT32 param3, HRESULT &hr )
{
    // param1: UINT32 spi (SPI.SPI_module cast to UINT32 in C#)
    g_SD_BL_Config.SPI.SPI_mod = param1;

    // param2: UINT32 chipSelectPort (Cpu.Pin cast to UINT32 in C#)
    g_SD_BL_Config.SPI.DeviceCS = param2;
	
	// param3: UINT32 cardDetect (Cpu.Pin cast to UINT32 in C#)
	if (param3 != GPIO_PIN_NONE)
	{
        // wire up our SD card detect interrupt handler (with glitch filter)
        CPU_GPIO_EnableInputPin( param3, TRUE, SD_InsertEjectIsr, GPIO_INT_EDGE_BOTH, RESISTOR_PULLUP );
	}

    /* if the card detect shows an inserted card (or if there is no card detect)
	   then go ahead and mount the card now. */
	if (param3 == GPIO_PIN_NONE || !CPU_GPIO_GetPinState( param3 ))
	{
        if (BlockStorageList::AddDevice( &g_SD_BS, &g_SD_BS_DeviceTable, &g_SD_BS_Config, TRUE ))
        {
            FS_MountVolume( param0, 0, 0, &g_SD_BS );		
        }
        else
        {
            hr = CLR_E_VOLUME_NOT_FOUND;
            return;
        }
	}
    
}

void StorageDevice::SDSPI_Unmount( HRESULT &hr )
{
    FS_UnmountVolume( &g_SD_BS );
    BlockStorageList::RemoveDevice( &g_SD_BS, TRUE );
}

void SD_InsertEjectIsr( GPIO_PIN Pin, BOOL PinState, void* Param )
{
    // if the SD card was inserted, try to mount it; if it was ejected, try to unmount it.
    if ( !PinState ) 
    {
        // card was inserted
        if (BlockStorageList::AddDevice( &g_SD_BS, &g_SD_BS_DeviceTable, &g_SD_BS_Config, TRUE ))
        {
            FS_MountVolume( "SD", 0, 0, &g_SD_BS );
        }
    }
    else
    {
        // card was ejected
        FS_UnmountVolume( &g_SD_BS );
        BlockStorageList::RemoveDevice( &g_SD_BS, TRUE );
    }
}
