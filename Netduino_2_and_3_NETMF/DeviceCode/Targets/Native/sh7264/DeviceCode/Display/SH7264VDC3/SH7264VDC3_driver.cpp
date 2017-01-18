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
#include "SH7264VDC3.h"
#include "..\..\Include\SH7264_VDC3_decl.h"

#undef  TRACE_ALWAYS
#define TRACE_ALWAYS               		0x00000001
#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

SH7264VDC3_Driver g_SH7264VDC3_Driver;

BOOL SH7264VDC3_Driver::Initialize()
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY(); 
    BOOL bRet = TRUE;
    
    //reset the cursor pos to the begining
    g_SH7264VDC3_Driver.m_cursor = 0;

	bRet = VDC3_Controller_Initialize(g_SH7264VDC3_Config);
	
//	Clear();	
    return bRet;	
}

BOOL SH7264VDC3_Driver::Uninitialize()
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    Clear();

	VDC3_Controller_Enable(FALSE);

    BOOL ret = VDC3_Controller_Uninitialize();

    return ret;
}

void SH7264VDC3_Driver::Clear()
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    memset( VDC3_GetFrameBuffer(), 0, SizeInBytes() );

    //reset the cursor pos to the begining
    g_SH7264VDC3_Driver.m_cursor = 0;
}

void SH7264VDC3_Driver::PowerSave( BOOL On )
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    return;
}

// Macro for retriving pixel value in 1-bit bitmaps
//#define SH7264VDC3_GETBIT(_x,_y,_data,_widthInWords) (((_data[((_x)/32) + (_y)*(_widthInWords)])>>((_x)%32)) & 0x1)
#define SH7264VDC3_GETBIT(_x,_y,_data,_widthInWords) ((_data[_y] >> _x)& 0x1)
#define LCD_BITBLT_COLOR_0  0x0000
#define LCD_BITBLT_COLOR_1  0x9E79


void SH7264VDC3_Driver::BitBlt( int width, int height, int widthInWords, UINT32 data[], BOOL fUseDelta )
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    ASSERT(width  <= g_SH7264VDC3_Config.Width );
    ASSERT(height <= g_SH7264VDC3_Config.Height);

    BitBltEx( 0, 0, width, height, data );

}

void SH7264VDC3_Driver::BitBltEx( int x, int y, int width, int height, UINT32 data[] )
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    // Make sure the region being redrawn is completely inside the screen
    ASSERT((x >= 0) && ((x+width)  <= g_SH7264VDC3_Config.Width));
    ASSERT((y >= 0) && ((y+height) <= g_SH7264VDC3_Config.Height));

    // Adjust x and width so they start out word-aligned
    UINT32  leftAdjustment = x % PixelsPerWord();
    x     -= leftAdjustment;
    width += leftAdjustment;


    // Set the starting addresses and size based on the clipping region
    UINT32  firstPixelAdj = (y * WidthInWords()) + (x / PixelsPerWord());
    UINT32* destAddr      = (UINT32*)VDC3_GetFrameBuffer() + firstPixelAdj;
    UINT32* srcAddr       = data + firstPixelAdj;
    UINT32  widthInBytes  = (width + (PixelsPerWord() - 1)) / PixelsPerWord() * 4;

    // Copy away
    for(int i = 0; i < height; i++)
    {
        memcpy(destAddr, srcAddr, widthInBytes);
        destAddr += WidthInWords();
        srcAddr  += WidthInWords();
    }
}

void SH7264VDC3_Driver::WriteChar( unsigned char c, int row, int col )
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();	
    // convert to LCD pixel coordinates
	
	row *= Font_Height();
    col *= Font_Width();
	
    if(row > (g_SH7264VDC3_Config.Height - Font_Height())) return;
    if(col > (g_SH7264VDC3_Config.Width  - Font_Width() )) return;

    const UINT8* font = Font_GetGlyph( c );

	unsigned short* ScreenBuffer = VDC3_GetFrameBuffer();

    for(int y = 0; y < Font_Height(); y++)
    {
		for(int x = 0; x < Font_Width(); x+=1)
        {
            // the font data is mirrored
            if(SH7264VDC3_GETBIT(Font_Width() - 1 - x  ,y,font,1)) 
				ScreenBuffer[(row+y)*g_SH7264VDC3_Config.Width + (col+x)] = 0xF800;
        }    	
    }
    
	VDC3_Controller_Enable(TRUE);
}

void SH7264VDC3_Driver::WriteFormattedChar( unsigned char c )
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
	
	if (g_SH7264VDC3_Driver.m_cursor > (g_SH7264VDC3_Config.Height * g_SH7264VDC3_Config.Width))
			g_SH7264VDC3_Driver.m_cursor = 0;
			
    if(c < 32)
    {		
        switch(c)
        {		
        case '\b':                      /* backspace, clear previous char and move cursor back */
            if((g_SH7264VDC3_Driver.m_cursor % TextColumns()) > 0)
            {
                g_SH7264VDC3_Driver.m_cursor--;
                LCD_WriteChar( ' ', g_SH7264VDC3_Driver.m_cursor / TextColumns(), g_SH7264VDC3_Driver.m_cursor % TextColumns() );
            }
            break;

        case '\f':                      /* formfeed, clear screen and home cursor */
            //LCD_Clear();
            g_SH7264VDC3_Driver.m_cursor = 0;
            break;

        case '\n':                      /* newline */
            g_SH7264VDC3_Driver.m_cursor += TextColumns();
            g_SH7264VDC3_Driver.m_cursor -= (g_SH7264VDC3_Driver.m_cursor % TextColumns());
            break;

        case '\r':                      /* carriage return */
            g_SH7264VDC3_Driver.m_cursor -= (g_SH7264VDC3_Driver.m_cursor % TextColumns());
            break;

        case '\t':                      /* horizontal tab */
            g_SH7264VDC3_Driver.m_cursor += (Font_TabWidth() - ((g_SH7264VDC3_Driver.m_cursor % TextColumns()) % Font_TabWidth()));
            // deal with line wrap scenario
            if((g_SH7264VDC3_Driver.m_cursor % TextColumns()) < Font_TabWidth())
            {
                // bring the cursor to start of line
                g_SH7264VDC3_Driver.m_cursor -= (g_SH7264VDC3_Driver.m_cursor % TextColumns());
            }
            break;

        case '\v':                      /* vertical tab */
            g_SH7264VDC3_Driver.m_cursor += TextColumns();
            break;

        default:
            DEBUG_TRACE2(TRACE_ALWAYS, "Unrecognized control character in LCD_WriteChar: %2u (0x%02x)\r\n", (unsigned int) c, (unsigned int) c);
            break;
        }
    }
    else
    {		
        LCD_WriteChar( c, g_SH7264VDC3_Driver.m_cursor / TextColumns(), g_SH7264VDC3_Driver.m_cursor % TextColumns() );
        g_SH7264VDC3_Driver.m_cursor++;
    }

    if(g_SH7264VDC3_Driver.m_cursor >= (TextColumns() * TextRows()))
    {
        g_SH7264VDC3_Driver.m_cursor = 0;
    }
}

UINT32 SH7264VDC3_Driver::PixelsPerWord()
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    return ((8*sizeof(UINT32)) / g_SH7264VDC3_Config.BitsPerPixel);
}

UINT32 SH7264VDC3_Driver::TextRows()
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    return (g_SH7264VDC3_Config.Height / Font_Height());
}

UINT32 SH7264VDC3_Driver::TextColumns()
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();	
    return (g_SH7264VDC3_Config.Width / Font_Width());
}

UINT32 SH7264VDC3_Driver::WidthInWords()
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    return ((g_SH7264VDC3_Config.Width + (PixelsPerWord() - 1)) / PixelsPerWord());
}

UINT32 SH7264VDC3_Driver::SizeInWords()
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    return (WidthInWords() * g_SH7264VDC3_Config.Height);
}

UINT32 SH7264VDC3_Driver::SizeInBytes()
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    return (SizeInWords() * sizeof(UINT32));
}
