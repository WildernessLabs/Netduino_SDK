////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PLATFORM_SH7264_RSK_SELECTOR_H_
#define _PLATFORM_SH7264_RSK_SELECTOR_H_ 1

/////////////////////////////////////////////////////////
//
// processor and features
//
#if defined(PLATFORM_SH7264_RSK)
#define HAL_SYSTEM_NAME                        "SH7264_RSK"

#define PLATFORM_SH7264                       
#define PLATFORM_SH7264_XXXX                 

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
#define FLASH_MEMORY_Size   0x4000000

#define TXPROTECTRESISTOR               RESISTOR_DISABLED
#define RXPROTECTRESISTOR               RESISTOR_DISABLED
#define CTSPROTECTRESISTOR              RESISTOR_DISABLED
#define RTSPROTECTRESISTOR              RESISTOR_DISABLED

#define PLATFORM_DEPENDENT_TX_USART_BUFFER_SIZE    512  // there is one TX for each usart port
#define PLATFORM_DEPENDENT_RX_USART_BUFFER_SIZE    512 // there is one RX for each usart port


#define GPIO_PIN_NONE                   0xFFFFFFFF
#define DRIVER_PAL_BUTTON_MAPPING                                                  \
    { 4, BUTTON_B0 }, /*SH7264_GPIO::c_SW_02*/ \
    { 5, BUTTON_B1 }, /*SH7264_GPIO::c_SW_03*/ \
    { 8, BUTTON_B2 }, /*SH7264_GPIO::c_SW_06*/ \
    { 9, BUTTON_B3 }, /*SH7264_GPIO::c_SW_07*/ \
    { 12, BUTTON_B4 }, /*SH7264_GPIO::c_SW_10*/ \
    { 13, BUTTON_B5 }, /*SH7264_GPIO::c_SW_11*/ 

 
//    UNUSED_GPIO_PULLUP(PINxxx),

   
#define INSTRUMENTATION_H_GPIO_PIN      GPIO_PIN_NONE

#if 1
    #define DEBUG_TEXT_PORT    COM4
    #define STDIO              COM4
    #define DEBUGGER_PORT      COM4
    #define MESSAGING_PORT     COM4
#else
    #define DEBUG_TEXT_PORT    COM1
    #define STDIO              COM1
    #define DEBUGGER_PORT      COM1
    #define MESSAGING_PORT     COM1
#endif
//
// constants
//////////////////////////////////////////////////////////////////////////////////////////////
//
//  SOCKETS ENABLED PLATFORM -- TODO TODO TODO: REMOVE WHEN PLATFORM BUILDER AVAILABLE
//
//Network SOCKET

#ifdef SH7264_RSK_ETHERNET
#define NETWORK_INTERFACE_COUNT                     1

#define PLATFORM_DEPENDENT__SOCKETS_MAX_SEND_LENGTH 8192

#define NETWORK_MEMORY_PROFILE__medium              1

#define NETWORK_MEMORY_POOL__INCLUDE_SSL            1

#ifdef TCPIP_LWIP
#include <pal\net\Network_Defines_LWIP.h>
#else
#include <pal\net\Network_Defines.h>
#endif

//#define NETWORK_USE_LOOPBACK                        1
//#define NETWORK_USE_DHCP                            1

//--//

#define PLATFORM_DEPENDENT__NETWORK_DNS_MIN_DELAY  4 
#define PLATFORM_DEPENDENT__NETWORK_DNS_MAX_DELAY  32
#define PLATFORM_DEPENDENT__NETWORK_DNS_RETRIES    3

#endif

//Network SOCKET

/////////////////////////////////////////////////////////


#include <processor_selector.h>

#endif // PLATFORM_SH7264_RSK

#endif // _PLATFORM_SH7264_RSK_SELECTOR_H_ 1
