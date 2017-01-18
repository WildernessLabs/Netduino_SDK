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
#include "..\LPC24XX.h"

///////////////////////////////////////////////////////////////////////////////

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_LPC24XX_I2C_Driver"
#endif

LPC24XX_I2C_Driver g_LPC24XX_I2C_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif


//--//


BOOL I2C_Internal_Initialize()
{
    return LPC24XX_I2C_Driver::Initialize();
}

BOOL I2C_Internal_Uninitialize()
{
    return LPC24XX_I2C_Driver::Uninitialize();
}

void I2C_Internal_XActionStart( I2C_HAL_XACTION* xAction, bool repeatedStart )
{
    LPC24XX_I2C_Driver::MasterXAction_Start( xAction, repeatedStart );
}

void I2C_Internal_XActionStop()
{
    LPC24XX_I2C_Driver::MasterXAction_Stop();
}

void  I2C_Internal_GetClockRate( UINT32 rateKhz, UINT8& clockRate, UINT8& clockRate2)
{
    clockRate = LPC24XX_I2C_Driver::GetClockRate( rateKhz);
    //clockrate2 not use.
    clockRate2 = 0;    
}

void I2C_Internal_GetPins(GPIO_PIN& scl, GPIO_PIN& sda)
{
    return LPC24XX_I2C_Driver::GetPins( scl, sda);
}

///////////////////////////////////////////////////////////////////////////////

BOOL LPC24XX_I2C_Driver::Initialize()
{
    LPC24XX_I2C& I2C = LPC24XX::I2C();
    
    g_LPC24XX_I2C_Driver.m_currentXAction     = NULL;
    g_LPC24XX_I2C_Driver.m_currentXActionUnit = NULL;

    CPU_GPIO_DisablePin( LPC24XX_I2C::c_I2C_SDA, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1 );
    CPU_GPIO_DisablePin( LPC24XX_I2C::c_I2C_SCL, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1 );

    CPU_INTC_ActivateInterrupt( LPC24XX_VIC::c_IRQ_INDEX_I2C0, &LPC24XX_I2C_Driver::ISR, NULL );


    // enable the I2c module
    I2C.I2CONSET  = LPC24XX_I2C::I2EN;
    
    // set the Subordinate address
    I2C.I2ADR = 0x7E;

    return TRUE;
}

BOOL LPC24XX_I2C_Driver::Uninitialize()
{
    LPC24XX_I2C& I2C = LPC24XX::I2C();

    CPU_INTC_DeactivateInterrupt( LPC24XX_VIC::LPC24XX_VIC::c_IRQ_INDEX_I2C0 );

    I2C.I2CONCLR = ( LPC24XX_I2C::AA | LPC24XX_I2C::SI | LPC24XX_I2C::STO | LPC24XX_I2C::STA | LPC24XX_I2C::I2EN );

    return TRUE;
}

void LPC24XX_I2C_Driver::MasterXAction_Start( I2C_HAL_XACTION* xAction, bool repeatedStart )
{
    LPC24XX_I2C& I2C = LPC24XX::I2C();
    
    g_LPC24XX_I2C_Driver.m_currentXAction     = xAction;
    g_LPC24XX_I2C_Driver.m_currentXActionUnit = xAction->m_xActionUnits[ xAction->m_current ]; ++xAction->m_current;
    
    if(!repeatedStart)
    {        
        /////////////////////////////////
        // Start sequence
        I2C.I2SCLH = xAction->m_clockRate ;
        I2C.I2SCLL = xAction->m_clockRate ;

        I2C.I2CONSET = LPC24XX_I2C::STA;

        // Start sequence
        /////////////////////////////////
    }
    else
    {
        /////////////////////////////////
        // Repeated start sequence
        I2C.I2CONSET = LPC24XX_I2C::STA;
        // Repeated start sequence
        /////////////////////////////////
    }
    
}

void LPC24XX_I2C_Driver::MasterXAction_Stop()
{
    LPC24XX_I2C& I2C = LPC24XX::I2C();

    // unset all flags that depend on the xaction flow

    I2C.I2CONSET = LPC24XX_I2C::STO;
    I2C.I2CONCLR = LPC24XX_I2C::AA | LPC24XX_I2C::SI | LPC24XX_I2C::STA;
    
    g_LPC24XX_I2C_Driver.m_currentXAction     = NULL;
    g_LPC24XX_I2C_Driver.m_currentXActionUnit = NULL;
}

UINT8 LPC24XX_I2C_Driver::GetClockRate( UINT32 rateKhz )
{
    UINT32 divider = LPC24XX_I2C::c_I2C_Clk_KHz / ( 2 * rateKhz );
   
    return (UINT8) divider;
}

void LPC24XX_I2C_Driver::WriteToSubordinate( I2C_HAL_XACTION_UNIT* unit )
{
    UINT8* queueData;

    LPC24XX_I2C& I2C = LPC24XX::I2C();

    queueData = unit->m_dataQueue.Pop();
    
    I2C.I2DAT = *queueData;

    ++unit->m_bytesTransferred;
    --unit->m_bytesToTransfer;
}

void LPC24XX_I2C_Driver::ReadFromSubordinate( I2C_HAL_XACTION_UNIT* unit )
{
    UINT8* queueData;

    LPC24XX_I2C& I2C = LPC24XX::I2C();

    queueData = unit->m_dataQueue.Push();

    ASSERT(queueData);

    UINT8 data = I2C.I2DAT;
    
    *queueData = data;

    ++unit->m_bytesTransferred;
    --unit->m_bytesToTransfer;
}

void LPC24XX_I2C_Driver::ISR( void* arg )
{
    UINT8 address;
    LPC24XX_I2C& I2C = LPC24XX::I2C();
    
    // read status
    volatile UINT32 status;
    
    I2C_HAL_XACTION*      xAction = g_LPC24XX_I2C_Driver.m_currentXAction;
    I2C_HAL_XACTION_UNIT* unit    = g_LPC24XX_I2C_Driver.m_currentXActionUnit;
        
    ASSERT(xAction);
    ASSERT(unit);

    status = I2C.I2STAT & 0xFF;


    switch(status)
    {
    case 0x08: // Start Condition transmitted
    case 0x10: // Repeated Start Condition transmitted
        // Write Subordinate address and Data direction
        address  = 0xFE & (xAction->m_address << 1);
        address |= unit->IsReadXActionUnit() ? LPC24XX_I2C_Driver::c_DirectionRead : LPC24XX_I2C_Driver::c_DirectionWrite;
        I2C.I2DAT = address;
        // Clear STA bit
        I2C.I2CONCLR = LPC24XX_I2C::STA;
        break;
    case 0x18: // Subordinate Address + W transmitted, Ack received
    case 0x28: // Data transmitted, Ack received
        // Write data
        // transaction completed
        if(unit->m_bytesToTransfer == 0)
        {
            if(xAction->ProcessingLastUnit())
            {
                xAction->Signal( I2C_HAL_XACTION::c_Status_Completed );
            }
            else
            {
                MasterXAction_Start( xAction, true );
            }
        }
        else
        {
            WriteToSubordinate( unit );
        }
        break;
    case 0x20: // Write Address not acknowledged by Subordinate
    case 0x30: // Data not acknowledged by Subordinate
    case 0x48: // Read Address not acknowledged by Subordinate
        xAction->Signal( I2C_HAL_XACTION::c_Status_Aborted );
        break;
    case 0x38: // Arbitration lost
        xAction->Signal( I2C_HAL_XACTION::c_Status_Aborted );
        break;
    case 0x40: // Subordinate Address + R transmitted, Ack received
        // if the transaction is one byte only to read, then we must send NAK immediately
        if(unit->m_bytesToTransfer == 1)
        {
            I2C.I2CONCLR = LPC24XX_I2C::AA;
        }
        else
        {
            I2C.I2CONSET = LPC24XX_I2C::AA;
        }
        break;
    case 0x50: // Data received, Ack Sent
    case 0x58: // Data received, NO Ack sent
        // read next byte
        ReadFromSubordinate( unit );   
        if(unit->m_bytesToTransfer == 1)
        {
            I2C.I2CONCLR = LPC24XX_I2C::AA;
        }
        if(unit->m_bytesToTransfer == 0)
        {
            if(xAction->ProcessingLastUnit())
            {
                // send transaction stop
                xAction->Signal( I2C_HAL_XACTION::c_Status_Completed );                     
            }
            else
            {
                // start next
                MasterXAction_Start( xAction, true );            
            }
        }
        break;
    case 0x00: // Bus Error
        // Clear Bus error
        I2C.I2CONSET = LPC24XX_I2C::STO;
        xAction->Signal( I2C_HAL_XACTION::c_Status_Aborted );
        break;
    default:
        xAction->Signal( I2C_HAL_XACTION::c_Status_Aborted );
        break;
    } // switch(status) 

    // clear the interrupt flag to start the next I2C transfer
    I2C.I2CONCLR = LPC24XX_I2C::SI;
}

void  LPC24XX_I2C_Driver::GetPins(GPIO_PIN& scl, GPIO_PIN& sda)
{
    scl = LPC24XX_I2C::c_I2C_SCL;
    sda = LPC24XX_I2C::c_I2C_SDA;
}
