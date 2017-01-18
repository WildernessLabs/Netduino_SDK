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
#include "..\LPC24XX.h"


////////////////////////////////////////////////////////////////////////////////////////

#undef  TRACE_ALWAYS

#define TRACE_ALWAYS               0x00000001

#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

////////////////////////////////////////////////////////////////////////////////////////

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_LPC24XX_WATCHDOG_Driver"
#endif

LPC24XX_WATCHDOG_Driver g_LPC24XX_WATCHDOG_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//


HRESULT Watchdog_Enable( UINT32 TimeoutMilliseconds, WATCHDOG_INTERRUPT_CALLBACK callback, void* context )
{
    return LPC24XX_WATCHDOG_Driver::Enable( TimeoutMilliseconds, callback, context );
}

void Watchdog_Disable()
{
    LPC24XX_WATCHDOG_Driver::Disable();
}

void Watchdog_ResetCounter()
{
    LPC24XX_WATCHDOG_Driver::ResetCounter();
}

void Watchdog_ResetCpu()
{
    LPC24XX_WATCHDOG_Driver::ResetCpu();
}

//--//
HRESULT LPC24XX_WATCHDOG_Driver::Enable( UINT32 TimeoutMilliseconds, WATCHDOG_INTERRUPT_CALLBACK callback, void* context )
{
    UINT32 units;

    LPC24XX_WATCHDOG& WTDG = LPC24XX::WTDG();

    if(TimeoutMilliseconds == 0) return CLR_E_FAIL;
	
    GLOBAL_LOCK(irq);

#if defined(WATCHDOG_USES_INTERRUPT)
    CPU_INTC_ActivateInterrupt( LPC24XX_VIC::c_IRQ_INDEX_WDT, ISR, NULL );

    g_LPC24XX_WATCHDOG_Driver.m_callback = callback;
    g_LPC24XX_WATCHDOG_Driver.m_context  = context;

#endif

    units = TimeoutMilliseconds * 10000 / c_Granularity;

    if (units < c_MinCounter) units =  c_MinCounter;

    // set counter first
    WTDG.WDTC = units; 

    // enable watchdog after
    WTDG.WDMOD |= LPC24XX_WATCHDOG::WDMOD__WDEN|
#if defined(WATCHDOG_USES_INTERRUPT)
                LPC24XX_WATCHDOG::WDMOD__WDINT|
#endif
                LPC24XX_WATCHDOG::WDMOD__WDRESET;

    // load the counter
    WTDG.WDFEED= LPC24XX_WATCHDOG::WDFEED_reload_1;
    WTDG.WDFEED= LPC24XX_WATCHDOG::WDFEED_reload_2;

    return S_OK;
}

void LPC24XX_WATCHDOG_Driver::Disable()
{
    // reading clears the interrupt
//    UINT32 intrStatus = LPC24XX::WTDG().WSTR & LPC24XX_WATCHDOG::WDMOD;
    
    LPC24XX::WTDG().WDMOD &= ~LPC24XX_WATCHDOG::WDMOD__WDEN;
}

void LPC24XX_WATCHDOG_Driver::ResetCounter()
{
    LPC24XX_WATCHDOG& WTDG = LPC24XX::WTDG();

    GLOBAL_LOCK(irq);

    // reload counter
    WTDG.WDFEED = LPC24XX_WATCHDOG::WDFEED_reload_1;
    WTDG.WDFEED = LPC24XX_WATCHDOG::WDFEED_reload_2;
}

void LPC24XX_WATCHDOG_Driver::ResetCpu()
{
    LPC24XX_WATCHDOG& WTDG = LPC24XX::WTDG();

    // disable interrupts
    GLOBAL_LOCK(irq);
    // set the smallest value
    WTDG.WDTC = 0xFF; 

    // assure its enabled (and counter is zero)
    WTDG.WDMOD = LPC24XX_WATCHDOG::WDMOD__WDEN | LPC24XX_WATCHDOG::WDMOD__WDRESET;

    WTDG.WDFEED = LPC24XX_WATCHDOG::WDFEED_reload_1;
    WTDG.WDFEED = LPC24XX_WATCHDOG::WDFEED_reload_2;

    // sit here forever and wait for reset to happen
    while(true);

}

//--//

void LPC24XX_WATCHDOG_Driver::ISR( void* Param )
{
    LPC24XX_WATCHDOG& WTDG = LPC24XX::WTDG();

#if !defined(BUILD_RTM)
        debug_printf( "WATCHDOG ISR  \r\n" );
        lcd_printf( "\fWATCHDOG ISR  \r\n" );
        HARD_BREAKPOINT();
#else
        ResetCpu();  return;
#endif
}
