////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <machine.h>
#include <smachine.h>

#include <..\Keypad\SH7264_Keypad.h>
//--//

extern void SH7264_SMSC_interrupt(void *param);

#define CPU_BASE            0xFFFE0000 

#define VB(x) (*(volatile unsigned char *)(x))
#define VS(x) (*(volatile unsigned short *)(x))

/* Power-Down Modes */
#define STBCR               VB(CPU_BASE+0x00000014)
#define STBCR2              VB(CPU_BASE+0x00000018)
#define STBCR3              VB(CPU_BASE+0x00000408)
#define STBCR4              VB(CPU_BASE+0x0000040C)
#define STBCR5              VB(CPU_BASE+0x00000410)
#define STBCR6              VB(CPU_BASE+0x00000414)
#define STBCR7              VB(CPU_BASE+0x00000418)
#define STBCR8              VB(CPU_BASE+0x0000041c)

#define IPR17               VS(0xFFFE0C16)
#define IPR01               VS(0xFFFE0818)

void HAL_AssertEx()
{
    // cause an abort and let the abort handler take over
    //*((char*)0xFFFFFFFF) = 'a';
    lcd_printf("\f*** ASSERT ****\r\n");
    while(true);
}

//--//

BOOL CPU_Initialize()
{
    return TRUE;
}

void CPU_Reset()
{
}

void CPU_Sleep(SLEEP_LEVEL level, UINT64 wakeEvents)
{
    UINT16 iprUsart = 0;
    UINT16 iprSocket = 0;
    
    int return_value;
    
    /* Detecting if Keypad is pushed.*/

    return_value = g_SH7264_Keypad_Driver.Key_Press();
    
    if(return_value != -1 ) trapa(32);
    
    if (0 != (wakeEvents & SYSTEM_EVENT_FLAG_COM_IN))
    {
        iprUsart = IPR17; 
        IPR17 |= 0xDDDD;
    }

    if (0 != (wakeEvents & SYSTEM_EVENT_FLAG_SOCKET)){
        iprSocket = IPR01; 
        IPR01 |= 0x0D00;
    }    
    
    switch(level)
    {
        case SLEEP_LEVEL__DEEP_SLEEP:
            STBCR = 8;
            break;
        default:
            STBCR = 0;
            break;
    }

    UINT8 mask = get_imask();

    set_imask(0xC);
    
    sleep();

    set_imask( mask );

    if (0 != (wakeEvents & SYSTEM_EVENT_FLAG_COM_IN))
    {
        IPR17 = iprUsart;
    }

    if (0 != (wakeEvents & SYSTEM_EVENT_FLAG_SOCKET)){        
        IPR01 = iprSocket;
    }
    
}

void CPU_ChangePowerLevel(POWER_LEVEL level)
{
}

BOOL CPU_IsSoftRebootSupported ()
{
    return TRUE;
}


void CPU_Halt()
{
    while(true);
}


