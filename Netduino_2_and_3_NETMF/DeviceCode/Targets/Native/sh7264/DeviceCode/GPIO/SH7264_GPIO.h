/************************************************************/
/*  FileName    : SH7619_GPIO.h                         */
/*  FUNC        : GPIO Register Header File             */
/************************************************************/

/*
 *    Copyright (C) 2010 Renesas Electronics America Inc. All rights reserved.
 */

#ifndef _SH7264_GPIO_H_
#define _SH7264_GPIO_H_

#define VL(x) (*(volatile unsigned long *)(x))
#define VS(x) (*(volatile unsigned short *)(x))
#define VB(x) (*(volatile unsigned char *)(x))

#define IO_PHCR0    VS(0xFFFE38EE)
//#define SH7264_MAX_GPIO     129
#define SH7264_MAX_GPIO     32
struct SH7264_GPIO
{


    static const UINT32 c_Pin_None  = 0xFFFFFFFF;
    static const UINT32 c_sw_key_offset = 8;

#if 0
    // PA0-PA3 4 pins
    static const UINT32 PA00  = 0;
    static const UINT32 PA01  = 1;
    static const UINT32 PA02  = 2;
    static const UINT32 PA03  = 3;


    // PB1 - PB22 22 pins
    static const UINT32 PB01  = 1;
    static const UINT32 PB02  = 2;
    static const UINT32 PB03  = 3;
    static const UINT32 PB04  = 4;
    static const UINT32 PB05  = 5;
    static const UINT32 PB06  = 6;
    static const UINT32 PB07  = 7;
    static const UINT32 PB08  = 8;
    static const UINT32 PB09  = 9;
    static const UINT32 PB10  = 10;
    static const UINT32 PB11  = 11;
    static const UINT32 PB12  = 12;
    static const UINT32 PB13  = 13;
    static const UINT32 PB14  = 14;
    static const UINT32 PB15  = 15;
    static const UINT32 PB16  = 16;
    static const UINT32 PB17  = 17;
    static const UINT32 PB18  = 18;
    static const UINT32 PB19  = 19;
    static const UINT32 PB20  = 20;
    static const UINT32 PB21  = 21;
    static const UINT32 PB22  = 22;

    // PC0-PC10 11 pins
    static const UINT32 PC00  = 0;
    static const UINT32 PC01  = 1;
    static const UINT32 PC02  = 2;
    static const UINT32 PC03  = 3;
    static const UINT32 PC04  = 4;
    static const UINT32 PC05  = 5;
    static const UINT32 PC06  = 6;
    static const UINT32 PC07  = 7;
    static const UINT32 PC08  = 8;
    static const UINT32 PC09  = 9;
    static const UINT32 PC10  = 10;

    // PD0-PD15 16 pins
    static const UINT32 PD00  = 0;
    static const UINT32 PD01  = 1;
    static const UINT32 PD02  = 2;
    static const UINT32 PD03  = 3;
    static const UINT32 PD04  = 4;
    static const UINT32 PD05  = 5;
    static const UINT32 PD06  = 6;
    static const UINT32 PD07  = 7;
    static const UINT32 PD08  = 8;
    static const UINT32 PD09  = 9;
    static const UINT32 PD10  = 10;
    static const UINT32 PD11  = 11;
    static const UINT32 PD12  = 12;
    static const UINT32 PD13  = 13;
    static const UINT32 PD14  = 14;
    static const UINT32 PD15  = 15;

    // PE0 -PE5 6 pins
    static const UINT32 PE00  = 0;
    static const UINT32 PE01  = 1;
    static const UINT32 PE02  = 2;
    static const UINT32 PE03  = 3;
    static const UINT32 PE04  = 4;
    static const UINT32 PE05  = 5;

    // PF0-PF12 13 pins
    static const UINT32 PF00  = 0;
    static const UINT32 PF01  = 1;
    static const UINT32 PF02  = 2;
    static const UINT32 PF03  = 3;
    static const UINT32 PF04  = 4;
    static const UINT32 PF05  = 5;
    static const UINT32 PF06  = 6;
    static const UINT32 PF07  = 7;
    static const UINT32 PF08  = 8;
    static const UINT32 PF09  = 9;
    static const UINT32 PF10  = 10;
    static const UINT32 PF11  = 11;
    static const UINT32 PF12  = 12;

    //PG0-PG24 25 pins
    static const UINT32 PG00  = 0;
    static const UINT32 PG01  = 1;
    static const UINT32 PG02  = 2;
    static const UINT32 PG03  = 3;
    static const UINT32 PG04  = 4;
    static const UINT32 PG05  = 5;
    static const UINT32 PG06  = 6;
    static const UINT32 PG07  = 7;
    static const UINT32 PG08  = 8;
    static const UINT32 PG09  = 9;
    static const UINT32 PG10  = 10;
    static const UINT32 PG11  = 11;
    static const UINT32 PG12  = 12;
    static const UINT32 PG13  = 13;
    static const UINT32 PG14  = 14;
    static const UINT32 PG15  = 15;
    static const UINT32 PG16  = 16;
    static const UINT32 PG17  = 17;
    static const UINT32 PG18  = 18;
    static const UINT32 PG19  = 19;
    static const UINT32 PG20  = 20;
    static const UINT32 PG21  = 21;
    static const UINT32 PG22  = 22;
    static const UINT32 PG23  = 23;
    static const UINT32 PG24  = 24;

    // PH0 -PH7 8 pins
    static const UINT32 PH00  = 0;
    static const UINT32 PH01  = 1;
    static const UINT32 PH02  = 2;
    static const UINT32 PH03  = 3;
    static const UINT32 PH04  = 4;
    static const UINT32 PH05  = 5;
    static const UINT32 PH06  = 6;
    static const UINT32 PH07  = 7;

    // PJ0-PJ11 12 pins
    static const UINT32 PJ00  = 0;
    static const UINT32 PJ01  = 1;
    static const UINT32 PJ02  = 2;
    static const UINT32 PJ03  = 3;
    static const UINT32 PJ04  = 4;
    static const UINT32 PJ05  = 5;
    static const UINT32 PJ06  = 6;
    static const UINT32 PJ07  = 7;
    static const UINT32 PJ08  = 8;
    static const UINT32 PJ09  = 9;
    static const UINT32 PJ10  = 10;
    static const UINT32 PJ11  = 11;

    // PK0-PK11  12 pins
    static const UINT32 PK00  = 0;
    static const UINT32 PK01  = 1;
    static const UINT32 PK02  = 2;
    static const UINT32 PK03  = 3;
    static const UINT32 PK04  = 4;
    static const UINT32 PK05  = 5;
    static const UINT32 PK06  = 6;
    static const UINT32 PK07  = 7;
    static const UINT32 PK08  = 8;
    static const UINT32 PK09  = 9;
    static const UINT32 PK10  = 10;
    static const UINT32 PK11  = 11;



#else
    static const UINT32 PH00 =   0;
    static const UINT32 PH01 =   1;
    static const UINT32 PH02 =   2;
    static const UINT32 PH03 =   3;
    static const UINT32 PH04 =   4;
    static const UINT32 PH05 =   5;
    static const UINT32 PH06 =   6;
    static const UINT32 PH07 =   7;  
#endif    
    //Key Scan Button
    static const UINT32 c_SW_K1    =   c_sw_key_offset+0; 
    static const UINT32 c_SW_K2    =   c_sw_key_offset+1; 
    static const UINT32 c_SW_K3    =   c_sw_key_offset+2; 
    static const UINT32 c_SW_K4    =   c_sw_key_offset+3; 
    static const UINT32 c_SW_K5    =   c_sw_key_offset+4; 
    static const UINT32 c_SW_K6    =   c_sw_key_offset+5; 
    static const UINT32 c_SW_K7    =   c_sw_key_offset+6; 
    static const UINT32 c_SW_K8    =   c_sw_key_offset+7; 
    static const UINT32 c_SW_K9    =   c_sw_key_offset+8; 
    static const UINT32 c_SW_K10   =   c_sw_key_offset+9; 
    static const UINT32 c_SW_K11   =   c_sw_key_offset+10; 
    static const UINT32 c_SW_K12   =   c_sw_key_offset+11; 
    static const UINT32 c_SW_K13   =   c_sw_key_offset+12; 
    static const UINT32 c_SW_K14   =   c_sw_key_offset+13; 
    static const UINT32 c_SW_K15   =   c_sw_key_offset+14; 
    static const UINT32 c_SW_K16   =   c_sw_key_offset+15; 
    
};

struct SH7264_GPIO_Driver
{
    struct PIN_ISR_DESCRIPTOR
    {
        HAL_COMPLETION                          m_completion;
        GPIO_INTERRUPT_SERVICE_ROUTINE          m_isr;
        void*                                   m_param;
        GPIO_PIN                                m_pin;
        GPIO_INT_EDGE                           m_intEdge;
        UINT8                                   m_flags;
        UINT8                                   m_status;

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
    
//  enum GPIO_NAMES {
//        PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
//        PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15,
//        PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12, PC13, PC14, PC15,
//      PC16, PC17, PC18, PC19, PC20, PC21, PC22, PC23, PC24, PC25, PC26, PC27, PC28, PC29, PC30, PC31
//      PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12, PD13, PD14, PD15,
//      PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7, PE8, PE9, PE10, PE11, PE12, PE13, PE14, PE15,
//      PE16, PE17, PE18, PE19, PE20, PE21, PE22, PE23, PE24, PE25, PE26, PE27, PE28, PE29, PE30, PE31
//    };
    
    //--//
    
    static const UINT32 c_MaxPins           = SH7264_MAX_GPIO;
    static const UINT32 c_MaxPorts          = (SH7264_MAX_GPIO / 16);
    static const UINT32 c_PinsPerPort       =  4;

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
    
 private:

    static UINT32 PinToPort( GPIO_PIN pin )
    {
        ASSERT(pin < c_MaxPins);

        return pin / (c_PinsPerPort*4);
    }

    static UINT32 PinToBit( GPIO_PIN pin )
    {
        ASSERT(pin < c_MaxPins);

        //return pin % c_PinsPerPort;
        return pin * c_PinsPerPort;
    }
    
    static void STUB_GPIOISRVector( GPIO_PIN Pin, BOOL PinState, void* Param );     
        
    static void GPIO_Reg_Write (unsigned short port, unsigned short data)
    {       
        IO_PHCR0 = data;    
    }

    static unsigned short GPIO_Reg_Read (unsigned short port)
    {   
        return IO_PHCR0;
    }
};
void CPU_GPIO_ISR();

#endif //_SH7264_GPIO_H_
