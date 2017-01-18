////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "AT91_USART.h"

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_AT91_USART_Driver"
#endif

AT91_USART_Driver g_AT91_USART_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif


//--//

BOOL AT91_USART_Driver::Initialize( int comPort, int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue )
{
    BOOL fRet = TRUE;
    UINT32 UsartId;
    UINT32 USMR = 0;
    GLOBAL_LOCK(irq);

    AT91_USART &usart = AT91::USART(comPort);


    if( comPort == 0)
    {
        UsartId = (AT91C_ID_SYS);
    }
    else
    {
        UsartId = (AT91C_ID_US0 +  (comPort - 1));
    }

    // Disable interrupts
    usart.US_IDR = (unsigned int) -1;

    // Reset receiver and transmitter
    usart.US_CR = (AT91_USART::US_RSTRX | AT91_USART::US_RSTTX | AT91_USART::US_RXDIS | AT91_USART::US_TXDIS);

    AT91_PMC &pmc = AT91::PMC();
    pmc.EnablePeriphClock(UsartId);

    // Define the baud rate divisor register
    {
        DWORD dwMasterClock = SYSTEM_PERIPHERAL_CLOCK_HZ;
        unsigned int baud_value = ((dwMasterClock*10)/(BaudRate * 16));
        if ((baud_value % 10) >= 5)
            baud_value = (baud_value / 10) + 1;
        else
            baud_value /= 10;
         usart.US_BRGR = baud_value;
    }

    // Write the Timeguard Register
    usart.US_TTGR = 0;

    if(!CPU_INTC_ActivateInterrupt( UsartId, USART_ISR, (void*)(size_t)comPort) )
    {
            return FALSE;
    }

    // inits GPIO pins
    ProtectPins( comPort, FALSE );

    // Enable Transmitter
    USMR = (AT91_USART::US_USMODE_NORMAL)  ;

    switch(Parity)
    {
        case USART_PARITY_ODD:
            USMR |= AT91_USART::US_PAR_ODD;
            break;
        case USART_PARITY_EVEN:
            USMR |= AT91_USART::US_PAR_EVEN;
            break;
        case USART_PARITY_MARK:
            USMR |= AT91_USART::US_PAR_MARK;
            break;
        case USART_PARITY_SPACE:
            USMR |= AT91_USART::US_PAR_SPACE;
            break;
        default: // not supported
            fRet = FALSE;
            //fall through for default
        case USART_PARITY_NONE:
            USMR |= AT91_USART::US_PAR_NONE;
            break;
    }

    switch(DataBits)
    {
        case 5:
            USMR |= AT91_USART::US_CHRL_5_BITS;
            break;
        case 6:
            USMR |= AT91_USART::US_CHRL_6_BITS;
            break;
        case 7:
            USMR |= AT91_USART::US_CHRL_7_BITS;
            break;
        default: // not supported
            fRet = FALSE;
            //fall through for default
        case 8:
            USMR |= AT91_USART::US_CHRL_8_BITS;
            break;
    }

    switch(StopBits)
    {
        case USART_STOP_BITS_ONE:
            // this board doesn't appear to work with 1 stop bits set
            USMR |= AT91_USART::US_NBSTOP_1_BIT;
            break;
        case USART_STOP_BITS_TWO:
            USMR |= AT91_USART::US_NBSTOP_2_BIT;
            break;
        case USART_STOP_BITS_ONEPOINTFIVE:
            USMR |= AT91_USART::US_NBSTOP_15_BIT;
            break;

        // not supported
        case USART_STOP_BITS_NONE:
        default:
            fRet = FALSE;
            break;
    }

    usart.US_MR = USMR;
    
    usart.US_CR = AT91_USART::US_RXEN;
    usart.US_CR = AT91_USART::US_TXEN;

    return fRet;
}


BOOL AT91_USART_Driver::Uninitialize( int comPort )
{
    UINT32 UsartId;
    
    GLOBAL_LOCK(irq);

    if( comPort == 0)
        UsartId = (AT91C_ID_SYS);
    else
        UsartId = (AT91C_ID_US0 +  (comPort - 1));

    // uninits GPIO pins
    ProtectPins( comPort, TRUE );
    if( !comPort)
    {
        AT91_PMC &pmc = AT91::PMC();
        pmc.DisablePeriphClock(UsartId);
    }
    
    return TRUE;
}

void AT91_USART_Driver::RefFlags( int comPort, INT8 RefFlags, BOOL Add )
{
    
    ASSERT_IRQ_MUST_BE_OFF();

    AT91_USART &usart = AT91::USART(comPort);


    if(Add)
    {
        // time to turn on the port?
        if(0 == g_AT91_USART_Driver.m_RefFlags[comPort])
        {
            ASSERT(RefFlags);
            // enable clock to peripheral
            // TODO TODO TODO: provide an API to allow power to peripheral
            // CPU_CMU_PeripheralClock((c_COM1 == comPort) ? MM9637A_CMU::MCLK_EN__USART0 : MM9637A_CMU::MCLK_EN__USART1, TRUE);
        }
        
        g_AT91_USART_Driver.m_RefFlags[comPort] |= RefFlags;
    }
    else
    {
        g_AT91_USART_Driver.m_RefFlags[comPort] &= ~RefFlags;
        
        // time to turn off this com port?
        if(0 == g_AT91_USART_Driver.m_RefFlags[comPort])
        {
            // turn off the peripheral clock
            // we should wait max 200uSec (2 char shift time, 115200) for this so we don't lose
            // characters in the shift register.  Ollie has no int for this last one, bummer.
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

BOOL AT91_USART_Driver::TxBufferEmpty( int comPort )
{
    AT91_USART &usart = AT91::USART(comPort);

    // if the port TX is disabled, we dump chars to nowhere, pretend we have room
    if(0 != (c_RefFlagTx & g_AT91_USART_Driver.m_RefFlags[comPort]))
    {
        return (0 != (usart.US_CSR & AT91_USART::US_TXRDY));
    }
    
    return TRUE;
}

BOOL AT91_USART_Driver::TxShiftRegisterEmpty( int comPort )
{
    AT91_USART &usart = AT91::USART(comPort);
    
    // if the port is disabled, we dump chars to nowhere
    if(0 != (c_RefFlagTx & g_AT91_USART_Driver.m_RefFlags[comPort]))
    {
        return (0 != (usart.US_CSR & AT91_USART::US_TXEMPTY));
    }
    
    return TRUE;
}

void AT91_USART_Driver::WriteCharToTxBuffer( int comPort, UINT8 c )
{
    AT91_USART &usart = AT91::USART(comPort);

    usart.US_THR = (char) c;
}

void AT91_USART_Driver::TxBufferEmptyInterruptEnable( int comPort, BOOL enable )
{
    
    GLOBAL_LOCK(irq);

    UINT32 previousRefFlags = g_AT91_USART_Driver.m_RefFlags[comPort];
    BOOL   previousState;

    AT91_USART &usart = AT91::USART(comPort);

    if(0 != (c_RefFlagTx & g_AT91_USART_Driver.m_RefFlags[comPort]))
    {
        previousState = (0 != ((usart.US_IMR) & AT91_USART::US_TXRDY));     
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

            usart.US_IER = AT91_USART::US_TXRDY;
        }
    }
    else
    {
        // if it is on, then turn it off
        if(previousState)
        {
            usart.US_IDR = AT91_USART::US_TXRDY;
            RefFlags( comPort, c_RefFlagTx, FALSE );
        }
    }

    // tell the driver when we toggle in our out of powersave mode
    if(0 == g_AT91_USART_Driver.m_RefFlags[comPort])
    {
        if(0 != previousRefFlags)
        {
            USART_PowerSave( comPort, 1 );
        }
    }
    else
    {
        if(0 == previousRefFlags)
        {
            USART_PowerSave( comPort, 0 );
        }
    }
}

BOOL AT91_USART_Driver::TxBufferEmptyInterruptState( int comPort )
{
    AT91_USART &usart = AT91::USART(comPort);

    if(0 != (c_RefFlagTx & g_AT91_USART_Driver.m_RefFlags[comPort]))
    {
        return (0 != (usart.US_IMR & AT91_USART::US_TXRDY));
    }

    return FALSE;
}

void AT91_USART_Driver::RxBufferFullInterruptEnable( int comPort, BOOL Enable )
{
    
    GLOBAL_LOCK(irq);

    UINT32 previousRefFlags = g_AT91_USART_Driver.m_RefFlags[comPort];
    BOOL   previousState;

    AT91_USART &usart = AT91::USART(comPort);

    if(0 != (c_RefFlagRx & g_AT91_USART_Driver.m_RefFlags[comPort]))
        {
        previousState = (0 != ((usart.US_IMR) & AT91_USART::US_RXRDY));
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

            usart.US_IER = AT91_USART::US_RXRDY;
        }
    }
    else
    {
        // if it is on, then turn it off
        if(previousState)
        {
            usart.US_IDR = AT91_USART::US_RXRDY;
            
            RefFlags( comPort, c_RefFlagRx, FALSE );
        }
    }

    // tell the driver when we toggle in our out of powersave mode
    if(0 == g_AT91_USART_Driver.m_RefFlags[comPort])
    {
        if(0 != previousRefFlags)
        {
            USART_PowerSave( comPort, 1 );
        }
    }
    else
    {
        if(0 == previousRefFlags)
        {
            USART_PowerSave( comPort, 0 );
        }
    }
}

BOOL AT91_USART_Driver::RxBufferFullInterruptState( int comPort )
{
    AT91_USART &usart = AT91::USART(comPort);
    
     if(0 != (c_RefFlagRx & g_AT91_USART_Driver.m_RefFlags[comPort]))
    {
        return (0 != (usart.US_IMR & AT91_USART::US_RXRDY));
    }

    return FALSE;
}

void AT91_USART_Driver::USART_ISR( void* param )
{
    // we lock since we are low priority and anyone might try to add a char in another ISR
    GLOBAL_LOCK(irq);

    UINT32  comPort = (UINT32)param;

    AT91_USART &usart = AT91::USART(comPort);
 
    // if the charger came in and turned off the serial port, bail now, as the source
    // will be going away

    char c;
    UINT32 Status;  

    Status = usart.US_CSR;
    if(Status & AT91_USART::US_RXRDY)
    {
        c = usart.US_RHR;
        USART_AddCharToRxBuffer( comPort, c );

        Events_Set( SYSTEM_EVENT_FLAG_COM_IN );

    }
    if(Status & AT91_USART::US_TXRDY)
    {
        if(0 == (c_RefFlagTx & g_AT91_USART_Driver.m_RefFlags[comPort]))
        {
            return;
        }
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
}

BOOL AT91_USART_Driver::TxHandshakeEnabledState( int comPort )
{
    // TODO: Add handshake code
    return TRUE;
}

void AT91_USART_Driver::ProtectPins( int comPort, BOOL on )
{
    GLOBAL_LOCK(irq);

    AT91_GPIO_Driver::GPIO_NAMES    SER_TDX;
    AT91_GPIO_Driver::GPIO_NAMES    SER_RDX;

    switch(comPort)
    {
        case 0:
            SER_RDX = AT91_DRXD;
            SER_TDX = AT91_DTXD;
            break;
#if ( AT91_MAX_USART > 1)                    
        case 1:
            SER_RDX = AT91_RXD0;
            SER_TDX = AT91_TXD0;
            break;
#endif
#if ( AT91_MAX_USART > 2)            
            case 2:
            SER_RDX = AT91_RXD1;
            SER_TDX = AT91_TXD1;
            break;
#endif
#if ( AT91_MAX_USART > 3)            
        case 3:
            SER_RDX = AT91_RXD2;
            SER_TDX = AT91_TXD2;
            break;
#endif        
        default: return;        
    }
    
    if(on)
    {
        RxBufferFullInterruptEnable( comPort, FALSE );
        TxBufferEmptyInterruptEnable( comPort, FALSE );

        // Config the GPIO as Input to save power
        CPU_GPIO_EnableInputPin( (GPIO_PIN)SER_RDX, FALSE, NULL, GPIO_INT_NONE, RESISTOR_DISABLED );            
        CPU_GPIO_EnableInputPin( (GPIO_PIN)SER_TDX, FALSE, NULL, GPIO_INT_NONE, RESISTOR_DISABLED );            
    }
    else
    {
        CPU_GPIO_DisablePin( (GPIO_PIN)SER_TDX, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1 );

        TxBufferEmptyInterruptEnable( comPort, TRUE );

        CPU_GPIO_DisablePin( (GPIO_PIN)SER_RDX, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1 );

        RxBufferFullInterruptEnable( comPort, TRUE );
    }
}

void AT91_USART_Driver::GetPins(int comPort, GPIO_PIN& rxPin, GPIO_PIN& txPin, GPIO_PIN& ctsPin, GPIO_PIN& rtsPin)
{
    switch(comPort)
    {
        case 0:
            rxPin  = (GPIO_PIN)AT91_DRXD;
            txPin  = (GPIO_PIN)AT91_DTXD;
            ctsPin = GPIO_PIN_NONE;
            rtsPin = GPIO_PIN_NONE;
             break;
#if ( AT91_MAX_USART > 1)                    
        case 1:
            rxPin =  (GPIO_PIN)AT91_RXD0;
            txPin =  (GPIO_PIN)AT91_TXD0;
            ctsPin = (GPIO_PIN)AT91_CTS0;
            rtsPin = (GPIO_PIN)AT91_RTS0;            
             break;
#endif
#if ( AT91_MAX_USART > 2)            
            case 2:
            rxPin = (GPIO_PIN)AT91_RXD1;
            txPin = (GPIO_PIN)AT91_TXD1;

// to be fill in by Atmel
            ctsPin = GPIO_PIN_NONE;
            rtsPin = GPIO_PIN_NONE;            
             break;
#endif
#if ( AT91_MAX_USART > 3)            
        case 3:

            rxPin = (GPIO_PIN)AT91_RXD2;
            txPin = (GPIO_PIN)AT91_TXD2;
// to be fill in by Atmel
            ctsPin = GPIO_PIN_NONE;
            rtsPin = GPIO_PIN_NONE;            
 
            break;
#endif        
        default: return;        
    }
}
void  AT91_USART_Driver::BaudrateBoundary(int ComPortNum, UINT32& maxBaudrateHz, UINT32& minBaudrateHz)
{
    maxBaudrateHz = AT91_USART::c_MAX_BAUDRATE;
    minBaudrateHz = AT91_USART::c_MIN_BAUDRATE;

}

BOOL AT91_USART_Driver::IsBaudrateSupported(int ComPortNum, UINT32 & BaudrateHz)
{
    if (BaudrateHz <= AT91_USART::c_MAX_BAUDRATE)
    {
        return true;
    }
    else
    {
        BaudrateHz = AT91_USART::c_MAX_BAUDRATE;
        return false;
    }
}


