////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MC9328MXL_LCD.h"

//--//

BOOL LCD_Controller_Initialize(DISPLAY_CONTROLLER_CONFIG& config)
{
    NATIVE_PROFILE_HAL_PROCESSOR_LCD();
    return MC9328MXL_LCD_Driver::Initialize( config );
}

BOOL LCD_Controller_Uninitialize()
{
    NATIVE_PROFILE_HAL_PROCESSOR_LCD();
    return MC9328MXL_LCD_Driver::Uninitialize();
}

BOOL LCD_Controller_Enable(BOOL fEnable)
{
    NATIVE_PROFILE_HAL_PROCESSOR_LCD();
    return MC9328MXL_LCD_Driver::Enable( fEnable );
}

UINT32* LCD_GetFrameBuffer()
{
    NATIVE_PROFILE_HAL_PROCESSOR_LCD();
    return MC9328MXL_LCD_Driver::GetScreenBuffer();
}

