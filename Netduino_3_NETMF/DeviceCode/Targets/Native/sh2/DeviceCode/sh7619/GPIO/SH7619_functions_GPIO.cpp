////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// This file is part of the Microsoft .NET Micro Framework Porting Kit Code Samples and is unsupported. 
// Copyright (C) Microsoft Corporation. All rights reserved. Use of this sample source code is subject to 
// the terms of the Microsoft license agreement under which you licensed this sample source code. 
// 
// THIS SAMPLE CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// 
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *    Copyright (C) Renesas Technology America,Ltd. 2009  All rights reserved.
 */

#include <tinyhal.h>
#include <SH7619_GPIO.h>

//--//
BOOL CPU_GPIO_Initialize()
{
    return SH7619_GPIO_Driver::Initialize();	
}

BOOL CPU_GPIO_Uninitialize()
{
    return SH7619_GPIO_Driver::Uninitialize();
}

UINT32 CPU_GPIO_Attributes( GPIO_PIN Pin )
{
    return SH7619_GPIO_Driver::Attributes( Pin );
}

void CPU_GPIO_DisablePin( GPIO_PIN Pin, GPIO_RESISTOR ResistorState, UINT32 Direction, GPIO_ALT_MODE AltFunction )
{
	SH7619_GPIO_Driver::DisablePin( Pin, ResistorState, Direction, AltFunction );
}

void CPU_GPIO_EnableOutputPin( GPIO_PIN Pin, BOOL InitialState )
{
	SH7619_GPIO_Driver::EnableOutputPin( Pin, InitialState );
}

BOOL CPU_GPIO_EnableInputPin( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE PIN_ISR, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
    return SH7619_GPIO_Driver::EnableInputPin( Pin, GlitchFilterEnable, PIN_ISR, 0, IntEdge, ResistorState );
}

BOOL CPU_GPIO_EnableInputPin2( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE PIN_ISR, void* ISR_Param, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
    return SH7619_GPIO_Driver::EnableInputPin( Pin, GlitchFilterEnable, PIN_ISR, ISR_Param, IntEdge, ResistorState );
}

BOOL CPU_GPIO_GetPinState( GPIO_PIN Pin )
{
    return SH7619_GPIO_Driver::GetPinState( Pin );
}

void CPU_GPIO_SetPinState( GPIO_PIN Pin, BOOL PinState )
{
	SH7619_GPIO_Driver::SetPinState( Pin, PinState );
}

BOOL CPU_GPIO_PinIsBusy( GPIO_PIN Pin )
{
    return SH7619_GPIO_Driver::PinIsBusy( Pin );
}

BOOL CPU_GPIO_ReservePin( GPIO_PIN Pin, BOOL fReserve )
{
    return SH7619_GPIO_Driver::ReservePin( Pin, fReserve );
}

UINT32 CPU_GPIO_GetDebounce()
{
    return SH7619_GPIO_Driver::GetDebounce();
}

BOOL CPU_GPIO_SetDebounce( INT64 debounceTimeMilliseconds )
{
    return SH7619_GPIO_Driver::SetDebounce( debounceTimeMilliseconds );
}

INT32 CPU_GPIO_GetPinCount()
{
    return SH7619_GPIO_Driver::c_MaxPins;
} 

void CPU_GPIO_GetPinsMap( UINT8* pins, size_t size )
{
    SH7619_GPIO_Driver::GetPinsMap( pins, size );
}

UINT8 CPU_GPIO_GetSupportedResistorModes( GPIO_PIN pin )
{
	return SH7619_GPIO_Driver::c_GPIO_ResistorMode;   
}

UINT8 CPU_GPIO_GetSupportedInterruptModes( GPIO_PIN pin )
{
    return SH7619_GPIO_Driver::c_GPIO_InterruptMode;    
}


/*
void CPU_GPIO_SetPinData( GPIO_PIN Pin, BOOL Value )
{
	SH7619_GPIO_Driver::SetPinData(Pin, Value);
}

unsigned short CPU_GPIO_GetPinData( GPIO_PIN Pin )
{
	return SH7619_GPIO_Driver::GetPinData(Pin);
}
*/
