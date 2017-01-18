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
#pragma arm section zidata = "g_MC9328MXL_TIME_Driver"
#endif

MC9328MXL_TIME_Driver g_MC9328MXL_TIME_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

BOOL MC9328MXL_TIME_Driver::Initialize()
{
    g_MC9328MXL_TIME_Driver.m_lastRead    = 0;
    g_MC9328MXL_TIME_Driver.m_nextCompare = 0x0000FFFFFFFFFFFFull;

    if(!MC9328MXL_TIMER_Driver::Initialize( MC9328MXL_Driver::c_SystemTime_Timer, TRUE, MC9328MXL_TIMER::TCTL_CLKSOURCE__PERCLK1, 1, ISR, NULL )) return FALSE;

    //
    // This bootstraps the overflow mechanism.
    //
    MC9328MXL_TIMER_Driver::SetCompare( 0, c_OverflowCheck );

    MC9328MXL_TIMER_Driver::EnableCompareInterrupt( 0 );

    return TRUE;
}

BOOL MC9328MXL_TIME_Driver::Uninitialize()
{
    if(!MC9328MXL_TIMER_Driver::Uninitialize( 0 )) return FALSE;

    return TRUE;
}

UINT64 MC9328MXL_TIME_Driver::CounterValue()
{
    UINT64 lastValue = g_MC9328MXL_TIME_Driver.m_lastRead;

    UINT32 value = MC9328MXL_TIMER_Driver::ReadCounter( MC9328MXL_Driver::c_SystemTime_Timer );

    UINT32 resHigh = (UINT32)(lastValue >> 32);
    UINT32 resLow  = (UINT32) lastValue       ;

    DEBUG_TRACE2(TRACE_COUNTER_READS,"CounterValue_Inner: %08x%08x ", resHigh, resLow);

    if((resLow & c_OverflowCheck) != (value & c_OverflowCheck))
    {
        if((value & c_OverflowCheck) == 0)
        {
            DEBUG_TRACE3(TRACE_COUNTER_OVERFLOWS,"CounterValue: Overflow %08x %08x=>%08x", resHigh, resLow, value);

            resHigh += 1;
        }
    }

    return (UINT64)resHigh << 32 | value;
}

void MC9328MXL_TIME_Driver::SetCompareValue( UINT64 CompareValue )
{
    GLOBAL_LOCK(irq);

    g_MC9328MXL_TIME_Driver.m_nextCompare = CompareValue;

    UINT32 highComp = (UINT32)(CompareValue >> 32);
    UINT32 lowComp  = (UINT32) CompareValue       ;

    UINT32 highRead = (UINT32)(g_MC9328MXL_TIME_Driver.m_lastRead >> 32);
    UINT32 lowRead  = (UINT32) g_MC9328MXL_TIME_Driver.m_lastRead       ;

    bool fForceInterrupt = false;

    DEBUG_TRACE4(TRACE_SETCOMPARE,"SetCompareValue: Comp:%08x%08x : Read:%08x%08x ", highComp, lowComp, highRead, lowRead);

    UINT32 lowReadNew = MC9328MXL_TIMER_Driver::ReadCounter( MC9328MXL_Driver::c_SystemTime_Timer );

    DEBUG_TRACE1(TRACE_SETCOMPARE,"%08x\r\n", lowReadNew);

    if((lowRead & c_OverflowCheck) != (lowReadNew & c_OverflowCheck))
    {
        fForceInterrupt = true; DEBUG_TRACE1(TRACE_SETCOMPARE,"ForceInterrupt: %d\r\n", __LINE__);
    }
    else
    {
        lowRead = lowReadNew;

        if(highComp < highRead)
        {
            fForceInterrupt = true; DEBUG_TRACE1(TRACE_SETCOMPARE,"ForceInterrupt: %d\r\n", __LINE__);
        }
        else if(highComp == highRead)
        {
            if(lowComp <= lowRead)
            {
                fForceInterrupt = true; DEBUG_TRACE1(TRACE_SETCOMPARE,"ForceInterrupt: %d\r\n", __LINE__);
            }
        }
        else
        {
            lowComp = 0xFFFFFFFF;
        }

        if(fForceInterrupt == false)
        {
            UINT32 nextComp = (lowRead & c_OverflowCheck) ? 0 : c_OverflowCheck;

            INT32 diff1 = (INT32)(lowComp  - lowRead);
            INT32 diff2 = (INT32)(nextComp - lowRead);

            if(diff1 > 0 && diff1 < diff2)
            {
                nextComp = lowComp;
            }

            MC9328MXL_TIMER_Driver::SetCompare( 0, nextComp );

            lowReadNew = MC9328MXL_TIMER_Driver::ReadCounter( MC9328MXL_Driver::c_SystemTime_Timer );

            INT32 diff = nextComp - lowReadNew;

            if(diff < 0)
            {
                //
                // We missed it, make sure we process an interrupt anyway.
                //
                fForceInterrupt = true; DEBUG_TRACE4(TRACE_SETCOMPARE,"ForceInterrupt: %d %d %08x %08x\r\n", __LINE__, diff, nextComp, lowReadNew);
            }
        }
    }

    if(fForceInterrupt)
    {
        // Force interrupt to process this.
        MC9328MXL_TIMER_Driver::ForceInterrupt( 0 );
    }
}

//--//

void MC9328MXL_TIME_Driver::ISR( void* Param )
{
    DEBUG_TRACE0(TRACE_SETCOMPARE,"MC9328MXL_TIME_Driver::ISR\r\n");

    if(MC9328MXL_TIMER_Driver::DidCompareHit( 0 ))
    {
        MC9328MXL_TIMER_Driver::ResetCompareHit( 0 );
    }

    g_MC9328MXL_TIME_Driver.m_lastRead = CounterValue();

    if(g_MC9328MXL_TIME_Driver.m_lastRead >= g_MC9328MXL_TIME_Driver.m_nextCompare)
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
        SetCompareValue( g_MC9328MXL_TIME_Driver.m_nextCompare );
    }
}

//--//

INT64 MC9328MXL_TIME_Driver::TicksToTime( UINT64 Ticks )
{
    return CPU_TicksToTime( Ticks );
}


INT64 MC9328MXL_TIME_Driver::CurrentTime()
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
#define MC9328MXL_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS 3

void __section(SectionForFlashOperations) MC9328MXL_TIME_Driver::Sleep_uSec( UINT32 uSec )
{
    GLOBAL_LOCK(irq);

    UINT32 value   = MC9328MXL_TIMER_Driver::ReadCounter( MC9328MXL_Driver::c_SystemTime_Timer );
    UINT32 maxDiff = CPU_MicrosecondsToTicks( uSec ) - MC9328MXL_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS;

    if(maxDiff <= MC9328MXL_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS) maxDiff  = MC9328MXL_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS;
    else                                                      maxDiff -= MC9328MXL_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS;

    while(((INT32)(MC9328MXL_TIMER_Driver::ReadCounter( MC9328MXL_Driver::c_SystemTime_Timer ) - value)) <= maxDiff);
}


// This routine is not designed for very accurate time delays.  It is designed to insure a minimum
// delay.  It is implemented using a simple timing loop that assumes the best of conditions - that
// is to say: no cache misses and no interrupts while it is executing.  Since this cannot be guaranteed,
// it is possible and even likely that this routine will take a bit longer to return than the
// requested time - but it will not return in less time.

void __section(SectionForFlashOperations) MC9328MXL_TIME_Driver::Sleep_uSec_Loop( UINT32 uSec )
{
    // iterations must be signed so that negative iterations will result in the minimum delay

    uSec *= (SYSTEM_CYCLE_CLOCK_HZ / CLOCK_COMMON_FACTOR);
    uSec /= (ONE_MHZ               / CLOCK_COMMON_FACTOR);

    // iterations is equal to the number of CPU instruction cycles in the required time minus
    // overhead cycles required to call this subroutine.
    int iterations = (int)uSec - 12;      // Subtract off call & calculation overhead

    CYCLE_DELAY_LOOP(iterations);
}


#pragma arm section code

