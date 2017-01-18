////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

//--//

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_AT91_TIMER_Driver"
#endif

AT91_TIMER_Driver g_AT91_TIMER_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

BOOL AT91_TIMER_Driver::Initialize( UINT32 timer, BOOL freeRunning, UINT32 clkSource, HAL_CALLBACK_FPN ISR, void* ISR_Param )
{
    ASSERT(timer < AT91_MAX_TIMER);

    GLOBAL_LOCK(irq);

    if(g_AT91_TIMER_Driver.m_descriptors[timer].configured == TRUE) return FALSE;

    g_AT91_TIMER_Driver.m_descriptors[timer].isr = ISR;
    g_AT91_TIMER_Driver.m_descriptors[timer].arg = ISR_Param;

    //--//

    if(ISR)
    { 
        if(!CPU_INTC_ActivateInterrupt( AT91C_ID_TC0 + timer, ISR_TIMER, (void*)timer )) return FALSE;
    }

    {
        AT91_TC &tc = AT91::TIMER(timer);


        // First, enable the clock of the TIMER
        AT91_PMC &pmc = AT91::PMC();
        pmc.PMC_PCER = (1 << (AT91C_ID_TC0+timer));

        // Disable the clock and the interrupts
        tc.TC_CCR = AT91_TC::TC_CLKDIS;
        tc.TC_IDR = 0xFFFFFFFF;

        // Clear status bit
        (void) tc.TC_SR;

        // Set the Mode of the timer Counter
        if( freeRunning == TRUE )
            tc.TC_CMR = (clkSource);
        else
            tc.TC_CMR = (AT91_TC::TC_CPCTRG | clkSource);
            
        // Enable Interrupt (Compare RC)
        tc.TC_IER = AT91_TC::TC_CPCS;

        // Enable the clock
        tc.TC_CCR = (AT91_TC::TC_CLKEN | AT91_TC::TC_SWTRG);
    }

    g_AT91_TIMER_Driver.m_descriptors[timer].configured = TRUE;

    return TRUE;
}

BOOL AT91_TIMER_Driver::Uninitialize( UINT32 timer )
{
    // Get Timer pointer
    AT91_TC &tc = AT91::TIMER(timer);

    CPU_INTC_DeactivateInterrupt( AT91C_ID_TC0 + timer );

    // Disable the clock
    tc.TC_CCR = AT91_TC::TC_CLKDIS;

    // Clear Irq Status
    (void) tc.TC_SR;

    // Disable All Irq
    tc.TC_IDR = 0xFFFFFFFF;

    // First, enable the clock of the TIMER
    AT91_PMC &pmc = AT91::PMC();
    pmc.PMC_PCDR = (1 << (AT91C_ID_TC0+timer));

    g_AT91_TIMER_Driver.m_descriptors[timer].configured = FALSE;
    g_AT91_TIMER_Driver.m_descriptors[timer].isr = NULL;
    g_AT91_TIMER_Driver.m_descriptors[timer].arg = NULL;

    return TRUE;
}

void AT91_TIMER_Driver::ISR_TIMER( void* param )
{
    UINT32 timer = (UINT32)param;
    
    ASSERT(timer < AT91_MAX_TIMER);
    
    // Execute the ISR for the Timer
    (g_AT91_TIMER_Driver.m_descriptors[timer].isr)(g_AT91_TIMER_Driver.m_descriptors[timer].arg);
}

#pragma arm section code = "SectionForFlashOperations"

AT91_TC & __section(SectionForFlashOperations) AT91::TIMER( int sel ) 
{ 
        return *(AT91_TC   *)(size_t)(AT91_TC::c_Base  + sel* 0x40); 
}

#pragma arm section code

