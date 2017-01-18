////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cores\arm\include\cpu.h>


BOOL CPU_USART_Initialize( int ComPortNum, int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue )
{
    return MC9328MXL_USART_Driver::Initialize(ComPortNum, BaudRate, Parity, DataBits, StopBits, FlowValue);
}

BOOL CPU_USART_Uninitialize( int ComPortNum )
{
    return MC9328MXL_USART_Driver::Uninitialize(ComPortNum);
}

BOOL CPU_USART_TxBufferEmpty( int ComPortNum )
{
    return MC9328MXL_USART_Driver::TxBufferEmpty(ComPortNum);
}

BOOL CPU_USART_TxShiftRegisterEmpty( int ComPortNum )
{
    return MC9328MXL_USART_Driver::TxShiftRegisterEmpty( ComPortNum );
}

void CPU_USART_WriteCharToTxBuffer( int ComPortNum, UINT8 c )
{
    MC9328MXL_USART_Driver::WriteCharToTxBuffer( ComPortNum, c );
}

void CPU_USART_TxBufferEmptyInterruptEnable( int ComPortNum, BOOL Enable )
{
    MC9328MXL_USART_Driver::TxBufferEmptyInterruptEnable( ComPortNum, Enable );
}

BOOL CPU_USART_TxBufferEmptyInterruptState( int ComPortNum )
{
    return MC9328MXL_USART_Driver::TxBufferEmptyInterruptState( ComPortNum );
}

void CPU_USART_RxBufferFullInterruptEnable( int ComPortNum, BOOL Enable )
{
    MC9328MXL_USART_Driver::RxBufferFullInterruptEnable( ComPortNum, Enable );
}

BOOL CPU_USART_RxBufferFullInterruptState( int ComPortNum )
{
    return MC9328MXL_USART_Driver::RxBufferFullInterruptState( ComPortNum );
}

BOOL CPU_USART_TxHandshakeEnabledState( int comPort )
{
    return MC9328MXL_USART_Driver::TxHandshakeEnabledState( comPort );
}

void CPU_USART_ProtectPins( int ComPortNum, BOOL On )
{
    MC9328MXL_USART_Driver::ProtectPins( ComPortNum, On );
}

UINT32 CPU_USART_PortsCount()
{
    return TOTAL_USART_PORT;
}

void CPU_USART_GetPins( int ComPortNum, GPIO_PIN& rxPin, GPIO_PIN& txPin, GPIO_PIN& ctsPin, GPIO_PIN& rtsPin )
{   
    return MC9328MXL_USART_Driver::GetPins(ComPortNum, rxPin, txPin, ctsPin, rtsPin);
}

BOOL CPU_USART_SupportNonStandardBaudRate ( int ComPortNum )
{
    return TRUE;
}

void CPU_USART_GetBaudrateBoundary( int ComPortNum, UINT32& maxBaudrateHz, UINT32& minBaudrateHz )
{
    return MC9328MXL_USART_Driver::BaudrateBoundary(ComPortNum, maxBaudrateHz, minBaudrateHz);
}

BOOL CPU_USART_IsBaudrateSupported( int ComPortNum, UINT32& BaudrateHz )
{
    return MC9328MXL_USART_Driver::IsBaudrateSupported(ComPortNum, BaudrateHz);
}

