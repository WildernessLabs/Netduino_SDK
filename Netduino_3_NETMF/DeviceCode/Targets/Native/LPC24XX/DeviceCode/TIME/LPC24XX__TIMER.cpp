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

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_LPC24XX_TIMER_Driver"
#endif

LPC24XX_TIMER_Driver g_LPC24XX_TIMER_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

BOOL LPC24XX_TIMER_Driver::Initialize( UINT32 Timer, HAL_CALLBACK_FPN ISR, void* ISR_Param )
{
    ASSERT(Timer < c_MaxTimers);

    GLOBAL_LOCK(irq);

    if(g_LPC24XX_TIMER_Driver.m_configured[Timer] == TRUE) return FALSE;

    //--//

    if(ISR)
    {
        if(!CPU_INTC_ActivateInterrupt( LPC24XX_TIMER::getIntNo(Timer) , ISR, ISR_Param )) return FALSE;
    }

    LPC24XX_TIMER& TIMER = LPC24XX::TIMER( Timer );

    TIMER.TCR = LPC24XX_TIMER::TCR_TEN;

    //--//

    g_LPC24XX_TIMER_Driver.m_configured[Timer] = TRUE;

    return TRUE;
}

BOOL LPC24XX_TIMER_Driver::Uninitialize( UINT32 Timer )
{
    ASSERT(Timer < c_MaxTimers);

    GLOBAL_LOCK(irq);

    if(g_LPC24XX_TIMER_Driver.m_configured[Timer] == FALSE) return FALSE;

    //--//

    if(!CPU_INTC_DeactivateInterrupt( LPC24XX_TIMER::getIntNo(Timer) )) return FALSE;

    LPC24XX_TIMER& TIMER = LPC24XX::TIMER( Timer );

    //Reset timer
    TIMER.TCR = 0x2;
    // disable Timer
    TIMER.TCR = 0;

    //--//

    g_LPC24XX_TIMER_Driver.m_configured[Timer] = FALSE;

    return TRUE;
}


#pragma arm section code = "SectionForFlashOperations"

UINT32 __section(SectionForFlashOperations) LPC24XX_TIMER_Driver::ReadCounter( UINT32 Timer )
{
    ASSERT(Timer < c_MaxTimers);

    LPC24XX_TIMER& TIMER = LPC24XX::TIMER( Timer );

    return TIMER.TC;
}

#pragma arm section code
