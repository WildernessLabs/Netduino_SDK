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
//  *** PWM Driver ***
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "..\STM32_GPIO\STM32_GPIO_functions.h";
#if defined(PLATFORM_ARM_STM32F4_ANY)
#include "..\stm32f4xx.h"
#elif defined(PLATFORM_ARM_STM32F2_ANY)
#include "..\stm32f2xx.h"
#endif

typedef  TIM_TypeDef* ptr_TIM_TypeDef;

//Timers, Channels, and Pins
static const UINT8 g_STM32_PWM_TimerNum[] = STM32_PWM_TIMER;
static const UINT8 g_STM32_PWM_Channel[] = STM32_PWM_CHANNEL;
static const UINT8 g_STM32_PWM_Pin[] = STM32_PWM_PIN;
#define STM32_PWM_CHANNELS ARRAYSIZE_CONST_EXPR(g_STM32_PWM_Pin) // number of channels

// IO addresses
static const ptr_TIM_TypeDef g_STM32_PWM_Timer[] =
    {TIM1, TIM2, TIM3, TIM4, TIM5, NULL, NULL, TIM8, TIM9, TIM10, TIM11, TIM12, TIM13, TIM14};
    
// Timer Alternate Functions
static const UINT8 g_STM32_PWM_TMR_AlternateFunction[] = {1,1,2,2,2,0,0,3,3,3,3,9,9,9};
// Timer Clocks
static const UINT32 g_STM32_PWM_TMR_ClockSpeed[] =
	{SYSTEM_APB2_CLOCK_HZ, SYSTEM_APB1_CLOCK_HZ, SYSTEM_APB1_CLOCK_HZ, SYSTEM_APB1_CLOCK_HZ, SYSTEM_APB1_CLOCK_HZ,
	 SYSTEM_APB1_CLOCK_HZ, SYSTEM_APB1_CLOCK_HZ, SYSTEM_APB2_CLOCK_HZ, SYSTEM_APB2_CLOCK_HZ, SYSTEM_APB2_CLOCK_HZ,
	 SYSTEM_APB2_CLOCK_HZ, SYSTEM_APB1_CLOCK_HZ, SYSTEM_APB1_CLOCK_HZ, SYSTEM_APB1_CLOCK_HZ};
// Clock Enable Bit
static const UINT32 g_STM32_PWM_TMR_ClockEnableBit[] = 
	{RCC_APB2ENR_TIM1EN, RCC_APB1ENR_TIM2EN, RCC_APB1ENR_TIM3EN, RCC_APB1ENR_TIM4EN, RCC_APB1ENR_TIM5EN,
	 RCC_APB1ENR_TIM6EN, RCC_APB1ENR_TIM7EN, RCC_APB2ENR_TIM8EN, RCC_APB2ENR_TIM9EN, RCC_APB2ENR_TIM10EN,
	 RCC_APB2ENR_TIM11EN, RCC_APB1ENR_TIM12EN, RCC_APB1ENR_TIM13EN, RCC_APB1ENR_TIM14EN};
//--//

BOOL PWM_Initialize(PWM_CHANNEL channel)
{
	if ((UINT32)channel >= STM32_PWM_CHANNELS)
		return FALSE;

	INT32 timerNum = g_STM32_PWM_TimerNum[channel];
	ptr_TIM_TypeDef timer = g_STM32_PWM_Timer[timerNum - 1];
	UINT32 timerChannel = g_STM32_PWM_Channel[channel];
	UINT32 clockSpeed = g_STM32_PWM_TMR_ClockSpeed[timerNum - 1];
	UINT32 clockEnableBit = g_STM32_PWM_TMR_ClockEnableBit[timerNum - 1];

    if(timer == TIM2 || timer == TIM3 || timer == TIM4 || timer == TIM5 || timer == TIM12 || timer == TIM13 || timer == TIM14)
    {
        if (!(RCC->APB1ENR & clockEnableBit)) { // not yet initialized
            RCC->APB1ENR |= clockEnableBit; // enable timer clock
            timer->CR1 = TIM_CR1_URS | TIM_CR1_ARPE; // double buffered update
            timer->PSC = ((clockSpeed / ONE_MHZ) * 2) - 1; // prescaler to 1MHz
            timer->EGR = TIM_EGR_UG; // enforce first update
        }
    } 
	else // if(timer == TIM1 | timer == TIM8 || timer == TIM9 || timer == TIM10 || timer == TIM11)
    {
        if (!(RCC->APB2ENR & clockEnableBit)) { // not yet initialized
            RCC->APB2ENR |= clockEnableBit; // enable timer clock
            timer->CR1 = TIM_CR1_URS | TIM_CR1_ARPE; // double buffered update
            timer->PSC = ((clockSpeed / ONE_MHZ) * 2) - 1; // prescaler to 1MHz
            timer->EGR = TIM_EGR_UG; // enforce first update
			
			if(timer == TIM1 || timer == TIM8)
				timer->BDTR |= TIM_BDTR_MOE; // main output enable (required for advanced timers TIM1 and TIM8)
        }
    }
	
	if (timerNum == 2 || timerNum == 5) // 32 bit timer
	{
		((uint32_t*)&timer->CCR1)[timerChannel] = 0; // reset compare register
	}
	else
	{
		*(__IO uint16_t*)&((uint32_t*)&timer->CCR1)[timerChannel] = 0; // reset compare register
	}

    // enable PWM channel
    UINT32 mode = TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1PE; // PWM1 mode, double buffered
    if (timerChannel & 1) mode <<= 8; // 1 or 3
    __IO uint16_t* reg = &timer->CCMR1;
    if (timerChannel & 2) reg = &timer->CCMR2; // 2 or 3
    *reg |= mode;

    return TRUE;
}

BOOL PWM_Uninitialize(PWM_CHANNEL channel)
{
    INT32 timerNum = g_STM32_PWM_TimerNum[channel];
    ptr_TIM_TypeDef timer = g_STM32_PWM_Timer[timerNum - 1];
    UINT32 timerChannel = g_STM32_PWM_Channel[channel];
    UINT32 clockEnableBit = g_STM32_PWM_TMR_ClockEnableBit[timerNum - 1];
    
    UINT32 mask = 0xFF; // disable PWM channel
    if (timerChannel & 1) mask = 0xFF00; // 1 or 3
    __IO uint16_t* reg = &timer->CCMR1;
    if (timerChannel & 2) reg = &timer->CCMR2; // 2 or 3
    *reg &= ~mask;

    if ((timer->CCMR1 | timer->CCMR2) == 0) // no channel active
	{
	    if(timer == TIM2 || timer == TIM3 || timer == TIM4 || timer == TIM5 || timer == TIM12 || timer == TIM13 || timer == TIM14)
        {
        	RCC->APB1ENR &= clockEnableBit; // disable timer clock
		}
		else // if(timer == TIM1 | timer == TIM8 || timer == TIM9 || timer == TIM10 || timer == TIM11)
        {
    		RCC->APB2ENR &= clockEnableBit; // disable timer clock
        }
    }

    return TRUE;
}

BOOL PWM_ApplyConfiguration(PWM_CHANNEL channel, GPIO_PIN pin, UINT32& period, UINT32& duration, PWM_SCALE_FACTOR& scale, BOOL invert)
{
    INT32 timerNum = g_STM32_PWM_TimerNum[channel];
    ptr_TIM_TypeDef timer = g_STM32_PWM_Timer[timerNum - 1];
    UINT32 timerChannel = g_STM32_PWM_Channel[channel];

	// adjust our period and duration to microseconds if necessary
	UINT32 periodUs, durationUs;
	if (scale == PWM_NANOSECONDS)
	{
		periodUs = period / 1000;
		durationUs = duration / 1000;
	}
	else if (scale == PWM_MICROSECONDS)
	{
		periodUs = period;
		durationUs = duration;
	}
	else if (scale == PWM_MILLISECONDS)
	{
		periodUs = period * 1000;
		durationUs = duration * 1000;
	}
	
	// validate that our inputs are within acceptable range
	if (timerNum != 2 && timerNum != 5) // 16 bit timer
	{
		if (periodUs >= 0x10000) // only first 16 bits of period are significant
			return FALSE;
	}
//	else // 32 bit timer
//	{
//		// TODO: determine if we need to only support lowest 31 bits
//		if (periodUs >= 0x80000000) // only lowest 31 bits of period are significant
//		if (periodUs >  0xFFFFFFFF) // all 32 bits of period are significant (this will never return FALSE)
//			return FALSE;
//	}
	
	if (durationUs >= periodUs) // only first 16 bits of duration are significant; also, must be <= period
		durationUs = periodUs;

    timer->ARR = periodUs - 1;

	if (timerNum == 2 || timerNum == 5) // 32 bit timer
	{
		((uint32_t*)&timer->CCR1)[timerChannel] = durationUs;
	}
	else
	{
		*(__IO uint16_t*)&((uint32_t*)&timer->CCR1)[timerChannel] = durationUs;
	}

    UINT32 invBit = TIM_CCER_CC1P << (4 * timerChannel);
    if (invert) {
        timer->CCER |= invBit;
    } else {
        timer->CCER &= ~invBit;
    }
    return TRUE;
}

BOOL PWM_Start(PWM_CHANNEL channel, GPIO_PIN pin)
{
    INT32 timerNum = g_STM32_PWM_TimerNum[channel];
    ptr_TIM_TypeDef timer = g_STM32_PWM_Timer[timerNum - 1];
	UINT8 alternateFunc = g_STM32_PWM_TMR_AlternateFunction[timerNum - 1];
    UINT32 timerChannel = g_STM32_PWM_Channel[channel];
    
    CPU_GPIO_DisablePin( pin, RESISTOR_DISABLED, 1, GPIO_ALT_MODE_1 );
    STM32_GPIO_AFConfig((UINT32)pin, alternateFunc);
    UINT16 enBit = TIM_CCER_CC1E << (4 * timerChannel);
    timer->CCER |= enBit; // enable output
    UINT16 cr1 = timer->CR1;
    if ((cr1 & TIM_CR1_CEN) == 0) { // timer stopped
        timer->EGR = TIM_EGR_UG; // enforce register update
        timer->CR1 = cr1 | TIM_CR1_CEN; // start timer
    }
    return TRUE;
}

void PWM_Stop(PWM_CHANNEL channel, GPIO_PIN pin)
{
    INT32 timerNum = g_STM32_PWM_TimerNum[channel];
    ptr_TIM_TypeDef timer = g_STM32_PWM_Timer[timerNum - 1];
    UINT32 timerChannel = g_STM32_PWM_Channel[channel];
    
    UINT16 ccer = timer->CCER;
    ccer &= ~(TIM_CCER_CC1E << (4 * timerChannel));
    timer->CCER = ccer; // disable output
    CPU_GPIO_DisablePin( pin, RESISTOR_DISABLED, 0, GPIO_ALT_PRIMARY );
    STM32_GPIO_AFConfig((UINT32)pin, 0);
    if ((ccer & (TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E)) == 0) { // idle
        timer->CR1 &= ~TIM_CR1_CEN; // stop timer
    }
#if defined(STM32_GPIO_USER_LED)
	// turn off LED when PWM is stopped
	if ((UINT32)pin == STM32_GPIO_USER_LED)
		CPU_GPIO_EnableOutputPin(STM32_GPIO_USER_LED, FALSE); // ONBOARD_LED
#endif
}

BOOL PWM_Start(PWM_CHANNEL* channel, GPIO_PIN* pin, UINT32 count)
{
	BOOL success = TRUE;
	
	for (int i = 0; i < count; i++)
	{
		if (!PWM_Start(channel[i], pin[i]))
			success = FALSE;
	}
    return success;
}

void PWM_Stop(PWM_CHANNEL* channel, GPIO_PIN* pin, UINT32 count)
{
    for (int i = 0; i < count; i++) 
	{
		PWM_Stop(channel[i], pin[i]);
    }
}

UINT32 PWM_PWMChannels()
{
    return STM32_PWM_CHANNELS;
}

GPIO_PIN PWM_GetPinForChannel( PWM_CHANNEL channel )
{
	if ((UINT32)channel >= STM32_PWM_CHANNELS)
		return GPIO_PIN_NONE;

    INT32 timerNum = g_STM32_PWM_TimerNum[channel];
	return (GPIO_PIN)g_STM32_PWM_Pin[channel];
}
