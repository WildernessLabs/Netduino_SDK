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
#define HAL_SYSTEM_NAME                 "<TEMPLATE>"
#define PLATFORM_ARM_SAM9261_ANY        1
    
//
// processor and features
//
/////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////
//
// constants, measured main clock = 18434048
//

// system cycle clock hz = main clock (18434048) * (MULA + 1) (1085) / DIVA (100)
#define SYSTEM_CYCLE_CLOCK_HZ           200009421  
#define SYSTEM_PERIPHERAL_CLOCK_HZ      (SYSTEM_CYCLE_CLOCK_HZ / 2)
// (SYSTEM_PERIPHERAL_CLOCK_HZ / 32)
#define SYSTEM_CLOCK_HZ                 3125147
#define CLOCK_COMMON_FACTOR             8000
#define SLOW_CLOCKS_PER_SECOND          32768
#define SLOW_CLOCKS_TEN_MHZ_GCD         128
#define SLOW_CLOCKS_MILLISECOND_GCD     8

#define SDRAM_MEMORY_Base               0x20000000
#define SDRAM_MEMORY_Size               (64*1024*1024)

#define SRAM_MEMORY_Base                0x00300000
#define SRAM_MEMORY_Size                (160*1024)

#define FLASH_MEMORY_Base               0x40000000
#define FLASH_MEMORY_Size               0x10000000
#define SRAM1_MEMORY_Base               0x00300000
#define SRAM1_MEMORY_Size               0x00028000


#define TXPROTECTRESISTOR               RESISTOR_DISABLED
#define RXPROTECTRESISTOR               RESISTOR_DISABLED
#define CTSPROTECTRESISTOR              RESISTOR_DISABLED
#define RTSPROTECTRESISTOR              RESISTOR_DISABLED

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

#define DRIVER_PAL_BUTTON_MAPPING                                           \
    { AT91_GPIO_Driver::PA24, BUTTON_B0 },  \
    { GPIO_PIN_NONE,          BUTTON_B1 },  \
    { AT91_GPIO_Driver::PA25, BUTTON_B2 },  \
    { GPIO_PIN_NONE,          BUTTON_B3 },  \
    { AT91_GPIO_Driver::PA26, BUTTON_B4 },  \
    { AT91_GPIO_Driver::PA27, BUTTON_B5 }, 

#define AT91_UDP_MATRIX_PULLUP      1
#define AT91_VBUS_DETECTION         1
#define AT91_VBUS                   AT91_GPIO_Driver::PB29

#define LCD_BIT_PIX 4
#define LCD_RGB_CONVERTION  1

#define AT91_NAND_CE		AT91_GPIO_Driver::PC14
#define AT91_NAND_RB		AT91_GPIO_Driver::PC15

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

#define USART_TX_IRQ_INDEX(x)       ( (x) ? 0 : 0 )     /* TODO set right indexes */
#define USART_DEFAULT_PORT          COM1
#define USART_DEFAULT_BAUDRATE      115200

#define USB_IRQ_INDEX               0  // TODO set right index


#define PLATFORM_DEPENDENT_TX_USART_BUFFER_SIZE    2*1024  // there is one TX for each usart port
#define PLATFORM_DEPENDENT_RX_USART_BUFFER_SIZE    2*1024  // there is one RX for each usart port
#define PLATFORM_DEPENDENT_USB_QUEUE_PACKET_COUNT  32  // there is one queue for each pipe of each endpoint and the size of a single packet is sizeof(USB_PACKET64) == 68 bytes
//
// communicaiton facilities
/////////////////////////////////////////////////////////

#include <processor_selector.h>

#endif // PLATFORM_ARM_<TEMPLATE>

#endif // _PLATFORM_<TEMPLATE>_SELECTOR_H_
