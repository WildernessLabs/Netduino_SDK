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
#pragma arm section zidata = "g_MC9328MXL_WATCHDOG_Driver"
#endif

MC9328MXL_WATCHDOG_Driver g_MC9328MXL_WATCHDOG_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

#define WATCHDOG_USES_INTERRUPT

//--//

HRESULT MC9328MXL_WATCHDOG_Driver::Enable( UINT32 TimeoutMilliseconds, WATCHDOG_INTERRUPT_CALLBACK callback, void* context )
{
    NATIVE_PROFILE_HAL_PROCESSOR_WATCHDOG();
    UINT32 units;

    
#if !defined(JTAG_DEBUGGING)

    MC9328MXL_WATCHDOG& WTDG = MC9328MXL::WTDG();

    if(TimeoutMilliseconds == 0) return CLR_E_FAIL;

    g_MC9328MXL_WATCHDOG_Driver.m_callback = callback;
    g_MC9328MXL_WATCHDOG_Driver.m_context  = context;
    
    GLOBAL_LOCK(irq);

#if defined(WATCHDOG_USES_INTERRUPT)
    CPU_INTC_ActivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_WDT_INT, ISR, NULL );
#endif

    units = TimeoutMilliseconds / c_Granularity;
    units = (units > c_MaxCounter ? c_MaxCounter : units);

    if(units == 0) units = 1;

    // set counter first
    WTDG.WCR = units << MC9328MXL_WATCHDOG::WCR__WT_shift;

    // enable watchdog after
    WTDG.WCR |= MC9328MXL_WATCHDOG::WCR__WDE |
#if defined(WATCHDOG_USES_INTERRUPT)
                MC9328MXL_WATCHDOG::WCR__WIE |
#endif
                MC9328MXL_WATCHDOG::WCR__WDEC;

    // load the counter
    WTDG.WSR = MC9328MXL_WATCHDOG::WSR_reload_1;
    WTDG.WSR = MC9328MXL_WATCHDOG::WSR_reload_2;
#endif

    return S_OK;
}

void MC9328MXL_WATCHDOG_Driver::Disable()
{
    NATIVE_PROFILE_HAL_PROCESSOR_WATCHDOG();
    // reading clears the interrupt
    UINT32 intrStatus = MC9328MXL::WTDG().WSTR & MC9328MXL_WATCHDOG::WSTR__TINT;
    
    MC9328MXL::WTDG().WCR &= ~MC9328MXL_WATCHDOG::WCR__WDE;
}

void MC9328MXL_WATCHDOG_Driver::ResetCounter()
{
    NATIVE_PROFILE_HAL_PROCESSOR_WATCHDOG();
    MC9328MXL_WATCHDOG& WTDG = MC9328MXL::WTDG();

    GLOBAL_LOCK(irq);

    // reload counter
    WTDG.WSR = MC9328MXL_WATCHDOG::WSR_reload_1;
    WTDG.WSR = MC9328MXL_WATCHDOG::WSR_reload_2;
}

void MC9328MXL_WATCHDOG_Driver::ResetCpu()
{
    NATIVE_PROFILE_HAL_PROCESSOR_WATCHDOG();
    MC9328MXL_WATCHDOG& WTDG = MC9328MXL::WTDG();

    // disable interrupts
    GLOBAL_LOCK(irq);

    // assure its enabled (and counter is zero)
    WTDG.WCR = MC9328MXL_WATCHDOG::WCR__WDE;

    WTDG.WSR = MC9328MXL_WATCHDOG::WSR_reload_1;
    WTDG.WSR = MC9328MXL_WATCHDOG::WSR_reload_2;

    // sit here forever and wait for reset to happen
    while(true);
}

//--//

void MC9328MXL_WATCHDOG_Driver::ISR( void* Param )
{
    NATIVE_PROFILE_HAL_PROCESSOR_WATCHDOG();
    MC9328MXL_WATCHDOG& WTDG = MC9328MXL::WTDG();

    // reading clears the interrupt
    UINT32 intrStatus = MC9328MXL::WTDG().WSTR & MC9328MXL_WATCHDOG::WSTR__TINT;

#if !defined(BUILD_RTM)
        debug_printf( "WATCHDOG ISR  \r\n" );
        lcd_printf( "WATCHDOG ISR  \r\n" );
#endif

    g_MC9328MXL_WATCHDOG_Driver.m_callback( g_MC9328MXL_WATCHDOG_Driver.m_context );    
}

//--//

#undef WATCHDOG_USES_INTERRUPT

//--//

