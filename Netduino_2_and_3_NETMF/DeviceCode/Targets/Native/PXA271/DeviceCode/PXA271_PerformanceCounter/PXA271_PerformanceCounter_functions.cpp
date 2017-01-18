////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cores\arm\include\cpu.h>

#if !defined(BUILD_RTM)


void Time_PerformanceCounter_Initialize()
{
    PXA271_Driver::PerformanceCounter_Initialize();
}

void Time_PerformanceCounter_Uninitialize()
{
    PXA271_Driver::PerformanceCounter_Uninitialize();
}

UINT32 Time_PerformanceCounter()
{
    return PXA271_Driver::PerformanceCounter();
}

//--//

void PXA271_Driver::PerformanceCounter_Initialize()
{
    if(g_PXA271_Driver.m_PerformanceCounter == NULL)
    {
        PXA271_TIMER_Driver::Initialize( c_PerformanceCounter_Timer, TRUE, PXA271_TIMER::OMCR__CRES_1uS, PXA271_TIMER::OMCR__XSYNC_NONE, NULL, NULL );

        g_PXA271_Driver.m_PerformanceCounter = &PXA271::TIMER().OSCR[c_PerformanceCounter_Timer-PXA271_TIMER_Driver::c_FirstPXA271_Timer];
    }
}


void PXA271_Driver::PerformanceCounter_Uninitialize()
{
    if(g_PXA271_Driver.m_PerformanceCounter != NULL)
    {
        PXA271_TIMER_Driver::Uninitialize( c_PerformanceCounter_Timer );

        g_PXA271_Driver.m_PerformanceCounter = NULL;
    }
}

UINT32 PXA271_Driver::PerformanceCounter()
{
    return *g_PXA271_Driver.m_PerformanceCounter;
}

#endif  // !defined(BUILD_RTM)


