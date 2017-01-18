//-----------------------------------------------------------------------------
// Software that is described herein is for illustrative purposes only  
// which provides customers with programming information regarding the  
// products. This software is supplied "AS IS" without any warranties.  
// NXP Semiconductors assumes no responsibility or liability for the 
// use of the software, conveys no license or title under any patent, 
// copyright, or mask work right to the product. NXP Semiconductors 
// reserves the right to make changes in the software without 
// notification. NXP Semiconductors also make no representation or 
// warranty that such application will be suitable for the specified 
// use without further testing or modification. 
//-----------------------------------------------------------------------------

#include <tinyhal.h>
#include "..\LPC22XX.h"
//--//

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_LPC22XX_TIME_Driver"
#endif

LPC22XX_TIME_Driver g_LPC22XX_TIME_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//


void HAL_Time_SetCompare( UINT64 CompareValue )
{
    LPC22XX_TIME_Driver::SetCompareValue( CompareValue );
}


void HAL_Time_Sleep_MicroSeconds_InterruptEnabled( UINT32 uSec )
{
    LPC22XX_TIME_Driver::Sleep_uSec_Loop( uSec );
}

BOOL HAL_Time_Initialize()
{
    return LPC22XX_TIME_Driver::Initialize();
}

BOOL HAL_Time_Uninitialize()
{
    return LPC22XX_TIME_Driver::Uninitialize();
}

UINT64 HAL_Time_CurrentTicks()
{
    return LPC22XX_TIME_Driver::CounterValue();
}

/// Remove this method once dependency on this are gone.
UINT64 Time_CurrentTicks()
{
    return HAL_Time_CurrentTicks();
}

INT64 HAL_Time_TicksToTime( UINT64 Ticks )
{
    return LPC22XX_TIME_Driver::TicksToTime( Ticks );
}

INT64 HAL_Time_CurrentTime()
{
    return LPC22XX_TIME_Driver::CurrentTime();
}

void HAL_Time_GetDriftParameters  ( INT32* a, INT32* b, INT64* c )
{
    *a = 1;
    *b = 1;
    *c = 0;
}


//--//

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
#if ONE_MHZ < SLOW_CLOCKS_PER_SECOND
    return uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
#else
    return uSec / (ONE_MHZ / SLOW_CLOCKS_PER_SECOND);
#endif
}

UINT32 __section(SectionForFlashOperations) CPU_MicrosecondsToTicks( UINT32 uSec )
{
#if ONE_MHZ < SLOW_CLOCKS_PER_SECOND
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

int CPU_SystemClocksToMicroseconds( int Ticks )
{
    Ticks *= (ONE_MHZ        /CLOCK_COMMON_FACTOR);
    Ticks /= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);

    return Ticks;
}



BOOL LPC22XX_TIME_Driver::Initialize()
{
    g_LPC22XX_TIME_Driver.m_lastRead    = 0;
    g_LPC22XX_TIME_Driver.m_nextCompare = 0x0000FFFFFFFFFFFFull;

    if(!LPC22XX_TIMER_Driver::Initialize( LPC22XX_Driver::c_SystemTime_Timer, ISR, NULL )) return FALSE;

    //
    // This bootstraps the overflow mechanism.
    //
    LPC22XX_TIMER_Driver::SetCompare( 0, c_OverflowCheck );

    LPC22XX_TIMER_Driver::EnableCompareInterrupt( 0 );

    return TRUE;
}

BOOL LPC22XX_TIME_Driver::Uninitialize()
{
    if(!LPC22XX_TIMER_Driver::Uninitialize( 0 )) return FALSE;

    return TRUE;
}

UINT64 LPC22XX_TIME_Driver::CounterValue()
{
    UINT64 lastValue = g_LPC22XX_TIME_Driver.m_lastRead;

    UINT32 value = LPC22XX_TIMER_Driver::ReadCounter( LPC22XX_Driver::c_SystemTime_Timer );

    UINT32 resHigh = (UINT32)(lastValue >> 32);
    UINT32 resLow  = (UINT32) lastValue       ;

    //DEBUG_TRACE2(TRACE_COUNTER_READS,"CounterValue_Inner: %08x%08x ", resHigh, resLow);

    if((resLow & c_OverflowCheck) != (value & c_OverflowCheck))
    {
        if((value & c_OverflowCheck) == 0)
        {
            //DEBUG_TRACE3(TRACE_COUNTER_OVERFLOWS,"CounterValue: Overflow %08x %08x=>%08x", resHigh, resLow, value);

            resHigh += 1;
        }
    }

    return (UINT64)resHigh << 32 | value;
}

void LPC22XX_TIME_Driver::SetCompareValue( UINT64 CompareValue )
{
    GLOBAL_LOCK(irq);

    g_LPC22XX_TIME_Driver.m_nextCompare = CompareValue;

    UINT32 highComp = (UINT32)(CompareValue >> 32);
    UINT32 lowComp  = (UINT32) CompareValue       ;

    UINT32 highRead = (UINT32)(g_LPC22XX_TIME_Driver.m_lastRead >> 32);
    UINT32 lowRead  = (UINT32) g_LPC22XX_TIME_Driver.m_lastRead       ;

    bool fForceInterrupt = false;

    //DEBUG_TRACE4(TRACE_SETCOMPARE,"SetCompareValue: Comp:%08x%08x : Read:%08x%08x ", highComp, lowComp, highRead, lowRead);

    UINT32 lowReadNew = LPC22XX_TIMER_Driver::ReadCounter( LPC22XX_Driver::c_SystemTime_Timer );

    //DEBUG_TRACE1(TRACE_SETCOMPARE,"%08x\r\n", lowReadNew);

    if((lowRead & c_OverflowCheck) != (lowReadNew & c_OverflowCheck))
    {
        fForceInterrupt = true; //DEBUG_TRACE1(TRACE_SETCOMPARE,"ForceInterrupt: %d\r\n", __LINE__);
    }
    else
    {
        lowRead = lowReadNew;

        if(highComp < highRead)
        {
            fForceInterrupt = true; //DEBUG_TRACE1(TRACE_SETCOMPARE,"ForceInterrupt: %d\r\n", __LINE__);
        }
        else if(highComp == highRead)
        {
            if(lowComp <= lowRead)
            {
                fForceInterrupt = true; //DEBUG_TRACE1(TRACE_SETCOMPARE,"ForceInterrupt: %d\r\n", __LINE__);
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

            LPC22XX_TIMER_Driver::SetCompare( 0, nextComp );

            lowReadNew = LPC22XX_TIMER_Driver::ReadCounter( LPC22XX_Driver::c_SystemTime_Timer );

            INT32 diff = nextComp - lowReadNew;

            if(diff < 0)
            {
                //
                // We missed it, make sure we process an interrupt anyway.
                //
                fForceInterrupt = true; //DEBUG_TRACE4(TRACE_SETCOMPARE,"ForceInterrupt: %d %d %08x %08x\r\n", __LINE__, diff, nextComp, lowReadNew);
            }
        }
    }

    if(fForceInterrupt)
    {
        // Force interrupt to process this.
        LPC22XX_TIMER_Driver::ForceInterrupt( 0 );
    }
}

//--//

void LPC22XX_TIME_Driver::ISR( void* Param )
{
    //DEBUG_TRACE0(TRACE_SETCOMPARE,"LPC22XX_TIME_Driver::ISR\r\n");

    if(LPC22XX_TIMER_Driver::DidCompareHit( 0 ))
    {
        LPC22XX_TIMER_Driver::ResetCompareHit( 0 );
    }

    g_LPC22XX_TIME_Driver.m_lastRead = CounterValue();

    if(g_LPC22XX_TIME_Driver.m_lastRead >= g_LPC22XX_TIME_Driver.m_nextCompare)
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
        SetCompareValue( g_LPC22XX_TIME_Driver.m_nextCompare );
    }
}

//--//

INT64 LPC22XX_TIME_Driver::TicksToTime( UINT64 Ticks )
{
    return CPU_TicksToTime( Ticks );
}


INT64 LPC22XX_TIME_Driver::CurrentTime()
{
    //return HAL_Time_TicksToTime(Time_CurrentTicks());
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
#define LPC22XX_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS 3

void __section(SectionForFlashOperations) HAL_Time_Sleep_MicroSeconds( UINT32 uSec )
{
    LPC22XX_TIME_Driver::Sleep_uSec( uSec );
}



void __section(SectionForFlashOperations) LPC22XX_TIME_Driver::Sleep_uSec( UINT32 uSec )
{
    GLOBAL_LOCK(irq);

    UINT32 value   = LPC22XX_TIMER_Driver::ReadCounter( LPC22XX_Driver::c_SystemTime_Timer );
    UINT32 maxDiff = CPU_MicrosecondsToTicks( uSec ) - LPC22XX_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS;

    if(maxDiff <= LPC22XX_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS) maxDiff  = LPC22XX_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS;
    else                                                      maxDiff -= LPC22XX_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS;

    while(((INT32)(LPC22XX_TIMER_Driver::ReadCounter( LPC22XX_Driver::c_SystemTime_Timer ) - value)) <= maxDiff);
}


// This routine is not designed for very accurate time delays.  It is designed to insure a minimum
// delay.  It is implemented using a simple timing loop that assumes the best of conditions - that
// is to say: no cache misses and no interrupts while it is executing.  Since this cannot be guaranteed,
// it is possible and even likely that this routine will take a bit longer to return than the
// requested time - but it will not return in less time.

void __section(SectionForFlashOperations) LPC22XX_TIME_Driver::Sleep_uSec_Loop( UINT32 uSec )
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


