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

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_LPC22XX_TIMER_Driver"
#endif

LPC22XX_TIMER_Driver g_LPC22XX_TIMER_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

BOOL LPC22XX_TIMER_Driver::Initialize( UINT32 Timer, HAL_CALLBACK_FPN ISR, void* ISR_Param )
{
    ASSERT(Timer < c_MaxTimers);

    GLOBAL_LOCK(irq);

    if(g_LPC22XX_TIMER_Driver.m_configured[Timer] == TRUE) return FALSE;

    //--//

    if(ISR)
    {
		UINT32 irq;

		if(Timer)
		{
			irq = LPC22XX_VIC::c_IRQ_INDEX_TIMER1;
		}
		else
		{
			irq = LPC22XX_VIC::c_IRQ_INDEX_TIMER0;
		}

        if(!CPU_INTC_ActivateInterrupt( irq, ISR, ISR_Param )) return FALSE;
    }

    LPC22XX_TIMER& TIMER = LPC22XX::TIMER( Timer );

    TIMER.TCR = LPC22XX_TIMER::TCR_TEN;

    //--//

    g_LPC22XX_TIMER_Driver.m_configured[Timer] = TRUE;

    return TRUE;
}

BOOL LPC22XX_TIMER_Driver::Uninitialize( UINT32 Timer )
{
    ASSERT(Timer < c_MaxTimers);

    GLOBAL_LOCK(irq);

    if(g_LPC22XX_TIMER_Driver.m_configured[Timer] == FALSE) return FALSE;

    //--//

    if(!CPU_INTC_DeactivateInterrupt( Timer ? LPC22XX_VIC::c_IRQ_INDEX_TIMER1 : LPC22XX_VIC::c_IRQ_INDEX_TIMER0 )) return FALSE;

    LPC22XX_TIMER& TIMER = LPC22XX::TIMER( Timer );

    //Reset timer
    TIMER.TCR = 0x2;
    // disable Timer
    TIMER.TCR = 0;

    //--//

    g_LPC22XX_TIMER_Driver.m_configured[Timer] = FALSE;

    return TRUE;
}


#pragma arm section code = "SectionForFlashOperations"

UINT32 __section(SectionForFlashOperations) LPC22XX_TIMER_Driver::ReadCounter( UINT32 Timer )
{
    ASSERT(Timer < c_MaxTimers);

    LPC22XX_TIMER& TIMER = LPC22XX::TIMER( Timer );

    return TIMER.TC;
}

#pragma arm section code


