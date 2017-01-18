////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cores\arm\include\cpu.h>
#include "AT91_TIME.h"


// CPWAIT is implemented as a subroutine in the boot area because
// otherwise, it is implemented in RO RAM - and not useable until
// boot is complete.
void CPU_CPWAIT()
{
    //BOOT_CPWAIT();
}

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

UINT64 CPU_MicrosecondsToTicks( UINT64 uSec )
{
#if ONE_MHZ < SLOW_CLOCKS_PER_SECOND
    return uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
#else
    return uSec / (ONE_MHZ / SLOW_CLOCKS_PER_SECOND);
#endif
}

UINT32 CPU_MicrosecondsToTicks( UINT32 uSec )
{
#if ONE_MHZ < SLOW_CLOCKS_PER_SECOND
    return uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
#else
    return uSec / (ONE_MHZ / SLOW_CLOCKS_PER_SECOND);
#endif
}

//--//

#pragma arm section code = "SectionForFlashOperations"

UINT32 __section(SectionForFlashOperations) CPU_MicrosecondsToSystemClocks( UINT32 uSec )
{
    uSec *= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
    uSec /= (ONE_MHZ        /CLOCK_COMMON_FACTOR);

    return uSec;
}

int __section(SectionForFlashOperations) CPU_MicrosecondsToSystemClocks( int uSec )
{
    uSec *= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
    uSec /= (ONE_MHZ        /CLOCK_COMMON_FACTOR);

    return uSec;
}

#pragma arm section code

//--//

int CPU_SystemClocksToMicroseconds( int Ticks )
{
    Ticks *= (ONE_MHZ        /CLOCK_COMMON_FACTOR);
    Ticks /= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);

    return Ticks;
}
// -- //


BOOL HAL_Time_Initialize()
{
    return AT91_TIME_Driver::Initialize();
}

BOOL HAL_Time_Uninitialize()
{
    return AT91_TIME_Driver::Uninitialize();
}

UINT64 HAL_Time_CurrentTicks()
{
    return AT91_TIME_Driver::CounterValue();
}

INT64 HAL_Time_TicksToTime( UINT64 Ticks )
{
    return AT91_TIME_Driver::TicksToTime( Ticks );
}

INT64 HAL_Time_CurrentTime()
{
    return AT91_TIME_Driver::CurrentTime();
}

void HAL_Time_SetCompare( UINT64 CompareValue )
{
    AT91_TIME_Driver::SetCompareValue( CompareValue );
}

void HAL_Time_GetDriftParameters  ( INT32* a, INT32* b, INT64* c )
{
    *a = 1;
    *b = 1;
    *c = 0;
}

UINT64 Time_CurrentTicks()
{
    return HAL_Time_CurrentTicks();
}

#pragma arm section code = "SectionForFlashOperations"

void __section(SectionForFlashOperations) HAL_Time_Sleep_MicroSeconds( UINT32 uSec )
{
    AT91_TIME_Driver::Sleep_uSec( uSec );
}

#pragma arm section code

void HAL_Time_Sleep_MicroSeconds_InterruptEnabled( UINT32 uSec )
{
    AT91_TIME_Driver::Sleep_uSec_Loop( uSec );
}


//////////////////

