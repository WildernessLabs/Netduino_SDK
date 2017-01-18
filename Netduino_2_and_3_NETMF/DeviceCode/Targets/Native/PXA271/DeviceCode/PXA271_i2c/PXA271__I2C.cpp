////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

///////////////////////////////////////////////////////////////////////////////

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_PXA271_I2C_Driver"
#endif

PXA271_I2C_Driver g_PXA271_I2C_Driver;

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

BOOL PXA271_I2C_Driver::Initialize()
{
    PXA271_I2C& I2C = PXA271::I2C();

    if(g_PXA271_I2C_Driver.m_initialized == FALSE)
    {
        g_PXA271_I2C_Driver.m_currentXAction     = NULL;
        g_PXA271_I2C_Driver.m_currentXActionUnit = NULL;
        
        // At this point, the I2C pins default to GPIO inputs and remain that way until MasterXAction_Start is called
        CPU_INTC_ActivateInterrupt( PXA271_AITC::c_IRQ_INDEX_I2C, &PXA271_I2C_Driver::ISR, NULL );

        // set the Subordinate address
        I2C.ISAR = 0x7E;
        
        g_PXA271_I2C_Driver.m_initialized = TRUE;
    }

    return TRUE;
}

BOOL PXA271_I2C_Driver::Uninitialize()
{
    PXA271_I2C& I2C = PXA271::I2C();

    if(g_PXA271_I2C_Driver.m_initialized == TRUE)
    {
        // Reset the I2C unit
        while(I2C.ISR & I2C.ISR__UB) ;      // Wait until the I2C unit is not busy
        I2C.ICR = I2C.ICR__UR;              // Reset and disable the I2C unit
        I2C.ISR = 0;                        // Clear all interrupts
        I2C.ICR = 0;                        // Clear the reset condition

        CPU_INTC_DeactivateInterrupt( PXA271_AITC::c_IRQ_INDEX_I2C );

        // Set the I2C pins as inputs
        CPU_GPIO_EnableInputPin( PXA271_I2C::c_I2C_SDA, FALSE, NULL, GPIO_INT_NONE, RESISTOR_DISABLED );
        CPU_GPIO_EnableInputPin( PXA271_I2C::c_I2C_SCL, FALSE, NULL, GPIO_INT_NONE, RESISTOR_DISABLED );

        // disable clock to I2C unit
        PXA271::CLKMNGR().CKEN &= ~PXA271_CLOCK_MANAGER::CKEN__I2C_CLK;

        g_PXA271_I2C_Driver.m_initialized = FALSE;
    }

    return TRUE;
}

void PXA271_I2C_Driver::MasterXAction_Start( I2C_HAL_XACTION* xAction, bool repeatedStart )
{
    UINT32 control = PXA271_I2C::ICR__DRFIE | PXA271_I2C::ICR__ITEIE | PXA271_I2C::ICR__IUE | PXA271_I2C::ICR__SCLEA | PXA271_I2C::ICR__GCD | PXA271_I2C::ICR__BEIE;
    UINT32 address;

    // Enable appropriate pins for I2C
    CPU_GPIO_DisablePin( PXA271_I2C::c_I2C_SDA, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_OUT, GPIO_ALT_MODE_1 );
    CPU_GPIO_DisablePin( PXA271_I2C::c_I2C_SCL, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_OUT, GPIO_ALT_MODE_1 );

    // enable clock to I2C unit
    PXA271::CLKMNGR().CKEN |= PXA271_CLOCK_MANAGER::CKEN__I2C_CLK;
    
    PXA271_I2C& I2C = PXA271::I2C();
    
    g_PXA271_I2C_Driver.m_currentXAction     = xAction;
    g_PXA271_I2C_Driver.m_currentXActionUnit = xAction->m_xActionUnits[ xAction->m_current ]; ++xAction->m_current;

    address  = 0xFE & (xAction->m_address << 1);

	if(g_PXA271_I2C_Driver.m_currentXActionUnit->IsReadXActionUnit())
	{
		address |= PXA271_I2C_Driver::c_DirectionRead;
	}
	else
	{
		address |= PXA271_I2C_Driver::c_DirectionWrite;
	}

    // Use high bit rate if requested
    if( xAction->m_clockRate == PXA271_I2C::ICR__FM_400KPS )
    {
        control |= PXA271_I2C::ICR__FM;
    }

    // Get address and R/nW ready to send
    I2C.IDBR = address;

    // Set up for sending the address
    control |= PXA271_I2C::ICR__START | PXA271_I2C::ICR__TB;
    
    // Initiate the send operation
    I2C.ICR = control;
}

void PXA271_I2C_Driver::MasterXAction_Stop()
{
    PXA271_I2C& I2C = PXA271::I2C();

    // unset all flags that depend on the xaction flow
    I2C.ICR = PXA271_I2C::ICR__MA;

    g_PXA271_I2C_Driver.m_currentXAction     = NULL;
    g_PXA271_I2C_Driver.m_currentXActionUnit = NULL;
}

UINT8 PXA271_I2C_Driver::GetClockRate( UINT32 rateKhz )
{
    if( rateKhz >= 400)     // Fast rate is 400 KHz, standard is 100 KHz - not much choice
    {
        return PXA271_I2C::ICR__FM_400KPS;
    }
    return PXA271_I2C::ICR__FM_100KPS;
}

//--//

BOOL PXA271_I2C_Driver::IsBusBusy()
{
    PXA271_I2C& I2C = PXA271::I2C();

    return (I2C.ISR & PXA271_I2C::ISR__IBB) != 0;
}

void PXA271_I2C_Driver::WriteToSubordinate( I2C_HAL_XACTION_UNIT* unit )
{
    UINT8* queueData;

    PXA271_I2C& I2C = PXA271::I2C();

    queueData = unit->m_dataQueue.Pop();

    I2C.IDBR = *queueData;              // Get data ready to send

    I2C.ICR |= PXA271_I2C::ICR__TB;     // Initiate the transmission

    ++unit->m_bytesTransferred;
    --unit->m_bytesToTransfer;
}

void PXA271_I2C_Driver::ReadFromSubordinate( I2C_HAL_XACTION_UNIT* unit )
{
    UINT8* queueData;

    PXA271_I2C& I2C = PXA271::I2C();

    queueData = unit->m_dataQueue.Push();

    ASSERT(queueData);

    if(queueData != NULL)
    {
        UINT32 data = I2C.IDBR;

        *queueData = data;

        unit->m_bytesTransferred++;
        unit->m_bytesToTransfer--;
    }
}

void PXA271_I2C_Driver::ISR( void* arg )
{
    PXA271_I2C& I2C = PXA271::I2C();

    // read control and status
    UINT32 status = I2C.ISR;
    UINT32 ctrl   = I2C.ICR;

    I2C_HAL_XACTION*      xAction = g_PXA271_I2C_Driver.m_currentXAction;
    I2C_HAL_XACTION_UNIT* unit    = g_PXA271_I2C_Driver.m_currentXActionUnit;

    ASSERT(xAction);
    ASSERT(unit);

    if( status & PXA271_I2C::ISR__ITE )        // If Transmit Buffer Empty interrupt
    {
        // clear interrupt flag
        I2C.ISR = PXA271_I2C::ISR__ITE;

        // If arbitration loss detected, clear that, too
        if( status & PXA271_I2C::ISR__ALD )
        {
            I2C.ISR = PXA271_I2C::ISR__ALD;
        }

        if( status & PXA271_I2C::ISR__RWM )    // If we just finished sending an address in Read mode
        {
            if( unit->m_bytesToTransfer == 1 )      // If we are expecting only one byte
            {
                // Must send stop and NAK after receiving last byte
                ctrl &= ~(PXA271_I2C::ICR__START);
                ctrl |= PXA271_I2C::ICR__STOP | PXA271_I2C::ICR__ALDIE | PXA271_I2C::ICR__ACKNAK | PXA271_I2C::ICR__TB;     // Initiate read from Subordinate
            }
            else                                    // If we are expecting more than one byte
            {
                // Do not send a stop and ACK the next received byte
                ctrl &= ~(PXA271_I2C::ICR__START | PXA271_I2C::ICR__STOP | PXA271_I2C::ICR__ACKNAK);
                ctrl |= PXA271_I2C::ICR__ALDIE | PXA271_I2C::ICR__TB;       // Initiate read from Subordinate
            }
            I2C.ICR = ctrl;     // Initiate the read operation
        }
        else        // If we have just finished sending address or data in Write mode
        {
            if(unit->m_bytesToTransfer == 0)
            {
                I2C.ICR &= ~ PXA271_I2C::ICR__STOP;     // Clear the stop bit
                
                if(xAction->ProcessingLastUnit())
                {
                    xAction->Signal( I2C_HAL_XACTION::c_Status_Completed );
                    I2C.ICR = PXA271_I2C::ICR__IUE;     // Shut down I2C port master
                }
                else
                {
                    MasterXAction_Start( xAction, true );
                }
            }
            else if(status & PXA271_I2C::ISR__BED)
            {
                I2C.ISR = PXA271_I2C::ISR__BED;                             // Clear the bus error
                xAction->Signal( I2C_HAL_XACTION::c_Status_Aborted );       // We're done
                I2C.ICR = PXA271_I2C::ICR__IUE;                             // Shut down I2C master
            }
            else
            {
                // If sending the last byte
                if( unit->m_bytesToTransfer == 1 )
                {
                    ctrl &= ~PXA271_I2C::ICR__START;
                    ctrl |= PXA271_I2C::ICR__STOP;
                }
                else
                {
                    ctrl &= ~(PXA271_I2C::ICR__STOP | PXA271_I2C::ICR__START);
                }
                I2C.ICR = ctrl | PXA271_I2C::ICR__ALDIE;      // Set up start/stop and arbitration loss detect
                WriteToSubordinate( unit );              // Set up next byte to transmit
            }
        }
    }
    else if( status & PXA271_I2C::ISR__IRF )       // If Receive Buffer Full interrupt
    {
        // clear interrupt flag
        I2C.ISR = PXA271_I2C::ISR__IRF;
        
        bool lastUnit = xAction->ProcessingLastUnit();

        ReadFromSubordinate( unit );

        // If last byte was just received
        if( unit->m_bytesToTransfer == 0 )
        {
            // Tidy up the control register
            I2C.ICR &= ~(PXA271_I2C::ICR__STOP & PXA271_I2C::ICR__ACKNAK);

            // Finish up the xAction
            if( !lastUnit )                        // If more to XAction
            {
                MasterXAction_Start( xAction, true );            
            }
            else
            {
                xAction->Signal( I2C_HAL_XACTION::c_Status_Completed );
                I2C.ICR = PXA271_I2C::ICR__IUE;     // Shut down I2C master
            }
        }
        else if( unit->m_bytesToTransfer == 1 )     // If exactly 1 byte left to receive
        {
            I2C.ICR &= ~(PXA271_I2C::ICR__START);
            I2C.ICR |= PXA271_I2C::ICR__STOP | PXA271_I2C::ICR__ALDIE | PXA271_I2C::ICR__ACKNAK | PXA271_I2C::ICR__TB;       // Initiate next byte read from Subordinate
        }
        else                                        // If more than one byte left to receive
        {
            I2C.ICR &= ~(PXA271_I2C::ICR__START | PXA271_I2C::ICR__STOP | PXA271_I2C::ICR__ACKNAK);
            I2C.ICR |= PXA271_I2C::ICR__ALDIE | PXA271_I2C::ICR__TB;            // Initiate next byte read from Subordinate
        }
    }
    else if( status & (PXA271_I2C::ISR__BED | PXA271_I2C::ISR__ALD) )        // If there was a bus error
    {
        I2C.ICR = PXA271_I2C::ICR__IUE;        // Shut down I2C interrupts, stop driving bus
        xAction->Signal( I2C_HAL_XACTION::c_Status_Aborted);     // Problem talking to Subordinate - we're done
    }
}

void  PXA271_I2C_Driver::GetPins( GPIO_PIN& scl, GPIO_PIN& sda )
{
    scl = PXA271_I2C::c_I2C_SCL;
    sda = PXA271_I2C::c_I2C_SDA;
}

