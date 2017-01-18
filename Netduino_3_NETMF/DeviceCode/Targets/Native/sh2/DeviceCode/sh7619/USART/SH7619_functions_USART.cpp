////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// This file is part of the Microsoft .NET Micro Framework Porting Kit Code Samples and is unsupported. 
// Copyright (C) Microsoft Corporation. All rights reserved. Use of this sample source code is subject to 
// the terms of the Microsoft license agreement under which you licensed this sample source code. 
// 
// THIS SAMPLE CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// 
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *    Copyright (C) Renesas Technology America,Ltd. 2009  All rights reserved.
 */
 
#include <tinyhal.h>

#include "scif.h"
#include "../iodefine.h"

#ifdef  PLATFORM_DEPENDENT_TX_USART_BUFFER_SIZE
#define TX_USART_BUFFER_SIZE    PLATFORM_DEPENDENT_TX_USART_BUFFER_SIZE
#else
#define TX_USART_BUFFER_SIZE    512
#endif

#ifdef  PLATFORM_DEPENDENT_RX_USART_BUFFER_SIZE
#define RX_USART_BUFFER_SIZE    PLATFORM_DEPENDENT_RX_USART_BUFFER_SIZE
#else
#define RX_USART_BUFFER_SIZE    512
#endif

extern UINT8 TxBuffer_Com[TX_USART_BUFFER_SIZE * TOTAL_USART_PORT];
extern UINT8 RxBuffer_Com[RX_USART_BUFFER_SIZE * TOTAL_USART_PORT];

//--//

BOOL CPU_USART_Initialize( int ComPortNum, int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue )
{   
    unsigned int i = 0;
    unsigned int scsmr = 0;

    if(Parity == USART_PARITY_NONE) scsmr = 0;      // 8N1, divide by 1 
    else if (Parity == USART_PARITY_ODD) scsmr = SCIF_SMR_PE | SCIF_SMR_OE;
    else if (Parity == USART_PARITY_EVEN) scsmr = SCIF_SMR_PE;
    else return FALSE;

    if(DataBits == 7) scsmr|= SCIF_SMR_CHR;
    if(StopBits == USART_STOP_BITS_TWO) scsmr|= SCIF_SMR_STOP;


    if( ComPortNum == 0){
         SCIF0.SCSCR.WORD = 0;      // SCSCR Clear
         SCIF0.SCFCR.WORD = SCIF_CR_TFRST | SCIF_CR_RFRST;  // Reset the TX/RX FIFOs'
         SCIF0.SCSCR.WORD = 0x0001; // Internal Clock            
         SCIF0.SCSMR.WORD = scsmr;
         SCIF0.SCBRR = BRR_CALC(BaudRate);
        // Delay 1 ms
        i = 31250;      // 8 ns/instruction * 4 instructions * loop_count = 1ms = 1000000 ns
        while (i--);    // loop count = 31250

        SCIF0.SCFCR.WORD = 0x0030;  // FIFO Transmit Trigger 
        SCIF0.SCSCR.WORD = 0x0071;  // Enable Transmit & Receive
        //SCIF0 Transmit interrput and Receive interrupt : level 5
        INTC.IPRD.WORD |= 0x5000;
    }
    else
        if( ComPortNum == 1 ){
        SCIF1.SCSCR.WORD = 0;       // SCSCR Clear
        SCIF1.SCFCR.WORD = SCIF_CR_TFRST | SCIF_CR_RFRST;   // Reset the TX/RX FIFOs'
        SCIF1.SCSCR.WORD = 0x0001;  // Internal Clock
        SCIF1.SCSMR.WORD = scsmr;
        SCIF1.SCBRR = BRR_CALC(BaudRate);
        // Delay 1 ms
        i = 31250;      // 8 ns/instruction * 4 instructions * loop_count = 1ms = 1000000 ns
        while (i--);    // loop count = 31250

        SCIF1.SCFCR.WORD = 0x0030;  // FIFO Transmit Trigger 
        SCIF1.SCSCR.WORD = 0x0071;  // Enable Transmit & Receive
        //SCIF1 Transmit interrput and Receive interrupt : level 5
        INTC.IPRD.WORD |= 0x0500;   
    }
    else
    if( ComPortNum == 2 ){
        SCIF2.SCSCR.WORD = 0;       // SCSCR Clear
        SCIF2.SCFCR.WORD = SCIF_CR_TFRST | SCIF_CR_RFRST;   // Reset the TX/RX FIFOs'
        SCIF2.SCSCR.WORD = 0x0001;  // Internal Clock
        SCIF2.SCSMR.WORD = scsmr;
        SCIF2.SCBRR = BRR_CALC(BaudRate);
        // Delay 1 ms
        i = 31250;      // 8 ns/instruction * 4 instructions * loop_count = 1ms = 1000000 ns
        while (i--);    // loop count = 31250

        SCIF2.SCFCR.WORD = 0x0030;  // FIFO Transmit Trigger 
        SCIF2.SCSCR.WORD = 0x0071;  // Enable Transmit & Receive

        //SCIF2 Transmit interrput and Receive interrupt : level 13 for debugger port
        INTC.IPRD.WORD |= 0x00D0;       
    }
    else
        return FALSE;

            
    return TRUE;
}

BOOL CPU_USART_Uninitialize( int ComPortNum )
{
    switch(ComPortNum)
    {
        case 0:
            SCIF0.SCSCR.WORD = 0;       // SCSCR Clear
            SCIF0.SCFCR.WORD = SCIF_CR_TFRST | SCIF_CR_RFRST;   // Reset the TX/RX FIFOs'
            break;
        case 1:
            SCIF1.SCSCR.WORD = 0;       // SCSCR Clear
            SCIF1.SCFCR.WORD = SCIF_CR_TFRST | SCIF_CR_RFRST;   // Reset the TX/RX FIFOs'
            break;
        case 2:
            SCIF2.SCSCR.WORD = 0;       // SCSCR Clear
            SCIF2.SCFCR.WORD = SCIF_CR_TFRST | SCIF_CR_RFRST;   // Reset the TX/RX FIFOs'
            break;
    }

    return TRUE;
}

BOOL CPU_USART_TxBufferEmpty( int ComPortNum )
{
    switch(ComPortNum)
    {
        case 0:
            return (SCIF0.SCFDR.WORD & 0x1F00) == 0;
        case 1:
            return (SCIF1.SCFDR.WORD & 0x1F00) == 0;
        case 2:
            return (SCIF2.SCFDR.WORD & 0x1F00) == 0;
    }

    return TRUE;
}

BOOL CPU_USART_TxShiftRegisterEmpty( int ComPortNum )
{
    return CPU_USART_TxBufferEmpty(ComPortNum);
}

void CPU_USART_WriteCharToTxBuffer( int ComPortNum, UINT8 c )
{

    while (FALSE == CPU_USART_TxShiftRegisterEmpty(ComPortNum));        //wait till ready.
    switch( ComPortNum )
    {
        case 0:
            while((SCIF0.SCFDR.WORD & 0x1000) != 0x0);
            SCIF0.SCFTDR = c;
            while(0 == (SCIF0.SCFSR.WORD & SCIF_SR_TEND));
            SCIF0.SCFSR.WORD &= ~SCIF_SR_TEND;
            break;
            
        case 1:
            while((SCIF1.SCFDR.WORD & 0x1000) != 0x0);
            SCIF1.SCFTDR = c;
            while(0 == (SCIF1.SCFSR.WORD & SCIF_SR_TEND));
            SCIF1.SCFSR.WORD &= ~SCIF_SR_TEND;
            break;

        case 2:
            while((SCIF2.SCFDR.WORD & 0x1000) != 0x0);
            SCIF2.SCFTDR = c;
            while(0 == (SCIF2.SCFSR.WORD & SCIF_SR_TEND));
            SCIF2.SCFSR.WORD &= ~SCIF_SR_TEND;
            break;
    }
}

void CPU_USART_TxBufferEmptyInterruptEnable( int ComPortNum, BOOL Enable )
{
    if( ComPortNum == 0 ){
        if(Enable) SCIF0.SCSCR.WORD |= SCIF_SCR_TIE;
        else SCIF0.SCSCR.WORD &= ~SCIF_SCR_TIE;
    }
    else
    if( ComPortNum == 1 ){
        if(Enable) SCIF1.SCSCR.WORD |= SCIF_SCR_TIE;
        else SCIF1.SCSCR.WORD &= ~SCIF_SCR_TIE;
    }
    else
    if( ComPortNum == 2 ){
        if(Enable) SCIF2.SCSCR.WORD |= SCIF_SCR_TIE;
        else SCIF2.SCSCR.WORD &= ~SCIF_SCR_TIE;
    }
}

BOOL CPU_USART_TxBufferEmptyInterruptState( int ComPortNum )
{
    switch(ComPortNum)
    {
        case 0:
            return 0 != (SCIF0.SCSCR.WORD & SCIF_SCR_TIE);
        case 1:
            return 0 != (SCIF1.SCSCR.WORD & SCIF_SCR_TIE);
        case 2:
            return 0 != (SCIF2.SCSCR.WORD & SCIF_SCR_TIE);
    }

    return TRUE;
}

void CPU_USART_RxBufferFullInterruptEnable( int ComPortNum, BOOL Enable )
{
    if( ComPortNum == 0 ){
        if(Enable) SCIF0.SCSCR.WORD |= SCIF_SCR_RIE;
        else SCIF0.SCSCR.WORD &= ~SCIF_SCR_RIE;
    }
    else
    if( ComPortNum == 1 ){
        if(Enable) SCIF1.SCSCR.WORD |= SCIF_SCR_RIE;
        else SCIF1.SCSCR.WORD &= ~SCIF_SCR_RIE;
    }
    else
    if( ComPortNum == 2 ){
        if(Enable) SCIF2.SCSCR.WORD |= SCIF_SCR_RIE;
        else SCIF2.SCSCR.WORD &= ~SCIF_SCR_RIE;
    }
}

BOOL CPU_USART_RxBufferFullInterruptState( int ComPortNum )
{
    switch(ComPortNum)
    {
        case 0 :
            return 0 != (SCIF0.SCSCR.WORD & SCIF_SCR_RIE);
        case 1:
            return 0 != (SCIF1.SCSCR.WORD & SCIF_SCR_RIE);
        case 2:
            return 0 != (SCIF2.SCSCR.WORD & SCIF_SCR_RIE);
    }
    
}

void CPU_USART_ProtectPins( int ComPortNum, BOOL On )
{
}



UINT32 CPU_USART_PortsCount()
{
    return TOTAL_USART_PORT;
}

void CPU_USART_GetPins( int ComPortNum, GPIO_PIN& rxPin, GPIO_PIN& txPin,GPIO_PIN& ctsPin, GPIO_PIN& rtsPin )
{   
    rxPin = GPIO_PIN_NONE; 
    txPin = GPIO_PIN_NONE; 
    ctsPin= GPIO_PIN_NONE; 
    rtsPin= GPIO_PIN_NONE; 

    return;
}

BOOL CPU_USART_SupportNonStandardBaudRate ( int ComPortNum )
{
    return FALSE;
}

void CPU_USART_GetBaudrateBoundary( int ComPortNum, UINT32& maxBaudrateHz, UINT32& minBaudrateHz )
{
    maxBaudrateHz = 0;
    minBaudrateHz = 0;
}

BOOL CPU_USART_IsBaudrateSupported( int ComPortNum, UINT32 & BaudrateHz )
{   
    return FALSE;
}

BOOL CPU_USART_TxHandshakeEnabledState( int comPort )
{
    return TRUE;
}

void USART_RxISR( UINT32 port )
{
    char c;

    GLOBAL_LOCK(irq);

    volatile unsigned short* rxReg;
    volatile unsigned char* rxData;

    switch(port)
    {
        case 0:
            rxReg  = &SCIF0.SCFSR.WORD;
            rxData = &SCIF0.SCFRDR;
            break;
            
        case 1: 
            rxReg  = &SCIF1.SCFSR.WORD;
            rxData = &SCIF1.SCFRDR;
            break;

        case 2:
            rxReg  = &SCIF2.SCFSR.WORD;
            rxData = &SCIF2.SCFRDR;
            break;

        default:
            ASSERT(FALSE);
            return;
    }
    
    while((*rxReg) & 0x0002)
    {
        c = *rxData;

        *rxReg &= 0xFFFD; 

        if(!USART_AddCharToRxBuffer(port, c))
        {
            break;
        }
        Events_Set( SYSTEM_EVENT_FLAG_COM_IN );

    }
}

void USART_TxISR( UINT32 port )
{
    char c;
    GLOBAL_LOCK(irq);

    if(USART_RemoveCharFromTxBuffer( port, c ))
    {
        CPU_USART_WriteCharToTxBuffer( port, c );
        Events_Set( SYSTEM_EVENT_FLAG_COM_OUT );
        
    }
    else
    {
        CPU_USART_TxBufferEmptyInterruptEnable( port, FALSE );
    }

}

