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
 *    Copyright (C) 2010 Renesas Electronics America Inc. All rights reserved.
 */
 
#include <tinyhal.h>
#include "SH7264_VDC3.h"
#include "..\Include\SH7264_VDC3_decl.h"

extern SH7264VDC3_CONTROLLER_CONFIG g_SH7264VDC3_Config;

BOOL VDC3_Initialize(unsigned short uWidth, unsigned short uHeight)
{
    static unsigned short tmp;
    
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    
    tmp = g_SH7264VDC3_Config.Width;        //Read to make it accessible
    tmp = g_SH7264VDC3_Config.Height;       //Read to make it accessible
    
    g_SH7264VDC3_Config.Width = uWidth;
    g_SH7264VDC3_Config.Height = uHeight;
      
    return SH7264_VDC3_Driver::Initialize( g_SH7264VDC3_Config );
}

BOOL VDC3_Controller_Initialize(SH7264VDC3_CONTROLLER_CONFIG& config)
{   
    NATIVE_PROFILE_HAL_PROCESSOR_LCD();
    return SH7264_VDC3_Driver::Initialize( config );    
}

BOOL VDC3_Controller_Uninitialize()
{
    NATIVE_PROFILE_HAL_PROCESSOR_LCD();
    return SH7264_VDC3_Driver::Uninitialize();
}

BOOL VDC3_Controller_Enable(BOOL fEnable)
{
    NATIVE_PROFILE_HAL_PROCESSOR_LCD();
    return SH7264_VDC3_Driver::Enable( fEnable );    
}

unsigned short* VDC3_GetFrameBuffer()
{
    NATIVE_PROFILE_HAL_PROCESSOR_LCD();
    return SH7264_VDC3_Driver::GetFrameBuffer();
}
