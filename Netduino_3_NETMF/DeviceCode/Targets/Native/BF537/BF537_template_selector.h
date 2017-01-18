////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PLATFORM_<TEMPLATE>_SELECTOR_H_
#define _PLATFORM_<TEMPLATE>_SELECTOR_H_ 1

/////////////////////////////////////////////////////////
//
// processor and features
//
#if defined(PLATFORM_BLACKFIN_<TEMPLATE>)
#define HAL_SYSTEM_NAME                        "<TEMPLATE>"

#define PLATFORM_BLACKFIN                       
#define PLATFORM_BLACKFIN_<TEMPLATE>                 

//
// processor and features
//
/////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////
//
// constants
//

#define SYSTEM_CLOCK_HZ                 600000000
#define CLOCK_COMMON_FACTOR             1000000
#define SLOW_CLOCKS_PER_SECOND          SYSTEM_CLOCK_HZ
#define SLOW_CLOCKS_TEN_MHZ_GCD         2000000
#define SLOW_CLOCKS_MILLISECOND_GCD     1000

#define SRAM1_MEMORY_Base               0x00000000
#define SRAM1_MEMORY_Size               (64*1024*1024)

#define FLASH_MEMORY_Base               0x10000000      // TODO: Change
#define FLASH_MEMORY_Size               (4*1024*1024)

#define TXPROTECTRESISTOR               RESISTOR_DISABLED
#define RXPROTECTRESISTOR               RESISTOR_DISABLED
#define CTSPROTECTRESISTOR              RESISTOR_DISABLED
#define RTSPROTECTRESISTOR              RESISTOR_DISABLED


#define GPIO_PIN_NONE                   0xFFFFFFFF
#define DRIVER_PAL_BUTTON_MAPPING                                            \
    { GPIO_PIN_NONE, BUTTON_B0 }, /* Upper Far Right - Backlight (spare n)   RP */ \
    { GPIO_PIN_NONE, BUTTON_B1 }, /* Lower Far Right - Channel               RP */ \
    { GPIO_PIN_NONE, BUTTON_B2 }, /* Upper Center    - Up                    R  */ \
    { GPIO_PIN_NONE, BUTTON_B3 }, /* Far Left        - Spare                    */ \
    { GPIO_PIN_NONE, BUTTON_B4 }, /* Center          - Enter                    */ \
    { GPIO_PIN_NONE, BUTTON_B5 }, /* Lower Center    - Down                   P */

#define MC9328XML_UNUSED_GPIOS \

//    UNUSED_GPIO_PULLUP(PINxxx),

   
#define INSTRUMENTATION_H_GPIO_PIN      GPIO_PIN_NONE

#if 1
    #define DEBUG_TEXT_PORT         COM1
    #define STDIO                   COM1
    #define DEBUGGER_PORT           COM1
    #define MESSAGING_PORT          COM1
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

#endif // PLATFORM_BLACKFIN_<TEMPLATE>


#endif // _PLATFORM_<TEMPLATE>_SELECTOR_H_ 1
