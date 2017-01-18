//-----------------------------------------------------------------------------
// Software that is described herein is for illustrative purposes only  
// which provides customers with programming information regarding the  
// products. This software is supplied "AS IS" without any warranties.  
// NXP Semiconductors assumes no responsibility or liability for the 
// use of the software, conveys no license or title under any patent, 
// copyright, or mask work right to the product. NXP Semiconductors 
// reserves the right to make changes in the software without 
// notification. NXP Semiconductors also make no representation or 
// warranty that such application will be suitable for the specified 
// use without further testing or modification. 
//-----------------------------------------------------------------------------

#ifndef _DRIVERS_DISPLAY_G240320LTSW_FUNCTIONS_H_
#define _DRIVERS_DISPLAY_G240320LTSW_FUNCTIONS_H_ 1
#include "TFT-G240320LTSW-118W.h"

//--//

BOOL LCD_Initialize()
{
    return G240320LTSW_Driver::Initialize();
}

BOOL LCD_Uninitialize()
{
    return G240320LTSW_Driver::Uninitialize();
}

void LCD_PowerSave( BOOL On )
{
    G240320LTSW_Driver::PowerSave( On );
}

void LCD_Clear()
{
    G240320LTSW_Driver::Clear();
}

void LCD_BitBlt( int width, int height, int widthInWords, UINT32 data[], BOOL fUseDelta )
{
    G240320LTSW_Driver::BitBlt( width, height, widthInWords, data, fUseDelta );
}

void LCD_BitBltEx( int x, int y, int width, int height, UINT32 data[] )
{
    G240320LTSW_Driver::BitBltEx( x, y, width, height, data );
}

void LCD_WriteChar( unsigned char c, int row, int col )
{
    G240320LTSW_Driver::WriteChar( c, row, col );
}

void LCD_WriteFormattedChar( unsigned char c )
{
    G240320LTSW_Driver::WriteFormattedChar( c );
}

INT32 LCD_GetWidth()
{
    return g_G240320LTSW_Config.ControllerConfig.Width;
}

INT32 LCD_GetHeight()
{
    return g_G240320LTSW_Config.ControllerConfig.Height;
}

INT32 LCD_GetBitsPerPixel()
{
    return g_G240320LTSW_Config.ControllerConfig.BitsPerPixel;
}

UINT32 LCD_GetPixelClockDivider()
{
    return g_G240320LTSW_Config.ControllerConfig.PixelClockDivider;
}

INT32 LCD_GetOrientation()
{
    return g_G240320LTSW_Config.ControllerConfig.Orientation;
}

UINT32 LCD_ConvertColor(UINT32 color)
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();

    return color;
}

//--//

#endif // _DRIVERS_DISPLAY_G240320LTSW_FUNCTIONS_H_


