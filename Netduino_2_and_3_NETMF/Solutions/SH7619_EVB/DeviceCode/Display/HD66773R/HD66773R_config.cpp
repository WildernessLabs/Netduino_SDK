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
#include "..\Drivers\Display\HD66773R\HD66773R.h"

//////////////////////////////////////////////////////////////////////////////

#define HD66773R_SCREEN_WIDTH           (128)  
#define HD66773R_SCREEN_HEIGHT          (160)  
#define HD66773R_BITS_PER_PIXEL         16
#define HD66773R_ORIENTATION            0

#define HD66773R_TIME_POWER_STABLE      10000
#define HD66773R_TIME_SDA_SETUP         2          // Time after presenting data until rising clock edge.
#define HD66773R_TIME_SDA_HOLD          2          // Time after rising clock edge to data change.
#define HD66773R_TIME_MIN_CMD_HOLD_OFF  2          // Minimum spacing between commands on serial bus.
#define HD66773R_TIME_LOAD_SETUP        2          // Time between asserting LOAD and first bit.
#define HD66773R_TIME_LOAD_HOLD         2          // Time after last bit and unasserting LOAD.

#define HD66773R_ENABLE_TFT             TRUE
#define HD66773R_ENABLE_COLOR           TRUE
#define HD66773R_PIXEL_POLARITY         TRUE
#define HD66773R_FIRST_LINE_POLARITY    FALSE
#define HD66773R_LINE_PULSE_POLARITY    FALSE
#define HD66773R_SHIFT_CLK_POLARITY     FALSE
#define HD66773R_OUTPUT_ENABLE_POLARITY TRUE
#define HD66773R_CLK_IDLE_ENABLE        TRUE
#define HD66773R_CLK_SELECT_ENABLE      TRUE

#define HD66773R_PIXELCLOCKDIVIDER      (29 - 1)    // Div by 19 (96/19 = 5.05 Mhz)
#define HD66773R_BUS_WIDTH              16
#define HD66773R_HWIDTH                 7
#define HD66773R_HWAIT1                 43
#define HD66773R_HWAIT2                 9
#define HD66773R_VWIDTH                 2
#define HD66773R_VWAIT1                 3
#define HD66773R_VWAIT2                 5

//////////////////////////////////////////////////////////////////////////////

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata = "g_HD66773R_Config"
#endif

DISPLAY_CONTROLLER_CONFIG g_HD66773R_Config =
{
    HD66773R_SCREEN_WIDTH,              // UINT32 Width;
    HD66773R_SCREEN_HEIGHT,             // UINT32 Height;
    HD66773R_ENABLE_TFT,                // BOOL EnableTFT;
    HD66773R_ENABLE_COLOR,              // BOOL EnableColor;
    HD66773R_PIXEL_POLARITY,            // BOOL PixelPolarity;           (TRUE == high)
    HD66773R_FIRST_LINE_POLARITY,       // BOOL FirstLineMarkerPolarity; (FALSE == low)
    HD66773R_LINE_PULSE_POLARITY,       // BOOL LinePulsePolarity;
    HD66773R_SHIFT_CLK_POLARITY,        // BOOL ShiftClockPolarity;
    HD66773R_OUTPUT_ENABLE_POLARITY,    // BOOL OutputEnablePolarity;
    HD66773R_CLK_IDLE_ENABLE,           // BOOL ClockIdleEnable;
    HD66773R_CLK_SELECT_ENABLE,         // BOOL ClockSelectEnable;

    HD66773R_PIXELCLOCKDIVIDER,         // UINT32 PixelClockDivider;
    HD66773R_BUS_WIDTH,                 // UINT32 BusWidth;
    HD66773R_BITS_PER_PIXEL,            // UINT32 BitsPerPixel;
    HD66773R_ORIENTATION,               // UINT8 Orientation;
    
    HD66773R_HWIDTH,                    // UINT32 HorizontalSyncPulseWidth;
    HD66773R_HWAIT1,                    // UINT32 HorizontalSyncWait1;
    HD66773R_HWAIT2,                    // UINT32 HorizontalSyncWait2;
    HD66773R_VWIDTH,                    // UINT32 VerticalSyncPulseWidth;
    HD66773R_VWAIT1,                    // UINT32 VerticalSyncWait1;
    HD66773R_VWAIT2,                    // UINT32 VerticalSyncWait2;    
};

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata
#endif

