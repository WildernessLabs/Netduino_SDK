////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PLATFORM_SH7216_RSK_SELECTOR_H_
#define _PLATFORM_SH7216_RSK_SELECTOR_H_ 1

/////////////////////////////////////////////////////////
//
// processor and features
//
#if defined(PLATFORM_SH7216_RSK)
#define HAL_SYSTEM_NAME                        "SH7216_RSK"
#define PLATFORM_SH2A                        1
#define PLATFORM_SH2A_7216                   1
#define PLATFORM_SH7216                      1

             

//
// processor and features
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//
// constants
//
#define PLATFORM_SUPPORTS_SOFT_REBOOT   TRUE

#define SYSTEM_CLOCK_HZ                 (100000000 / (512)) //200000000 
#define CLOCK_COMMON_FACTOR             1
#define SLOW_CLOCKS_PER_SECOND          (100000000 / (512)) //200000000 
#define SLOW_CLOCKS_TEN_MHZ_GCD         1
#define SLOW_CLOCKS_MILLISECOND_GCD     1000

#define SRAM1_MEMORY_Base   0x0C000000
#define SRAM1_MEMORY_Size   0x01000000

#define FLASH_MEMORY_Base   0x00000000
#define FLASH_MEMORY_Size   0x00108000

#define TXPROTECTRESISTOR               RESISTOR_DISABLED
#define RXPROTECTRESISTOR               RESISTOR_DISABLED
#define CTSPROTECTRESISTOR              RESISTOR_DISABLED
#define RTSPROTECTRESISTOR              RESISTOR_DISABLED


//#define GPIO_PIN_NONE                   0xFFFFFFFF
//#define DRIVER_PAL_BUTTON_MAPPING                                                  \
//    { GPIO_PIN_NONE, BUTTON_B0 }, /* Upper Far Right - Backlight (spare n)   RP */ \
//    { GPIO_PIN_NONE, BUTTON_B1 }, /* Lower Far Right - Channel               RP */ \
//    { GPIO_PIN_NONE, BUTTON_B2 }, /* Upper Center    - Up                    R  */ \
//    { GPIO_PIN_NONE, BUTTON_B3 }, /* Far Left        - Spare                    */ \
//    { GPIO_PIN_NONE, BUTTON_B4 }, /* Center          - Enter                    */ \
//    { GPIO_PIN_NONE, BUTTON_B5 }, /* Lower Center    - Down                   P */

#define INSTRUMENTATION_H_GPIO_PIN      GPIO_PIN_NONE


#define DEBUG_TEXT_PORT         COM2
#define STDIO                   COM2
#define DEBUGGER_PORT           COM2
#define MESSAGING_PORT          COM2	
//
// constants
/////////////////////////////////////////////////////////


#include <processor_selector.h>

#endif // PLATFORM_SH7216_RSK

#endif // _PLATFORM_SH7216_RSK_SELECTOR_H_ 1
