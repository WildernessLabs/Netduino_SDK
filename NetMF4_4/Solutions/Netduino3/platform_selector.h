/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This file is part of the Microsoft .NET Micro Framework Porting Kit Code Samples and is unsupported. 
// Copyright (c) Microsoft Open Technologies, Inc. All rights reserved.
// 
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use these files except in compliance with the License.
// You may obtain a copy of the License at:
// 
// http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing
// permissions and limitations under the License.
// 
// Based on the Implementation for (STM32F4) by Oberon microsystems, Inc.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PLATFORM_Netduino3_SELECTOR_H_
#define _PLATFORM_Netduino3_SELECTOR_H_

/////////////////////////////////////////////////////////
//
// processor and features
//

#if defined(PLATFORM_ARM_Netduino3)
#define HAL_SYSTEM_NAME "Netduino3"

#define PLATFORM_ARM_STM32F4 1 // STM32F407 cpu
#define STM32F429_439xx       // enable F429 features
#define USB_ALLOW_CONFIGURATION_OVERRIDE 1

//
// processor and features
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//
// constants
//

#define GPIO_PORTA 0
#define GPIO_PORTB 1
#define GPIO_PORTC 2
#define GPIO_PORTD 3
#define GPIO_PORTE 4
#define GPIO_PORTF 5
#define GPIO_PORTG 6
// The remaining ports are not broken out - except PH0 and PH1,
// which are deliberately omitted to keep the range continuous.

#define PORT_PIN(port,pin) ( ( (int)port) * 16 + ( pin ) )

// System clock
#define SYSTEM_CLOCK_HZ                 168000000   // 168 MHz
#define SYSTEM_CYCLE_CLOCK_HZ           168000000   // 168 MHz
#define SYSTEM_APB1_CLOCK_HZ             42000000   //  42 MHz
#define SYSTEM_APB2_CLOCK_HZ             84000000   //  84 MHz

#define SYSTEM_CRYSTAL_CLOCK_HZ         25000000    // 25MHz external clock

#define SUPPLY_VOLTAGE_MV                    3300   // 3.3V supply

#define CLOCK_COMMON_FACTOR               1000000   // GCD(SYSTEM_CLOCK_HZ, 1M)

#define SLOW_CLOCKS_PER_SECOND            1000000   // 1 MHz
#define SLOW_CLOCKS_TEN_MHZ_GCD           1000000   // GCD(SLOW_CLOCKS_PER_SECOND, 10M)
#define SLOW_CLOCKS_MILLISECOND_GCD          1000   // GCD(SLOW_CLOCKS_PER_SECOND, 1k)

#define FLASH_MEMORY_Base               0x08000000
#define FLASH_MEMORY_Size               0x00200000
#define SRAM1_MEMORY_Base               0x20000000
#define SRAM1_MEMORY_Size               0x00030000

#define TXPROTECTRESISTOR               RESISTOR_DISABLED
#define RXPROTECTRESISTOR               RESISTOR_DISABLED
#define CTSPROTECTRESISTOR              RESISTOR_DISABLED
#define RTSPROTECTRESISTOR              RESISTOR_DISABLED

#define TOTAL_GPIO_PORT                 (GPIO_PORTG + 1)
#define TOTAL_GPIO_PINS                 (TOTAL_GPIO_PORT*16)

#define INSTRUMENTATION_H_GPIO_PIN      GPIO_PIN_NONE

#define TOTAL_USART_PORT                7 // 6 physical UARTS

#define USART_DEFAULT_PORT              COM1
#define USART_DEFAULT_BAUDRATE          115200

#define TOTAL_GENERIC_PORTS             1 // 1 generic port extensions (ITM channel 0 )
#define ITM_GENERIC_PORTNUM             0 // ITM0 is index 0 in generic port interface table

#define DEBUG_TEXT_PORT                 ITM0
#define STDIO                           ITM0
#define DEBUGGER_PORT                   USB1
#define MESSAGING_PORT                  USB1

#define TOTAL_USB_CONTROLLER            1  // Silicon has 2, but only one supported in this port at this time...
#define USB_MAX_QUEUES                  4  // 4 endpoints (EP0 + 3)

// System Timer Configuration
#define STM32F4_32B_TIMER 5 // |   2   |   5    |
#define STM32F4_16B_TIMER 3 // |3,4,8,9|1,3,8,12|

// Analog Configuration
#define STM32F4_ADC 1
#define STM32F4_AD_CHANNELS {0,1,2,3,4,8,9,10,11,12,13,14,15} // PA0-PA7, PB0,PB1 PC1-PC5

// PWM Configuration
#define STM32F4_PWM_TIMER    {  10,  11,   9,   2,   9,  12,   1,   1,   1,   1} /* timer numbers use one-based index */
#define STM32F4_PWM_CHNL  {   0,   0,   0,   2,   0,   1,   2,   0,   1,   3} /* channel values use zero-based index */
#define STM32F4_PWM_PINS      {0x18,0x19,0x45,0x1A,0x02,0x1F,0x0A,0x49,0x4B,0x4E}

#define STM32F4_SPI_SCLK_PINS {0x05,0x1D,0x2A,0x42} // PA5,PB13,PC10,PE2
#define STM32F4_SPI_MISO_PINS {0x06,0x1E,0x2B,0x4D} // PA6,PB14, PC11,PE13
#define STM32F4_SPI_MOSI_PINS {0x07,0x1F,0x2C,0x46} // PA7,PB15, PC12,PE6

#define STM32F4_I2C_PORT     1
#define STM32F4_I2C_SCL_PIN  22  // PB6
#define STM32F4_I2C_SDA_PIN  23  // PB7

#define STM32F4_UART_TXD_PINS  {  0x26,  0x02,  0x00,  0x16,  0x38,  0x48,  0x41}
#define STM32F4_UART_RXD_PINS  {  0x27,  0x03,  0x01,  0x17,  0x39,  0x47,  0x40}
#define STM32F4_UART_RTS_PINS {  0xFF,  0x01,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF} /* 0xFF = unused */
#define STM32F4_UART_CTS_PINS {  0xFF,  0x00,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF} /* 0xFF = unused */

// GPIO Configuration
#define STM32_GPIO_USER_LED	    0x0A
#define STM32_GPIO_RESET_BTN	    0x15 /* NOTE: we will need to make this "software interrupt" when digital header pin D9 is used as interrupt */
//#define STM32_GPIO_RESET_BTN	    0x3B /* NOTE: this conflicts with the MicroSD card insertion */

//
// constants
/////////////////////////////////////////////////////////

#include <processor_selector.h>

#endif // PLATFORM_ARM_Netduino3

#endif // _PLATFORM_Netduino3_SELECTOR_H_
