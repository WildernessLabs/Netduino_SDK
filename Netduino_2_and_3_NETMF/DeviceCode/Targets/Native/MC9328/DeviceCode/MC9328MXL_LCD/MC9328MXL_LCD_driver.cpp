////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "MC9328MXL_LCD.h"

//////////////////////////////////////////////////////////////////////////////

#undef  TRACE_ALWAYS
#define TRACE_ALWAYS               0x00000001

#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

//////////////////////////////////////////////////////////////////////////////

#define BITS_PER_WORD 32
#define BITS_PER_PIXEL_TO_PIXELS_PER_WORD(x) (BITS_PER_WORD / x)

#define GET_EXTRA_REG_INDEX(x) (((UINT32)(x)- MC9328MXL_LCDC::c_Base) / sizeof(UINT32))

BOOL MC9328MXL_LCD_Driver::Initialize(DISPLAY_CONTROLLER_CONFIG& config)
{
    NATIVE_PROFILE_HAL_PROCESSOR_LCD();

    MC9328MXL_LCDC& LCDC = MC9328MXL::LCDC();

    UINT32 Extras[offsetof(MC9328MXL_LCDC,LCDISR)/sizeof(UINT32) + 1];

    memset(Extras, 0, sizeof(Extras));

    for(int i=0; i<ARRAYSIZE(config.RegisterExtras); i++)
    {
        if(config.RegisterExtras[i].RegAddr == 0) break;

        int val = GET_EXTRA_REG_INDEX(config.RegisterExtras[i].RegAddr);

        if(val < 0 || ARRAYSIZE(Extras) < val) continue;

        Extras[val] = config.RegisterExtras[i].RegValue;
    }
    
    // Disable GPIO on all the LCD Pins (SPI1, Port D6, 13 ~ 30)
    {
        static const UINT8 c_LCD[] =
        {
            MC9328MXL_GPIO::c_Port_D_06,
            MC9328MXL_GPIO::c_Port_D_13,
            MC9328MXL_GPIO::c_Port_D_14,
            MC9328MXL_GPIO::c_Port_D_15,
            MC9328MXL_GPIO::c_Port_D_16,
            MC9328MXL_GPIO::c_Port_D_17,
            MC9328MXL_GPIO::c_Port_D_18,
            MC9328MXL_GPIO::c_Port_D_19,
            MC9328MXL_GPIO::c_Port_D_20,
            MC9328MXL_GPIO::c_Port_D_21,
            MC9328MXL_GPIO::c_Port_D_22,
            MC9328MXL_GPIO::c_Port_D_23,
            MC9328MXL_GPIO::c_Port_D_24,
            MC9328MXL_GPIO::c_Port_D_25,
            MC9328MXL_GPIO::c_Port_D_26,
            MC9328MXL_GPIO::c_Port_D_27,
            MC9328MXL_GPIO::c_Port_D_28,
            MC9328MXL_GPIO::c_Port_D_29,
            MC9328MXL_GPIO::c_Port_D_30,
        };

        for(int pin=0; pin < ARRAYSIZE(c_LCD); pin++)
        {
            CPU_GPIO_DisablePin( c_LCD[pin], RESISTOR_DISABLED, MC9328MXL_GPIO::DDIR__OUT, GPIO_ALT_PRIMARY);
        }
    }

    // Set Screen Start Addr, Resolution
    LCDC.SSA  = MC9328MXL_LCD_Driver::c_Screen_Buffer << MC9328MXL_LCDC::SSA__SSA_shift;

    LCDC.SIZE = ((UINT32)config.Width  << MC9328MXL_LCDC::SIZE__XMAX_shift) |
                ((UINT32)config.Height << MC9328MXL_LCDC::SIZE__YMAX_shift);

    LCDC.VPW  = ((UINT32)(config.Width +  BITS_PER_PIXEL_TO_PIXELS_PER_WORD(config.BitsPerPixel) - 1) / BITS_PER_PIXEL_TO_PIXELS_PER_WORD(config.BitsPerPixel)) << MC9328MXL_LCDC::VPW__VPW_shift;
    
    // LCD timing
    {
        UINT32 val = 0;

        val |= config.EnableTFT?   MC9328MXL_LCDC::PCR__TFT__Enable   << MC9328MXL_LCDC::PCR__TFT_shift  : 0;
        val |= config.EnableColor? MC9328MXL_LCDC::PCR__COLOR__Enable << MC9328MXL_LCDC::PCR__COLOR_shift: 0;

        switch(config.BusWidth)
        {
            case 1:
                val |= MC9328MXL_LCDC::PCR__PBSIZ__1_Bit << MC9328MXL_LCDC::PCR__PBSIZ_shift;
                break;
            case 2:
                val |= MC9328MXL_LCDC::PCR__PBSIZ__2_Bit << MC9328MXL_LCDC::PCR__PBSIZ_shift;
                break;
            case 4:
                val |= MC9328MXL_LCDC::PCR__PBSIZ__4_Bit << MC9328MXL_LCDC::PCR__PBSIZ_shift;
                break;
            case 8:
                val |= MC9328MXL_LCDC::PCR__PBSIZ__8_Bit << MC9328MXL_LCDC::PCR__PBSIZ_shift;
                break;
            case 16:
                val |= MC9328MXL_LCDC::PCR__PBSIZ__TFT_16_Bit << MC9328MXL_LCDC::PCR__PBSIZ_shift;
                break;
        }

        switch(config.BitsPerPixel)
        {
            case 1:
                val |= MC9328MXL_LCDC::PCR__BPIX__1bpp << MC9328MXL_LCDC::PCR__BPIX_shift;
                break;
            case 2:
                val |= MC9328MXL_LCDC::PCR__BPIX__2bpp << MC9328MXL_LCDC::PCR__BPIX_shift;
                break;
            case 4:
                val |= MC9328MXL_LCDC::PCR__BPIX__4bpp << MC9328MXL_LCDC::PCR__BPIX_shift;
                break;
            case 8:
                val |= MC9328MXL_LCDC::PCR__BPIX__8bpp << MC9328MXL_LCDC::PCR__BPIX_shift;
                break;
            case 12: // fall through
            case 16:
                val |= MC9328MXL_LCDC::PCR__BPIX__12bpp_16bpp << MC9328MXL_LCDC::PCR__BPIX_shift;
                break;
        }
        val |= (!config.PixelPolarity          ) ? MC9328MXL_LCDC::PCR__PIXPOL__Active_Low     << MC9328MXL_LCDC::PCR__PIXPOL_shift   : 0; // bit set indicates low polarity
        val |= (!config.FirstLineMarkerPolarity) ? MC9328MXL_LCDC::PCR__FLMPOL__Active_Low     << MC9328MXL_LCDC::PCR__FLMPOL_shift   : 0; // bit set indicates low polarity
        val |= (!config.LinePulsePolarity      ) ? MC9328MXL_LCDC::PCR__LPPOL__Active_Low      << MC9328MXL_LCDC::PCR__LPPOL_shift    : 0; // bit set indicates low polarity
        val |= (!config.ShiftClockPolarity     ) ? MC9328MXL_LCDC::PCR__CLKPOL__Active_Low     << MC9328MXL_LCDC::PCR__CLKPOL_shift   : 0; // bit set indicates low polarity
        val |= (!config.OutputEnablePolarity   ) ? MC9328MXL_LCDC::PCR__OEPOL__Active_Low      << MC9328MXL_LCDC::PCR__OEPOL_shift    : 0; // bit set indicates low polarity
        val |=   config.ClockIdleEnable          ? MC9328MXL_LCDC::PCR__SCLKIDLE__Enable_LSCLK << MC9328MXL_LCDC::PCR__SCLKIDLE_shift : 0;
        val |=   config.ClockSelectEnable        ? MC9328MXL_LCDC::PCR__SCLKSEL_Enable_LSCLK   << MC9328MXL_LCDC::PCR__SCLKSEL_shift  : 0;
        val |= (UINT32)config.PixelClockDivider << MC9328MXL_LCDC::PCR__PCD_shift;
        
        LCDC.PCR = val | Extras[GET_EXTRA_REG_INDEX(&LCDC.PCR)];
    }

    {
        UINT32 val = 0;

        val |= (UINT32)config.HorizontalSyncPulseWidth << MC9328MXL_LCDC::HCR__H_WIDTH_shift;
        val |= (UINT32)config.HorizontalSyncWait1      << MC9328MXL_LCDC::HCR__H_WAIT_1_shift;
        val |= (UINT32)config.HorizontalSyncWait2      << MC9328MXL_LCDC::HCR__H_WAIT_2_shift;

        LCDC.HCR = val;
    }

    {
        UINT32 val = 0;

        val |= (UINT32)config.VerticalSyncPulseWidth << MC9328MXL_LCDC::VCR__V_WIDTH_shift;
        val |= (UINT32)config.VerticalSyncWait1      << MC9328MXL_LCDC::VCR__V_WAIT_1_shift;
        val |= (UINT32)config.VerticalSyncWait2      << MC9328MXL_LCDC::VCR__V_WAIT_2_shift;

        LCDC.VCR = val;
    }

    LCDC.LSCR1  = Extras[GET_EXTRA_REG_INDEX(&LCDC.LSCR1)];
    LCDC.PWMR   = Extras[GET_EXTRA_REG_INDEX(&LCDC.PWMR)];
    LCDC.LCDICR = Extras[GET_EXTRA_REG_INDEX(&LCDC.LCDICR)];
    LCDC.LCDISR = Extras[GET_EXTRA_REG_INDEX(&LCDC.LCDISR)];

    // DMA control
    if(Extras[GET_EXTRA_REG_INDEX(&LCDC.DMACR)])
    {
        LCDC.DMACR = Extras[GET_EXTRA_REG_INDEX(&LCDC.DMACR)];
    }
    else
    {
        UINT32 val = 0;

        val |=                                     8 << MC9328MXL_LCDC::DMACR__TM_shift;
        val |=                                     3 << MC9328MXL_LCDC::DMACR__HM_shift;
        val |= MC9328MXL_LCDC::DMACR__BURST__DYNAMIC << MC9328MXL_LCDC::DMACR__BURST_shift;

        LCDC.DMACR = val;
    }

    return TRUE;
}

BOOL MC9328MXL_LCD_Driver::Uninitialize()
{
    NATIVE_PROFILE_HAL_PROCESSOR_LCD();
    HAL_Time_Sleep_MicroSeconds_InterruptEnabled( MC9328_LCD_TIME_POWER_STABLE );       // Wait for LCD charge pumps to shut down

    CPU_INTC_DeactivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_LCDC_INT );

    // Turn off LCDC
    MC9328MXL::LCDC().RMCR = MC9328MXL_LCDC::RMCR__LCDC_EN__DISABLE << MC9328MXL_LCDC::RMCR__LCDC_EN_shift;

    return TRUE;
}

BOOL MC9328MXL_LCD_Driver::Enable( BOOL fEnable )
{
    NATIVE_PROFILE_HAL_PROCESSOR_LCD();
    MC9328MXL_LCDC& LCDC = MC9328MXL::LCDC();

    if(fEnable)
    {
        // Turn on LCDC
        LCDC.RMCR = MC9328MXL_LCDC::RMCR__LCDC_EN__ENABLE << MC9328MXL_LCDC::RMCR__LCDC_EN_shift;        
    }
    else
    {
        LCDC.RMCR = 0;
    }

    return TRUE;
}

