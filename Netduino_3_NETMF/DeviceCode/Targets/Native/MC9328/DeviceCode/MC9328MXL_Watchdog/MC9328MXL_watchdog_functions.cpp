////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cores\arm\include\cpu.h>

HRESULT Watchdog_Enable( UINT32 TimeoutMilliseconds, WATCHDOG_INTERRUPT_CALLBACK isr, void* context )
{
    NATIVE_PROFILE_HAL_PROCESSOR_WATCHDOG();
    return MC9328MXL_WATCHDOG_Driver::Enable( TimeoutMilliseconds, isr, context );
}

void Watchdog_Disable()
{
    NATIVE_PROFILE_HAL_PROCESSOR_WATCHDOG();
    MC9328MXL_WATCHDOG_Driver::Disable();
}

void Watchdog_ResetCounter()
{
    NATIVE_PROFILE_HAL_PROCESSOR_WATCHDOG();
    MC9328MXL_WATCHDOG_Driver::ResetCounter();
}

void Watchdog_ResetCpu()
{
    NATIVE_PROFILE_HAL_PROCESSOR_WATCHDOG();
    MC9328MXL_WATCHDOG_Driver::ResetCpu();
}

