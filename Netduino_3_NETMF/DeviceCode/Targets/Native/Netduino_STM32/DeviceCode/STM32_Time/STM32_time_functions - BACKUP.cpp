////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
//  Copyright (c) Microsoft Corporation. All rights reserved.
//  Implementation for STM32: Copyright (c) Oberon microsystems, Inc.
//
//  *** System Timer Driver ***
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cores\arm\include\cpu.h>
#include "..\stm32.h"


static UINT64 g_nextEvent;   // tick time of next event to be scheduled
static UINT64 g_overflowCounter;

UINT32 CPU_SystemClock()
{
    return SYSTEM_CLOCK_HZ;
}

UINT32 CPU_TicksPerSecond()
{
    return SLOW_CLOCKS_PER_SECOND;
}

//--//

UINT64 CPU_TicksToTime( UINT64 Ticks )
{
    Ticks *= (TEN_MHZ               /SLOW_CLOCKS_TEN_MHZ_GCD);
    Ticks /= (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_TEN_MHZ_GCD);

    return Ticks;
}

UINT64 CPU_TicksToTime( UINT32 Ticks32 )
{
    UINT64 Ticks;

    Ticks  = (UINT64)Ticks32 * (TEN_MHZ               /SLOW_CLOCKS_TEN_MHZ_GCD);
    Ticks /=                   (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_TEN_MHZ_GCD);

    return Ticks;
}

//--//

UINT64 CPU_MillisecondsToTicks( UINT64 Ticks )
{
    Ticks *= (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_MILLISECOND_GCD);
    Ticks /= (1000                  /SLOW_CLOCKS_MILLISECOND_GCD);

    return Ticks;
}

UINT64 CPU_MillisecondsToTicks( UINT32 Ticks32 )
{
    UINT64 Ticks;

    Ticks  = (UINT64)Ticks32 * (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_MILLISECOND_GCD);
    Ticks /=                   (1000                  /SLOW_CLOCKS_MILLISECOND_GCD);

    return Ticks;
}

//--//

#pragma arm section code = "SectionForFlashOperations"

UINT64 __section(SectionForFlashOperations) CPU_MicrosecondsToTicks( UINT64 uSec )
{
#if ONE_MHZ <= SLOW_CLOCKS_PER_SECOND
    return uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
#else
    return uSec / (ONE_MHZ / SLOW_CLOCKS_PER_SECOND);
#endif
}

UINT32 __section(SectionForFlashOperations) CPU_MicrosecondsToTicks( UINT32 uSec )
{
#if ONE_MHZ <= SLOW_CLOCKS_PER_SECOND
    return uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
#else
    return uSec / (ONE_MHZ / SLOW_CLOCKS_PER_SECOND);
#endif
}

#pragma arm section code

//--//

UINT32 CPU_MicrosecondsToSystemClocks( UINT32 uSec )
{
    uSec *= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
    uSec /= (ONE_MHZ        /CLOCK_COMMON_FACTOR);

    return uSec;
}

int CPU_MicrosecondsToSystemClocks( int uSec )
{
    uSec *= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
    uSec /= (ONE_MHZ        /CLOCK_COMMON_FACTOR);

    return uSec;
}

//--//

int CPU_SystemClocksToMicroseconds( int Ticks )
{
    Ticks *= (ONE_MHZ        /CLOCK_COMMON_FACTOR);
    Ticks /= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);

    return Ticks;
}

//--//


void Timer_Interrupt (void* param) // timer2 compare event
{
    INTERRUPT_START

    TIM5->SR &= ~TIM_SR_CC1IF; // reset interrupt flag

    if (HAL_Time_CurrentTicks() >= g_nextEvent) { // handle event
       HAL_COMPLETION::DequeueAndExec(); // this also schedules the next one, if there is one
    }

    INTERRUPT_END
}


#pragma arm section code = "SectionForFlashOperations"

UINT64 __section(SectionForFlashOperations) HAL_Time_CurrentTicks()
{
	//g_overflowCounter &= (0xFFFFFFFFFFFF0000ull);

	if(TIM5->SR & TIM_SR_UIF)
	{
		TIM5->SR = ~TIM_SR_UIF;
    	g_overflowCounter += (0x1ull << 16);
	}

    return g_overflowCounter | TIM5->CNT;
}

#pragma arm section code

void HAL_Time_SetCompare( UINT64 CompareValue )
{
    GLOBAL_LOCK(irq);
    g_nextEvent = CompareValue;
    TIM5->CCR1 = (UINT16)CompareValue; // compare to low bits

    if (HAL_Time_CurrentTicks() >= CompareValue) { // missed event
        // trigger immediate interrupt
        TIM5->EGR = TIM_EGR_CC1G; // trigger compare1 event
    }

}

#if SYSTEM_APB1_CLOCK_HZ == SYSTEM_CYCLE_CLOCK_HZ
#define TIM5CLK_HZ SYSTEM_APB1_CLOCK_HZ
#else
#define TIM5CLK_HZ (SYSTEM_APB1_CLOCK_HZ * 2)
#endif

BOOL HAL_Time_Initialize()
{
    g_nextEvent = 0xFFFFFFFFFFFF; // never
    g_overflowCounter = 0;

    // enable timer 2-4 clocks
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

    // timer2
    TIM5->CR1 = TIM_CR1_URS;
    TIM5->CR2 = TIM_CR2_MMS_1; // master mode selection = update event
//    TIM5->CR2 = 0; 
    TIM5->SMCR = 0; // TS = 000, SMS = 000, internal clock
    TIM5->DIER = TIM_DIER_CC1IE; // enable compare1 interrupt
    TIM5->CCMR1 = 0; // compare, no outputs
    TIM5->CCMR2 = 0; // compare, no outputs
    TIM5->PSC = (TIM5CLK_HZ / SLOW_CLOCKS_PER_SECOND) - 1; // prescaler to 1MHz
    TIM5->CCR1 = 0;
    TIM5->ARR = 0xFFFF;
    TIM5->EGR = TIM_EGR_UG; // enforce first update

    TIM5->CR1 |= TIM_CR1_CEN;

    return CPU_INTC_ActivateInterrupt(TIM5_IRQn, Timer_Interrupt, 0);
}

BOOL HAL_Time_Uninitialize()
{
    CPU_INTC_DeactivateInterrupt(TIM5_IRQn);

    TIM5->CR1 &= ~TIM_CR1_CEN;

    // disable timer 2-4 clocks
    RCC->APB1ENR &= ~(RCC_APB1ENR_TIM5EN);

    return TRUE;
}


#pragma arm section code = "SectionForFlashOperations"

//
// To calibrate this constant, uncomment #define CALIBRATE_SLEEP_USEC in TinyHAL.c
//
#define STM32_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS 3

void __section(SectionForFlashOperations) HAL_Time_Sleep_MicroSeconds( UINT32 uSec )
{
    GLOBAL_LOCK(irq);

    UINT32 current   = HAL_Time_CurrentTicks();
    UINT32 maxDiff = CPU_MicrosecondsToTicks( uSec );

    if(maxDiff <= STM32_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS) maxDiff  = 0;
    else maxDiff -= STM32_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS;

    while(((INT32)(HAL_Time_CurrentTicks() - current)) <= maxDiff);
}

void HAL_Time_Sleep_MicroSeconds_InterruptEnabled( UINT32 uSec )
{
    // iterations must be signed so that negative iterations will result in the minimum delay

    uSec *= (SYSTEM_CYCLE_CLOCK_HZ / CLOCK_COMMON_FACTOR);
    uSec /= (ONE_MHZ               / CLOCK_COMMON_FACTOR);

    // iterations is equal to the number of CPU instruction cycles in the required time minus
    // overhead cycles required to call this subroutine.
    int iterations = (int)uSec - 5;      // Subtract off call & calculation overhead

    CYCLE_DELAY_LOOP(iterations);
}

#pragma arm section code

INT64 HAL_Time_TicksToTime( UINT64 Ticks )
{
    return CPU_TicksToTime( Ticks );
}

INT64 HAL_Time_CurrentTime()
{
    return CPU_TicksToTime( HAL_Time_CurrentTicks() );
}

void HAL_Time_GetDriftParameters  ( INT32* a, INT32* b, INT64* c )
{
    *a = 1;
    *b = 1;
    *c = 0;
}
