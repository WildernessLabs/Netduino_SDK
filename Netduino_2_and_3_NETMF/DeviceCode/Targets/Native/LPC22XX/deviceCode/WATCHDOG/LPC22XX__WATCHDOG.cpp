////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "..\LPC22XX.h"


////////////////////////////////////////////////////////////////////////////////////////

#undef  TRACE_ALWAYS

#define TRACE_ALWAYS               0x00000001

#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

////////////////////////////////////////////////////////////////////////////////////////

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_LPC22XX_WATCHDOG_Driver"
#endif

LPC22XX_WATCHDOG_Driver g_LPC22XX_WATCHDOG_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

#define WATCHDOG_USES_INTERRUPT

//--//

HRESULT Watchdog_Enable( UINT32 TimeoutMilliseconds, WATCHDOG_INTERRUPT_CALLBACK callback, void* context )
{
    return LPC22XX_WATCHDOG_Driver::Enable( TimeoutMilliseconds, callback, context );
}

void Watchdog_Disable()
{
    LPC22XX_WATCHDOG_Driver::Disable();
}

void Watchdog_ResetCounter()
{
    LPC22XX_WATCHDOG_Driver::ResetCounter();
}

void Watchdog_ResetCpu()
{
    LPC22XX_WATCHDOG_Driver::ResetCpu();
}

//--//

HRESULT LPC22XX_WATCHDOG_Driver::Enable( UINT32 TimeoutMilliseconds, WATCHDOG_INTERRUPT_CALLBACK callback, void* context )
{
    UINT32 units;

    LPC22XX_WATCHDOG& WTDG = LPC22XX::WTDG();

    if(TimeoutMilliseconds == 0) return CLR_E_FAIL;

    GLOBAL_LOCK(irq);

#if defined(WATCHDOG_USES_INTERRUPT)
    CPU_INTC_ActivateInterrupt( LPC22XX_VIC::c_IRQ_INDEX_WDT, ISR, NULL );

    g_LPC22XX_WATCHDOG_Driver.m_callback = callback;
    g_LPC22XX_WATCHDOG_Driver.m_context  = context;
#endif

    units = TimeoutMilliseconds * 10000 / c_Granularity;

    if (units < c_MinCounter) units =  c_MinCounter;

    // set counter first
    WTDG.WDTC = units; 

    // enable watchdog after
    WTDG.WDMOD |= LPC22XX_WATCHDOG::WDMOD__WDEN|
#if defined(WATCHDOG_USES_INTERRUPT)
                LPC22XX_WATCHDOG::WDMOD__WDINT|
#endif
                LPC22XX_WATCHDOG::WDMOD__WDRESET;

    // load the counter
    WTDG.WDFEED= LPC22XX_WATCHDOG::WDFEED_reload_1;
    WTDG.WDFEED= LPC22XX_WATCHDOG::WDFEED_reload_2;

    return S_OK;
}

void LPC22XX_WATCHDOG_Driver::Disable()
{
    // reading clears the interrupt
//    UINT32 intrStatus = LPC22XX::WTDG().WSTR & LPC22XX_WATCHDOG::WDMOD;
    
    LPC22XX::WTDG().WDMOD &= ~LPC22XX_WATCHDOG::WDMOD__WDEN;
}

void LPC22XX_WATCHDOG_Driver::ResetCounter()
{
    LPC22XX_WATCHDOG& WTDG = LPC22XX::WTDG();

    GLOBAL_LOCK(irq);

    // reload counter
    WTDG.WDFEED = LPC22XX_WATCHDOG::WDFEED_reload_1;
    WTDG.WDFEED = LPC22XX_WATCHDOG::WDFEED_reload_2;
}

void LPC22XX_WATCHDOG_Driver::ResetCpu()
{
    LPC22XX_WATCHDOG& WTDG = LPC22XX::WTDG();

    // disable interrupts
    GLOBAL_LOCK(irq);
    // set the smallest value
    WTDG.WDTC = 0xFF; 

    // assure its enabled (and counter is zero)
    WTDG.WDMOD = LPC22XX_WATCHDOG::WDMOD__WDEN | LPC22XX_WATCHDOG::WDMOD__WDRESET;

    WTDG.WDFEED = LPC22XX_WATCHDOG::WDFEED_reload_1;
    WTDG.WDFEED = LPC22XX_WATCHDOG::WDFEED_reload_2;

    // sit here forever and wait for reset to happen
    while(true);

}

//--//

void LPC22XX_WATCHDOG_Driver::ISR( void* Param )
{
    LPC22XX_WATCHDOG& WTDG = LPC22XX::WTDG();

#if !defined(BUILD_RTM)
        debug_printf( "WATCHDOG ISR  \r\n" );
        lcd_printf( "\fWATCHDOG ISR  \r\n" );
        HARD_BREAKPOINT();
#endif

    g_LPC22XX_WATCHDOG_Driver.m_callback( g_LPC22XX_WATCHDOG_Driver.m_context );
}

//--//

#undef WATCHDOG_USES_INTERRUPT

//--//
