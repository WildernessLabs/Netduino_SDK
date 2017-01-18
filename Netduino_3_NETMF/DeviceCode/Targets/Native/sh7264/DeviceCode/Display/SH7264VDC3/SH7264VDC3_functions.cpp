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
 
#include "SH7264VDC3.h"


//////////////////////////////////////////////////////////////////////////////

BOOL LCD_Initialize()
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    return SH7264VDC3_Driver::Initialize();
}

BOOL LCD_Uninitialize()
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();    
    return SH7264VDC3_Driver::Uninitialize();
}

void LCD_PowerSave( BOOL On )
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    SH7264VDC3_Driver::PowerSave( On );
}

void LCD_Clear()
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    SH7264VDC3_Driver::Clear();
}

void LCD_BitBlt( int width, int height, int widthInWords, UINT32 data[], BOOL fUseDelta )
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    SH7264VDC3_Driver::BitBlt( width, height, widthInWords, data, fUseDelta );
}

void LCD_BitBltEx( int x, int y, int width, int height, UINT32 data[] )
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    SH7264VDC3_Driver::BitBltEx( x, y, width, height, data );
}

void LCD_WriteChar( unsigned char c, int row, int col )
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    SH7264VDC3_Driver::WriteChar( c, row, col );
}

void LCD_WriteFormattedChar( unsigned char c )
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    SH7264VDC3_Driver::WriteFormattedChar( c );
}

INT32 LCD_GetWidth()
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    //Screen is rotated
    return g_SH7264VDC3_Config.Width;
}

INT32 LCD_GetHeight()
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    //Screen is rotated
    return g_SH7264VDC3_Config.Height;
}

INT32 LCD_GetBitsPerPixel()
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    return g_SH7264VDC3_Config.BitsPerPixel;
}

UINT32 LCD_GetPixelClockDivider()
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    return g_SH7264VDC3_Config.PixelClockDivider;
}

INT32 LCD_GetOrientation()
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    return g_SH7264VDC3_Config.Orientation;
}

UINT32 LCD_ConvertColor(UINT32 color)
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    return color;
}
