////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// This file is part of the Microsoft .NET Micro Framerwork Porting Kit Code Samples and is unsupported. 
// Copyright (C) Microsoft Corporation. All rights reserved. Use of this sample source code is subject to 
// the terms of the Microsoft license agreement under which you licensed this sample source code. 
// 
// THIS SAMPLE CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// 
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#define BOARD_OSCOUNT           (AT91_PMC::CKGR_OSCOUNT & (64 << 8))
#define BOARD_CKGR_PLLA         ( (0x1 << 29) | AT91_PMC::CKGR_OUT_2)
#define BOARD_PLLACOUNT         (63 << 8)
#define BOARD_MULA              (AT91_PMC::CKGR_MUL & (199 << 16))
#define BOARD_DIVA              (AT91_PMC::CKGR_DIV & 12)
#define BOARD_PRESCALER         AT91_PMC::PMC_MDIV_2

#define BOARD_USBDIV            AT91C_CKGR_USBDIV_2
#define BOARD_CKGR_PLLB         AT91C_CKGR_OUT_0
#define BOARD_PLLBCOUNT         BOARD_PLLACOUNT
#define BOARD_MULB              (124 << 16)
#define BOARD_DIVB              12

#define BOARD_USBEN				(1<<16)
#define BOARD_USBPLLCOUNT		(0x0F<<20)
#define BOARD_BIASEN			(1<<24)
#define BOARD_BIASCOUNT			(0x0F<<28)

#define READ(peripheral, register)          (peripheral.register)
#define WRITE(peripheral, register, value)  (peripheral.register = value)

/*
* Setup PLL & SDRAM
*/
void AT91_SAM_ClockInit(void)
{
    // Power Management Controller
    AT91_PMC &pmc = AT91::PMC();

    // Initialize main oscillator
    pmc.PMC_CKGR_MOR = BOARD_OSCOUNT | AT91_PMC::CKGR_MOSCEN;
    while (!(pmc.PMC_SR & AT91_PMC::PMC_MOSCS));

    // Initialize PLLA at 200MHz 
    pmc.PMC_CKGR_PLLAR = BOARD_CKGR_PLLA
                                | BOARD_PLLACOUNT
                                | BOARD_MULA
                                | BOARD_DIVA;
    while (!(pmc.PMC_SR & AT91_PMC::PMC_LOCKA));

    // Initialize UTMI for USB usage
    pmc.PMC_CKGR_UCKR = BOARD_USBEN|BOARD_USBPLLCOUNT|BOARD_BIASEN|BOARD_BIASCOUNT;
    while (!(pmc.PMC_SR & AT91_PMC::PMC_LOCKU));
	
	
    // Wait for the master clock if it was already initialized
    while (!(pmc.PMC_SR & AT91_PMC::PMC_MCKRDY));

    // Switch to fast clock
    // Switch to main oscillator + prescaler
    pmc.PMC_MCKR = BOARD_PRESCALER;
    while (!(pmc.PMC_SR & AT91_PMC::PMC_MCKRDY));

    // Switch to PLL + prescaler
    pmc.PMC_MCKR |= AT91_PMC::PMC_CSS_PLLA_CLK;
    while (!(pmc.PMC_SR & AT91_PMC::PMC_MCKRDY));
}


#define GPIO_PIN_TO_PORT(x) (x / AT91_GPIO_Driver::c_PinsPerPort)
#define GPIO_PIN_TO_BIT(x)  1<<(x % AT91_GPIO_Driver::c_PinsPerPort)

/*
* Localize the SDRAM GPIO pin disable code because we are not fully initialized
* In particular, the SmartPtr (GLOBAL_LOCK) code is mapped to the 0 address space 
* which isn't valid until MMU is initialized.
*/
void AT91_SAM_Sdram_DisablePin(UINT32 gpioPin)
{
    struct AT91_PIO * pPioX = (struct AT91_PIO *)(AT91C_BASE_PIOA + GPIO_PIN_TO_PORT(gpioPin) * 0x200);
    UINT32 bit = GPIO_PIN_TO_BIT(gpioPin);
    
    pPioX->PIO_PPUDR = bit;
    pPioX->PIO_PDR   = bit;
    pPioX->PIO_ASR   = bit;
    pPioX->PIO_IDR   = bit;
	pPioX->PIO_MDDR = bit;
}   
    

void AT91_SAM_SdramInit(void)
{
    volatile UINT32 *pSdram = (volatile UINT32*) SDRAM_MEMORY_Base;
    int i;


    // Affect PIO lines to SDRAM controller
    AT91_SAM_Sdram_DisablePin( AT91_GPIO_Driver::PB16 );
    AT91_SAM_Sdram_DisablePin( AT91_GPIO_Driver::PB17 );
    AT91_SAM_Sdram_DisablePin( AT91_GPIO_Driver::PB18 );
    AT91_SAM_Sdram_DisablePin( AT91_GPIO_Driver::PB19 );
    AT91_SAM_Sdram_DisablePin( AT91_GPIO_Driver::PB20 );
    AT91_SAM_Sdram_DisablePin( AT91_GPIO_Driver::PB21 );
    AT91_SAM_Sdram_DisablePin( AT91_GPIO_Driver::PB22 );
    AT91_SAM_Sdram_DisablePin( AT91_GPIO_Driver::PB23 );
    AT91_SAM_Sdram_DisablePin( AT91_GPIO_Driver::PB24 );
    AT91_SAM_Sdram_DisablePin( AT91_GPIO_Driver::PB25 );
    AT91_SAM_Sdram_DisablePin( AT91_GPIO_Driver::PB26 );
    AT91_SAM_Sdram_DisablePin( AT91_GPIO_Driver::PB27 );
    AT91_SAM_Sdram_DisablePin( AT91_GPIO_Driver::PB28 );
    AT91_SAM_Sdram_DisablePin( AT91_GPIO_Driver::PB29 );
    AT91_SAM_Sdram_DisablePin( AT91_GPIO_Driver::PB30 );
    AT91_SAM_Sdram_DisablePin( AT91_GPIO_Driver::PB31 );

    // AT91 Bus Matrix controller
    AT91_MATRIX &matrix = AT91::MATRIX();

    // Enable EBI chip select for the SDRAM
    WRITE(matrix, MATRIX_EBICSA, AT91_MATRIX::MATRIX_CS1A_SDRAMC);

    // Sdramc Controller
    AT91_SDRAMC &sdramc = AT91::SDRAMC();

    // CFG Control Register
    sdramc.SDRAMC_CR = (   AT91_SDRAMC::SDRAMC_NC_9    
                                | AT91_SDRAMC::SDRAMC_NR_13  
                                | AT91_SDRAMC::SDRAMC_CAS_2
                                | AT91_SDRAMC::SDRAMC_NB_4_BANKS
                                | AT91_SDRAMC::SDRAMC_DBW_32_BITS
                                | AT91_SDRAMC::SDRAMC_TWR_2
                                | AT91_SDRAMC::SDRAMC_TRC_7
                                | AT91_SDRAMC::SDRAMC_TRP_3
                                | AT91_SDRAMC::SDRAMC_TRCD_3  
                                | AT91_SDRAMC::SDRAMC_TRAS_5
                                | AT91_SDRAMC::SDRAMC_TXSR_8);

        for (i = 0; i < 100000; i++);

        WRITE(sdramc, SDRAMC_MR, AT91_SDRAMC::SDRAMC_MODE_NOP_CMD);          // Perform NOP
        pSdram[0] = 0x00000000;

        WRITE(sdramc, SDRAMC_MR, AT91_SDRAMC::SDRAMC_MODE_PRCGALL_CMD);      // Set PRCHG AL
        pSdram[0] = 0x00000000;                                             // Perform PRCHG

        for (i = 0; i < 100000; i++);

        WRITE(sdramc, SDRAMC_MR, AT91_SDRAMC::SDRAMC_MODE_RFSH_CMD);        // Set 1st CBR
        pSdram[1] = 0x00000001;                                                 // Perform CBR

        WRITE(sdramc, SDRAMC_MR, AT91_SDRAMC::SDRAMC_MODE_RFSH_CMD);        // Set 2 CBR
        pSdram[2] = 0x00000002;                                                 // Perform CBR

        WRITE(sdramc, SDRAMC_MR, AT91_SDRAMC::SDRAMC_MODE_RFSH_CMD);        // Set 3 CBR
        pSdram[3] = 0x00000003;                                                 // Perform CBR

        WRITE(sdramc, SDRAMC_MR, AT91_SDRAMC::SDRAMC_MODE_RFSH_CMD);        // Set 4 CBR
        pSdram[4] = 0x00000004;                                                 // Perform CBR

        WRITE(sdramc, SDRAMC_MR, AT91_SDRAMC::SDRAMC_MODE_RFSH_CMD);        // Set 5 CBR
        pSdram[5] = 0x00000005;                                                 // Perform CBR

        WRITE(sdramc, SDRAMC_MR, AT91_SDRAMC::SDRAMC_MODE_RFSH_CMD);        // Set 6 CBR
        pSdram[6] = 0x00000006;                                                 // Perform CBR

        WRITE(sdramc, SDRAMC_MR, AT91_SDRAMC::SDRAMC_MODE_RFSH_CMD);        // Set 7 CBR
        pSdram[7] = 0x00000007;                                                 // Perform CBR

        WRITE(sdramc, SDRAMC_MR, AT91_SDRAMC::SDRAMC_MODE_RFSH_CMD);        // Set 8 CBR
        pSdram[8] = 0x00000008;                                                 // Perform CBR

        WRITE(sdramc, SDRAMC_MR, AT91_SDRAMC::SDRAMC_MODE_LMR_CMD);         // Set LMR operation
        pSdram[9] = 0xcafedede;                                                 // Perform LMR burst=1, lat=2

        WRITE(sdramc, SDRAMC_TR, (SYSTEM_PERIPHERAL_CLOCK_HZ * 7) / 1000000); // Set Refresh Timer

        WRITE(sdramc, SDRAMC_MR, AT91_SDRAMC::SDRAMC_MODE_NORMAL_CMD);      // Set Normal mode
        pSdram[0] = 0x00000000;
        pSdram[0] = 0x00000000;
}


