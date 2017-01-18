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
#include "SH7264_I2C.h"
#include "..\iodefine.h"
#include <machine.h>
///////////////////////////////////////////////////////////////////////////////

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_SH7264_I2C_Driver"
#endif

SH7264_I2C_Driver g_SH7264_I2C_Driver;
UINT8 State;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

UINT32 ICC_Clock_RateHz[16] =
{
	818,//KHz
	692,
	562,
	500,
	428,
	391,
	360,
	333,
	204,
	173,
	140,
	125,
	107,
	98,
	90,
	83
};

//--//


BOOL I2C_Internal_Initialize()
{
    return SH7264_I2C_Driver::Initialize();
}

BOOL I2C_Internal_Uninitialize()
{
    return SH7264_I2C_Driver::Uninitialize();
}

void I2C_Internal_XActionStart( I2C_HAL_XACTION* xAction, bool repeatedStart )
{
    SH7264_I2C_Driver::MasterXAction_Start( xAction, repeatedStart );
}

void I2C_Internal_XActionStop()
{
    SH7264_I2C_Driver::MasterXAction_Stop();
}

void  I2C_Internal_GetClockRate( UINT32 rateKhz, UINT8& clockRate, UINT8& clockRate2)
{
    clockRate = SH7264_I2C_Driver::GetClockRate( rateKhz );
    //clockrate2 not use.
    clockRate2 = 0;
}

void I2C_Internal_GetPins(GPIO_PIN& scl, GPIO_PIN& sda)
{
    return SH7264_I2C_Driver::GetPins( scl, sda);
}

void SH7264_I2C_TXI_ISR()
{
	SH7264_I2C_Driver::IIC_TXI_Isr();
}

void SH7264_I2C_TEI_ISR()
{
	SH7264_I2C_Driver::IIC_TEI_Isr();
}

void SH7264_I2C_RXI_ISR()
{
	SH7264_I2C_Driver::IIC_RXI_Isr();
}

void SH7264_I2C_NAKI_ISR()
{
	SH7264_I2C_Driver::IIC_NAKI_Isr();
}

void SH7264_I2C_STPI_ISR()
{
	SH7264_I2C_Driver::IIC_STPI_Isr();
}
//--//

BOOL SH7264_I2C_Driver::Initialize()
{
	int i;
	volatile struct st_iic3	&I2C = SH7264_I2C_Driver::I2C();

    if(g_SH7264_I2C_Driver.m_initialized == FALSE)
    {
        g_SH7264_I2C_Driver.m_currentXAction     = NULL;
        g_SH7264_I2C_Driver.m_currentXActionUnit = NULL;
		
		switch (c_I2C_Channel)
		{
			case 2:
				//Set pins
				PORT.PECR1.BIT.PE5MD = 1;
				PORT.PECR1.BIT.PE4MD = 1;
				//Ativate
				CPG.STBCR5.BIT.MSTP55 = 0;
				//Interrupt priority
				INTC.IPR16.BIT._IIC32 = 0xf;
				break;
			case 1:
				//Set pins
				PORT.PECR0.BIT.PE3MD = 1;
				PORT.PECR0.BIT.PE2MD = 1;
				//Ativate
				CPG.STBCR5.BIT.MSTP56 = 0;
				//Interrupt priority
				INTC.IPR16.BIT._IIC31 = 0xf;
				break;
			case 0:
				PORT.PECR0.BIT.PE1MD = 1;
				PORT.PECR0.BIT.PE0MD = 1;
				//Ativate
				CPG.STBCR5.BIT.MSTP57 = 0;
				//Interrupt priority
				INTC.IPR16.BIT._IIC30 = 0xf;
	
				break;
		}
		I2C.ICIER.BYTE = 0x00; //Disable all interrupts
        I2C.SAR.BYTE = 0x7E;
        g_SH7264_I2C_Driver.m_initialized = TRUE;
    }

    // even if the driver is initialized already we should still change the pins
    // to their i2c function, in case they have been stolen by another peripheral 
    //CPU_GPIO_DisablePin( SH7264_I2C::c_I2C_SDA, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1 );
    //CPU_GPIO_DisablePin( SH7264_I2C::c_I2C_SCL, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1 );

    return TRUE;
}

BOOL SH7264_I2C_Driver::Uninitialize()
{
	volatile struct st_iic3	&I2C = SH7264_I2C_Driver::I2C();
	
    if(g_SH7264_I2C_Driver.m_initialized == TRUE)
    {	
        I2C.ICIER.BYTE = 0x00; //Disable interrupts

		I2C.ICCR1.BIT.ICE = 0;			// IIC Disbale //

        g_SH7264_I2C_Driver.m_initialized = FALSE;
    }
    
    return TRUE;
}

void SH7264_I2C_Driver::MasterXAction_Start( I2C_HAL_XACTION* xAction, bool repeatedStart )
{
	int timeout;	
	UINT8 address;
	volatile struct st_iic3	&I2C = SH7264_I2C_Driver::I2C();
	
	if(g_SH7264_I2C_Driver.m_initialized == FALSE) return;

	g_SH7264_I2C_Driver.m_currentXAction     = xAction;
    g_SH7264_I2C_Driver.m_currentXActionUnit = xAction->m_xActionUnits[ xAction->m_current ]; ++xAction->m_current;
	I2C_HAL_XACTION_UNIT* unit    = g_SH7264_I2C_Driver.m_currentXActionUnit;

	I2C.ICCR1.BIT.ICE = 0;		// IIC Disable //
	
    I2C.ICMR.BYTE = 0x30; 		// Set the IIC Bus Format to 9 bits
	
	I2C.ICCR1.BYTE = 0x80 | xAction->m_clockRate;		// CKS and IIC Enable //
		
	//Check Bus avalibility
	timeout = IIC_BUS_BUSY_TIMEOUT;
    while(I2C.ICCR2.BIT.BBSY)
    {
        timeout--;
        if (timeout == 0)
        {
			IICFailure();
			
			return;
        }
    }
	
    address  = 0xFE & (xAction->m_address << 1);
	address |= unit->IsReadXActionUnit() ? SH7264_I2C_Driver::c_DirectionRead : SH7264_I2C_Driver::c_DirectionWrite;
	
	I2C.ICCR1.BYTE |= 0x30; //Master transmit
	
	I2C.ICCR2.BYTE = ( I2C.ICCR2.BYTE & 0x3F ) | 0x80;//Start

	I2C.ICDRT = address;//Send slave address
	
	//Waiting until Slave address is sent
    timeout = IIC_TEND_TIMEOUT;
    while ((I2C.ICSR.BIT.TEND) == 0)
    {
        timeout--;
        if (timeout == 0)
        {
            IICFailure();
            return;
        }
    }
	
	//Ckeck ACK
	if(I2C.ICIER.BIT.ACKBR)
	{
		IICFailure();
		return;
	}
		
	if(!unit->IsReadXActionUnit())//Write transaction
    {   
    	//Enable Interrupts TIE, TEIE. transmit data in interrupt handling
		I2C.ICIER.BYTE |= (0xC0);		
	}
    else //Read
    {
        //Change mode to Master Receive
        I2C.ICSR.BIT.TEND = 0;
        I2C.ICCR1.BIT.TRS = 0;
        I2C.ICSR.BIT.TDRE = 0;
		
        //If this read is only for 1 byte
        if (unit->m_bytesToTransfer==1)
        {
			GLOBAL_LOCK(irq);
			
            //The next byte will be the last byte
            //Set ACKBT
            I2C.ICIER.BIT.ACKBT = 1;

            // Set RCVD in ICCR1 to end reception
            I2C.ICCR1.BIT.RCVD = 1;

            //Do a dummy read
            UINT8 dummy = I2C.ICDRR;
			
			//Read the last byte
			IICReadLastByte();
        }
		//If this bytes read is in interrupt handling
		else
		{
            //Set 0 to be sent at Ack Timing - Clear ACKBT in ICIER
            I2C.ICIER.BIT.ACKBT = 0;

            //Dummy read of ICDRR
            UINT8 dummy = I2C.ICDRR;

			//Enable intrrupt RIE, NAKIE, STIE
            I2C.ICIER.BYTE = (0x30);
		}
    }	
}

void SH7264_I2C_Driver::MasterXAction_Stop()
{
	volatile struct st_iic3	&I2C = SH7264_I2C_Driver::I2C();

	if(g_SH7264_I2C_Driver.m_initialized == FALSE) return;
	
    // unset all flags that depend on the xaction flow
	I2C.ICIER.BYTE = (0x00);	//Disable interrupts
	
	//Set slave recieve mode - Set MST and TRS in ICCR1 to 0
	I2C.ICCR1.BYTE &= ~(0x30);   
	
	I2C.ICSR.BYTE = 0; //Clear all flags
	
	I2C.ICCR1.BIT.ICE = 0;		// IIC Disable //
	
    g_SH7264_I2C_Driver.m_currentXAction     = NULL;
    g_SH7264_I2C_Driver.m_currentXActionUnit = NULL;
}

UINT8 SH7264_I2C_Driver::GetClockRate( UINT32 rateKhz )
{
	UINT8 div = 0;
	
	if (ICC_Clock_RateHz[0] <= rateKhz) return 0x0;
	if (ICC_Clock_RateHz[15] >= rateKhz) return 0xF;
	
	while(div <= 15)
	{
		if((ICC_Clock_RateHz[div]) < rateKhz)
			break;
		div++;
	}
	
	if ((ICC_Clock_RateHz[div-1] - rateKhz) < (rateKhz - ICC_Clock_RateHz[div])) div--;//rateKhz is closer ICC_Clock_RateHz[div-1] than ICC_Clock_RateHz[div]
	
    return (div & 0xFF);
}

void SH7264_I2C_Driver::WriteToSubordinate( I2C_HAL_XACTION_UNIT* unit )
{
    UINT8* queueData;
	volatile struct st_iic3	&I2C = SH7264_I2C_Driver::I2C();

	if(g_SH7264_I2C_Driver.m_initialized == FALSE) return;

    queueData = unit->m_dataQueue.Pop();
    
    I2C.ICDRT = *queueData;
	
    ++unit->m_bytesTransferred;
    --unit->m_bytesToTransfer;
}

void SH7264_I2C_Driver::ReadFromSubordinate( I2C_HAL_XACTION_UNIT* unit )
{
    UINT8* queueData;
	volatile struct st_iic3	&I2C = SH7264_I2C_Driver::I2C();
	
	if(g_SH7264_I2C_Driver.m_initialized == FALSE) return;

    queueData = unit->m_dataQueue.Push();

    ASSERT(queueData);

    UINT8 data = I2C.ICDRR;
    
    *queueData = data;

    ++unit->m_bytesTransferred;
    --unit->m_bytesToTransfer;
}

void  SH7264_I2C_Driver::GetPins(GPIO_PIN& scl, GPIO_PIN& sda)
{
    scl = SH7264_I2C_Driver::c_I2C_SCL;
    sda = SH7264_I2C_Driver::c_I2C_SDA;
}

void SH7264_I2C_Driver::IICGenerateStop()
{
    int             timeout;
	UINT8 			temp;
	volatile struct st_iic3	&I2C = SH7264_I2C_Driver::I2C();    
	I2C_HAL_XACTION*      xAction = g_SH7264_I2C_Driver.m_currentXAction;	
    I2C_HAL_XACTION_UNIT* unit    = g_SH7264_I2C_Driver.m_currentXActionUnit;

    // Need to generate stop condition, first, clear the stop status flag,
    // so it can be checked after the stop condition generated
	I2C.ICSR.BIT.STOP = 0;

    //Generate a stop condition - Set BBSY=0 and SCP=0 in ICCR2
	temp = I2C.ICCR2.BYTE;
	temp &= ~(0x60); //SCP=0 
	temp &= ~(0x80); //BBSY=0
    I2C.ICCR2.BYTE = temp;

    // Wait for stop bit in ICSR to be set
    for(timeout = IIC_STOP_TIMEOUT; timeout > 0; timeout--)
    {
        if((I2C.ICSR.BIT.STOP) != 0)
        {
            break;
        }
    }

    //Set slave recieve mode - Set MST and TRS in ICCR1 to 0
    I2C.ICCR1.BYTE &= ~(0x30);
	
	//Clear flag TDRE
	I2C.ICSR.BIT.TDRE = 0;	
}


void SH7264_I2C_Driver::IICFailure()
{
	volatile struct st_iic3	&I2C = SH7264_I2C_Driver::I2C();    
	I2C_HAL_XACTION*      xAction = g_SH7264_I2C_Driver.m_currentXAction;	
	
	//Generate Stop
	IICGenerateStop	();
	
	// IIC Disable //
	I2C.ICCR1.BIT.ICE = 0;			

	//Diable all interupts
	I2C.ICIER.BYTE = 0x00;
	
	xAction->Signal( I2C_HAL_XACTION::c_Status_Aborted );
}

void SH7264_I2C_Driver::IICReadLastByte()
{
	int timeout;
	volatile struct st_iic3	&I2C = SH7264_I2C_Driver::I2C();    
	I2C_HAL_XACTION*      xAction = g_SH7264_I2C_Driver.m_currentXAction;	
    I2C_HAL_XACTION_UNIT* unit    = g_SH7264_I2C_Driver.m_currentXActionUnit;

    //Wait for byte to arrive
	while ((I2C.ICSR.BIT.RDRF) == 0){}
	
	/*  Generate stop condition */
	I2C.ICSR.BIT.STOP = 0;

    //Generate a stop condition - Set BBSY=0 and SCP=0 in ICCR2
    I2C.ICCR2.BYTE &= 0x3F;

    // Wait for stop bit in ICSR to be set
    for(timeout = IIC_STOP_TIMEOUT; timeout > 0; timeout--)
    {
        if((I2C.ICSR.BIT.STOP) != 0)
        {
            break;
        }
    }

	/* End Generate stop condition */

	// Read last data
	ReadFromSubordinate( unit );
	
	// Clear RCVD in ICCR1
	I2C.ICCR1.BIT.RCVD = 0;

    //Set slave recieve mode - Set MST in ICCR1 to 0
    I2C.ICCR1.BYTE &= ~(0x20);
	
	//Clear flag TDRE
	I2C.ICSR.BIT.TDRE = 0;
	
	I2C.ICCR1.BIT.ICE = 0;			// IIC Disable //
	
	if(xAction->ProcessingLastUnit())
	{
		xAction->Signal( I2C_HAL_XACTION::c_Status_Completed );//Complete
	}
	else
	{
		MasterXAction_Start( xAction, true );//Still other transaction
	}
}

// Interrupts for IIC Transmit buffer empty
void SH7264_I2C_Driver::IIC_TXI_Isr()
{
	volatile struct st_iic3	&I2C = SH7264_I2C_Driver::I2C();    
    I2C_HAL_XACTION_UNIT* unit    = g_SH7264_I2C_Driver.m_currentXActionUnit;

	GLOBAL_LOCK(irq);
	    
	if (unit->m_bytesToTransfer)
    {
        //More Data to write, this clears TDRE
		WriteToSubordinate(unit);
    }
    else
    {
        I2C.ICIER.BIT.TIE = 0;
    }
}

// Interrupts for IIC Transmit end
void SH7264_I2C_Driver::IIC_TEI_Isr()
{
	volatile struct st_iic3	&I2C = SH7264_I2C_Driver::I2C();    
	I2C_HAL_XACTION*      xAction = g_SH7264_I2C_Driver.m_currentXAction;
	I2C_HAL_XACTION_UNIT* unit    = g_SH7264_I2C_Driver.m_currentXActionUnit;

	GLOBAL_LOCK(irq);
	
	//Clear flag Stop
	I2C.ICSR.BIT.STOP = 0;

	//Disable interrupts
    I2C.ICIER.BYTE = 0x00;

    // Indicate the transmission is done
	IICGenerateStop();
	
	I2C.ICCR1.BIT.ICE = 0;			// IIC Enable //	
	
	if(xAction->ProcessingLastUnit())
	{
		xAction->Signal( I2C_HAL_XACTION::c_Status_Completed );//completed
	}
	else
	{
		#if(IIC_READ_WRITE_DELAY==1)
			HAL_Time_Sleep_MicroSeconds_InterruptEnabled(500);
		#endif
		MasterXAction_Start( xAction, true );//still have other transaction
	}
}

// Interrupts for IIC Rx buffer full
void SH7264_I2C_Driver::IIC_RXI_Isr()
{
	volatile struct st_iic3	&I2C = SH7264_I2C_Driver::I2C();    
    I2C_HAL_XACTION_UNIT* unit    = g_SH7264_I2C_Driver.m_currentXActionUnit;
	I2C_HAL_XACTION*      xAction = g_SH7264_I2C_Driver.m_currentXAction;
	
	GLOBAL_LOCK(irq);

	while (1)
	{
		//Wait RDRF flag
		while (I2C.ICSR.BIT.RDRF == 0);
		
		if (unit->m_bytesToTransfer == 2)
		{							
			I2C.ICIER.BYTE = 0x00;
					
			// The next byte will be the last byte
			I2C.ICIER.BIT.ACKBT = 1;

			// Set RCVD in ICCR1 to end reception 
			I2C.ICCR1.BIT.RCVD = 1;
		
			//Read the data 
			ReadFromSubordinate(unit);
		
			//read last byte
			IICReadLastByte();
			
			break;
		}
		else
		{
			// Read the data 
			ReadFromSubordinate(unit);
		}
	}
}

// Interrupts for IIC NACK Failure
void SH7264_I2C_Driver::IIC_NAKI_Isr()
{
	volatile struct st_iic3	&I2C = SH7264_I2C_Driver::I2C();    
	I2C_HAL_XACTION*      xAction = g_SH7264_I2C_Driver.m_currentXAction;
	
	GLOBAL_LOCK(irq);
	
    //Clear NACKF
    I2C.ICSR.BIT.NACKF = 0;

	IICGenerateStop();
	
    // Disable interrupts 
    I2C.ICIER.BYTE = 0x00;
	
	xAction->Signal( I2C_HAL_XACTION::c_Status_Aborted );
}

// Interrupts for IIC stop condition generated
void SH7264_I2C_Driver::IIC_STPI_Isr()
{
	volatile struct st_iic3	&I2C = SH7264_I2C_Driver::I2C(); 
    
	GLOBAL_LOCK(irq);
    
	I2C.ICSR.BIT.STOP = 0;
}