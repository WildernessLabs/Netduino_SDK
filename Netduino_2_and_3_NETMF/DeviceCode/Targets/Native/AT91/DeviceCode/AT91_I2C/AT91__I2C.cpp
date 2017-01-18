////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "..\at91.h"

////////////////////////////////////////////////////////////////////////////////
#undef  TRACE_ALWAYS
#define TRACE_ALWAYS               0x00000001

#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

////////////////////////////////////////////////////////////////////////////////

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata
#endif

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_AT91_I2C_Driver"
#endif

AT91_I2C_Driver g_AT91_I2C_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

BOOL AT91_I2C_Driver::Initialize()
{
    AT91_I2C& I2C = AT91::I2C();

    // reset

    if(g_AT91_I2C_Driver.m_initialized == FALSE)
    {
        I2C.TWI_CR = AT91_I2C::TWI_CR_SWRST;
        
        g_AT91_I2C_Driver.m_currentXAction     = NULL;
        g_AT91_I2C_Driver.m_currentXActionUnit = NULL;

        CPU_GPIO_DisablePin( AT91_TWI_SDA, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1 );
        CPU_GPIO_DisablePin( AT91_TWI_SCL, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1 );

        CPU_INTC_ActivateInterrupt( AT91C_ID_TWI, &AT91_I2C_Driver::ISR, NULL );

        AT91_PMC &pmc = AT91::PMC();
        pmc.EnablePeriphClock(AT91C_ID_TWI);

        // set Clock to 400KHz?
//        UINT16 clkDiv;
//        clkDiv = SYSTEM_PERIPHERAL_CLOCK_HZ/(400000*2) -3;
//        I2C.TWI_CWGR = clkDiv | (clkDiv <<AT91_I2C::TWI_CWGR_CHDIV_SHIFT ) | (1 <<AT91_I2C::TWI_CWGR_CKDIV_SHIFT )  ;

        I2C.TWI_MMR = 0x7e<< AT91_I2C::TWI_MMR_DADR_SHIFT;        
//        // enable interrupts
//        I2C.TWI_IER = AT91_I2C::TWI_IER_NACK | AT91_I2C::TWI_IER_RXRDY | AT91_I2C::TWI_IER_TXCOMP | AT91_I2C::TWI_IER_TXRDY;

       g_AT91_I2C_Driver.m_initialized  = TRUE; 
   }
    
    return TRUE;
}

BOOL AT91_I2C_Driver::Uninitialize()
{

    NATIVE_PROFILE_HAL_PROCESSOR_I2C();

    if(g_AT91_I2C_Driver.m_initialized == TRUE)
    {
        AT91_I2C& I2C = AT91::I2C();

        CPU_INTC_DeactivateInterrupt( AT91C_ID_TWI);

        AT91_PMC &pmc = AT91::PMC();
        pmc.DisablePeriphClock(AT91C_ID_TWI);

        // disable
        I2C.TWI_CR = AT91_I2C::TWI_CR_MSDIS;

        // disable all the interrupt
        I2C.TWI_IDR = AT91_I2C::TWI_IDR_NACK | AT91_I2C::TWI_IDR_RXRDY | AT91_I2C::TWI_IDR_TXCOMP | AT91_I2C::TWI_IDR_TXRDY;
        

        g_AT91_I2C_Driver.m_initialized = FALSE;

        CPU_GPIO_EnableInputPin( AT91_TWI_SDA, FALSE, NULL, GPIO_INT_NONE, RESISTOR_DISABLED );
        CPU_GPIO_EnableInputPin( AT91_TWI_SCL, FALSE, NULL, GPIO_INT_NONE, RESISTOR_DISABLED );

    }
    return TRUE;
}

void AT91_I2C_Driver::MasterXAction_Start( I2C_HAL_XACTION* xAction, bool repeatedStart )
{
    NATIVE_PROFILE_HAL_PROCESSOR_I2C();
    UINT32 control  = 0;
    UINT32 address;

    AT91_I2C& I2C = AT91::I2C();
    
    g_AT91_I2C_Driver.m_currentXAction     = xAction;
    g_AT91_I2C_Driver.m_currentXActionUnit = xAction->m_xActionUnits[ xAction->m_current ]; ++xAction->m_current;
        
    address  = (xAction->m_address << AT91_I2C::TWI_MMR_DADR_SHIFT);


    // set MSEN first 
    
#if defined(PLATFORM_ARM_SAM7_ANY)
// for SAM7x and SAM7S
    I2C.TWI_CR = AT91_I2C::TWI_CR_MSEN;
#elif defined(PLATFORM_ARM_SAM9261_ANY)
    I2C.TWI_CR = AT91_I2C::TWI_CR_MSEN | AT91_I2C::TWI_CR_SVDIS;
#endif

    if (g_AT91_I2C_Driver.m_currentXActionUnit->IsReadXActionUnit())
        address |=  AT91_I2C::TWI_MMR_MREAD_R ;

// At91 I2C do not support repeated start.
//    if(!repeatedStart) 
    {        
        /////////////////////////////////
        // Start sequence
        // set clock

        I2C.TWI_CWGR = xAction->m_clockRate | (xAction->m_clockRate <<AT91_I2C::TWI_CWGR_CHDIV_SHIFT ) | (xAction->m_clockRate2 <<AT91_I2C::TWI_CWGR_CKDIV_SHIFT )  ;

        control |= AT91_I2C::TWI_CR_START ;

        // if the transaction is one byte only to read, then we must set Start and Stop bit immediately
        if((g_AT91_I2C_Driver.m_currentXActionUnit->m_bytesToTransfer == 1)  && (address & AT91_I2C::TWI_MMR_MREAD_R ))
        {
            control |= AT91_I2C::TWI_CR_STOP ;
        }

        // Start sequence

        /////////////////////////////////

        I2C.TWI_IER = AT91_I2C::TWI_IER_NACK | AT91_I2C::TWI_IER_RXRDY | AT91_I2C::TWI_IER_TXCOMP | AT91_I2C::TWI_IER_TXRDY;

        I2C.TWI_MMR = address;

        I2C.TWI_CR = control;
       
    }

}

void AT91_I2C_Driver::MasterXAction_Stop()
{
    NATIVE_PROFILE_HAL_PROCESSOR_I2C();
    AT91_I2C& I2C = AT91::I2C();

    // unset all flags that depend on the xaction flow
    I2C.TWI_CR = AT91_I2C::TWI_CR_MSDIS ;
    I2C.TWI_IDR = AT91_I2C::TWI_IDR_NACK | AT91_I2C::TWI_IDR_RXRDY | AT91_I2C::TWI_IDR_TXCOMP | AT91_I2C::TWI_IDR_TXRDY;

    g_AT91_I2C_Driver.m_currentXAction     = NULL;
    g_AT91_I2C_Driver.m_currentXActionUnit = NULL;

}


void AT91_I2C_Driver::GetClockRate( UINT32 rateKhz, UINT8& clockRate, UINT8& clockRate2)
{


#if defined(PLATFORM_ARM_SAM7_ANY)
#define CLOCK_RATE_CONSTANT     3 
#elif defined(PLATFORM_ARM_SAM9261_ANY)
#define CLOCK_RATE_CONSTANT     4
#endif



#define MIN_CLK_RATE    (SYSTEM_PERIPHERAL_CLOCK_HZ/1000)/(128 *255+CLOCK_RATE_CONSTANT)
#define MAX_CLK_RATE    400   //kHz

//clockRate = CHDiv and CLDiv , we put the 50 % duty cycle for the clock
//clockRate2 =>clkDiv


    if (rateKhz < MIN_CLK_RATE)
    {
        clockRate = 255;
        clockRate2 = 7;
    }
    else if (rateKhz >=MAX_CLK_RATE)
    {

        clockRate  = (SYSTEM_PERIPHERAL_CLOCK_HZ/(2*1000))/MAX_CLK_RATE - CLOCK_RATE_CONSTANT;
        clockRate2 = 0;
    }
    else
    {

        UINT32 power = 1;
        UINT32 clkDiv;
        UINT32 clkLHDiv;

        clkDiv = 0;
        clkLHDiv = (SYSTEM_PERIPHERAL_CLOCK_HZ/(2*1000))/rateKhz - CLOCK_RATE_CONSTANT ;

        if (clkLHDiv > 255 )
        {
            clkLHDiv +=CLOCK_RATE_CONSTANT;
            for (clkDiv=1; clkDiv <=7; clkDiv ++)
            {
                clkLHDiv /= 2;
                power *= 2; // save the calculation     
                if (clkLHDiv <=255)
                    break;
            }

            clkLHDiv = (((SYSTEM_PERIPHERAL_CLOCK_HZ/(2*1000))/rateKhz) - CLOCK_RATE_CONSTANT )/power ;
        }
        clockRate = clkLHDiv;
        clockRate2 = clkDiv;
    }    

}



void AT91_I2C_Driver::WriteToSubordinate( I2C_HAL_XACTION_UNIT* unit )
{
    NATIVE_PROFILE_HAL_PROCESSOR_I2C();
    UINT8* queueData;

    AT91_I2C& I2C = AT91::I2C();
    queueData = unit->m_dataQueue.Pop();
    I2C.TWI_THR = *queueData;
    ++unit->m_bytesTransferred;
    --unit->m_bytesToTransfer;

}

void AT91_I2C_Driver::ReadFromSubordinate( I2C_HAL_XACTION_UNIT* unit )
{
    UINT8* queueData;

    AT91_I2C& I2C = AT91::I2C();

    queueData = unit->m_dataQueue.Push();

    ASSERT(queueData);

    UINT8 data = I2C.TWI_RHR;
    
    *queueData = data;
    ++unit->m_bytesTransferred;
    --unit->m_bytesToTransfer;
    
}

void AT91_I2C_Driver::ISR( void* arg )
{    
    AT91_I2C& I2C = AT91::I2C();
   

    // read control and status
    UINT32 status = I2C.TWI_SR;
    UINT32 ctrl   = I2C.TWI_MMR;
    
    I2C_HAL_XACTION*      xAction = g_AT91_I2C_Driver.m_currentXAction;
    I2C_HAL_XACTION_UNIT* unit    = g_AT91_I2C_Driver.m_currentXActionUnit;
        
    ASSERT(xAction);
    ASSERT(unit);

    // tx
    if((ctrl & AT91_I2C::TWI_MMR_MREAD_R ) == 0)
    {                    
        // write operation

        // NAK received from Subordinate
        if(status & AT91_I2C::TWI_SR_NACK)
        {  
            xAction->Signal( I2C_HAL_XACTION::c_Status_Aborted );
        }
        else
        {
            if(unit->m_bytesToTransfer == 0)
            {
        
                if (status & AT91_I2C::TWI_SR_TXCOMP)
                {

                    if(xAction->ProcessingLastUnit())
                    {
                        xAction->Signal( I2C_HAL_XACTION::c_Status_Completed );
                    }
                    else
                    {
                        MasterXAction_Start( xAction, false );
                    }
               }
            }
            else
            {
                // end of address cycle
                if(unit->IsReadXActionUnit())
                {
                 //   UINT8 dummy= I2C.TWI_RHR;
                }
                else if (status && AT91_I2C::TWI_SR_TXRDY)
                {
                    WriteToSubordinate( unit );
                }
            }
        }
    }
    else
    {
        // rx
        if ((status & AT91_I2C::TWI_SR_TXCOMP) && unit->m_bytesToTransfer > 0)
        {
            // operation has aborted (since transmission is complete and the STOP condition has been sent, but data was not read)
            xAction->Signal( I2C_HAL_XACTION::c_Status_Aborted );
        }
        else
        {
            bool lastUnit = xAction->ProcessingLastUnit();

            // read data first 
            if(unit->m_bytesToTransfer > 0)
            {
                if (status & AT91_I2C::TWI_SR_RXRDY)
                {
                    // prepare for nak bytesToTransfer has updated.
                    if(unit->m_bytesToTransfer == 2)
                    {
                       I2C.TWI_CR = AT91_I2C::TWI_CR_STOP ;                
                    }
    
                    ReadFromSubordinate( unit );   
                }
            }    


    // the RxDY and COMP should happens at the same time, have to re-read the databyte.

           UINT32 dataByte = unit->m_bytesToTransfer;

            if( dataByte == 0)
            {
                if (status & AT91_I2C::TWI_SR_TXCOMP)
                {
                    if(lastUnit)
                    {
                        // send transaction stop
                        xAction->Signal( I2C_HAL_XACTION::c_Status_Completed );                     
                    }
                    else 
                    {
                        // start next
                        MasterXAction_Start( xAction, false);            
                    }
                }
           }
       }
    }
}

void  AT91_I2C_Driver::GetPins(GPIO_PIN& scl, GPIO_PIN& sda)
{
    NATIVE_PROFILE_HAL_PROCESSOR_I2C();

    scl = AT91_TWI_SCL;
    sda = AT91_TWI_SDA;
}

