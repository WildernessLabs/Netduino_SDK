////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PLATFORM_SH7264_M3A_HS64_SELECTOR_H_
#define _PLATFORM_SH7264_M3A_HS64_SELECTOR_H_ 1

/////////////////////////////////////////////////////////
//
// processor and features
//
#if defined(PLATFORM_SH7264_M3A_HS64)
#define HAL_SYSTEM_NAME                        "SH7264_M3A_HS64"
#define PLATFORM_SH2A                           1
#define PLATFORM_SH7264                         1
#define PLATFORM_SH7264_M3A_HS64_XXXX                 

//
// processor and features
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//
// constants
//

#define SYSTEM_CLOCK_HZ                 (144000000/(2*512))
#define CLOCK_COMMON_FACTOR             1
#define SLOW_CLOCKS_PER_SECOND          (144000000/(2*512))
#define SLOW_CLOCKS_TEN_MHZ_GCD         1
#define SLOW_CLOCKS_MILLISECOND_GCD     1000

#define SRAM1_MEMORY_Base   0x0C000000
#define SRAM1_MEMORY_Size   0x04000000

#define FLASH_MEMORY_Base   0x20000000
#define FLASH_MEMORY_Size   0x203FFFFF

#define TXPROTECTRESISTOR               RESISTOR_DISABLED
#define RXPROTECTRESISTOR               RESISTOR_DISABLED
#define CTSPROTECTRESISTOR              RESISTOR_DISABLED
#define RTSPROTECTRESISTOR              RESISTOR_DISABLED

#define PLATFORM_DEPENDENT_TX_USART_BUFFER_SIZE    512  // there is one TX for each usart port
#define PLATFORM_DEPENDENT_RX_USART_BUFFER_SIZE    512 // there is one RX for each usart port


#define GPIO_PIN_NONE                   0xFFFFFFFF
#define DRIVER_PAL_BUTTON_MAPPING                                                  \
    { 16, BUTTON_B0 }, /*SH7264_GPIO::c_SW_4*/ \
    { 17, BUTTON_B1 }, /*SH7264_GPIO::c_SW_5*/ \
    { 18, BUTTON_B2 }, /*SH7264_GPIO::c_SW_6*/ \
    { 19, BUTTON_B3 }, /*SH7264_GPIO::c_SW_7*/ \
    { 20, BUTTON_B4 }, /*SH7264_GPIO::c_SW_8*/ \
    { 21, BUTTON_B5 }, /*SH7264_GPIO::c_SW_9*/ 

 
//    UNUSED_GPIO_PULLUP(PINxxx),

   
#define INSTRUMENTATION_H_GPIO_PIN      GPIO_PIN_NONE

#if 1
    #define DEBUG_TEXT_PORT    COM1
    #define STDIO              COM1
    #define DEBUGGER_PORT      COM1
    #define MESSAGING_PORT     COM1
#else
    #define DEBUG_TEXT_PORT    COM1
    #define STDIO              COM1
    #define DEBUGGER_PORT      COM1
    #define MESSAGING_PORT     COM1
#endif
//
// constants
/////////////////////////////////////////////////////////


#include <processor_selector.h>

#endif // PLATFORM_SH7264_M3A_HS64

#endif // _PLATFORM_SH7264_M3A_HS64_SELECTOR_H_ 1
