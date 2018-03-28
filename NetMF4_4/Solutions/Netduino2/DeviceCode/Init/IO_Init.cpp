////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
//  Copyright (c) Microsoft Corporation. All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "..\..\..\..\DeviceCode\Targets\Native\Netduino_STM32\DeviceCode\stm32.h"
#if defined(PLATFORM_ARM_STM32F2_ANY)
//#include "..\..\..\..\DeviceCode\Targets\Native\Netduino_STM32\DeviceCode\stm32f2xx.h"
#elif defined(PLATFORM_ARM_STM32F4_ANY)
//#include "..\..\..\..\DeviceCode\Targets\Native\Netduino_STM32\DeviceCode\stm32f4xx.h"
#endif


/* Pin Configuration Register Values
    0x0  Analog Input
    0x1  Output 10Mhz
    0x2  Output  2Mhz
    0x3  Output 50Mhz
    0x4  Floating Input (default)
    0x5  Open-Drain 10Mhz
    0x6  Open-Drain  2Mhz
    0x7  Open-Drain 50Mhz
    0x8  Input with pull up/down
    0x9  Alternate Output 10Mhz
    0xA  Alternate Output  2Mhz
    0xB  Alternate Output 50Mhz
    0xC     -
    0xD  Alternate Open-Drain 10Mhz
    0xE  Alternate Open-Drain  2Mhz
    0xF  Alternate Open-Drain 50Mhz
*/

//#define Port(port) ((GPIO_TypeDef *) (GPIOA_BASE + (port << 10)))

void __section(SectionForBootstrapOperations) BootstrapCode_GPIO() {

    /* Enable GPIOA, GPIOB, GPIOC, and GPIOD clocks */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN;

#if defined(PLATFORM_ARM_NetduinoPlus2)
    GPIOA->MODER = 0xA8020000; // 20000 puts pin A8 in "alternate function mode"
#else
    GPIOA->MODER = 0xA8000000;
#endif
    GPIOB->MODER = 0x00000280;
    GPIOC->MODER = 0x00A00000;
    //GPIOD->MODER = 0x00002A80;

	GPIOA->OTYPER = 0x00000000; // default to push-pull (default state)
	GPIOB->OTYPER = 0x00000000;
	GPIOC->OTYPER = 0x00000000;
	//GPIOD->OTYPER = 0x00000000;

#if defined(PLATFORM_ARM_NetduinoPlus2)
    GPIOA->OSPEEDR = 0x00030000; // 30000 puts pin A8 in "100mhz mode"
#else
    GPIOA->OSPEEDR = 0x00000000;
#endif
    GPIOB->OSPEEDR = 0x000000C0;
    GPIOC->OSPEEDR = 0x00A00000;
    //GPIOD->OSPEEDR = 0x00002A80;

#if defined(PLATFORM_ARM_NetduinoPlus2)
    GPIOA->PUPDR = 0x64000000; // 00000 puts pin A8 in "no pullup/pulldown"
#else
    GPIOA->PUPDR = 0x64000000;
#endif
	GPIOB->PUPDR = 0x00000100;
    GPIOC->PUPDR = 0x00000000;
    //GPIOD->PUPDR = 0x00000000;

#if defined(PLATFORM_ARM_NetduinoPlus2)
    /* configure MCO1 to AF0 */
    STM32_GPIO_AFConfig(0x08, 0); // NOTE: this is the default mode, so setting this again is not technically necessary.
#else
	STM32_GPIO_AFConfig(0x08, 2); // NOTE: check to see if this is really necessary to disable pin 8 MCO
#endif

#if defined(PLATFORM_ARM_Netduino2) || defined(PLATFORM_ARM_NetduinoPlus2)
        // optionally...turn on power headers automatically (important if we're debugging/deploying via COM1 on pins D0/D1)
        CPU_GPIO_EnableOutputPin(0x12, TRUE); // PB2: CONTROL_OF_POWER_HEADERS		
#endif

#if defined(PLATFORM_ARM_NetduinoPlus2)
	// optionally...turn on SD card power automatically
    CPU_GPIO_EnableOutputPin(0x11, FALSE); // PB1: /PWR_CTRL_MICROSD
	// turn on ENC28J60 power automatically and enable reset
    CPU_GPIO_EnableOutputPin(0x2F, FALSE); // PC15: /PWR_CTRL_ETHERNET
    CPU_GPIO_EnableOutputPin(0x32, TRUE); // PD2: /ENC_RESET
#endif
}
