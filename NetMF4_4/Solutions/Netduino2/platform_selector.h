////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
//  Copyright (c) Microsoft Corporation. All rights reserved.
//  Portions Copyright (c) Secret Labs LLC and Netduino Community Members. All rights reserved.
//  Portions (c) Oberon microsystems, Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PLATFORM_Netduino2_SELECTOR_H_
#define _PLATFORM_Netduino2_SELECTOR_H_ 1

/////////////////////////////////////////////////////////
//
// processor and features
//

#if defined(PLATFORM_ARM_Netduino2)
#define HAL_SYSTEM_NAME                     "Netduino2"

#define STM32F2XX
#define PLATFORM_ARM_STM32 /* this is an old designation */
#define PLATFORM_ARM_STM32F2_ANY /* this is our new designation for STM32F2 */
//#define PLATFORM_ARM_STM32F205RF /* this is our particular MCU */

#define USB_ALLOW_CONFIGURATION_OVERRIDE  1

//
// processor and features
//
/////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////
//
// constants
//

// legal SYSCLK: 24, 30, 32, 36, 40, 42, 48, 54, 56, 60, 64, 72, 84, 96, 108, 120MHz
#define SYSTEM_CLOCK_HZ                 120000000  // 120MHz
#define SYSTEM_CYCLE_CLOCK_HZ           120000000  // 120MHz
#define SYSTEM_APB1_CLOCK_HZ            30000000   // 30MHz
#define SYSTEM_APB2_CLOCK_HZ            60000000   // 60MHz
#define SYSTEM_CRYSTAL_CLOCK_HZ         25000000   // 25MHz external clock
#define SUPPLY_VOLTAGE_MV               3300       // 3.3V supply

#define CLOCK_COMMON_FACTOR             1000000    // GCD(SYSTEM_CLOCK_HZ, 1M)

#define SLOW_CLOCKS_PER_SECOND          1000000    // 1MHz
#define SLOW_CLOCKS_TEN_MHZ_GCD         1000000    // GCD(SLOW_CLOCKS_PER_SECOND, 10M)
#define SLOW_CLOCKS_MILLISECOND_GCD     1000       // GCD(SLOW_CLOCKS_PER_SECOND, 1k)

#define FLASH_MEMORY_Base               0x08000000
#define FLASH_MEMORY_Size               0x000C0000
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
#define STM32_PWM_TIMER    {  10,  11,  13,   2,   9,  12,   1} /* timer numbers use one-based index */
#define STM32_PWM_CHANNEL  {   0,   0,   0,   2,   0,   1,   2} /* channel values use zero-based index */
#define STM32_PWM_PIN      {0x18,0x19,0x06,0x1A,0x02,0x1F,0x0A}

// UART Configuration
#define STM32_UART         {USART6,USART2,UART4, USART1}
#define STM32_UART_NUM     {     5,     1,    3,      0} /* uart numbers use zero-based index */
#define STM32_UART_TX_PIN  {  0x26,  0x02,  0x00,  0x16}
#define STM32_UART_RX_PIN  {  0x27,  0x03,  0x01,  0x17}
#define STM32_UART_RTS_PIN {  0xFF,  0x01,  0xFF,  0xFF} /* 0xFF = unused */
#define STM32_UART_CTS_PIN {  0xFF,  0x00,  0xFF,  0xFF} /* 0xFF = unused */

// GPIO Configuration
#define STM32_GPIO_USER_LED	    0x0A
#define STM32_GPIO_RESET_BTN	0x1B

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

#define TOTAL_USART_PORT                4
#define USART_DEFAULT_PORT              COM1
#define USART_DEFAULT_BAUDRATE          115200

#define PLATFORM_DEPENDENT_TX_USART_BUFFER_SIZE     512  // there is one TX for each usart port
#define PLATFORM_DEPENDENT_RX_USART_BUFFER_SIZE     512  // there is one RX for each usart port
#define PLATFORM_DEPENDENT_USB_QUEUE_PACKET_COUNT     8  // there is one queue for each pipe of each endpoint and the size of a single packet is sizeof(USB_PACKET64) == 68 bytes

#define PLATFORM_DEPENDENT_FATFS_SECTORCACHE_MAXSIZE  3
#define PLATFORM_DEPENDENT_FATFS_SECTORCACHE_LINESIZE 512
#define PLATFORM_DEPENDENT_FATFS_MAX_OPEN_HANDLES     4
#define PLATFORM_DEPENDENT_FATFS_MAX_VOLUMES          2

////////////////////////////////////////////////
// CLR runtime memory configuration 
//
#define RUNTIME_MEMORY_PROFILE__extrasmall 1
//
//
////////////////////////////////////////////////


#include <processor_selector.h>

#endif // PLATFORM_ARM_Netduino2
//
// drivers
/////////////////////////////////////////////////////////

#endif // _PLATFORM_Netduino2_SELECTOR_H_
