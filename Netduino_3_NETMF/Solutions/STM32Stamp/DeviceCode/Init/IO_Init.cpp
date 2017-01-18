////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
//  Copyright (c) Microsoft Corporation. All rights reserved.
//  Implementation for STM32Stamp board (STM32): Copyright (c) Oberon microsystems, Inc.
//
//  *** Board-specific IO Port Initialization ***
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "..\..\..\..\DeviceCode\Targets\Native\STM32\DeviceCode\stm32f10x.h"


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


void __section(SectionForBootstrapOperations) BootstrapCode_GPIO() {

    /* Enable GPIOA, GPIOB, and GPIOC clocks */  
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN;

    // pins:      15------8                 7------0
    GPIOA->CRH = 0x44444444; GPIOA->CRL = 0x44444444;
    GPIOB->CRH = 0x44444444; GPIOB->CRL = 0x44444444;
    GPIOC->CRH = 0x44444444; GPIOC->CRL = 0x44444444;

}
