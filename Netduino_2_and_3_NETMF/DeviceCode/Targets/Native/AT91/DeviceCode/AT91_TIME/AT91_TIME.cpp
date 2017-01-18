////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "AT91_TIME.h"

//--//

#undef  TRACE_ALWAYS

#define TRACE_ALWAYS               0x00000001
#define TRACE_COUNTER_READS        0x00000002
#define TRACE_COUNTER_OVERFLOWS    0x00000004
#define TRACE_SETCOMPARE           0x00000008
#define TRACE_ISR                  0x00000010

#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_AT91_TIME_Driver"
#endif

AT91_TIME_Driver g_AT91_TIME_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

BOOL AT91_TIME_Driver::Initialize()
{
    g_AT91_TIME_Driver.m_lastRead    = 0;
    g_AT91_TIME_Driver.m_nextCompare = (UINT64) AT91_TIMER_Driver::c_MaxTimerValue;

    if(!AT91_TIMER_Driver::Initialize( AT91_TIMER_Driver::c_SystemTimer, TRUE, AT91_TC::TC_CLKS_TIMER_DIV5_CLOCK, AT91_TIME_Driver::ISR, NULL ))
        return FALSE;

    AT91_TIMER_Driver::SetCompare( AT91_TIMER_Driver::c_SystemTimer, AT91_TIMER_Driver::c_MaxTimerValue );

    return TRUE;
}

BOOL AT91_TIME_Driver::Uninitialize()
{
    if(!AT91_TIMER_Driver::Uninitialize( AT91_TIMER_Driver::c_SystemTimer ))
        return FALSE;

    return TRUE;
}

UINT64 AT91_TIME_Driver::CounterValue()
{
    GLOBAL_LOCK(irq);

    UINT16 value = AT91_TIMER_Driver::ReadCounter( AT91_TIMER_Driver::c_SystemTimer );

    g_AT91_TIME_Driver.m_lastRead &= (0xFFFFFFFFFFFF0000ull);

    if(AT91_TIMER_Driver::DidTimerOverFlow( AT91_TIMER_Driver::c_SystemTimer ))
    {
        g_AT91_TIME_Driver.m_lastRead += (0x1ull << 16);
    }

    g_AT91_TIME_Driver.m_lastRead |= value;

    return g_AT91_TIME_Driver.m_lastRead;
}

void AT91_TIME_Driver::SetCompareValue( UINT64 CompareValue )
{
    GLOBAL_LOCK(irq);

    g_AT91_TIME_Driver.m_nextCompare = CompareValue;

    bool fForceInterrupt = false;

    UINT64 CntrValue = CounterValue();

    if(CompareValue <= CntrValue)
    {
        fForceInterrupt = true;
    }
    else
    {
        UINT32 diff;

        if((CompareValue - CntrValue) > AT91_TIMER_Driver::c_MaxTimerValue)
        {
            diff = AT91_TIMER_Driver::c_MaxTimerValue;
        }
        else
        {
            diff = (UINT32)(CompareValue - CntrValue);
        }        

        AT91_TIMER_Driver::SetCompare( AT91_TIMER_Driver::c_SystemTimer, 
            (UINT16)(AT91_TIMER_Driver::ReadCounter( AT91_TIMER_Driver::c_SystemTimer ) + diff) );

        if(CounterValue() > CompareValue)
        {
            fForceInterrupt = true;
        }
    }

    if(fForceInterrupt)
    {
        // Force interrupt to process this.
        AT91_TIMER_Driver::ForceInterrupt( AT91_TIMER_Driver::c_SystemTimer);
    }
}

//--//

void AT91_TIME_Driver::ISR( void* Param )
{
    if(CounterValue() >= g_AT91_TIME_Driver.m_nextCompare)
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
        SetCompareValue( g_AT91_TIME_Driver.m_nextCompare );
    }
}

//--//

INT64 AT91_TIME_Driver::TicksToTime( UINT64 Ticks )
{
    return CPU_TicksToTime( Ticks );
}


INT64 AT91_TIME_Driver::CurrentTime()
{
    return CPU_TicksToTime( CounterValue() );
}

//--//

//
// Since this is probably calibrated for RAM execution and it's used by the FLASH routines,
// make sure to have this function in RAM...
//
#pragma arm section code = "SectionForFlashOperations"

//
// To calibrate this constant, uncomment #define CALIBRATE_SLEEP_USEC in TinyHAL.c
//
#define AT91_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS 4

void __section(SectionForFlashOperations) AT91_TIME_Driver::Sleep_uSec( UINT32 uSec )
{
    GLOBAL_LOCK(irq);

    UINT32 value   = AT91_TIMER_Driver::ReadCounter( AT91_TIMER_Driver::c_SystemTimer );
    UINT32 maxDiff  = CPU_MicrosecondsToTicks( uSec );      // The free-running timer clocks at a constant 3.25 MHz

    if(maxDiff <= AT91_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS) maxDiff  = AT91_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS;
    else                                                 maxDiff -= AT91_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS;

    while((AT91_TIMER_Driver::ReadCounter( AT91_TIMER_Driver::c_SystemTimer ) - value) <= maxDiff);
}

// This routine is not designed for very accurate time delays.  It is designed to insure a minimum
// delay.  It is implemented using a simple timing loop that assumes the best of conditions - that
// is to say: turbo mode, cache, and branch prediction enabled, no cache misses and no interrupts
// while it is executing.  Since this cannot be guaranteed, it is possible and even likely that
// this routine will take a bit longer to return than the requested time - but it will not return
// in less time.

void __section(SectionForFlashOperations) AT91_TIME_Driver::Sleep_uSec_Loop( UINT32 uSec )
{
    // iterations must be signed so that negative iterations will result in the minimum delay

    uSec *= (SYSTEM_CYCLE_CLOCK_HZ / CLOCK_COMMON_FACTOR);
    uSec /= (ONE_MHZ               / CLOCK_COMMON_FACTOR);

    // iterations is equal to the number of CPU instruction cycles in the required time minus
    // overhead cycles required to call this subroutine.
    int iterations = (int)uSec - 14;      // Subtract off call & calculation overhead

    CYCLE_DELAY_LOOP2(iterations);
}

#pragma arm section code
