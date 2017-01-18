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
 *    Copyright (C) 2010 Renesas Electronics America Inc. All rights reserved.
 */
 
#include <tinyhal.h>

#include "scif.h"
#include "../include/iodefine.h"

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
         SCI0.SCSCR.BYTE = 0;       // SCSCR Clear
         SCI0.SCSCR.BYTE = 0x01;    // Internal Clock            
         SCI0.SCSMR.BYTE = scsmr;
         SCI0.SCBRR = BRR_CALC(BaudRate);
        // Delay 1 ms
        i = 31250;      // 8 ns/instruction * 4 instructions * loop_count = 1ms = 1000000 ns
        while (i--);    // loop count = 31250

        SCI0.SCSCR.BYTE = 0x31; // Enable Transmit & Receive
        
        //SCI0 Transmit interrput and Receive interrupt : level 5
        INTC.IPR16.WORD |= 0x5000;
    }
    else if( ComPortNum == 1 ){
        SCI1.SCSCR.BYTE = 0;        // SCSCR Clear
        SCI1.SCSCR.BYTE = 0x01; // Internal Clock
        SCI1.SCSMR.BYTE = scsmr;
        SCI1.SCBRR = BRR_CALC(BaudRate);
        // Delay 1 ms
        i = 31250;      // 8 ns/instruction * 4 instructions * loop_count = 1ms = 1000000 ns
        while (i--);    // loop count = 31250
       
        PFC.PACRL2.WORD |= 0x0006;
        PFC.PACRL1.WORD |= 0x6000;
        SCI1.SCSCR.BYTE = 0x31; // Enable Transmit & Receive
        
        //SCI1 Transmit interrput and Receive interrupt : level 5
        INTC.IPR16.WORD |= 0x0500;
    }
    else if( ComPortNum == 2 ){
        SCI2.SCSCR.BYTE = 0;        // SCSCR Clear
        SCI2.SCSCR.BYTE = 0x01; // Internal Clock
        SCI2.SCSMR.BYTE = scsmr;
        SCI2.SCBRR = BRR_CALC(BaudRate);
        // Delay 1 ms
        i = 31250;      // 8 ns/instruction * 4 instructions * loop_count = 1ms = 1000000 ns
        while (i--);    // loop count = 31250

        SCI2.SCSCR.BYTE = 0x31; // Enable Transmit & Receive

        //SCI2 Transmit interrput and Receive interrupt : level 5
        INTC.IPR16.WORD |= 0x0050;  
    }
    else if( ComPortNum == 3 ){

        SCI4.SCSCR.BYTE = 0;        // SCSCR Clear
        SCI4.SCSCR.BYTE = 0x01; // Internal Clock
        SCI4.SCSMR.BYTE = scsmr;
        SCI4.SCBRR = BRR_CALC(BaudRate);
        // Delay 1 ms
        i = 31250;      // 8 ns/instruction * 4 instructions * loop_count = 1ms = 1000000 ns
        while (i--);    // loop count = 31250

        SCI4.SCSCR.BYTE = 0x31; // Enable Transmit & Receive
    
        INTC.IPR17.WORD |= 0x0005;
    }
    else
        return FALSE;
            
     
    CPU_USART_RxBufferFullInterruptEnable(1, TRUE);  
    return TRUE;
}

BOOL CPU_USART_Uninitialize( int ComPortNum )
{
    switch(ComPortNum)
    {
        case 0:
            SCI0.SCSCR.BYTE = 0;       // SCSCR Clear
            break;
        case 1:
            SCI1.SCSCR.BYTE = 0;       // SCSCR Clear
            break;
        case 2:
            SCI2.SCSCR.BYTE = 0;       // SCSCR Clear
            break;
        case 3:
            SCI4.SCSCR.BYTE = 0;       // SCSCR Clear
            break;
    }

    return TRUE;
}

BOOL CPU_USART_TxBufferEmpty( int ComPortNum )
{
    switch(ComPortNum)
    {
        case 0:
            return (SCI0.SCSSR.BYTE & 0x84) != 84;
        case 1:
           return (SCI1.SCSSR.BYTE & 0x84)!= 84;
        case 2:
            return (SCI2.SCSSR.BYTE & 0x84)!= 84;
        case 3:
            return (SCI4.SCSSR.BYTE & 0x84)!= 84;
    }

    return TRUE;
}

BOOL CPU_USART_TxShiftRegisterEmpty( int ComPortNum )
{
    return CPU_USART_TxBufferEmpty(ComPortNum);
}

void CPU_USART_WriteCharToTxBuffer( int ComPortNum, UINT8 c )
{

    unsigned int i = 0;
//Make sure SCSSR.SCIF_SR_TDRE == 1
    while (FALSE == CPU_USART_TxShiftRegisterEmpty(ComPortNum));        //wait till ready.
    if( ComPortNum == 0 ){
        while((SCI0.SCSSR.BYTE & SCIF_SR_TDRE) != SCIF_SR_TDRE);
        SCI0.SCTDR = c;
        while(SCI0.SCSSR.BYTE & SCIF_SR_TDRE & SCIF_SR_TEND);
        SCI0.SCSSR.BYTE &= ~SCIF_SR_TDRE;
        SCI0.SCSSR.BYTE &= ~SCIF_SR_TEND;
        while(SCI0.SCSSR.BYTE & SCIF_SR_TEND);
    }
    else
    if( ComPortNum == 1 ){
        while((SCI1.SCSSR.BYTE & SCIF_SR_TDRE) != SCIF_SR_TDRE);    
        SCI1.SCTDR = c;
        while(SCI1.SCSSR.BYTE & SCIF_SR_TDRE & SCIF_SR_TEND);
        SCI1.SCSSR.BYTE &= ~SCIF_SR_TDRE;
        SCI1.SCSSR.BYTE &= ~SCIF_SR_TEND;
        while(SCI1.SCSSR.BYTE & SCIF_SR_TEND);
    }
    else
    if( ComPortNum == 2 ){
        while((SCI2.SCSSR.BYTE & SCIF_SR_TDRE) != SCIF_SR_TDRE);
        SCI2.SCTDR = c;
        while(SCI2.SCSSR.BYTE & SCIF_SR_TDRE & SCIF_SR_TEND);
        SCI2.SCSSR.BYTE &= ~SCIF_SR_TDRE;
        SCI2.SCSSR.BYTE &= ~SCIF_SR_TEND;
        while(SCI2.SCSSR.BYTE & SCIF_SR_TEND);
    }
    else
    if( ComPortNum == 3 ){
        while((SCI4.SCSSR.BYTE & SCIF_SR_TDRE) != SCIF_SR_TDRE);
        SCI4.SCTDR = c;
        while(SCI4.SCSSR.BYTE & SCIF_SR_TDRE & SCIF_SR_TEND);
        SCI4.SCSSR.BYTE &= ~SCIF_SR_TDRE;
        SCI4.SCSSR.BYTE &= ~SCIF_SR_TEND;
        while(SCI4.SCSSR.BYTE & SCIF_SR_TEND);
    }

        // Delay 
        i = 5000;       // 8 ns/instruction * 4 instructions * loop_count  
        while (i--);    // loop count = 2000
    
}

void CPU_USART_TxBufferEmptyInterruptEnable( int ComPortNum, BOOL Enable )
{
    if( ComPortNum == 0 ){
        if(Enable) SCI0.SCSCR.BYTE |= SCIF_SCR_TIE;
        else SCI0.SCSCR.BYTE &= ~SCIF_SCR_TIE;
    }
    else
    if( ComPortNum == 1 ){
        if(Enable) SCI1.SCSCR.BYTE |= SCIF_SCR_TIE;
        else SCI1.SCSCR.BYTE &= ~SCIF_SCR_TIE;
    }
    else
    if( ComPortNum == 2 ){
        if(Enable) SCI2.SCSCR.BYTE |= SCIF_SCR_TIE;
        else SCI2.SCSCR.BYTE &= ~SCIF_SCR_TIE;
    }
    else
    if( ComPortNum == 3 ){
        if(Enable) SCI4.SCSCR.BYTE |= SCIF_SCR_TIE;
        else SCI4.SCSCR.BYTE &= ~SCIF_SCR_TIE;
    }
}

BOOL CPU_USART_TxBufferEmptyInterruptState( int ComPortNum )
{
    switch(ComPortNum)
    {
        case 0:
            return 0 != (SCI0.SCSCR.BYTE & SCIF_SCR_TIE);
        case 1:
            return 0 != (SCI1.SCSCR.BYTE & SCIF_SCR_TIE);
        case 2:
            return 0 != (SCI2.SCSCR.BYTE & SCIF_SCR_TIE);
        case 3:
            return 0 != (SCI4.SCSCR.BYTE & SCIF_SCR_TIE);

    }

    return TRUE;
}

void CPU_USART_RxBufferFullInterruptEnable( int ComPortNum, BOOL Enable )
{

    if( ComPortNum == 0 ){
        if(Enable) SCI0.SCSCR.BYTE |= SCIF_SCR_RIE;
        else SCI0.SCSCR.BYTE &= ~SCIF_SCR_RIE;
    }
    else
    if( ComPortNum == 1 ){
        if(Enable) SCI1.SCSCR.BYTE |= SCIF_SCR_RIE;
        else SCI1.SCSCR.BYTE &= ~SCIF_SCR_RIE;
    }
    else
    if( ComPortNum == 2 ){
        if(Enable) SCI2.SCSCR.BYTE |= SCIF_SCR_RIE;
        else SCI2.SCSCR.BYTE &= ~SCIF_SCR_RIE;
    }
    else
    if( ComPortNum == 3 ){
        if(Enable) SCI4.SCSCR.BYTE |= SCIF_SCR_RIE;
        else SCI4.SCSCR.BYTE &= ~SCIF_SCR_RIE;
    }
}

BOOL CPU_USART_RxBufferFullInterruptState( int ComPortNum )
{
    switch(ComPortNum)
    {
        case 0 :
            return 0 != (SCI0.SCSCR.BYTE & SCIF_SCR_RIE);
        case 1:
            return 0 != (SCI1.SCSCR.BYTE & SCIF_SCR_RIE);
        case 2:
            return 0 != (SCI2.SCSCR.BYTE & SCIF_SCR_RIE);
        case 3:
            return 0 != (SCI4.SCSCR.BYTE & SCIF_SCR_RIE);
    }
    return TRUE;
}

void CPU_USART_ProtectPins( int ComPortNum, BOOL On )
{
}

UINT32 CPU_USART_PortsCount()
{
    return 0;
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
    static bool flag = true;
    GLOBAL_LOCK(irq);

    volatile unsigned char* rxReg;
    volatile unsigned char* rxData;

    CPU_USART_RxBufferFullInterruptEnable(port, FALSE);
    switch(port)
    {
        case 0:
            rxReg  = &SCI0.SCSSR.BYTE;
            rxData = &SCI0.SCRDR;
            break;
            
        case 1: 
            rxReg  = &SCI1.SCSSR.BYTE;
            rxData = &SCI1.SCRDR;
            break;

        case 2:
            rxReg  = &SCI2.SCSSR.BYTE;
            rxData = &SCI2.SCRDR;
            break;
            
         case 3:
            rxReg  = &SCI4.SCSSR.BYTE;
            rxData = &SCI4.SCRDR;
            break;

        default:
            ASSERT(FALSE);
            return;
    }
    
    *rxReg &= 0xC7;
    while((*rxReg) & 0x40)
    {
        c = *rxData;

        *rxReg &= 0xBF; 

        if(!USART_AddCharToRxBuffer(port, c))
        {
            break;
        }
#if 0  // Currently this the workaround
      if(flag)
      {
        USART_AddCharToRxBuffer(port, 'S');
        USART_AddCharToRxBuffer(port, 'p');
        USART_AddCharToRxBuffer(port, 'k');
        USART_AddCharToRxBuffer(port, 't');
        USART_AddCharToRxBuffer(port, 'V');
        USART_AddCharToRxBuffer(port, '1');
        USART_AddCharToRxBuffer(port, 0x00);
        USART_AddCharToRxBuffer(port, 0x5A);
        USART_AddCharToRxBuffer(port, 0x2D);
        USART_AddCharToRxBuffer(port, 0x41);
        USART_AddCharToRxBuffer(port, 0x79);
        USART_AddCharToRxBuffer(port, 0x94);
        USART_AddCharToRxBuffer(port, 0xB9);
        USART_AddCharToRxBuffer(port, 0x43);
        USART_AddCharToRxBuffer(port, 0xb7);
        USART_AddCharToRxBuffer(port, 0x00);
        USART_AddCharToRxBuffer(port, 0x00);
        USART_AddCharToRxBuffer(port, 0x00);
        USART_AddCharToRxBuffer(port, 0x00);
        USART_AddCharToRxBuffer(port, 0x3D);
        USART_AddCharToRxBuffer(port, 0x24);
        USART_AddCharToRxBuffer(port, 0x00);
        USART_AddCharToRxBuffer(port, 0x00);
        USART_AddCharToRxBuffer(port, 0x03);
        USART_AddCharToRxBuffer(port, 0x00);
        USART_AddCharToRxBuffer(port, 0x00);
        USART_AddCharToRxBuffer(port, 0x00);
        USART_AddCharToRxBuffer(port, 0x08);
        USART_AddCharToRxBuffer(port, 0x00);
        USART_AddCharToRxBuffer(port, 0x00);
        USART_AddCharToRxBuffer(port, 0x00);
        USART_AddCharToRxBuffer(port, 0x02);
        USART_AddCharToRxBuffer(port, 0x00);
        USART_AddCharToRxBuffer(port, 0x00);
        USART_AddCharToRxBuffer(port, 0x00);
        USART_AddCharToRxBuffer(port, 0x00);
        USART_AddCharToRxBuffer(port, 0x00);
        USART_AddCharToRxBuffer(port, 0x00);
        USART_AddCharToRxBuffer(port, 0x00);
        flag = false;
      }
#endif
    }
    CPU_USART_RxBufferFullInterruptEnable(port, TRUE);
}

void USART_RxErrorISR( UINT32 port )
{
    GLOBAL_LOCK(irq);

    volatile unsigned char* rxReg;
    volatile unsigned char* rxData;

    CPU_USART_RxBufferFullInterruptEnable(port, FALSE);
    switch(port)
    {
        case 0:
            rxReg  = &SCI0.SCSSR.BYTE;
            break;
            
        case 1: 
            rxReg  = &SCI1.SCSSR.BYTE;
            break;

        case 2:
            rxReg  = &SCI2.SCSSR.BYTE;
            break;
            
         case 3:
            rxReg  = &SCI4.SCSSR.BYTE;
            break;

        default:
            ASSERT(FALSE);
            return;
    }

    *rxReg &= 0x87;
#if 0
    USART_AddCharToRxBuffer(port, 'M');
    USART_AddCharToRxBuffer(port, 'S');
    USART_AddCharToRxBuffer(port, 'p');
    USART_AddCharToRxBuffer(port, 'k');
    USART_AddCharToRxBuffer(port, 't');
    USART_AddCharToRxBuffer(port, 'V');
    USART_AddCharToRxBuffer(port, '1');
    USART_AddCharToRxBuffer(port, 0x00);
    USART_AddCharToRxBuffer(port, 0x5A);
    USART_AddCharToRxBuffer(port, 0x2D);
    USART_AddCharToRxBuffer(port, 0x41);
    USART_AddCharToRxBuffer(port, 0x79);
    USART_AddCharToRxBuffer(port, 0x94);
    USART_AddCharToRxBuffer(port, 0xB9);
    USART_AddCharToRxBuffer(port, 0x43);
    USART_AddCharToRxBuffer(port, 0xb7);
    USART_AddCharToRxBuffer(port, 0x00);
    USART_AddCharToRxBuffer(port, 0x00);
    USART_AddCharToRxBuffer(port, 0x00);
    USART_AddCharToRxBuffer(port, 0x00);
    USART_AddCharToRxBuffer(port, 0x3D);
    USART_AddCharToRxBuffer(port, 0x24);
    USART_AddCharToRxBuffer(port, 0x00);
    USART_AddCharToRxBuffer(port, 0x00);
    USART_AddCharToRxBuffer(port, 0x03);
    USART_AddCharToRxBuffer(port, 0x00);
    USART_AddCharToRxBuffer(port, 0x00);
    USART_AddCharToRxBuffer(port, 0x00);
    USART_AddCharToRxBuffer(port, 0x08);
    USART_AddCharToRxBuffer(port, 0x00);
    USART_AddCharToRxBuffer(port, 0x00);
    USART_AddCharToRxBuffer(port, 0x00);
    USART_AddCharToRxBuffer(port, 0x02);
    USART_AddCharToRxBuffer(port, 0x00);
    USART_AddCharToRxBuffer(port, 0x00);
    USART_AddCharToRxBuffer(port, 0x00);
    USART_AddCharToRxBuffer(port, 0x00);
    USART_AddCharToRxBuffer(port, 0x00);
    USART_AddCharToRxBuffer(port, 0x00);
    USART_AddCharToRxBuffer(port, 0x00);
#endif
    CPU_USART_RxBufferFullInterruptEnable(port, TRUE);
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

