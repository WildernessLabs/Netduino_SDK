////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PLATFORM_SAM7S_EK_SELECTOR_H_
#define _PLATFORM_SAM7S_EK_SELECTOR_H_ 1

/////////////////////////////////////////////////////////
//
// processor and features
//

#if defined(PLATFORM_ARM_SAM7S_EK)

#define HAL_SYSTEM_NAME             "SAM7S_EK"
#define PLATFORM_ARM_SAM7_ANY       1
#define PLATFORM_ARM_SAM7S_ANY      1
#define PLATFORM_ARM_SAM7S256       1

//
// processor and features
//
/////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////
//
// constants
//
#define SYSTEM_CYCLE_CLOCK_HZ           48000000
#define SYSTEM_PERIPHERAL_CLOCK_HZ      SYSTEM_CYCLE_CLOCK_HZ
#define SYSTEM_CLOCK_HZ                 48000000
#define CLOCK_COMMON_FACTOR             1000000
//TClk 5 ( Select MCK/1024)
#define SLOW_CLOCKS_PER_SECOND          46875
#define SLOW_CLOCKS_TEN_MHZ_GCD         15625
#define SLOW_CLOCKS_MILLISECOND_GCD     125

#define FLASH_MEMORY_Base   0x00100000
#define FLASH_MEMORY_Size   0x00040000

#define SRAM1_MEMORY_Base   0x00200000
#define SRAM1_MEMORY_Size   0x00020000

#define TXPROTECTRESISTOR               RESISTOR_DISABLED
#define RXPROTECTRESISTOR               RESISTOR_DISABLED
#define CTSPROTECTRESISTOR              RESISTOR_DISABLED
#define RTSPROTECTRESISTOR              RESISTOR_DISABLED

#define INSTRUMENTATION_H_GPIO_PIN      GPIO_PIN_NONE

#if 0

    #define DEBUG_TEXT_PORT    COM1
    #define STDIO               COM1
    #define DEBUGGER_PORT      COM1
    #define MESSAGING_PORT     COM1
#else

    #define DEBUG_TEXT_PORT    USB1
    #define STDIO               USB1
    #define DEBUGGER_PORT      USB1
    #define MESSAGING_PORT     USB1


#endif

#define AT91_UDP_EXTERNAL_PULLUP       1
#define AT91_DP_PULLUP                 AT91_GPIO_Driver::PA16
#define AT91_VBUS_DETECTION            1
#define AT91_VBUS                      AT91_GPIO_Driver::PA13


#define DRIVER_PAL_BUTTON_MAPPING                       \
    { GPIO_PIN_NONE,          BUTTON_B0 }, /* Up */        \
    { GPIO_PIN_NONE,          BUTTON_B1 }, /* Down */      \
    { AT91_GPIO_Driver::PA19, BUTTON_B2 }, /* Left */      \
    { AT91_GPIO_Driver::PA20, BUTTON_B3 }, /* Right */     \
    { AT91_GPIO_Driver::PA14, BUTTON_B4 }, /* Enter */     \
    { AT91_GPIO_Driver::PA15, BUTTON_B5 }, 
//
// constants
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//
// macros
//

#define GLOBAL_LOCK(x)             SmartPtr_IRQ x
#define DISABLE_INTERRUPTS()       SmartPtr_IRQ::ForceDisabled()
#define ENABLE_INTERRUPTS()        SmartPtr_IRQ::ForceEnabled()
#define INTERRUPTS_ENABLED_STATE() SmartPtr_IRQ::GetState()
#define GLOBAL_LOCK_SOCKETS(x)     SmartPtr_IRQ x

#if defined(_DEBUG)
#define ASSERT_IRQ_MUST_BE_OFF()   ASSERT(!SmartPtr_IRQ::GetState())
#define ASSERT_IRQ_MUST_BE_ON()    ASSERT( SmartPtr_IRQ::GetState())
#else
#define ASSERT_IRQ_MUST_BE_OFF()
#define ASSERT_IRQ_MUST_BE_ON()
#endif
//
// macros
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//
// global functions
//

//
// global functions
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// communicaiton facilities
//
// Port definitions
#define TOTAL_USART_PORT       1
#define COM1                   ConvertCOM_ComHandle(0)
#define COM2                   ConvertCOM_ComHandle(1)

#define TOTAL_USB_CONTROLLER   1
#define USB1                   ConvertCOM_UsbHandle(0)

#define TOTAL_SOCK_PORT        0

#define TOTAL_DEBUG_PORT       1
#define COM_DEBUG              ConvertCOM_DebugHandle(0)

#define COM_MESSAGING          ConvertCOM_MessagingHandle(0)

#define USART_TX_IRQ_INDEX(x)       ( (x) ? 0 : 0 )     // TODO set right indexes
#define USART_DEFAULT_PORT          COM1
#define USART_DEFAULT_BAUDRATE      115200

#define USB_IRQ_INDEX               0                   // TODO set right index

#define PLATFORM_DEPENDENT_TX_USART_BUFFER_SIZE    512  // there is one TX for each usart port
#define PLATFORM_DEPENDENT_RX_USART_BUFFER_SIZE    512  // there is one RX for each usart port
#define PLATFORM_DEPENDENT_USB_QUEUE_PACKET_COUNT    8  // there is one queue for each pipe of each endpoint and the size of a single packet is sizeof(USB_PACKET64) == 68 bytes
//
// communicaiton facilities
/////////////////////////////////////////////////////////


////////////////////////////////////////////////
// CLR runtime memory configuration 
//
#define RUNTIME_MEMORY_PROFILE__extrasmall 1
//
//
////////////////////////////////////////////////

#include <processor_selector.h>

#endif // PLATFORM_ARM_SAM7S_EK
#endif // _PLATFORM_SAM7S_EK_SELECTOR_H_

