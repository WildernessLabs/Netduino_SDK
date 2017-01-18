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

BOOL PWM_Initialize(PWM_CHANNEL channel)
{
    return LPC24XX_PWM_Driver::Initialize(channel);
}

BOOL PWM_Uninitialize(PWM_CHANNEL channel)
{
    return LPC24XX_PWM_Driver::Uninitialize(channel);
}

BOOL PWM_ApplyConfiguration(PWM_CHANNEL channel, GPIO_PIN pin, UINT32& period, UINT32& duration, PWM_SCALE_FACTOR& scale, BOOL invert)
{
    return LPC24XX_PWM_Driver::ApplyConfiguration(channel, pin, period, duration, scale, invert);
}

BOOL PWM_Start(PWM_CHANNEL channel, GPIO_PIN pin)
{
    return LPC24XX_PWM_Driver::Start(channel,pin);
}

void PWM_Stop(PWM_CHANNEL channel, GPIO_PIN pin)
{
    return LPC24XX_PWM_Driver::Stop(channel,pin);
}

BOOL PWM_Start(PWM_CHANNEL* channel, GPIO_PIN* pin, UINT32 count)
{
    return LPC24XX_PWM_Driver::Start(channel, pin, count);
}

void PWM_Stop(PWM_CHANNEL* channel, GPIO_PIN* pin, UINT32 count)
{
    return LPC24XX_PWM_Driver::Stop(channel, pin, count);
}

UINT32 PWM_PWMChannels() 
{
    return LPC24XX_PWM_Driver::Channels();
}

GPIO_PIN PWM_GetPinForChannel( PWM_CHANNEL channel )
{
    return LPC24XX_PWM_Driver::GetPinForChannel( channel );
}

//--//

BOOL LPC24XX_PWM_Driver::Initialize(PWM_CHANNEL channel)
{
    LPC24XX_PWM& pwm = LPC24XX::PWM( channel );
    LPC24XX_SYSCON& scs = LPC24XX::SYSCON();
        
    BOOL isChannel0 = channel == PWM_CHANNEL_0;
    
    // make sure power is on and select a clock
    if(isChannel0) scs.PCONP |= LPC24XX_SYSCON::ENABLE_PWM0;
    else scs.PCONP |= LPC24XX_SYSCON::ENABLE_PWM1;
    //scs.PCONP    |= isChannel0 ? LPC24XX_SYSCON::ENABLE_PWM0 : LPC24XX_SYSCON::ENABLE_PWM1;
    
    if(isChannel0) scs.PCLKSEL0 |= LPC24XX_SYSCON::PCLK_PWM0_CLK_DIV_1;
    else scs.PCLKSEL0 |= LPC24XX_SYSCON::PCLK_PWM1_CLK_DIV_1;       
    //scs.PCLKSEL0 |= isChannel0 ? LPC24XX_SYSCON::PCLK_PWM0_CLK_DIV_1 : LPC24XX_SYSCON::PCLK_PWM1_CLK_DIV_1;       

    // enable PWM mode on this controller 
    pwm.TCR |= LPC24XX_PWM::TCR_PWM_ENABLE;    

    return TRUE;
}

BOOL LPC24XX_PWM_Driver::Uninitialize(PWM_CHANNEL channel)
{
    LPC24XX_PWM& pwm = LPC24XX::PWM( channel );
    LPC24XX_SYSCON& scs = LPC24XX::SYSCON();
    
    BOOL isChannel0 = channel == PWM_CHANNEL_0;
    
    // disable PWM mode and counter on this controller 
    pwm.TCR &= ~(LPC24XX_PWM::TCR_PWM_ENABLE);    
    
    // make sure power is off 
    if(isChannel0) scs.PCONP &= ~ LPC24XX_SYSCON::ENABLE_PWM0;
    else scs.PCONP &= ~LPC24XX_SYSCON::ENABLE_PWM1;

    //scs.PCONP &= ~( isChannel0 ? LPC24XX_SYSCON::ENABLE_PWM0 : LPC24XX_SYSCON::ENABLE_PWM1);

    return TRUE;
}

#define ONE_HZ  1000
#define ONE_GHZ 1000000000ul

#define TIME_TO_PWM_TICKS(x,s) \
    (s) <= CLOCK_COMMON_FACTOR ? (x) * (SYSTEM_CYCLE_CLOCK_HZ / (s)) : \
    (((x) * (SYSTEM_CYCLE_CLOCK_HZ / CLOCK_COMMON_FACTOR  )) / \
    (       (s)                    / CLOCK_COMMON_FACTOR))

BOOL LPC24XX_PWM_Driver::ApplyConfiguration(PWM_CHANNEL channel, GPIO_PIN pin,UINT32 & period,UINT32 & duration, PWM_SCALE_FACTOR & scale, BOOL invert)
{
    LPC24XX_PWM& pwm = LPC24XX::PWM( channel );

    UINT32 period_ticks   = (UINT32)TIME_TO_PWM_TICKS(period  , scale);  
    UINT32 duration_ticks = (UINT32)TIME_TO_PWM_TICKS(duration, scale);

    BOOL isChannel0 = channel == PWM_CHANNEL_0;
    
    // set the MCR register
    pwm.MR0 = period_ticks;
    
    if(isChannel0)
    {
        pwm.MR2 = duration_ticks;
    }
    else
    {
        pwm.MR1 = duration_ticks;
    }
    
    // enable latching, in case this channel was started already
    if(isChannel0) pwm.LER |= LPC24XX_PWM::LER_ENABLE_0 | LPC24XX_PWM::LER_ENABLE_2;
    else pwm.LER |= LPC24XX_PWM::LER_ENABLE_0 | LPC24XX_PWM::LER_ENABLE_1;
    //pwm.LER |=  (LPC24XX_PWM::LER_ENABLE_0 | (isChannel0 ? LPC24XX_PWM::LER_ENABLE_2 : LPC24XX_PWM::LER_ENABLE_1));
    
    return TRUE;
}

void LPC24XX_PWM_Driver::EnablePin( PWM_CHANNEL channel, GPIO_PIN pin )
{
    LPC24XX_PCB& pcb    = LPC24XX::PCB();
    LPC24XX_PWM& pwm    = LPC24XX::PWM( channel );
    
    // as we allow only a precision of 32 bits and the match register is 32 bits then we need not use neither the prescaler nor the divider     
    pwm.TC  = 0;
    pwm.PR  = 0;
    pwm.PC  = 0;    
    pwm.MCR |= LPC24XX_PWM::MCR_MR0_RESET_TC;
    
    if(channel == PWM_CHANNEL_0)
    {    
        pwm.PCR |= LPC24XX_PWM::PCR_ENABLE_2;
        CPU_GPIO_DisablePin( pin, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_3 );
    }
    else
    { 
        pwm.PCR |= LPC24XX_PWM::PCR_ENABLE_1;
        CPU_GPIO_DisablePin( pin, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_2 );
    }
    
    // start counter
    pwm.TCR |= LPC24XX_PWM::TCR_COUNTER_ENABLE;
}

void LPC24XX_PWM_Driver::DisablePin( PWM_CHANNEL channel, GPIO_PIN pin )
{
    LPC24XX_PCB& pcb    = LPC24XX::PCB();
    LPC24XX_PWM& pwm    = LPC24XX::PWM( channel );

    if(channel == PWM_CHANNEL_0) 
    {    
        pwm.PCR &= ~LPC24XX_PWM::PCR_ENABLE_2;
    }
    else
    { 
        pwm.PCR &= ~LPC24XX_PWM::PCR_ENABLE_1;
    }

    // stop counters
    pwm.TCR &= ~(LPC24XX_PWM::TCR_COUNTER_ENABLE);

    // reset registers       
    pwm.TC  = 0;
    pwm.PR  = 0;
    pwm.PC  = 0;
    pwm.MCR &= ~(LPC24XX_PWM::MCR_MR0_RESET_TC);   

    // enable pin for primary function
    CPU_GPIO_DisablePin( pin, RESISTOR_PULLDOWN, 0, GPIO_ALT_PRIMARY );
}

BOOL LPC24XX_PWM_Driver::Start(PWM_CHANNEL channel, GPIO_PIN pin)
{
    // we can enable the pin, as the match register is supposed to be set already by a previous call to ApplyConfiguration
    LPC24XX_PWM_Driver::EnablePin(channel, pin);

    return TRUE;
}

void LPC24XX_PWM_Driver::Stop(PWM_CHANNEL channel, GPIO_PIN pin)
{    
    LPC24XX_PWM_Driver::DisablePin(channel, pin);
}

BOOL LPC24XX_PWM_Driver::Start(PWM_CHANNEL* channel,GPIO_PIN* pin,UINT32 count)
{
    return TRUE;
}

void LPC24XX_PWM_Driver::Stop(PWM_CHANNEL* channel,GPIO_PIN* pin,UINT32 count)
{
}

UINT32 LPC24XX_PWM_Driver::Channels()
{
    return c_Channels;
}

GPIO_PIN LPC24XX_PWM_Driver::GetPinForChannel ( PWM_CHANNEL channel )
{
    if(channel == PWM_CHANNEL_0)
    {
        return c_Channel_0_Pin2;
    }
    else if(channel == PWM_CHANNEL_1)
    {
        return c_Channel_1_Pin1;
    }

    return GPIO_PIN_NONE;
}

