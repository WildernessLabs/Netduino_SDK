/********************************************************************************/
/* FileName: SH7619_driver_GPIO.cpp                                            */
/* Function: SH7619 GPIO Functions                                              */
/********************************************************************************/

/*
 *    Copyright (C) 2009-2010 Renesas Electronics America Inc. All rights reserved.
 */
 
#include <tinyhal.h>
#include "SH7619_GPIO.h"
#include "..\KeyPad\Keypad.h"


const UINT8 SH7619_GPIO_Driver::c_Gpio_Attributes[SH7619_GPIO_Driver::c_MaxPins] =
{
//    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   0
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT    , //   PC00
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT    , //  1
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT    , //  2
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT    , //  3    
    GPIO_ATTRIBUTE_INPUT                            , //  4
    GPIO_ATTRIBUTE_INPUT                            , //  5
    GPIO_ATTRIBUTE_INPUT                            , //  6
    GPIO_ATTRIBUTE_INPUT                            , //  7
    GPIO_ATTRIBUTE_OUTPUT                           , //  8    
    GPIO_ATTRIBUTE_OUTPUT                           , //  9
    GPIO_ATTRIBUTE_OUTPUT                           , //  10
    GPIO_ATTRIBUTE_OUTPUT                           , //  11
    GPIO_ATTRIBUTE_INPUT                            , //  12
    GPIO_ATTRIBUTE_OUTPUT                           , //  13    
    GPIO_ATTRIBUTE_INPUT                            , //  14
    GPIO_ATTRIBUTE_INPUT                            , //  15
    GPIO_ATTRIBUTE_INPUT                            , //  16
    GPIO_ATTRIBUTE_INPUT                            , //  17
    GPIO_ATTRIBUTE_INPUT                            , //  18    
    GPIO_ATTRIBUTE_INPUT                            , //  19
    GPIO_ATTRIBUTE_INPUT                            , //  20
    GPIO_ATTRIBUTE_INPUT                            , //  21
    GPIO_ATTRIBUTE_INPUT                            , //  22
    GPIO_ATTRIBUTE_INPUT                            , //  23    
    GPIO_ATTRIBUTE_INPUT                            , //  24
    GPIO_ATTRIBUTE_INPUT                            , //  25
    GPIO_ATTRIBUTE_INPUT                            , //  26
    GPIO_ATTRIBUTE_INPUT                            , //  27
    GPIO_ATTRIBUTE_INPUT                            , //  28    
    GPIO_ATTRIBUTE_INPUT                            , //  29
    GPIO_ATTRIBUTE_INPUT                            , //  30
    GPIO_ATTRIBUTE_INPUT                            , //  31
    GPIO_ATTRIBUTE_INPUT                            , //  32
    GPIO_ATTRIBUTE_INPUT                            , //  33    
    GPIO_ATTRIBUTE_INPUT                            , //  34
    GPIO_ATTRIBUTE_INPUT                            , //  35
    GPIO_ATTRIBUTE_INPUT                            , //  36
    GPIO_ATTRIBUTE_INPUT                            , //  37
    GPIO_ATTRIBUTE_INPUT                            , //  38    
    GPIO_ATTRIBUTE_INPUT                            , //  39
    GPIO_ATTRIBUTE_INPUT                            , //  40
    GPIO_ATTRIBUTE_INPUT                            , //  41
    GPIO_ATTRIBUTE_INPUT                            , //  42
    GPIO_ATTRIBUTE_INPUT                            , //  43    
    GPIO_ATTRIBUTE_INPUT                            , //  44
    GPIO_ATTRIBUTE_INPUT                            , //  45
};

extern GPIO_KEYPAD_Driver g_GPIO_KEYPAD_Driver;

SH7619_GPIO_Driver g_SH7619_GPIO_Driver;

void SH7619_GPIO_Driver::PIN_ISR_DESCRIPTOR::Fire( void* arg )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    PIN_ISR_DESCRIPTOR* desc = (PIN_ISR_DESCRIPTOR*)arg;
        
    desc->m_isr( desc->m_pin, (desc->m_status & c_Status_AllowHighEdge) != 0, desc->m_param );

    // we need to clear (invert) the status only for actual dual edge pins
    UINT8 mask = c_Flags_RequireHighEdge | c_Flags_RequireLowEdge;
    
    if((desc->m_flags & mask) == mask)
    {
        desc->m_status ^= PIN_ISR_DESCRIPTOR::c_Status_AllowLowEdge | PIN_ISR_DESCRIPTOR::c_Status_AllowHighEdge;
    }
}

void SH7619_GPIO_Driver::STUB_GPIOISRVector( GPIO_PIN Pin, BOOL PinState, void* Param )
{
//    lcd_printf( "\fSTUB_GPI %04x\r\n", (size_t)Param );
     hal_printf( "\fSTUB_GPI %04x %04x\r\n", Pin, (size_t)Param );
//    DEBUG_HARD_BREAKPOINT();
}

BOOL SH7619_GPIO_Driver::Initialize()
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    UINT32 i;

    // initialize the interrupt information
    {
        PIN_ISR_DESCRIPTOR* pinIsr = g_SH7619_GPIO_Driver.m_PinIsr;

        for(i = 0; i < c_MaxPins; i++)
        {
            pinIsr->m_pin     = i;
            pinIsr->m_intEdge = GPIO_INT_NONE;
            pinIsr->m_isr     = STUB_GPIOISRVector;
            pinIsr->m_param   = NULL;

            pinIsr->m_completion.Initialize( );
            pinIsr->m_completion.InitializeForISR( &PIN_ISR_DESCRIPTOR::Fire, pinIsr );

            pinIsr++;
        }
    }

//    for(i = 0; i < c_MaxPins; i++)
    for(i = 0; i < c_MaxPorts; i++)
    {
        // initialize pins as free
        g_SH7619_GPIO_Driver.m_PinReservationInfo[i] = 0;
    }   
    
    g_GPIO_KEYPAD_Driver.KeyPad_Initialize();
    
    return TRUE;
}

BOOL SH7619_GPIO_Driver::Uninitialize()
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    GLOBAL_LOCK(irq);

    // uninitialize the interrupt information
    {
        PIN_ISR_DESCRIPTOR* pinIsr = g_SH7619_GPIO_Driver.m_PinIsr;
        for(int i = 0; i < c_MaxPins; i++)
        {
            pinIsr->m_completion.Abort();
            pinIsr++;
        }
    }

    return TRUE;
}

UINT32 SH7619_GPIO_Driver::Attributes( GPIO_PIN Pin )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    ASSERT(pin < c_MaxPins);
    
    if(Pin < c_MaxPins)
    {
        return c_Gpio_Attributes[Pin];
    }

    return GPIO_ATTRIBUTE_NONE;
}

void SH7619_GPIO_Driver::DisablePin( GPIO_PIN Pin, GPIO_RESISTOR ResistorState, UINT32 direction, GPIO_ALT_MODE AltFunction )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    UINT32   bitMask  = 1 << PinToBit( Pin );
    UINT32 ioPort = PinToIOPort(Pin);
    UINT32 ioBit = PinToIOBit(Pin);     
    
    GLOBAL_LOCK(irq);
    
    // Set pin to specified alternate function
    GPIO_IOReg_Write(ioPort, (GPIO_IOReg_Read(ioPort) | (0x1 << ioBit)));
    
    if(direction)
        GPIO_IOReg_Write(ioPort, (GPIO_IOReg_Read(ioPort) | (0x1 << ioBit))); // Set pin as output
    else
        GPIO_IOReg_Write(ioPort, (GPIO_IOReg_Read(ioPort) & ~(0x1 << ioBit))); // Set pin as input

    PIN_ISR_DESCRIPTOR& pinIsr = g_SH7619_GPIO_Driver.m_PinIsr[Pin];

    pinIsr.m_intEdge = GPIO_INT_NONE;
    pinIsr.m_isr     = STUB_GPIOISRVector;
    pinIsr.m_param   = NULL;
}

void SH7619_GPIO_Driver::EnableOutputPin( GPIO_PIN Pin, BOOL InitialState )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    ASSERT(pin < c_MaxPins);

    unsigned short Port, Bit, ioPort, ioBit;    
    
    GLOBAL_LOCK(irq);

    Port = PinToPort(Pin);
    Bit = PinToBit(Pin);
    ioPort = PinToIOPort(Pin);
    ioBit = PinToIOBit(Pin);
    
    GPIO_ControlReg_Write(Port, (GPIO_ControlReg_Read(Port) & ~(0x1 << Bit)));
    GPIO_IOReg_Write(ioPort, (GPIO_IOReg_Read(ioPort) | (0x1 << ioBit)));
}

BOOL SH7619_GPIO_Driver::EnableInputPin( GPIO_PIN pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE ISR, void* pinIsrParam, GPIO_INT_EDGE intEdge, GPIO_RESISTOR resistorState )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    ASSERT(pin < c_MaxPins);

    unsigned short Port, Bit, ioPort, ioBit;    
    
    GLOBAL_LOCK(irq);

    if(pin < 21)        //Is it a port?
    {
        Port = PinToPort(pin);
         Bit = PinToBit(pin);
        ioPort = PinToIOPort(pin);
        ioBit = PinToIOBit(pin);
    
        GPIO_ControlReg_Write(Port, (GPIO_ControlReg_Read(Port) & ~(0x1 << Bit)));
        GPIO_IOReg_Write(ioPort, (GPIO_IOReg_Read(ioPort) & ~(0x1 << ioBit)));
    }
    else if(pin < 46){   //Is it a key scan button?
        g_GPIO_KEYPAD_Driver.Keyscan_Active = 1;

        PIN_ISR_DESCRIPTOR& pinIsr = g_SH7619_GPIO_Driver.m_PinIsr[ pin ];

        pinIsr.m_intEdge = intEdge;
        pinIsr.m_isr     = (ISR != NULL) ? ISR : STUB_GPIOISRVector;
        pinIsr.m_param   = pinIsrParam;
        pinIsr.m_flags   = GlitchFilterEnable ? PIN_ISR_DESCRIPTOR::c_Flags_Debounce : 0;
        pinIsr.m_status  = 0;                        
        pinIsr.m_completion.Abort();
        pinIsr.m_completion.Initialize();

        if(ISR)
        {
            // since there is no HW debounce, we always have to listen to both edges and 
            // filter the interrupt based on the desired edge and the debounce time 
            switch(intEdge)
            {
                case GPIO_INT_NONE:
                    return FALSE;
            
                case GPIO_INT_EDGE_LOW :
                case GPIO_INT_LEVEL_LOW:
                    pinIsr.m_status |= PIN_ISR_DESCRIPTOR::c_Status_AllowLowEdge;
                    break;

                case GPIO_INT_EDGE_HIGH:
                case GPIO_INT_LEVEL_HIGH:
                    pinIsr.m_status |= PIN_ISR_DESCRIPTOR::c_Status_AllowHighEdge;
                    break;

                case GPIO_INT_EDGE_BOTH:
                    pinIsr.m_flags |= PIN_ISR_DESCRIPTOR::c_Flags_RequireHighEdge | PIN_ISR_DESCRIPTOR::c_Flags_RequireLowEdge; 
                    break;

                default:
                    ASSERT(0);
                    return FALSE;
            }
        }
        else return FALSE;
    }
    
    return TRUE;
}

BOOL SH7619_GPIO_Driver::GetPinState( GPIO_PIN Pin )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();    
    ASSERT(pin < c_MaxPins);
    
    UINT32 ioPort, ioBit;
   
    GLOBAL_LOCK(irq);

    ioPort = PinToIOPort(Pin);
    ioBit = PinToIOBit(Pin);

//    return (((GPIO_IOReg_Read(ioPort)) >> ioBit) & 0x1);    
    return (GPIO_DataReg_Read(ioPort) & (0x1 << ioBit));
}

void SH7619_GPIO_Driver::SetPinState( GPIO_PIN Pin, BOOL PinState )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    ASSERT(Pin < c_MaxPins);
    
    UINT32 ioPort, ioBit;
    
    GLOBAL_LOCK(irq);
    
    ioPort = PinToIOPort(Pin);
    ioBit = PinToIOBit(Pin);

    if(PinState)
//        GPIO_IOReg_Write(ioPort, (GPIO_IOReg_Read(ioPort) | (0x1 << ioBit)));
        GPIO_DataReg_Write(ioPort, (GPIO_DataReg_Read(ioPort) | (0x1 << ioBit)));
    else
//        GPIO_DataReg_Write(ioPort, (GPIO_DataReg_Read(ioPort) | (0x1 << ioBit)));       
        GPIO_DataReg_Write(ioPort, (GPIO_DataReg_Read(ioPort) & ~(0x1 << ioBit)));
}

void SH7619_GPIO_Driver::ISR()
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    GPIO_PIN pin = g_GPIO_KEYPAD_Driver.Keyscan_result;
    
    if (pin != -1){
        pin = pin + 20;   //Need to figure out c_Key_n number.
        PIN_ISR_DESCRIPTOR& pinIsr = g_SH7619_GPIO_Driver.m_PinIsr[ pin ];

        if ( g_GPIO_KEYPAD_Driver.Edge_flag == 0 ) pinIsr.m_status = PIN_ISR_DESCRIPTOR::c_Status_AllowLowEdge;
        else if ( g_GPIO_KEYPAD_Driver.Edge_flag == 1 ) pinIsr.m_status = PIN_ISR_DESCRIPTOR::c_Status_AllowHighEdge;
        
        if (pinIsr.m_intEdge != GPIO_INT_NONE) pinIsr.Fire((void*)&pinIsr);
    }
    
    return;
}

void SH7619_GPIO_Driver::SetResistor( GPIO_PIN Pin, GPIO_RESISTOR ResistorState )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();    
    return; 
}

BOOL SH7619_GPIO_Driver::PinIsBusy( GPIO_PIN Pin )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    UINT32  port     = Pin / c_PinsPerPort;
    UINT32  bitMask  = 1 << PinToBit( Pin );
    UINT32& res      = g_SH7619_GPIO_Driver.m_PinReservationInfo[port];

    return (res & bitMask) != 0 ? TRUE : FALSE;
}

BOOL SH7619_GPIO_Driver::ReservePin( GPIO_PIN Pin, BOOL fReserve )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    GLOBAL_LOCK(irq);

    UINT32  port = Pin / c_PinsPerPort;
    UINT32  bit  = PinToBit( Pin );
    UINT32& res  = g_SH7619_GPIO_Driver.m_PinReservationInfo[port];
    UINT32  mask = (fReserve ? 1 : 0) << bit;

    if((res & (1 << bit)) == mask) 
        return FALSE;

    if(fReserve)
    {
        res |= (1 << bit);
    }
    else
    {
        res &= ~(1 << bit);
    }

    return TRUE;
}

UINT32 SH7619_GPIO_Driver::GetDebounce()
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    return CPU_TicksToTime( g_SH7619_GPIO_Driver.m_DebounceTicks ) / 10000;
}

BOOL SH7619_GPIO_Driver::SetDebounce( INT64 debounceTimeMilliseconds )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    if(debounceTimeMilliseconds < c_MinDebounceTimeMs || c_MaxDebounceTimeMs < debounceTimeMilliseconds )
    {
        return FALSE;
    }

    g_SH7619_GPIO_Driver.m_DebounceTicks = CPU_MillisecondsToTicks( (UINT32)debounceTimeMilliseconds );
    
    return TRUE;
}

void SH7619_GPIO_Driver::GetPinsMap( UINT8* pins, size_t size )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();

    const UINT8*    src = c_Gpio_Attributes;
    UINT8* dst = pins;   
    UINT32 maxpin = c_MaxPins;

    if ( size ==0 ) return;
    while ((size--) && (maxpin --))
    {
        *dst = *src;  
        ++dst; ++src;
    }   
}

/*
void SH7619_GPIO_Driver::SetPinData( GPIO_PIN Pin, BOOL Value )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    ASSERT(pin < c_MaxPins);

    unsigned short ioPort, ioBit;    
    
    GLOBAL_LOCK(irq);

    ioPort = PinToIOPort(Pin);
    ioBit = PinToIOBit(Pin);
    
    if (Value == 1)  // Pin High
    {       
        GPIO_DataReg_Write(ioPort, (GPIO_DataReg_Read(ioPort) | (0x1 << ioBit)));
    }
    else            // Pin Low
    {
        GPIO_DataReg_Write(ioPort, (GPIO_DataReg_Read(ioPort) & ~(0x1 << ioBit)));
    }
}

unsigned short SH7619_GPIO_Driver::GetPinData( GPIO_PIN Pin)
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    ASSERT(pin < c_MaxPins);

    unsigned short ioPort, ioBit;   
    
    GLOBAL_LOCK(irq);

    ioPort = PinToIOPort(Pin);
    ioBit = PinToIOBit(Pin);    
            
    return (GPIO_DataReg_Read(ioPort) & (0x1 << ioBit));
}
*/
