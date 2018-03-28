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
//  *** Serial Driver ***
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cores\arm\include\cpu.h>
#include "..\STM32_GPIO\STM32_GPIO_functions.h";
#if defined(PLATFORM_ARM_STM32F4_ANY)
#include "..\stm32f4xx.h"
#elif defined(PLATFORM_ARM_STM32F2_ANY)
#include "..\stm32f2xx.h"
#endif

static const UINT8 g_STM32_UART_UartNum[] = STM32_UART_NUM;
static const UINT8 g_STM32_UART_TxPin[] = STM32_UART_TX_PIN;
static const UINT8 g_STM32_UART_RxPin[] = STM32_UART_RX_PIN;
static const UINT8 g_STM32_UART_RtsPin[] = STM32_UART_RTS_PIN;
static const UINT8 g_STM32_UART_CtsPin[] = STM32_UART_CTS_PIN;
//#define STM32_UART_PORTS ARRAYSIZE_CONST_EXPR(g_STM32_UART_TxPin) // number of ports

// IO addresses
//#if defined(PLATFORM_ARM_Netduino3) || defined(PLATFORM_ARM_Netduino3Ethernet) || defined(PLATFORM_ARM_Netduino3Wifi)
//static USART_TypeDef* g_STM32_Uart[] = {USART1, USART2, USART3, UART4, UART5, USART6, UART7, UART8};
//#else
//static USART_TypeDef* g_STM32_Uart[] = {USART1, USART2, USART3, UART4, UART5, USART6};
//#endif
static USART_TypeDef* g_STM32_Uart[] = STM32_UART;
   
// Timer Alternate Functions
#if defined(PLATFORM_ARM_Netduino3) || defined(PLATFORM_ARM_Netduino3Ethernet) || defined(PLATFORM_ARM_Netduino3Wifi)
static const UINT8 g_STM32_UART_AlternateFunction[] = {7,7,7,8,8,8,8,8};
#else
static const UINT8 g_STM32_UART_AlternateFunction[] = {7,7,7,8,8,8};
#endif

// Timer IRQs
#if defined(PLATFORM_ARM_Netduino3) || defined(PLATFORM_ARM_Netduino3Ethernet) || defined(PLATFORM_ARM_Netduino3Wifi)
static const UINT32 g_STM32_UART_Irq[] = {USART1_IRQn, USART2_IRQn, USART3_IRQn, UART4_IRQn, UART5_IRQn, USART6_IRQn, UART7_IRQn, UART8_IRQn};
#else
static const UINT32 g_STM32_UART_Irq[] = {USART1_IRQn, USART2_IRQn, USART3_IRQn, UART4_IRQn, UART5_IRQn, USART6_IRQn};
#endif


void STM32_USART_Handle_RX_IRQ (int ComPortNum, USART_TypeDef* uart)
{
    INTERRUPT_START;

    char c = (char)(uart->DR); // read RX data
    USART_AddCharToRxBuffer(ComPortNum, c);
    Events_Set(SYSTEM_EVENT_FLAG_COM_IN);

    INTERRUPT_END;
}

void STM32_USART_Handle_TX_IRQ (int ComPortNum, USART_TypeDef* uart)
{
    INTERRUPT_START;

    char c;
    if (USART_RemoveCharFromTxBuffer(ComPortNum, c)) {
        uart->DR = c;  // write TX data
    } else {
        uart->CR1 &= ~USART_CR1_TXEIE; // TX int disable
    }
    Events_Set(SYSTEM_EVENT_FLAG_COM_OUT);

    INTERRUPT_END;
}

void STM32_USART_Interrupt0(void* param)
{
    USART_TypeDef* uart = g_STM32_Uart[0];
    UINT16 sr = uart->SR;
    if (sr & USART_SR_RXNE) STM32_USART_Handle_RX_IRQ(0, uart);
    if (sr & USART_SR_TXE)  STM32_USART_Handle_TX_IRQ(0, uart);
}

void STM32_USART_Interrupt1(void* param)
{
    USART_TypeDef* uart = g_STM32_Uart[1];
    UINT16 sr = uart->SR;
    if (sr & USART_SR_RXNE) STM32_USART_Handle_RX_IRQ(1, uart);
    if (sr & USART_SR_TXE)  STM32_USART_Handle_TX_IRQ(1, uart);
}

void STM32_USART_Interrupt2(void* param)
{
    USART_TypeDef* uart = g_STM32_Uart[2];
    UINT16 sr = uart->SR;
    if (sr & USART_SR_RXNE) STM32_USART_Handle_RX_IRQ(2, uart);
    if (sr & USART_SR_TXE)  STM32_USART_Handle_TX_IRQ(2, uart);
}

void STM32_USART_Interrupt3(void* param)
{
    USART_TypeDef* uart = g_STM32_Uart[3];
    UINT16 sr = uart->SR;
    if (sr & USART_SR_RXNE) STM32_USART_Handle_RX_IRQ(3, uart);
    if (sr & USART_SR_TXE)  STM32_USART_Handle_TX_IRQ(3, uart);
}

void STM32_USART_Interrupt4(void* param)
{
    USART_TypeDef* uart = g_STM32_Uart[4];
    UINT16 sr = uart->SR;
    if (sr & USART_SR_RXNE) STM32_USART_Handle_RX_IRQ(4, uart);
    if (sr & USART_SR_TXE)  STM32_USART_Handle_TX_IRQ(4, uart);
}

void STM32_USART_Interrupt5(void* param)
{
    USART_TypeDef* uart = g_STM32_Uart[5];
    UINT16 sr = uart->SR;
    if (sr & USART_SR_RXNE) STM32_USART_Handle_RX_IRQ(5, uart);
    if (sr & USART_SR_TXE)  STM32_USART_Handle_TX_IRQ(5, uart);
}

#if defined(PLATFORM_ARM_Netduino3) || defined(PLATFORM_ARM_Netduino3Ethernet) || defined(PLATFORM_ARM_Netduino3Wifi)
void STM32_USART_Interrupt6(void* param)
{
    USART_TypeDef* uart = g_STM32_Uart[6];
    UINT16 sr = uart->SR;
    if (sr & USART_SR_RXNE) STM32_USART_Handle_RX_IRQ(6, uart);
    if (sr & USART_SR_TXE)  STM32_USART_Handle_TX_IRQ(6, uart);
}

void STM32_USART_Interrupt7(void* param)
{
    USART_TypeDef* uart = g_STM32_Uart[7];
    UINT16 sr = uart->SR;
    if (sr & USART_SR_RXNE) STM32_USART_Handle_RX_IRQ(7, uart);
    if (sr & USART_SR_TXE)  STM32_USART_Handle_TX_IRQ(7, uart);
}
#endif

BOOL CPU_USART_Initialize( int ComPortNum, int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue )
{
    if (ComPortNum >= TOTAL_USART_PORT) return FALSE;
    if (Parity >= USART_PARITY_MARK) return FALSE;
	if (DataBits < 7 || (DataBits == 7 && Parity == 0) || (DataBits == 9 && Parity) || DataBits > 9) return FALSE;
    
    GLOBAL_LOCK(irq);

    USART_TypeDef* uart = g_STM32_Uart[ComPortNum];
	UINT32 uartNum = g_STM32_UART_UartNum[ComPortNum];
	UINT8 alternateFunc = g_STM32_UART_AlternateFunction[uartNum];
    UINT32 clk;

    GPIO_PIN rxPin, txPin, ctsPin, rtsPin;
    CPU_USART_GetPins(ComPortNum, rxPin, txPin, ctsPin, rtsPin);

	if ((FlowValue & USART_FLOW_HW_OUT_EN) && (ctsPin != 0xFF))
    	STM32_GPIO_AFConfig(ctsPin, alternateFunc);
	if ((FlowValue & USART_FLOW_HW_IN_EN) && (rtsPin != 0xFF)	)
    	STM32_GPIO_AFConfig(rtsPin, alternateFunc);

    // enable UART clock
    if(uart == USART1 || uart == USART6) { // USART1/USART6 on APB2
		UINT32 shift = (((UINT32)uart - USART1_BASE) >> 0x0A) + 1;
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN >> 1 << shift;
        clk = SYSTEM_APB2_CLOCK_HZ;
    } else { // USART2/USART3/UART4/UART5 on APB1
		UINT32 shift = (((UINT32)uart - USART2_BASE) >> 0x0A) + 1;
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN >> 1 << shift;
        clk = SYSTEM_APB1_CLOCK_HZ;
    }

    //  baudrate
    UINT16 div = (UINT16)((clk + (BaudRate >> 1)) / BaudRate); // rounded
    uart->BRR = div;

    // control
    UINT16 ctrl = USART_CR1_TE | USART_CR1_RE;
    if (DataBits == 9) ctrl |= USART_CR1_M;

    if (Parity) {
		ctrl |= USART_CR1_PCE;
		if (DataBits == 8) ctrl |= USART_CR1_M;
	}
    if (Parity == USART_PARITY_ODD) ctrl |= USART_CR1_PS;
    uart->CR1 = ctrl;
    
    if (StopBits == USART_STOP_BITS_ONE) StopBits = 0;
    uart->CR2 = (UINT16)(StopBits << 12);

    ctrl = 0;
    if ((FlowValue & USART_FLOW_HW_OUT_EN) && (ctsPin != 0xFF)) ctrl |= USART_CR3_CTSE;
    if ((FlowValue & USART_FLOW_HW_IN_EN) && (rtsPin != 0xFF))  ctrl |= USART_CR3_RTSE;
    uart->CR3 = ctrl;

    //CPU_GPIO_DisablePin(rxPin, RESISTOR_PULLUP, 0, GPIO_ALT_MODE_5); // should we pull up RX by default?
    CPU_GPIO_DisablePin(rxPin, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_5); // alternate input
    STM32_GPIO_AFConfig(rxPin, alternateFunc);
    CPU_GPIO_DisablePin(txPin, RESISTOR_DISABLED, 1, GPIO_ALT_MODE_1);  // alternate output
    STM32_GPIO_AFConfig(txPin, alternateFunc);

    if (FlowValue & USART_FLOW_HW_OUT_EN)
	{
		if (ctsPin == 0xFF) return FALSE;
    	CPU_GPIO_DisablePin(ctsPin, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_5); // alternate input
	    STM32_GPIO_AFConfig(ctsPin, alternateFunc);
	}
    if (FlowValue & USART_FLOW_HW_IN_EN)
	{
		if (rtsPin == 0xFF) return FALSE;
    	CPU_GPIO_DisablePin(rtsPin, RESISTOR_DISABLED, 1, GPIO_ALT_MODE_1);  // alternate output
	    STM32_GPIO_AFConfig(rtsPin, alternateFunc);
	}

    CPU_USART_ProtectPins(ComPortNum, FALSE);
    
    if (ComPortNum == 0) {
        CPU_INTC_ActivateInterrupt(g_STM32_UART_Irq[uartNum], STM32_USART_Interrupt0, 0);
    } else if (ComPortNum == 1) {
        CPU_INTC_ActivateInterrupt(g_STM32_UART_Irq[uartNum], STM32_USART_Interrupt1, 0);
    } else if (ComPortNum == 2) {
        CPU_INTC_ActivateInterrupt(g_STM32_UART_Irq[uartNum], STM32_USART_Interrupt2, 0);
    } else if (ComPortNum == 3) {
        CPU_INTC_ActivateInterrupt(g_STM32_UART_Irq[uartNum], STM32_USART_Interrupt3, 0);
    } else if (ComPortNum == 4) {
        CPU_INTC_ActivateInterrupt(g_STM32_UART_Irq[uartNum], STM32_USART_Interrupt4, 0);
    } else if (ComPortNum == 5) {
        CPU_INTC_ActivateInterrupt(g_STM32_UART_Irq[uartNum], STM32_USART_Interrupt5, 0);
#if defined(PLATFORM_ARM_Netduino3) || defined(PLATFORM_ARM_Netduino3Ethernet) || defined(PLATFORM_ARM_Netduino3Wifi)
    } else if (ComPortNum == 6) {
        CPU_INTC_ActivateInterrupt(g_STM32_UART_Irq[uartNum], STM32_USART_Interrupt6, 0);
    } else if (ComPortNum == 7) {
        CPU_INTC_ActivateInterrupt(g_STM32_UART_Irq[uartNum], STM32_USART_Interrupt7, 0);
#endif
    }

    uart->CR1 |= USART_CR1_UE; // start uart

    return TRUE;
}

BOOL CPU_USART_Uninitialize( int ComPortNum )
{
    GLOBAL_LOCK(irq);
    
    USART_TypeDef* uart = g_STM32_Uart[ComPortNum];
	UINT32 uartNum = g_STM32_UART_UartNum[ComPortNum];

    uart->CR1 = 0; // stop uart
    CPU_INTC_DeactivateInterrupt(g_STM32_UART_Irq[uartNum]);

    CPU_USART_ProtectPins(ComPortNum, TRUE);
    
    // disable UART clock
    if(uart == USART1 || uart == USART6) { // USART1/USART6 on APB2
		UINT32 shift = (((UINT32)uart - USART1_BASE) >> 0x0A) + 1;
        RCC->APB2ENR &= ~(RCC_APB2ENR_USART1EN >> 1 << shift);
    } else { // USART2/USART3/UART4/UART5 on APB1
		UINT32 shift = (((UINT32)uart - USART2_BASE) >> 0x0A) + 1;
        RCC->APB1ENR &= ~(RCC_APB1ENR_USART2EN >> 1 << shift);
    }

    return TRUE;
}

BOOL CPU_USART_TxBufferEmpty( int ComPortNum )
{
    if (g_STM32_Uart[ComPortNum]->SR & USART_SR_TXE) return TRUE;
    return FALSE;
}

BOOL CPU_USART_TxShiftRegisterEmpty( int ComPortNum )
{
    if (g_STM32_Uart[ComPortNum]->SR & USART_SR_TC) return TRUE;
    return FALSE;
}

void CPU_USART_WriteCharToTxBuffer( int ComPortNum, UINT8 c )
{
#ifdef DEBUG
    ASSERT(CPU_USART_TxBufferEmpty(ComPortNum));
#endif
    g_STM32_Uart[ComPortNum]->DR = c;
}

void CPU_USART_TxBufferEmptyInterruptEnable( int ComPortNum, BOOL Enable )
{
    USART_TypeDef* uart = g_STM32_Uart[ComPortNum];
    if (Enable) {
        uart->CR1 |= USART_CR1_TXEIE;  // tx int enable
    } else {
        uart->CR1 &= ~USART_CR1_TXEIE; // tx int disable
    }
}

BOOL CPU_USART_TxBufferEmptyInterruptState( int ComPortNum )
{
    if (g_STM32_Uart[ComPortNum]->CR1 & USART_CR1_TXEIE) return TRUE;
    return FALSE;
}

void CPU_USART_RxBufferFullInterruptEnable( int ComPortNum, BOOL Enable )
{
    USART_TypeDef* uart = g_STM32_Uart[ComPortNum];
    if (Enable) {
        uart->CR1 |= USART_CR1_RXNEIE;  // rx int enable
    } else {
        uart->CR1 &= ~USART_CR1_RXNEIE; // rx int disable
    }
}

BOOL CPU_USART_RxBufferFullInterruptState( int ComPortNum )
{
    if (g_STM32_Uart[ComPortNum]->CR1 & USART_CR1_RXNEIE) return TRUE;

    return FALSE;
}

BOOL CPU_USART_TxHandshakeEnabledState( int ComPortNum )
{
    // The state of the CTS input only matters if Flow Control is enabled
    if (g_STM32_Uart[ComPortNum]->CR3 & USART_CR3_CTSE)
    {
        return !CPU_GPIO_GetPinState((GPIO_PIN)g_STM32_UART_CtsPin[ComPortNum]); // CTS active
    }
    return TRUE; // If this handshake input is not being used, it is assumed to be good
}

void CPU_USART_ProtectPins( int ComPortNum, BOOL On )  // idempotent
{
    if (On) {
        CPU_USART_RxBufferFullInterruptEnable(ComPortNum, FALSE);
        CPU_USART_TxBufferEmptyInterruptEnable(ComPortNum, FALSE);
    } else {
        CPU_USART_TxBufferEmptyInterruptEnable(ComPortNum, TRUE);
        CPU_USART_RxBufferFullInterruptEnable(ComPortNum, TRUE);
    }
}

UINT32 CPU_USART_PortsCount()
{
    return TOTAL_USART_PORT;
}

void CPU_USART_GetPins( int ComPortNum, GPIO_PIN& rxPin, GPIO_PIN& txPin, GPIO_PIN& ctsPin, GPIO_PIN& rtsPin )
{
	rxPin = (GPIO_PIN)g_STM32_UART_RxPin[ComPortNum];
	txPin = (GPIO_PIN)g_STM32_UART_TxPin[ComPortNum];
	ctsPin = (GPIO_PIN)g_STM32_UART_CtsPin[ComPortNum];
	rtsPin = (GPIO_PIN)g_STM32_UART_RtsPin[ComPortNum];
}

void CPU_USART_GetBaudrateBoundary( int ComPortNum, UINT32 & maxBaudrateHz, UINT32 & minBaudrateHz )
{
	USART_TypeDef* uart = g_STM32_Uart[ComPortNum];
	UINT32 clk;

    if(uart == USART1 || uart == USART6) { // USART1/USART6 on APB2
		clk = SYSTEM_APB2_CLOCK_HZ;
    } else { // USART2/USART3/UART4/UART5 on APB1
		clk = SYSTEM_APB1_CLOCK_HZ;
	}

    maxBaudrateHz = clk >> 4;
    minBaudrateHz = clk >> 16;
}

BOOL CPU_USART_SupportNonStandardBaudRate( int ComPortNum )
{
    return TRUE;
}

BOOL CPU_USART_IsBaudrateSupported( int ComPortNum, UINT32& BaudrateHz )
{
	USART_TypeDef* uart = g_STM32_Uart[ComPortNum];
	UINT32 clk;

    if(uart == USART1 || uart == USART6) { // USART1/USART6 on APB2
		clk = SYSTEM_APB2_CLOCK_HZ;
    } else { // USART2/USART3/UART4/UART5 on APB1
		clk = SYSTEM_APB1_CLOCK_HZ;
	}

	UINT32 max = clk >> 4;

    if (BaudrateHz <= max) return TRUE;
    BaudrateHz = max;
    return FALSE;
}


