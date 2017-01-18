////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

///////////////////////////////////////////////////////////////////////////////

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_MC9328MXL_I2C_Driver"
#endif

MC9328MXL_I2C_Driver g_MC9328MXL_I2C_Driver;


UINT16 MC9328MXL_I2C_Driver::s_Divider[64] = 
{
     30,  32,  36,  42,  48,  52,  60,  72,   80,   88,  104,  128,  144,  160,  192,  240,
    288, 320, 384, 480, 576, 640, 768, 960, 1152, 1280, 1536, 1920, 2304, 2560, 3072, 3840,
     22,  24,  26,  28,  32,  36,  40,  44,   48,   56,   64,   72,   80,   96,  112,  128,
    160, 192, 224, 256, 320, 384, 448, 512,  640, 768,   896, 1024, 1280, 1536, 1792, 2048,    
};

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

BOOL MC9328MXL_I2C_Driver::Initialize()
{
    NATIVE_PROFILE_HAL_PROCESSOR_I2C();

    if(g_MC9328MXL_I2C_Driver.m_initialized == FALSE)
    {
        MC9328MXL_I2C& I2C = MC9328MXL::I2C();
        
        g_MC9328MXL_I2C_Driver.m_currentXAction     = NULL;
        g_MC9328MXL_I2C_Driver.m_currentXActionUnit = NULL;

        CPU_INTC_ActivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_I2C_INT, &MC9328MXL_I2C_Driver::ISR, NULL );

        // enable the I2c module
        I2C.I2CR = MC9328MXL_I2C::I2CR__IEN;
        
        // enable interrupts
        I2C.I2CR |= MC9328MXL_I2C::I2CR__IIEN;

        // set the Subordinate address
        I2C.IADR = 0x7E;

        // clear arbitration lost and bus busy
        I2C.I2SR &= ~(MC9328MXL_I2C::I2SR__IAL | MC9328MXL_I2C::I2SR__IBB);

        g_MC9328MXL_I2C_Driver.m_initialized = TRUE;
    }

    // even if the driver is initialized already we should still change the pins
    // to their i2c function, in case they have been stolen by another peripheral 
    CPU_GPIO_DisablePin( MC9328MXL_I2C::c_I2C_SDA, RESISTOR_PULLUP, MC9328MXL_GPIO::DDIR__IN,  GPIO_ALT_PRIMARY );
    CPU_GPIO_DisablePin( MC9328MXL_I2C::c_I2C_SCL, RESISTOR_PULLUP, MC9328MXL_GPIO::DDIR__OUT, GPIO_ALT_PRIMARY );

    return TRUE;
}

BOOL MC9328MXL_I2C_Driver::Uninitialize()
{
    NATIVE_PROFILE_HAL_PROCESSOR_I2C();

    if(g_MC9328MXL_I2C_Driver.m_initialized == TRUE)
    {
        MC9328MXL_I2C& I2C = MC9328MXL::I2C();

        CPU_INTC_DeactivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_I2C_INT );

        I2C.I2CR = 0;

        g_MC9328MXL_I2C_Driver.m_initialized = FALSE;
    }
    
    return TRUE;
}

void MC9328MXL_I2C_Driver::MasterXAction_Start( I2C_HAL_XACTION* xAction, bool repeatedStart )
{
    NATIVE_PROFILE_HAL_PROCESSOR_I2C();
    UINT8 control;
    UINT8 address;

    MC9328MXL_I2C& I2C = MC9328MXL::I2C();
    
    g_MC9328MXL_I2C_Driver.m_currentXAction     = xAction;
    g_MC9328MXL_I2C_Driver.m_currentXActionUnit = xAction->m_xActionUnits[ xAction->m_current ]; ++xAction->m_current;
        
    address  = 0xFE & (xAction->m_address << 1);
    address |= g_MC9328MXL_I2C_Driver.m_currentXActionUnit->IsReadXActionUnit() ? MC9328MXL_I2C_Driver::c_DirectionRead : MC9328MXL_I2C_Driver::c_DirectionWrite;
    
    if(!repeatedStart)
    {        
        /////////////////////////////////
        // Start sequence
        I2C.IFDR = xAction->m_clockRate & MC9328MXL_I2C::IFDR_mask;

        control = MC9328MXL_I2C::I2CR__MSTA | MC9328MXL_I2C::I2CR__MTX;

        // if the transaction is one byte only to read, then we must send NAK immediately
        if((g_MC9328MXL_I2C_Driver.m_currentXActionUnit->m_bytesToTransfer == 1) && (address & MC9328MXL_I2C_Driver::c_DirectionRead))
        {
            control |= MC9328MXL_I2C::I2CR__TXAK;
        }

        I2C.I2CR |= control;
        // Start sequence
        /////////////////////////////////
    }
    else
    {
        /////////////////////////////////
        // Repeated start sequence
        I2C.I2CR |= MC9328MXL_I2C::I2CR__RSTA;
        // Repeated start sequence
        /////////////////////////////////
    }
    
    // send device address
    I2C.I2DR = address;
}

void MC9328MXL_I2C_Driver::MasterXAction_Stop()
{
    NATIVE_PROFILE_HAL_PROCESSOR_I2C();
    MC9328MXL_I2C& I2C = MC9328MXL::I2C();

    // unset all flags that depend on the xaction flow
    I2C.I2CR &= ~(MC9328MXL_I2C::I2CR__MSTA | MC9328MXL_I2C::I2CR__MTX | MC9328MXL_I2C::I2CR__TXAK | MC9328MXL_I2C::I2CR__RSTA);

    g_MC9328MXL_I2C_Driver.m_currentXAction     = NULL;
    g_MC9328MXL_I2C_Driver.m_currentXActionUnit = NULL;
}

UINT8 MC9328MXL_I2C_Driver::GetClockRate( UINT32 rateKhz )
{
    NATIVE_PROFILE_HAL_PROCESSOR_I2C();
    UINT32 divider = ((CPU_SystemClock() * (CPU_CMU_ReadPeripheralClock() + 1)) / (rateKhz * 1000));

    ASSERT(s_Divider[0x1F] == 3840);
    
    if(divider >= 3840) return 0x1F;

    UINT16* entry = s_Divider;
    UINT8   best  = 0x3F;
    
    for(UINT8 i = 0; i < ARRAYSIZE(s_Divider); ++i)
    {
        if(*entry >= divider)
        {
            if(s_Divider[best] > *entry)
            {
                best = i;
            }
        }

        ++entry;
    }

    return best;
}

//--//

BOOL MC9328MXL_I2C_Driver::IsBusBusy()
{
    NATIVE_PROFILE_HAL_PROCESSOR_I2C();
    MC9328MXL_I2C& I2C = MC9328MXL::I2C();

    return (I2C.I2SR & MC9328MXL_I2C::I2SR__IBB) != 0;
}

void MC9328MXL_I2C_Driver::WriteToSubordinate( I2C_HAL_XACTION_UNIT* unit )
{
    NATIVE_PROFILE_HAL_PROCESSOR_I2C();
    UINT8* queueData;

    MC9328MXL_I2C& I2C = MC9328MXL::I2C();

    queueData = unit->m_dataQueue.Pop();
    
    I2C.I2DR = *queueData;

    ++unit->m_bytesTransferred;
    --unit->m_bytesToTransfer;
}

void MC9328MXL_I2C_Driver::ReadFromSubordinate( I2C_HAL_XACTION_UNIT* unit )
{
    NATIVE_PROFILE_HAL_PROCESSOR_I2C();
    UINT8* queueData;

    MC9328MXL_I2C& I2C = MC9328MXL::I2C();

    queueData = unit->m_dataQueue.Push();

    ASSERT(queueData);

    UINT8 data = I2C.I2DR;
    
    *queueData = data;

    ++unit->m_bytesTransferred;
    --unit->m_bytesToTransfer;
}

void MC9328MXL_I2C_Driver::ISR( void* arg )
{
    MC9328MXL_I2C& I2C = MC9328MXL::I2C();
    
    // clear interrupt flag
    I2C.I2SR &= ~MC9328MXL_I2C::I2SR__IIF;

    // read control and status
    UINT8 status = I2C.I2SR;
    UINT8 ctrl   = I2C.I2CR;
    
    I2C_HAL_XACTION*      xAction = g_MC9328MXL_I2C_Driver.m_currentXAction;
    I2C_HAL_XACTION_UNIT* unit    = g_MC9328MXL_I2C_Driver.m_currentXActionUnit;
        
    ASSERT(xAction);
    ASSERT(unit);

    // master mode
    if(ctrl & MC9328MXL_I2C::I2CR__MSTA)
    {        
        // tx
        if(ctrl & MC9328MXL_I2C::I2CR__MTX)
        {                    
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
            // NAK received from Subordinate
            else if(status & MC9328MXL_I2C::I2SR__RXAK)
            {  
                xAction->Signal( I2C_HAL_XACTION::c_Status_Aborted );
            }
            else
            {
                // end of address cycle
                if(unit->IsReadXActionUnit())
                {
                    I2C.I2CR &= ~MC9328MXL_I2C::I2CR__MTX;

                    UINT8 dummy = I2C.I2DR;
                }
                // write next byte
                else
                {
                    WriteToSubordinate( unit );
                }
            }
        }
        // rx
        else
        {
            bool lastUnit = xAction->ProcessingLastUnit();
            
            // prepare for nak
            if(unit->m_bytesToTransfer == 2)
            {
                I2C.I2CR |= MC9328MXL_I2C::I2CR__TXAK;
                
            }
            
            if(unit->m_bytesToTransfer == 1)
            {
                if(lastUnit)
                {
                    // send transaction stop
                    xAction->Signal( I2C_HAL_XACTION::c_Status_Completed );                     
                }
            }
            
            // read next byte
            ReadFromSubordinate( unit );   
        
            if(!lastUnit)
            {
                // start next
                MasterXAction_Start( xAction, true );            
            }
        }
    }
    // Subordinate mode
    else
    {
        debug_printf( "[MC9328MXL_I2C_Driver::ISR] arbitration lost\r\n" );

        I2C.I2SR &= ~MC9328MXL_I2C::I2SR__IAL;

        if(I2C.I2SR & MC9328MXL_I2C::I2SR__IAAS)
        {
            debug_printf( "[MC9328MXL_I2C_Driver::ISR] addressed as Subordinate\r\n" );            
        }
        
        xAction->Signal( I2C_HAL_XACTION::c_Status_Aborted );
    }
}

void  MC9328MXL_I2C_Driver::GetPins(GPIO_PIN& scl, GPIO_PIN& sda)
{
    NATIVE_PROFILE_HAL_PROCESSOR_I2C();

    scl = MC9328MXL_I2C::c_I2C_SCL;
    sda = MC9328MXL_I2C::c_I2C_SDA;
}


