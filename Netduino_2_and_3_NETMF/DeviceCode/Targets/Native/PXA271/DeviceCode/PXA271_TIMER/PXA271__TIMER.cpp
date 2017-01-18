////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

//--//

#undef  TRACE_ALWAYS

#define TRACE_ALWAYS               0x00000001

#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_PXA271_TIMER_Driver"
#endif

PXA271_TIMER_Driver g_PXA271_TIMER_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

#pragma arm section code = "SectionForFlashOperations"

PXA271_TIMER & __section(SectionForFlashOperations) PXA271::TIMER() 
{ 
    return *(PXA271_TIMER *)(size_t)(PXA271_TIMER   ::c_Base);
}

#pragma arm section code


BOOL PXA271_TIMER_Driver::Initialize  ( UINT32 Timer, BOOL FreeRunning, UINT32 ClkSource, UINT32 externalSync, HAL_CALLBACK_FPN ISR, void* ISR_Param )
{
    ASSERT(Timer < c_MaxTimers);

    GLOBAL_LOCK(irq);

    if(g_PXA271_TIMER_Driver.m_descriptors[Timer].configured == TRUE) return FALSE;

    //--//

    PXA271_CLOCK_MANAGER &CLOCK = PXA271::CLKMNGR();

    CLOCK.CKEN |= PXA271_CLOCK_MANAGER::CKEN__OS_TIMER;     // Make sure the clock to the OS timer is enabled

    DisableCompareInterrupt( Timer );

    g_PXA271_TIMER_Driver.m_descriptors[Timer].isr = ISR;
    g_PXA271_TIMER_Driver.m_descriptors[Timer].arg = ISR_Param;
    
    if(ISR)
    {
        if( Timer < c_FirstPXA271_Timer )     // If one of the PXA250 compatible timers
        {
            // There are separate interrupts for each of the PXA250 compatible timers
            UINT32 interrupt = 0;

            switch(Timer)
            {
            case 0:
                interrupt = PXA271_AITC::c_IRQ_INDEX_OS_TIMER0;
                break;
            case 1:
                interrupt = PXA271_AITC::c_IRQ_INDEX_OS_TIMER1;
                break;
            case 2:
                interrupt = PXA271_AITC::c_IRQ_INDEX_OS_TIMER2;
                break;
            case 3:
                interrupt = PXA271_AITC::c_IRQ_INDEX_OS_TIMER3;
                break;
            }
 
            if( !CPU_INTC_ActivateInterrupt(interrupt, ISR, ISR_Param) ) return FALSE;
        }
        else
        {
            int i;

            // All PXA271 timers share a single interrupt
            for( i = c_FirstPXA271_Timer; (i < c_MaxTimers) && !g_PXA271_TIMER_Driver.m_descriptors[i].configured; i++ );     // Search for configured timers
            if( i == c_MaxTimers )       // If the single interrupt for all PXA271 timers has not been activated yet
            {
                if( !CPU_INTC_ActivateInterrupt(PXA271_AITC::c_IRQ_INDEX_OS_TIMER, &ISR4_11, NULL) ) return FALSE;
            }
        }
    }

    if( Timer >= c_FirstPXA271_Timer )     // If the timer is a PXA271 timer and configurable
    {
        PXA271_TIMER& TIMER = PXA271::TIMER();

        if( FreeRunning )
        {
            TIMER.OMCR[Timer-c_FirstPXA271_Timer] = PXA271_TIMER::OMCR__MATCHx | PXA271_TIMER::OMCR__PER | ClkSource | externalSync;
            TIMER.OSCR[Timer-c_FirstPXA271_Timer] = 0;        // Start counter running
        }
        else
        {
            TIMER.OMCR[Timer-c_FirstPXA271_Timer] = PXA271_TIMER::OMCR__MATCHx | PXA271_TIMER::OMCR__ROM | ClkSource | externalSync;
        }
    }

    g_PXA271_TIMER_Driver.m_descriptors[Timer].configured = TRUE;

    return TRUE;
}

BOOL PXA271_TIMER_Driver::Uninitialize( UINT32 Timer )
{
    ASSERT(Timer < c_MaxTimers);

    GLOBAL_LOCK(irq);

    DisableCompareInterrupt( Timer );

    if(g_PXA271_TIMER_Driver.m_descriptors[Timer].configured == FALSE) return FALSE;

    g_PXA271_TIMER_Driver.m_descriptors[Timer].configured = FALSE;

    //--//

    if( Timer < c_FirstPXA271_Timer )     // If one of the PXA250 compatible timers
    {
        // Interrupts for each timer are shut down individually
        UINT32 interrupt = 0;
        
        switch(Timer)
        {
        case 0:
            interrupt = PXA271_AITC::c_IRQ_INDEX_OS_TIMER0;
            break;
        case 1:
            interrupt = PXA271_AITC::c_IRQ_INDEX_OS_TIMER1;
            break;
        case 2:
            interrupt = PXA271_AITC::c_IRQ_INDEX_OS_TIMER2;
            break;
        case 3:
            interrupt = PXA271_AITC::c_IRQ_INDEX_OS_TIMER3;
            break;
        }

        if(!CPU_INTC_DeactivateInterrupt( interrupt )) return FALSE;
    }
    else
    {
        int i;

        // All PXA271 timers share a single interrupt
        for( i = c_FirstPXA271_Timer; i < c_MaxTimers && !g_PXA271_TIMER_Driver.m_descriptors[i].configured; i++ );     // Search for configured timers
        if( i == c_MaxTimers )       // If the last timer has been deactivated
        {
            if( !CPU_INTC_DeactivateInterrupt(PXA271_AITC::c_IRQ_INDEX_OS_TIMER) ) return FALSE;
        }
    }

    //--//

    // If the last timer is disabled, make sure the clock is shut off also to conserve power
    {
        int i;

        for( i = 0; i < c_MaxTimers && !g_PXA271_TIMER_Driver.m_descriptors[i].configured; i++);
        if( i == c_MaxTimers )      // If the last timer was uninitialized
        {
            PXA271_CLOCK_MANAGER &CLOCK = PXA271::CLKMNGR();

            CLOCK.CKEN &= ~PXA271_CLOCK_MANAGER::CKEN__OS_TIMER;     // Shut down the clock to the OS Timer
        }
    }

    return TRUE;
}

// This interrupt service routine became necessary for Timer interrupts 4-11 (PXA271 timers) because
// unlike the interrupts for Timers 0-3 (PXA250 compatible timers) 4-11 share the same interrupt line.
void PXA271_TIMER_Driver::ISR4_11( void* param )
{
    UINT32 Timer;
    PXA271_TIMER& TIMER = PXA271::TIMER();

    for( Timer = c_FirstPXA271_Timer; Timer < c_MaxTimers; Timer++ )
    {
        UINT32 Mask = (1 << Timer);
        
        if( (TIMER.OSSR & Mask) && (TIMER.OIER & Mask) )        // If this timer caused the interrupt
        {
            TIMER.OSSR = Mask;      // Clear the interrupt
            (g_PXA271_TIMER_Driver.m_descriptors[Timer].isr)(g_PXA271_TIMER_Driver.m_descriptors[Timer].arg);       // Execute the ISR for the Timer
        }
    }
}

#pragma arm section code = "SectionForFlashOperations"

// this function is used in the sleep_usec=> flash read/write operation, therefore need to be in RAM
UINT32 __section(SectionForFlashOperations) PXA271_TIMER_Driver::ReadCounter( UINT32 Timer )
{
    ASSERT(Timer < c_MaxTimers);

    PXA271_TIMER& TIMER = PXA271::TIMER();

    if( Timer < c_FirstPXA271_Timer )     // If one of the PXA250 compatible timers
        return TIMER.OSCR0;
    else
        return TIMER.OSCR[Timer-c_FirstPXA271_Timer];
}

#pragma arm section code 
