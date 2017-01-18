////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// This file is part of the Microsoft .NET Micro Framerwork Porting Kit Code Samples and is unsupported. 
// Copyright (C) Microsoft Corporation. All rights reserved. Use of this sample source code is subject to 
// the terms of the Microsoft license agreement under which you licensed this sample source code. 
// 
// THIS SAMPLE CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// 
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cores\arm\include\cpu.h>
#include "AT91_USBHS.h"

USB_CONTROLLER_STATE * CPU_USB_GetState( int Controller )
{
    return AT91_USBHS_Driver::GetState( Controller );
}

HRESULT CPU_USB_Initialize( int Controller )
{
    return AT91_USBHS_Driver::Initialize( Controller );
}

HRESULT CPU_USB_Uninitialize( int Controller )
{
    return AT91_USBHS_Driver::Uninitialize( Controller );
}

BOOL CPU_USB_StartOutput( USB_CONTROLLER_STATE* State, int endpoint )
{
    return AT91_USBHS_Driver::StartOutput( State, endpoint );
}

BOOL CPU_USB_RxEnable( USB_CONTROLLER_STATE* State, int endpoint )
{
    return AT91_USBHS_Driver::RxEnable( State, endpoint );
}

BOOL CPU_USB_GetInterruptState()
{
    return AT91_USBHS_Driver::GetInterruptState();
}

BOOL CPU_USB_ProtectPins( int Controller, BOOL On )
{
    return AT91_USBHS_Driver::ProtectPins( Controller, On );
}

