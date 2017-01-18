////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

//--//

OEM_MODEL_SKU OEM_Model_SKU;

#if 1

HAL_SYSTEM_CONFIG HalSystemConfig =
{
    { TRUE },                                       // HAL_DRIVER_CONFIG_HEADER Header;
            
    //--//

    {                                               // UINT32      DebuggerPorts[MAX_DEBUGGERS];
        0,
    },

    {                                               // Mesagging Ports
        0,
    },

    //--//

    0,                                              // UINT32  DebugTextPort;
    115200,                                         // UINT32  USART_DefaultBaudRate;
    0,                                              // FILE*   stdio;

    { 0, 4*1024*1024 },                             // HAL_SYSTEM_MEMORY_CONFIG RAM1;
    { 0x10000000, 2*1024*1024 },                    // HAL_SYSTEM_MEMORY_CONFIG FLASH;
};


#else


HAL_SYSTEM_CONFIG HalSystemConfig =
{
    { TRUE },                                       // HAL_DRIVER_CONFIG_HEADER Header;
            
    //--//

    {                                               // UINT32      DebuggerPorts[MAX_DEBUGGERS];
        DEBUGGER_PORT,
    },

    {
        MESSAGING_PORT,                             // UINT32      MessagingPorts[MAX_MESSAGING];
    },

    //--//

    DEBUG_TEXT_PORT,                                // UINT32  DebugTextPort;
    115200,                                         // UINT32  USART_DefaultBaudRate;
    STDIO,                                          // FILE*   stdio;

    { SRAM1_MEMORY_Base, SRAM1_MEMORY_Size },       // HAL_SYSTEM_MEMORY_CONFIG RAM1;
    { FLASH_MEMORY_Base, FLASH_MEMORY_Size },       // HAL_SYSTEM_MEMORY_CONFIG FLASH;
};
#endif
/***************************************************************************/

const char HalName[] = HAL_SYSTEM_NAME;

//--//

