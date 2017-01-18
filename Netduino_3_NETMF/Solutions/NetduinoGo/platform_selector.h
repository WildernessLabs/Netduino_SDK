////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
//  Copyright (c) Microsoft Corporation. All rights reserved.
//  Portions Copyright (c) Secret Labs LLC and Netdiuno Community Members. All rights reserved.
//  Portions (c) Oberon microsystems, Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PLATFORM_NetduinoGo_SELECTOR_H_
#define _PLATFORM_NetduinoGo_SELECTOR_H_ 1

/////////////////////////////////////////////////////////
//
// processor and features
//

#if defined(PLATFORM_ARM_NetduinoGo)
#define HAL_SYSTEM_NAME                     "NetduinoGo"

#define STM32F4XX
#define PLATFORM_ARM_STM32 /* this is an old designation */
#define PLATFORM_ARM_STM32F4_ANY /* this is our new designation for STM32F4 */
//#define PLATFORM_ARM_STM32F405RG /* this is our particular MCU */

#define USB_ALLOW_CONFIGURATION_OVERRIDE  1

//
// processor and features
//
/////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////
//
// constants
//

#define SYSTEM_CLOCK_HZ                 168000000  // 168MHz
#define SYSTEM_CYCLE_CLOCK_HZ           168000000  // 168MHz
#define SYSTEM_APB1_CLOCK_HZ            42000000   //  42MHz
#define SYSTEM_APB2_CLOCK_HZ            84000000   //  84MHz
#define SYSTEM_CRYSTAL_CLOCK_HZ         25000000    // 25MHz external clock

#define CLOCK_COMMON_FACTOR             1000000   // GCD(SYSTEM_CLOCK_HZ, 1M)

#define SLOW_CLOCKS_PER_SECOND          1000000   // 1MHz
#define SLOW_CLOCKS_TEN_MHZ_GCD         1000000   // GCD(SLOW_CLOCKS_PER_SECOND, 10M)
#define SLOW_CLOCKS_MILLISECOND_GCD     1000      // GCD(SLOW_CLOCKS_PER_SECOND, 1k)

#define FLASH_MEMORY_Base               0x08000000
#define FLASH_MEMORY_Size               0x00100000
#define SRAM1_MEMORY_Base               0x20000000
#define SRAM1_MEMORY_Size               0x00020000

#define TXPROTECTRESISTOR               RESISTOR_DISABLED
#define RXPROTECTRESISTOR               RESISTOR_DISABLED
#define CTSPROTECTRESISTOR              RESISTOR_DISABLED
#define RTSPROTECTRESISTOR              RESISTOR_DISABLED

#define INSTRUMENTATION_H_GPIO_PIN      GPIO_PIN_NONE

#if 1
    #define DEFAULT_DEPLOYMENT_PORT    USB1
#else
    #define DEFAULT_DEPLOYMENT_PORT    COM1
#endif
#define DEBUG_TEXT_PORT    DEFAULT_DEPLOYMENT_PORT
#define STDIO              DEFAULT_DEPLOYMENT_PORT
#define DEBUGGER_PORT      DEFAULT_DEPLOYMENT_PORT
#define MESSAGING_PORT     DEFAULT_DEPLOYMENT_PORT

//#define STM32_USB_Attach_Pin_Direct     (16 + 5) // B5

// System Timer configuration (for main time functions)
#define STM32_32BIT_TIMER 5 /* use 32-bit timer 5 (other option: timer 2) */

// PWM Configuration
#define STM32_PWM_TIMER    {   4,   4,   4,   4,   3,   3,   3,   3} /* timer numbers use one-based-index */
#define STM32_PWM_CHANNEL  {   0,   1,   2,   3,   0,   1,   2,   3} /* channel values use zero-based-index */
#define STM32_PWM_PIN      {0x16,0x17,0x18,0x19,0x26,0x27,0x28,0x29}

// UART Configuration
#define STM32_UART         {USART2, UART4}
#define STM32_UART_NUM     {     1,     3} /* uart numbers use zero-based index */
#define STM32_UART_TX_PIN  {  0x02,  0x00}
#define STM32_UART_RX_PIN  {  0x03,  0x01}
#define STM32_UART_RTS_PIN {  0xFF,  0xFF} /* 0xFF = unused */
#define STM32_UART_CTS_PIN {  0xFF,  0xFF} /* 0xFF = unused */

// GPIO Configuration
#define STM32_GPIO_RESET_BTN	0x2E

//
// constants
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
// communication facilities
//
#define TOTAL_USB_CONTROLLER            1
#define USB_MAX_QUEUES                  4  // 4 endpoints (EP0 + 3)

#define TOTAL_USART_PORT                2
#define USART_DEFAULT_PORT              COM1
#define USART_DEFAULT_BAUDRATE          115200
#include <processor_selector.h>

#endif // PLATFORM_ARM_NetduinoGo
//
// drivers
/////////////////////////////////////////////////////////

#endif // _PLATFORM_NetduinoGo_SELECTOR_H_
