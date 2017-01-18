//-----------------------------------------------------------------------------
// Software that is described herein is for illustrative purposes only  
// which provides customers with programming information regarding the  
// products. This software is supplied "AS IS" without any warranties.  
// NXP Semiconductors assumes no responsibility or liability for the 
// use of the software, conveys no license or title under any patent, 
// copyright, or mask work right to the product. NXP Semiconductors 
// reserves the right to make changes in the software without 
// notification. NXP Semiconductors also make no representation or 
// warranty that such application will be suitable for the specified 
// use without further testing or modification. 
//-----------------------------------------------------------------------------

#include <tinyhal.h>
#include "..\LPC22XX.h"

//--//
// TEMP FOR COMPILE success purpose

#define SET_BITS(Var,Shift,Mask,fieldsMask) {Var = setFieldValue(Var,Shift,Mask,fieldsMask);}

static inline UINT32 setFieldValue(volatile UINT32 oldVal,UINT32 shift,UINT32 mask,UINT32 val)
{
    volatile UINT32 temp = oldVal;

    temp &= ~mask;
    temp |= val<<shift;
    return temp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_LPC22XX_USART_Driver"
#endif

LPC22XX_USART_Driver g_LPC22XX_USART_Driver;


#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//


BOOL CPU_USART_Initialize( int ComPortNum, int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue )
{
    return LPC22XX_USART_Driver::Initialize(ComPortNum, BaudRate, Parity, DataBits, StopBits, FlowValue);
}

BOOL CPU_USART_Uninitialize( int ComPortNum )
{
    return LPC22XX_USART_Driver::Uninitialize(ComPortNum);
}

BOOL CPU_USART_TxBufferEmpty( int ComPortNum )
{
    return LPC22XX_USART_Driver::TxBufferEmpty(ComPortNum);
}

BOOL CPU_USART_TxShiftRegisterEmpty( int ComPortNum )
{
    return LPC22XX_USART_Driver::TxShiftRegisterEmpty( ComPortNum );
}

void CPU_USART_WriteCharToTxBuffer( int ComPortNum, UINT8 c )
{
    LPC22XX_USART_Driver::WriteCharToTxBuffer( ComPortNum, c );
}

void CPU_USART_TxBufferEmptyInterruptEnable( int ComPortNum, BOOL Enable )
{
    LPC22XX_USART_Driver::TxBufferEmptyInterruptEnable( ComPortNum, Enable );
}

BOOL CPU_USART_TxBufferEmptyInterruptState( int ComPortNum )
{
    return LPC22XX_USART_Driver::TxBufferEmptyInterruptState( ComPortNum );
}

void CPU_USART_RxBufferFullInterruptEnable( int ComPortNum, BOOL Enable )
{
    LPC22XX_USART_Driver::RxBufferFullInterruptEnable( ComPortNum, Enable );
}

BOOL CPU_USART_RxBufferFullInterruptState( int ComPortNum )
{
    return LPC22XX_USART_Driver::RxBufferFullInterruptState( ComPortNum );
}

BOOL CPU_USART_TxHandshakeEnabledState( int comPort )
{
    return LPC22XX_USART_Driver::TxHandshakeEnabledState( comPort );
}

void CPU_USART_ProtectPins( int ComPortNum, BOOL On )
{
    LPC22XX_USART_Driver::ProtectPins( ComPortNum, On );
}

UINT32 CPU_USART_PortsCount()
{
    return TOTAL_USART_PORT;
}

void CPU_USART_GetPins( int ComPortNum,  GPIO_PIN& rxPin, GPIO_PIN& txPin,GPIO_PIN& ctsPin, GPIO_PIN& rtsPin )
{   
    return LPC22XX_USART_Driver::GetPins(ComPortNum, rxPin, txPin, ctsPin, rtsPin );
}

BOOL CPU_USART_SupportNonStandardBaudRate (int ComPortNum)
{
    return TRUE;
}

void CPU_USART_GetBaudrateBoundary(int ComPortNum, UINT32& maxBaudrateHz, UINT32& minBaudrateHz)
{
    return LPC22XX_USART_Driver::BaudrateBoundary(ComPortNum, maxBaudrateHz, minBaudrateHz);
}

BOOL CPU_USART_IsBaudrateSupported(int ComPortNum, UINT32& BaudrateHz)
{
    return LPC22XX_USART_Driver::IsBaudrateSupported(ComPortNum, BaudrateHz);
}


//--//


BOOL LPC22XX_USART_Driver::IsValidPortNum(int ComPortNum)
{
   if (ComPortNum == (int) 0) return TRUE;
   if (ComPortNum == (int) 1) return TRUE;
   return FALSE;
}

//--//

        
void LPC22XX_USART_Driver::UART_IntHandler (void *param)
{
   GLOBAL_LOCK(irq);

   ASSERT(LPC22XX_USART_Driver::IsValidPortNum((UINT32)param));    

   char c; 
   UINT32 ComNum = (UINT32)param;
   UINT32 i,rxcnt;  
   LPC22XX_USART& USARTC = LPC22XX::UART(ComNum);
   volatile UINT32 IIR_Value;
   volatile UINT32 LSR_Value;

    // Read data from Rx FIFO

    LSR_Value = USARTC.UART_LSR;

    while (LSR_Value & LPC22XX_USART::UART_LSR_RFDR)
    {
      UINT8 rxdata = (UINT8 )USARTC.SEL1.RBR.UART_RBR;
      rxcnt++;
      if ( 0 == (LSR_Value &
                   (
                       LPC22XX_USART::UART_LSR_PEI | 
                       LPC22XX_USART::UART_LSR_OEI |
                       LPC22XX_USART::UART_LSR_FEI                    
                   )    
                )
         )                                                         
      {
          // No errors in Rx data       
          USART_AddCharToRxBuffer(ComNum, rxdata);
          Events_Set( SYSTEM_EVENT_FLAG_COM_IN );
      }
      LSR_Value = USARTC.UART_LSR;               
  }
   // Send up to 2 bytes of Tx data

   for (i = 0; i <2; i++)
   {

       if (USART_RemoveCharFromTxBuffer( ComNum, c ))
       {
           WriteCharToTxBuffer( ComNum, c );
           Events_Set( SYSTEM_EVENT_FLAG_COM_OUT );
       }
       else
       {
           // Disable further TxBufferEmptyInterrupt. It will be
		       // enabled by the PAL uart driver when a character to the
		       // TxBuffer is added
           TxBufferEmptyInterruptEnable( ComNum, FALSE );
           break;
       }
   }

   // Check if IIR read is required to clear the uart Rx interrupt
   IIR_Value = USARTC.SEL3.IIR.UART_IIR; 

}

//--//

BOOL LPC22XX_USART_Driver::Initialize( int ComPortNum, int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue )
{

    GLOBAL_LOCK(irq);
    LPC22XX_USART& USARTC = LPC22XX::UART(ComPortNum);
    UINT32 divisor;  
    BOOL   fRet = TRUE;
    
    
    ASSERT(LPC22XX_USART_Driver::IsValidPortNum(ComPortNum));
    
    //calculate the divisor that's required.
    divisor     =  ((LPC22XX_USART::c_ClockRate / (BaudRate * 16)));
    
    // CWS: Disable interrupts
    USARTC.UART_LCR = 0; // prepare to Init UART
    USARTC.SEL2.IER.UART_IER &= ~(LPC22XX_USART::UART_IER_INTR_ALL_SET);          // Disable all UART interrupts
    /* CWS: Set baud rate to baudRate bps */
    USARTC.UART_LCR|= LPC22XX_USART::UART_LCR_DLAB;                                          // prepare to access Divisor
    USARTC.SEL1.DLL.UART_DLL = divisor & 0xFF;      //GET_LSB(divisor);                                                      // Set baudrate.
    USARTC.SEL2.DLM.UART_DLM = (divisor>>8) & 0xFF; // GET_MSB(divisor);
    USARTC.UART_LCR&= ~LPC22XX_USART::UART_LCR_DLAB;                                              // prepare to access RBR, THR, IER
    // CWS: Set port for 8 bit, 1 stop, no parity  

    // DataBit range 5-8
    if(5 <= DataBits && DataBits <= 8)
    {
        SET_BITS(USARTC.UART_LCR,
                 LPC22XX_USART::UART_LCR_WLS_shift,
                 LPC22XX_USART::UART_LCR_WLS_mask,
                 DataBits-5);
    }
    else
    {   // not supported
        fRet = FALSE;

        // set up 8 data bits incase return value is ignored
        SET_BITS(USARTC.UART_LCR,
                 LPC22XX_USART::UART_LCR_WLS_shift,
                 LPC22XX_USART::UART_LCR_WLS_mask,
                 LPC22XX_USART::UART_LCR_WLS_8_BITS);
    }

    switch(StopBits)
    {
        case USART_STOP_BITS_ONE:
            USARTC.UART_LCR |= LPC22XX_USART::UART_LCR_NSB_1_STOPBITS;
            break;
        case USART_STOP_BITS_ONEPOINTFIVE:
            USARTC.UART_LCR |= LPC22XX_USART::UART_LCR_NSB_15_STOPBITS;
            break;

        // not supported
        case USART_STOP_BITS_NONE:
        default:
            fRet = FALSE;
            break;
    }

    switch(Parity)
    {
        case USART_PARITY_SPACE:
            USARTC.UART_LCR |= LPC22XX_USART::UART_LCR_SPE;
        case USART_PARITY_EVEN:
            USARTC.UART_LCR |= (LPC22XX_USART::UART_LCR_EPE | LPC22XX_USART::UART_LCR_PBE);
            break;

        case USART_PARITY_MARK:
            USARTC.UART_LCR |= LPC22XX_USART::UART_LCR_SPE;
        case USART_PARITY_ODD:
            USARTC.UART_LCR |= LPC22XX_USART::UART_LCR_PBE;
            break;       

        case USART_PARITY_NONE:
            USARTC.UART_LCR &= ~LPC22XX_USART::UART_LCR_PBE;
            break;
            
        // not supported
        default:
            fRet = FALSE;
            break;
    }
    
    // CWS: Set the RX FIFO trigger level (to 8 bytes), reset RX, TX FIFO 
    USARTC.SEL3.FCR.UART_FCR =  (LPC22XX_USART::UART_FCR_RFITL_01>> LPC22XX_USART::UART_FCR_RFITL_shift )  |
                                LPC22XX_USART::UART_FCR_TFR      | 
                                LPC22XX_USART::UART_FCR_RFR      |
                                LPC22XX_USART::UART_FCR_FME;

    ProtectPins( ComPortNum, FALSE );

    CPU_INTC_ActivateInterrupt( LPC22XX_USART::getIntNo(ComPortNum),
                                UART_IntHandler,
                                (void *)ComPortNum);    
    
    return fRet;
}

BOOL LPC22XX_USART_Driver::Uninitialize                    ( int ComPortNum               )
{

    GLOBAL_LOCK(irq);
    LPC22XX_USART& USARTC = LPC22XX::UART(ComPortNum);
    
    
    ASSERT(LPC22XX_USART_Driver::IsValidPortNum(ComPortNum));


    ProtectPins( ComPortNum, TRUE );

    
    // CWS: Disable interrupts 

    USARTC.UART_LCR = 0; // prepare to Init UART
    USARTC.SEL2.IER.UART_IER &= ~(LPC22XX_USART::UART_IER_INTR_ALL_SET);         // Disable all UART interrupt

    return TRUE;
}

BOOL LPC22XX_USART_Driver::TxBufferEmpty ( int ComPortNum )
{
    LPC22XX_USART& USARTC = LPC22XX::UART(ComPortNum);
    
    
    ASSERT(LPC22XX_USART_Driver::IsValidPortNum(ComPortNum));
    
    
    if (USARTC.UART_LSR & LPC22XX_USART::UART_LSR_THRE) return TRUE; 
    return FALSE;
}

BOOL LPC22XX_USART_Driver::TxShiftRegisterEmpty ( int ComPortNum )
{
    LPC22XX_USART& USARTC = LPC22XX::UART(ComPortNum);
    
    
    ASSERT(LPC22XX_USART_Driver::IsValidPortNum(ComPortNum));
    
    
    if (USARTC.UART_LSR & LPC22XX_USART::UART_LSR_TE) return TRUE; 
    return FALSE;


}

void LPC22XX_USART_Driver::WriteCharToTxBuffer ( int ComPortNum, UINT8 c )
{
    UINT32 ui32Char = (UINT32) c;
    LPC22XX_USART& USARTC = LPC22XX::UART(ComPortNum);
    
    ASSERT(LPC22XX_USART_Driver::IsValidPortNum(ComPortNum));    
    
    while (FALSE == TxBufferEmpty(ComPortNum));        //wait till ready.

    //transmit the character.
    USARTC.SEL1.THR.UART_THR = ui32Char;
    // SET_BITS(USARTC.SEL1.THR.UART_THR,LPC22XX_USART::UART_THR_DATA_shift,LPC22XX_USART::UART_THR_DATA_mask,ui32Char);
    
    // special handling to newline ('\n' -> '\n\r').
   // if (c == '\n') WriteCharToTxBuffer(ComPortNum, '\r');
}

void LPC22XX_USART_Driver::TxBufferEmptyInterruptEnable( int ComPortNum, BOOL Enable  )
{
    GLOBAL_LOCK(irq);

    LPC22XX_USART& USARTC = LPC22XX::UART(ComPortNum);
    
    ASSERT(LPC22XX_USART_Driver::IsValidPortNum(ComPortNum));    

    if (Enable)
    {
       USARTC.SEL2.IER.UART_IER |=  (LPC22XX_USART::UART_IER_THREIE);      

        char c;
        for (int i=0; i<2; i++)
        {
            if (USART_RemoveCharFromTxBuffer( ComPortNum, c ))
            {
                WriteCharToTxBuffer( ComPortNum, c );
                Events_Set( SYSTEM_EVENT_FLAG_COM_OUT );
            }
            else
                break;
        }

       
    }
    else
    {
       USARTC.SEL2.IER.UART_IER &= ~(LPC22XX_USART::UART_IER_THREIE);              
    }

}

BOOL LPC22XX_USART_Driver::TxBufferEmptyInterruptState ( int ComPortNum )
{
    LPC22XX_USART& USARTC = LPC22XX::UART(ComPortNum);
    
    ASSERT(LPC22XX_USART_Driver::IsValidPortNum(ComPortNum));    

    if (USARTC.SEL2.IER.UART_IER & (LPC22XX_USART::UART_IER_THREIE)) return TRUE;
    return FALSE;
}

void LPC22XX_USART_Driver::RxBufferFullInterruptEnable ( int ComPortNum, BOOL Enable  )
{
    GLOBAL_LOCK(irq);

    LPC22XX_USART& USARTC = LPC22XX::UART(ComPortNum);
    
    ASSERT(LPC22XX_USART_Driver::IsValidPortNum(ComPortNum));    

    if (Enable)
    {
       USARTC.SEL2.IER.UART_IER |=  (LPC22XX_USART::UART_IER_RDAIE );      
    }
    else
    {
       USARTC.SEL2.IER.UART_IER &= ~(LPC22XX_USART::UART_IER_RDAIE);               
    }
}

BOOL LPC22XX_USART_Driver::RxBufferFullInterruptState ( int ComPortNum )
{
    LPC22XX_USART& USARTC = LPC22XX::UART(ComPortNum);
    
    ASSERT(LPC22XX_USART_Driver::IsValidPortNum(ComPortNum));    

    if (USARTC.SEL2.IER.UART_IER & (LPC22XX_USART::UART_IER_RDAIE)) return TRUE;
    return FALSE;

}

BOOL LPC22XX_USART_Driver::TxHandshakeEnabledState( int ComPortNum )
{
    // TODO: Add handshake code
    return TRUE;   
}

void LPC22XX_USART_Driver::ProtectPins ( int ComPortNum, BOOL On )
{
    ASSERT(LPC22XX_USART_Driver::IsValidPortNum(ComPortNum));    

    static BOOL COM1_PinsProtected = TRUE;   // start out doing work on first unprotect
    static BOOL COM2_PinsProtected = TRUE;   // start out doing work on first unprotect

    GLOBAL_LOCK(irq);

    UINT32                  SER_TXD;
    UINT32                  SER_RXD;
    BOOL*                   PinsProtected;

    switch(ComPortNum)
    {
    case c_COM1: SER_TXD = LPC22XX_USART::c_SER1_TXD; SER_RXD = LPC22XX_USART::c_SER1_RXD; PinsProtected = &COM1_PinsProtected; break;
    case c_COM2: SER_TXD = LPC22XX_USART::c_SER2_TXD; SER_RXD = LPC22XX_USART::c_SER2_RXD; PinsProtected = &COM2_PinsProtected; break;
    default: return;
    }

    if (On) 
    {
    
        if(!*PinsProtected)
        {
            *PinsProtected = TRUE;

            TxBufferEmptyInterruptEnable( ComPortNum, FALSE );
            // TODO Add config for uart pin protected state
            CPU_GPIO_EnableOutputPin( SER_TXD, RESISTOR_DISABLED );

            RxBufferFullInterruptEnable( ComPortNum, FALSE );
            // TODO Add config for uart pin protected state
            CPU_GPIO_EnableOutputPin( SER_RXD, RESISTOR_DISABLED );
        }
    }
    else 
    {
        if(*PinsProtected)
        {
            *PinsProtected = FALSE;

            // Connect pin to UART
            CPU_GPIO_DisablePin( SER_TXD, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );

            // Connect pin to UART
            CPU_GPIO_DisablePin( SER_RXD, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );

            TxBufferEmptyInterruptEnable( ComPortNum, TRUE );

            RxBufferFullInterruptEnable( ComPortNum, TRUE );

        }
    }
    
}
void LPC22XX_USART_Driver::GetPins(int comPort, GPIO_PIN& rxPin, GPIO_PIN& txPin, GPIO_PIN& ctsPin, GPIO_PIN& rtsPin)
{
    switch(comPort)
    {
        case c_COM1: 
            txPin  = LPC22XX_USART::c_SER1_TXD; 
            rxPin  = LPC22XX_USART::c_SER1_RXD; 
            ctsPin = GPIO_PIN_NONE;
            rtsPin = GPIO_PIN_NONE;            
            break;
        case c_COM2: 
            txPin  = LPC22XX_USART::c_SER2_TXD; 
            rxPin  = LPC22XX_USART::c_SER2_RXD; 
            ctsPin = LPC22XX_USART::c_SER2_CTS; 
            rtsPin = LPC22XX_USART::c_SER2_RTS; 

            break;

        default: return;
    }
}

void  LPC22XX_USART_Driver::BaudrateBoundary(int ComPortNum, UINT32& maxBaudrateHz, UINT32& minBaudrateHz)
{
    // same for both UART
    maxBaudrateHz = LPC22XX_USART::c_MAX_BAUDRATE;
    minBaudrateHz = LPC22XX_USART::c_MIN_BAUDRATE;

}

BOOL LPC22XX_USART_Driver::IsBaudrateSupported(int ComPortNum, UINT32 & BaudrateHz)
{


    if (BaudrateHz <= LPC22XX_USART::c_MAX_BAUDRATE)
    {
        return true;
    }
    else
    {

        BaudrateHz = LPC22XX_USART::c_MAX_BAUDRATE;
        return false;
    }
}

#if 0
{
    UINT32 divisor;
    UINT32 newBR =  LPC22XX_USART::c_MAX_BAUDRATE;

    // same for both UART
    if (BaudrateHz <= LPC22XX_USART::c_MAX_BAUDRATE)
    {

        divisor =  ((LPC22XX_USART::c_ClockRate / (BaudrateHz * 16)));
        newBR   = ((LPC22XX_USART::c_ClockRate / (divisor * 16))); 

        if (newBR == BaudrateHz)
        {
            debug_printf( "TTTsupport br %d \r\n", BaudrateHz );
            return true;
        }
    }
    BaudrateHz = newBR;
    debug_printf( "FFFsupport br %d \r\n", BaudrateHz );    
    return false;


}
#endif


