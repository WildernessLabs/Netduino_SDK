/************************************************************/
/*	FileName	: SH7619_GPIO.h							*/
/*	FUNC		: GPIO Register Header File				*/
/************************************************************/

/*
 *    Copyright (C) Renesas Technology America,Ltd. 2009  All rights reserved.
 */
 
#ifndef	_SH7619_GPIO_H_
#define	_SH7619_GPIO_H_

#define VL(x) (*(volatile unsigned long *)(x))
#define VS(x) (*(volatile unsigned short *)(x))
#define VB(x) (*(volatile unsigned char *)(x))

#define PFC_BASE			0xF8050000
//#define SH7619_MAX_GPIO		24
#define SH7619_MAX_GPIO		46

#define PADRH 				VS(PFC_BASE + 0x00)
#define PAIORH 				VS(PFC_BASE + 0x04)
#define PACRH1 				VS(PFC_BASE + 0x08)
#define PACRH2 				VS(PFC_BASE + 0x0A)
#define PBDRL 				VS(PFC_BASE + 0x12)
#define PBIORL 				VS(PFC_BASE + 0x16)
#define PBCRL1 				VS(PFC_BASE + 0x1C)
#define PBCRL2 				VS(PFC_BASE + 0x1E)
#define PCDRH 				VS(PFC_BASE + 0x20)
#define PCDRL 				VS(PFC_BASE + 0x22)
#define PCIORH 				VS(PFC_BASE + 0x24)
#define PCIORL 				VS(PFC_BASE + 0x26)
#define PCCRH2 				VS(PFC_BASE + 0x2A)
#define PCCRL1 				VS(PFC_BASE + 0x2C)
#define PCCRL2 				VS(PFC_BASE + 0x2E)
#define PDDRL 				VS(PFC_BASE + 0x32)
#define PDIORL 				VS(PFC_BASE + 0x36)
#define PDCRL2 				VS(PFC_BASE + 0x3E)
#define PEDRH 				VS(PFC_BASE + 0x40)
#define PEDRL 				VS(PFC_BASE + 0x42)
#define PEIORH 				VS(PFC_BASE + 0x44)
#define PEIORL 				VS(PFC_BASE + 0x46)
#define PECRH1 				VS(PFC_BASE + 0x48)
#define PECRH2 				VS(PFC_BASE + 0x4A)
#define PECRL1 				VS(PFC_BASE + 0x4C)
#define PECRL2 				VS(PFC_BASE + 0x4E)

struct SH7619_GPIO
{
    static const UINT32 c_GPIO_Base = 0xF8050000;

    //--//

    static const UINT32 c_Pin_None  = 0xFFFFFFFF;
	static const UINT32 PC00 =   0;
	static const UINT32 PC01 =   1;
	static const UINT32 PC02 =   2;
	static const UINT32 PC03 =   3;
	static const UINT32 PC04 =   4;
	static const UINT32 PC05 =   5;
	static const UINT32 PC06 =   6;
	static const UINT32 PC07 =   7;
	static const UINT32 PC08 =   8;
	static const UINT32 PC09 =   9;
	static const UINT32 PC10 =   10;
	static const UINT32 PC11 =   11;
	static const UINT32 PC12 =   12;
	static const UINT32 PC13 =   13;
	static const UINT32 PC14 =   14;
	static const UINT32 PC15 =   15;
	static const UINT32 PC16 =   16;
	static const UINT32 PC17 =   17;
	static const UINT32 PC18 =   18;
	static const UINT32 PC19 =   19;
	static const UINT32 PC20 =   20;


	//Key Scan Button//
    static const UINT32 c_Key_1    =   21;
    static const UINT32 c_Key_2    =   22; 
    static const UINT32 c_Key_3    =   23; 
    static const UINT32 c_Key_4    =   24; 
    static const UINT32 c_Key_5    =   25; 
    static const UINT32 c_Key_6    =   26; 
    static const UINT32 c_Key_7    =   27; 
    static const UINT32 c_Key_8    =   28; 
    static const UINT32 c_Key_9    =   29; 
    static const UINT32 c_Key_10   =   30; 
    static const UINT32 c_Key_11   =   31; 
    static const UINT32 c_Key_12   =   32; 
    static const UINT32 c_Key_13   =   33; 
    static const UINT32 c_Key_14   =   34; 
    static const UINT32 c_Key_15   =   35; 
    static const UINT32 c_Key_16   =   36; 
    static const UINT32 c_Key_17   =   37; 
    static const UINT32 c_Key_18   =   38; 
    static const UINT32 c_Key_19   =   39; 
    static const UINT32 c_Key_20   =   40; 
    static const UINT32 c_Key_21   =   41; 
    static const UINT32 c_Key_22   =   42;
    static const UINT32 c_Key_23   =   43;
    static const UINT32 c_Key_24   =   44; 
    static const UINT32 c_Key_25   =   45; 
};

struct SH7619_GPIO_Driver
{
    struct PIN_ISR_DESCRIPTOR
    {
        HAL_COMPLETION                 			m_completion;
        GPIO_INTERRUPT_SERVICE_ROUTINE 			m_isr;
        void*                          			m_param;
        GPIO_PIN                       			m_pin;
        GPIO_INT_EDGE                  			m_intEdge;
        UINT8                          			m_flags;
        UINT8                          			m_status;

        //--//
        
        static const UINT8 c_Flags_Debounce        = 0x01;
        static const UINT8 c_Flags_RequireLowEdge  = 0x02;
        static const UINT8 c_Flags_RequireHighEdge = 0x04;

        static const UINT8 c_Status_AllowLowEdge   = 0x01;
        static const UINT8 c_Status_AllowHighEdge  = 0x02;

        //--//

        static void Fire( void* arg );

        void HandleDebounce( BOOL edge );
    };

    //--//

    enum GPIO_NAMES {
//        PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
//        PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15,
        PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12, PC13, PC14, PC15,
		PC16, PC17, PC18, PC19, PC20, PC21, PC22, PC23, PC24, PC25, PC26, PC27, PC28, PC29, PC30, PC31
//		PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12, PD13, PD14, PD15,
//		PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7, PE8, PE9, PE10, PE11, PE12, PE13, PE14, PE15,
//		PE16, PE17, PE18, PE19, PE20, PE21, PE22, PE23, PE24, PE25, PE26, PE27, PE28, PE29, PE30, PE31
    };

    //--//

    static const UINT32 c_MaxPins           = SH7619_MAX_GPIO;
    static const UINT32 c_MaxPorts          = (SH7619_MAX_GPIO / 8);
    static const UINT32 c_PinsPerPort       =  8;	

    static const INT32  c_MinDebounceTimeMs =    1; // 1 ms
    static const INT32  c_MaxDebounceTimeMs = 5000; // 5  s

    //--//
    
    static const UINT8 c_Gpio_Attributes[c_MaxPins];

    // applies to all GPIO
    static const UINT8 c_NumberofResistorMode = 2;
    static const UINT8 c_GPIO_ResistorMode = (1<<RESISTOR_DISABLED) | (1<<RESISTOR_PULLUP);

    static const UINT8 c_NumberofInterruptMode = 5;
    static const UINT8 c_GPIO_InterruptMode = (1<<GPIO_INT_EDGE_LOW) | (1<<GPIO_INT_EDGE_HIGH ) | (1<<GPIO_INT_EDGE_BOTH) | (1<<GPIO_INT_LEVEL_HIGH) | (1<<GPIO_INT_LEVEL_LOW);


    PIN_ISR_DESCRIPTOR m_PinIsr            [c_MaxPins ];
    UINT32             m_PinReservationInfo[c_MaxPorts];
    UINT32             m_DebounceTicks;	
		
	//--//

    static BOOL Initialize  ();
    static BOOL Uninitialize();
    static UINT32 Attributes( GPIO_PIN Pin );
    static void DisablePin( GPIO_PIN Pin, GPIO_RESISTOR ResistorState, UINT32 direction, GPIO_ALT_MODE AltFunction );
    static void EnableOutputPin( GPIO_PIN Pin, BOOL InitialState );
    static BOOL EnableInputPin( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE ISR, void* ISR_Param, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState );
    static BOOL GetPinState( GPIO_PIN Pin );
    static void SetPinState( GPIO_PIN Pin, BOOL PinState );
    static void ISR();
    static void SetResistor( GPIO_PIN Pin, GPIO_RESISTOR ResistorState );
    static BOOL PinIsBusy( GPIO_PIN Pin );
    static BOOL ReservePin( GPIO_PIN Pin, BOOL fReserve );
    static UINT32 GetDebounce();
    static BOOL SetDebounce( INT64 debounceTimeMilliseconds );
    static void GetPinsMap( UINT8* pins, size_t size );
//	static void SetPinData( GPIO_PIN Pin, BOOL Value );
//	static unsigned short GetPinData( GPIO_PIN Pin);
	
private:

    static UINT32 PinToPort( GPIO_PIN pin )
    {
        ASSERT(pin < c_MaxPins);

        return pin / c_PinsPerPort;
    }

    static UINT32 PinToBit( GPIO_PIN pin )
    {
        ASSERT(pin < c_MaxPins);

        return pin % c_PinsPerPort;
    }
	
	static UINT32 PinToIOPort( GPIO_PIN pin )
    {
        ASSERT(pin < c_MaxPins);

        return pin / (c_PinsPerPort *2);
    }
	
	static UINT32 PinToIOBit( GPIO_PIN pin )
    {
        ASSERT(pin < c_MaxPins);

        return pin % 16;
    }

    static UINT32 BitToPin( UINT32 bit, UINT32 port )
    {
        ASSERT(bit  < c_PinsPerPort);
        ASSERT(port < c_MaxPorts);

        return bit + (port * c_PinsPerPort);
    }	

    static void STUB_GPIOISRVector( GPIO_PIN Pin, BOOL PinState, void* Param );
	
	static void GPIO_ControlReg_Write (unsigned short port, unsigned short data)
	{
		if (port == 0)
			PCCRL2 = data;
		else if (port == 1)
			PCCRL1 = data;
		else if (port == 2)
			PCCRH2 = data;
	}

	static unsigned short GPIO_ControlReg_Read (unsigned short port)
	{
		unsigned short data;
		if (port == 0)
			data = PCCRL2;
		else if (port == 1)
			data = PCCRL1;
		else if (port == 2)
			data = PCCRH2;
		return data;
	}	
		
	static void GPIO_IOReg_Write (unsigned short port, unsigned short data)
	{
		if (port == 0)
			PCIORL = data;
		else if (port == 1)
			PCIORH = data;
	}

	static unsigned short GPIO_IOReg_Read (unsigned short port)
	{
		unsigned short data;
		if (port == 0)
			data = PCIORL;
		else if (port == 1)
			data = PCIORH;
		return data;
	}
	
	static void GPIO_DataReg_Write (unsigned short port, unsigned short data)
	{
		if (port == 0)
			PCDRL = data;
		else if (port == 1)
			PCDRH = data;
	}

	static unsigned short GPIO_DataReg_Read (unsigned short port)
	{
		unsigned short data;
		if (port == 0)
			data = PCDRL;
		else if (port == 1)
			data = PCDRH;
		return data;
	}

};

#endif //_SH7619_GPIO_H_