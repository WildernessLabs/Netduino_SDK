////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
//  Copyright (c) Microsoft Corporation. All rights reserved.
//  Portions Copyright (c) Oberon microsystems, Inc.
//  Portions Copyright (c) Secret Labs LLC and the Netduino community. All rights reserved.
//
//  *** Bootstrap ***
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "..\stm32.h"

///////////////////////////////////////////////////////////////////////////////


/* STM32 clock configuration */



/************************* PLL Parameters *************************************/
/* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N */
#define PLL_M      25

#ifdef STM32F2XX
	#define PLL_N      240
	/* SYSCLK = PLL_VCO / PLL_P */
	#define PLL_P      2
	/* USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ */
	#define PLL_Q      5
#endif

#ifdef STM32F4XX
	#define PLL_N      336
	/* SYSCLK = PLL_VCO / PLL_P */
	#define PLL_P      2
	/* USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ */
	#define PLL_Q      7
#endif

/******************************************************************************/


#pragma arm section code = "SectionForBootstrapOperations"


/* IO initialization implemented in solution DeviceCode\Init */
void BootstrapCode_GPIO();


extern "C"
{
static void SetSysClock(void)
{
/******************************************************************************/
/*            PLL (clocked by HSE) used as System clock source                */
/******************************************************************************/
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
#if defined(PLATFORM_ARM_NetduinoPlus2)
  /* configure MCO1 to output HSE / 1 */
  RCC->CFGR &= RCC_CFGR_MCO1_RESET_MASK;
  RCC->CFGR |= RCC_CFGR_MCO1Source_HSE | RCC_CFGR_MCO1Div_1;
#endif

  /* Enable HSE */
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);

  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }

  if (HSEStatus == (uint32_t)0x01)
  {
#ifdef STM32F4XX
    /* Select regulator voltage output Scale 1 mode, System frequency up to 168 MHz */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;
#endif
    /* HCLK = SYSCLK / 1*/
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

    /* PCLK2 = HCLK / 2*/
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;

    /* PCLK1 = HCLK / 4*/
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

    /* Configure the main PLL */
    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
                   (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);

    /* Enable the main PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till the main PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }

    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
#ifdef STM32F2XX
    FLASH->ACR = FLASH_ACR_PRFTEN |FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_3WS;
#endif
#ifdef STM32F4XX
	FLASH->ACR = FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;
#endif
    /* Select the main PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }

}

void __section("SectionForBootstrapOperations") STM32_BootstrapCode()
{
    // assure interupts off
    __disable_irq();

    // configure jtag debug support
    DBGMCU->CR = DBGMCU_CR_DBG_SLEEP;
    DBGMCU->APB1FZ = DBGMCU_APB1_FZ_DBG_TIM5_STOP;

    // allow unaligned memory access and do not enforce 8 byte stack alignment
    SCB->CCR &= ~(SCB_CCR_UNALIGN_TRP_Msk | SCB_CCR_STKALIGN_Msk);

    // catch all faults
    CoreDebug->DEMCR |= CoreDebug_DEMCR_VC_HARDERR_Msk | CoreDebug_DEMCR_VC_INTERR_Msk |
                        CoreDebug_DEMCR_VC_BUSERR_Msk | CoreDebug_DEMCR_VC_STATERR_Msk |
                        CoreDebug_DEMCR_VC_CHKERR_Msk | CoreDebug_DEMCR_VC_NOCPERR_Msk |
                        CoreDebug_DEMCR_VC_MMERR_Msk;

  /* Reset the RCC clock configuration to the default reset state ------------*/
  /* Set HSION bit */
  RCC->CR |= (uint32_t)0x00000001;

  /* Reset CFGR register */
  RCC->CFGR = 0x00000000;

  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  /* Reset PLLCFGR register */
  RCC->PLLCFGR = 0x24003010;

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Disable all interrupts */
  RCC->CIR = 0x00000000;

	SetSysClock();
/*
    // set flash access time & enable prefetch buffer
    //FLASH->ACR = FLASH_ACR_LATENCY_BITS | FLASH_ACR_PRFTBE;
    FLASH->ACR = FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;

    // wait for PPL to lock
    while(!(RCC->CR & RCC_CR_PLLRDY));

    // final clock setup
    RCC->CFGR =

    				//RCC_CFGR_PLLMULL_BITS   // pll multiplier
              //| RCC_CFGR_PLLSRC_HSE     // pll in = HSE
               RCC_CFGR_SW_PLL         // sysclk = pll out (SYSTEM_CLOCK_HZ)
              | RCC_CFGR_USBPRE_BIT     // USB clock
              | RCC_CFGR_HPRE_DIV_BITS  // AHB clock
              | RCC_CFGR_PPRE1_DIV_BITS // APB1 clock
              | RCC_CFGR_PPRE2_DIV_BITS; // APB2 clock
              //| RCC_CFGR_ADC_BITS;      // ADC clock (max 14MHz)

    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
                   (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);

    // minimal peripheral clocks
    //RCC->AHBENR  = RCC_AHBENR_SRAMEN | RCC_AHBENR_FLITFEN;
    //RCC->APB2ENR = RCC_APB2ENR_AFIOEN;
    RCC->APB1ENR = RCC_APB1ENR_PWREN;

    // stop HSI clock
    RCC->CR &= ~RCC_CR_HSION;
    */
}


void __section("SectionForBootstrapOperations") BootstrapCode()
{
    STM32_BootstrapCode();

    BootstrapCode_GPIO();

    PrepareImageRegions();
}

void __section("SectionForBootstrapOperations") BootstrapCodeMinimal()
{
    STM32_BootstrapCode();

    BootstrapCode_GPIO();
}

}

