////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_PXA271_USART_Driver"
#endif

PXA271_USART_Driver g_PXA271_USART_Driver;

    struct USART_PORT_INFO
    {
        UINT32        TXD_Pin;
        GPIO_ALT_MODE TXD_Pin_function;
        UINT32        RXD_Pin;
        GPIO_ALT_MODE RXD_Pin_function;
        UINT32        CTS_Pin;
        GPIO_ALT_MODE CTS_Pin_function;
        UINT32        RTS_Pin;
        GPIO_ALT_MODE RTS_Pin_function;
        UINT32        ClockIndex;
        BOOL          PinsProtected;
        BOOL          CTS_Pin_used;
        BOOL          RTS_Pin_used;
    };

    static struct USART_PORT_INFO All_USART_ports[] =
    {
        {
            PXA271_USART::c_STD_TXD,                // TXD_Pin
            GPIO_ALT_MODE_1,                        // TXD_Pin_function
            PXA271_USART::c_STD_RXD,                // RXD_Pin
            GPIO_ALT_MODE_2,                        // RXD_Pin_function
            PXA271_GPIO::c_Pin_None,                // CTS_Pin
            GPIO_ALT_PRIMARY,                       // CTS_Pin_function
            PXA271_GPIO::c_Pin_None,                // RTS_Pin
            GPIO_ALT_PRIMARY,                       // RTS_Pin_function
            PXA271_CLOCK_MANAGER::CKEN__STUART,     // ClockIndex
            TRUE,                                   // PinsProtected
            FALSE,                                  // CTS_Pin_used
            FALSE                                   // RTS_Pin_used
        },
        {
            PXA271_USART::c_BT_TXD,                 // TXD_Pin
            GPIO_ALT_MODE_2,                        // TXD_Pin_function
            PXA271_USART::c_BT_RXD,                 // RXD_Pin
            GPIO_ALT_MODE_1,                        // RXD_Pin_function
            PXA271_USART::c_BT_CTS,                 // CTS_Pin
            GPIO_ALT_MODE_1,                        // CTS_Pin_function
            PXA271_USART::c_BT_RTS,                 // RTS_Pin
            GPIO_ALT_MODE_2,                        // RTS_Pin_function
            PXA271_CLOCK_MANAGER::CKEN__BTUART,     // ClockIndex
            TRUE,                                   // PinsProtected
            FALSE,                                  // CTS_Pin_used
            FALSE                                   // RTS_Pin_used
        },
        {
            PXA271_USART::c_FF_TXD,                 // TXD_Pin
            GPIO_ALT_MODE_3,                        // TXD_Pin_function
            PXA271_USART::c_FF_RXD,                 // RXD_Pin
            GPIO_ALT_MODE_3,                        // RXD_Pin_function
            PXA271_USART::c_FF_CTS,                 // CTS_Pin
            GPIO_ALT_MODE_3,                        // CTS_Pin_function
            PXA271_USART::c_FF_RTS,                 // RTS_Pin
            GPIO_ALT_MODE_3,                        // RTS_Pin_function
            PXA271_CLOCK_MANAGER::CKEN__FFUART,     // ClockIndex
            TRUE,                                   // PinsProtected
            FALSE,                                  // CTS_Pin_used
            FALSE                                   // RTS_Pin_used
        }
    };

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif
    
    struct BaudRateLookup
    {
        UINT32 BaudRate;
        UINT32 DLL;
        UINT32 DLH;
    };
    const BaudRateLookup c_BaudRateLookup[] =
    {
    // The PXA271 USART hardware gets its baud rate clock from (312MHz / 21) / 16 = 928571 Hz
    // rather than 921600 Hz, so each baud rate has a built in 0.76% error.
        {230400, 0x04,  0},      // Actually 232143
        {115200, 0x08,  0},      // Actually 116071
        { 57600, 0x10,  0},      // Actually 58035
        { 38400, 0x18,  0},      // Actually 38690
        { 19200, 0x30,  0},      // Actually 19345
        {  9600, 0x61,  0},      // Actually 9573
        {  4800, 0xC1,  0},      // Actually 4811
        {  2400, 0x180, 0},      // Actually 2418
        {  1200, 0x300, 0},      // Actually 1209
        {   300, 0xC00, 0},      // Actually 302        
        {     0,    0, 0}  //for not defined baudrate
    };

//--//

BOOL PXA271_USART_Driver::Initialize( int comPort, int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue )
{
    BOOL fRet = TRUE;
    
    //--//

    GLOBAL_LOCK(irq);
    UINT32       dll;
    UINT32       dlh;

    g_PXA271_USART_Driver.m_RefFlags[comPort] = 0;

    if( FlowValue & USART_FLOW_HW_OUT_EN )
    {
        // Can't ask for HW flow control if there is no pin available to implement it
        if( All_USART_ports[comPort].CTS_Pin == PXA271_GPIO::c_Pin_None )
            return FALSE;
        All_USART_ports[comPort].CTS_Pin_used = TRUE;
    }
    if( FlowValue & USART_FLOW_HW_IN_EN )
    {
        // Can't ask for HW flow control if there is no pin available to implement it
        if( All_USART_ports[comPort].RTS_Pin == PXA271_GPIO::c_Pin_None )
            return FALSE;
        All_USART_ports[comPort].RTS_Pin_used = TRUE;
    }

    ProtectPins( comPort, FALSE );      // Connect pins to the com port and connect its clock

    PXA271_USART& USART = PXA271::USART( comPort );

    const BaudRateLookup* ptr = c_BaudRateLookup;

    while(true)
    {
        if(ptr->BaudRate == BaudRate)
        {
            dll     = ptr->DLL;
            dlh     = ptr->DLH;
            break;
        }

        if(ptr->BaudRate == 0)
        {
            // TODO TODO TODO if there is not preset baud rate, calculate the parameter here
            return FALSE;
        }

        ptr++;
    }

    //--//

    // NOTE:
    // The baud rate should NOT be set while the clock to the UART is enabled.  If so, the baud rate may
    // not be accepted.  The register will read back as if it were, but the actual baud rate will be incorrect.
    USART.LCR |= PXA271_USART::LCR__DLAB;               // Enable the divisor latch registers
    USART.DLL = dll;
    USART.DLH = dlh;                                    // Set up the selected baud rate
    USART.LCR &= ~PXA271_USART::LCR__DLAB;              // Exchange divisor latch control for FIFO access

    // Set up 8 bits of data and 1 stop bit
    // LCR__STB_2 is 2 stop bits except for when data bits are 5 then it is 1 1/2 stop bits.
    switch(StopBits)
    {
        case USART_STOP_BITS_ONE:
            // do nothing (default)
            break;

        case USART_STOP_BITS_TWO:
        case USART_STOP_BITS_ONEPOINTFIVE: // based on data bits, this will either be 2 or 1.5 stop bits
            USART.LCR |= PXA271_USART::LCR__STB_2;
            break;

        // not supported
        case USART_STOP_BITS_NONE:
        default:
            fRet = FALSE;
            break;
    }

    // DataBits falls in the range of 5 to 8
    if(5 <= DataBits && DataBits <= 8)
    {
        USART.LCR |= (DataBits - 5);
    }
    else
    {
        fRet = FALSE; // invalid Data Bit Value
        USART.LCR |= PXA271_USART::LCR__WLS_8;
    }
    
    switch(Parity)
    {
        case USART_PARITY_MARK:
            USART.LCR |= PXA271_USART::LCR__STKYP;
            //fall through
        case USART_PARITY_ODD:
            // odd parity is 0
            USART.LCR |= PXA271_USART::LCR__PEN;
            break;
            
        case USART_PARITY_SPACE:
            USART.LCR |= PXA271_USART::LCR__STKYP;
            // fall through 
        case USART_PARITY_EVEN:
            USART.LCR |= PXA271_USART::LCR__EPS | PXA271_USART::LCR__PEN;
            break;

        case USART_PARITY_NONE:
            USART.LCR &= ~PXA271_USART::LCR__PEN;
            break;
            
        default:
            fRet = FALSE;
            break;
    }

    // now enable ISRs and interrupts for the com port
    CPU_INTC_ActivateInterrupt( USART_TX_IRQ_INDEX(comPort), USART_ISR, (void*)(size_t)comPort );     // Only one ISR for send and receive

    USART.MCR = PXA271_USART::MCR__OUT2;    // Connect interrupts to the AITC
    if( FlowValue & USART_FLOW_HW_OUT_EN )
        USART.MCR |= PXA271_USART::MCR__AFE;
    if( FlowValue & USART_FLOW_HW_IN_EN )
        USART.MCR |= PXA271_USART::MCR__RTS;    // Note: it may be illegal to have HW input flow control only (without HW output flow control) - untested - !!
    USART.IER = PXA271_USART::IER__UUE | PXA271_USART::IER__TIE | PXA271_USART::IER__RAVIE;        // Enable transmit & receive interrupts
    USART.FCR = PXA271_USART::FCR__ITL_1 | PXA271_USART::FCR__BUS_8 | PXA271_USART::FCR__TIL_EMPTY
        | PXA271_USART::FCR__TRFIFOE | PXA271_USART::FCR__RESETRF | PXA271_USART::FCR__RESETTF;       // Enable FIFOs, and interrupts on empty Tx FIFO and 1 char in Rx FIFO & clear FIFOs

    PXA271::CLKMNGR().CKEN |= All_USART_ports[comPort].ClockIndex;         // Enable clock to USART

    return fRet;
}


BOOL PXA271_USART_Driver::Uninitialize( int comPort )
{
    GLOBAL_LOCK(irq);
    
    PXA271::CLKMNGR().CKEN &= ~All_USART_ports[comPort].ClockIndex;         // Disable clock to USART

    PXA271_USART& USART = PXA271::USART( comPort );

    CPU_INTC_DeactivateInterrupt( USART_TX_IRQ_INDEX(comPort) );

    // uninit GPIO pins
    ProtectPins( comPort, TRUE );       // Also, disable clock to USART to reduce power consumption

    All_USART_ports[comPort].CTS_Pin_used = FALSE;
    All_USART_ports[comPort].RTS_Pin_used = FALSE;
    
    // uninit control registers
    USART.IER = 0;  // Disable USART
    USART.FCR = PXA271_USART::FCR__RESETRF | PXA271_USART::FCR__RESETTF;        // Reset FIFOs

    return TRUE;
}

void PXA271_USART_Driver::RefFlags( int comPort, INT8 RefFlags, BOOL Add )
{
    ASSERT_IRQ_MUST_BE_OFF();

    if(Add)
    {
        // time to turn on the port?
        if(0 == g_PXA271_USART_Driver.m_RefFlags[comPort])
        {
            ASSERT(RefFlags);

            PXA271_USART& USART = PXA271::USART( comPort );

            // enable clock to peripheral
            // TODO TODO TODO: provide an API to allow power to peripheral
            //CPU_CMU_PeripheralClock((COM1 == comPort) ? MM9637A_CMU::MCLK_EN__USART0 : MM9637A_CMU::MCLK_EN__USART1, TRUE);
        }

        g_PXA271_USART_Driver.m_RefFlags[comPort] |= RefFlags;
    }
    else
    {
        g_PXA271_USART_Driver.m_RefFlags[comPort] &= ~RefFlags;

        // time to turn off this com port?
        if(0 == g_PXA271_USART_Driver.m_RefFlags[comPort])
        {
            // turn off the peripheral clock
            // we should wait max 200uSec (2 char shift time, 115200) for this so we don't lose
            // characters in the shift register.
            // we usually expect TxBufferEmpty most times, so really it is about 100uSec @ 115200
            // don't clear RefFlags before these waits, otherwise they return immediately
            while(!TxBufferEmpty       ( comPort ));
            while(!TxShiftRegisterEmpty( comPort ));

            // enable clock to peripheral
            // TODO TODO TODO: provide an API to shut down power to peripheral
            //CPU_CMU_PeripheralClock( (COM1 == comPort) ? MM9637A_CMU::MCLK_EN__USART0 : MM9637A_CMU::MCLK_EN__USART1, FALSE );
        }
    }
}

BOOL PXA271_USART_Driver::TxBufferEmpty( int comPort )
{
    // if the port TX is disabled, we dump chars to nowhere, pretend we have room
    if(0 != (c_RefFlagTx & g_PXA271_USART_Driver.m_RefFlags[comPort]))
    {
        return (0 != (PXA271::USART( comPort ).LSR & PXA271_USART::LSR__TDRQ));     // Return whether the Tx FIFO is ready to accept characters
    }

    return TRUE;
}


BOOL PXA271_USART_Driver::TxShiftRegisterEmpty( int comPort )
{
    // if the port is disabled, we dump chars to nowhere
    if(0 != (c_RefFlagTx & g_PXA271_USART_Driver.m_RefFlags[comPort]))
    {
        return (0 != (PXA271::USART( comPort ).LSR & PXA271_USART::LSR__TEMT));     // Return whether Tx SR is empty (last character sent)
    }

    return TRUE;
}


void PXA271_USART_Driver::WriteCharToTxBuffer( int comPort, UINT8 c )
{
    // if the port is disabled, we dump chars to nowhere
    if(0 != (c_RefFlagTx & g_PXA271_USART_Driver.m_RefFlags[comPort]))
    {
        ASSERT(TxBufferEmpty(comPort));

        PXA271::USART( comPort ).THR = c;
    }
}


void PXA271_USART_Driver::TxBufferEmptyInterruptEnable( int comPort, BOOL enable )
{
    GLOBAL_LOCK(irq);

    PXA271_USART& USART = PXA271::USART( comPort );

    BOOL   previousState;

    if(0 != (c_RefFlagTx & g_PXA271_USART_Driver.m_RefFlags[comPort]))
    {
        previousState = (0 != (USART.IER & PXA271_USART::IER__TIE));
    }
    else
    {
        previousState = FALSE;
    }

    if(enable)
    {
        // only do this work if the ETI is off
        if(!previousState)
        {
            // TX on doesn't necessarily add a refcount
            RefFlags( comPort, c_RefFlagTx, TRUE );

#if defined(_DEBUG)
            UINT32 error = USART.LSR & PXA271_USART::LSR__ERRORS;

            if(0 != (c_RefFlagRx & g_PXA271_USART_Driver.m_RefFlags[comPort]))
            {
                if(error)
                {
                    lcd_printf("\fLSR=%02x\r\n", error);
                }
            }
#endif
            USART.IER |= PXA271_USART::IER__TIE;
        }
    }
    else
    {
        // if it is on, then turn it off
        if(previousState)
        {
            USART.IER &= ~PXA271_USART::IER__TIE;

            RefFlags( comPort, c_RefFlagTx, FALSE );
        }
    }
}


BOOL PXA271_USART_Driver::TxBufferEmptyInterruptState( int comPort )
{
    if(0 != (c_RefFlagTx & g_PXA271_USART_Driver.m_RefFlags[comPort]))
    {
        return (0 != (PXA271::USART( comPort ).IER & PXA271_USART::IER__TIE));
    }

    return FALSE;
}


void PXA271_USART_Driver::RxBufferFullInterruptEnable( int comPort, BOOL Enable )
{
    GLOBAL_LOCK(irq);

    PXA271_USART& USART = PXA271::USART( comPort );

    BOOL   previousState;

    if(0 != (c_RefFlagRx & g_PXA271_USART_Driver.m_RefFlags[comPort]))
    {
        previousState = (0 != (PXA271_USART::IER__RAVIE & USART.IER));
    }
    else
    {
        previousState = FALSE;
    }

    if(Enable)
    {
        // only do this work if the RRDYEN is off
        if(!previousState)
        {
            RefFlags( comPort, c_RefFlagRx, TRUE );

#if defined(_DEBUG)
            UINT32 error = USART.LSR & PXA271_USART::LSR__ERRORS;

            if(0 != (c_RefFlagRx & g_PXA271_USART_Driver.m_RefFlags[comPort]))
            {
                if(error)
                {
                    lcd_printf("\fLSR=%02x\r\n", error);
                }
            }
#endif

            USART.IER |= PXA271_USART::IER__RAVIE;
        }
    }
    else
    {
        // if it is on, then turn it off
        if(previousState)
        {
            USART.IER &= ~PXA271_USART::IER__RAVIE;

            RefFlags( comPort, c_RefFlagRx, FALSE );
        }
    }
}


BOOL PXA271_USART_Driver::RxBufferFullInterruptState( int comPort )
{
    if(0 != (c_RefFlagRx & g_PXA271_USART_Driver.m_RefFlags[comPort]))
    {
        return (0 != (PXA271::USART( comPort ).IER & PXA271_USART::IER__RAVIE));
    }

    return FALSE;
}

BOOL PXA271_USART_Driver::TxHandshakeEnabledState(int comPort)
{
    // The state of the CTS input only matters if it is enabled
    if( All_USART_ports[comPort].CTS_Pin_used )
    {
        return (PXA271::USART( comPort ).MSR & PXA271_USART::MSR__CTS) ? TRUE : FALSE;
    }
    return TRUE;        // If this handshake input is not being used, it is assumed to be good
}

void PXA271_USART_Driver::USART_ISR( void* param )
{
    // we lock since we are low priority and anyone might try to add a char in another ISR
    GLOBAL_LOCK(irq);
    UINT32       comPort = (UINT32)param;

    PXA271_USART& USART = PXA271::USART( comPort );

    switch( USART.IIR & PXA271_USART::IIR__IID_MASK )
    {

    case PXA271_USART::IIR__IID_RECEIVE_DATA:       // If data in input FIFO
        // if the charger came in and turned off the serial port, bail now, as the source
        // will be going away
        if(c_RefFlagRx & g_PXA271_USART_Driver.m_RefFlags[comPort])
        {
            if(USART.LSR & PXA271_USART::LSR__DR)
            {
                USART_AddCharToRxBuffer( comPort, (char)(USART.RBR & PXA271_USART::RBR__DATA_MASK8) );

                Events_Set( SYSTEM_EVENT_FLAG_COM_IN );
            }
        }
        break;

    case PXA271_USART::IIR__IID_TRANSMIT_FIFO:      // If transmit FIFO can accept more characters
        // if the charger came in and turned off the serial port, bail now, as the source
        // will be going away
        if(c_RefFlagTx & g_PXA271_USART_Driver.m_RefFlags[comPort])
        {
            char c;

            if(USART_RemoveCharFromTxBuffer( comPort, c ))
            {
                WriteCharToTxBuffer( comPort, c );
            }
            else
            {
                // disable further Tx interrupts since we are level triggered
                TxBufferEmptyInterruptEnable( comPort, FALSE );
            }

            Events_Set( SYSTEM_EVENT_FLAG_COM_OUT );
        }
        break;

#if defined(_DEBUG)
    case PXA271_USART::IIR__IID_RECEIVE_ERROR:      // If there was an error receiving a character
        if(0 != (c_RefFlagRx & g_PXA271_USART_Driver.m_RefFlags[comPort]))
        {
            lcd_printf("\fLSR=%02x\r\n", USART.LSR & PXA271_USART::LSR__ERRORS);
        }
        break;
#endif

    }

}


void PXA271_USART_Driver::ProtectPins( int comPort, BOOL on )
{
    struct PXA271_USART_CONFIG*  Config = &g_PXA271_USART_Config;

    if(comPort < 0 || comPort >= TOTAL_USART_PORT)
        return;

    USART_PORT_INFO& USART_port = All_USART_ports[comPort];

    GLOBAL_LOCK(irq);

    if(on)
    {
        if(!USART_port.PinsProtected)
        {
            USART_port.PinsProtected = TRUE;

            RxBufferFullInterruptEnable( comPort, FALSE );

            if(Config->RxProtectInput)
            {
                CPU_GPIO_EnableInputPin( USART_port.RXD_Pin, FALSE, NULL, GPIO_INT_NONE, Config->RxProtectResistor );
            }
            else
            {
                CPU_GPIO_EnableOutputPin( USART_port.RXD_Pin, Config->RxProtectOutputValue );
            }

            TxBufferEmptyInterruptEnable( comPort, FALSE );

            if(Config->TxProtectInput)
            {
                CPU_GPIO_EnableInputPin( USART_port.TXD_Pin, FALSE, NULL, GPIO_INT_NONE, Config->TxProtectResistor );
            }
            else
            {
                CPU_GPIO_EnableOutputPin( USART_port.TXD_Pin, Config->TxProtectOutputValue );
            }
            if(USART_port.CTS_Pin_used)
            {
                if(Config->CTSProtectInput)
                {
                    CPU_GPIO_EnableInputPin( USART_port.CTS_Pin, FALSE, NULL, GPIO_INT_NONE, Config->CTSProtectResistor );
                }
                else
                {
                    CPU_GPIO_EnableOutputPin( USART_port.CTS_Pin, Config->CTSProtectOutputValue );
                }
            }
            if(USART_port.RTS_Pin_used)
            {
                if(Config->RTSProtectInput)
                {
                    CPU_GPIO_EnableInputPin( USART_port.RTS_Pin, FALSE, NULL, GPIO_INT_NONE, Config->RTSProtectResistor );
                }
                else
                {
                    CPU_GPIO_EnableOutputPin( USART_port.RTS_Pin, Config->RTSProtectOutputValue );
                }
            }
        }
    }
    else
    {
        if(USART_port.PinsProtected)
        {
            USART_port.PinsProtected = FALSE;

            CPU_GPIO_DisablePin( USART_port.TXD_Pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_OUT, USART_port.TXD_Pin_function );

            TxBufferEmptyInterruptEnable( comPort, TRUE );

            CPU_GPIO_DisablePin( USART_port.RXD_Pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_IN, USART_port.RXD_Pin_function );

            RxBufferFullInterruptEnable( comPort, TRUE );

            if(USART_port.CTS_Pin_used)
                CPU_GPIO_DisablePin( USART_port.CTS_Pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_IN, USART_port.CTS_Pin_function );

            if(USART_port.RTS_Pin_used)
                CPU_GPIO_DisablePin( USART_port.RTS_Pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_OUT, USART_port.RTS_Pin_function );
        }
    }
}

void  PXA271_USART_Driver::GetPins( int comPort, GPIO_PIN& rxPin, GPIO_PIN& txPin, GPIO_PIN& ctsPin, GPIO_PIN& rtsPin )
{
    const USART_PORT_INFO& USART_port = All_USART_ports[comPort];
    txPin  = (GPIO_PIN) USART_port.TXD_Pin; 
    rxPin  = (GPIO_PIN) USART_port.RXD_Pin; 
    ctsPin = (GPIO_PIN) USART_port.CTS_Pin; 
    rtsPin = (GPIO_PIN) USART_port.RTS_Pin; 
    
}

void  PXA271_USART_Driver::BaudrateBoundary( int ComPortNum, UINT32& maxBaudrateHz, UINT32& minBaudrateHz )
{
    maxBaudrateHz = PXA271_USART::c_MAX_BAUDRATE;
    minBaudrateHz = PXA271_USART::c_MIN_BAUDRATE;

}

BOOL PXA271_USART_Driver::IsBaudrateSupported( int ComPortNum, UINT32 & BaudrateHz )
{

    const BaudRateLookup* ptr = c_BaudRateLookup;

    if (BaudrateHz > PXA271_USART::c_MAX_BAUDRATE)
    {
        BaudrateHz = PXA271_USART::c_MAX_BAUDRATE;
        return false;
    }
    else
    {    
        while (ptr->BaudRate !=0)
        {
            if (ptr->BaudRate < BaudrateHz)
            {
                BaudrateHz=ptr->BaudRate;
                return false;
            }
            else if (ptr->BaudRate == BaudrateHz)
            {   
                return true;
            }
            ptr++;
        }
        BaudrateHz=0;
        return false;
    }
}


