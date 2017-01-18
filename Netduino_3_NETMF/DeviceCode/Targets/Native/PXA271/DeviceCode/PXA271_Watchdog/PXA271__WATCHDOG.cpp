////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

////////////////////////////////////////////////////////////////////////////////////////

#undef  TRACE_ALWAYS

#define TRACE_ALWAYS               0x00000001

#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

////////////////////////////////////////////////////////////////////////////////////////

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_PXA271_WATCHDOG_Driver"
#endif

PXA271_WATCHDOG_Driver g_PXA271_WATCHDOG_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

HRESULT PXA271_WATCHDOG_Driver::Enable( UINT32 TimeoutMilliseconds, WATCHDOG_INTERRUPT_CALLBACK callback, void* context )
{
    // The maximum amount of this timer is around 20 minutes
    if( TimeoutMilliseconds == 0 || TimeoutMilliseconds > c_MaxMilliseconds ) return CLR_E_FAIL;

    GLOBAL_LOCK(irq);

    if( c_WatchdogUsesInterrupt )
    {
        PXA271_TIMER_Driver::Initialize( PXA271_Driver::c_Watchdog_Timer, TRUE, 0, 0, ISR, NULL );
        g_PXA271_WATCHDOG_Driver.m_callback = callback;
        g_PXA271_WATCHDOG_Driver.m_context  = context;
    }
    else
    {
        PXA271_TIMER_Driver::Initialize( PXA271_Driver::c_Watchdog_Timer, TRUE, 0, 0, NULL, NULL );
    }

    g_PXA271_WATCHDOG_Driver.m_ResetCount = TimeoutMilliseconds * c_CountsPerMillisecond;

    // set timeout first
    PXA271_TIMER_Driver::SetCompare( PXA271_Driver::c_Watchdog_Timer, 
        PXA271_TIMER_Driver::ReadCounter(PXA271_Driver::c_SystemTime_Timer) + g_PXA271_WATCHDOG_Driver.m_ResetCount);

    // enable watchdog
    if( c_WatchdogUsesInterrupt )
    {
        // WARNING!!
        // This is just a normal interrupt - if IRQ is disabled, the watchdog will be too.
        PXA271_TIMER_Driver::EnableCompareInterrupt( PXA271_Driver::c_Watchdog_Timer );
    }
    else
    {
        // WARNING!!
        // Once the Watchdog reset is enabled, it cannot be disabled without shutting down
        // ALL timers or resetting the CPU.
        PXA271::TIMER().OWER = PXA271_TIMER::OWER__WME;
    }

    return S_OK;
}

void PXA271_WATCHDOG_Driver::Disable()
{
    GLOBAL_LOCK(irq);

    PXA271_TIMER_Driver::Uninitialize( PXA271_Driver::c_Watchdog_Timer );
    if( !c_WatchdogUsesInterrupt )
    {
        // WARNING!!
        // This disables ALL timers.
        PXA271::CLKMNGR().CKEN &= ~ PXA271_CLOCK_MANAGER::CKEN__OS_TIMER;
    }
}

void PXA271_WATCHDOG_Driver::ResetCounter()
{
    GLOBAL_LOCK(irq);

    // Reload match register
    PXA271_TIMER_Driver::SetCompare( PXA271_Driver::c_Watchdog_Timer, 
        PXA271_TIMER_Driver::ReadCounter(PXA271_Driver::c_SystemTime_Timer) + g_PXA271_WATCHDOG_Driver.m_ResetCount);
}

void PXA271_WATCHDOG_Driver::ResetCpu()
{
    // disable interrupts
    GLOBAL_LOCK(irq);

    // assure it's enabled and that timeout is set
    PXA271_TIMER_Driver::SetCompare( PXA271_Driver::c_Watchdog_Timer,
        PXA271_TIMER_Driver::ReadCounter(PXA271_Driver::c_SystemTime_Timer) + c_CountsPerMillisecond );
    PXA271::CLKMNGR().CKEN |= PXA271_CLOCK_MANAGER::CKEN__OS_TIMER;     // Make sure timer clocks are enabled

    PXA271::TIMER().OWER = PXA271_TIMER::OWER__WME;                     // Make sure Watchdog is enabled

    // sit here forever and wait for reset to happen
    while(true);
}

//--//

void PXA271_WATCHDOG_Driver::ISR( void* Param )
{        
#if !defined(BUILD_RTM)
        debug_printf( "WATCHDOG ISR  \r\n" );

        debug_printf( "TimeoutTicks : 0x%08x\r\n", g_PXA271_WATCHDOG_Driver.m_ResetCount);
        debug_printf( "WatchdogCnt  : 0x%08x\r\n", PXA271_TIMER_Driver::GetCompare(PXA271_Driver::c_Watchdog_Timer));
        debug_printf( "SystemTimeCnt: 0x%08x\r\n", PXA271_TIMER_Driver::ReadCounter(PXA271_Driver::c_SystemTime_Timer));    
#endif

    // clear the interrrupt
    PXA271_TIMER_Driver::ResetCompareHit( PXA271_Driver::c_Watchdog_Timer );


    g_PXA271_WATCHDOG_Driver.m_callback( g_PXA271_WATCHDOG_Driver.m_context );
}
