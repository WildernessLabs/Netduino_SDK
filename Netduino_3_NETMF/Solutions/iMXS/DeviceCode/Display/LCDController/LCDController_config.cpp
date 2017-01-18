////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <Drivers\Display\A025DL02\A025DL02.h>

//////////////////////////////////////////////////////////////////////////////

#define A025DL02_SCREEN_WIDTH            320
#define A025DL02_SCREEN_HEIGHT           240
#define A025DL02_BITS_PER_PIXEL          16
#define A025DL02_ORIENTATION             0

#define A025DL02_ENABLE_TFT              TRUE
#define A025DL02_ENABLE_COLOR            TRUE
#define A025DL02_PIXEL_POLARITY          TRUE
#define A025DL02_FIRST_LINE_POLARITY     FALSE
#define A025DL02_LINE_PULSE_POLARITY     FALSE
#define A025DL02_SHIFT_CLK_POLARITY      TRUE
#define A025DL02_OUTPUT_ENABLE_POLARITY  TRUE
#define A025DL02_CLK_IDLE_ENABLE         TRUE
#define A025DL02_CLK_SELECT_ENABLE       TRUE

#define A025DL02_PIXELCLOCKDIVIDER       (19 - 1)    // Div by 19 (96/19 = 5.05 Mhz)
#define A025DL02_BUS_WIDTH               16
#define A025DL02_HWIDTH                  0
#define A025DL02_HWAIT1                  8
#define A025DL02_HWAIT2                  57
#define A025DL02_VWIDTH                  1
#define A025DL02_VWAIT1                  2
#define A025DL02_VWAIT2                  20

//////////////////////////////////////////////////////////////////////////////


#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata = "g_LcdController_Config"
#endif


DISPLAY_CONTROLLER_CONFIG g_LcdController_Config =
{
    A025DL02_SCREEN_WIDTH,           // UINT32 Width;
    A025DL02_SCREEN_HEIGHT,          // UINT32 Height;
    A025DL02_ENABLE_TFT,             // BOOL EnableTFT;
    A025DL02_ENABLE_COLOR,           // BOOL EnableColor;
    A025DL02_PIXEL_POLARITY,         // BOOL PixelPolarity;           (TRUE == high)
    A025DL02_FIRST_LINE_POLARITY,    // BOOL FirstLineMarkerPolarity; (FALSE == low)
    A025DL02_LINE_PULSE_POLARITY,    // BOOL LinePulsePolarity;
    A025DL02_SHIFT_CLK_POLARITY,     // BOOL ShiftClockPolarity;
    A025DL02_OUTPUT_ENABLE_POLARITY, // BOOL OutputEnablePolarity;
    A025DL02_CLK_IDLE_ENABLE,        // BOOL ClockIdleEnable;
    A025DL02_CLK_SELECT_ENABLE,      // BOOL ClockSelectEnable;

    A025DL02_PIXELCLOCKDIVIDER, // UINT32 PixelClockDivider;
    A025DL02_BUS_WIDTH,         // UINT32 BusWidth;
    A025DL02_BITS_PER_PIXEL,    // UINT32 BitsPerPixel;
    A025DL02_ORIENTATION,       // UINT8 Orientation;

    A025DL02_HWIDTH,  // UINT32 HorizontalSyncPulseWidth;
    A025DL02_HWAIT1,  // UINT32 HorizontalSyncWait1;
    A025DL02_HWAIT2,  // UINT32 HorizontalSyncWait2;
    A025DL02_VWIDTH,  // UINT32 VerticalSyncPulseWidth;
    A025DL02_VWAIT1,  // UINT32 VerticalSyncWait1;
    A025DL02_VWAIT2,  // UINT32 VerticalSyncWait2;
};

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata
#endif

