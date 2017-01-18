////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SH7216_timer.h"

//--//

//--//


SH7216_TIMER_Driver g_SH7216_TIMER_Driver;

//--//

/* INTC Registers */
#define VS(x) (*(volatile unsigned short *)(x))
#define IPR08                VS(0xFFFE0C04)

BOOL SH7216_TIMER_Driver::Initialize( UINT32 timer, UINT32 clkSource, HAL_CALLBACK_FPN ISR, void* ISR_Param )
{
    GLOBAL_LOCK(irq);

    if(timer > 1) return FALSE;

    TimerISR& tmr = g_SH7216_TIMER_Driver.m_timers[timer];

    if(tmr.Configured == TRUE) return FALSE;

    if(clkSource > CLOCK_SOURCE_DIV512) return FALSE;

    tmr.ISR  = ISR;
    tmr.Arg  = ISR_Param;
    tmr.pCMT = (timer == 0 ? &CMT0 : &CMT1);

    //--//

    // Disable the clock and the interrupts
    CMT.CMSTR.WORD &= (timer == 0 ? 2 : 1);

    // Clear status bit
    volatile UINT32 tmp = tmr.pCMT->CMCSR.WORD;
    tmr.pCMT->CMCSR.WORD = 0;

    // set this timers to have the highest priority
    IPR08 |= (timer == 0 ? 0xF000 : 0x0F00);

    // Set the Mode of the timer Counter
    tmr.pCMT->CMCSR.WORD = clkSource | (ISR == NULL ? 0 : 0x40);

    tmr.pCMT->CMCOR = 0xFFFF;
        
    // Enable the clock
    CMT.CMSTR.WORD |= (timer == 0 ? 0x0001 : 0x0002);

    tmr.Configured = TRUE;

    return TRUE;
}

BOOL SH7216_TIMER_Driver::Uninitialize(UINT32 timer)
{
    if(timer > 1) return FALSE;
    
    TimerISR& tmr = g_SH7216_TIMER_Driver.m_timers[timer];
    
    if(tmr.Configured) return FALSE;
    
    CMT.CMSTR.WORD &= (timer == 0 ? 2 : 1);

    // Clear status bit
    volatile UINT32 tmp = tmr.pCMT->CMCSR.WORD;
    tmr.pCMT->CMCSR.WORD = 0;

    tmr.Configured = FALSE;

    return TRUE;
}

UINT16 SH7216_TIMER_Driver::ReadCompare( UINT32 timer ) 
{ 
    TimerISR& tmr = g_SH7216_TIMER_Driver.m_timers[timer];

    return tmr.pCMT->CMCOR;
}
UINT16 SH7216_TIMER_Driver::ReadCounter( UINT32 timer ) 
{ 
    TimerISR& tmr = g_SH7216_TIMER_Driver.m_timers[timer];

    return tmr.pCMT->CMCNT;
}
void SH7216_TIMER_Driver::ForceInterrupt( UINT32 timer ) 
{ 
    TimerISR& tmr = g_SH7216_TIMER_Driver.m_timers[timer];

    tmr.pCMT->CMCOR = (UINT16)(tmr.pCMT->CMCNT + 2);  
}
void SH7216_TIMER_Driver::SetCompare( UINT32 timer, UINT16 val ) 
{ 
    TimerISR& tmr = g_SH7216_TIMER_Driver.m_timers[timer];

    tmr.pCMT->CMCNT = 0;
    tmr.pCMT->CMCOR = val;  
}

void SH7216_TIMER_ISR(UINT32 timer)
{
    volatile UINT32 tmp;

    if(timer > 1) return;

    // clear the match 
    if(timer == 0)
    {
        tmp = CMT0.CMCSR.WORD;
        CMT0.CMCSR.WORD &= ~((UINT16)0x80);
    }
    else
    {
        tmp = CMT1.CMCSR.WORD;
        CMT1.CMCSR.WORD &= ~((UINT16)0x80);        
    }

    g_SH7216_TIMER_Driver.m_timers[timer].ISR(g_SH7216_TIMER_Driver.m_timers[timer].Arg);
}

