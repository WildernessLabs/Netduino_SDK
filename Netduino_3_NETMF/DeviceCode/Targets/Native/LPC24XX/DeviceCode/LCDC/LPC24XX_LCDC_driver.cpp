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
#include "LPC24XX_LCDC.h"

BOOL LPC24XX_LCDC_Driver::Initialize(DISPLAY_CONTROLLER_CONFIG& config)
{
	UINT32 tmp;

	LPC24XX_LCDC & LCDC = *(LPC24XX_LCDC *)LPC24XX_LCDC::c_LCDC_Base;

	ConnectPins();

	/* Power Up the LCD controller. */
	LPC24XX::SYSCON().PCONP |= LPC24XX_SYSCON::ENABLE_LCD;

	/* Disable the display in case it is on */
	LCDC.LCD_CTRL = 0;

	/* Generate the horizontal axis plane control word */
	tmp = (CLCDC_LCDTIMING0_PPL(config.Width) |
	     CLCDC_LCDTIMING0_HSW(config.HorizontalSyncPulseWidth) |
	     CLCDC_LCDTIMING0_HFP(config.HorizontalSyncWait1) |
	     CLCDC_LCDTIMING0_HBP(config.HorizontalSyncWait2));
	LCDC.LCD_TIMH = tmp;

	/* Generate the vertical axis plane control word */
	tmp = (CLCDC_LCDTIMING1_LPP(config.Height) |
	     CLCDC_LCDTIMING1_VSW(config.VerticalSyncPulseWidth) |
	     CLCDC_LCDTIMING1_VFP(config.VerticalSyncWait1) |
	     CLCDC_LCDTIMING1_VBP(config.VerticalSyncWait2));
	LCDC.LCD_TIMV = tmp;

	/* Generate the clock and signal polarity control word */
	tmp = 0;

	if (config.OutputEnablePolarity != TRUE)
	{
	tmp |= CLCDC_LCDTIMING2_IOE;
	}

	if (config.ShiftClockPolarity != TRUE)
	{
	tmp |= CLCDC_LCDTIMING2_IPC;
	}

	if (config.LinePulsePolarity != TRUE)
	{
	tmp |= CLCDC_LCDTIMING2_IHS;
	}

	if (config.FirstLineMarkerPolarity != TRUE)
	{
	tmp |= CLCDC_LCDTIMING2_IVS;
	}

	/* Compute clocks per line based on panel type */
	// TODO support other panel types
	if ( config.EnableTFT != TRUE )
	{
	   debug_printf("LCD driver needs to be modified to support non TFT display");
	   return FALSE;
	}

	/* For TFT Clocks per line and pixels per line are the same */
	  tmp = tmp | CLCDC_LCDTIMING2_CPL(config.Width - 1);

	/* Set pixel clock divider */
	if (config.PixelClockDivider <= 1)
	{
	/* Pixel clock divider is 1, skip divider logic */
	tmp |= CLCDC_LCDTIMING2_BCD;
	}
	else
	{
	/* Add in new divider */
	tmp &= ~(CLCDC_LCDTIMING2_BCD | CLCDC_LCDTIMING2_PCD(0x1F) 
	       | 0xF8000000);
	tmp |= ((((config.PixelClockDivider-2) >> 0) & 0x1F) 
	       | ((((config.PixelClockDivider-2) >> 5) & 0x1F) << 24));
	}

	LCDC.LCD_POL = tmp;

	/* Skip line end control word - just set to 0x0 */
	LCDC.LCD_LE = 0x0;

	/* All interrupts off */
	LCDC.LCD_INTMSK = 0x0;

	if ( config.BitsPerPixel != 16 )
	{
	   debug_printf("LCD driver needs to be modified to support BPP != 16");
	   return FALSE;
	}

	// TODO support other panel types and BPP
	tmp = CLCDC_LCDCTRL_BPP16_565_MODE;

	// TODO Use unused DISPLAY_CONTROLLER_CONFIG member for Color format selection
	tmp |= CLCDC_LCDCTRL_TFT | CLCDC_LCDCTRL_BGR;

	LCDC.LCD_CTRL = tmp;

	volatile UINT32 * pPal = LCDC.LCD_PAL;
	for(int i =0; i <128;i++)
		pPal[i]=0; 

	//Set divider 0
	LPC24XX::SYSCON().LCD_CFG = 0;

	return TRUE;
}

BOOL LPC24XX_LCDC_Driver::Uninitialize()
{

    LPC24XX::SYSCON().PCONP &= ~(LPC24XX_SYSCON::ENABLE_LCD);
    return TRUE;
}

BOOL LPC24XX_LCDC_Driver::Enable( BOOL fEnable )
{
    LPC24XX_LCDC & LCDC = *(LPC24XX_LCDC *)LPC24XX_LCDC::c_LCDC_Base;

    if(fEnable)
    {
        // Turn on LCDC
        LCDC.LCD_CTRL |= CLCDC_LCDCTRL_PWR;
        LCDC.LCD_CTRL |= CLCDC_LCDCTRL_ENABLE;
    }
    else
    {
        LCDC.LCD_CTRL &= ~CLCDC_LCDCTRL_PWR;
        LCDC.LCD_CTRL &= ~CLCDC_LCDCTRL_ENABLE;
    }

    return TRUE;
}

UINT32* LPC24XX_LCDC_Driver::GetScreenBuffer()
{
    LPC24XX_LCDC & LCDC = *(LPC24XX_LCDC *)LPC24XX_LCDC::c_LCDC_Base;
    return (UINT32 *)LCDC.LCD_UPBASE;
}

void LPC24XX_LCDC_Driver::SetScreenBuffer(UINT32* ScreenBuffer)
{
    LPC24XX_LCDC & LCDC = *(LPC24XX_LCDC *)LPC24XX_LCDC::c_LCDC_Base;
    LCDC.LCD_UPBASE = (UINT32)ScreenBuffer;
}

void LPC24XX_LCDC_Driver::ConnectPins (void)
{
    // TODO Make pin list part of the display config. Compute value of PINSEL11 from Display config
    // Initialize pins
    LPC24XX::PCB().PINSEL10 = 0x0;
    LPC24XX::PCB().PINSEL11 = TFT_16_565;

    CPU_GPIO_DisablePin( LPC24XX_LCDC::c_LCD_PWR, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_3 );
    CPU_GPIO_DisablePin( LPC24XX_LCDC::c_LCD_LE, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_3 );
    CPU_GPIO_DisablePin( LPC24XX_LCDC::c_LCD_DCLK, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_3 );
    CPU_GPIO_DisablePin( LPC24XX_LCDC::c_LCD_FP, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_3 );
    CPU_GPIO_DisablePin( LPC24XX_LCDC::c_LCD_ENAB, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_3 );
    CPU_GPIO_DisablePin( LPC24XX_LCDC::c_LCD_LP, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_3 );
   
    CPU_GPIO_DisablePin( LPC24XX_LCDC::c_LCD_VD_7, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_3 );
    CPU_GPIO_DisablePin( LPC24XX_LCDC::c_LCD_VD_6, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_3 );
    CPU_GPIO_DisablePin( LPC24XX_LCDC::c_LCD_VD_5, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_3 );
    CPU_GPIO_DisablePin( LPC24XX_LCDC::c_LCD_VD_4, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_3 );
    CPU_GPIO_DisablePin( LPC24XX_LCDC::c_LCD_VD_3, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
    
    CPU_GPIO_DisablePin( LPC24XX_LCDC::c_LCD_VD_15, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
    CPU_GPIO_DisablePin( LPC24XX_LCDC::c_LCD_VD_14, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
    CPU_GPIO_DisablePin( LPC24XX_LCDC::c_LCD_VD_13, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
    CPU_GPIO_DisablePin( LPC24XX_LCDC::c_LCD_VD_12, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
    CPU_GPIO_DisablePin( LPC24XX_LCDC::c_LCD_VD_11, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
    CPU_GPIO_DisablePin( LPC24XX_LCDC::c_LCD_VD_10, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );

    CPU_GPIO_DisablePin( LPC24XX_LCDC::c_LCD_VD_23, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
    CPU_GPIO_DisablePin( LPC24XX_LCDC::c_LCD_VD_22, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
    CPU_GPIO_DisablePin( LPC24XX_LCDC::c_LCD_VD_21, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
    CPU_GPIO_DisablePin( LPC24XX_LCDC::c_LCD_VD_20, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
    CPU_GPIO_DisablePin( LPC24XX_LCDC::c_LCD_VD_19, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
}
