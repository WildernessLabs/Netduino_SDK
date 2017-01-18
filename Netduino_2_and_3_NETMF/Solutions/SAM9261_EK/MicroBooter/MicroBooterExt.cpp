//-----------------------------------------------------------------------------
//
//  <No description>
//
//  Microsoft dotNetMF Project
//  Copyright ©2004 Microsoft Corporation
//  One Microsoft Way, Redmond, Washington 98052-6399 U.S.A.
//  All rights reserved.
//  MICROSOFT CONFIDENTIAL
//
//-----------------------------------------------------------------------------

#include "MicroBooter.h"
#include <Drivers\MFUpdate\Storage\FS\FSUpdateStorage.h>

// required by file system driver
void PostManagedEvent( UINT8 category, UINT8 subCategory, UINT16 data1, UINT32 data2 )
{
}


static const IUpdateStorageProvider* s_UpdateStorageList[] =
{
    &g_FSUpdateStorageProvider
};

const IUpdateStorageProvider** g_MicroBooter_UpdateStorageList      = s_UpdateStorageList;
UINT32                         g_MicroBooter_UpdateStorageListCount = ARRAYSIZE(s_UpdateStorageList);

BOOL EnterMicroBooter(INT32& timeout)
{
    CPU_GPIO_Initialize();
    
    Events_WaitForEvents(0,100); // wait for buttons to init

    // check up/down button state
    if(!CPU_GPIO_GetPinState( AT91_GPIO_Driver::PA25 ) && !CPU_GPIO_GetPinState( AT91_GPIO_Driver::PA27 ))
    {
        // user override, so lets stay forever
        timeout = -1;
        return TRUE;
    }
    
    timeout = 0;
    return FALSE;
}

UINT32 MicroBooter_ProgramMarker()
{
    return 0xE321F0DF;
}

#define CODE_BASEADDRESS   0x00060000
#define EXCODE_BASEADDRESS 0x20040000 

#define DATA_BASEADDRESS   0x002A0000

#define CONFIG_BASEADDRESS 0x00020000
#define CONGIG_Size        0x00020000

#define DEPLOY_BASEADDRESS 0x002E0000

#define CODE_SIZE           DATA_BASEADDRESS-CODE_BASEADDRESS
#define DATA_SIZE           DEPLOY_BASEADDRESS-DATA_BASEADDRESS

UINT32 MicroBooter_PrepareForExecution(UINT32 physicalEntryPointAddress)
{
    if (physicalEntryPointAddress == CODE_BASEADDRESS)
    {
        BlockStorageDevice *device;
        ByteAddress ByteAddress;
        UINT32 physicalAddress = CODE_BASEADDRESS;
        
        if (BlockStorageList::FindDeviceForPhysicalAddress( &device, physicalAddress, ByteAddress)) 
        {
            BlockStorageStream stream;
    
            if(stream.Initialize(BlockUsage::CODE, device))
            {
                BYTE *dst;

                if(stream.CurrentAddress() != CODE_BASEADDRESS)
                {
                    hal_fprintf( STREAM_LCD, "Warn: at wrong offset: 0x%08x\r\n", (UINT32)stream.CurrentAddress());
                    debug_printf( "Warn: at wrong offset: 0x%08x\r\n", (UINT32)stream.CurrentAddress());
                    stream.Seek(CODE_BASEADDRESS - stream.CurrentAddress(), BlockStorageStream::SeekCurrent);
                }

                // load ER_FLASH only
                dst =(BYTE *) EXCODE_BASEADDRESS  ;
                
                
                stream.Read( &dst, CODE_SIZE );
                

                CPU_DrainWriteBuffers();
            }
            
        }
    
    }

    return EXCODE_BASEADDRESS;
}



