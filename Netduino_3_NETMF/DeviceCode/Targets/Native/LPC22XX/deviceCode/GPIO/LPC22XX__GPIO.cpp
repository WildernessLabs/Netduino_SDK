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
#include "..\LPC22XX.h"

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata = "c_Gpio_Attributes_LPC22XX"
#endif

const UINT8 __section(rodata) LPC22XX_GPIO_Driver::c_Gpio_Attributes[LPC22XX_GPIO_Driver::c_MaxPins] =
{
    GPIO_ATTRIBUTE_NONE,                          // 00 Port 0 UART0 - TXD0
    GPIO_ATTRIBUTE_NONE,                          // 01        UART0 - RXD0
    GPIO_ATTRIBUTE_NONE,                          // 02
    GPIO_ATTRIBUTE_NONE,                          // 03
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 04
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 05
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 06
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 07
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 08
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 09
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 10
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 11
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 12
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 13
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 14
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 15
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 16
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 17
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 18
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 19
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 20
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 21
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 22
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 23
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 24
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 25
    GPIO_ATTRIBUTE_NONE,                          // 26 Reserved
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 27
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 28
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 29
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 30
};

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_LPC22XX_GPIO_Driver"
#endif

LPC22XX_GPIO_Driver g_LPC22XX_GPIO_Driver;
LPC22XX_GPIO_Driver g_LPC22XX_PINSEL_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

BOOL CPU_GPIO_Initialize()
{
    return LPC22XX_GPIO_Driver::Initialize();
}

BOOL CPU_GPIO_Uninitialize()
{
    return LPC22XX_GPIO_Driver::Uninitialize();
}

UINT32 CPU_GPIO_Attributes( GPIO_PIN Pin )
{
    return LPC22XX_GPIO_Driver::Attributes( Pin );
}

void CPU_GPIO_DisablePin( GPIO_PIN Pin, GPIO_RESISTOR ResistorState, UINT32 Direction, GPIO_ALT_MODE AltFunction )
{
    LPC22XX_GPIO_Driver::DisablePin( Pin, ResistorState, Direction, AltFunction);
}

void CPU_GPIO_EnableOutputPin( GPIO_PIN Pin, BOOL InitialState )
{
    LPC22XX_GPIO_Driver::EnableOutputPin( Pin, InitialState );
}

BOOL CPU_GPIO_EnableInputPin( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE ISR, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
    return LPC22XX_GPIO_Driver::EnableInputPin( Pin, GlitchFilterEnable, ISR, 0, IntEdge, ResistorState );
}

BOOL CPU_GPIO_EnableInputPin2( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE ISR, void* ISR_Param, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
    return LPC22XX_GPIO_Driver::EnableInputPin( Pin, GlitchFilterEnable, ISR, ISR_Param, IntEdge, ResistorState );
}

BOOL CPU_GPIO_GetPinState( GPIO_PIN Pin )
{
    return LPC22XX_GPIO_Driver::GetPinState( Pin );
}

void CPU_GPIO_SetPinState( GPIO_PIN Pin, BOOL PinState )
{
    LPC22XX_GPIO_Driver::SetPinState( Pin, PinState );
}

INT32 CPU_GPIO_GetPinCount()
{
    return LPC22XX_GPIO_Driver::GetPinCount();
}

void CPU_GPIO_GetPinsMap( UINT8* pins, size_t size )
{
    LPC22XX_GPIO_Driver::GetPinsMap( pins, size );
}

UINT8 CPU_GPIO_GetSupportedResistorModes(GPIO_PIN pin)
{
    return LPC22XX_GPIO_Driver::GetSupportedResistorModes( pin);
}

UINT8 CPU_GPIO_GetSupportedInterruptModes(GPIO_PIN pin)
{
    return LPC22XX_GPIO_Driver::GetSupportedInterruptModes( pin );
}

BOOL CPU_GPIO_PinIsBusy( GPIO_PIN Pin )
{
    return LPC22XX_GPIO_Driver::PinIsBusy( Pin );
}

BOOL CPU_GPIO_ReservePin( GPIO_PIN Pin, BOOL fReserve )
{
    return LPC22XX_GPIO_Driver::ReservePin( Pin, fReserve );
}

UINT32 CPU_GPIO_GetDebounce()
{
    return LPC22XX_GPIO_Driver::GetDebounce();
}

BOOL CPU_GPIO_SetDebounce( INT64 debounceTimeMilliseconds )
{
    return LPC22XX_GPIO_Driver::SetDebounce( debounceTimeMilliseconds );
}

//--//

///////////////////////////////////////////////////////////////////////////////
//  Initialize
///////////////////////////////////////////////////////////////////////////////
BOOL LPC22XX_GPIO_Driver::Initialize()
{
    int i;
    UINT32 port;
    LPC22XX_GPIO& GPIO = LPC22XX::GPIO();

    for (port=0; port < c_MaxPins; port++)
    {
        g_LPC22XX_GPIO_Driver.m_PinReservationInfo[port] = FALSE;
    }

    // initialize the interrupt information
    {
        PIN_ISR_DESCRIPTOR* pinIsr = g_LPC22XX_GPIO_Driver.m_PinIsr;

        for(i = 0; i < c_MaxPins; i++)
        {
            pinIsr->m_pin     = i;
            pinIsr->m_intEdge = GPIO_INT_NONE;
            pinIsr->m_isr     = LPC22XX_GPIO_Driver::STUB_ISRVector;
            pinIsr->m_param   = NULL;
            pinIsr++;
        }
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//  Uninitialize
///////////////////////////////////////////////////////////////////////////////

BOOL LPC22XX_GPIO_Driver::Uninitialize()
{
    GLOBAL_LOCK(irq);

// TODO Implementation
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//  Attributes
///////////////////////////////////////////////////////////////////////////////

UINT32 LPC22XX_GPIO_Driver::Attributes( GPIO_PIN pin )
{
    ASSERT(pin < c_MaxPins);

    if(pin < c_MaxPins)
    {
        return c_Gpio_Attributes[pin];
    }

    return GPIO_ATTRIBUTE_NONE;
}
 
///////////////////////////////////////////////////////////////////////////////
//  DisablePin
///////////////////////////////////////////////////////////////////////////////

void LPC22XX_GPIO_Driver::DisablePin( GPIO_PIN pin, GPIO_RESISTOR resistorState, UINT32 Direction, GPIO_ALT_MODE AltFunction )
{
    UINT32 PCBReg, PCBRegBit, temp;

    LPC22XX_PCB& PCB = LPC22XX::PCB();

    PCBReg = PinToPCBReg(pin);
    PCBRegBit = PinToPCBRegBit(pin);

    // Configure the pin to its first alternate function.
    GLOBAL_LOCK(irq);

    temp = (PCB.Regs[PCBReg].PINSEL) & ~( 0x3 << PCBRegBit );
    PCB.Regs[PCBReg].PINSEL = temp | ( AltFunction << PCBRegBit );
    
    // Disable ISR
    PIN_ISR_DESCRIPTOR& PinIsr = g_LPC22XX_GPIO_Driver.m_PinIsr[pin];

    PinIsr.m_intEdge = GPIO_INT_NONE;
    PinIsr.m_isr     = STUB_ISRVector;
    PinIsr.m_param   = (void*)(size_t)pin;
}

///////////////////////////////////////////////////////////////////////////////
//  EnableOutputPin
///////////////////////////////////////////////////////////////////////////////

void LPC22XX_GPIO_Driver::EnableOutputPin( GPIO_PIN pin, BOOL initialState )
{
    ASSERT(pin < c_MaxPins);

    UINT32 Port, Bit;
    
    LPC22XX_GPIO& GPIO = LPC22XX::GPIO();

    GLOBAL_LOCK(irq);

//    hal_printf( "EnableOutputPin: %08x\r\n", pin);
    Port = PinToPort(pin);
    Bit = PinToBit(pin);
    
// Set initial state before changing direction of the port pin
    if(initialState) GPIO.Regs[Port].IOSET_PX |=  (0x1 << Bit);
    else             GPIO.Regs[Port].IOCLR_PX |=  (0x1 << Bit);

    GPIO.Regs[Port].IODIR_PX |= 0x1 << Bit;

    // TODO: Disable ISR
}

///////////////////////////////////////////////////////////////////////////////
//  EnableInputPin
///////////////////////////////////////////////////////////////////////////////
BOOL LPC22XX_GPIO_Driver::EnableInputPin( GPIO_PIN pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE ISR, void* Param, GPIO_INT_EDGE intEdge, GPIO_RESISTOR resistorState )
{
    ASSERT(pin < c_MaxPins);

    UINT32 Port, Bit;
    
    LPC22XX_GPIO& GPIO = LPC22XX::GPIO();

    GLOBAL_LOCK(irq);

//    hal_printf( "EnableInputPin: %08x\r\n", pin);
    Port = PinToPort(pin);
    Bit = PinToBit(pin);

    GPIO.Regs[Port].IODIR_PX &= ~(0x1 << Bit);
    
    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
//  SetnIRQISR
///////////////////////////////////////////////////////////////////////////////
BOOL LPC22XX_GPIO_Driver::SetnIRQISR( GPIO_PIN pin, HAL_CALLBACK_FPN ISR, GPIO_INT_EDGE intEdge, GPIO_RESISTOR resistorState )
{

    UINT32 Port, Bit;
    
    LPC22XX_GPIO& GPIO = LPC22XX::GPIO();

    Port = PinToPort(pin);
    Bit = PinToBit(pin);

    // current pin state, INT_EDGE_BOTH may override it (this is crucial for caller to get correct state before edge interrupts occur)
    // we already have input mode set, so it should be reading real world now (sans glitch filter delay if enabled)
    BOOL PinState = GetPinState(pin);

    UINT32 Val;
    switch(intEdge)
    {
        case GPIO_INT_EDGE_LOW:
            Val = 2;
            break;
        case GPIO_INT_EDGE_HIGH:
            Val = 3;
            break;
        case GPIO_INT_LEVEL_LOW:
            Val = 0;
            break;
        case GPIO_INT_LEVEL_HIGH:
            Val = 1;
            break;
        case GPIO_INT_EDGE_BOTH:
            // Start with high or low edge according to the current pin state
            // Interrupt handler will toggle it
            Val = (PinState == TRUE)? 2 : 3; // 2 = GPIO_INT_EDGE_LOW, 3 = GPIO_INT_EDGE_HIGH
            break;
        default:
            break;
    }    
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//  GetPinBynIRQte
///////////////////////////////////////////////////////////////////////////////
UINT32 LPC22XX_GPIO_Driver::GetPinBynIRQ( UINT32 nIRQ )
{
    return(nIRQ);
}   
    
UINT32 LPC22XX_GPIO_Driver::GetnIRQByPin( UINT32 Pin )
{
    return(Pin);
}

///////////////////////////////////////////////////////////////////////////////
//  GetPinState
///////////////////////////////////////////////////////////////////////////////

BOOL LPC22XX_GPIO_Driver::GetPinState( GPIO_PIN pin )
{
    ASSERT(pin < c_MaxPins);
    
    UINT32 Port, Bit;

//    hal_printf( "GetPinState: %08x\r\n", pin);
    LPC22XX_GPIO& GPIO = LPC22XX::GPIO();

    Port = PinToPort(pin);
    Bit = PinToBit(pin);

    return(((GPIO.Regs[Port].IOPIN_PX) >> Bit) & 0x1);
}


///////////////////////////////////////////////////////////////////////////////
//  SetPinState
///////////////////////////////////////////////////////////////////////////////
void LPC22XX_GPIO_Driver::SetPinState( GPIO_PIN pin, BOOL pinState )
{

    ASSERT(pin < c_MaxPins);

    UINT32 Port, Bit;

    LPC22XX_GPIO& GPIO = LPC22XX::GPIO();
//hal_printf( "SetPinState: %08x\r\n", pin);
    GLOBAL_LOCK(irq);

    Port = PinToPort(pin);
    Bit = PinToBit(pin);

    if(pinState) GPIO.Regs[Port].IOSET_PX |=  (0x1 << Bit);
    else         GPIO.Regs[Port].IOCLR_PX |=  (0x1 << Bit);
}


///////////////////////////////////////////////////////////////////////////////
//  PinIsBusy
///////////////////////////////////////////////////////////////////////////////
BOOL LPC22XX_GPIO_Driver::PinIsBusy( GPIO_PIN pin )
{
    return g_LPC22XX_GPIO_Driver.m_PinReservationInfo[pin];
}

///////////////////////////////////////////////////////////////////////////////
//  ReservePin
///////////////////////////////////////////////////////////////////////////////
BOOL LPC22XX_GPIO_Driver::ReservePin( GPIO_PIN pin, BOOL fReserve )
{
    GLOBAL_LOCK(irq);

    g_LPC22XX_GPIO_Driver.m_PinReservationInfo[pin] = fReserve;

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//  GetDebounce
///////////////////////////////////////////////////////////////////////////////
UINT32 LPC22XX_GPIO_Driver::GetDebounce()
{
// TODO: Need to implement
//hal_printf( "GetDebounce\r\n");

    return CPU_TicksToTime( g_LPC22XX_GPIO_Driver.m_DebounceTicks ) / 10000;
}

///////////////////////////////////////////////////////////////////////////////
//  SetDebounce
///////////////////////////////////////////////////////////////////////////////
BOOL LPC22XX_GPIO_Driver::SetDebounce( INT64 debounceTimeMilliseconds )
{   
// TODO: Need to implement
//hal_printf( "SetDebounce\r\n");

    if(debounceTimeMilliseconds < c_MinDebounceTimeMs || c_MaxDebounceTimeMs < debounceTimeMilliseconds )
    {
        return FALSE;
    }

    g_LPC22XX_GPIO_Driver.m_DebounceTicks = CPU_MillisecondsToTicks( (UINT32)debounceTimeMilliseconds );

    return TRUE;
}

INT32 LPC22XX_GPIO_Driver::GetPinCount()
{
    return c_MaxPins;
}

void LPC22XX_GPIO_Driver::GetPinsMap( UINT8* pins, size_t size )
{
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

UINT8 LPC22XX_GPIO_Driver::GetSupportedResistorModes( GPIO_PIN pin)
{
    return LPC22XX_GPIO_Driver::c_GPIO_ResistorMode;
}


UINT8 LPC22XX_GPIO_Driver::GetSupportedInterruptModes( GPIO_PIN pin)
{

    return LPC22XX_GPIO_Driver::c_GPIO_InterruptMode;
}

    
void LPC22XX_GPIO_Driver::PortConfiguration( UINT32 Port, UINT32 ConfigValue )
{
    LPC22XX_GPIO& GPIO = LPC22XX::GPIO();

}

void LPC22XX_GPIO_Driver::PortDirection( UINT32 Port, UINT32 DirValue )
{
    LPC22XX_GPIO& GPIO = LPC22XX::GPIO();

}

void LPC22XX_GPIO_Driver::nIRQ_ISR( void* Param )
{
}

void LPC22XX_GPIO_Driver::STUB_ISRVector( GPIO_PIN Pin, BOOL PinState, void* Param )
{
    lcd_printf( "\fSTUB_GPI %04x\r\n", (size_t)Param );
    //HARD_BREAKPOINT();
}
