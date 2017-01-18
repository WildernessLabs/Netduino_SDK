////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_MC9328MXL_USART_Driver"
#endif

MC9328MXL_USART_Driver g_MC9328MXL_USART_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

struct USARTPortPinInfo
{
    BOOL PinsProtected;
    BOOL CTSEnabled;
    BOOL RTSEnabled;
    UINT32 SerRxPin;
    UINT32 SerTxPin;
    UINT32 SerCTSPin;
    UINT32 SerRTSPin;
};

static struct USARTPortPinInfo USARTPinInfo[TOTAL_USART_PORT] =
{
    {
        TRUE,           // Pins are assumed to be in protected state at Reset
        FALSE,          // CTS defaults to unused
        FALSE,          // RTS defaults to unused
        MC9328MXL_USART::c_SER1_RDX,
        MC9328MXL_USART::c_SER1_TDX,
        MC9328MXL_USART::c_SER1_CTS,
        MC9328MXL_USART::c_SER1_RTS
    },
    {
        TRUE,           // Pins are assumed to be in protected state at Reset
        FALSE,          // CTS defaults to unused
        FALSE,          // RTS defaults to unused
        MC9328MXL_USART::c_SER2_RDX,
        MC9328MXL_USART::c_SER2_TDX,
        MC9328MXL_USART::c_SER2_CTS,
        MC9328MXL_USART::c_SER2_RTS
    }
};

//--//

BOOL MC9328MXL_USART_Driver::Initialize( int comPort, int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue )
{
    BOOL fRet = TRUE;
    
    struct BaudRateLookup
    {
        UINT32 BaudRate;
        UINT32 RFDIV;
        UINT32 UBIR_X;
        UINT32 UBMR_X;
    };

    const BaudRateLookup c_BaudRateLookup[] =
    {
        {230400, 5, 0x71, 0x270},
        {115200, 5, 0x47, 0x270},
        { 57600, 5, 0x23, 0x270},
        { 38400, 5, 0x17, 0x270},
        { 19200, 5,  0xB, 0x270},
        {  9600, 5,  0x5, 0x270},
        {     0, 0,    0,     0},  //for not defined baudrate
    };

    //--//

    GLOBAL_LOCK(irq);
    UINT32       divisor;
    UINT32       bir;
    UINT32       brm;
    UINT32       cr2 = 0;

    if(comPort < 0 || comPort >= TOTAL_USART_PORT)
    {
        return FALSE;
    }

    MC9328MXL_USART& USART = MC9328MXL::USART( comPort );

    g_MC9328MXL_USART_Driver.m_RefFlags[comPort] = 0;

    const BaudRateLookup* ptr = c_BaudRateLookup;

    while(true)
    {
        if(ptr->BaudRate == BaudRate)
        {
            divisor = ptr->RFDIV;
            bir     = ptr->UBIR_X;
            brm     = ptr->UBMR_X;
            break;
        }

        if(ptr->BaudRate == 0)
        {
            if(BaudRate <= 0)
            {
                return FALSE; 
            }

            BaudRateLookup best_baud = {0xFFFFFFF,0,0,0};  // we use the baudrate field to measure smallest delta from target frequency

            const int    refdiv_count = 7;
            const UINT32 refdiv_table[refdiv_count] = {SYSTEM_CLOCK_HZ/6, SYSTEM_CLOCK_HZ/5, SYSTEM_CLOCK_HZ/4, SYSTEM_CLOCK_HZ/3, SYSTEM_CLOCK_HZ/2, SYSTEM_CLOCK_HZ/1, SYSTEM_CLOCK_HZ/7};

            for(int i=0; i<refdiv_count; i++)
            {
                // from MC9328MXL speck (section 25.5.8 Binary Rate Multiplier (BRM))
                // (baud-rate*16)/(PERCLK1/RFDIV)=NUM/DENOM
                UINT64 REF_FREQ = (UINT64)refdiv_table[i];
                UINT64 DEN      = 1ULL << 16;              
                UINT64 NUM      = BaudRate * 16;
                NUM           <<= 16;                      // shift for precision (NUM/DEN should always less than zero)
                NUM            /= REF_FREQ;

                while(NUM > (1UL<<16) || DEN > (1UL<<16)) // we subtract one from the NUM and DEN so they can actually be 0x10000
                {
                    NUM >>= 1;
                    DEN >>= 1;
                }

                // baud-rate = NUM/DEN * REF_FREQ / 16
                INT32 diff = BaudRate - (NUM * REF_FREQ) / (DEN * 16);  
                if(diff<0) 
                {
                    diff = -diff;
                }

                if(best_baud.BaudRate > (UINT32)diff)
                {
                    best_baud.BaudRate = diff;
                    best_baud.RFDIV    = i;
                    best_baud.UBIR_X   = NUM - 1;
                    best_baud.UBMR_X   = DEN - 1;
                }
            }

            divisor = best_baud.RFDIV;
            bir     = best_baud.UBIR_X;
            brm     = best_baud.UBMR_X;
            break;           
        }

        ptr++;
    }

    //--//

    // This must be done before ProtectPins() is called or CTS and RTS won't be enabled
    USARTPinInfo[comPort].CTSEnabled = (BOOL)(FlowValue & USART_FLOW_HW_IN_EN);
    USARTPinInfo[comPort].RTSEnabled = (BOOL)(FlowValue & USART_FLOW_HW_OUT_EN);

    // init control registers and FIFO registers
    USART.UCR1_X = MC9328MXL_USART::UCR1__UARTCLKEN | MC9328MXL_USART::UCR1__UARTEN;

    switch(Parity)
    {
        case USART_PARITY_ODD:
            cr2 |= MC9328MXL_USART::UCR2__PREN | MC9328MXL_USART::UCR2__PROE;
            break;

        case USART_PARITY_EVEN:
            cr2 |= MC9328MXL_USART::UCR2__PREN;
            break;

        case USART_PARITY_NONE:
            // do nothing
            break;

        // not supported
        case USART_PARITY_MARK:
        case USART_PARITY_SPACE:
        default:
            fRet = FALSE;
            break;
    }

    switch(StopBits)
    {
        case USART_STOP_BITS_ONE:
            // default - do nothing
            break;
        case USART_STOP_BITS_TWO:
            cr2 |=  MC9328MXL_USART::UCR2__STPB;
            break;

        // not supported
        case USART_STOP_BITS_ONEPOINTFIVE:
        case USART_STOP_BITS_NONE:
        default:
            fRet = FALSE;
            break;
    }

    switch(DataBits)
    {
        case 7:
            // do nothing 
            break;
            
        default: // NOT SUPPORTED
            fRet = FALSE; 
            // fall through (so that we setup 8 bytes by default incase the return value is ignored).
        case 8:
            cr2 |= MC9328MXL_USART::UCR2__WS;
            break;
    }
    
    if(!(FlowValue & USART_FLOW_HW_OUT_EN))
    {
        cr2 |= MC9328MXL_USART::UCR2__IRTS;    // If no RTS Flow Control enabled, ignore RTS input
    }
    if(FlowValue & USART_FLOW_HW_IN_EN)
    {
        cr2 |= MC9328MXL_USART::UCR2__CTSC;    // If input flow control enabled have FIFO control CTS output
    }

    USART.UCR2_X = cr2 | MC9328MXL_USART::UCR2__TXEN | MC9328MXL_USART::UCR2__RXEN | MC9328MXL_USART::UCR2__SRST_L;

    if(FlowValue & USART_FLOW_HW_IN_EN)
    {
        USART.UCR4_X = ((MC9328MXL_USART::c_FIFO_SIZE * 3)/4) << MC9328MXL_USART::UCR4__CTSTL_shift;        // Set CTL cutoff level at 3/4 FIFO size
    }
    else
    {
        USART.UCR4_X = 0;
    }

    if(Parity != USART_PARITY_NONE)
    {
        USART.UCR3_X = MC9328MXL_USART::UCR3__FRAERREN | MC9328MXL_USART::UCR3__PARERREN;
    }
    else
    {
        USART.UCR3_X = 0;
    }

    USART.UBIR_X = bir;
    USART.UBMR_X = brm;

    {
        UINT32 val = 0;

        val |= (divisor << MC9328MXL_USART::UFCR__RFDIV_shift) & MC9328MXL_USART::UFCR__RFDIV_mask;
        val |= (16      << MC9328MXL_USART::UFCR__TXTL_shift ) & MC9328MXL_USART::UFCR__TXTL_mask;
        val |= ( 1      << MC9328MXL_USART::UFCR__RXTL_shift ) & MC9328MXL_USART::UFCR__RXTL_mask;

        USART.UFCR_X = val;
    }

    // inits GPIO pins
    ProtectPins( comPort, FALSE );

    // now enable ISRs and interrupts for the com port
    if(c_COM1 == comPort)
    {
        CPU_INTC_ActivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_UART1_MINT_TX, TxISR, (void*)(size_t)comPort );
        CPU_INTC_ActivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_UART1_MINT_RX, RxISR, (void*)(size_t)comPort );

        if(Parity != USART_PARITY_NONE)
        {
    		CPU_INTC_ActivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_UART1_MINT_PFERR, ParityError, (void*)(size_t)comPort );
        }
    }
    else
    {
        CPU_INTC_ActivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_UART2_MINT_TX, TxISR, (void*)(size_t)comPort );
        CPU_INTC_ActivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_UART2_MINT_RX, RxISR, (void*)(size_t)comPort );

        if(Parity != USART_PARITY_NONE)
        {
    		CPU_INTC_ActivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_UART2_MINT_PFERR, ParityError, (void*)(size_t)comPort );
        }
    }

    return fRet;
}

void MC9328MXL_USART_Driver::ParityError( void* Param )
{
    UINT32 comPort = (UINT32)Param;

    MC9328MXL_USART& USART = MC9328MXL::USART( comPort );    

    UINT32 URXDn = USART.URXDn_X[0];
    UINT32 USRn = USART.USR1_X;

    UINT32 errEvt = 0;   

    if((URXDn & MC9328MXL_USART::URXDn__PRERR) | (USRn & MC9328MXL_USART::USR1__PARITYERR))
    {
        errEvt = USART_EVENT_ERROR_RXPARITY;
    }
    else if((URXDn & MC9328MXL_USART::URXDn__FRMERR) | (USRn & MC9328MXL_USART::USR1__FRAMEERR))
    {
        errEvt = USART_EVENT_ERROR_FRAME;
    }
    else if(URXDn & MC9328MXL_USART::URXDn__OVRRUN)
    {
        errEvt = USART_EVENT_ERROR_OVERRUN;
    }

    // clear the interrupt
    USART.USR1_X = (MC9328MXL_USART::USR1__PARITYERR | MC9328MXL_USART::USR1__FRAMEERR);

    USART_SetEvent( comPort, errEvt );
}

BOOL MC9328MXL_USART_Driver::Uninitialize( int comPort )
{
    GLOBAL_LOCK(irq);
    
    if(comPort < 0 || comPort >= TOTAL_USART_PORT)
    {
        return FALSE;
    }

    MC9328MXL_USART& USART = MC9328MXL::USART( comPort );

	if(c_COM1 == comPort)
	{
		CPU_INTC_DeactivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_UART1_MINT_TX );
		CPU_INTC_DeactivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_UART1_MINT_RX );
		CPU_INTC_DeactivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_UART1_MINT_PFERR );
	}
	else
	{
		CPU_INTC_DeactivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_UART2_MINT_TX );
		CPU_INTC_DeactivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_UART2_MINT_RX );
		CPU_INTC_DeactivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_UART2_MINT_PFERR );
	}

    // uninits GPIO pins
    ProtectPins( comPort, TRUE );

    // This must be done after ProtectPins() or these pins won't be protected
    USARTPinInfo[comPort].RTSEnabled = FALSE;
    USARTPinInfo[comPort].CTSEnabled = FALSE;

    // uninit control registers
    USART.UCR2_X = 0;  // writing 0 on the first bit will cause reset

    return TRUE;
}

void MC9328MXL_USART_Driver::RefFlags( int comPort, INT8 RefFlags, BOOL Add )
{
    ASSERT_IRQ_MUST_BE_OFF();

    if(Add)
    {
        // time to turn on the port?
        if(0 == g_MC9328MXL_USART_Driver.m_RefFlags[comPort])
        {
            ASSERT(RefFlags);

            MC9328MXL_USART& USART = MC9328MXL::USART( comPort );

            // enable clock to peripheral
            // TODO TODO TODO: provide an API to allow power to peripheral
            //CPU_CMU_PeripheralClock((c_COM1 == comPort) ? MM9637A_CMU::MCLK_EN__USART0 : MM9637A_CMU::MCLK_EN__USART1, TRUE);
        }

        g_MC9328MXL_USART_Driver.m_RefFlags[comPort] |= RefFlags;
    }
    else
    {
        g_MC9328MXL_USART_Driver.m_RefFlags[comPort] &= ~RefFlags;

        // time to turn off this com port?
        if(0 == g_MC9328MXL_USART_Driver.m_RefFlags[comPort])
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
            //CPU_CMU_PeripheralClock( (c_COM1 == comPort) ? MM9637A_CMU::MCLK_EN__USART0 : MM9637A_CMU::MCLK_EN__USART1, FALSE );
        }
    }
}

BOOL MC9328MXL_USART_Driver::TxBufferEmpty( int comPort )
{
    // if the port TX is disabled, we dump chars to nowhere, pretend we have room
    if(0 != (c_RefFlagTx & g_MC9328MXL_USART_Driver.m_RefFlags[comPort]))
    {
        return (0 != (MC9328MXL::USART( comPort ).USR2_X & MC9328MXL_USART::USR2__TXFE));
    }

    return TRUE;
}


BOOL MC9328MXL_USART_Driver::TxShiftRegisterEmpty( int comPort )
{
    // if the port is disabled, we dump chars to nowhere
    if(0 != (c_RefFlagTx & g_MC9328MXL_USART_Driver.m_RefFlags[comPort]))
    {
        return (0 != (MC9328MXL::USART( comPort ).USR2_X & MC9328MXL_USART::USR2__TXDC));
    }

    return TRUE;
}


void MC9328MXL_USART_Driver::WriteCharToTxBuffer( int comPort, UINT8 c )
{
    // if the port is disable, we dump chars to nowhere
    if(0 != (c_RefFlagTx & g_MC9328MXL_USART_Driver.m_RefFlags[comPort]))
    {
        ASSERT(TxBufferEmpty(comPort));

        MC9328MXL::USART( comPort ).UTXDn_X[0] = c;
    }
}


void MC9328MXL_USART_Driver::TxBufferEmptyInterruptEnable( int comPort, BOOL enable )
{
    GLOBAL_LOCK(irq);

    MC9328MXL_USART& USART = MC9328MXL::USART( comPort );

    BOOL   previousState;

    if(0 != (c_RefFlagTx & g_MC9328MXL_USART_Driver.m_RefFlags[comPort]))
    {
        previousState = (0 != (MC9328MXL_USART::UCR1__TXMPTYEN & USART.UCR1_X));
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
            UINT32 error = USART.USR1_X & (MC9328MXL_USART::USR1__FRAMEERR | MC9328MXL_USART::USR1__PARITYERR);

            error |= USART.USR2_X & MC9328MXL_USART::USR2__ORE;
            if(0 != (c_RefFlagRx & g_MC9328MXL_USART_Driver.m_RefFlags[comPort]))
            {
                if(error)
                {
                    lcd_printf("\fUSRx_X=%02x\r\n", error);
                }
            }
#endif
            USART.UCR1_X |= MC9328MXL_USART::UCR1__TXMPTYEN;
        }
    }
    else
    {
        // if it is on, then turn it off
        if(previousState)
        {
            USART.UCR1_X &= ~MC9328MXL_USART::UCR1__TXMPTYEN;

            RefFlags( comPort, c_RefFlagTx, FALSE );
        }
    }
}


BOOL MC9328MXL_USART_Driver::TxBufferEmptyInterruptState( int comPort )
{
    if(0 != (c_RefFlagTx & g_MC9328MXL_USART_Driver.m_RefFlags[comPort]))
    {
        return (0 != (MC9328MXL::USART( comPort ).UCR1_X & MC9328MXL_USART::UCR1__TXMPTYEN));
    }

    return FALSE;
}


void MC9328MXL_USART_Driver::RxBufferFullInterruptEnable( int comPort, BOOL Enable )
{
    GLOBAL_LOCK(irq);

    MC9328MXL_USART& USART = MC9328MXL::USART( comPort );

    BOOL   previousState;

    if(0 != (c_RefFlagRx & g_MC9328MXL_USART_Driver.m_RefFlags[comPort]))
    {
        previousState = (0 != (MC9328MXL_USART::UCR1__RRDYEN & USART.UCR1_X));
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
            UINT32 error = USART.USR1_X & (MC9328MXL_USART::USR1__FRAMEERR | MC9328MXL_USART::USR1__PARITYERR);

            error |= USART.USR2_X & MC9328MXL_USART::USR2__ORE;
            if(0 != (c_RefFlagRx & g_MC9328MXL_USART_Driver.m_RefFlags[comPort]))
            {
                if(error)
                {
                    lcd_printf("\fUSRx_X=%02x\r\n", error);
                }
            }
#endif

            USART.UCR1_X |= MC9328MXL_USART::UCR1__RRDYEN;
        }
    }
    else
    {
        // if it is on, then turn it off
        if(previousState)
        {
            USART.UCR1_X &= ~MC9328MXL_USART::UCR1__RRDYEN;

            RefFlags( comPort, c_RefFlagRx, FALSE );
        }
    }
}


BOOL MC9328MXL_USART_Driver::RxBufferFullInterruptState( int comPort )
{
    if(0 != (c_RefFlagRx & g_MC9328MXL_USART_Driver.m_RefFlags[comPort]))
    {
        return (0 != (MC9328MXL::USART( comPort ).UCR1_X & MC9328MXL_USART::UCR1__RRDYEN));
    }

    return FALSE;
}


// Returns FALSE only if Tx is being held up due to HW Handshake
BOOL MC9328MXL_USART_Driver::TxHandshakeEnabledState( int comPort )
{
    // The state of the RTS pin only matters if HW Handshake is enabled
    if(USARTPinInfo[comPort].RTSEnabled)
    {
        return (MC9328MXL::USART(comPort).USR1_X & MC9328MXL_USART::USR1__RTSS) ? TRUE : FALSE;
    }
    // If HW Handshake is not enabled, then the state of the handshake line is ignored and assumed good
    return TRUE;
}


void MC9328MXL_USART_Driver::RxISR( void* param )
{
    // we lock since we are low priority and anyone might try to add a char in another ISR
    GLOBAL_LOCK(irq);
    UINT32       comPort = (UINT32)param;

    MC9328MXL_USART& USART = MC9328MXL::USART( comPort );

    // if the charger came in and turned off the serial port, bail now, as the source
    // will be going away
    if(0 == (c_RefFlagRx & g_MC9328MXL_USART_Driver.m_RefFlags[comPort]))
    {
        return;
    }

    UINT32 URXDn = USART.URXDn_X[0];

#if defined(_DEBUG)
    if(0 != (c_RefFlagRx & g_MC9328MXL_USART_Driver.m_RefFlags[comPort]))
    {
        if(URXDn & MC9328MXL_USART::URXDn__ERRORS)
        {
            lcd_printf("\fURDXn_X=%02x\r\n", URXDn);
        }
    }
#endif

    if(URXDn & MC9328MXL_USART::URXDn__ERRORS)
    {
        UINT32 errEvt = 0;   

        if(URXDn & MC9328MXL_USART::URXDn__PRERR)
        {
            errEvt = USART_EVENT_ERROR_RXPARITY;
        }
        else if(URXDn & MC9328MXL_USART::URXDn__FRMERR)
        {
            errEvt = USART_EVENT_ERROR_FRAME;
        }
        else if(URXDn & MC9328MXL_USART::URXDn__OVRRUN)
        {
            errEvt = USART_EVENT_ERROR_OVERRUN;
        }

        USART_SetEvent( comPort, errEvt );

        return;
    }

    if(URXDn & MC9328MXL_USART::URXDn__CHARRDY)
    {
        USART_AddCharToRxBuffer( comPort, (char)(URXDn & MC9328MXL_USART::URXDn__DATA_mask) );

        Events_Set( SYSTEM_EVENT_FLAG_COM_IN );
    }
}


void MC9328MXL_USART_Driver::TxISR( void* param )
{
    // we lock since we are low priority and anyone might try to add a char in another ISR
    GLOBAL_LOCK(irq);
    UINT32       comPort = (UINT32)param;

    MC9328MXL_USART& USART = MC9328MXL::USART( comPort );

    // if the charger came in and turned off the serial port, bail now, as the source
    // will be going away
    if(0 == (c_RefFlagTx & g_MC9328MXL_USART_Driver.m_RefFlags[comPort]))
    {
        return;
    }

#if defined(_DEBUG)
    UINT32 errors = USART.USR1_X & (MC9328MXL_USART::USR1__FRAMEERR | MC9328MXL_USART::USR1__PARITYERR);

    errors |= USART.USR2_X & MC9328MXL_USART::USR2__ORE;
    if(0 != (c_RefFlagRx & g_MC9328MXL_USART_Driver.m_RefFlags[comPort]))
    {
        if(errors)
        {
            lcd_printf("\fUSRx_X=%02x\r\n", errors);
        }
    }
#endif

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


void MC9328MXL_USART_Driver::ProtectPins( int comPort, BOOL on )
{
    GLOBAL_LOCK(irq);

    if(comPort < 0 || comPort >= TOTAL_USART_PORT)
    {
        return;
    }

    MC9328MXL_USART_CONFIG* Config = &g_MC9328MXL_USART_Config;
    struct USARTPortPinInfo &PinInfo = USARTPinInfo[comPort];

    if(on)
    {
        if(!PinInfo.PinsProtected)
        {
            PinInfo.PinsProtected = TRUE;

            RxBufferFullInterruptEnable( comPort, FALSE );

            if(Config->RxProtectInput)
            {
                CPU_GPIO_EnableInputPin( PinInfo.SerRxPin, FALSE, NULL, GPIO_INT_NONE, Config->RxProtectResistor );
            }
            else
            {
                CPU_GPIO_EnableOutputPin( PinInfo.SerRxPin, Config->RxProtectOutputValue );
            }

            TxBufferEmptyInterruptEnable( comPort, FALSE );

            if(Config->TxProtectInput)
            {
                CPU_GPIO_EnableInputPin( PinInfo.SerTxPin, FALSE, NULL, GPIO_INT_NONE, Config->TxProtectResistor );
            }
            else
            {
                CPU_GPIO_EnableOutputPin( PinInfo.SerTxPin, Config->TxProtectOutputValue );
            }

            if(PinInfo.CTSEnabled)
            {
                if(Config->CTSProtectInput)
                {
                    CPU_GPIO_EnableInputPin( PinInfo.SerCTSPin, FALSE, NULL, GPIO_INT_NONE, Config->CTSProtectResistor );
                }
                else
                {
                    CPU_GPIO_EnableOutputPin( PinInfo.SerCTSPin, Config->CTSProtectOutputValue );
                }
            }

            if(PinInfo.RTSEnabled)
            {
                if(Config->RTSProtectInput)
                {
                    CPU_GPIO_EnableInputPin( PinInfo.SerRTSPin, FALSE, NULL, GPIO_INT_NONE, Config->RTSProtectResistor );
                }
                else
                {
                    CPU_GPIO_EnableOutputPin( PinInfo.SerRTSPin, Config->RTSProtectOutputValue );
                }
            }
            
        }
    }
    else
    {
        if(PinInfo.PinsProtected)
        {
            PinInfo.PinsProtected = FALSE;

            CPU_GPIO_DisablePin( PinInfo.SerTxPin, RESISTOR_DISABLED, MC9328MXL_GPIO::DDIR__OUT, GPIO_ALT_PRIMARY );

            TxBufferEmptyInterruptEnable( comPort, TRUE );

            CPU_GPIO_DisablePin( PinInfo.SerRxPin, RESISTOR_DISABLED, MC9328MXL_GPIO::DDIR__IN, GPIO_ALT_PRIMARY );

            RxBufferFullInterruptEnable( comPort, TRUE );
            
            if(PinInfo.CTSEnabled)
            {
                CPU_GPIO_DisablePin( PinInfo.SerCTSPin, RESISTOR_DISABLED, MC9328MXL_GPIO::DDIR__OUT, GPIO_ALT_PRIMARY );
            }

            if(PinInfo.RTSEnabled)
            {
                CPU_GPIO_DisablePin( PinInfo.SerRTSPin, RESISTOR_DISABLED, MC9328MXL_GPIO::DDIR__IN, GPIO_ALT_PRIMARY );
            }
            
        }
    }
}

void MC9328MXL_USART_Driver::GetPins(int comPort, GPIO_PIN& rxPin, GPIO_PIN& txPin, GPIO_PIN& ctsPin, GPIO_PIN& rtsPin)
{
    switch(comPort)
    {
        case c_COM1: 
            txPin  = MC9328MXL_USART::c_SER1_TDX; 
            rxPin  = MC9328MXL_USART::c_SER1_RDX; 
            ctsPin = MC9328MXL_USART::c_SER1_CTS; 
            rtsPin = MC9328MXL_USART::c_SER1_RTS; 
            break;
        case c_COM2: 
            txPin  = MC9328MXL_USART::c_SER2_TDX; 
            rxPin  = MC9328MXL_USART::c_SER2_RDX; 
            ctsPin = MC9328MXL_USART::c_SER2_CTS; 
            rtsPin = MC9328MXL_USART::c_SER2_RTS; 
            break;
        default: return;
    }
}

void MC9328MXL_USART_Driver::BaudrateBoundary(int ComPortNum, UINT32& maxBaudrateHz, UINT32& minBaudrateHz)
{
    maxBaudrateHz = MC9328MXL_USART::c_MAX_BAUDRATE;
    minBaudrateHz = MC9328MXL_USART::c_MIN_BAUDRATE;

}

BOOL MC9328MXL_USART_Driver::IsBaudrateSupported(int ComPortNum, UINT32 & BaudrateHz)
{
    if (BaudrateHz <= MC9328MXL_USART::c_MAX_BAUDRATE)
    {
        return true;
    }
    else
    {
        BaudrateHz = MC9328MXL_USART::c_MAX_BAUDRATE;
        return false;
    }
}


