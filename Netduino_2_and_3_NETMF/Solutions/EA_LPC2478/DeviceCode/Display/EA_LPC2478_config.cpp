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

#include <tinyhal.h>
#include "TFT-G240320LTSW-118W.h"

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata = "g_G240320LTSW_Config"
#endif

#define G240320LTSW_WIDTH   240
#define G240320LTSW_HEIGHT  320
#define G240320LTSW_BPP     16



// Screen buffer for EA_LPC2378 LCD controller, has to be double word aligned, requirement for the LPC24xx core.
// for GNU, we used section and at scatterfile to put the screenbuffer to be aligned, instead of using the "attribute" to save some dummy memory.

#if defined(__GNUC__)

UINT8 __attribute__ ((aligned (8))) ScreenBuffer[(G240320LTSW_WIDTH*G240320LTSW_HEIGHT*G240320LTSW_BPP)/8];

#else

__align(8) UINT8 ScreenBuffer[(G240320LTSW_WIDTH*G240320LTSW_HEIGHT*G240320LTSW_BPP)/8];

#endif


G240320LTSW_CONFIG g_G240320LTSW_Config =
{
    { TRUE },                      // HAL_DRIVER_CONFIG_HEADER Header;
    {
        (UINT32 *)ScreenBuffer, // Screen Buffer
        LPC24XX_GPIO::c_P0_19,     // GPIO_PIN Data_or_Cmd
        {                          // GPIO_FLAG LcdEnable
            LPC24XX_GPIO::c_P1_18, 
            FALSE, 
        },
        {                          // SPI_CONFIGURATION SPI_Config
            LPC24XX_GPIO::c_P0_16, // Chip select
            FALSE,                 // Chip Select polarity
            TRUE,                  // MSK_IDLE
            TRUE,                  // MSK_SAMPLE_EDGE
            FALSE,                 // 16-bit mode
            900,                   // SPI Clock Rate KHz
            1,                     // CS setup time us
            1,                     // CS hold time us
            LPC24XX_SPI::c_SPI0,   // SPI Module
            {
                GPIO_PIN_NONE,     // SPI BusyPin
                FALSE,             // SPI BusyPinActiveState
            },
        },
    },
    {
        G240320LTSW_WIDTH,      // UINT32 Width;
        G240320LTSW_HEIGHT,     // UINT32 Height;
        TRUE,     // BOOL EnableTFT;
        TRUE,     // BOOL EnableColor;
        FALSE,    // Not applicable BOOL PixelPolarity; (TRUE == high)
        FALSE,    // BOOL FirstLineMarkerPolarity;      (FALSE == low)
        FALSE,    // BOOL LinePulsePolarity; Hsync
        FALSE,    // BOOL ShiftClockPolarity; Vsync
        TRUE,     // BOOL OutputEnablePolarity;
        FALSE,    // Not applicable BOOL ClockIdleEnable;
        FALSE,    // Not applicable BOOL ClockSelectEnable;

        14,       // UINT32 PixelClockDivider;
        16,       // UINT32 BusWidth;
        G240320LTSW_BPP,       // UINT32 BitsPerPixel;
        0,        // UINT8 Orientation;

        2,        // UINT32 HorizontalSyncPulseWidth;
        10,       // UINT32 HorizontalSyncWait1;
        30,       // UINT32 HorizontalSyncWait2;
        2,        // UINT32 VerticalSyncPulseWidth;
        2,        // UINT32 VerticalSyncWait1;
        4,        // UINT32 VerticalSyncWait2;
    },
};

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata
#endif
