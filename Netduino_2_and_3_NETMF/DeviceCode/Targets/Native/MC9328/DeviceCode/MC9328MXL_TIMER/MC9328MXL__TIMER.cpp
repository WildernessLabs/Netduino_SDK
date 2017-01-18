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
#pragma arm section zidata = "g_MC9328MXL_TIMER_Driver"
#endif

MC9328MXL_TIMER_Driver g_MC9328MXL_TIMER_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

BOOL MC9328MXL_TIMER_Driver::Initialize( UINT32 Timer, BOOL FreeRunning, UINT32 ClkSource, UINT32 captureEdge, HAL_CALLBACK_FPN ISR, void* ISR_Param )
{
    ASSERT(Timer < c_MaxTimers);

    GLOBAL_LOCK(irq);

    if(g_MC9328MXL_TIMER_Driver.m_configured[Timer] == TRUE) return FALSE;

    //--//

    if(ISR)
    {
		UINT32 index;

		if(Timer)
		{
			index = MC9328MXL_AITC::c_IRQ_INDEX_TIMER2_INT;
		}
		else
		{
			index = MC9328MXL_AITC::c_IRQ_INDEX_TIMER1_INT;
		}

        if(!CPU_INTC_ActivateInterrupt( index, ISR, ISR_Param )) return FALSE;
    }

    UINT32 mode = MC9328MXL_TIMER::TCTL_TEN;

    if(FreeRunning) mode |=                       MC9328MXL_TIMER::TCTL_FRR;
    /*************/ mode |= ( ClkSource        << MC9328MXL_TIMER::TCTL_CLKSOURCE_shift) & MC9328MXL_TIMER::TCTL_CLKSOURCE_mask;
    /*************/ mode |= ((captureEdge - 1) << MC9328MXL_TIMER::TCTL_CAP_shift      ) & MC9328MXL_TIMER::TCTL_CAP_mask      ;

    MC9328MXL_TIMER& TIMER = MC9328MXL::TIMER( Timer );

    TIMER.TCTL = mode;

    //--//

    g_MC9328MXL_TIMER_Driver.m_configured[Timer] = TRUE;

    return TRUE;
}

BOOL MC9328MXL_TIMER_Driver::Uninitialize( UINT32 Timer )
{
    ASSERT(Timer < c_MaxTimers);

    GLOBAL_LOCK(irq);

    if(g_MC9328MXL_TIMER_Driver.m_configured[Timer] == FALSE) return FALSE;

    //--//

	UINT32 index;

	if(Timer)
	{
		index = MC9328MXL_AITC::c_IRQ_INDEX_TIMER2_INT;
	}
	else
	{
		index = MC9328MXL_AITC::c_IRQ_INDEX_TIMER1_INT;
	}

    if(!CPU_INTC_DeactivateInterrupt( index )) return FALSE;

    MC9328MXL_TIMER& TIMER = MC9328MXL::TIMER( Timer );

    TIMER.TCTL = 0;

    //--//

    g_MC9328MXL_TIMER_Driver.m_configured[Timer] = FALSE;

    return TRUE;
}


#pragma arm section code = "SectionForFlashOperations"

UINT32 __section(SectionForFlashOperations) MC9328MXL_TIMER_Driver::ReadCounter( UINT32 Timer )
{
    ASSERT(Timer < c_MaxTimers);

    MC9328MXL_TIMER& TIMER = MC9328MXL::TIMER( Timer );

    return TIMER.TCN;
}

#pragma arm section code

#pragma arm section code = "SectionForFlashOperations"
MC9328MXL_TIMER& __section(SectionForFlashOperations) MC9328MXL::TIMER( int sel ) 
{ 
	if(sel == 0)
		return *(MC9328MXL_TIMER   *)(size_t)MC9328MXL_TIMER::c_Base_1; 
	else
		return *(MC9328MXL_TIMER   *)(size_t)MC9328MXL_TIMER::c_Base_2; 
}

#pragma arm section code 


