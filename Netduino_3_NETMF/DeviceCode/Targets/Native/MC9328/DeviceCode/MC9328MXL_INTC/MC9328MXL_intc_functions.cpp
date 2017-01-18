////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cores\arm\include\cpu.h>


void CPU_INTC_Initialize()
{
    MC9328MXL_AITC_Driver::Initialize();
}

BOOL CPU_INTC_ActivateInterrupt( UINT32 Irq_Index, HAL_CALLBACK_FPN ISR, void* ISR_Param )
{
    return MC9328MXL_AITC_Driver::ActivateInterrupt( Irq_Index, FALSE, ISR, ISR_Param, MC9328MXL_AITC::c_IRQ_Priority_0 );
}

BOOL CPU_INTC_DeactivateInterrupt( UINT32 Irq_Index )
{
    return MC9328MXL_AITC_Driver::DeactivateInterrupt( Irq_Index );
}

BOOL CPU_INTC_InterruptEnable( UINT32 Irq_Index )
{
    return MC9328MXL_AITC_Driver::InterruptEnable( Irq_Index );
}

BOOL CPU_INTC_InterruptDisable( UINT32 Irq_Index )
{
    return MC9328MXL_AITC_Driver::InterruptDisable( Irq_Index );
}

BOOL CPU_INTC_InterruptEnableState( UINT32 Irq_Index )
{
    return MC9328MXL_AITC_Driver::InterruptEnableState( Irq_Index );
}

BOOL CPU_INTC_InterruptState( UINT32 Irq_Index )
{
    return MC9328MXL_AITC_Driver::InterruptState( Irq_Index );
}
