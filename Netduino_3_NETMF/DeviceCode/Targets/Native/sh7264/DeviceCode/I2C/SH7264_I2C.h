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

#ifndef _SH7264_I2C_H_
#define _SH7264_I2C_H_ 1


#define 	I2C_KHZ			0xE

#define IIC_STATE_IDLE				1
#define IIC_STATE_BUS_MASTER		2
#define IIC_STATE_READING			3
#define IIC_STATE_WRITING			4
#define IIC_STATE_READ_COMPLETE		5	
#define IIC_STATE_WRITE_COMPLETE	6
#define IIC_STATE_READ_LAST			7

#define IIC_BUS_BUSY_TIMEOUT        10000
#define IIC_STOP_TIMEOUT            10000
#define IIC_TDRE_TIMEOUT            10000
#define IIC_TEND_TIMEOUT            100000
//////////////////////////////////////////////////////////////////////////////
// I2C driver
//

struct I2C_HAL_XACTION_UNIT;
struct I2C_HAL_XACTION;
struct I2C_CONFIGURATION;

struct SH7264_I2C_Driver
{		
    BOOL m_initialized;
	//--//

	#define IIC_READ_WRITE_DELAY 1
	
    I2C_HAL_XACTION*       m_currentXAction;
    I2C_HAL_XACTION_UNIT*  m_currentXActionUnit;

    //--//

    static const UINT8 c_DirectionWrite = 0x00;
    static const UINT8 c_DirectionRead  = 0x01;
	static const UINT32 c_I2C_Base      = 0xFFFEE000;
	static const UINT8  c_I2C_Channel	= 1;    
	static const UINT8  c_I2C_KHZ		= 0x9;
    static const UINT32 c_I2C_SCL 		= 1;
    static const UINT32 c_I2C_SDA 		= 2;
    //--//
	static volatile struct st_iic3	&I2C() {return *(volatile struct st_iic3 *)(c_I2C_Base + 0x400*c_I2C_Channel);}
    static BOOL  Initialize         (                                              );
    static BOOL  Uninitialize       (                                              );
    static void  MasterXAction_Start( I2C_HAL_XACTION* xAction, bool repeatedStart );
    static void  MasterXAction_Stop (                                              );
    static UINT8 GetClockRate       ( UINT32 rateKhz                               );
    static void  GetPins            ( GPIO_PIN& scl, GPIO_PIN& sda                 );
    static void ISR                 ( void*            arg          );
	static void IIC_TXI_Isr();
	static void IIC_TEI_Isr();
	static void IIC_RXI_Isr();
	static void IIC_NAKI_Isr();
	static void IIC_STPI_Isr();
private:
    static void WriteToSubordinate ( I2C_HAL_XACTION_UNIT* xAction );
    static void ReadFromSubordinate( I2C_HAL_XACTION_UNIT* xAction );
	static void IICReadLastByte();
	static void IICGenerateStop();
	static void IICFailure();
};

void SH7264_I2C_TXI_ISR();
void SH7264_I2C_TEI_ISR();
void SH7264_I2C_RXI_ISR();
void SH7264_I2C_NAKI_ISR();
void SH7264_I2C_STPI_ISR();

extern SH7264_I2C_Driver g_SH7264_I2C_Driver;

//
// I2C
#endif // _SH7264_I2C_ 1
