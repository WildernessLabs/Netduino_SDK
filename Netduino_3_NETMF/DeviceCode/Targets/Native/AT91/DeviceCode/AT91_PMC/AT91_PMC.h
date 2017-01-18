////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _AT91__PMC_H_1
#define _AT91__PMC_H_1   1

// -------- CKGR_MOR : (CKGR Offset: 0x0) Main Oscillator Register -------- 
#define AT91C_CKGR_MOSCEN     (0x1 <<  0)   // (CKGR) Main Oscillator Enable
#define AT91C_CKGR_OSCBYPASS  (0x1 <<  1)   // (CKGR) Main Oscillator Bypass
#define AT91C_CKGR_OSCOUNT    (0xFF <<  8)  // (CKGR) Main Oscillator Start-up Time

// -------- CKGR_MCFR : (CKGR Offset: 0x4) Main Clock Frequency Register -------- 
#define AT91C_CKGR_MAINF      (0xFFFF <<  0)    // (CKGR) Main Clock Frequency
#define AT91C_CKGR_MAINRDY    (0x1 << 16)       // (CKGR) Main Clock Ready

// -------- CKGR_PLLR : (CKGR Offset: 0xc) PLL B Register -------- 
#define AT91C_CKGR_DIV        (0xFF <<  0)  // (CKGR) Divider Selected
#define AT91C_CKGR_DIV_0      (0x0)         // (CKGR) Divider output is 0
#define AT91C_CKGR_DIV_BYPASS (0x1)         // (CKGR) Divider is bypassed
#define AT91C_CKGR_PLLCOUNT   (0x3F <<  8)  // (CKGR) PLL Counter
#define AT91C_CKGR_OUT        (0x3 << 14)   // (CKGR) PLL Output Frequency Range
#define AT91C_CKGR_OUT_0      (0x0 << 14)   // (CKGR) Please refer to the PLL datasheet
#define AT91C_CKGR_OUT_1      (0x1 << 14)   // (CKGR) Please refer to the PLL datasheet
#define AT91C_CKGR_OUT_2      (0x2 << 14)   // (CKGR) Please refer to the PLL datasheet
#define AT91C_CKGR_OUT_3      (0x3 << 14)   // (CKGR) Please refer to the PLL datasheet
#define AT91C_CKGR_MUL        (0x7FF << 16) // (CKGR) PLL Multiplier
#define AT91C_CKGR_USBDIV     (0x3 << 28)   // (CKGR) Divider for USB Clocks
#define AT91C_CKGR_USBDIV_0   (0x0 << 28)   // (CKGR) Divider output is PLL clock output
#define AT91C_CKGR_USBDIV_1   (0x1 << 28)   // (CKGR) Divider output is PLL clock output divided by 2
#define AT91C_CKGR_USBDIV_2   (0x2 << 28)   // (CKGR) Divider output is PLL clock output divided by 4

struct AT91_PMC {
    volatile UINT32     PMC_SCER;  // System Clock Enable Register
    volatile UINT32     PMC_SCDR;  // System Clock Disable Register
    volatile UINT32     PMC_SCSR;  // System Clock Status Register
    volatile UINT32     Reserved0[1];  // 
    volatile UINT32     PMC_PCER;  // Peripheral Clock Enable Register
    volatile UINT32     PMC_PCDR;  // Peripheral Clock Disable Register
    volatile UINT32     PMC_PCSR;  // Peripheral Clock Status Register
    volatile UINT32     Reserved1[1];  // 
    volatile UINT32     PMC_MOR;   // Main Oscillator Register
    volatile UINT32     PMC_MCFR;  // Main Clock  Frequency Register
    volatile UINT32     PMC_PLLAR;  // PLL Register
    volatile UINT32     PMC_PLLR;   // PLL Register (SAM7) / PLLB (SAM9)
    volatile UINT32     PMC_MCKR;  // Master Clock Register
    volatile UINT32     Reserved3[3];  // 
    volatile UINT32     PMC_PCKR[4];   // Programmable Clock Register
    volatile UINT32     Reserved4[4];  // 
    volatile UINT32     PMC_IER;   // Interrupt Enable Register
    volatile UINT32     PMC_IDR;   // Interrupt Disable Register
    volatile UINT32     PMC_SR;    // Status Register
    volatile UINT32     PMC_IMR;   // Interrupt Mask Register
    volatile UINT32     Reserved5[3]; // Reserved
    volatile UINT32     PMC_PLLICPR; // Charge Pump Current Register    
};

// -------- PMC_SCDR : (PMC Offset: 0x4) System Clock Disable Register -------- 
// -------- PMC_SCSR : (PMC Offset: 0x8) System Clock Status Register -------- 
// -------- CKGR_MOR : (PMC Offset: 0x20) Main Oscillator Register -------- 
// -------- CKGR_MCFR : (PMC Offset: 0x24) Main Clock Frequency Register -------- 
// -------- CKGR_PLLR : (PMC Offset: 0x2c) PLL B Register -------- 
// -------- PMC_MCKR : (PMC Offset: 0x30) Master Clock Register -------- 
#define AT91C_PMC_CSS           (0x3 <<  0) // (PMC) Programmable Clock Selection
#define AT91C_PMC_CSS_SLOW_CLK  (0x0)       // (PMC) Slow Clock is selected
#define AT91C_PMC_CSS_MAIN_CLK  (0x1)       // (PMC) Main Clock is selected
#define AT91C_PMC_CSS_PLLA_CLK  (0x2)       // (PMC) Clock from PLL1 is selected (SAM9 only)
#define AT91C_PMC_CSS_PLL_CLK   (0x3)       // (PMC) Clock from PLL is selected
#define AT91C_PMC_PRES          (0x7 <<  2) // (PMC) Programmable Clock Prescaler
#define AT91C_PMC_PRES_CLK      (0x0 <<  2) // (PMC) Selected clock
#define AT91C_PMC_PRES_CLK_2    (0x1 <<  2) // (PMC) Selected clock divided by 2
#define AT91C_PMC_PRES_CLK_4    (0x2 <<  2) // (PMC) Selected clock divided by 4
#define AT91C_PMC_PRES_CLK_8    (0x3 <<  2) // (PMC) Selected clock divided by 8
#define AT91C_PMC_PRES_CLK_16   (0x4 <<  2) // (PMC) Selected clock divided by 16
#define AT91C_PMC_PRES_CLK_32   (0x5 <<  2) // (PMC) Selected clock divided by 32
#define AT91C_PMC_PRES_CLK_64   (0x6 <<  2) // (PMC) Selected clock divided by 64

#define AT91C_PMC_MDIV        (0x3 <<  8) // (PMC) Master Clock Division
#define AT91C_PMC_MDIV_1      (0x0 <<  8) // (PMC) The master clock and the processor clock are the same
#define AT91C_PMC_MDIV_2      (0x1 <<  8) // (PMC) The processor clock is twice as fast as the master clock
#define AT91C_PMC_MDIV_3      (0x2 <<  8) // (PMC) The processor clock is four times faster than the master clock


// -------- PMC_PCKR : (PMC Offset: 0x40) Programmable Clock Register -------- 
// -------- PMC_IER : (PMC Offset: 0x60) PMC Interrupt Enable Register -------- 
#define AT91C_PMC_MOSCS     (0x1 <<  0) // (PMC) MOSC Status/Enable/Disable/Mask
#define AT91C_PMC_LOCKA     (0x1 <<  1) // (PMC) PLL Status/Enable/Disable/Mask (SAM9 Only)
#define AT91C_PMC_LOCK      (0x1 <<  2) // (PMC) PLL Status/Enable/Disable/Mask
#define AT91C_PMC_MCKRDY    (0x1 <<  3) // (PMC) MCK_RDY Status/Enable/Disable/Mask
#define AT91C_PMC_PCK0RDY   (0x1 <<  8) // (PMC) PCK0_RDY Status/Enable/Disable/Mask
#define AT91C_PMC_PCK1RDY   (0x1 <<  9) // (PMC) PCK1_RDY Status/Enable/Disable/Mask
#define AT91C_PMC_PCK2RDY   (0x1 << 10) // (PMC) PCK2_RDY Status/Enable/Disable/Mask
#define AT91C_PMC_PCK3RDY   (0x1 << 11) // (PMC) PCK3_RDY Status/Enable/Disable/Mask

#define AT91C_PMC_PLLICPR__PLLA (0x1 <<  0)
#define AT91C_PMC_PLLICPR__PLLB (0x1 << 16)


#endif
