////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _AT91_H_1
#define _AT91_H_1 1

#if defined(PLATFORM_ARM_SAM7X_ANY)
#include "AT91_SAM/AT91_SAM7X.h"
#endif

#if defined(PLATFORM_ARM_SAM7S_ANY)
#include "AT91_SAM/AT91_SAM7S.h"
#endif


#if defined(PLATFORM_ARM_SAM9261_ANY)
#include "AT91_SAM/AT91_SAM9261.h"
#endif

// THIS FILE DOESN'T EXIST
//#if defined(PLATFORM_ARM_SAM92RL_ANY)
//#include "AT91_SAM/AT91_SAM9RL.h"
//#endif

#if defined(PLATFORM_ARM_SAM9RL64_ANY)
#include "AT91_SAM/AT91_SAM9RL64.h"
#endif

#if defined(PLATFORM_ARM_SAM9RL64_ANY)
struct AT91_PMC {
    static const UINT32 c_Base = AT91C_BASE_PMC;

    /****/ volatile UINT32 PMC_SCER;                // System Clock Enable Register

    /****/ volatile UINT32 PMC_SCDR;                // System Clock Disable Register

    /****/ volatile UINT32 PMC_SCSR;                // System Clock Status Register

    /****/ volatile UINT32 Reserved0[1];            // 

    /****/ volatile UINT32 PMC_PCER;                // Peripheral Clock Enable Register

    /****/ volatile UINT32 PMC_PCDR;                // Peripheral Clock Disable Register

    /****/ volatile UINT32 PMC_PCSR;                // Peripheral Clock Status Register

    /****/ volatile UINT32 PMC_CKGR_UCKR;            // UTMI Clock Register

    /****/ volatile UINT32 PMC_CKGR_MOR;            // Main Oscillator Register
    static const    UINT32 CKGR_MOSCEN     = (0x1 << 0);       // (CKGR) Main Oscillator Enable
    static const    UINT32 CKGR_OSCBYPASS  = (0x1 << 1);       // (CKGR) Main Oscillator Bypass
    static const    UINT32 CKGR_OSCOUNT    = (0xFF << 8);      // (CKGR) Main Oscillator Start-up Time


    /****/ volatile UINT32 PMC_CKGR_MCFR;  // Main Clock  Frequency Register
    static const    UINT32 CKGR_MAINF      = (0xFFFF << 0);     // (CKGR) Main Clock Frequency
    static const    UINT32 CKGR_MAINRDY    = (0x1UL << 16);       // (CKGR) Main Clock Ready
    

    /****/ volatile UINT32 PMC_CKGR_PLLAR;  // PLL Register

       
    static const    UINT32 CKGR_DIV        = (0xFF << 0);     // (CKGR) Divider Selected
    static const    UINT32 CKGR_DIV_0      = (0x0);           // (CKGR) Divider output is 0
    static const    UINT32 CKGR_DIV_BYPASS = (0x1);           // (CKGR) Divider is bypassed
    static const    UINT32 CKGR_PLLCOUNT   = (0x3F << 8);     // (CKGR) PLL Counter
    static const    UINT32 CKGR_OUT        = (0x3 << 14);     // (CKGR) PLL Output Frequency Range
    static const    UINT32 CKGR_OUT_0      = (0x0 << 14);     // (CKGR) Please refer to the PLL datasheet
    static const    UINT32 CKGR_OUT_1      = (0x1 << 14);     // (CKGR) Please refer to the PLL datasheet
    static const    UINT32 CKGR_OUT_2      = (0x2 << 14);     // (CKGR) Please refer to the PLL datasheet
    static const    UINT32 CKGR_OUT_3      = (0x3 << 14);     // (CKGR) Please refer to the PLL datasheet
    static const    UINT32 CKGR_MUL        = (0x7FFUL << 16);   // (CKGR) PLL Multiplier
    static const    UINT32 CKGR_USBDIV     = (0x3UL << 28);     // (CKGR) Divider for USB Clocks
    static const    UINT32 CKGR_USBDIV_0   = (0x0UL << 28);     // (CKGR) Divider output is PLL clock output
    static const    UINT32 CKGR_USBDIV_1   = (0x1UL << 28);     // (CKGR) Divider output is PLL clock output divided by 2
    static const    UINT32 CKGR_USBDIV_2   = (0x2UL << 28);     // (CKGR) Divider output is PLL clock output divided by 4

    /****/ volatile UINT32 Reserved1[1];            // 
    
    /****/ volatile UINT32 PMC_MCKR;  // Master Clock Register
    static const    UINT32 PMC_CSS           = (0x3 <<  0);   //  (PMC) Programmable Clock Selection
    static const    UINT32 PMC_CSS_SLOW_CLK  = (0x0);         //  (PMC) Slow Clock is selected
    static const    UINT32 PMC_CSS_MAIN_CLK  = (0x1);         //  (PMC) Main Clock is selected
    static const    UINT32 PMC_CSS_PLLA_CLK  = (0x2);         //  (PMC) Clock from PLL1 is selected = (SAM9 only);
    static const    UINT32 PMC_CSS_PLL_CLK   = (0x3);         //  (PMC) Clock from PLL is selected
    static const    UINT32 PMC_PRES          = (0x7 <<  2);   //  (PMC) Programmable Clock Prescaler
    static const    UINT32 PMC_PRES_CLK      = (0x0 <<  2);   //  (PMC) Selected clock
    static const    UINT32 PMC_PRES_CLK_2    = (0x1 <<  2);   //  (PMC) Selected clock divided by 2
    static const    UINT32 PMC_PRES_CLK_4    = (0x2 <<  2);   //  (PMC) Selected clock divided by 4
    static const    UINT32 PMC_PRES_CLK_8    = (0x3 <<  2);   //  (PMC) Selected clock divided by 8
    static const    UINT32 PMC_PRES_CLK_16   = (0x4 <<  2);   //  (PMC) Selected clock divided by 16
    static const    UINT32 PMC_PRES_CLK_32   = (0x5 <<  2);   //  (PMC) Selected clock divided by 32
    static const    UINT32 PMC_PRES_CLK_64   = (0x6 <<  2);   //  (PMC) Selected clock divided by 64
    static const    UINT32 PMC_MDIV          = (0x3 <<  8);   //  (PMC) Master Clock Division
    static const    UINT32 PMC_MDIV_1        = (0x0 <<  8);   //  (PMC) The master clock and the processor clock are the same
    static const    UINT32 PMC_MDIV_2        = (0x1 <<  8);   //  (PMC) The processor clock is twice as fast as the master clock
    static const    UINT32 PMC_MDIV_3        = (0x2 <<  8);   //  (PMC) The processor clock is four times faster than the master clock

    /****/ volatile UINT32 Reserved3[3];  // 

    /****/ volatile UINT32 PMC_PCKR[2];   // Programmable Clock Register

    /****/ volatile UINT32 Reserved4[6];  // 

    /****/ volatile UINT32 PMC_IER;   // Interrupt Enable Register
    static const    UINT32 PMC_MOSCS     = (0x1 <<  0);       // (PMC) MOSC Status/Enable/Disable/Mask
    static const    UINT32 PMC_LOCKA     = (0x1 <<  1);       // (PMC) PLL Status/Enable/Disable/Mask =(SAM9 Only);
    static const    UINT32 PMC_LOCK      = (0x1 <<  2);       // (PMC) PLL Status/Enable/Disable/Mask
    static const    UINT32 PMC_MCKRDY    = (0x1 <<  3);       // (PMC) MCK_RDY Status/Enable/Disable/Mask
    static const    UINT32 PMC_PCK0RDY   = (0x1 <<  8);       // (PMC) PCK0_RDY Status/Enable/Disable/Mask
    static const    UINT32 PMC_PCK1RDY   = (0x1 <<  9);       // (PMC) PCK1_RDY Status/Enable/Disable/Mask
    static const    UINT32 PMC_PCK2RDY   = (0x1 << 10);       // (PMC) PCK2_RDY Status/Enable/Disable/Mask
    static const    UINT32 PMC_PCK3RDY   = (0x1 << 11);       // (PMC) PCK3_RDY Status/Enable/Disable/Mask
    static const    UINT32 PMC_LOCKU      = (0x1 << 6);           //(PMC) USB clock locked
    /****/ volatile UINT32 PMC_IDR;   // Interrupt Disable Register

    /****/ volatile UINT32 PMC_SR;    // Status Register

    /****/ volatile UINT32 PMC_IMR;   // Interrupt Mask Register

    /****/ volatile UINT32 Reserved5[4]; // Reserved


    static const    UINT32 PMC_PLLICPR__PLLA = (0x1 <<  0);
    static const    UINT32 PMC_PLLICPR__PLLB = (0x1 << 16);
    
    __inline void EnableSystemClock(UINT32 clkIds)
    {
        PMC_SCER = clkIds;
    }

    __inline void DisableSystemClock(UINT32 clkIds)
    {
        PMC_SCDR = clkIds;
    }

    __inline void EnablePeriphClock(UINT32 periphIds)
    {
        PMC_PCER = (1 << periphIds);
    }

    __inline void DisablePeriphClock(UINT32 periphIds)
    {
        PMC_PCDR = (1 << periphIds);
    }
 

};

#else
struct AT91_PMC {
    static const UINT32 c_Base = AT91C_BASE_PMC;

    /****/ volatile UINT32 PMC_SCER;                // System Clock Enable Register

    /****/ volatile UINT32 PMC_SCDR;                // System Clock Disable Register

    /****/ volatile UINT32 PMC_SCSR;                // System Clock Status Register

    /****/ volatile UINT32 Reserved0[1];            // 

    /****/ volatile UINT32 PMC_PCER;                // Peripheral Clock Enable Register

    /****/ volatile UINT32 PMC_PCDR;                // Peripheral Clock Disable Register

    /****/ volatile UINT32 PMC_PCSR;                // Peripheral Clock Status Register

    /****/ volatile UINT32 Reserved1[1];            // 

    /****/ volatile UINT32 PMC_CKGR_MOR;            // Main Oscillator Register
    static const    UINT32 CKGR_MOSCEN     = (0x1 << 0);       // (CKGR) Main Oscillator Enable
    static const    UINT32 CKGR_OSCBYPASS  = (0x1 << 1);       // (CKGR) Main Oscillator Bypass
    static const    UINT32 CKGR_OSCOUNT    = (0xFF << 8);      // (CKGR) Main Oscillator Start-up Time


    /****/ volatile UINT32 PMC_CKGR_MCFR;  // Main Clock  Frequency Register
    static const    UINT32 CKGR_MAINF      = (0xFFFF << 0);     // (CKGR) Main Clock Frequency
    static const    UINT32 CKGR_MAINRDY    = (0x1 << 16);       // (CKGR) Main Clock Ready
    

    /****/ volatile UINT32 PMC_CKGR_PLLAR;  // PLL Register

    // -------- CKGR_PLLR : (CKGR Offset: 0xc) PLL B Register -------- 
    /****/ volatile UINT32 PMC_CKGR_PLLR;   // PLL Register (SAM7) / PLLB (SAM9)
    static const    UINT32 CKGR_DIV        = (0xFF << 0);     // (CKGR) Divider Selected
    static const    UINT32 CKGR_DIV_0      = (0x0);           // (CKGR) Divider output is 0
    static const    UINT32 CKGR_DIV_BYPASS = (0x1);           // (CKGR) Divider is bypassed
    static const    UINT32 CKGR_PLLCOUNT   = (0x3F << 8);     // (CKGR) PLL Counter
    static const    UINT32 CKGR_OUT        = (0x3UL << 14);     // (CKGR) PLL Output Frequency Range
    static const    UINT32 CKGR_OUT_0      = (0x0UL << 14);     // (CKGR) Please refer to the PLL datasheet
    static const    UINT32 CKGR_OUT_1      = (0x1UL << 14);     // (CKGR) Please refer to the PLL datasheet
    static const    UINT32 CKGR_OUT_2      = (0x2UL << 14);     // (CKGR) Please refer to the PLL datasheet
    static const    UINT32 CKGR_OUT_3      = (0x3UL << 14);     // (CKGR) Please refer to the PLL datasheet
    static const    UINT32 CKGR_MUL        = (0x7FFUL << 16);   // (CKGR) PLL Multiplier
    static const    UINT32 CKGR_USBDIV     = (0x3UL << 28);     // (CKGR) Divider for USB Clocks
    static const    UINT32 CKGR_USBDIV_0   = (0x0UL << 28);     // (CKGR) Divider output is PLL clock output
    static const    UINT32 CKGR_USBDIV_1   = (0x1UL << 28);     // (CKGR) Divider output is PLL clock output divided by 2
    static const    UINT32 CKGR_USBDIV_2   = (0x2UL << 28);     // (CKGR) Divider output is PLL clock output divided by 4

    /****/ volatile UINT32 PMC_MCKR;  // Master Clock Register
    static const    UINT32 PMC_CSS           = (0x3 <<  0);   //  (PMC) Programmable Clock Selection
    static const    UINT32 PMC_CSS_SLOW_CLK  = (0x0);         //  (PMC) Slow Clock is selected
    static const    UINT32 PMC_CSS_MAIN_CLK  = (0x1);         //  (PMC) Main Clock is selected
    static const    UINT32 PMC_CSS_PLLA_CLK  = (0x2);         //  (PMC) Clock from PLL1 is selected = (SAM9 only);
    static const    UINT32 PMC_CSS_PLL_CLK   = (0x3);         //  (PMC) Clock from PLL is selected
    static const    UINT32 PMC_PRES          = (0x7 <<  2);   //  (PMC) Programmable Clock Prescaler
    static const    UINT32 PMC_PRES_CLK      = (0x0 <<  2);   //  (PMC) Selected clock
    static const    UINT32 PMC_PRES_CLK_2    = (0x1 <<  2);   //  (PMC) Selected clock divided by 2
    static const    UINT32 PMC_PRES_CLK_4    = (0x2 <<  2);   //  (PMC) Selected clock divided by 4
    static const    UINT32 PMC_PRES_CLK_8    = (0x3 <<  2);   //  (PMC) Selected clock divided by 8
    static const    UINT32 PMC_PRES_CLK_16   = (0x4 <<  2);   //  (PMC) Selected clock divided by 16
    static const    UINT32 PMC_PRES_CLK_32   = (0x5 <<  2);   //  (PMC) Selected clock divided by 32
    static const    UINT32 PMC_PRES_CLK_64   = (0x6 <<  2);   //  (PMC) Selected clock divided by 64
    static const    UINT32 PMC_MDIV          = (0x3 <<  8);   //  (PMC) Master Clock Division
    static const    UINT32 PMC_MDIV_1        = (0x0 <<  8);   //  (PMC) The master clock and the processor clock are the same
    static const    UINT32 PMC_MDIV_2        = (0x1 <<  8);   //  (PMC) The processor clock is twice as fast as the master clock
    static const    UINT32 PMC_MDIV_3        = (0x2 <<  8);   //  (PMC) The processor clock is four times faster than the master clock

    /****/ volatile UINT32 Reserved3[3];  // 

    /****/ volatile UINT32 PMC_PCKR[4];   // Programmable Clock Register

    /****/ volatile UINT32 Reserved4[4];  // 

    /****/ volatile UINT32 PMC_IER;   // Interrupt Enable Register
    static const    UINT32 PMC_MOSCS     = (0x1 <<  0);       // (PMC) MOSC Status/Enable/Disable/Mask
    static const    UINT32 PMC_LOCKA     = (0x1 <<  1);       // (PMC) PLL Status/Enable/Disable/Mask =(SAM9 Only);
    static const    UINT32 PMC_LOCK      = (0x1 <<  2);       // (PMC) PLL Status/Enable/Disable/Mask
    static const    UINT32 PMC_MCKRDY    = (0x1 <<  3);       // (PMC) MCK_RDY Status/Enable/Disable/Mask
    static const    UINT32 PMC_PCK0RDY   = (0x1 <<  8);       // (PMC) PCK0_RDY Status/Enable/Disable/Mask
    static const    UINT32 PMC_PCK1RDY   = (0x1 <<  9);       // (PMC) PCK1_RDY Status/Enable/Disable/Mask
    static const    UINT32 PMC_PCK2RDY   = (0x1 << 10);       // (PMC) PCK2_RDY Status/Enable/Disable/Mask
    static const    UINT32 PMC_PCK3RDY   = (0x1 << 11);       // (PMC) PCK3_RDY Status/Enable/Disable/Mask
    static const    UINT32 PMC_LOCKU      = (0x1 << 6);           //(PMC) USB clock locked
    /****/ volatile UINT32 PMC_IDR;   // Interrupt Disable Register

    /****/ volatile UINT32 PMC_SR;    // Status Register

    /****/ volatile UINT32 PMC_IMR;   // Interrupt Mask Register

    /****/ volatile UINT32 Reserved5[3]; // Reserved

    /****/ volatile UINT32 PMC_PLLICPR; // Charge Pump Current Register
    static const    UINT32 PMC_PLLICPR__PLLA = (0x1 <<  0);
    static const    UINT32 PMC_PLLICPR__PLLB = (0x1 << 16);
    
    __inline void EnableSystemClock(UINT32 clkIds)
    {
        PMC_SCER = clkIds;
    }

    __inline void DisableSystemClock(UINT32 clkIds)
    {
        PMC_SCDR = clkIds;
    }

    __inline void EnablePeriphClock(UINT32 periphIds)
    {
        PMC_PCER = (1 << periphIds);
    }

    __inline void DisablePeriphClock(UINT32 periphIds)
    {
        PMC_PCDR = (1 << periphIds);
    }
 

};
#endif


__inline void AT91_RSTC_EXTRST()
{
    volatile UINT32 *pReset       = (volatile UINT32*)AT91C_BASE_RSTC;
    volatile UINT32 *pResetStatus = (volatile UINT32*)AT91C_BASE_RSTC_SR;
    volatile UINT32 *pResetMode   = (volatile UINT32*)AT91C_BASE_RSTC_MR;

    *pResetMode = (AT91C_RSTC__RESET_KEY | 4ul << 8);
    *pReset     = (AT91C_RSTC__RESET_KEY | AT91C_RTSC__EXTRST);

    //Wait for hardware reset end
    while(((*pResetStatus) & AT91C_RTSC_SR__SRCMP)) {;}   
}

//////////////////////////////////////////////////////////////////////////////
// AT91_AIC
//
struct AT91_AIC
{
    static const UINT32 c_Base = AT91C_BASE_AIC;

    /****/ volatile UINT32 AIC_SMR[32];    // Source Mode Register
    static const    UINT32 AIC_PRIOR       = (0x7 <<  0); // (AIC) Priority Level
    static const    UINT32 AIC_PRIOR_LOWEST               = (0x0); // (AIC) Lowest priority level
    static const    UINT32 AIC_PRIOR_HIGHEST              = (0x7); // (AIC) Highest priority level
    static const    UINT32 AIC_SRCTYPE     = (0x3 <<  5); // (AIC) Interrupt Source Type
    static const    UINT32 AIC_SRCTYPE_INT_HIGH_LEVEL       = (0x0 <<  5); // (AIC) Internal Sources Code Label High-level Sensitive
    static const    UINT32 AIC_SRCTYPE_EXT_LOW_LEVEL        = (0x0 <<  5); // (AIC) External Sources Code Label Low-level Sensitive
    static const    UINT32 AIC_SRCTYPE_INT_POSITIVE_EDGE    = (0x1 <<  5); // (AIC) Internal Sources Code Label Positive Edge triggered
    static const    UINT32 AIC_SRCTYPE_EXT_NEGATIVE_EDGE    = (0x1 <<  5); // (AIC) External Sources Code Label Negative Edge triggered
    static const    UINT32 AIC_SRCTYPE_HIGH_LEVEL           = (0x2 <<  5); // (AIC) Internal Or External Sources Code Label High-level Sensitive
    static const    UINT32 AIC_SRCTYPE_POSITIVE_EDGE        = (0x3 <<  5); // (AIC) Internal Or External Sources Code Label Positive Edge triggered



    /****/ volatile UINT32 AIC_SVR[32];    // Source Vector Register

    /****/ volatile UINT32 AIC_IVR;        // IRQ Vector Register

    /****/ volatile UINT32 AIC_FVR;        // FIQ Vector Register

    /****/ volatile UINT32 AIC_ISR;        // Interrupt Status Register

    /****/ volatile UINT32 AIC_IPR;        // Interrupt Pending Register

    /****/ volatile UINT32 AIC_IMR;        // Interrupt Mask Register

    /****/ volatile UINT32 AIC_CISR;       // Core Interrupt Status Register
    static const    UINT32 AIC_NFIQ        = (0x1 <<  0); // (AIC) NFIQ Status
    static const    UINT32 AIC_NIRQ        = (0x1 <<  1); // (AIC) NIRQ Status

    /****/ volatile UINT32 Reserved6[2];   

    /****/ volatile UINT32 AIC_IECR;       // Interrupt Enable Command Register

    /****/ volatile UINT32 AIC_IDCR;       // Interrupt Disable Command Register
    static const    UINT32 AIC_IDCR_DIABLE_ALL  = 0xFFFFFFFF; // disable all

    /****/ volatile UINT32 AIC_ICCR;       // Interrupt Clear Command Register
    static const    UINT32 AIC_ICCR_CLEAR_ALL   = 0xFFFFFFFF; // clear all

    /****/ volatile UINT32 AIC_ISCR;       // Interrupt Set Command Register

    /****/ volatile UINT32 AIC_EOICR;      // End of Interrupt Command Register

    /****/ volatile UINT32 AIC_SPU;        // Spurious Vector Register

    /****/ volatile UINT32 AIC_DCR;        // Debug Control Register (Protect)
    static const    UINT32 AIC_DCR_PROT    = (0x1 <<  0); // (AIC) Protection Mode
    static const    UINT32 AIC_DCR_GMSK    = (0x1 <<  1); // (AIC) General Mask

    /****/ volatile UINT32 Reserved7[1];   

    /****/ volatile UINT32 AIC_FFER;       // Fast Forcing Enable Register

    /****/ volatile UINT32 AIC_FFDR;       // Fast Forcing Disable Register

    /****/ volatile UINT32 AIC_FFSR;       // Fast Forcing Status Register


    BOOL IsInterruptPending()
    {
        if(AIC_IPR & AIC_IMR )
        {
            return TRUE;
        }

        return FALSE;
    }
}; 

//
// AT91_AIC
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// AT91 Timer Channel
//
struct AT91_TC {

    static const UINT32 c_Base = AT91C_BASE_TC0;

    /****/ volatile UINT32 TC_CCR;    // Channel Control Register
    static const    UINT32 TC_CLKEN        = (0x1 <<  0);// (TC) Counter Clock Enable Command
    static const    UINT32 TC_CLKDIS       = (0x1 <<  1);// (TC)Counter Clock Disable Command
    static const    UINT32 TC_SWTRG        = (0x1 <<  2);// (TC)Software Trigger Command

    /****/ volatile UINT32 TC_CMR;    // Channel Mode Register (Capture Mode / Waveform Mode)
    static const    UINT32 TC_CLKS         = (0x7 <<  0);// (TC)Clock Selection
    static const    UINT32     TC_CLKS_TIMER_DIV1_CLOCK     = (0x0);// (TC)Clock selected: TIMER_DIV1_CLOCK
    static const    UINT32     TC_CLKS_TIMER_DIV2_CLOCK     = (0x1);// (TC)Clock selected: TIMER_DIV2_CLOCK
    static const    UINT32     TC_CLKS_TIMER_DIV3_CLOCK     = (0x2);// (TC)Clock selected: TIMER_DIV3_CLOCK
    static const    UINT32     TC_CLKS_TIMER_DIV4_CLOCK     = (0x3);// (TC)Clock selected: TIMER_DIV4_CLOCK
    static const    UINT32     TC_CLKS_TIMER_DIV5_CLOCK     = (0x4);// (TC)Clock selected: TIMER_DIV5_CLOCK
    static const    UINT32     TC_CLKS_XC0                  = (0x5);// (TC)Clock selected: XC0
    static const    UINT32     TC_CLKS_XC1                  = (0x6);// (TC)Clock selected: XC1
    static const    UINT32     TC_CLKS_XC2                  = (0x7);// (TC)Clock selected: XC2
    static const    UINT32 TC_CLKI         = (0x1 <<  3);// (TC)Clock Invert
    static const    UINT32 TC_BURST        = (0x3 <<  4);// (TC)Burst Signal Selection
    static const    UINT32     TC_BURST_NONE                 = (0x0 <<  4);// (TC)The clock is not gated by an external signal
    static const    UINT32     TC_BURST_XC0                  = (0x1 <<  4);// (TC)XC0 is ANDed with the selected clock
    static const    UINT32     TC_BURST_XC1                  = (0x2 <<  4);// (TC)XC1 is ANDed with the selected clock
    static const    UINT32     TC_BURST_XC2                  = (0x3 <<  4);// (TC)XC2 is ANDed with the selected clock
    static const    UINT32 TC_CPCSTOP      = (0x1 <<  6);// (TC)Counter Clock Stopped with RC Compare
    static const    UINT32 TC_LDBSTOP      = (0x1 <<  6);// (TC)Counter Clock Stopped with RB Loading
    static const    UINT32 TC_CPCDIS       = (0x1 <<  7);// (TC)Counter Clock Disable with RC Compare
    static const    UINT32 TC_LDBDIS       = (0x1 <<  7);// (TC)Counter Clock Disabled with RB Loading
    static const    UINT32 TC_ETRGEDG      = (0x3 <<  8);// (TC)External Trigger Edge Selection
    static const    UINT32     TC_ETRGEDG_NONE                 = (0x0 <<  8);// (TC)Edge: None
    static const    UINT32     TC_ETRGEDG_RISING               = (0x1 <<  8);// (TC)Edge: rising edge
    static const    UINT32     TC_ETRGEDG_FALLING              = (0x2 <<  8);// (TC)Edge: falling edge
    static const    UINT32     TC_ETRGEDG_BOTH                 = (0x3 <<  8);// (TC)Edge: each edge
    static const    UINT32 TC_EEVTEDG      = (0x3 <<  8);// (TC)External Event Edge Selection
    static const    UINT32     TC_EEVTEDG_NONE                 = (0x0 <<  8);// (TC)Edge: None
    static const    UINT32     TC_EEVTEDG_RISING               = (0x1 <<  8);// (TC)Edge: rising edge
    static const    UINT32     TC_EEVTEDG_FALLING              = (0x2 <<  8);// (TC)Edge: falling edge
    static const    UINT32     TC_EEVTEDG_BOTH                 = (0x3 <<  8);// (TC)Edge: each edge
    static const    UINT32 TC_EEVT         = (0x3 << 10);// (TC)External Event  Selection
    static const    UINT32     TC_EEVT_TIOB                 = (0x0 << 10);// (TC)Signal selected as external event: TIOB TIOB direction: input
    static const    UINT32     TC_EEVT_XC0                  = (0x1 << 10);// (TC)Signal selected as external event: XC0 TIOB direction: output
    static const    UINT32     TC_EEVT_XC1                  = (0x2 << 10);// (TC)Signal selected as external event: XC1 TIOB direction: output
    static const    UINT32     TC_EEVT_XC2                  = (0x3 << 10);// (TC)Signal selected as external event: XC2 TIOB direction: output
    static const    UINT32 TC_ABETRG       = (0x1 << 10);// (TC)TIOA or TIOB External Trigger Selection
    static const    UINT32 TC_ENETRG       = (0x1 << 12);// (TC)External Event Trigger enable
    static const    UINT32 TC_WAVESEL      = (0x3 << 13);// (TC)Waveform  Selection
    static const    UINT32     TC_WAVESEL_UP                   = (0x0 << 13);// (TC)UP mode without atomatic trigger on RC Compare
    static const    UINT32     TC_WAVESEL_UPDOWN               = (0x1 << 13);// (TC)UPDOWN mode without automatic trigger on RC Compare
    static const    UINT32     TC_WAVESEL_UP_AUTO              = (0x2 << 13);// (TC)UP mode with automatic trigger on RC Compare
    static const    UINT32     TC_WAVESEL_UPDOWN_AUTO          = (0x3 << 13);// (TC)UPDOWN mode with automatic trigger on RC Compare
    static const    UINT32 TC_CPCTRG       = (0x1UL << 14);// (TC)RC Compare Trigger Enable
    static const    UINT32 TC_WAVE         = (0x1UL << 15);// (TC)
    static const    UINT32 TC_ACPA         = (0x3UL << 16);// (TC)RA Compare Effect on TIOA
    static const    UINT32     TC_ACPA_NONE                 = (0x0UL << 16);// (TC)Effect: none
    static const    UINT32     TC_ACPA_SET                  = (0x1UL << 16);// (TC)Effect: set
    static const    UINT32     TC_ACPA_CLEAR                = (0x2UL << 16);// (TC)Effect: clear
    static const    UINT32     TC_ACPA_TOGGLE               = (0x3UL << 16);// (TC)Effect: toggle
    static const    UINT32 TC_LDRA         = (0x3UL << 16);// (TC)RA Loading Selection
    static const    UINT32     TC_LDRA_NONE                 = (0x0UL << 16);// (TC)Edge: None
    static const    UINT32     TC_LDRA_RISING               = (0x1UL << 16);// (TC)Edge: rising edge of TIOA
    static const    UINT32     TC_LDRA_FALLING              = (0x2UL << 16);// (TC)Edge: falling edge of TIOA
    static const    UINT32     TC_LDRA_BOTH                 = (0x3UL << 16);// (TC)Edge: each edge of TIOA
    static const    UINT32 TC_ACPC         = (0x3UL << 18);// (TC)RC Compare Effect on TIOA
    static const    UINT32     TC_ACPC_NONE                 = (0x0UL << 18);// (TC)Effect: none
    static const    UINT32     TC_ACPC_SET                  = (0x1UL << 18);// (TC)Effect: set
    static const    UINT32     TC_ACPC_CLEAR                = (0x2UL << 18);// (TC)Effect: clear
    static const    UINT32     TC_ACPC_TOGGLE               = (0x3UL << 18);// (TC)Effect: toggle
    static const    UINT32 TC_LDRB         = (0x3UL << 18);// (TC)RB Loading Selection
    static const    UINT32     TC_LDRB_NONE                 = (0x0UL << 18);// (TC)Edge: None
    static const    UINT32     TC_LDRB_RISING               = (0x1UL << 18);// (TC)Edge: rising edge of TIOA
    static const    UINT32     TC_LDRB_FALLING              = (0x2UL << 18);// (TC)Edge: falling edge of TIOA
    static const    UINT32     TC_LDRB_BOTH                 = (0x3UL << 18);// (TC)Edge: each edge of TIOA
    static const    UINT32 TC_AEEVT        = (0x3UL << 20);// (TC)External Event Effect on TIOA
    static const    UINT32     TC_AEEVT_NONE                 = (0x0UL << 20);// (TC)Effect: none
    static const    UINT32     TC_AEEVT_SET                  = (0x1UL << 20);// (TC)Effect: set
    static const    UINT32     TC_AEEVT_CLEAR                = (0x2UL << 20);// (TC)Effect: clear
    static const    UINT32     TC_AEEVT_TOGGLE               = (0x3UL << 20);// (TC)Effect: toggle
    static const    UINT32 TC_ASWTRG       = (0x3UL << 22);// (TC)Software Trigger Effect on TIOA
    static const    UINT32     TC_ASWTRG_NONE                 = (0x0UL << 22);// (TC)Effect: none
    static const    UINT32     TC_ASWTRG_SET                  = (0x1UL << 22);// (TC)Effect: set
    static const    UINT32     TC_ASWTRG_CLEAR                = (0x2UL << 22);// (TC)Effect: clear
    static const    UINT32     TC_ASWTRG_TOGGLE               = (0x3UL << 22);// (TC)Effect: toggle
    static const    UINT32 TC_BCPB         = (0x3UL << 24);// (TC)RB Compare Effect on TIOB
    static const    UINT32     TC_BCPB_NONE                 = (0x0UL << 24);// (TC)Effect: none
    static const    UINT32     TC_BCPB_SET                  = (0x1UL << 24);// (TC)Effect: set
    static const    UINT32     TC_BCPB_CLEAR                = (0x2UL << 24);// (TC)Effect: clear
    static const    UINT32     TC_BCPB_TOGGLE               = (0x3UL << 24);// (TC)Effect: toggle
    static const    UINT32 TC_BCPC         = (0x3UL << 26);// (TC)RC Compare Effect on TIOB
    static const    UINT32     TC_BCPC_NONE                 = (0x0UL << 26);// (TC)Effect: none
    static const    UINT32     TC_BCPC_SET                  = (0x1UL << 26);// (TC)Effect: set
    static const    UINT32     TC_BCPC_CLEAR                = (0x2UL << 26);// (TC)Effect: clear
    static const    UINT32     TC_BCPC_TOGGLE               = (0x3UL << 26);// (TC)Effect: toggle
    static const    UINT32 TC_BEEVT        = (0x3UL << 28);// (TC)External Event Effect on TIOB
    static const    UINT32     TC_BEEVT_NONE                 = (0x0UL << 28);// (TC)Effect: none
    static const    UINT32     TC_BEEVT_SET                  = (0x1UL << 28);// (TC)Effect: set
    static const    UINT32     TC_BEEVT_CLEAR                = (0x2UL << 28);// (TC)Effect: clear
    static const    UINT32     TC_BEEVT_TOGGLE               = (0x3UL << 28);// (TC)Effect: toggle
    static const    UINT32 TC_BSWTRG       = ((UINT32)0x3UL << 30);// (TC)Software Trigger Effect on TIOB
    static const    UINT32     TC_BSWTRG_NONE                 = ((UINT32)0x0UL << 30);// (TC)Effect: none
    static const    UINT32     TC_BSWTRG_SET                  = ((UINT32)0x1UL << 30);// (TC)Effect: set
    static const    UINT32     TC_BSWTRG_CLEAR                = ((UINT32)0x2UL << 30);// (TC)Effect: clear
    static const    UINT32     TC_BSWTRG_TOGGLE               = ((UINT32)0x3UL << 30);// (TC)Effect: toggle
    
    /****/ volatile UINT32 Reserved0[2];  

    /****/ volatile UINT32 TC_CV;     // Counter Value

    /****/ volatile UINT32 TC_RA;     // Register A

    /****/ volatile UINT32 TC_RB;     // Register B

    /****/ volatile UINT32 TC_RC;     // Register C

    /****/ volatile UINT32 TC_SR;     // Status Register
    static const    UINT32 TC_COVFS        = (0x1 <<  0);// (TC)Counter Overflow
    static const    UINT32 TC_LOVRS        = (0x1 <<  1);// (TC)Load Overrun
    static const    UINT32 TC_CPAS         = (0x1 <<  2);// (TC)RA Compare
    static const    UINT32 TC_CPBS         = (0x1 <<  3);// (TC)RB Compare
    static const    UINT32 TC_CPCS         = (0x1 <<  4);// (TC)RC Compare
    static const    UINT32 TC_LDRAS        = (0x1 <<  5);// (TC)RA Loading
    static const    UINT32 TC_LDRBS        = (0x1 <<  6);// (TC)RB Loading
    static const    UINT32 TC_ETRGS        = (0x1 <<  7);// (TC)External Trigger
    static const    UINT32 TC_CLKSTA       = (0x1UL << 16);// (TC)Clock Enabling
    static const    UINT32 TC_MTIOA        = (0x1UL << 17);// (TC)TIOA Mirror
    static const    UINT32 TC_MTIOB        = (0x1UL << 18);// (TC)TIOA Mirror

    /****/ volatile UINT32 TC_IER;    // Interrupt Enable Register

    /****/ volatile UINT32 TC_IDR;    // Interrupt Disable Register

    /****/ volatile UINT32 TC_IMR;    // Interrupt Mask Register
};

//
// AT91 Timer Channel
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// WATCHDOG
//
struct AT91_WATCHDOG
{
    static const UINT32 c_Base = 0xFFFFFD40;

    //--//

    /****/ volatile UINT32 WDT_CR;
    static const    UINT32 WCR__KEY_mask = 0xFF000000;
    static const    UINT32 WCR__KEY      = 0xA5000000;
    static const    UINT32 WCR__RESTART  = 0x00000001;

    /****/ volatile UINT32 WDT_MR;
    static const    UINT32 WMR_WDV_mask  = 0x00000FFF;
    static const    UINT32 WMR_WDFIEN    = 0x00001000;
    static const    UINT32 WMR_WDRSTEN   = 0x00002000;

    static const    UINT32 WMR_WDRPROC   = 0x00004000;
    static const    UINT32 WMR_WDDIS     = 0x00008000;

    static const    UINT32 WMR_WDD_mask  = 0x0FFF0000;
    static const    UINT32 WMR_WDDBGHLT  = 0x10000000;
    static const    UINT32 WMR_WDIDLEHLT = 0x20000000;


    /****/ volatile UINT32 WDT_SR;
    static const    UINT32 WSR__WDUNF    = 0x00000001;
    static const    UINT32 WSR__WDERR    = 0x00000002;
};
//
// WATCHDOG
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// AT91_GPIO
//
struct AT91_PIO {


     static const UINT32 c_Base = AT91C_BASE_PIOA;

     static const UINT32 c_Base_Offset = 0x200;
        

    /****/ volatile UINT32 PIO_PER;   // PIO Enable Register

    /****/ volatile UINT32 PIO_PDR;   // PIO Disable Register

    /****/ volatile UINT32 PIO_PSR;   // PIO Status Register

    /****/ volatile UINT32 Reserved0[1];  // 

    /****/ volatile UINT32 PIO_OER;   // Output Enable Register

    /****/ volatile UINT32 PIO_ODR;   // Output Disable Registerr

    /****/ volatile UINT32 PIO_OSR;   // Output Status Register

    /****/ volatile UINT32 Reserved1[1];  // 

    /****/ volatile UINT32 PIO_IFER;  // Input Filter Enable Register

    /****/ volatile UINT32 PIO_IFDR;  // Input Filter Disable Register

    /****/ volatile UINT32 PIO_IFSR;  // Input Filter Status Register

    /****/ volatile UINT32 Reserved2[1];  // 

    /****/ volatile UINT32 PIO_SODR;  // Set Output Data Register

    /****/ volatile UINT32 PIO_CODR;  // Clear Output Data Register

    /****/ volatile UINT32 PIO_ODSR;  // Output Data Status Register

    /****/ volatile UINT32 PIO_PDSR;  // Pin Data Status Register

    /****/ volatile UINT32 PIO_IER;   // Interrupt Enable Register

    /****/ volatile UINT32 PIO_IDR;   // Interrupt Disable Register

    /****/ volatile UINT32 PIO_IMR;   // Interrupt Mask Register

    /****/ volatile UINT32 PIO_ISR;   // Interrupt Status Register

    /****/ volatile UINT32 PIO_MDER;  // Multi-driver Enable Register

    /****/ volatile UINT32 PIO_MDDR;  // Multi-driver Disable Register

    /****/ volatile UINT32 PIO_MDSR;  // Multi-driver Status Register

    /****/ volatile UINT32 Reserved3[1];  // 

    /****/ volatile UINT32 PIO_PPUDR;     // Pull-up Disable Register

    /****/ volatile UINT32 PIO_PPUER;     // Pull-up Enable Register

    /****/ volatile UINT32 PIO_PPUSR;     // Pull-up Status Register

    /****/ volatile UINT32 Reserved4[1];  // 

    /****/ volatile UINT32 PIO_ASR;   // Select A Register

    /****/ volatile UINT32 PIO_BSR;   // Select B Register

    /****/ volatile UINT32 PIO_ABSR;  // AB Select Status Register

    /****/ volatile UINT32 Reserved5[9];  // 

    /****/ volatile UINT32 PIO_OWER;  // Output Write Enable Register

    /****/ volatile UINT32 PIO_OWDR;  // Output Write Disable Register

    /****/ volatile UINT32 PIO_OWSR;  // Output Write Status Register
};

// --//

//////////////////////////////////////////////////////////////////////////////
// AT91_SPI
//

struct AT91_SPI
{
    static const UINT32 c_Base_1 = AT91C_BASE_SPI0;
    static const UINT32 c_Base_2 = AT91C_BASE_SPI1;


    static const UINT32 c_MAX_SPI =AT91C_MAX_SPI;
    
    /****/ volatile UINT32 SPI_CR;         // Control Register
    static const    UINT32 SPI_CR_ENABLE_SPI        = (0x1 <<  0); // enable SPI
    static const    UINT32 SPI_CR_DISABLE_SPI       = (0x1 <<  1); // disable SPI
    static const    UINT32 SPI_CR_SW_RESET          = (0x1 <<  7); // Software reset SPI

    /****/ volatile UINT32 SPI_MR;         // Mode Register
    static const    UINT32 SPI_MR_MSTR              = (0x1 <<  0); // 1--Master mode\0--Slave mode
    static const    UINT32 SPI_MR_PS                = (0x1 <<  1); // peripheral select 1--varable\0--fixed
    static const    UINT32 SPI_MR_MODFDIS           = (0x1 <<  4); // peripheral select 1--varable\0--fixed
    static const    UINT32 SPI_MR_CS0               = (0x0UL <<  16);
    static const    UINT32 SPI_MR_CS1               = (0x1UL <<  16);
    static const    UINT32 SPI_MR_CS2               = (0x3UL <<  16);
    static const    UINT32 SPI_MR_CS3               = (0x7UL <<  16);

    /****/ volatile UINT32 SPI_RDR;        // Receive Data Register

    /****/ volatile UINT32 SPI_TDR;        // Transmit Data Register

    /****/ volatile UINT32 SPI_SR;         // Status Register
    static const    UINT32 SPI_SR_RDRF              = (0x1 <<  0); // RDR full
    static const    UINT32 SPI_SR_TDRE              = (0x1 <<  1); // TDR empty
    static const    UINT32 SPI_SR_RXBUFF            = (0x1 <<  6); // receive buffer full
    static const    UINT32 SPI_SR_TXBUFE            = (0x1 <<  7); // transmit buffer empty
    static const    UINT32 SPI_SR_NSSR              = (0x1 <<  8); // Slave mode control
    static const    UINT32 SPI_SR_TXEMPTY           = (0x1 <<  9); // transmit register and internal shifters are empty
    static const    UINT32 SPI_SR_SPIENS            = (0x1UL <<  16); // SPI enable status

    /****/ volatile UINT32 SPI_IER;        // Interrupt Enable Register
    /****/ volatile UINT32 SPI_IDR;        // Interrupt Disable Register
    /****/ volatile UINT32 SPI_IMR;        // Interrupt MaskRegister
    /****/ volatile UINT32 Reserved1[4];   

    /****/ volatile UINT32 SPI_CSR0;       // Chip Select Register 0
    static const    UINT32 SPI_CSR_CPOL             = (0x1 <<  0); // clock polarity
    static const    UINT32 SPI_CSR_NCPHA            = (0x1 <<  1); // clock phase
    static const    UINT32 SPI_CSR_BITS_MASK        = (0xF << 4); // bits per transfer
    static const    UINT32 SPI_CSR_8BITS            = (0x0 <<  4);
    static const    UINT32 SPI_CSR_16BITS           = (0x8 <<  4);
    static const    UINT32 SPI_CSR_SCBR_MASK        = (0xFFUL << 8); // serial clock baud rate
    static const    UINT32 SPI_CSR_SCBR_SHIFT       = (0x8);     // serial clock baud rate
    static const    UINT32 SPI_CSR_DLYBS_MASK       = (0xFFUL << 16); // delay before SPCK
    static const    UINT32 SPI_CSR_DLYBS_SHIFT      = (16);     // delay before SPCK
    static const    UINT32 SPI_CSR_DLYBCT_MASK      = (0xFFUL << 24); // delay between transfer
    static const    UINT32 SPI_CSR_DLYBCT_SHIFT     = (24);     // delay between transfer

    /****/ volatile UINT32 SPI_CSR1;       // Chip Select Register 1
    /****/ volatile UINT32 SPI_CSR2;       // Chip Select Register 2
    /****/ volatile UINT32 SPI_CSR3;       // Chip Select Register 3

    __inline static UINT32 ConvertClockRateToDivisor(UINT32 Clock_RateKHz)
    {
        #define SYSTEM_CLOCK_KHZ  (SYSTEM_PERIPHERAL_CLOCK_HZ/1000)
        
        return SYSTEM_CLOCK_KHZ/Clock_RateKHz;
    }
    
    __inline static UINT32 ConfigurationToCSR( const SPI_CONFIGURATION& Configuration )
    {
        UINT32 CSR = 0;

        if(Configuration.MD_16bits)
        {
            CSR |= SPI_CSR_16BITS;
        }
        else
        {
            CSR |= SPI_CSR_8BITS;
        }

    // NOTE :: AT91 core SPI phase definition is opposite to the typical convention
    // phase = 0 for at91, data change at 2nd edge
    // phase = 1 for at91, data change at 1st edge

        if(Configuration.MSK_IDLE)
        {
           // Idle is Active high, set 1, 
            CSR |= SPI_CSR_CPOL;   
           
            if(!Configuration.MSK_SampleEdge)
            {
                CSR |= SPI_CSR_NCPHA;        
            }
            // else phase set to 0
        }
        else
        {
            if(Configuration.MSK_SampleEdge)
            {
                CSR |= SPI_CSR_NCPHA;        
            }
            // else phase set to 0;
        }
        CSR |=  ConvertClockRateToDivisor(Configuration.Clock_RateKHz)<<SPI_CSR_SCBR_SHIFT ;
        return CSR;
    }

     
     // no data in TX FIFO
    __inline BOOL TransmitBufferEmpty(AT91_SPI & _SPI)
    {
        return (_SPI.SPI_SR & SPI_SR_TXEMPTY ) != 0;
    }
};

//
// AT91_SPI
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// AT91_I2C
//
struct AT91_I2C
{
    static const UINT32 c_Base = AT91C_BASE_TWI;

    //--//
    
    /****/ volatile UINT32 TWI_CR;         // Control Register
    static const    UINT32 TWI_CR_START      = (0x1 <<  0); // send START condition 
    static const    UINT32 TWI_CR_STOP       = (0x1 <<  1); // send STOP condition 
    static const    UINT32 TWI_CR_MSEN       = (0x1 <<  2); // Enable Master Transfer
    static const    UINT32 TWI_CR_MSDIS      = (0x1 <<  3); // Disable Master Transfer
// for SAM9
    static const    UINT32 TWI_CR_SVEN       = (0x1 <<  4); // enable Slave Transfer
    static const    UINT32 TWI_CR_SVDIS      = (0x1 <<  5); // Disable Slave Transfer
    static const    UINT32 TWI_CR_QUICK      = (0x1 <<  6); // 
////

    static const    UINT32 TWI_CR_SWRST      = (0x1 <<  7); // Softwre Reset

    /****/ volatile UINT32 TWI_MMR;           // Master Mode Register
    static const    UINT32 TWI_MMR_IADRSZ_0   = (0x0 <<  8);  // No internal device address
    static const    UINT32 TWI_MMR_IADRSZ_1   = (0x1 <<  8);  // One byte device address
    static const    UINT32 TWI_MMR_IADRSZ_2   = (0x2 <<  8);  // Two bytes device address
    static const    UINT32 TWI_MMR_MREAD_W    = (0x0 <<  12); // Three bytes device address
    static const    UINT32 TWI_MMR_MREAD_R    = (0x1 <<  12); // Three bytes device address
    static const    UINT32 TWI_MMR_DADR_MASK  = 0x00FF0000 ;  // Subordinate device address mask
    static const    UINT32 TWI_MMR_DADR_SHIFT = 16;           // subordinate address position

    /****/ volatile UINT32 Reserved0[1];    
    
    /****/ volatile UINT32 TWI_IADR;           // Internal address register
    static const    UINT32 TWI_IADR_MASK_0     = 0x00000000; // No internal device address
    static const    UINT32 TWI_IADR_MASK_1     = 0x000000FF; // One byte device address
    static const    UINT32 TWI_IADR_MASK_2     = 0x0000FFFF; // Two bytes device address
    static const    UINT32 TWI_IADR_MASK_3     = 0x00FFFFFF; // Three bytes device address

    /****/ volatile UINT32 TWI_CWGR;           // Clock waveform generator Register
    static const    UINT32 TWI_CWGR_CLDIV_MASK = 0x000000FF; // Clock Low Divider
    static const    UINT32 TWI_CWGR_CHDIV_MASK = 0x0000FF00; // Clock High Divider
    static const    UINT32 TWI_CWGR_CHDIV_SHIFT= 8;         // Clock High Divider shift   
    static const    UINT32 TWI_CWGR_CKDIV_MASK = 0x00070000; // Clock Divider
    static const    UINT32 TWI_CWGR_CKDIV_SHIFT= 16;         // Clock Divider shift

    /****/ volatile UINT32 Reserved1[3]; 
    
    /****/ volatile UINT32 TWI_SR;             // Status Register
    static const    UINT32 TWI_SR_TXCOMP       = (0x1 <<  0); // holding and shift register are empty and STOP condition has been sent
    static const    UINT32 TWI_SR_RXRDY        = (0x1 <<  1); // Data moved from shifter since last read
    static const    UINT32 TWI_SR_TXRDY        = (0x1 <<  2); // Data moved to shifter
    static const    UINT32 TWI_SR_NACK         = (0x1 <<  8); // No acknoledge received

    /****/ volatile UINT32 TWI_IER;            // Interrupt Enable Register
    static const    UINT32 TWI_IER_TXCOMP      = (0x1 <<  0); // Transmission completed
    static const    UINT32 TWI_IER_RXRDY       = (0x1 <<  1); // receive holding register ready
    static const    UINT32 TWI_IER_TXRDY       = (0x1 <<  2); // transmit holding register ready
    static const    UINT32 TWI_IER_NACK        = (0x1 <<  8); // No acknoledge received

    /****/ volatile UINT32 TWI_IDR;            // Interrupt Disable Register
    static const    UINT32 TWI_IDR_TXCOMP      = (0x1 <<  0); // Transmission completed
    static const    UINT32 TWI_IDR_RXRDY       = (0x1 <<  1); // receive holding register ready
    static const    UINT32 TWI_IDR_TXRDY       = (0x1 <<  2); // transmit holding register ready
    static const    UINT32 TWI_IDR_NACK        = (0x1 <<  8); // No acknoledge received

    /****/ volatile UINT32 TWI_IMR;            // Interrupt Mask Register
    static const    UINT32 TWI_IMR_TXCOMP      = (0x1 <<  0); // Transmission completed
    static const    UINT32 TWI_IMR_RXRDY       = (0x1 <<  1); // receive holding register ready
    static const    UINT32 TWI_IMR_TXRDY       = (0x1 <<  2); // transmit holding register ready
    static const    UINT32 TWI_IMR_NACK        = (0x1 <<  8); // No acknoledge received

    /****/ volatile UINT32 TWI_RHR;            // Receive Holding Register
    static const    UINT32 TWI_RHR_RXDATA_MASK = 0x000000FF; // data mask
    
    /****/ volatile UINT32 TWI_THR;            // Receive Holding Register
    static const    UINT32 TWI_THR_TXDATA_MASK = 0x000000FF; // data mask
};
//
// AT91_I2C
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// AT91_USART
//
struct AT91_USART {

    static const UINT32 c_Base_dbg      = AT91C_BASE_DBGU;
    static const UINT32 c_Base          = AT91C_BASE_US0;
    static const UINT32 c_Base_offset   = 0x4000;

    static const UINT32 c_MAX_BAUDRATE  = ((SYSTEM_PERIPHERAL_CLOCK_HZ*10)/ 16); 
    static const UINT32 c_MIN_BAUDRATE  = 0;
    
    /****/ volatile UINT32 US_CR;          // Control Register
    static const    UINT32 US_RSTRX        = ((unsigned int) 0x1 <<  2); // (DBGU) Reset Receiver
    static const    UINT32 US_RSTTX        = ((unsigned int) 0x1 <<  3); //  (DBGU) Reset Transmitter
    static const    UINT32 US_RXEN         = ((unsigned int) 0x1 <<  4); //  (DBGU) Receiver Enable
    static const    UINT32 US_RXDIS        = ((unsigned int) 0x1 <<  5); //  (DBGU) Receiver Disable
    static const    UINT32 US_TXEN         = ((unsigned int) 0x1 <<  6); //  (DBGU) Transmitter Enable
    static const    UINT32 US_TXDIS        = ((unsigned int) 0x1 <<  7); //  (DBGU) Transmitter Disable
    static const    UINT32 US_RSTSTA       = ((unsigned int) 0x1 <<  8); //  (DBGU) Reset Status Bits
    static const    UINT32 US_STTBRK       = ((unsigned int) 0x1 <<  9); //  (USART) Start Break
    static const    UINT32 US_STPBRK       = ((unsigned int) 0x1 << 10); //  (USART) Stop Break
    static const    UINT32 US_STTTO        = ((unsigned int) 0x1 << 11); //  (USART) Start Time-out
    static const    UINT32 US_SENDA        = ((unsigned int) 0x1 << 12); //  (USART) Send Address
    static const    UINT32 US_RSTIT        = ((unsigned int) 0x1 << 13); //  (USART) Reset Iterations
    static const    UINT32 US_RSTNACK      = ((unsigned int) 0x1 << 14); //  (USART) Reset Non Acknowledge
    static const    UINT32 US_RETTO        = ((unsigned int) 0x1 << 15); //  (USART) Rearm Time-out
    static const    UINT32 US_DTREN        = ((unsigned int) 0x1UL << 16); //  (USART) Data Terminal ready Enable
    static const    UINT32 US_DTRDIS       = ((unsigned int) 0x1UL << 17); //  (USART) Data Terminal ready Disable
    static const    UINT32 US_RTSEN        = ((unsigned int) 0x1UL << 18); //  (USART) Request to Send enable
    static const    UINT32 US_RTSDIS       = ((unsigned int) 0x1UL << 19); //  (USART) Request to Send Disable

    /****/ volatile UINT32 US_MR;          // Mode Register
    static const    UINT32 US_USMODE       = ((unsigned int) 0xF <<  0); //  (USART) Usart mode
    static const    UINT32     US_USMODE_NORMAL               = ((unsigned int) 0x0); //  (USART) Normal
    static const    UINT32     US_USMODE_RS485                = ((unsigned int) 0x1); //  (USART) RS485
    static const    UINT32     US_USMODE_HWHSH                = ((unsigned int) 0x2); //  (USART) Hardware Handshaking
    static const    UINT32     US_USMODE_MODEM                = ((unsigned int) 0x3); //  (USART) Modem
    static const    UINT32     US_USMODE_ISO7816_0            = ((unsigned int) 0x4); //  (USART) ISO7816 protocol: T = 0
    static const    UINT32     US_USMODE_ISO7816_1            = ((unsigned int) 0x6); //  (USART) ISO7816 protocol: T = 1
    static const    UINT32     US_USMODE_IRDA                 = ((unsigned int) 0x8); //  (USART) IrDA
    static const    UINT32     US_USMODE_SWHSH                = ((unsigned int) 0xC); //  (USART) Software Handshaking
    static const    UINT32 US_CLKS         = ((unsigned int) 0x3 <<  4); //  (USART) Clock Selection = (Baud Rate generator Input Clock
    static const    UINT32     US_CLKS_CLOCK                = ((unsigned int) 0x0 <<  4); //  (USART) Clock
    static const    UINT32     US_CLKS_FDIV1                = ((unsigned int) 0x1 <<  4); //  (USART) fdiv1
    static const    UINT32     US_CLKS_SLOW                 = ((unsigned int) 0x2 <<  4); //  (USART) slow_clock = (ARM);
    static const    UINT32     US_CLKS_EXT                  = ((unsigned int) 0x3 <<  4); //  (USART) External = (SCK);
    static const    UINT32 US_CHRL         = ((unsigned int) 0x3 <<  6); //  (USART) Clock Selection = (Baud Rate generator Input Clock
    static const    UINT32     US_CHRL_5_BITS               = ((unsigned int) 0x0 <<  6); //  (USART) Character Length: 5 bits
    static const    UINT32     US_CHRL_6_BITS               = ((unsigned int) 0x1 <<  6); //  (USART) Character Length: 6 bits
    static const    UINT32     US_CHRL_7_BITS               = ((unsigned int) 0x2 <<  6); //  (USART) Character Length: 7 bits
    static const    UINT32     US_CHRL_8_BITS               = ((unsigned int) 0x3 <<  6); //  (USART) Character Length: 8 bits
    static const    UINT32 US_SYNC         = ((unsigned int) 0x1 <<  8); //  (USART) Synchronous Mode Select
    static const    UINT32 US_NBSTOP       = ((unsigned int) 0x3 << 12); //  (USART) Number of Stop bits
    static const    UINT32     US_NBSTOP_1_BIT                = ((unsigned int) 0x0 << 12); //  (USART) 1 stop bit
    static const    UINT32     US_NBSTOP_15_BIT               = ((unsigned int) 0x1 << 12); //  (USART) Asynchronous = (SYNC=0); 2 stop bits Synchronous = (SYNC=1); 2 stop bits
    static const    UINT32     US_NBSTOP_2_BIT                = ((unsigned int) 0x2 << 12); //  (USART) 2 stop bits
    static const    UINT32 US_MSBF         = ((unsigned int) 0x1 << 16); //  (USART) Bit Order
    static const    UINT32 US_MODE9        = ((unsigned int) 0x1 << 17); //  (USART) 9-bit Character length
    static const    UINT32 US_CKLO         = ((unsigned int) 0x1 << 18); //  (USART) Clock Output Select
    static const    UINT32 US_OVER         = ((unsigned int) 0x1 << 19); //  (USART) Over Sampling Mode
    static const    UINT32 US_INACK        = ((unsigned int) 0x1 << 20); //  (USART) Inhibit Non Acknowledge
    static const    UINT32 US_DSNACK       = ((unsigned int) 0x1 << 21); //  (USART) Disable Successive NACK
    static const    UINT32 US_MAX_ITER     = ((unsigned int) 0x1 << 24); //  (USART) Number of Repetitions
    static const    UINT32 US_FILTER       = ((unsigned int) 0x1 << 28); //  (USART) Receive Line Filter
    static const    UINT32 US_PAR          = (0x7 <<  9); // = (DBGU); Parity type
    static const    UINT32     US_PAR_EVEN                 = (0x0 <<  9); // = (DBGU); Even Parity
    static const    UINT32     US_PAR_ODD                  = (0x1 <<  9); // = (DBGU); Odd Parity
    static const    UINT32     US_PAR_SPACE                = (0x2 <<  9); // = (DBGU); Parity forced to 0 = (Space);
    static const    UINT32     US_PAR_MARK                 = (0x3 <<  9); // = (DBGU); Parity forced to 1 = (Mark);
    static const    UINT32     US_PAR_NONE                 = (0x4 <<  9); // = (DBGU); No Parity
    static const    UINT32     US_PAR_MULTI_DROP           = (0x6 <<  9); // = (DBGU); Multi-drop mode
    static const    UINT32 US_CHMODE       = (0x3 << 14); // = (DBGU); Channel Mode
    static const    UINT32     US_CHMODE_NORMAL               = (0x0UL << 14); // = (DBGU); Normal Mode: The USART channel operates as an RX/TX USART.
    static const    UINT32     US_CHMODE_AUTO                 = (0x1UL << 14); // = (DBGU); Automatic Echo: Receiver Data Input is connected to the TXD pin.
    static const    UINT32     US_CHMODE_LOCAL                = (0x2UL << 14); // = (DBGU); Local Loopback: Transmitter Output Signal is connected to Receiver Input Signal.
    static const    UINT32     US_CHMODE_REMOTE               = (0x3UL << 14); // = (DBGU); Remote Loopback: RXD pin is internally connected to TXD pin.

    /****/ volatile UINT32 US_IER;         // Interrupt Enable Register
    static const    UINT32 US_RXBRK        = ((unsigned int) 0x1 <<  2); //  (USART) Break Received/End of Break
    static const    UINT32 US_TIMEOUT      = ((unsigned int) 0x1 <<  8); //  (USART) Receiver Time-out
    static const    UINT32 US_ITERATION    = ((unsigned int) 0x1 << 10); //  (USART) Max number of Repetitions Reached
    static const    UINT32 US_NACK         = ((unsigned int) 0x1 << 13); //  (USART) Non Acknowledge
    static const    UINT32 US_RIIC         = ((unsigned int) 0x1 << 16); //  (USART) Ring INdicator Input Change Flag
    static const    UINT32 US_DSRIC        = ((unsigned int) 0x1 << 17); //  (USART) Data Set Ready Input Change Flag
    static const    UINT32 US_DCDIC        = ((unsigned int) 0x1 << 18); //  (USART) Data Carrier Flag
    static const    UINT32 US_CTSIC        = ((unsigned int) 0x1 << 19); //  (USART) Clear To Send Input Change Flag
    static const    UINT32 US_RXRDY        = (0x1UL <<  0); // = (DBGU); RXRDY Interrupt
    static const    UINT32 US_TXRDY        = (0x1UL <<  1); // = (DBGU); TXRDY Interrupt
    static const    UINT32 US_ENDRX        = (0x1UL <<  3); // = (DBGU); End of Receive Transfer Interrupt
    static const    UINT32 US_ENDTX        = (0x1UL <<  4); // = (DBGU); End of Transmit Interrupt
    static const    UINT32 US_OVRE         = (0x1UL <<  5); // = (DBGU); Overrun Interrupt
    static const    UINT32 US_FRAME        = (0x1UL <<  6); // = (DBGU); Framing Error Interrupt
    static const    UINT32 US_PARE         = (0x1UL <<  7); // = (DBGU); Parity Error Interrupt
    static const    UINT32 US_TXEMPTY      = (0x1UL <<  9); // = (DBGU); TXEMPTY Interrupt
    static const    UINT32 US_TXBUFE       = (0x1UL << 11); // = (DBGU); TXBUFE Interrupt
    static const    UINT32 US_RXBUFF       = (0x1UL << 12); // = (DBGU); RXBUFF Interrupt
    static const    UINT32 US_COMM_TX      = (0x1UL << 30); // = (DBGU); COMM_TX Interrupt
    static const    UINT32 US_COMM_RX      = (0x1UL << 31); // = (DBGU); COMM_RX Interrupt

    /****/ volatile UINT32 US_IDR;         // Interrupt Disable Register

    /****/ volatile UINT32 US_IMR;         // Interrupt Mask Register

    /****/ volatile UINT32 US_CSR;         // Channel Status Register
    static const    UINT32 US_RI           = ((unsigned int) 0x1 << 20); //  (USART) Image of RI Input
    static const    UINT32 US_DSR          = ((unsigned int) 0x1 << 21); //  (USART) Image of DSR Input
    static const    UINT32 US_DCD          = ((unsigned int) 0x1 << 22); //  (USART) Image of DCD Input
    static const    UINT32 US_CTS          = ((unsigned int) 0x1 << 23); //  (USART) Image of CTS Input

    /****/ volatile UINT32 US_RHR;         // Receiver Holding Register

    /****/ volatile UINT32 US_THR;         // Transmitter Holding Register

    /****/ volatile UINT32 US_BRGR;        // Baud Rate Generator Register

    /****/ volatile UINT32 US_RTOR;        // Receiver Time-out Register

    /****/ volatile UINT32 US_TTGR;        // Transmitter Time-guard Register

    /****/ volatile UINT32 Reserved0[5];   // 

    /****/ volatile UINT32 US_FIDI;        // FI_DI_Ratio Register

    /****/ volatile UINT32 US_NER;         // Nb Errors Register

    /****/ volatile UINT32 Reserved1[1];   // 

    /****/ volatile UINT32 US_IF;          // IRDA_FILTER Register
    
#if defined(PLATFORM_ARM_SAM9RL64_ANY)

    /****/ volatile UINT32 US_MAN;          // Manchester Encoder Decoder Register

    /****/ volatile UINT32 Reserved2[43];

#else
    /****/ volatile UINT32 Reserved2[44];

#endif

    /****/ volatile UINT32 US_RPR;         // Receive Pointer Register

    /****/ volatile UINT32 US_RCR;         // Receive Counter Register

    /****/ volatile UINT32 US_TPR;         // Transmit Pointer Register

    /****/ volatile UINT32 US_TCR;         // Transmit Counter Register

    /****/ volatile UINT32 US_RNPR;        // Receive Next Pointer Register

    /****/ volatile UINT32 US_RNCR;        // Receive Next Counter Register

    /****/ volatile UINT32 US_TNPR;        // Transmit Next Pointer Register

    /****/ volatile UINT32 US_TNCR;        // Transmit Next Counter Register

    /****/ volatile UINT32 US_PTCR;        // PDC Transfer Control Register

    /****/ volatile UINT32 US_PTSR;        // PDC Transfer Status Register
};

//////////////////////////////////////////////////////////////////////////////
// AT91_UDP  USB controller
//
struct AT91_UDP {

    static const UINT32 c_Base = AT91C_BASE_UDP;

    /****/ volatile UINT32 UDP_NUM;     // Frame Number Register
    static const    UINT32 UDP_FRM_NUM         = (0x7FF <<  0); // (UDP) Frame Number as Defined in the Packet Field Formats
    static const    UINT32 UDP_FRM_ERR         = (0x1 << 16); // (UDP) Frame Error
    static const    UINT32 UDP_FRM_OK          = (0x1 << 17); // (UDP) Frame OK

    /****/ volatile UINT32 UDP_GLBSTATE;     // Global State Register
// -------- UDP_GLB_STATE : = (UDP Offset: 0x4); USB Global State Register -------- 
    static const    UINT32 UDP_FADDEN          = (0x1 <<  0); // (UDP) Function Address Enable
    static const    UINT32 UDP_CONFG           = (0x1 <<  1); // (UDP) Configured
    static const    UINT32 UDP_ESR             = (0x1 <<  2); // (UDP) Enable Send Resume
    static const    UINT32 UDP_RSMINPR         = (0x1 <<  3); // (UDP) A Resume Has Been Sent to the Host
    static const    UINT32 UDP_RMWUPE          = (0x1 <<  4); // (UDP) Remote Wake Up Enable

    /****/ volatile UINT32 UDP_FADDR;     // Function Address Register
// -------- UDP_FADDR : = (UDP Offset: 0x8); USB Function Address Register -------- 
    static const    UINT32 UDP_FADD            = (0xFF <<  0); // (UDP) Function Address Value
    static const    UINT32 UDP_FEN             = (0x1 <<  8); // (UDP) Function Enable

    /****/ volatile UINT32 Reserved0[1];     // 

    /****/ volatile UINT32 UDP_IER;     // Interrupt Enable Register
// -------- UDP_IER : = (UDP Offset: 0x10); USB Interrupt Enable Register -------- 
    static const    UINT32 UDP_EPINT0          = (0x1 <<  0); // (UDP) Endpoint 0 Interrupt
    static const    UINT32 UDP_EPINT1          = (0x1 <<  1); // (UDP) Endpoint 0 Interrupt
    static const    UINT32 UDP_EPINT2          = (0x1 <<  2); // (UDP) Endpoint 2 Interrupt
    static const    UINT32 UDP_EPINT3          = (0x1 <<  3); // (UDP) Endpoint 3 Interrupt
    static const    UINT32 UDP_EPINT4          = (0x1 <<  4); // (UDP) Endpoint 4 Interrupt
    static const    UINT32 UDP_EPINT5          = (0x1 <<  5); // (UDP) Endpoint 5 Interrupt
    static const    UINT32 UDP_EPINT_MASK      = (0x3Ful   ); // (UDP) Endpoint Interrupt Mask
    static const    UINT32 UDP_RXSUSP          = (0x1 <<  8); // (UDP) USB Suspend Interrupt
    static const    UINT32 UDP_RXRSM           = (0x1 <<  9); // (UDP) USB Resume Interrupt
    static const    UINT32 UDP_EXTRSM          = (0x1 << 10); // (UDP) USB External Resume Interrupt
    static const    UINT32 UDP_SOFINT          = (0x1 << 11); // (UDP) USB Start Of frame Interrupt
    static const    UINT32 UDP_WAKEUP          = (0x1 << 13); // (UDP) USB Resume Interrupt

    /****/ volatile UINT32 UDP_IDR;     // Interrupt Disable Register

    /****/ volatile UINT32 UDP_IMR;     // Interrupt Mask Register

    /****/ volatile UINT32 UDP_ISR;     // Interrupt Status Register
// -------- UDP_ISR : = (UDP Offset: 0x1c); USB Interrupt Status Register -------- 
    static const    UINT32 UDP_ISRMASK            = 0x00003B3F;
    static const    UINT32 UDP_ENDBUSRES       = (0x1 << 12); // (UDP) USB End Of Bus Reset Interrupt

    /****/ volatile UINT32 UDP_ICR;     // Interrupt Clear Register

    /****/ volatile UINT32 Reserved1[1];     // 

    /****/ volatile UINT32 UDP_RSTEP;     // Reset Endpoint Register
// -------- UDP_RST_EP : = (UDP Offset: 0x28); USB Reset Endpoint Register -------- 
    static const    UINT32 UDP_EP0             = (0x1 <<  0); // (UDP) Reset Endpoint 0
    static const    UINT32 UDP_EP1             = (0x1 <<  1); // (UDP) Reset Endpoint 1
    static const    UINT32 UDP_EP2             = (0x1 <<  2); // (UDP) Reset Endpoint 2
    static const    UINT32 UDP_EP3             = (0x1 <<  3); // (UDP) Reset Endpoint 3
    static const    UINT32 UDP_EP4             = (0x1 <<  4); // (UDP) Reset Endpoint 4
    static const    UINT32 UDP_EP5             = (0x1 <<  5); // (UDP) Reset Endpoint 5

    /****/ volatile UINT32 Reserved2[1];     // 

    /****/ volatile UINT32 UDP_CSR[6];     // Endpoint Control and Status Register
// -------- UDP_CSR : = (UDP Offset: 0x30); USB Endpoint Control and Status Register -------- 
    static const    UINT32 UDP_TXCOMP          = (0x1 <<  0); // (UDP) Generates an IN packet with data previously written in the DPR
    static const    UINT32 UDP_RX_DATA_BK0     = (0x1 <<  1); // (UDP) Receive Data Bank 0
    static const    UINT32 UDP_RXSETUP         = (0x1 <<  2); // (UDP) Sends STALL to the Host = (Control endpoints);
    static const    UINT32 UDP_ISOERROR        = (0x1 <<  3); // (UDP) Isochronous error = (Isochronous endpoints);
    static const    UINT32 UDP_STALLSENT       = (0x1 <<  3); // (UDP) Stall sent = (Control, bulk, interrupt endpoints);
    static const    UINT32 UDP_TXPKTRDY        = (0x1 <<  4); // (UDP) Transmit Packet Ready
    static const    UINT32 UDP_FORCESTALL      = (0x1 <<  5); // (UDP) Force Stall = (used by Control, Bulk and Isochronous endpoints);.
    static const    UINT32 UDP_RX_DATA_BK1     = (0x1 <<  6); // (UDP) Receive Data Bank 1 = (only used by endpoints with ping-pong attributes);.
    static const    UINT32 UDP_DIR             = (0x1 <<  7); // (UDP) Transfer Direction
    static const    UINT32 UDP_EPTYPE          = (0x7 <<  8); // (UDP) Endpoint type
    static const    UINT32 UDP_EPTYPE_CTRL     = (0x0 <<  8); // (UDP) Control
    static const    UINT32 UDP_EPTYPE_ISO_OUT  = (0x1 <<  8); // (UDP) Isochronous OUT
    static const    UINT32 UDP_EPTYPE_BULK_OUT = (0x2 <<  8); // (UDP) Bulk OUT
    static const    UINT32 UDP_EPTYPE_INT_OUT  = (0x3 <<  8); // (UDP) Interrupt OUT
    static const    UINT32 UDP_EPTYPE_ISO_IN   = (0x5 <<  8); // (UDP) Isochronous IN
    static const    UINT32 UDP_EPTYPE_BULK_IN  = (0x6 <<  8); // (UDP) Bulk IN
    static const    UINT32 UDP_EPTYPE_INT_IN   = (0x7 <<  8); // (UDP) Interrupt IN
    static const    UINT32 UDP_DTGLE           = (0x1 << 11); // (UDP) Data Toggle
    static const    UINT32 UDP_EPEDS           = (0x1UL << 15); // (UDP) Endpoint Enable Disable
    static const    UINT32 UDP_RXBYTECNT       = (0x7FFUL << 16); // (UDP) Number Of Bytes Available in the FIFO

    /****/ volatile UINT32 Reserved3[2];     // 

    /****/ volatile UINT32 UDP_FDR[6];     // Endpoint FIFO Data Register

    /****/ volatile UINT32 Reserved4[3];     // 

    /****/ volatile UINT32 UDP_TXVC;     // Transceiver Control Register
// -------- UDP_TXVC : = (UDP Offset: 0x74); Transceiver Control Register -------- 
    static const    UINT32 UDP_TXVDIS          = (0x1 <<  8); // (UDP) 
    static const    UINT32 UDP_PUON            = (0x1 <<  9); // (UDP) Pull-up ON
} ;



//////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//---// SAM9261 only 
#if defined(PLATFORM_ARM_SAM9261_ANY) 

struct AT91_MATRIX {
     static const UINT32 c_Base = AT91C_BASE_MATRIX;

    /****/ volatile UINT32 MATRIX_MCFG;     //  Master Configuration Register
    static const    UINT32 MATRIX_RCA926I                    = (0x1 <<  0); // (MATRIX) Remap Command for ARM926EJ-S Instruction Master
    static const    UINT32 MATRIX_RCA926D                    = (0x1 <<  1); // (MATRIX) Remap Command for ARM926EJ-S Data Master

    /****/ volatile UINT32 MATRIX_SCFG0;    //  Slave Configuration Register 0
    static const    UINT32 MATRIX_SLOT_CYCLE                 = (0xFF <<  0); // (MATRIX) Maximum Number of Allowed Cycles for a Burst
    static const    UINT32 MATRIX_DEFMSTR_TYPE               = (0x3UL << 16); // (MATRIX) Default Master Type
    static const    UINT32 MATRIX_DEFMSTR_TYPE_NO_DEFMSTR    = (0x0UL << 16); // (MATRIX) No Default Master. At the end of current Slave access, if no other master request is pending, the Slave is deconnected from all masters. This results in having a one cycle latency for the first transfer of a burst.
    static const    UINT32 MATRIX_DEFMSTR_TYPE_LAST_DEFMSTR  = (0x1UL << 16); // (MATRIX) Last Default Master. At the end of current Slave access, if no other master request is pending, the Slave stay connected with the last master having accessed it. This results in not having the one cycle latency when the last master re-trying access on the Slave.
    static const    UINT32 MATRIX_DEFMSTR_TYPE_FIXED_DEFMSTR = (0x2UL << 16); // (MATRIX) Fixed Default Master. At the end of current Slave access, if no other master request is pending, the Slave connects with fixed which number is in FIXED_DEFMSTR field. This results in not having the one cycle latency when the fixed master re-trying access on the Slave.
    static const    UINT32 MATRIX_FIXED_DEFMSTR0             = (0x7UL << 18); // (MATRIX) Fixed Index of Default Master
    static const    UINT32 MATRIX_FIXED_DEFMSTR0_ARM926I     = (0x0UL << 18); // (MATRIX) ARM926EJ-S Instruction Master is Default Master
    static const    UINT32 MATRIX_FIXED_DEFMSTR0_ARM926D     = (0x1UL << 18); // (MATRIX) ARM926EJ-S Data Master is Default Master
    static const    UINT32 MATRIX_FIXED_DEFMSTR0_HPDC3       = (0x2UL << 18); // (MATRIX) HPDC3 Master is Default Master
    static const    UINT32 MATRIX_FIXED_DEFMSTR0_LCDC        = (0x3UL << 18); // (MATRIX) LCDC Master is Default Master
    static const    UINT32 MATRIX_FIXED_DEFMSTR0_UHP         = (0x4UL << 18); // (MATRIX) UHP Master is Default Master

    /****/ volatile UINT32 MATRIX_SCFG1;    //  Slave Configuration Register 1
    static const    UINT32 MATRIX_FIXED_DEFMSTR1             = (0x7UL << 18); // (MATRIX) Fixed Index of Default Master
    static const    UINT32 MATRIX_FIXED_DEFMSTR1_ARM926I     = (0x0UL << 18); // (MATRIX) ARM926EJ-S Instruction Master is Default Master
    static const    UINT32 MATRIX_FIXED_DEFMSTR1_ARM926D     = (0x1UL << 18); // (MATRIX) ARM926EJ-S Data Master is Default Master
    static const    UINT32 MATRIX_FIXED_DEFMSTR1_HPDC3       = (0x2UL << 18); // (MATRIX) HPDC3 Master is Default Master
    static const    UINT32 MATRIX_FIXED_DEFMSTR1_LCDC        = (0x3UL << 18); // (MATRIX) LCDC Master is Default Master
    static const    UINT32 MATRIX_FIXED_DEFMSTR1_UHP         = (0x4UL << 18); // (MATRIX) UHP Master is Default Master

    /****/ volatile UINT32 MATRIX_SCFG2;    //  Slave Configuration Register 2
    static const    UINT32 MATRIX_FIXED_DEFMSTR2             = (0x1UL << 18); // (MATRIX) Fixed Index of Default Master
    static const    UINT32 MATRIX_FIXED_DEFMSTR2_ARM926I     = (0x0UL << 18); // (MATRIX) ARM926EJ-S Instruction Master is Default Master
    static const    UINT32 MATRIX_FIXED_DEFMSTR2_ARM926D     = (0x1UL << 18); // (MATRIX) ARM926EJ-S Data Master is Default Master

    /****/ volatile UINT32 MATRIX_SCFG3;    //  Slave Configuration Register 3
    static const    UINT32 MATRIX_FIXED_DEFMSTR3             = (0x7UL << 18); // (MATRIX) Fixed Index of Default Master
    static const    UINT32 MATRIX_FIXED_DEFMSTR3_ARM926I     = (0x0UL << 18); // (MATRIX) ARM926EJ-S Instruction Master is Default Master
    static const    UINT32 MATRIX_FIXED_DEFMSTR3_ARM926D     = (0x1UL << 18); // (MATRIX) ARM926EJ-S Data Master is Default Master
    static const    UINT32 MATRIX_FIXED_DEFMSTR3_HPDC3       = (0x2UL << 18); // (MATRIX) HPDC3 Master is Default Master
    static const    UINT32 MATRIX_FIXED_DEFMSTR3_LCDC        = (0x3UL << 18); // (MATRIX) LCDC Master is Default Master
    static const    UINT32 MATRIX_FIXED_DEFMSTR3_UHP         = (0x4UL << 18); // (MATRIX) UHP Master is Default Master

    /****/ volatile UINT32 MATRIX_SCFG4;    //  Slave Configuration Register 4
    static const    UINT32 MATRIX_FIXED_DEFMSTR4             = (0x3UL << 18); // (MATRIX) Fixed Index of Default Master
    static const    UINT32  MATRIX_FIXED_DEFMSTR4_ARM926I    = (0x0UL << 18); // (MATRIX) ARM926EJ-S Instruction Master is Default Master
    static const    UINT32  MATRIX_FIXED_DEFMSTR4_ARM926D    = (0x1UL << 18); // (MATRIX) ARM926EJ-S Data Master is Default Master
    static const    UINT32  MATRIX_FIXED_DEFMSTR4_HPDC3      = (0x2UL << 18); // (MATRIX) HPDC3 Master is Default Master

    /****/ volatile UINT32 Reserved0[3];    // 

    /****/ volatile UINT32 MATRIX_TCMR;     //  Slave 0 Special Function Register
    static const    UINT32 MATRIX_ITCM_SIZE                  = (0xF <<  0); // (MATRIX) Size of ITCM enabled memory block
    static const    UINT32 MATRIX_ITCM_SIZE_0KB              = (0x0); // (MATRIX) 0 KB = (No ITCM Memory);
    static const    UINT32 MATRIX_ITCM_SIZE_16KB             = (0x5); // (MATRIX) 16 KB
    static const    UINT32 MATRIX_ITCM_SIZE_32KB             = (0x6); // (MATRIX) 32 KB
    static const    UINT32 MATRIX_ITCM_SIZE_64KB             = (0x7); // (MATRIX) 64 KB
    static const    UINT32 MATRIX_DTCM_SIZE                  = (0xF <<  4); // (MATRIX) Size of DTCM enabled memory block
    static const    UINT32 MATRIX_DTCM_SIZE_0KB              = (0x0 <<  4); // (MATRIX) 0 KB = (No DTCM Memory);
    static const    UINT32 MATRIX_DTCM_SIZE_16KB             = (0x5 <<  4); // (MATRIX) 16 KB
    static const    UINT32 MATRIX_DTCM_SIZE_32KB             = (0x6 <<  4); // (MATRIX) 32 KB
    static const    UINT32 MATRIX_DTCM_SIZE_64KB             = (0x7 <<  4); // (MATRIX) 64 KB

    /****/ volatile UINT32 Reserved1[2];    // 
    /****/ volatile UINT32 MATRIX_EBICSA;   //  Slave 3 Special Function Register
    static const    UINT32 MATRIX_CS1A                       = (0x1 <<  1); // (MATRIX) Chip Select 1 Assignment
    static const    UINT32 MATRIX_CS1A_SMC                   = (0x0 <<  1); // (MATRIX) Chip Select 1 is assigned to the Static Memory Controller.
    static const    UINT32 MATRIX_CS1A_SDRAMC                = (0x1 <<  1); // (MATRIX) Chip Select 1 is assigned to the SDRAM Controller.
    static const    UINT32 MATRIX_CS3A                       = (0x1 <<  3); // (MATRIX) Chip Select 3 Assignment
    static const    UINT32 MATRIX_CS3A_SMC                   = (0x0 <<  3); // (MATRIX) Chip Select 3 is only assigned to the Static Memory Controller and NCS3 behaves as defined by the Static Memory Controller.
    static const    UINT32 MATRIX_CS3A_SM                    = (0x1 <<  3); // (MATRIX) Chip Select 3 is assigned to the Static Memory Controller and the SmartMedia Logic is activated.
    static const    UINT32 MATRIX_CS4A                       = (0x1 <<  4); // (MATRIX) Chip Select 4 Assignment
    static const    UINT32 MATRIX_CS4A_SMC                   = (0x0 <<  4); // (MATRIX) Chip Select 4 is only assigned to the Static Memory Controller and NCS4 behaves as defined by the Static Memory Controller.
    static const    UINT32 MATRIX_CS4A_CF                    = (0x1 <<  4); // (MATRIX) Chip Select 4 is assigned to the Static Memory Controller and the CompactFlash Logic = (first slot); is activated.
    static const    UINT32 MATRIX_CS5A                       = (0x1 <<  5); // (MATRIX) Chip Select 5 Assignment
    static const    UINT32 MATRIX_CS5A_SMC                   = (0x0 <<  5); // (MATRIX) Chip Select 5 is only assigned to the Static Memory Controller and NCS5 behaves as defined by the Static Memory Controller
    static const    UINT32 MATRIX_CS5A_CF                    = (0x1 <<  5); // (MATRIX) Chip Select 5 is assigned to the Static Memory Controller and the CompactFlash Logic = (second slot); is activated.
    static const    UINT32 MATRIX_DBPUC                      = (0x1 <<  8); // (MATRIX) Data Bus Pull-up Configuration

    /****/ volatile UINT32 MATRIX_USBPCR;   //  Slave 4 Special Function Register
    static const    UINT32 MATRIX_USBPCR_PUON                = (0x1 << 30); // (MATRIX) PullUp On
    static const    UINT32 MATRIX_USBPCR_PUIDLE              = ((UINT32)0x1 << 31); // (MATRIX) PullUp Idle

    /****/ volatile UINT32 Reserved2[3];    // 

    /****/ volatile UINT32 MATRIX_VERSION;  //  Version Register
};

#endif 
//--//

#if defined(PLATFORM_ARM_SAM9RL64_ANY)
struct AT91_MATRIX {
     static const UINT32 c_Base = AT91C_BASE_MATRIX;

    /****/ volatile UINT32 MATRIX_MCFG0;     //  Master Configuration Register
    /****/ volatile UINT32 MATRIX_MCFG1;     //  Master Configuration Register
    /****/ volatile UINT32 MATRIX_MCFG2;     //  Master Configuration Register
    /****/ volatile UINT32 MATRIX_MCFG3;     //  Master Configuration Register
    /****/ volatile UINT32 MATRIX_MCFG4;     //  Master Configuration Register
    /****/ volatile UINT32 MATRIX_MCFG5;     //  Master Configuration Register
    
    /****/ volatile UINT32 Reserved0[10];    // 
    /****/ volatile UINT32 MATRIX_SCFG0;    //  Slave Configuration Register 0
    static const    UINT32 MATRIX_SLOT_CYCLE                  = (0xFF <<  0); // (MATRIX) Maximum Number of Allowed Cycles for a Burst
    static const    UINT32 MATRIX_DEFMSTR_TYPE                = (0x3UL << 16); // (MATRIX) Default Master Type
    static const    UINT32  MATRIX_DEFMSTR_TYPE_NO_DEFMSTR    = (0x0UL << 16); // (MATRIX) No Default Master. At the end of current slave access, if no other master request is pending, the slave is deconnected from all masters. This results in having a one cycle latency for the first transfer of a burst.
    static const    UINT32  MATRIX_DEFMSTR_TYPE_LAST_DEFMSTR  = (0x1UL << 16); // (MATRIX) Last Default Master. At the end of current slave access, if no other master request is pending, the slave stay connected with the last master having accessed it. This results in not having the one cycle latency when the last master re-trying access on the slave.
    static const    UINT32  MATRIX_DEFMSTR_TYPE_FIXED_DEFMSTR = (0x2UL << 16); // (MATRIX) Fixed Default Master. At the end of current slave access, if no other master request is pending, the slave connects with fixed which number is in FIXED_DEFMSTR field. This results in not having the one cycle latency when the fixed master re-trying access on the slave.
    static const    UINT32 MATRIX_FIXED_DEFMSTR0              = (0x7UL << 18); // (MATRIX) Fixed Index of Default Master
    static const    UINT32  MATRIX_FIXED_DEFMSTR0_ARM926I     = (0x0UL << 18); // (MATRIX) ARM926EJ-S Instruction Master is Default Master
    static const    UINT32  MATRIX_FIXED_DEFMSTR0_ARM926D     = (0x1UL << 18); // (MATRIX) ARM926EJ-S Data Master is Default Master
    static const    UINT32  MATRIX_FIXED_DEFMSTR0_HPDC3       = (0x2UL << 18); // (MATRIX) HPDC3 Master is Default Master
    static const    UINT32  MATRIX_FIXED_DEFMSTR0_LCDC        = (0x3UL << 18); // (MATRIX) LCDC Master is Default Master
    static const    UINT32  MATRIX_FIXED_DEFMSTR0_UHP         = (0x4UL << 18); // (MATRIX) UHP Master is Default Master

    /****/ volatile UINT32 MATRIX_SCFG1;    //  Slave Configuration Register 1
    static const    UINT32 MATRIX_FIXED_DEFMSTR1              = (0x7UL << 18); // (MATRIX) Fixed Index of Default Master
    static const    UINT32  MATRIX_FIXED_DEFMSTR1_ARM926I     = (0x0UL << 18); // (MATRIX) ARM926EJ-S Instruction Master is Default Master
    static const    UINT32  MATRIX_FIXED_DEFMSTR1_ARM926D     = (0x1UL << 18); // (MATRIX) ARM926EJ-S Data Master is Default Master
    static const    UINT32  MATRIX_FIXED_DEFMSTR1_HPDC3       = (0x2UL << 18); // (MATRIX) HPDC3 Master is Default Master
    static const    UINT32  MATRIX_FIXED_DEFMSTR1_LCDC        = (0x3UL << 18); // (MATRIX) LCDC Master is Default Master
    static const    UINT32  MATRIX_FIXED_DEFMSTR1_UHP         = (0x4UL << 18); // (MATRIX) UHP Master is Default Master

    /****/ volatile UINT32 MATRIX_SCFG2;    //  Slave Configuration Register 2
    static const    UINT32 MATRIX_FIXED_DEFMSTR2              = (0x1UL << 18); // (MATRIX) Fixed Index of Default Master
    static const    UINT32  MATRIX_FIXED_DEFMSTR2_ARM926I     = (0x0UL << 18); // (MATRIX) ARM926EJ-S Instruction Master is Default Master
    static const    UINT32  MATRIX_FIXED_DEFMSTR2_ARM926D     = (0x1UL << 18); // (MATRIX) ARM926EJ-S Data Master is Default Master

    /****/ volatile UINT32 MATRIX_SCFG3;    //  Slave Configuration Register 3
    static const    UINT32 MATRIX_FIXED_DEFMSTR3              = (0x7UL << 18); // (MATRIX) Fixed Index of Default Master
    static const    UINT32  MATRIX_FIXED_DEFMSTR3_ARM926I     = (0x0UL << 18); // (MATRIX) ARM926EJ-S Instruction Master is Default Master
    static const    UINT32  MATRIX_FIXED_DEFMSTR3_ARM926D     = (0x1UL << 18); // (MATRIX) ARM926EJ-S Data Master is Default Master
    static const    UINT32  MATRIX_FIXED_DEFMSTR3_HPDC3       = (0x2UL << 18); // (MATRIX) HPDC3 Master is Default Master
    static const    UINT32  MATRIX_FIXED_DEFMSTR3_LCDC        = (0x3UL << 18); // (MATRIX) LCDC Master is Default Master
    static const    UINT32  MATRIX_FIXED_DEFMSTR3_UHP         = (0x4UL << 18); // (MATRIX) UHP Master is Default Master

    /****/ volatile UINT32 MATRIX_SCFG4;    //  Slave Configuration Register 4
    static const    UINT32 MATRIX_FIXED_DEFMSTR4              = (0x3UL << 18); // (MATRIX) Fixed Index of Default Master
    static const    UINT32  MATRIX_FIXED_DEFMSTR4_ARM926I     = (0x0UL << 18); // (MATRIX) ARM926EJ-S Instruction Master is Default Master
    static const    UINT32  MATRIX_FIXED_DEFMSTR4_ARM926D     = (0x1UL << 18); // (MATRIX) ARM926EJ-S Data Master is Default Master
    static const    UINT32  MATRIX_FIXED_DEFMSTR4_HPDC3       = (0x2UL << 18); // (MATRIX) HPDC3 Master is Default Master

    /****/ volatile UINT32 MATRIX_SCFG5;    //  Slave Configuration Register 5
    
    /****/ volatile UINT32 Reserved1[10];    // 
    
    /****/ volatile UINT32 MATRIX_PRAS0;    //  Priority Register A for Slave 0
    
    /****/ volatile UINT32 Reserved2[1];
    
    /****/ volatile UINT32 MATRIX_PRAS1;    //  Priority Register A for Slave 1
    
    /****/ volatile UINT32 Reserved3[1];
    
    /****/ volatile UINT32 MATRIX_PRAS2;    //  Priority Register A for Slave 2
    
    /****/ volatile UINT32 Reserved4[1];
    
    /****/ volatile UINT32 MATRIX_PRAS3;    //  Priority Register A for Slave 3
    
    /****/ volatile UINT32 Reserved5[1];
    
    /****/ volatile UINT32 MATRIX_PRAS4;    //  Priority Register A for Slave 4
    
    /****/ volatile UINT32 Reserved6[1];
    
    /****/ volatile UINT32 MATRIX_PRAS5;    //  Priority Register A for Slave 5

    /****/ volatile UINT32 Reserved7[21];
    
    /****/ volatile UINT32 MATRIX_MRCR;    //  Master Remap Control Register
    
    /****/ volatile UINT32 Reserved8[4];

    /****/ volatile UINT32 MATRIX_TCR;    //  Bus Marix TCM Configuration Register
    
    /****/ volatile UINT32 Reserved9[2];
    
    /****/ volatile UINT32 MATRIX_EBICSA;   //  Slave 3 Special Function Register
    static const    UINT32 MATRIX_CS1A                = (0x1 <<  1); // (MATRIX) Chip Select 1 Assignment
    static const    UINT32  MATRIX_CS1A_SMC       = (0x0 <<  1); // (MATRIX) Chip Select 1 is assigned to the Static Memory Controller.
    static const    UINT32  MATRIX_CS1A_SDRAMC    = (0x1 <<  1); // (MATRIX) Chip Select 1 is assigned to the SDRAM Controller.
    static const    UINT32 MATRIX_CS3A                = (0x1 <<  3); // (MATRIX) Chip Select 3 Assignment
    static const    UINT32  MATRIX_CS3A_SMC       = (0x0 <<  3); // (MATRIX) Chip Select 3 is only assigned to the Static Memory Controller and NCS3 behaves as defined by the SMC.
    static const    UINT32  MATRIX_CS3A_SM            = (0x1 <<  3); // (MATRIX) Chip Select 3 is assigned to the Static Memory Controller and the SmartMedia Logic is activated.
    static const    UINT32 MATRIX_CS4A                = (0x1 <<  4); // (MATRIX) Chip Select 4 Assignment
    static const    UINT32  MATRIX_CS4A_SMC       = (0x0 <<  4); // (MATRIX) Chip Select 4 is only assigned to the Static Memory Controller and NCS4 behaves as defined by the SMC.
    static const    UINT32  MATRIX_CS4A_CF            = (0x1 <<  4); // (MATRIX) Chip Select 4 is assigned to the Static Memory Controller and the CompactFlash Logic = (first slot); is activated.
    static const    UINT32 MATRIX_CS5A                = (0x1 <<  5); // (MATRIX) Chip Select 5 Assignment
    static const    UINT32  MATRIX_CS5A_SMC       = (0x0 <<  5); // (MATRIX) Chip Select 5 is only assigned to the Static Memory Controller and NCS5 behaves as defined by the SMC
    static const    UINT32  MATRIX_CS5A_CF            = (0x1 <<  5); // (MATRIX) Chip Select 5 is assigned to the Static Memory Controller and the CompactFlash Logic = (second slot); is activated.
    static const    UINT32 MATRIX_DBPUC           = (0x1 <<  8); // (MATRIX) Data Bus Pull-up Configuration

    
};

#endif


//--//

// common for both SAM9261_ek and SAM9RL64

#if defined(PLATFORM_ARM_SAM9261_ANY)  || defined(PLATFORM_ARM_SAM9RL64_ANY)
struct AT91_LCDC {

     static const UINT32 c_Base = AT91C_BASE_LCDC;

    /****/ volatile UINT32 LCDC_BA1;        // DMA Base Address Register 1

    /****/ volatile UINT32 LCDC_BA2;        // DMA Base Address Register 2

    /****/ volatile UINT32 LCDC_FRMP1;      // DMA Frame Pointer Register 1
    static const    UINT32 LCDC_FRMPT1     = ((UINT32) 0x3FFFFF <<  0); // (LCDC) Frame Pointer Address 1

    /****/ volatile UINT32 LCDC_FRMP2;      // DMA Frame Pointer Register 2
    static const    UINT32 LCDC_FRMPT2     = ((UINT32) 0x1FFFFF <<  0); // (LCDC) Frame Pointer Address 2
    
    /****/ volatile UINT32 LCDC_FRMA1;      // DMA Frame Address Register 1

    /****/ volatile UINT32 LCDC_FRMA2;      // DMA Frame Address Register 2

    /****/ volatile UINT32 LCDC_FRMCFG;     // DMA Frame Configuration Register
    static const    UINT32 LCDC_FRSIZE     = ((UINT32) 0x3FFFFF <<  0); // (LCDC) FRAME SIZE
    static const    UINT32 LCDC_BLENGTH    = ((UINT32) 0xFUL << 24); // (LCDC) BURST LENGTH
    
    /****/ volatile UINT32 LCDC_DMACON;     // DMA Control Register
    static const    UINT32 LCDC_DMAEN      = ((UINT32) 0x1 <<  0); // (LCDC) DAM Enable
    static const    UINT32 LCDC_DMARST     = ((UINT32) 0x1 <<  1); // (LCDC) DMA Reset (WO)
    static const    UINT32 LCDC_DMABUSY    = ((UINT32) 0x1 <<  2); // (LCDC) DMA Reset (WO)

    /****/ volatile UINT32 LCDC_DMA2DCFG;   // DMA 2D addressing configuration
    static const    UINT32 LCDC_ADDRINC    = ((UINT32) 0xFFFF <<  0); // (LCDC) Number of 32b words that the DMA must jump when going to the next line
    static const    UINT32 LCDC_PIXELOFF   = ((UINT32) 0x1FUL << 24); // (LCDC) Offset (in bits) of the first pixel of the screen in the memory word which contain it

    /****/ volatile UINT32 Reserved0[503]; 

    /****/ volatile UINT32 LCDC_LCDCON1;    // LCD Control 1 Register
    static const    UINT32 LCDC_BYPASS     = ((UINT32) 0x1 <<  0); // (LCDC) Bypass lcd_pccklk divider
    static const    UINT32 LCDC_CLKVAL     = ((UINT32) 0x1FFUL << 12); // (LCDC) 9-bit Divider for pixel clock frequency
    static const    UINT32 LCDC_LINCNT     = ((UINT32) 0x7FFUL << 21); // (LCDC) Line Counter (RO)

    /****/ volatile UINT32 LCDC_LCDCON2;    // LCD Control 2 Register
    static const    UINT32 LCDC_DISTYPE    = ((UINT32) 0x3 <<  0); // (LCDC) Display Type
    static const    UINT32  LCDC_DISTYPE_STNMONO              = ((UINT32) 0x0); // (LCDC) STN Mono
    static const    UINT32  LCDC_DISTYPE_STNCOLOR             = ((UINT32) 0x1); // (LCDC) STN Color
    static const    UINT32  LCDC_DISTYPE_TFT                  = ((UINT32) 0x2); // (LCDC) TFT
    static const    UINT32 LCDC_SCANMOD    = ((UINT32) 0x1 <<  2); // (LCDC) Scan Mode
    static const    UINT32  LCDC_SCANMOD_SINGLESCAN           = ((UINT32) 0x0 <<  2); // (LCDC) Single Scan
    static const    UINT32  LCDC_SCANMOD_DUALSCAN             = ((UINT32) 0x1 <<  2); // (LCDC) Dual Scan
    static const    UINT32 LCDC_IFWIDTH    = ((UINT32) 0x3 <<  3);// (LCDC) Interface Width
    static const    UINT32  LCDC_IFWIDTH_FOURBITSWIDTH        = ((UINT32) 0x0 <<  3); // (LCDC) 4 Bits
    static const    UINT32  LCDC_IFWIDTH_EIGTHBITSWIDTH       = ((UINT32) 0x1 <<  3); // (LCDC) 8 Bits
    static const    UINT32  LCDC_IFWIDTH_SIXTEENBITSWIDTH     = ((UINT32) 0x2 <<  3); // (LCDC) 16 Bits
    static const    UINT32 LCDC_PIXELSIZE  = ((UINT32) 0x7 <<  5); // (LCDC) Bits per pixel
    static const    UINT32  LCDC_PIXELSIZE_ONEBITSPERPIXEL      = ((UINT32) 0x0 <<  5); // (LCDC) 1 Bits
    static const    UINT32  LCDC_PIXELSIZE_TWOBITSPERPIXEL      = ((UINT32) 0x1 <<  5); // (LCDC) 2 Bits
    static const    UINT32  LCDC_PIXELSIZE_FOURBITSPERPIXEL     = ((UINT32) 0x2 <<  5); // (LCDC) 4 Bits
    static const    UINT32  LCDC_PIXELSIZE_EIGTHBITSPERPIXEL    = ((UINT32) 0x3 <<  5); // (LCDC) 8 Bits
    static const    UINT32  LCDC_PIXELSIZE_SIXTEENBITSPERPIXEL  = ((UINT32) 0x4 <<  5); // (LCDC) 16 Bits
    static const    UINT32  LCDC_PIXELSIZE_TWENTYFOURBITSPERPIXEL = ((UINT32) 0x5 <<  5); // (LCDC) 24 Bits
    static const    UINT32 LCDC_INVVD      = ((UINT32) 0x1 <<  8); // (LCDC) lcd datas polarity
    static const    UINT32  LCDC_INVVD_NORMALPOL            = ((UINT32) 0x0 <<  8); // (LCDC) Normal Polarity
    static const    UINT32  LCDC_INVVD_INVERTEDPOL          = ((UINT32) 0x1 <<  8); // (LCDC) Inverted Polarity
    static const    UINT32 LCDC_INVFRAME   = ((UINT32) 0x1 <<  9); // (LCDC) lcd vsync polarity
    static const    UINT32  LCDC_INVFRAME_NORMALPOL            = ((UINT32) 0x0 <<  9); // (LCDC) Normal Polarity
    static const    UINT32  LCDC_INVFRAME_INVERTEDPOL          = ((UINT32) 0x1 <<  9); // (LCDC) Inverted Polarity
    static const    UINT32 LCDC_INVLINE    = ((UINT32) 0x1 << 10); // (LCDC) lcd hsync polarity
    static const    UINT32  LCDC_INVLINE_NORMALPOL            = ((UINT32) 0x0 << 10); // (LCDC) Normal Polarity
    static const    UINT32  LCDC_INVLINE_INVERTEDPOL          = ((UINT32) 0x1 << 10); // (LCDC) Inverted Polarity
    static const    UINT32 LCDC_INVCLK     = ((UINT32) 0x1 << 11); // (LCDC) lcd pclk polarity
    static const    UINT32  LCDC_INVCLK_NORMALPOL            = ((UINT32) 0x0 << 11); // (LCDC) Normal Polarity
    static const    UINT32  LCDC_INVCLK_INVERTEDPOL          = ((UINT32) 0x1 << 11); // (LCDC) Inverted Polarity
    static const    UINT32 LCDC_INVDVAL    = ((UINT32) 0x1 << 12); // (LCDC) lcd dval polarity
    static const    UINT32  LCDC_INVDVAL_NORMALPOL            = ((UINT32) 0x0 << 12); // (LCDC) Normal Polarity
    static const    UINT32  LCDC_INVDVAL_INVERTEDPOL          = ((UINT32) 0x1 << 12); // (LCDC) Inverted Polarity
    static const    UINT32 LCDC_CLKMOD     = ((UINT32) 0x1 << 15); // (LCDC) lcd pclk Mode
    static const    UINT32  LCDC_CLKMOD_ACTIVEONLYDISP       = ((UINT32) 0x0 << 15); // (LCDC) Active during display period
    static const    UINT32  LCDC_CLKMOD_ALWAYSACTIVE         = ((UINT32) 0x1 << 15); // (LCDC) Always Active
    static const    UINT32 LCDC_MEMOR      = ((UINT32) 0x1 << 31); // (LCDC) lcd pclk Mode
    static const    UINT32  LCDC_MEMOR_BIGIND               = ((UINT32) 0x0 << 31); // (LCDC) Big Endian
    static const    UINT32  LCDC_MEMOR_LITTLEIND            = ((UINT32) 0x1 << 31); // (LCDC) Little Endian

    /****/ volatile UINT32 LCDC_TIM1;       // LCD Timing Config 1 Register
    static const    UINT32 LCDC_VFP        = ((UINT32) 0xFF <<  0); // (LCDC) Vertical Front Porch
    static const    UINT32 LCDC_VBP        = ((UINT32) 0xFF <<  8); // (LCDC) Vertical Back Porch
    static const    UINT32 LCDC_VPW        = ((UINT32) 0x3F << 16); // (LCDC) Vertical Synchronization Pulse Width
    static const    UINT32 LCDC_VHDLY      = ((UINT32) 0xF << 24); // (LCDC) Vertical to Horizontal Delay

    /****/ volatile UINT32 LCDC_TIM2;       // LCD Timing Config 2 Register
    static const    UINT32 LCDC_HBP        = ((UINT32) 0xFF <<  0); // (LCDC) Horizontal Back Porch
    static const    UINT32 LCDC_HPW        = ((UINT32) 0x3F <<  8); // (LCDC) Horizontal Synchronization Pulse Width
    static const    UINT32 LCDC_HFP        = ((UINT32) 0x3FF << 22); // (LCDC) Horizontal Front Porch

    /****/ volatile UINT32 LCDC_LCDFRCFG;   // LCD Frame Config Register
    static const    UINT32 LCDC_LINEVAL    = ((UINT32) 0x7FF <<  0); // (LCDC) Vertical Size of LCD Module
    static const    UINT32 LCDC_HOZVAL     = ((UINT32) 0x7FF << 21); // (LCDC) Horizontal Size of LCD Module

    /****/ volatile UINT32 LCDC_FIFO;       // LCD FIFO Register
    static const    UINT32 LCDC_FIFOTH     = ((UINT32) 0xFFFF <<  0); // (LCDC) FIFO Threshold

    /****/ volatile UINT32 LCDC_MVAL;       // LCD Mode Toggle Rate Value Register
    static const    UINT32 LCDC_MVALUE     = ((UINT32) 0xFF <<  0); // (LCDC) Toggle Rate Value
    static const    UINT32 LCDC_MMODE      = ((UINT32) 0x1 << 31); // (LCDC) Toggle Rate Sel
    static const    UINT32  LCDC_MMODE_EACHFRAME            = ((UINT32) 0x0 << 31); // (LCDC) Each Frame
    static const    UINT32  LCDC_MMODE_MVALDEFINED          = ((UINT32) 0x1 << 31); // (LCDC) Defined by MVAL

    /****/ volatile UINT32 LCDC_DP1_2;      // Dithering Pattern DP1_2 Register
    static const    UINT32 LCDC_DP1_2_FIELD = ((UINT32) 0xFF <<  0); // (LCDC) Ratio

    /****/ volatile UINT32 LCDC_DP4_7;      // Dithering Pattern DP4_7 Register
    static const    UINT32 LCDC_DP4_7_FIELD = ((UINT32) 0xFFFFFFF <<  0); // (LCDC) Ratio

    /****/ volatile UINT32 LCDC_DP3_5;      // Dithering Pattern DP3_5 Register
    static const    UINT32 LCDC_DP3_5_FIELD = ((UINT32) 0xFFFFF <<  0); // (LCDC) Ratio

    /****/ volatile UINT32 LCDC_DP2_3;      // Dithering Pattern DP2_3 Register
    static const    UINT32 LCDC_DP2_3_FIELD = ((UINT32) 0xFFF <<  0); // (LCDC) Ratio

    /****/ volatile UINT32 LCDC_DP5_7;      // Dithering Pattern DP5_7 Register
    static const    UINT32 LCDC_DP5_7_FIELD = ((UINT32) 0xFFFFFFF <<  0); // (LCDC) Ratio

    /****/ volatile UINT32 LCDC_DP3_4;      // Dithering Pattern DP3_4 Register
    static const    UINT32 LCDC_DP3_4_FIELD = ((UINT32) 0xFFFF <<  0); // (LCDC) Ratio

    /****/ volatile UINT32 LCDC_DP4_5;      // Dithering Pattern DP4_5 Register
    static const    UINT32 LCDC_DP4_5_FIELD = ((UINT32) 0xFFFFF <<  0); // (LCDC) Ratio

    /****/ volatile UINT32 LCDC_DP6_7;      // Dithering Pattern DP6_7 Register
    static const    UINT32 LCDC_DP6_7_FIELD = ((UINT32) 0xFFFFFFF <<  0); // (LCDC) Ratio

    /****/ volatile UINT32 LCDC_PWRCON;     // Power Control Register
    static const    UINT32 LCDC_PWR        = ((UINT32) 0x1 <<  0); // (LCDC) LCD Module Power Control
    static const    UINT32 LCDC_GUARDT     = ((UINT32) 0x7F <<  1); // (LCDC) Delay in Frame Period
    static const    UINT32 LCDC_BUSY       = ((UINT32) 0x1UL << 31); // (LCDC) Read Only : 1 indicates that LCDC is busy
    static const    UINT32  LCDC_BUSY_LCDNOTBUSY           = ((UINT32) 0x0 << 31); // (LCDC) LCD is Not Busy
    static const    UINT32  LCDC_BUSY_LCDBUSY              = ((UINT32) 0x1 << 31); // (LCDC) LCD is Busy

    /****/ volatile UINT32 LCDC_CTRSTCON;   // Contrast Control Register
    static const    UINT32 LCDC_PS         = ((UINT32) 0x3 <<  0); // (LCDC) LCD Contrast Counter Prescaler
    static const    UINT32  LCDC_PS_NOTDIVIDED           = ((UINT32) 0x0); // (LCDC) Counter Freq is System Freq.
    static const    UINT32  LCDC_PS_DIVIDEDBYTWO         = ((UINT32) 0x1); // (LCDC) Counter Freq is System Freq divided by 2.
    static const    UINT32  LCDC_PS_DIVIDEDBYFOUR        = ((UINT32) 0x2); // (LCDC) Counter Freq is System Freq divided by 4.
    static const    UINT32  LCDC_PS_DIVIDEDBYEIGHT       = ((UINT32) 0x3); // (LCDC) Counter Freq is System Freq divided by 8.
    static const    UINT32 LCDC_POL        = ((UINT32) 0x1 <<  2); // (LCDC) Polarity of output Pulse
    static const    UINT32  LCDC_POL_NEGATIVEPULSE        = ((UINT32) 0x0 <<  2); // (LCDC) Negative Pulse
    static const    UINT32  LCDC_POL_POSITIVEPULSE        = ((UINT32) 0x1 <<  2); // (LCDC) Positive Pulse
    static const    UINT32 LCDC_ENA        = ((UINT32) 0x1 <<  3); // (LCDC) PWM generator Control
    static const    UINT32  LCDC_ENA_PWMGEMDISABLED       = ((UINT32) 0x0 <<  3); // (LCDC) PWM Generator Disabled
    static const    UINT32  LCDC_ENA_PWMGEMENABLED        = ((UINT32) 0x1 <<  3); // (LCDC) PWM Generator Disabled

    /****/ volatile UINT32 LCDC_CTRSTVAL;   // Contrast Value Register
    static const    UINT32 LCDC_CVAL       = ((UINT32) 0xFF <<  0); // (LCDC) PWM Compare Value

    /****/ volatile UINT32 LCDC_IER;        // Interrupt Enable Register
    static const    UINT32 LCDC_LNI        = ((UINT32) 0x1 <<  0); // (LCDC) Line Interrupt
    static const    UINT32 LCDC_LSTLNI     = ((UINT32) 0x1 <<  1); // (LCDC) Last Line Interrupt
    static const    UINT32 LCDC_EOFI       = ((UINT32) 0x1 <<  2); // (LCDC) End Of Frame Interrupt
    static const    UINT32 LCDC_UFLWI      = ((UINT32) 0x1 <<  4); // (LCDC) FIFO Underflow Interrupt
    static const    UINT32 LCDC_OWRI       = ((UINT32) 0x1 <<  5); // (LCDC) Over Write Interrupt
    static const    UINT32 LCDC_MERI       = ((UINT32) 0x1 <<  6); // (LCDC) Memory Error  Interrupt

    /****/ volatile UINT32 LCDC_IDR;        // Interrupt Disable Register
    /****/ volatile UINT32 LCDC_IMR;        // Interrupt Mask Register
    /****/ volatile UINT32 LCDC_ISR;        // Interrupt Enable Register
    /****/ volatile UINT32 LCDC_ICR;        // Interrupt Clear Register
    /****/ volatile UINT32 LCDC_GPR;        // General Purpose Register
    static const    UINT32 LCDC_GPRBUS     = ((UINT32) 0xFF <<  0); // (LCDC) 8 bits available
    /****/ volatile UINT32 LCDC_ITR;        // Interrupts Test Register
    /****/ volatile UINT32 LCDC_IRR;        // Interrupts Raw Status Register
    /****/ volatile UINT32 Reserved1[230];
    /****/ volatile UINT32 LCDC_LUT_ENTRY[256];     // LUT Entries Register
};


struct AT91_SMC {
     static const UINT32 c_Base = AT91C_BASE_SMC;

    /****/ volatile UINT32 SMC_SETUP0;  //  Setup Register for CS 0

    /****/ volatile UINT32 SMC_PULSE0;  //  Pulse Register for CS 0

    /****/ volatile UINT32 SMC_CYCLE0;  //  Cycle Register for CS 0

    /****/ volatile UINT32 SMC_CTRL0;   //  Control Register for CS 0
    static const    UINT32 SMC_READMODE    = (0x1 <<  0); // (Static Memory Controller) Read Mode
    static const    UINT32 SMC_WRITEMODE   = (0x1 <<  1); // (Static Memory Controller) Write Mode
    static const    UINT32 SMC_NWAITM      = (0x3 <<  5); // (Static Memory Controller) NWAIT Mode
    static const    UINT32  SMC_NWAITM_NWAIT_DISABLE        = (0x0 <<  5); // (Static Memory Controller) External NWAIT disabled.
    static const    UINT32  SMC_NWAITM_NWAIT_ENABLE_FROZEN  = (0x2 <<  5); // (Static Memory Controller) External NWAIT enabled in frozen mode.
    static const    UINT32  SMC_NWAITM_NWAIT_ENABLE_READY   = (0x3 <<  5); // (Static Memory Controller) External NWAIT enabled in ready mode.
    static const    UINT32 SMC_BAT         = (0x1 <<  8); // (Static Memory Controller) Byte Access Type
    static const    UINT32  SMC_BAT_BYTE_SELECT          = (0x0 <<  8); // (Static Memory Controller) Write controled by ncs, nbs0, nbs1, nbs2, nbs3. Read controled by ncs, nrd, nbs0, nbs1, nbs2, nbs3.
    static const    UINT32  SMC_BAT_BYTE_WRITE           = (0x1 <<  8); // (Static Memory Controller) Write controled by ncs, nwe0, nwe1, nwe2, nwe3. Read controled by ncs and nrd.
    static const    UINT32 SMC_DBW         = (0x3 << 12); // (Static Memory Controller) Data Bus Width
    static const    UINT32  SMC_DBW_WIDTH_EIGTH_BITS     = (0x0 << 12); // (Static Memory Controller) 8 bits.
    static const    UINT32  SMC_DBW_WIDTH_SIXTEEN_BITS   = (0x1 << 12); // (Static Memory Controller) 16 bits.
    static const    UINT32  SMC_DBW_WIDTH_THIRTY_TWO_BITS = (0x2 << 12); // (Static Memory Controller) 32 bits.
    static const    UINT32 SMC_TDF         = (0xFUL << 16); // (Static Memory Controller) Data Float Time.
    static const    UINT32 SMC_TDFEN       = (0x1UL << 20); // (Static Memory Controller) TDF Enabled.
    static const    UINT32 SMC_PMEN        = (0x1UL << 24); // (Static Memory Controller) Page Mode Enabled.
    static const    UINT32 SMC_PS          = (0x3UL << 28); // (Static Memory Controller) Page Size
    static const    UINT32  SMC_PS_SIZE_FOUR_BYTES      = (0x0UL << 28); // (Static Memory Controller) 4 bytes.
    static const    UINT32  SMC_PS_SIZE_EIGHT_BYTES     = (0x1UL << 28); // (Static Memory Controller) 8 bytes.
    static const    UINT32  SMC_PS_SIZE_SIXTEEN_BYTES   = (0x2UL << 28); // (Static Memory Controller) 16 bytes.
    static const    UINT32  SMC_PS_SIZE_THIRTY_TWO_BYTES = (0x3UL << 28); // (Static Memory Controller) 32 bytes.

    /****/ volatile UINT32 SMC_SETUP1;  //  Setup Register for CS 1

    /****/ volatile UINT32 SMC_PULSE1;  //  Pulse Register for CS 1

    /****/ volatile UINT32 SMC_CYCLE1;  //  Cycle Register for CS 1

    /****/ volatile UINT32 SMC_CTRL1;   //  Control Register for CS 1

    /****/ volatile UINT32 SMC_SETUP2;  //  Setup Register for CS 2

    /****/ volatile UINT32 SMC_PULSE2;  //  Pulse Register for CS 2

    /****/ volatile UINT32 SMC_CYCLE2;  //  Cycle Register for CS 2

    /****/ volatile UINT32 SMC_CTRL2;   //  Control Register for CS 2

    /****/ volatile UINT32 SMC_SETUP3;  //  Setup Register for CS 3

    /****/ volatile UINT32 SMC_PULSE3;  //  Pulse Register for CS 3

    /****/ volatile UINT32 SMC_CYCLE3;  //  Cycle Register for CS 3

    /****/ volatile UINT32 SMC_CTRL3;   //  Control Register for CS 3

    /****/ volatile UINT32 SMC_SETUP4;  //  Setup Register for CS 4

    /****/ volatile UINT32 SMC_PULSE4;  //  Pulse Register for CS 4

    /****/ volatile UINT32 SMC_CYCLE4;  //  Cycle Register for CS 4

    /****/ volatile UINT32 SMC_CTRL4;   //  Control Register for CS 4

    /****/ volatile UINT32 SMC_SETUP5;  //  Setup Register for CS 5

    /****/ volatile UINT32 SMC_PULSE5;  //  Pulse Register for CS 5

    /****/ volatile UINT32 SMC_CYCLE5;  //  Cycle Register for CS 5

    /****/ volatile UINT32 SMC_CTRL5;   //  Control Register for CS 5

    /****/ volatile UINT32 SMC_SETUP6;  //  Setup Register for CS 6

    /****/ volatile UINT32 SMC_PULSE6;  //  Pulse Register for CS 6

    /****/ volatile UINT32 SMC_CYCLE6;  //  Cycle Register for CS 6

    /****/ volatile UINT32 SMC_CTRL6;   //  Control Register for CS 6

    /****/ volatile UINT32 SMC_SETUP7;  //  Setup Register for CS 7

    /****/ volatile UINT32 SMC_PULSE7;  //  Pulse Register for CS 7

    /****/ volatile UINT32 SMC_CYCLE7;  //  Cycle Register for CS 7

    /****/ volatile UINT32 SMC_CTRL7;   //  Control Register for CS 7
};

//--//
//////////////////////////////////////////////////////////////////////////////
// AT91_SDRAMC
//
struct AT91_SDRAMC {

    static const UINT32 c_Base = AT91C_BASE_SDRAMC;
     
    /****/ volatile UINT32 SDRAMC_MR;   // SDRAM Controller Mode Register
    static const    UINT32 SDRAMC_MODE     = (0xF <<  0); // (SDRAMC); Mode
    static const    UINT32  SDRAMC_MODE_NORMAL_CMD           = (0x0); // (SDRAMC) Normal Mode
    static const    UINT32  SDRAMC_MODE_NOP_CMD              = (0x1); // (SDRAMC) Issue a NOP Command at every access
    static const    UINT32  SDRAMC_MODE_PRCGALL_CMD          = (0x2); // (SDRAMC) Issue a All Banks Precharge Command at every access
    static const    UINT32  SDRAMC_MODE_LMR_CMD              = (0x3); // (SDRAM) Issue a Load Mode Register at every access
    static const    UINT32  SDRAMC_MODE_RFSH_CMD             = (0x4); // (SDRAM) Issue a Refresh
    static const    UINT32  SDRAMC_MODE_EXT_LMR_CMD          = (0x5); // (SDRAM) Issue an Extended Load Mode Register
    static const    UINT32  SDRAMC_MODE_DEEP_CMD             = (0x6); // (SDRAM) Enter Deep Power Mode

    /****/ volatile UINT32 SDRAMC_TR;   // SDRAM Controller Refresh Timer Register
    static const    UINT32 SDRAMC_COUNT    = (0xFFF <<  0); // (SDRAM) Refresh Counter

    /****/ volatile UINT32 SDRAMC_CR;   // SDRAM Controller Configuration Register
    static const    UINT32 SDRAMC_NC       = (0x3 <<  0); // (SDRAM) Number of Column Bits
    static const    UINT32  SDRAMC_NC_8                    = (0x0); // (SDRAM) 8 Bits
    static const    UINT32  SDRAMC_NC_9                    = (0x1); // (SDRAM) 9 Bits
    static const    UINT32  SDRAMC_NC_10                   = (0x2); // (SDRAM) 10 Bits
    static const    UINT32  SDRAMC_NC_11                   = (0x3); // (SDRAM) 11 Bits
    static const    UINT32 SDRAMC_NR       = (0x3 <<  2); // (SDRAM) Number of Row Bits
    static const    UINT32  SDRAMC_NR_11                   = (0x0 <<  2); // (SDRAM) 11 Bits
    static const    UINT32  SDRAMC_NR_12                   = (0x1 <<  2); // (SDRAM) 12 Bits
    static const    UINT32  SDRAMC_NR_13                   = (0x2 <<  2); // (SDRAM) 13 Bits
    static const    UINT32 SDRAMC_NB       = (0x1 <<  4); // (SDRAM) Number of Banks
    static const    UINT32  SDRAMC_NB_2_BANKS              = (0x0 <<  4); // (SDRAM) 2 banks
    static const    UINT32  SDRAMC_NB_4_BANKS              = (0x1 <<  4); // (SDRAM) 4 banks
    static const    UINT32 SDRAMC_CAS      = (0x3 <<  5); // (SDRAM) CAS Latency
    static const    UINT32  SDRAMC_CAS_2                    = (0x2 <<  5); // (SDRAM) 2 cycles
    static const    UINT32  SDRAMC_CAS_3                    = (0x3 <<  5); // (SDRAM) 3 cycles
    static const    UINT32 SDRAMC_DBW      = (0x1 <<  7); // (SDRAM) Data Bus Width
    static const    UINT32  SDRAMC_DBW_32_BITS              = (0x0 <<  7); // (SDRAM) 32 Bits datas bus
    static const    UINT32  SDRAMC_DBW_16_BITS              = (0x1 <<  7); // (SDRAM) 16 Bits datas bus
    static const    UINT32 SDRAMC_TWR      = (0xF <<  8); // (SDRAM) Number of Write Recovery Time Cycles
    static const    UINT32  SDRAMC_TWR_0                    = (0x0 <<  8); // (SDRAM) Value :  0
    static const    UINT32  SDRAMC_TWR_1                    = (0x1 <<  8); // (SDRAM) Value :  1
    static const    UINT32  SDRAMC_TWR_2                    = (0x2 <<  8); // (SDRAM) Value :  2
    static const    UINT32  SDRAMC_TWR_3                    = (0x3 <<  8); // (SDRAM) Value :  3
    static const    UINT32  SDRAMC_TWR_4                    = (0x4 <<  8); // (SDRAM) Value :  4
    static const    UINT32  SDRAMC_TWR_5                    = (0x5 <<  8); // (SDRAM) Value :  5
    static const    UINT32  SDRAMC_TWR_6                    = (0x6 <<  8); // (SDRAM) Value :  6
    static const    UINT32  SDRAMC_TWR_7                    = (0x7 <<  8); // (SDRAM) Value :  7
    static const    UINT32  SDRAMC_TWR_8                    = (0x8 <<  8); // (SDRAM) Value :  8
    static const    UINT32  SDRAMC_TWR_9                    = (0x9 <<  8); // (SDRAM) Value :  9
    static const    UINT32  SDRAMC_TWR_10                   = (0xA <<  8); // (SDRAM) Value : 10
    static const    UINT32  SDRAMC_TWR_11                   = (0xB <<  8); // (SDRAM) Value : 11
    static const    UINT32  SDRAMC_TWR_12                   = (0xC <<  8); // (SDRAM) Value : 12
    static const    UINT32  SDRAMC_TWR_13                   = (0xD <<  8); // (SDRAM) Value : 13
    static const    UINT32  SDRAMC_TWR_14                   = (0xE <<  8); // (SDRAM) Value : 14
    static const    UINT32  SDRAMC_TWR_15                   = (0xF <<  8); // (SDRAM) Value : 15
    static const    UINT32 SDRAMC_TRC      = (0xF << 12); // (SDRAM) Number of RAS Cycle Time Cycles
    static const    UINT32  SDRAMC_TRC_0                    = (0x0 << 12); // (SDRAM) Value :  0
    static const    UINT32  SDRAMC_TRC_1                    = (0x1 << 12); // (SDRAM) Value :  1
    static const    UINT32  SDRAMC_TRC_2                    = (0x2 << 12); // (SDRAM) Value :  2
    static const    UINT32  SDRAMC_TRC_3                    = (0x3 << 12); // (SDRAM) Value :  3
    static const    UINT32  SDRAMC_TRC_4                    = (0x4 << 12); // (SDRAM) Value :  4
    static const    UINT32  SDRAMC_TRC_5                    = (0x5 << 12); // (SDRAM) Value :  5
    static const    UINT32  SDRAMC_TRC_6                    = (0x6 << 12); // (SDRAM) Value :  6
    static const    UINT32  SDRAMC_TRC_7                    = (0x7 << 12); // (SDRAM) Value :  7
    static const    UINT32  SDRAMC_TRC_8                    = (0x8 << 12); // (SDRAM) Value :  8
    static const    UINT32  SDRAMC_TRC_9                    = (0x9 << 12); // (SDRAM) Value :  9
    static const    UINT32  SDRAMC_TRC_10                   = (0xA << 12); // (SDRAM) Value : 10
    static const    UINT32  SDRAMC_TRC_11                   = (0xB << 12); // (SDRAM) Value : 11
    static const    UINT32  SDRAMC_TRC_12                   = (0xC << 12); // (SDRAM) Value : 12
    static const    UINT32  SDRAMC_TRC_13                   = (0xD << 12); // (SDRAM) Value : 13
    static const    UINT32  SDRAMC_TRC_14                   = (0xE << 12); // (SDRAM) Value : 14
    static const    UINT32  SDRAMC_TRC_15                   = (0xF << 12); // (SDRAM) Value : 15
    static const    UINT32 SDRAMC_TRP      = (0xFUL << 16); // (SDRAM) Number of RAS Precharge Time Cycles
    static const    UINT32  SDRAMC_TRP_0                    = (0x0UL << 16); // (SDRAM) Value :  0
    static const    UINT32  SDRAMC_TRP_1                    = (0x1UL << 16); // (SDRAM) Value :  1
    static const    UINT32  SDRAMC_TRP_2                    = (0x2UL << 16); // (SDRAM) Value :  2
    static const    UINT32  SDRAMC_TRP_3                    = (0x3UL << 16); // (SDRAM) Value :  3
    static const    UINT32  SDRAMC_TRP_4                    = (0x4UL << 16); // (SDRAM) Value :  4
    static const    UINT32  SDRAMC_TRP_5                    = (0x5UL << 16); // (SDRAM) Value :  5
    static const    UINT32  SDRAMC_TRP_6                    = (0x6UL << 16); // (SDRAM) Value :  6
    static const    UINT32  SDRAMC_TRP_7                    = (0x7UL << 16); // (SDRAM) Value :  7
    static const    UINT32  SDRAMC_TRP_8                    = (0x8UL << 16); // (SDRAM) Value :  8
    static const    UINT32  SDRAMC_TRP_9                    = (0x9UL << 16); // (SDRAM) Value :  9
    static const    UINT32  SDRAMC_TRP_10                   = (0xAUL << 16); // (SDRAM) Value : 10
    static const    UINT32  SDRAMC_TRP_11                   = (0xBUL << 16); // (SDRAM) Value : 11
    static const    UINT32  SDRAMC_TRP_12                   = (0xCUL << 16); // (SDRAM) Value : 12
    static const    UINT32  SDRAMC_TRP_13                   = (0xDUL << 16); // (SDRAM) Value : 13
    static const    UINT32  SDRAMC_TRP_14                   = (0xEUL << 16); // (SDRAM) Value : 14
    static const    UINT32  SDRAMC_TRP_15                   = (0xFUL << 16); // (SDRAM) Value : 15
    static const    UINT32 SDRAMC_TRCD     = (0xFUL << 20); // (SDRAM) Number of RAS to CAS Delay Cycles
    static const    UINT32  SDRAMC_TRCD_0                    = (0x0UL << 20); // (SDRAM) Value :  0
    static const    UINT32  SDRAMC_TRCD_1                    = (0x1UL << 20); // (SDRAM) Value :  1
    static const    UINT32  SDRAMC_TRCD_2                    = (0x2UL << 20); // (SDRAM) Value :  2
    static const    UINT32  SDRAMC_TRCD_3                    = (0x3UL << 20); // (SDRAM) Value :  3
    static const    UINT32  SDRAMC_TRCD_4                    = (0x4UL << 20); // (SDRAM) Value :  4
    static const    UINT32  SDRAMC_TRCD_5                    = (0x5UL << 20); // (SDRAM) Value :  5
    static const    UINT32  SDRAMC_TRCD_6                    = (0x6UL << 20); // (SDRAM) Value :  6
    static const    UINT32  SDRAMC_TRCD_7                    = (0x7UL << 20); // (SDRAM) Value :  7
    static const    UINT32  SDRAMC_TRCD_8                    = (0x8UL << 20); // (SDRAM) Value :  8
    static const    UINT32  SDRAMC_TRCD_9                    = (0x9UL << 20); // (SDRAM) Value :  9
    static const    UINT32  SDRAMC_TRCD_10                   = (0xAUL << 20); // (SDRAM) Value : 10
    static const    UINT32  SDRAMC_TRCD_11                   = (0xBUL << 20); // (SDRAM) Value : 11
    static const    UINT32  SDRAMC_TRCD_12                   = (0xCUL << 20); // (SDRAM) Value : 12
    static const    UINT32  SDRAMC_TRCD_13                   = (0xDUL << 20); // (SDRAM) Value : 13
    static const    UINT32  SDRAMC_TRCD_14                   = (0xEUL << 20); // (SDRAM) Value : 14
    static const    UINT32  SDRAMC_TRCD_15                   = (0xFUL << 20); // (SDRAM) Value : 15
    static const    UINT32 SDRAMC_TRAS     = (0xFUL << 24); // (SDRAM) Number of RAS Active Time Cycles
    static const    UINT32  SDRAMC_TRAS_0                    = (0x0UL << 24); // (SDRAM) Value :  0
    static const    UINT32  SDRAMC_TRAS_1                    = (0x1UL << 24); // (SDRAM) Value :  1
    static const    UINT32  SDRAMC_TRAS_2                    = (0x2UL << 24); // (SDRAM) Value :  2
    static const    UINT32  SDRAMC_TRAS_3                    = (0x3UL << 24); // (SDRAM) Value :  3
    static const    UINT32  SDRAMC_TRAS_4                    = (0x4UL << 24); // (SDRAM) Value :  4
    static const    UINT32  SDRAMC_TRAS_5                    = (0x5UL << 24); // (SDRAM) Value :  5
    static const    UINT32  SDRAMC_TRAS_6                    = (0x6UL << 24); // (SDRAM) Value :  6
    static const    UINT32  SDRAMC_TRAS_7                    = (0x7UL << 24); // (SDRAM) Value :  7
    static const    UINT32  SDRAMC_TRAS_8                    = (0x8UL << 24); // (SDRAM) Value :  8
    static const    UINT32  SDRAMC_TRAS_9                    = (0x9UL << 24); // (SDRAM) Value :  9
    static const    UINT32  SDRAMC_TRAS_10                   = (0xAUL << 24); // (SDRAM) Value : 10
    static const    UINT32  SDRAMC_TRAS_11                   = (0xBUL << 24); // (SDRAM) Value : 11
    static const    UINT32  SDRAMC_TRAS_12                   = (0xCUL << 24); // (SDRAM) Value : 12
    static const    UINT32  SDRAMC_TRAS_13                   = (0xDUL << 24); // (SDRAM) Value : 13
    static const    UINT32  SDRAMC_TRAS_14                   = (0xEUL << 24); // (SDRAM) Value : 14
    static const    UINT32  SDRAMC_TRAS_15                   = (0xFUL << 24); // (SDRAM) Value : 15
    static const    UINT32 SDRAMC_TXSR     = ((UINT32) 0xFUL << 28); // (SDRAM) Number of Command Recovery Time Cycles
    static const    UINT32  SDRAMC_TXSR_0                    = (0x0ul << 28); // (SDRAM) Value :  0
    static const    UINT32  SDRAMC_TXSR_1                    = (0x1ul << 28); // (SDRAM) Value :  1
    static const    UINT32  SDRAMC_TXSR_2                    = (0x2ul << 28); // (SDRAM) Value :  2
    static const    UINT32  SDRAMC_TXSR_3                    = (0x3ul << 28); // (SDRAM) Value :  3
    static const    UINT32  SDRAMC_TXSR_4                    = (0x4ul << 28); // (SDRAM) Value :  4
    static const    UINT32  SDRAMC_TXSR_5                    = (0x5ul << 28); // (SDRAM) Value :  5
    static const    UINT32  SDRAMC_TXSR_6                    = (0x6ul << 28); // (SDRAM) Value :  6
    static const    UINT32  SDRAMC_TXSR_7                    = (0x7ul << 28); // (SDRAM) Value :  7
    static const    UINT32  SDRAMC_TXSR_8                    = (0x8ul << 28); // (SDRAM) Value :  8
    static const    UINT32  SDRAMC_TXSR_9                    = (0x9ul << 28); // (SDRAM) Value :  9
    static const    UINT32  SDRAMC_TXSR_10                   = (0xAul << 28); // (SDRAM) Value : 10
    static const    UINT32  SDRAMC_TXSR_11                   = (0xBul << 28); // (SDRAM) Value : 11
    static const    UINT32  SDRAMC_TXSR_12                   = (0xCul << 28); // (SDRAM) Value : 12
    static const    UINT32  SDRAMC_TXSR_13                   = (0xDul << 28); // (SDRAM) Value : 13
    static const    UINT32  SDRAMC_TXSR_14                   = (0xEul << 28); // (SDRAM) Value : 14
    static const    UINT32  SDRAMC_TXSR_15                   = (0xFul << 28); // (SDRAM) Value : 15

    /****/ volatile UINT32 SDRAMC_HSR;  // SDRAM Controller High Speed Register
    static const    UINT32 SDRAMC_DA       = (0x1 <<  0); // (SDRAM) Decode Cycle Enable Bit
    static const    UINT32  SDRAMC_DA_DISABLE              = (0x0); // (SDRAM) Disable Decode Cycle
    static const    UINT32  SDRAMC_DA_ENABLE               = (0x1); // (SDRAM) Enable Decode Cycle

    /****/ volatile UINT32 SDRAMC_LPR;  // SDRAM Controller Low Power Register
    static const    UINT32 SDRAMC_LPCB     = (0x3 <<  0); // (SDRAM) Low-power Configurations
    static const    UINT32  SDRAMC_LPCB_DISABLE              = (0x0); // (SDRAM) Disable Low Power Features
    static const    UINT32  SDRAMC_LPCB_SELF_REFRESH         = (0x1); // (SDRAM) Enable SELF_REFRESH
    static const    UINT32  SDRAMC_LPCB_POWER_DOWN           = (0x2); // (SDRAM) Enable POWER_DOWN
    static const    UINT32  SDRAMC_LPCB_DEEP_POWER_DOWN      = (0x3); // (SDRAM) Enable DEEP_POWER_DOWN
    static const    UINT32 SDRAMC_PASR     = (0x7 <<  4); // (SDRAM) Partial Array Self Refresh = (only for Low Power SDRAM);
    static const    UINT32 SDRAMC_TCSR     = (0x3 <<  8); // (SDRAM) Temperature Compensated Self Refresh = (only for Low Power SDRAM);
    static const    UINT32 SDRAMC_DS       = (0x3 << 10); // (SDRAM) Drive Strenght = (only for Low Power SDRAM);
    static const    UINT32 SDRAMC_TIMEOUT  = (0x3 << 12); // (SDRAM) Time to define when Low Power Mode is enabled
    static const    UINT32  SDRAMC_TIMEOUT_0_CLK_CYCLES         = (0x0 << 12); // (SDRAM) Activate SDRAM Low Power Mode Immediately
    static const    UINT32  SDRAMC_TIMEOUT_64_CLK_CYCLES        = (0x1 << 12); // (SDRAM) Activate SDRAM Low Power Mode after 64 clock cycles after the end of the last transfer
    static const    UINT32  SDRAMC_TIMEOUT_128_CLK_CYCLES       = (0x2 << 12); // (SDRAM) Activate SDRAM Low Power Mode after 64 clock cycles after the end of the last transfer

    /****/ volatile UINT32 SDRAMC_IER;  // SDRAM Controller Interrupt Enable Register
    static const    UINT32 SDRAMC_RES      = (0x1 <<  0); // (SDRAM) Refresh Error Status
    
    /****/ volatile UINT32 SDRAMC_IDR;  // SDRAM Controller Interrupt Disable Register
    /****/ volatile UINT32 SDRAMC_IMR;  // SDRAM Controller Interrupt Mask Register
    /****/ volatile UINT32 SDRAMC_ISR;  // SDRAM Controller Interrupt Mask Register
    /****/ volatile UINT32 SDRAMC_MDR;  // SDRAM Memory Device Register
    static const    UINT32 SDRAMC_MD       = (0x3 <<  0); // (SDRAM) Memory Device Type
    static const    UINT32  SDRAMC_MD_SDRAM                = (0x0); // (SDRAM) SDRAM Mode
    static const    UINT32  SDRAMC_MD_LOW_POWER_SDRAM      = (0x1); // (SDRAM) SDRAM Low Power Mode

};
#endif

/**********************************************************************************************************************/
/**********************************************************************************************************************/

struct AT91
{
/*
    static const UINT32 c_UncachableMask = 0x80000000;

    static AT91_EIM     & EIM()             { return *(AT91_EIM     *)(size_t)(      AT91_EIM     ::c_Base                                      ); }
    static AT91_SC      & SC()              { return *(AT91_SC      *)(size_t)(      AT91_SC      ::c_Base                                      ); }
    static AT91_CMU     & CMU  (         )  { return *(AT91_CMU     *)(size_t)(      AT91_CMU     ::c_Base                                      ); }
    static AT91_PWM     & PWM()             { return *(AT91_PWM     *)(size_t)(      AT91_PWM     ::c_Base                                      ); }
    static AT91_DMA     & DMA()             { return *(AT91_DMA     *)(size_t)(      AT91_DMA     ::c_Base                                      ); }
*/
    static AT91_I2C     & I2C()             { return *(AT91_I2C     *)(size_t)(      AT91_I2C     ::c_Base                                      ); }
    static AT91_AIC     & AIC()             { return *(AT91_AIC     *)(size_t)(AT91_AIC     ::c_Base                                      ); }
    static AT91_PIO     & PIO( int sel )    { return *(AT91_PIO     *)(size_t)(AT91_PIO     ::c_Base + AT91_PIO::c_Base_Offset * sel ); }
    static AT91_PMC     & PMC()             { return *(AT91_PMC     *)(size_t)(AT91_PMC     ::c_Base                                      ); }
    static AT91_SPI     & SPI( int sel )    { if ( sel==0 ) return *(AT91_SPI     *)(size_t)(AT91_SPI::c_Base_1);
                                                  else      return *(AT91_SPI     *)(size_t)(AT91_SPI::c_Base_2);                              }

    static AT91_TC      & TIMER( int sel );
    static AT91_WATCHDOG& WTDG()            { return *(AT91_WATCHDOG*)(size_t)(AT91_WATCHDOG::c_Base                                      ); }

    static AT91_USART   & USART( int sel )  { if( sel == 0) return *(AT91_USART   *)(size_t)(AT91_USART::c_Base_dbg);
                                                  else      return *(AT91_USART   *)(size_t)(AT91_USART::c_Base + ((sel - 1) * 0x4000));                        }

    static AT91_UDP     & UDP()             { return *(AT91_UDP     *)(size_t)(AT91_UDP     ::c_Base                                      ); }

#if defined(PLATFORM_ARM_SAM9261_ANY) || defined(PLATFORM_ARM_SAM9RL64_ANY)
    static AT91_LCDC    & LCDC()            { return *(AT91_LCDC    *)(size_t)(AT91_LCDC    ::c_Base                                      ); }    

    static AT91_SDRAMC  & SDRAMC()          { return *(AT91_SDRAMC  *)(size_t)(AT91_SDRAMC  ::c_Base                                      ); }
    static AT91_SMC     & SMCTRL()          { return *(AT91_SMC     *)(size_t)(AT91_SMC     ::c_Base                                      ); }
    static AT91_MATRIX  & MATRIX()          { return *(AT91_MATRIX  *)(size_t)(AT91_MATRIX  ::c_Base                                      ); }      
#endif
    //--//

};

//
//
//
// LAYOUT SECTION
/////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
// AT91SAM_AIC  driver
//
struct AT91_AIC_Driver
{
    static const UINT32 c_VECTORING_GUARD = 32;
    static const UINT32 c_MaxInterruptIndex = 32;

    //--//
    struct IRQ_VECTORING
    {
        UINT32       Priority;
        HAL_CALLBACK Handler;
    };

    //--//
    static IRQ_VECTORING s_IsrTable[];

    //--//
    static void Initialize();
    static BOOL ActivateInterrupt(UINT32 Irq_Index, BOOL Fast, HAL_CALLBACK_FPN ISR, void* ISR_Param);
    static BOOL DeactivateInterrupt(UINT32 Irq_Index);
    static BOOL InterruptEnable(UINT32 Irq_Index);
    static BOOL InterruptDisable(UINT32 Irq_Index );
    static BOOL InterruptEnableState( UINT32 Irq_Index );
    static BOOL InterruptState(UINT32 Irq_Index );
    static void /*__irq*/ IRQ_Handler(void);
    static IRQ_VECTORING* IRQToIRQVector( UINT32 IRQ );

    static void ForceInterrupt(UINT32 Irq_Index )
    {
        AT91_AIC &AIC = AT91::AIC();

        AIC.AIC_ISCR = (1 << Irq_Index);
    }

    static void RemoveForcedInterrupt(UINT32 Irq_Index )
    {
        AT91_AIC &AIC = AT91::AIC();
        AIC.AIC_ICCR = (1 << Irq_Index);
    }

private:
    static void STUB_ISRVector( void* Param );
};
//
// AT91_AIC  driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// TIMER driver
//
struct AT91_TIMER_Driver
{
    static const UINT32 c_SystemTimer   = 0;
    static const UINT16 c_MaxTimerValue = 0xFFFF; 
    
    //--//
    
    static BOOL Initialize   ( UINT32 Timer, BOOL FreeRunning, UINT32 ClkSource, HAL_CALLBACK_FPN ISR, void* ISR_Param );
    static BOOL Uninitialize ( UINT32 Timer );
    static void ISR_TIMER( void* param );

    static void EnableCompareInterrupt( UINT32 Timer )
    {
        ASSERT(Timer < AT91_MAX_TIMER);
        AT91_TC &TC = AT91::TIMER(Timer);


        (void) TC.TC_SR;
        TC.TC_IER = AT91_TC::TC_CPCS;
        TC.TC_CCR = (AT91_TC::TC_SWTRG  | AT91_TC::TC_CLKEN) ;
    }

    static void DisableCompareInterrupt( UINT32 Timer )
    {
        ASSERT(Timer < AT91_MAX_TIMER);
        AT91_TC &TC = AT91::TIMER(Timer);

        TC.TC_IDR = AT91_TC::TC_CPCS; 
    }

    static void ForceInterrupt( UINT32 Timer )
    {
        ASSERT(Timer < AT91_MAX_TIMER);
        ASSERT_IRQ_MUST_BE_OFF();

        AT91_AIC_Driver::ForceInterrupt( AT91C_ID_TC0 + Timer );
    }

    static void SetCompare( UINT32 Timer, UINT16 Compare )
    {
        ASSERT(Timer < AT91_MAX_TIMER);
        AT91_TC &TC = AT91::TIMER(Timer);

        TC.TC_RC = Compare;
    }

    static UINT16 ReadCounter( UINT32 Timer )
    {
        ASSERT(Timer < AT91_MAX_TIMER);
        AT91_TC &TC = AT91::TIMER(Timer);
        return    TC.TC_CV;
    }

    static BOOL DidTimerOverFlow( UINT32 Timer )
    {
        ASSERT(Timer < AT91_MAX_TIMER);
        AT91_TC &TC = AT91::TIMER(Timer);
        
        return (TC.TC_SR & AT91_TC::TC_COVFS) != 0;        
    }

//--//

private:
    struct Descriptors
    {
        HAL_CALLBACK_FPN isr;
        void* arg;
        BOOL configured;
    };
    Descriptors m_descriptors[AT91_MAX_TIMER];

   
};

extern AT91_TIMER_Driver g_AT91_TIMER_Driver;



//////////////////////////////////////////////////////////////////////////////
// AT91_SPI_DRIVER
//
struct AT91_SPI_Driver
{
    static const int SPI_0 = 0;
    static const int SPI_1 = 1;
    
    BOOL      m_initialized[AT91_SPI::c_MAX_SPI];
    BOOL      m_Enabled    [AT91_SPI::c_MAX_SPI];


    static BOOL Initialize  ();
    static void Uninitialize();
    static void GetPins( UINT32 spi_mod, GPIO_PIN& msk, GPIO_PIN& miso, GPIO_PIN& mosi );

    static BOOL nWrite16_nRead16( const SPI_CONFIGURATION& Configuration, UINT16* Write16, INT32 WriteCount, UINT16* Read16, INT32 ReadCount, INT32 ReadStartOffset );
    static BOOL nWrite8_nRead8  ( const SPI_CONFIGURATION& Configuration, UINT8*  Write8 , INT32 WriteCount, UINT8*  Read8 , INT32 ReadCount, INT32 ReadStartOffset );

    static BOOL Xaction_Start( const SPI_CONFIGURATION& Configuration );
    static BOOL Xaction_Stop ( const SPI_CONFIGURATION& Configuration );

    static BOOL Xaction_nWrite16_nRead16( SPI_XACTION_16& Transaction );
    static BOOL Xaction_nWrite8_nRead8  ( SPI_XACTION_8&  Transaction );

private:
    static void ISR( void* Param );

};

//--//

extern AT91_SPI_Driver g_AT91_SPI_Driver;

//
// AT91_SPI_DRIVER
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// AT91_I2C_DRIVER
//
struct AT91_I2C_Driver
{
    I2C_HAL_XACTION*       m_currentXAction;
    I2C_HAL_XACTION_UNIT*  m_currentXActionUnit;
    BOOL                   m_initialized;

    //--//

    static BOOL Initialize();
    static BOOL Uninitialize();
    static void MasterXAction_Start( I2C_HAL_XACTION* xAction, bool repeatedStart );
    static void MasterXAction_Stop();
    static void GetClockRate( UINT32 rateKhz, UINT8& clockRate, UINT8& clockRate2 );

    static void WriteToSubordinate( I2C_HAL_XACTION_UNIT* unit );
    static void ReadFromSubordinate( I2C_HAL_XACTION_UNIT* unit );
    static void GetPins(GPIO_PIN& scl, GPIO_PIN& sda);
    
private:
    static void ISR( void* arg );
        
};

//
// AT91_I2C_DRIVER
//////////////////////////////////////////////////////////////////////////////

#endif // _AT91_H_1

