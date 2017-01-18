////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////#include <tinyhal.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PLATFORM_SH7619_EVB_SELECTOR_H_
#define _PLATFORM_SH7619_EVB_SELECTOR_H_ 1

/////////////////////////////////////////////////////////
//
// processor and features
//
#if defined(PLATFORM_SH7619_EVB)
#define HAL_SYSTEM_NAME                    "SH7619_EVB"

#define PLATFORM_SH2                        1
#define PLATFORM_SH2_7619                   1

#define USB_ALLOW_CONFIGURATION_OVERRIDE    1

//--//


// processor and features
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//
// constants
//
#define PLATFORM_SUPPORTS_SOFT_REBOOT   TRUE

#define SYSTEM_CLOCK_HZ                 (61440000 / (2 * 512)) // 61.44MHz / (2 (peripheral clock divisor) * 512 (system timer divisor))
#define CLOCK_COMMON_FACTOR             1
#define SLOW_CLOCKS_PER_SECOND          (61440000 / (2 * 512))
#define SLOW_CLOCKS_TEN_MHZ_GCD         1
#define SLOW_CLOCKS_MILLISECOND_GCD     1000

//#define SRAM1_MEMORY_Base   0x08000000
//#define SRAM1_MEMORY_Size   0x02000000
#define SRAM1_MEMORY_Base   0x0C000000
//#define SRAM1_MEMORY_Size   0x0C7FFFFF
#define SRAM1_MEMORY_Size   0x00800000

//#define FLASH_MEMORY_Base   0x10000000
//#define FLASH_MEMORY_Size   0x01000000
#define FLASH_MEMORY_Base   0xA0000000
#define FLASH_MEMORY_Size   0x00400000
//#define FLASH_MEMORY_Size   0xA03FFFFF

#define TXPROTECTRESISTOR               RESISTOR_DISABLED
#define RXPROTECTRESISTOR               RESISTOR_DISABLED
#define CTSPROTECTRESISTOR              RESISTOR_DISABLED
#define RTSPROTECTRESISTOR              RESISTOR_DISABLED


#define DRIVER_PAL_BUTTON_MAPPING                                           \
    { 26, BUTTON_B0 }, /*SH7619_GPIO::c_Key_6*/ /* Upper Far Right - Backlight (spare n)   RP */ \
    { 24, BUTTON_B1 }, /*SH7619_GPIO::c_Key_4*/ /* Lower Far Right - Channel               RP */ \
    { 21, BUTTON_B2 }, /*SH7619_GPIO::c_Key_1*/ /* Upper Center    - Up                    R  */ \
    { 23, BUTTON_B3 }, /*SH7619_GPIO::c_Key_3*/ /* Far Left        - Spare                    */ \
    { 25, BUTTON_B4 }, /*SH7619_GPIO::c_Key_5*/ /* Center          - Enter                    */ \
    { 22, BUTTON_B5 }, /*SH7619_GPIO::c_Key_2*/ /* Lower Center    - Down                   P */

#define SH7619_EVB_UNUSED_GPIOS \
    UNUSED_GPIO_PULLUP(A_03), \

   
#define INSTRUMENTATION_H_GPIO_PIN      GPIO_PIN_NONE

#if 1
    #define DEBUG_TEXT_PORT         COM3
    #define STDIO                   COM3
    #define DEBUGGER_PORT           COM3
    #define MESSAGING_PORT          COM3
#else
    #define DEBUG_TEXT_PORT         USB1
    #define STDIO                   USB1
    #define DEBUGGER_PORT           USB1
    #define MESSAGING_PORT          USB1
#endif
//
//
// constants
/////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
//
//  SOCKETS ENABLED PLATFORM -- TODO TODO TODO: REMOVE WHEN PLATFORM BUILDER AVAILABLE
//
//Network SOCKET

#define NETWORK_INTERFACE_COUNT             1

#define PLATFORM_DEPENDENT__SOCKETS_MAX_SEND_LENGTH 8192

#define NETWORK_MEMORY_PROFILE__medium      1

#define NETWORK_MEMORY_POOL__INCLUDE_SSL    1
#include <pal\net\Network_Defines.h>

//#define NETWORK_USE_LOOPBACK                1
//#define NETWORK_USE_DHCP                    1

//--//

#define PLATFORM_DEPENDENT__NETWORK_DNS_MIN_DELAY  4 
#define PLATFORM_DEPENDENT__NETWORK_DNS_MAX_DELAY  32
#define PLATFORM_DEPENDENT__NETWORK_DNS_RETRIES    3

//Network SOCKET


// LCD index Memory mapping address
// SH7619 LCD Memory Map
#define PF_INDEX_REG  0x18000000
#define PF_DATA_REG   0x1A000000



#include <processor_selector.h>

#endif // PLATFORM_SH7619_EVB
//
// drivers
/////////////////////////////////////////////////////////

#endif // _PLATFORM_SH7619_EVB_SELECTOR_H_ 1
