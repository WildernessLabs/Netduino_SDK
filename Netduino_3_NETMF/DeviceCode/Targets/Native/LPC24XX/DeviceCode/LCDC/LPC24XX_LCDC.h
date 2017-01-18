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
#include "..\LPC24XX.h"

#ifndef _LPC24XX_LCDC_H_
#define _LPC24XX_LCDC_H_ 1

/***********************************************************************
 * _BIT(p) sets the bit at position "p"
 **********************************************************************/
#undef _BIT
#define _BIT(p)	(((UINT32)(1)) << (p))

/***********************************************************************
 * _BITMASK constructs a symbol with 'field_width' least significant bits set.
 **********************************************************************/
#undef _BITMASK
#define _BITMASK(field_width) (_BIT(field_width) - 1)

/***********************************************************************
 * _SBF(p,v) sets the bit field starting at position "p" to value "v". 
 **********************************************************************/
#undef _SBF
#define _SBF(p,v) (((UINT32)(v)) << (p))

/***********************************************************************
 * Color LCD controller horizontal axis plane control register definitions
 **********************************************************************/

/* LCD controller horizontal axis plane control register pixels per line */
#define CLCDC_LCDTIMING0_PPL_WIDTH 6
#define CLCDC_LCDTIMING0_PPL(n) _SBF(2, (((n) / 16) - 1) & _BITMASK(CLCDC_LCDTIMING0_PPL_WIDTH))
/* LCD controller horizontal axis plane control register HSYNC pulse width */
#define CLCDC_LCDTIMING0_HSW_WIDTH 8
#define CLCDC_LCDTIMING0_HSW(n) _SBF(8, ((n) - 1) & _BITMASK(CLCDC_LCDTIMING0_HSW_WIDTH))
/* LCD controller horizontal axis plane control register horizontal front porch */
#define CLCDC_LCDTIMING0_HFP_WIDTH 8
#define CLCDC_LCDTIMING0_HFP(n)	_SBF(16, ((n) - 1) & _BITMASK(CLCDC_LCDTIMING0_HFP_WIDTH))
/* LCD controller horizontal axis plane control register horizontal back porch */
#define CLCDC_LCDTIMING0_HBP_WIDTH 8
#define CLCDC_LCDTIMING0_HBP(n)	_SBF(24, ((n) - 1) & _BITMASK(CLCDC_LCDTIMING0_HBP_WIDTH))

/***********************************************************************
 * Color LCD controller vertical axis plane control register definitions
 **********************************************************************/

/* LCD controller vertical axis plane control register lines per panel */
#define CLCDC_LCDTIMING1_LPP_WIDTH 10
#define CLCDC_LCDTIMING1_LPP(n)	_SBF(0, ((n) - 1) & _BITMASK(CLCDC_LCDTIMING1_LPP_WIDTH))
/* LCD controller vertical axis plane control register VSYNC pulse width */
#define CLCDC_LCDTIMING1_VSW_WIDTH 6
#define CLCDC_LCDTIMING1_VSW(n)	_SBF(10, ((n) - 1) & _BITMASK(CLCDC_LCDTIMING1_VSW_WIDTH))
/* LCD controller vertical axis plane control register vertical front porch */
#define CLCDC_LCDTIMING1_VFP_WIDTH 8 
#define CLCDC_LCDTIMING1_VFP(n)	_SBF(16, (n) & _BITMASK(CLCDC_LCDTIMING1_VFP_WIDTH))
/* LCD controller vertical axis plane control register vertical back porch */
#define CLCDC_LCDTIMING1_VBP_WIDTH 8
#define CLCDC_LCDTIMING1_VBP(n)	_SBF(24, (n) & _BITMASK(CLCDC_LCDTIMING1_VBP_WIDTH))

/***********************************************************************
 * Color LCD controller clock and signal polarity control register definitions
 **********************************************************************/

/* LCD controller clock and signal polarity control register panel clock divisor low*/
#define CLCDC_LCDTIMING2_PCD(n)	 CLCDC_LCDTIMING2_PCD_hi(n) | CLCDC_LCDTIMING2_PCD_lo(n)
#define CLCDC_LCDTIMING2_PCD_lo(n)	 (_SBF(0, ((n) - 2 )) & 0x0000001f)
/* LCD controller clock and signal polarity control register clock select */
#define CLCDC_LCDTIMING2_CLKSEL _BIT(5)
/* LCD controller clock and signal polarity control register AC bias pin frequency */
#define CLCDC_LCDTIMING2_ACB_WIDTH 5
#define CLCDC_LCDTIMING2_ACB(n)	_SBF(6, ((n) - 1) & _BITMASK(CLCDC_LCDTIMING2_ACB_WIDTH))
/* LCD controller clock and signal polarity control register invert VSYNC */
#define CLCDC_LCDTIMING2_IVS    _BIT(11)
/* LCD controller clock and signal polarity control register invert HSYNC */
#define CLCDC_LCDTIMING2_IHS    _BIT(12)
/* LCD controller clock and signal polarity control register invert plane clock */
#define CLCDC_LCDTIMING2_IPC    _BIT(13)
/* LCD controller clock and signal polarity control register invert output enable */
#define CLCDC_LCDTIMING2_IOE    _BIT(14)
/* LCD controller clock and signal polarity control register clocks per line */
#define CLCDC_LCDTIMING2_CPL_WIDTH 10
#define CLCDC_LCDTIMING2_CPL(n)	_SBF(16, (n) & _BITMASK(CLCDC_LCDTIMING2_CPL_WIDTH))
/* LCD controller clock and signal polarity control register bypass pixel clock divider */
#define CLCDC_LCDTIMING2_BCD 	_BIT(26)
/* LCD controller clock and signal polarity control register panel clock divisor high*/
#define CLCDC_LCDTIMING2_PCD_hi(n)	 (_SBF(22,((n) - 2 )) & 0xf8000000)

/**********************************************************************
 * Color LCD Controller line end control register definitions
 *********************************************************************/ 

/* Line End Signal Delay */ 
#define CLCDC_LCDTIMING3_LED_WIDTH 7
#define CLCDC_LCDTIMING3_LED(n) _SBF(0, ((n) - 1) & _BITMASK(CLCDC_LCDTIMING3_LED_WIDTH))
/* Line End Enable */ 
#define CLCDC_LCDTIMING3_LEE    _BIT(16)

/***********************************************************************
 * Color LCD controller control register definitions
 **********************************************************************/

/* LCD control enable bit */
#define CLCDC_LCDCTRL_ENABLE    _BIT(0)
/* LCD control 1 bit per pixel bit field */ 
#define CLCDC_LCDCTRL_BPP1      _SBF(1, 0)
/* LCD control 2 bits per pixel bit field */ 
#define CLCDC_LCDCTRL_BPP2      _SBF(1, 1)
/* LCD control 4 bits per pixel bit field */ 
#define CLCDC_LCDCTRL_BPP4      _SBF(1, 2)
/* LCD control 8 bits per pixel bit field */ 
#define CLCDC_LCDCTRL_BPP8      _SBF(1, 3)
/* LCD control 16 bits per pixel bit field */ 
#define CLCDC_LCDCTRL_BPP16     _SBF(1, 4)
/* LCD control 24 bits per pixel bit field */ 
#define CLCDC_LCDCTRL_BPP24     _SBF(1, 5)
/* LCD control 16 bits (5:6:5 mode) per pixel bit field */ 
#define CLCDC_LCDCTRL_BPP16_565_MODE _SBF(1, 6)
/* LCD control 12 bits (4:4:4 mode) per pixel bit field */ 
#define CLCDC_LCDCTRL_BPP12_444_MODE _SBF(1, 7)
/* LCD control mono select bit */
#define CLCDC_LCDCTRL_BW_COLOR  _SBF(4, 0)
#define CLCDC_LCDCTRL_BW_MONO   _SBF(4, 1)
/* LCD controler TFT select bit */
#define CLCDC_LCDCTRL_TFT       _BIT(5)
/* LCD control monochrome LCD has 4-bit/8-bit select bit */ 
#define CLCDC_LCDCTRL_MON8      _BIT(6)
/* LCD control dual panel select bit */
#define CLCDC_LCDCTRL_DUAL      _BIT(7)
/* LCD control RGB or BGR format select bit */
#define CLCDC_LCDCTRL_RGB       _SBF(8, 0)
#define CLCDC_LCDCTRL_BGR       _SBF(8, 1)
/* LCD control big-endian byte order select bit */
#define CLCDC_LCDCTRL_BEBO      _BIT(9)
/* LCD control big-endian pixel order within a byte select bit */
#define CLCDC_LCDCTRL_BEPO      _BIT(10)
/* LCD control power enable bit */ 
#define CLCDC_LCDCTRL_PWR       _BIT(11)
/* LCD control VCOMP interrupt is start of VSYNC */
#define CLCDC_LCDCTRL_VCOMP_VS  _SBF(12, 0)
/* LCD control VCOMP interrupt is start of back porch */
#define CLCDC_LCDCTRL_VCOMP_BP  _SBF(12, 1)
/* LCD control VCOMP interrupt is start of active video */
#define CLCDC_LCDCTRL_VCOMP_AV  _SBF(12, 2)
/* LCD control VCOMP interrupt is start of front porch */
#define CLCDC_LCDCTRL_VCOMP_FP  _SBF(12, 3)
/* LCD control watermark level is 8 or more words free bit */
#define CLCDC_LCDCTRL_WATERMARK _BIT(16)

/* PINSEL11 selction for TFT 16 5:6:5 */
#define TFT_16_565 0xB

//////////////////////////////////////////////////////////////////////////////
// LCD Controller
//
struct LPC24XX_LCDC
{
    static const UINT32 c_LCDC_Base     = 0xFFE10000;

    // LCD pins
    static const UINT32 c_LCD_PWR  = LPC24XX_GPIO::c_P2_00;
    static const UINT32 c_LCD_LE   = LPC24XX_GPIO::c_P2_01;
    static const UINT32 c_LCD_DCLK = LPC24XX_GPIO::c_P2_02;
    static const UINT32 c_LCD_FP   = LPC24XX_GPIO::c_P2_03;
    static const UINT32 c_LCD_ENAB = LPC24XX_GPIO::c_P2_04;
    static const UINT32 c_LCD_LP   = LPC24XX_GPIO::c_P2_05;

    // Assignment for TFT 16 5:6:5 mode
    static const UINT32 c_LCD_VD_7  = LPC24XX_GPIO::c_P2_09; // Red 4
    static const UINT32 c_LCD_VD_6  = LPC24XX_GPIO::c_P2_08; // Red 3
    static const UINT32 c_LCD_VD_5  = LPC24XX_GPIO::c_P2_07; // Red 2
    static const UINT32 c_LCD_VD_4  = LPC24XX_GPIO::c_P2_06; // Red 1
    static const UINT32 c_LCD_VD_3  = LPC24XX_GPIO::c_P2_12; // Red 0

    static const UINT32 c_LCD_VD_15 = LPC24XX_GPIO::c_P1_25; // Green 5
    static const UINT32 c_LCD_VD_14 = LPC24XX_GPIO::c_P1_24; // Green 4
    static const UINT32 c_LCD_VD_13 = LPC24XX_GPIO::c_P1_23; // Green 3
    static const UINT32 c_LCD_VD_12 = LPC24XX_GPIO::c_P1_22; // Green 2
    static const UINT32 c_LCD_VD_11 = LPC24XX_GPIO::c_P1_21; // Green 1       
    static const UINT32 c_LCD_VD_10 = LPC24XX_GPIO::c_P1_20; // Green 0

    static const UINT32 c_LCD_VD_23 = LPC24XX_GPIO::c_P1_29; // Blue 4
    static const UINT32 c_LCD_VD_22 = LPC24XX_GPIO::c_P1_28; // Blue 3
    static const UINT32 c_LCD_VD_21 = LPC24XX_GPIO::c_P1_27; // Blue 2
    static const UINT32 c_LCD_VD_20 = LPC24XX_GPIO::c_P1_26; // Blue 1
    static const UINT32 c_LCD_VD_19 = LPC24XX_GPIO::c_P2_13; // Blue 0       

    /****/ volatile UINT32 LCD_TIMH;      /* LCD horizontal axis plane control register */
    /****/ volatile UINT32 LCD_TIMV;      /* LCD vertical axis plane control register */
    /****/ volatile UINT32 LCD_POL;       /* LCD clock and signal polarity control register */
    /****/ volatile UINT32 LCD_LE;        /* LCD line end control register */
    /****/ volatile UINT32 LCD_UPBASE;    /* LCD upper plane frame base address register */
    /****/ volatile UINT32 LCD_LPBASE;    /* LCD lower plane frame base address register */
    /****/ volatile UINT32 LCD_CTRL;      /* LCD control register */
    /****/ volatile UINT32 LCD_INTMSK;    /* LCD interrupt mask set/clear register */
    /****/ volatile UINT32 LCD_INTRAW;    /* LCD raw interrupt status register */
    /****/ volatile UINT32 LCD_INTSTAT;   /* LCD masked interrupt status register */
    /****/ volatile UINT32 LCD_INTCLR;    /* LCD interrupt clear register */
    /****/ volatile UINT32 LCD_UPCURR;    /* LCD upper panel current address value register */
    /****/ volatile UINT32 LCD_LPCURR;    /* LCD lower panel current address value register */
    /****/ volatile UINT32 dummy0[115];   /* LCD reserved */
    /****/ volatile UINT32 LCD_PAL[128];  /* LCD palette registers */
    /****/ volatile UINT32 dummy1[256];   /* LCD reserved */
    /****/ volatile UINT32 CRSR_IMG[256]; /* LCD cursor image */
    /****/ volatile UINT32 CRSR_CTRL;     /* LCD cursor control register */
    /****/ volatile UINT32 CRSR_CFG;      /* LCD cursor configuration register */
    /****/ volatile UINT32 CRSR_PAL0;     /* LCD cursor palette register */
    /****/ volatile UINT32 CRSR_PAL1;     /* LCD cursor palette register */
    /****/ volatile UINT32 CRSR_XY;       /* LCD cursor xy position register */
    /****/ volatile UINT32 CRSR_CLIP;     /* LCD cursor clip position register */
    /****/ volatile UINT32 dummy2[2];     /* LCD reserved */
    /****/ volatile UINT32 CRSR_INTMSK;   /* LCD cursor interrupt mask set/clear register */
    /****/ volatile UINT32 CRSR_INTCLR;   /* LCD cursor interrupt clear register */
    /****/ volatile UINT32 CRSR_INTRAW;   /* LCD cursor raw interrupt status register */
    /****/ volatile UINT32 CRSR_INTSTAT;  /* LCD cursor masked interrupt status register */
};                  

//////////////////////////////////////////////////////////////////////////////
// LCD Controller Driver
//
struct LPC24XX_LCDC_Driver
{
    //--//
    static BOOL Initialize(DISPLAY_CONTROLLER_CONFIG& config);
    static BOOL Enable(BOOL fEnable);
    static BOOL Uninitialize();
    //--//
    static void SetScreenBuffer(UINT32* ScreenBuffer);
    static UINT32* GetScreenBuffer();
    static void ConnectPins (void);
};

//
// LCD Controller Driver
//////////////////////////////////////////////////////////////////////////////

#endif // _LPC24XX_LCDC_H_
