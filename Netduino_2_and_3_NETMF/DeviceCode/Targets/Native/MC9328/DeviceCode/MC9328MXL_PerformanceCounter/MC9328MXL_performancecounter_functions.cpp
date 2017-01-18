////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cores\arm\include\cpu.h>

#if !defined(BUILD_RTM)


void Time_PerformanceCounter_Initialize()
{
    MC9328MXL_Driver::PerformanceCounter_Initialize();
}

void Time_PerformanceCounter_Uninitialize()
{
    MC9328MXL_Driver::PerformanceCounter_Uninitialize();
}

UINT32 Time_PerformanceCounter()
{
    return MC9328MXL_Driver::PerformanceCounter();
}


//--//


#endif  // !defined(BUILD_RTM)

#if !defined(BUILD_RTM)

void MC9328MXL_Driver::PerformanceCounter_Initialize()
{
    if(g_MC9328MXL_Driver.m_PerformanceCounter == NULL)
    {
        MC9328MXL_TIMER_Driver::Initialize( c_PerformanceCounter_Timer, TRUE, MC9328MXL_TIMER::TCTL_CLKSOURCE__PERCLK1, 1, NULL, NULL );

        g_MC9328MXL_Driver.m_PerformanceCounter = &MC9328MXL::TIMER( c_PerformanceCounter_Timer ).TCN;
    }
}


void MC9328MXL_Driver::PerformanceCounter_Uninitialize()
{
    if(g_MC9328MXL_Driver.m_PerformanceCounter != NULL)
    {
        MC9328MXL_TIMER_Driver::Uninitialize( c_PerformanceCounter_Timer );

        g_MC9328MXL_Driver.m_PerformanceCounter = NULL;
    }
}

UINT32 MC9328MXL_Driver::PerformanceCounter()
{
    return *g_MC9328MXL_Driver.m_PerformanceCounter;
}

#endif  // !defined(BUILD_RTM)


