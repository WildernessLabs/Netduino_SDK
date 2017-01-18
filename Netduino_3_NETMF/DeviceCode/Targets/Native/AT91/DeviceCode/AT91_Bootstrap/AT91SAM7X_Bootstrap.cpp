////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#define BOARD_OSCOUNT           (AT91_PMC::CKGR_OSCOUNT & (0x40 << 8))
#define BOARD_USBDIV            AT91_PMC::CKGR_USBDIV_1
#define BOARD_CKGR_PLL          AT91_PMC::CKGR_OUT_0
#define BOARD_PLLCOUNT          (16 << 8)
#define BOARD_MUL               (AT91_PMC::CKGR_MUL & ((375-1) << 16))
#define BOARD_DIV               (AT91_PMC::CKGR_DIV & 72)
#define BOARD_PRESCALER         AT91_PMC::PMC_PRES_CLK_2

void BootstrapCode_Clocks()
{
    // Power Management Controller
    AT91_PMC &pmc = AT91::PMC();

    // Initialize main oscillator
    pmc.PMC_CKGR_MOR = BOARD_OSCOUNT | AT91_PMC::CKGR_MOSCEN;
    while (!(pmc.PMC_SR & AT91_PMC::PMC_MOSCS));

    /* Initialize PLL at 96MHz  and USB clock to 48MHz */
    pmc.PMC_CKGR_PLLR = BOARD_USBDIV | BOARD_CKGR_PLL | BOARD_PLLCOUNT \
                               | BOARD_MUL | BOARD_DIV;
    while (!(pmc.PMC_SR & AT91_PMC::PMC_LOCK));

    // Wait for the master clock if it was already initialized
    while (!(pmc.PMC_SR & AT91_PMC::PMC_MCKRDY));

    // Switch to slow clock + prescaler
    pmc.PMC_MCKR = BOARD_PRESCALER;
    while (!(pmc.PMC_SR & AT91_PMC::PMC_MCKRDY));

    // Switch to fast clock + prescaler
    pmc.PMC_MCKR |= AT91_PMC::PMC_CSS_PLL_CLK;
    while (!(pmc.PMC_SR & AT91_PMC::PMC_MCKRDY));    

}

void BootstrapCode_ARM()
{
    // Disable Watchdog
    *((volatile UINT32*) 0xFFFFFD44) = 0x8000;

    // Embedded Flash Controllers 
    AT91_BL_EFC &efc0= AT91_BL_EFC::BL_EFC(0);
    AT91_BL_EFC &efc1= AT91_BL_EFC::BL_EFC(1);
   

    /* 48 MHz --> 1 Wait State */ 
    efc0.EFC_FMR = AT91_BL_EFC::MC_FWS_1FWS;
    efc1.EFC_FMR = AT91_BL_EFC::MC_FWS_1FWS;

    /* Remap Internal Memory @0x00000000 */
    /* Todo */
    *((volatile UINT32*) 0x00000000) = 0xdeadbeef;
    if ( *((volatile UINT32*) 0x00000000) != 0xdeadbeef )
        *((volatile UINT32*) 0xFFFFFF00) = 1;
}

void BootstrapCode ()
{
    BootstrapCode_ARM();
    BootstrapCode_Clocks();
    PrepareImageRegions();
}
