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
#include "..\..\..\..\DeviceCode\Targets\Native\SH7264\DeviceCode\VDC3\SH7264_VDC3.h"

//////////////////////////////////////////////////////////////////////////////

#define SH7264VDC3_SCREEN_WIDTH           (240)
#define SH7264VDC3_SCREEN_HEIGHT          (320)
#define SH7264VDC3_BITS_PER_PIXEL         16
#define SH7264VDC3_ORIENTATION            0

#define SH7264VDC3_ENABLE_TFT             TRUE

#define SH7264VDC3_CLK_SELECT_ENABLE      TRUE

#define SH7264VDC3_PIXELCLOCKDIVIDER      4    
#define SH7264VDC3_BUS_WIDTH              16


#define SH7264VDC3_ENABLE_ALPHA               FALSE
#define SH7264VDC3_ENABLE_CHROMA_KEY          FALSE

#define SH7264VDC3_VERT_BLANKING          0x000001FF
#define SH7264VDC3_HORZ_BLANKING          0x00000200
#define SH7264VDC3_VERT_SYNCSTART         0
#define SH7264VDC3_VERT_SYNCEND           0x00000004
#define SH7264VDC3_HORZ_SYNCSTART         0
#define SH7264VDC3_HORZ_SYNCEND           0x00000004
#define SH7264VDC3_DE_START_V             0x0000000b
#define SH7264VDC3_DE_START_H             0x00000010

#define SH7264VDC3_AC_LINE_NUM            0x0000000C


#define SH7264VDC3_GRA_VSYNC_TIM        0
#define SH7264VDC3_GRA_HPOS             0   //LEFT START POS
#define SH7264VDC3_GRA_VPOS             0

#define SH7264VDC3_ALPHA_WIDTH          0
#define SH7264VDC3_ALPHA_HEIGHT         0
#define SH7264VDC3_ALPHA_HPOS           0
#define SH7264VDC3_ALPHA_VPOS           0
#define SH7264VDC3_REP_COLOR            0

// all rising edge trigger
#define SH7264VDC3_LCD_DATA_EDGE        0
#define SH7264VDC3_LCD_VSYNC_EDGE       0
#define SH7264VDC3_LCD_HSYNC_EDGE       0
#define SH7264VDC3_LCD_DE_EDGE          0
#define SH7264VDC3_LCD_MDISP_EDGE       0
//inverted
#define SH7264VDC3_LCD_VSYNC_TYPE       1
#define SH7264VDC3_LCD_HSYNC_TYPE       1
#define SH7264VDC3_LCD_DE_TYPE          1
#define SH7264VDC3_LCD_MDISP_TYPE       1


#ifdef CRT_MONITOR

#define SH7264VDC3_SCREEN_WIDTH           (240)
#define SH7264VDC3_SCREEN_HEIGHT          (320)
#define SH7264VDC3_BITS_PER_PIXEL         16
#define SH7264VDC3_ORIENTATION            0

#define SH7264VDC3_ENABLE_TFT             TRUE

#define SH7264VDC3_CLK_SELECT_ENABLE      TRUE

#define SH7264VDC3_PIXELCLOCKDIVIDER      0x03    
#define SH7264VDC3_BUS_WIDTH              16


#define SH7264VDC3_ENABLE_ALPHA               FALSE
#define SH7264VDC3_ENABLE_CHROMA_KEY          FALSE

#define SH7264VDC3_VERT_BLANKING          0x00000210
#define SH7264VDC3_HORZ_BLANKING          0x00000330
#define SH7264VDC3_VERT_SYNCSTART         0
#define SH7264VDC3_VERT_SYNCEND           0x00000002
#define SH7264VDC3_HORZ_SYNCSTART         0
#define SH7264VDC3_HORZ_SYNCEND           0x00000028
#define SH7264VDC3_DE_START_V             0x00000021
#define SH7264VDC3_DE_START_H             0x00000065

#define SH7264VDC3_AC_LINE_NUM            0x00000012


#define SH7264VDC3_GRA_VSYNC_TIM        0
#define SH7264VDC3_GRA_HPOS             85   //LEFT START POS
#define SH7264VDC3_GRA_VPOS             0

#define SH7264VDC3_ALPHA_WIDTH          0
#define SH7264VDC3_ALPHA_HEIGHT         0
#define SH7264VDC3_ALPHA_HPOS           0
#define SH7264VDC3_ALPHA_VPOS           0
#define SH7264VDC3_REP_COLOR            0

// all falling edge trigger
#define SH7264VDC3_LCD_DATA_EDGE        1
#define SH7264VDC3_LCD_VSYNC_EDGE       1 
#define SH7264VDC3_LCD_HSYNC_EDGE       1
#define SH7264VDC3_LCD_DE_EDGE          1
#define SH7264VDC3_LCD_MDISP_EDGE       1  

// all non-inverted
#define SH7264VDC3_LCD_VSYNC_TYPE       0
#define SH7264VDC3_LCD_HSYNC_TYPE       0
#define SH7264VDC3_LCD_DE_TYPE          0
#define SH7264VDC3_LCD_MDISP_TYPE       0

#endif 


//////////////////////////////////////////////////////////////////////////////

SH7264VDC3_CONTROLLER_CONFIG g_SH7264VDC3_Config =
{
    SH7264VDC3_SCREEN_WIDTH,            // UINT32 Width;
    SH7264VDC3_SCREEN_HEIGHT,           // UINT32 Height;
 
    SH7264VDC3_ENABLE_ALPHA,            //BOOL EnableAplha;
    SH7264VDC3_ENABLE_CHROMA_KEY,       //BOOL EnableChromaKey;

    
    SH7264VDC3_ENABLE_TFT,              // BOOL EnableTFT;
    SH7264VDC3_CLK_SELECT_ENABLE,       // BOOL ClockSelectEnable;
    SH7264VDC3_PIXELCLOCKDIVIDER,       // UINT8ixelClockDivider;
    SH7264VDC3_BUS_WIDTH,               // UINT8BusWidth;
    SH7264VDC3_BITS_PER_PIXEL,          // UINT8BitsPerPixel;
    SH7264VDC3_ORIENTATION,             // UINT8 Orientation;

    SH7264VDC3_LCD_DATA_EDGE,           // UINT8 LcdDataEdge;      // falling edge = 1     

    SH7264VDC3_LCD_VSYNC_EDGE,          // UINT8 LcdVSyncEdge;     // falling edge = 1
    SH7264VDC3_LCD_HSYNC_EDGE,          // UINT8 LcdHSyncEdge;     // falling edge = 1
    SH7264VDC3_LCD_DE_EDGE,             // UINT8 LcdDeEdge;        // falling edge = 1
    SH7264VDC3_LCD_MDISP_EDGE,          // UINT8 LcdMDispEdge;     // falling edge = 1

    SH7264VDC3_LCD_VSYNC_TYPE,          // UINT8 VSyncType;        // Inverted = 1; No inverted = 0
    SH7264VDC3_LCD_HSYNC_TYPE,          // UINT8 HSyncType;        // Inverted = 1; No inverted = 0
    SH7264VDC3_LCD_DE_TYPE,             // UINT8 DESycnType;       // Inverted = 1; No inverted = 0
    SH7264VDC3_LCD_MDISP_TYPE,          // UINT8 MDispType;        // Inverted = 1; No inverted = 0

    SH7264VDC3_HORZ_SYNCSTART,          // UINT8 HorizontalSyncStart;
    SH7264VDC3_HORZ_SYNCEND,            // UINT8 HorizontalSyncEnd;
    SH7264VDC3_HORZ_BLANKING,           // UINT16 HorizontalBlankingInterval;


    SH7264VDC3_VERT_SYNCSTART,          //UINT8 VerticalSyncStart;
    SH7264VDC3_HORZ_SYNCEND,            //UINT8 VerticalSyncEnd;
    SH7264VDC3_VERT_BLANKING,           //UINT16 VerticalBlankingInterval;

    SH7264VDC3_DE_START_H,              //UINT16 DE_Start_H;
    SH7264VDC3_DE_START_V,              //UINT16 DE_Start_V;

    SH7264VDC3_AC_LINE_NUM,             //UINT16 AcLineNum;
    SH7264VDC3_GRA_VSYNC_TIM,           //UINT16 GraVsyncTim;

    SH7264VDC3_GRA_HPOS,                //UINT16 GraHpos;
    SH7264VDC3_GRA_VPOS,                //UINT16 GraVpos;


    SH7264VDC3_ALPHA_WIDTH,             //UINT16 AlphaWidth;
    SH7264VDC3_ALPHA_HEIGHT,            //UINT16 AlphaHeight;
    SH7264VDC3_ALPHA_HPOS,              //UINT16 AlphaHpos;
    SH7264VDC3_ALPHA_VPOS,              //UINT16 AlphaVpos;

    SH7264VDC3_REP_COLOR,               //UINT16 RepColor;

 };

