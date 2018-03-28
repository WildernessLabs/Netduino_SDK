////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
//  Copyright (c) Microsoft Corporation. All rights reserved.
//  Portions Copyright (c) Oberon microsystems, Inc.
//  Portions Copyright (c) Secret Labs LLC and the Netduino community. All rights reserved.
//
//  *** AD Conversion ***
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#if defined(PLATFORM_ARM_STM32F4_ANY)
#include "..\stm32f4xx.h"
#elif defined(PLATFORM_ARM_STM32F2_ANY)
#include "..\stm32f2xx.h"
#endif

///////////////////////////////////////////////////////////////////////////////

#define STM32_AD_CHANNELS 6       // number of channels
#define STM32_AD_FIRST_PIN 32     // channel 0 pin (C0)
#define STM32_AD_FIRST_CHANNEL 10 // channel 0 hw channel
#define STM32_AD_SAMPLE_TIME 2    // sample time = 28 cycles

//--//

BOOL AD_Initialize( ANALOG_CHANNEL channel, INT32 precisionInBits )
{
    if (!(RCC->APB2ENR & RCC_APB2ENR_ADC1EN)) { // not yet initialized
        RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; // enable AD clock
        ADC->CCR = 0; // set ADCCLK to default (PCLK / 2), disable temp sensor and vbat measurements
        ADC1->SQR1 = 0; // 1 conversion
        ADC1->CR1 = 0; // 12-bit resolution, interrupts disabled, analog watchdog disabled
        ADC1->CR2 = ADC_CR2_ADON; // AD on
        ADC1->SMPR1 = 0x00249249 * STM32_AD_SAMPLE_TIME;
        ADC1->SMPR2 = 0x09249249 * STM32_AD_SAMPLE_TIME;
    }
    // set pin as analog input
    CPU_GPIO_DisablePin(AD_GetPinForChannel(channel), RESISTOR_DISABLED, 0, GPIO_ALT_MODE_4);
    return TRUE;
}

void AD_Uninitialize( ANALOG_CHANNEL channel )
{
    // free pin
    CPU_GPIO_DisablePin(AD_GetPinForChannel(channel), RESISTOR_DISABLED, 0, GPIO_ALT_PRIMARY);
}

INT32 AD_Read( ANALOG_CHANNEL channel )
{
    int x = ADC1->DR; // clear EOC flag
    ADC1->SQR3 = STM32_AD_FIRST_CHANNEL + channel; // select channel
	ADC1->CR2 |= ADC_CR2_SWSTART;
    while (!(ADC1->SR & ADC_SR_EOC)); // wait for completion
    return ADC1->DR; // read result
}

UINT32 AD_ADChannels()
{
    return STM32_AD_CHANNELS;
}

GPIO_PIN AD_GetPinForChannel( ANALOG_CHANNEL channel )
{
    if ((UINT32)channel >= STM32_AD_CHANNELS) return GPIO_PIN_NONE;
    return STM32_AD_FIRST_PIN + channel;
}

BOOL AD_GetAvailablePrecisionsForChannel( ANALOG_CHANNEL channel, INT32* precisions, UINT32& size )
{
    size = 0;
    if (precisions == NULL || (UINT32)channel >= STM32_AD_CHANNELS) return FALSE;
    precisions[0] = 12;
    size = 1;
    return TRUE;
}
