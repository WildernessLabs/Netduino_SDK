////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cores\arm\include\cpu.h>
#include "AT91_GPIO.h"
//--//

BOOL CPU_GPIO_Initialize()
{
    return AT91_GPIO_Driver::Initialize();
}

BOOL CPU_GPIO_Uninitialize()
{
    return AT91_GPIO_Driver::Uninitialize();
}

UINT32 CPU_GPIO_Attributes( GPIO_PIN Pin )
{
    return AT91_GPIO_Driver::Attributes( Pin );
}

void CPU_GPIO_DisablePin( GPIO_PIN Pin, GPIO_RESISTOR ResistorState, UINT32 Direction, GPIO_ALT_MODE AltFunction )
{
    AT91_GPIO_Driver::DisablePin( Pin, ResistorState, Direction, AltFunction );
}

void CPU_GPIO_EnableOutputPin( GPIO_PIN Pin, BOOL InitialState )
{
    AT91_GPIO_Driver::EnableOutputPin( Pin, InitialState );
}

BOOL CPU_GPIO_EnableInputPin( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE ISR, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
    return AT91_GPIO_Driver::EnableInputPin( Pin, GlitchFilterEnable, ISR, 0, IntEdge, ResistorState );
}

BOOL CPU_GPIO_EnableInputPin2( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE ISR, void* ISR_Param, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
    return AT91_GPIO_Driver::EnableInputPin( Pin, GlitchFilterEnable, ISR, ISR_Param, IntEdge, ResistorState );
}

BOOL CPU_GPIO_GetPinState( GPIO_PIN Pin )
{
    return AT91_GPIO_Driver::GetPinState( Pin );
}

void CPU_GPIO_SetPinState( GPIO_PIN Pin, BOOL PinState )
{
    AT91_GPIO_Driver::SetPinState( Pin, PinState );
}

INT32 CPU_GPIO_GetPinCount()
{
    return AT91_GPIO_Driver::c_MaxPins;
}

void CPU_GPIO_GetPinsMap( UINT8* pins, size_t size )
{
    AT91_GPIO_Driver::GetPinsMap( pins, size );
}

UINT8 CPU_GPIO_GetSupportedResistorModes(GPIO_PIN pin)
{
    return AT91_GPIO_Driver::c_GPIO_ResistorMode;
}

UINT8 CPU_GPIO_GetSupportedInterruptModes(GPIO_PIN pin)
{
   return AT91_GPIO_Driver::c_GPIO_InterruptMode;
}


//--//

BOOL CPU_GPIO_PinIsBusy( GPIO_PIN Pin )
{
    return AT91_GPIO_Driver::PinIsBusy( Pin );
}

BOOL CPU_GPIO_ReservePin( GPIO_PIN Pin, BOOL fReserve )
{
    return AT91_GPIO_Driver::ReservePin( Pin, fReserve );
}

UINT32 CPU_GPIO_GetDebounce()
{
    return AT91_GPIO_Driver::GetDebounce();
}

BOOL CPU_GPIO_SetDebounce( INT64 debounceTimeMilliseconds )
{
    return AT91_GPIO_Driver::SetDebounce( debounceTimeMilliseconds );
}

