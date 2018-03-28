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
//  *** SPI Driver ***
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cores\arm\include\cpu.h>
#include "..\stm32.h"
#include "..\STM32_RCC\STM32_RCC_functions.h"

// pins
#if defined(PLATFORM_ARM_NetduinoGo)
#define SPI1_SCLK_Pin (0x05)  // PA5
#define SPI1_MISO_Pin (0x06)  // PA6
#define SPI1_MOSI_Pin (0x07)  // PA7
#define SPI2_SCLK_Pin (0x1D)  // PB13
#define SPI2_MISO_Pin (0x1E)  // PB14
#define SPI2_MOSI_Pin (0x1F)  // PB15
#define SPI3_SCLK_Pin (0x2A)  // PB3
#define SPI3_MISO_Pin (0x14)  // PB4
#define SPI3_MOSI_Pin (0x15)  // PB5
#elif defined(PLATFORM_ARM_Netduino2) || defined(PLATFORM_ARM_NetduinoPlus2)
#define SPI1_SCLK_Pin (0x05)  // PA5
#define SPI1_MISO_Pin (0x14)  // PB4
#define SPI1_MOSI_Pin (0x07)  // PA7
#define SPI2_SCLK_Pin (0x1D)  // PB13
#define SPI2_MISO_Pin (0x1E)  // PB14
#define SPI2_MOSI_Pin (0x1F)  // PB15
#define SPI3_SCLK_Pin (0x2A)  // PC10
#define SPI3_MISO_Pin (0x2B)  // PC11
#define SPI3_MOSI_Pin (0x2C)  // PC12
#elif defined(PLATFORM_ARM_Netduino3) || defined(PLATFORM_ARM_Netduino3Ethernet) || defined(PLATFORM_ARM_Netduino3Wifi)
#define SPI1_SCLK_Pin (0x05)  // PA5
#define SPI1_MISO_Pin (0x06)  // PA6
#define SPI1_MOSI_Pin (0x07)  // PA7
#define SPI2_SCLK_Pin (0x1D)  // PB13
#define SPI2_MISO_Pin (0x1E)  // PB14
#define SPI2_MOSI_Pin (0x1F)  // PB15
#define SPI3_SCLK_Pin (0x2A)  // PC10
#define SPI3_MISO_Pin (0x2B)  // PC11
#define SPI3_MOSI_Pin (0x2C)  // PC12
#define SPI4_SCLK_Pin (0x42)  // PE2
#define SPI4_MISO_Pin (0x4D)  // PE13
#define SPI4_MOSI_Pin (0x46)  // PE6
#endif

/*
struct SPI_CONFIGURATION
{
    GPIO_PIN       DeviceCS;
    BOOL           CS_Active;             // False = LOW active,      TRUE = HIGH active
    BOOL           MSK_IDLE;              // False = LOW during idle, TRUE = HIGH during idle
    BOOL           MSK_SampleEdge;        // False = sample falling edge, TRUE = samples on rising
    BOOL           MD_16bits;
    UINT32         Clock_RateKHz;
    UINT32         CS_Setup_uSecs;
    UINT32         CS_Hold_uSecs;
    UINT32         SPI_mod;
    GPIO_FLAG      BusyPin;
};
*/

#if defined(PLATFORM_ARM_Netduino2) || defined(PLATFORM_ARM_NetduinoPlus2)
static SPI_TypeDef* g_STM32_Spi[] = {SPI2, SPI1, SPI3}; // IO addresses
#elif defined(PLATFORM_ARM_Netduino3) || defined(PLATFORM_ARM_Netduino3Ethernet) || defined(PLATFORM_ARM_Netduino3Wifi)
static SPI_TypeDef* g_STM32_Spi[] = {SPI2, SPI1, SPI3, SPI4}; // IO addresses
#else
static SPI_TypeDef* g_STM32_Spi[] = {SPI1, SPI2, SPI3}; // IO addresses
#endif


BOOL CPU_SPI_Initialize()
{
    return TRUE;
}

void CPU_SPI_Uninitialize()
{
}

BOOL CPU_SPI_nWrite16_nRead16( const SPI_CONFIGURATION& Configuration, UINT16* Write16, INT32 WriteCount, UINT16* Read16, INT32 ReadCount, INT32 ReadStartOffset )
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
    if(!CPU_SPI_Xaction_Start( Configuration )) return FALSE;
    SPI_XACTION_16 Transaction;
    Transaction.Read16          = Read16;
    Transaction.ReadCount       = ReadCount;
    Transaction.ReadStartOffset = ReadStartOffset;
    Transaction.Write16         = Write16;
    Transaction.WriteCount      = WriteCount;
    Transaction.SPI_mod         = Configuration.SPI_mod;
    if(!CPU_SPI_Xaction_nWrite16_nRead16( Transaction )) return FALSE;
    return CPU_SPI_Xaction_Stop( Configuration );
}

BOOL CPU_SPI_nWrite8_nRead8( const SPI_CONFIGURATION& Configuration, UINT8* Write8, INT32 WriteCount, UINT8* Read8, INT32 ReadCount, INT32 ReadStartOffset )
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
    if(!CPU_SPI_Xaction_Start( Configuration )) return FALSE;
    SPI_XACTION_8 Transaction;
    Transaction.Read8           = Read8;
    Transaction.ReadCount       = ReadCount;
    Transaction.ReadStartOffset = ReadStartOffset;
    Transaction.Write8          = Write8;
    Transaction.WriteCount      = WriteCount;
    Transaction.SPI_mod         = Configuration.SPI_mod;
    if(!CPU_SPI_Xaction_nWrite8_nRead8( Transaction )) 
		return FALSE;
    return CPU_SPI_Xaction_Stop( Configuration );
}

BOOL CPU_SPI_Xaction_Start( const SPI_CONFIGURATION& Configuration )
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
    // enable SPI
    switch (Configuration.SPI_mod) {
		case 0:
#if defined(PLATFORM_ARM_Netduino2) || defined(PLATFORM_ARM_NetduinoPlus2) || defined(PLATFORM_ARM_Netduino3) || defined(PLATFORM_ARM_Netduino3Ethernet) || defined(PLATFORM_ARM_Netduino3Wifi)
		    STM32_RCC_APB1PeripheralClockEnable(RCC_APB1ENR_SPI2EN);
#else
		    STM32_RCC_APB2PeripheralClockEnable(RCC_APB2ENR_SPI1EN);
#endif
			break;
		case 1:
#if defined(PLATFORM_ARM_Netduino2) || defined(PLATFORM_ARM_NetduinoPlus2) || defined(PLATFORM_ARM_Netduino3) || defined(PLATFORM_ARM_Netduino3Ethernet) || defined(PLATFORM_ARM_Netduino3Wifi)
		    STM32_RCC_APB2PeripheralClockEnable(RCC_APB2ENR_SPI1EN);
#else
		    STM32_RCC_APB1PeripheralClockEnable(RCC_APB1ENR_SPI2EN);
#endif
			break;
		case 2:
		    STM32_RCC_APB1PeripheralClockEnable(RCC_APB1ENR_SPI3EN);
			break;
#if defined(PLATFORM_ARM_Netduino3) || defined(PLATFORM_ARM_Netduino3Ethernet) || defined(PLATFORM_ARM_Netduino3Wifi)
		case 3:
		    STM32_RCC_APB2PeripheralClockEnable(RCC_APB2ENR_SPI4EN);
			break;
#endif
		default:
			return FALSE;
    }
    
    SPI_TypeDef* spi = g_STM32_Spi[Configuration.SPI_mod];

    // I/O setup
    GPIO_PIN msk, miso, mosi;
    CPU_SPI_GetPins(Configuration.SPI_mod, msk, miso, mosi);

    UINT8 gpioAF = 0x05;
#if defined(PLATFORM_ARM_Netduino3) || defined(PLATFORM_ARM_Netduino3Ethernet) || defined(PLATFORM_ARM_Netduino3Wifi)
    if((Configuration.SPI_mod > 1) && (Configuration.SPI_mod != 3))
#else
    if(Configuration.SPI_mod > 1)
#endif
        gpioAF = 0x06;

    STM32_GPIO_AFConfig(msk, gpioAF);
    STM32_GPIO_AFConfig(miso, gpioAF);
    STM32_GPIO_AFConfig(mosi, gpioAF);

    GPIO_RESISTOR res = RESISTOR_PULLDOWN;
    if (Configuration.MSK_IDLE) res = RESISTOR_PULLUP;

    CPU_GPIO_DisablePin( msk,  res, 1, GPIO_ALT_MODE_2);
    CPU_GPIO_DisablePin( miso, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_5);
    CPU_GPIO_DisablePin( mosi, RESISTOR_DISABLED, 1, GPIO_ALT_MODE_2);

    // set mode bits and enable SPI
	spi->CR1 = 0;

    // set clock prescaler
    UINT32 clock = SYSTEM_APB2_CLOCK_HZ / 2000; // SPI1 on APB2
#if defined(PLATFORM_ARM_Netduino2) || defined(PLATFORM_ARM_NetduinoPlus2)
    if (Configuration.SPI_mod != 1) clock = SYSTEM_APB1_CLOCK_HZ / 2000; // SPI2/3 on APB1
#elif defined(PLATFORM_ARM_Netduino3) || defined(PLATFORM_ARM_Netduino3Ethernet) || defined(PLATFORM_ARM_Netduino3Wifi)
    if ((Configuration.SPI_mod != 1) && (Configuration.SPI_mod != 3)) clock = SYSTEM_APB1_CLOCK_HZ / 2000; // SPI2/3 on APB1
#else
    if (Configuration.SPI_mod != 0) clock = SYSTEM_APB1_CLOCK_HZ / 2000; // SPI2/3 on APB1
#endif
    
    if (clock > Configuration.Clock_RateKHz << 3) {
        clock >>= 4;
        spi->CR1 |= SPI_CR1_BR_2;
    }
    if (clock > Configuration.Clock_RateKHz << 1) {
        clock >>= 2;
        spi->CR1 |= SPI_CR1_BR_1;
    }
    if (clock > Configuration.Clock_RateKHz) {
        spi->CR1 |= SPI_CR1_BR_0;
    }

    // configure CPOL and CPHA
    if (Configuration.MSK_IDLE) spi->CR1 |= SPI_CR1_CPOL | SPI_CR1_CPHA;
    if (!Configuration.MSK_SampleEdge) spi->CR1 ^= SPI_CR1_CPHA; // toggle phase

    // configure bits per frame
    if (Configuration.MD_16bits) spi->CR1 |= SPI_CR1_DFF;

    // configure software SPI_SS mode
    spi->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;

	// enable SPI	
	spi->CR1 |= SPI_CR1_MSTR | SPI_CR1_SPE;
    
    // CS setup
    CPU_GPIO_EnableOutputPin( Configuration.DeviceCS, Configuration.CS_Active );
    if(Configuration.CS_Setup_uSecs)
    {
        HAL_Time_Sleep_MicroSeconds_InterruptEnabled( Configuration.CS_Setup_uSecs );
    }
    
    return TRUE;
}

BOOL CPU_SPI_Xaction_Stop( const SPI_CONFIGURATION& Configuration )
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
    
    SPI_TypeDef* spi = g_STM32_Spi[Configuration.SPI_mod];
    while (spi->SR & SPI_SR_BSY); // wait for completion

    if(Configuration.CS_Hold_uSecs)
    {
        HAL_Time_Sleep_MicroSeconds_InterruptEnabled( Configuration.CS_Hold_uSecs );
    }
    CPU_GPIO_SetPinState( Configuration.DeviceCS, !Configuration.CS_Active );
    GPIO_RESISTOR res = RESISTOR_PULLDOWN;
    if (Configuration.MSK_IDLE) res = RESISTOR_PULLUP;
    GPIO_PIN msk, miso, mosi;
    CPU_SPI_GetPins(Configuration.SPI_mod, msk, miso, mosi);
    CPU_GPIO_EnableInputPin( msk,  FALSE, NULL, GPIO_INT_NONE, res );
    CPU_GPIO_EnableInputPin( miso, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLDOWN );
    CPU_GPIO_EnableInputPin( mosi, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLDOWN );
    
    // disable SPI
    spi->CR1 = 0; // disable SPI
    switch (Configuration.SPI_mod) {
		case 0:
#if defined(PLATFORM_ARM_Netduino2) || defined(PLATFORM_ARM_NetduinoPlus2) || defined(PLATFORM_ARM_Netduino3) || defined(PLATFORM_ARM_Netduino3Ethernet) || defined(PLATFORM_ARM_Netduino3Wifi)
		    STM32_RCC_APB1PeripheralClockDisable(RCC_APB1ENR_SPI2EN);
#else
		    STM32_RCC_APB2PeripheralClockDisable(RCC_APB2ENR_SPI1EN);
#endif
			break;
		case 1:
#if defined(PLATFORM_ARM_Netduino2) || defined(PLATFORM_ARM_NetduinoPlus2) || defined(PLATFORM_ARM_Netduino3) || defined(PLATFORM_ARM_Netduino3Ethernet) || defined(PLATFORM_ARM_Netduino3Wifi)
		    STM32_RCC_APB2PeripheralClockDisable(RCC_APB2ENR_SPI1EN);
#else
		    STM32_RCC_APB1PeripheralClockDisable(RCC_APB1ENR_SPI2EN);
#endif
			break;
		case 2:
		    STM32_RCC_APB1PeripheralClockDisable(RCC_APB1ENR_SPI3EN);
			break;
#if defined(PLATFORM_ARM_Netduino3) || defined(PLATFORM_ARM_Netduino3Ethernet) || defined(PLATFORM_ARM_Netduino3Wifi)
		case 3:
		    STM32_RCC_APB2PeripheralClockDisable(RCC_APB2ENR_SPI4EN);
			break;
#endif
		default:
			return FALSE;
    }

    return TRUE;
}

BOOL CPU_SPI_Xaction_nWrite16_nRead16( SPI_XACTION_16& Transaction )
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
    
    SPI_TypeDef* spi = g_STM32_Spi[Transaction.SPI_mod];
    
    UINT16* outBuf = Transaction.Write16;
    UINT16* inBuf  = Transaction.Read16;
    INT32 outLen = Transaction.WriteCount;
    INT32 num, ii, i = 0;
    
    if (Transaction.ReadCount) { // write & read
        num = Transaction.ReadCount + Transaction.ReadStartOffset;
        ii = -Transaction.ReadStartOffset;
    } else { // write only
        num = outLen;
        ii = 0x80000000; // disable write to inBuf
    }

    UINT16 out = outBuf[0];
    UINT16 in;
    spi->DR = out; // write first word
    while (++i < num) {
        if (i < outLen) out = outBuf[i]; // get new output data
        while (!(spi->SR & SPI_SR_RXNE)); // wait for Rx buffer full
        in = spi->DR; // read input
        spi->DR = out; // start output
        if (ii >= 0) inBuf[ii] = in; // save input data
        ii++;
    }
    while (!(spi->SR & SPI_SR_RXNE)); // wait for Rx buffer full
    in = spi->DR; // read last input
    if (ii >= 0) inBuf[ii] = in; // save last input

    return TRUE;
}

BOOL CPU_SPI_Xaction_nWrite8_nRead8( SPI_XACTION_8& Transaction )
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
    
    SPI_TypeDef* spi = g_STM32_Spi[Transaction.SPI_mod];
    
    UINT8* outBuf = Transaction.Write8;
    UINT8* inBuf  = Transaction.Read8;
    INT32 outLen = Transaction.WriteCount;
    INT32 num, ii, i = 0;
    
    if (Transaction.ReadCount) { // write & read
        num = Transaction.ReadCount + Transaction.ReadStartOffset;
        ii = -Transaction.ReadStartOffset;
    } else { // write only
        num = outLen;
        ii = 0x80000000; // disable write to inBuf
    }

    UINT8 out = outBuf[0];
    UINT16 in;
    spi->DR = out; // write first word
    while (++i < num) {
        if (i < outLen) out = outBuf[i]; // get new output data
        while (!(spi->SR & SPI_SR_RXNE)); // wait for Rx buffer full
        in = spi->DR; // read input
        spi->DR = out; // start output
        if (ii >= 0) inBuf[ii] = (UINT8)in; // save input data
        ii++;
    }
    while (!(spi->SR & SPI_SR_RXNE)); // wait for Rx buffer full
    in = spi->DR; // read last input
    if (ii >= 0) inBuf[ii] = (UINT8)in; // save last input

    return TRUE;
}

UINT32 CPU_SPI_PortsCount()
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
#if defined(PLATFORM_ARM_Netduino3) || defined(PLATFORM_ARM_Netduino3Ethernet) || defined(PLATFORM_ARM_Netduino3Wifi)
    return 4;
#else
    return 3;
#endif
}

void CPU_SPI_GetPins( UINT32 spi_mod, GPIO_PIN& msk, GPIO_PIN& miso, GPIO_PIN& mosi )
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
    if (spi_mod == 0) {
#if defined(PLATFORM_ARM_Netduino2) || defined(PLATFORM_ARM_NetduinoPlus2) || defined(PLATFORM_ARM_Netduino3) || defined(PLATFORM_ARM_Netduino3Ethernet) || defined(PLATFORM_ARM_Netduino3Wifi)
        msk  = SPI2_SCLK_Pin;
        miso = SPI2_MISO_Pin;
        mosi = SPI2_MOSI_Pin;
#else
        msk  = SPI1_SCLK_Pin;
        miso = SPI1_MISO_Pin;
        mosi = SPI1_MOSI_Pin;
#endif
    } else if (spi_mod == 1) {
#if defined(PLATFORM_ARM_Netduino2) || defined(PLATFORM_ARM_NetduinoPlus2) || defined(PLATFORM_ARM_Netduino3) || defined(PLATFORM_ARM_Netduino3Ethernet) || defined(PLATFORM_ARM_Netduino3Wifi)
        msk  = SPI1_SCLK_Pin;
        miso = SPI1_MISO_Pin;
        mosi = SPI1_MOSI_Pin;
#else
        msk  = SPI2_SCLK_Pin;
        miso = SPI2_MISO_Pin;
        mosi = SPI2_MOSI_Pin;
#endif
    } else if (spi_mod == 2) {
        msk  = SPI3_SCLK_Pin;
        miso = SPI3_MISO_Pin;
        mosi = SPI3_MOSI_Pin;
#if defined(PLATFORM_ARM_Netduino3) || defined(PLATFORM_ARM_Netduino3Ethernet) || defined(PLATFORM_ARM_Netduino3Wifi)
    } else if (spi_mod == 3) {
        msk  = SPI4_SCLK_Pin;
        miso = SPI4_MISO_Pin;
        mosi = SPI4_MOSI_Pin;
#endif
    }
}

