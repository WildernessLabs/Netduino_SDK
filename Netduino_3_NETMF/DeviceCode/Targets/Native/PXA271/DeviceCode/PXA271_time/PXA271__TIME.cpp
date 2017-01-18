////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

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
#pragma arm section zidata = "g_PXA271_TIME_Driver"
#endif

PXA271_TIME_Driver g_PXA271_TIME_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

BOOL PXA271_TIME_Driver::Initialize()
{
    g_PXA271_TIME_Driver.m_lastRead    = 0;
    g_PXA271_TIME_Driver.m_nextCompare = 0x0000FFFFFFFFFFFFull;

    if(!PXA271_TIMER_Driver::Initialize( PXA271_Driver::c_SystemTime_Timer, TRUE, PXA271_TIMER::OMCR__CRES_1uS, PXA271_TIMER::OMCR__XSYNC_NONE, ISR, NULL )) return FALSE;

    //
    // This bootstraps the overflow mechanism.
    //
    PXA271_TIMER_Driver::SetCompare( PXA271_Driver::c_SystemTime_Timer, c_OverflowCheck );

    PXA271_TIMER_Driver::EnableCompareInterrupt( PXA271_Driver::c_SystemTime_Timer );

    return TRUE;
}

BOOL PXA271_TIME_Driver::Uninitialize()
{
    if(!PXA271_TIMER_Driver::Uninitialize( PXA271_Driver::c_SystemTime_Timer )) return FALSE;

    return TRUE;
}

UINT64 PXA271_TIME_Driver::CounterValue()
{
    GLOBAL_LOCK(irq);
    
    UINT32 lastLowValue = (UINT32)(g_PXA271_TIME_Driver.m_lastRead & 0x00000000FFFFFFFFull);

    UINT32 value = PXA271_TIMER_Driver::ReadCounter( PXA271_Driver::c_SystemTime_Timer );

    g_PXA271_TIME_Driver.m_lastRead &= (0xFFFFFFFF00000000ull);

    if(lastLowValue > value )
    {
        DEBUG_TRACE3(TRACE_COUNTER_OVERFLOWS,"CounterValue: Overflow %08x %08x=>%08x", (g_PXA271_TIME_Driver.m_lastRead >> 32), lastLowValue, value);

        g_PXA271_TIME_Driver.m_lastRead += (0x1ull << 32);
    }

    g_PXA271_TIME_Driver.m_lastRead |= value;

    return g_PXA271_TIME_Driver.m_lastRead;
}

void PXA271_TIME_Driver::SetCompareValue( UINT64 CompareValue )
{
    GLOBAL_LOCK(irq);

    g_PXA271_TIME_Driver.m_nextCompare = CompareValue;

    bool fForceInterrupt = false;

    if(PXA271_TIMER_Driver::DidCompareHit( PXA271_Driver::c_SystemTime_Timer ))
    {
        PXA271_TIMER_Driver::ResetCompareHit( PXA271_Driver::c_SystemTime_Timer );
    }

    UINT64 CntrValue = CounterValue();

    if(CompareValue <= CntrValue)
    {
        fForceInterrupt = true; DEBUG_TRACE1(TRACE_SETCOMPARE,"ForceInterrupt: %d\r\n", __LINE__);
    }
    else
    {
        UINT32 diff;

        if((CompareValue - CntrValue) > 0xFFFFFFFFull)
        {
            diff = 0xFFFFFFFFul;
        }
        else
        {
            diff = (UINT32)(CompareValue - CntrValue);
        }        

        PXA271_TIMER_Driver::SetCompare( PXA271_Driver::c_SystemTime_Timer, 
            PXA271_TIMER_Driver::ReadCounter( PXA271_Driver::c_SystemTime_Timer ) + diff );

        if(CounterValue() > CompareValue)
        {
            fForceInterrupt = true;
        }
    }

    if(fForceInterrupt)
    {
        // Force interrupt to process this.
        PXA271_TIMER_Driver::ForceInterrupt( PXA271_Driver::c_SystemTime_Timer );
    }
}

//--//

void PXA271_TIME_Driver::ISR( void* Param )
{
    DEBUG_TRACE0(TRACE_SETCOMPARE,"PXA271_TIME_Driver::ISR\r\n");

    if(PXA271_TIMER_Driver::DidCompareHit( PXA271_Driver::c_SystemTime_Timer ))
    {
        PXA271_TIMER_Driver::ResetCompareHit( PXA271_Driver::c_SystemTime_Timer );
    }

    if(CounterValue() >= g_PXA271_TIME_Driver.m_nextCompare)
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
        SetCompareValue( g_PXA271_TIME_Driver.m_nextCompare );
    }
}

//--//

INT64 PXA271_TIME_Driver::TicksToTime( UINT64 Ticks )
{
    return CPU_TicksToTime( Ticks );
}


INT64 PXA271_TIME_Driver::CurrentTime()
{
    //return Time_TicksToTime(Time_CurrentTicks());
    // we collapse the above to improve perf on a high hit function

    INT64 Time = CPU_TicksToTime( CounterValue() );

#if defined(HAL_TIMEWARP)
    return s_timewarp_compensate + Time;
#else
    return Time;
#endif
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
#define PXA271_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS 4

void __section(SectionForFlashOperations) PXA271_TIME_Driver::Sleep_uSec( UINT32 uSec )
{
    GLOBAL_LOCK(irq);

    UINT32 value   = PXA271_TIMER_Driver::ReadCounter( PXA271_Driver::c_SystemTime_Timer );
    UINT32 maxDiff  = CPU_MicrosecondsToSystemClocks( uSec );      // The free-running timer clocks at a constant 3.25 MHz

    if(maxDiff <= PXA271_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS) maxDiff  = PXA271_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS;
    else                                                   maxDiff -= PXA271_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS;

    while((PXA271_TIMER_Driver::ReadCounter( PXA271_Driver::c_SystemTime_Timer ) - value) <= maxDiff);
}

// This routine is not designed for very accurate time delays.  It is designed to insure a minimum
// delay.  It is implemented using a simple timing loop that assumes the best of conditions - that
// is to say: turbo mode, cache, and branch prediction enabled, no cache misses and no interrupts
// while it is executing.  Since this cannot be guaranteed, it is possible and even likely that
// this routine will take a bit longer to return than the requested time - but it will not return
// in less time.

void __section(SectionForFlashOperations) PXA271_TIME_Driver::Sleep_uSec_Loop( UINT32 uSec )
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
