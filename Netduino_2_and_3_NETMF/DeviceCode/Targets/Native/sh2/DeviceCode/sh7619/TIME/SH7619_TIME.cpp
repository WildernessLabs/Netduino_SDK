////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SH7619_TIME.h"
#include "..\Timer\SH7619_TIMER.h"

//--//

SH7619_TIME_Driver g_SH7619_TIME_Driver;

//--//

BOOL SH7619_TIME_Driver::Initialize()
{
    g_SH7619_TIME_Driver.m_lastRead    = 0;
    g_SH7619_TIME_Driver.m_nextCompare = (UINT64) SH7619_TIMER_Driver::c_MaxTimerValue;

    if(!SH7619_TIMER_Driver::Initialize( SH7619_TIMER_Driver::c_Timer_System, SH7619_TIMER_Driver::CLOCK_SOURCE_DIV512, SH7619_TIME_Driver::Overflow_ISR, NULL ))
        return FALSE;

    if(!SH7619_TIMER_Driver::Initialize( SH7619_TIMER_Driver::c_Timer_Compare, SH7619_TIMER_Driver::CLOCK_SOURCE_DIV512, SH7619_TIME_Driver::Compare_ISR, NULL ))
        return FALSE;

    return TRUE;
}

BOOL SH7619_TIME_Driver::Uninitialize()
{
    if(!SH7619_TIMER_Driver::Uninitialize( SH7619_TIMER_Driver::c_Timer_System ))
        return FALSE;

    if(!SH7619_TIMER_Driver::Uninitialize( SH7619_TIMER_Driver::c_Timer_Compare ))
        return FALSE;

    return TRUE;
}

UINT64 SH7619_TIME_Driver::CounterValue()
{
    return g_SH7619_TIME_Driver.m_lastRead + SH7619_TIMER_Driver::ReadCounter( SH7619_TIMER_Driver::c_Timer_System );
}

void SH7619_TIME_Driver::SetCompareValue( UINT64 CompareValue )
{
    GLOBAL_LOCK(irq);

    g_SH7619_TIME_Driver.m_nextCompare = CompareValue;

    UINT64 cntr = CounterValue();

    if(cntr > CompareValue)
    {
        SH7619_TIMER_Driver::ForceInterrupt( SH7619_TIMER_Driver::c_Timer_Compare );
    }
    else
    {
        UINT64 diff = CompareValue - cntr;

        if (diff > SH7619_TIMER_Driver::c_MaxTimerValue)
        {
            diff = SH7619_TIMER_Driver::c_MaxTimerValue;
        }

        SH7619_TIMER_Driver::SetCompare( SH7619_TIMER_Driver::c_Timer_Compare, (UINT16)diff );

        if(CounterValue() > CompareValue)
        {
            SH7619_TIMER_Driver::ForceInterrupt( SH7619_TIMER_Driver::c_Timer_Compare );
        }
    }
}

//--//

void SH7619_TIME_Driver::Overflow_ISR( void* Param )
{
    g_SH7619_TIME_Driver.m_lastRead += SH7619_TIMER_Driver::c_MaxTimerValue;
}

void SH7619_TIME_Driver::Compare_ISR( void* Param )
{
    if(CounterValue() >= g_SH7619_TIME_Driver.m_nextCompare)
    {
        // this also schedules the next one, if there is one
        HAL_COMPLETION::DequeueAndExec();
    }
    else
    {
        //
        // Because we are limited in the resolution of timer,
        // resetting the compare will properly configure the next interrupt.
        //
        SetCompareValue( g_SH7619_TIME_Driver.m_nextCompare );
    }
}

//--//

INT64 SH7619_TIME_Driver::TicksToTime( UINT64 Ticks )
{
    return CPU_TicksToTime( Ticks );
}


INT64 SH7619_TIME_Driver::CurrentTime()
{
    return CPU_TicksToTime( CounterValue() );
}

//--//

//
// Since this is probably calibrated for RAM execution and it's used by the FLASH routines,
// make sure to have this function in RAM...
//
//#pragma section RAM_CODE

//
// To calibrate this constant, uncomment #define CALIBRATE_SLEEP_USEC in TinyHAL.c
//
#define SH7619_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS 4

void SH7619_TIME_Driver::Sleep_uSec( UINT32 uSec )
{
    GLOBAL_LOCK(irq);

    UINT64 value    = CounterValue();
    UINT64 maxDiff  = CPU_MicrosecondsToTicks( uSec );      // The free-running timer clocks at a constant 3.25 MHz

    if(maxDiff <= SH7619_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS) maxDiff  = SH7619_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS;
    else                                                   maxDiff -= SH7619_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS;

    while((CounterValue() - value) <= maxDiff);
}

// This routine is not designed for very accurate time delays.  It is designed to insure a minimum
// delay.  It is implemented using a simple timing loop that assumes the best of conditions - that
// is to say: turbo mode, cache, and branch prediction enabled, no cache misses and no interrupts
// while it is executing.  Since this cannot be guaranteed, it is possible and even likely that
// this routine will take a bit longer to return than the requested time - but it will not return
// in less time.

void SH7619_TIME_Driver::Sleep_uSec_Loop( UINT32 uSec )
{
    // CODE TAKEN FROM LCD Delay_MS - this should be recalibrated and placed in RAM
    
    unsigned long i;
    
    i = 31250;          // 8 ns/instruction * 4 instructions * loop_count = 1ms = 1000000 ns
    i *= uSec;
    i /= 1000;
    while (i--);        /* loop count */
}

//#pragma section RAM_CODE

