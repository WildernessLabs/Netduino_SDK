////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PLATFORM_<TEMPLATE>_SELECTOR_H_
#define _PLATFORM_<TEMPLATE>_SELECTOR_H_ 1

/////////////////////////////////////////////////////////
//
// processor and features
//
#if defined(PLATFORM_ARM_<TEMPLATE>)
#define HAL_SYSTEM_NAME                    "<TEMPLATE>"

#define PLATFORM_ARM_MC9328                1
#define PLATFORM_ARM_MC9328MXS             1

//--//


#define HARDWARE_BOARD_i_MXS_DEMO_REV_V1_2 3
#define HARDWARE_BOARD_i_MXS_DEMO_REV_V1_3 4

#define HARDWARE_BOARD_TYPE                HARDWARE_BOARD_i_MXS_DEMO_REV_V1_3

//
// processor and features
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//
// constants
//
#define PLATFORM_SUPPORTS_SOFT_REBOOT   TRUE

#define SYSTEM_CLOCK_HZ                 16000000
#define CLOCK_COMMON_FACTOR             1000000
#define SLOW_CLOCKS_PER_SECOND          SYSTEM_CLOCK_HZ
#define SLOW_CLOCKS_TEN_MHZ_GCD         2000000
#define SLOW_CLOCKS_MILLISECOND_GCD     1000

#define SRAM1_MEMORY_Base               0x08000000
#define SRAM1_MEMORY_Size               (32*1024*1024)

#define FLASH_MEMORY_Base               0x10000000
#define FLASH_MEMORY_Size               (16*1024*1024)

#define TXPROTECTRESISTOR               RESISTOR_DISABLED
#define RXPROTECTRESISTOR               RESISTOR_DISABLED
#define CTSPROTECTRESISTOR              RESISTOR_DISABLED
#define RTSPROTECTRESISTOR              RESISTOR_DISABLED


#define DRIVER_PAL_BUTTON_MAPPING                                           \
    { MC9328MXL_GPIO::c_Port_B_15, BUTTON_B0 }, /* Upper Far Right - Backlight (spare n)   RP */ \
    { MC9328MXL_GPIO::c_Port_B_12, BUTTON_B1 }, /* Lower Far Right - Channel               RP */ \
    { MC9328MXL_GPIO::c_Port_B_11, BUTTON_B2 }, /* Upper Center    - Up                    R  */ \
    { MC9328MXL_GPIO::c_Port_B_16, BUTTON_B3 }, /* Far Left        - Spare                    */ \
    { MC9328MXL_GPIO::c_Port_B_08, BUTTON_B4 }, /* Center          - Enter                    */ \
    { MC9328MXL_GPIO::c_Port_B_10, BUTTON_B5 }, /* Lower Center    - Down                   P */

#define MC9328XML_UNUSED_GPIOS \
    UNUSED_GPIO_PULLUP(A_03), \
    UNUSED_GPIO_PULLUP(A_04), \
    UNUSED_GPIO_PULLUP(A_05), \
    UNUSED_GPIO_PULLUP(A_06), \
    UNUSED_GPIO_PULLUP(A_07), \
    UNUSED_GPIO_PULLUP(A_08), \
    UNUSED_GPIO_PULLUP(A_09), \
    UNUSED_GPIO_PULLUP(A_10), \
    UNUSED_GPIO_PULLUP(A_11), \
    UNUSED_GPIO_PULLUP(A_12), \
    UNUSED_GPIO_PULLUP(A_13), \
    UNUSED_GPIO_PULLUP(A_14), \
    UNUSED_GPIO_PULLUP(A_17), \
    UNUSED_GPIO_PULLUP(A_18), \
    UNUSED_GPIO_PULLUP(A_19), 

    
#define INSTRUMENTATION_H_GPIO_PIN      MC9328MXL_GPIO::c_Pin_None

#if 0
    #define DEBUG_TEXT_PORT         COM1
    #define STDIO                   COM1
    #define DEBUGGER_PORT           COM1
    #define MESSAGING_PORT          COM1
#elif 0
    #define DEBUG_TEXT_PORT         COM_SOCKET_DBG
    #define STDIO                   COM_SOCKET_DBG
    #define DEBUGGER_PORT           COM_SOCKET_DBG
    #define MESSAGING_PORT          COM_SOCKET_DBG
#else
    #define DEBUG_TEXT_PORT         USB1
    #define STDIO                   USB1
    #define DEBUGGER_PORT           USB1
    #define MESSAGING_PORT          USB1
#endif
//
// constants
/////////////////////////////////////////////////////////

#include <processor_selector.h>

#endif // PLATFORM_ARM_<TEMPLATE>
//
// drivers
/////////////////////////////////////////////////////////

#endif // _PLATFORM_<TEMPLATE>_SELECTOR_H_ 1
