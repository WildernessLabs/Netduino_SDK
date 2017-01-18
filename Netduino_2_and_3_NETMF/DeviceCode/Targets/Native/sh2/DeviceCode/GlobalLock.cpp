////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "tinyhal.h"
#include <smachine.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
// highest priority=> no interrupts can access
#define DISABLE_STATE   0xF
#define ENABLE_STATE    0x0
//#define SR_Init    0x00000040


SmartPtr_IRQ::SmartPtr_IRQ(void* context)
{ 
    m_context = context; 
    Disable(); 
}

SmartPtr_IRQ::~SmartPtr_IRQ() 
{ 
    Restore(); 
}

BOOL SmartPtr_IRQ::WasDisabled()
{
    return (m_state == DISABLE_STATE);
}

void SmartPtr_IRQ::Acquire()
{
    UINT32 Cp = m_state;

    if(Cp  == DISABLE_STATE)
    {
        Disable();
    }
}

void SmartPtr_IRQ::Release()
{
    UINT32 Cp = m_state;

    if(Cp != DISABLE_STATE) 
    {
        set_imask(Cp);
       
    }
}

void SmartPtr_IRQ::Probe()
{
    UINT32 Cp = m_state;


    if(Cp != DISABLE_STATE)
    {
        set_imask(Cp);
        set_imask(DISABLE_STATE);
    }
}

BOOL SmartPtr_IRQ::GetState(void* context)
{
    UINT32 Cp;
	
    Cp = get_imask();
    
    return (Cp != DISABLE_STATE);
}

BOOL SmartPtr_IRQ::ForceDisabled(void* context)
{
    UINT32 Cp;

    Cp = get_imask();
    set_imask(DISABLE_STATE);

    return (Cp != DISABLE_STATE);
}

BOOL SmartPtr_IRQ::ForceEnabled(void* context)
{
    UINT32 Cp;

    Cp = get_imask();
    
    set_imask(ENABLE_STATE);

    return (Cp != DISABLE_STATE);
}

void SmartPtr_IRQ::Disable()
{
    UINT32 Cp;

    Cp = get_imask();

    set_imask(DISABLE_STATE);

    m_state = Cp;
}

void SmartPtr_IRQ::Restore()
{
    UINT32 Cp = m_state;

    if(Cp != DISABLE_STATE)
    {
        set_imask(Cp);
    }
}


