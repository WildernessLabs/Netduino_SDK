////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <machine.h>
#include <smachine.h>

#include "..\KeyPad\Keypad.h"

//--//

#define CPU_BASE            0xF8000000  

#define VB(x) (*(volatile unsigned char *)(x))
#define VS(x) (*(volatile unsigned short *)(x))

/* Power-Down Modes */
#define STBCR               VB(CPU_BASE+0x0015FF82)
#define STBCR2              VB(CPU_BASE+0x0015FF88)
#define STBCR3              VB(CPU_BASE+0x000A0000)
#define STBCR4              VB(CPU_BASE+0x000A0004)
#define STBCR5              VB(CPU_BASE+0x000A0010)

#define IPRC                VS(0xF8080000)
#define IPRD                VS(0xF8080002)


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
    return_value = g_GPIO_KEYPAD_Driver.KeyPad_Search();
	if(return_value != -1 ) trapa(32);
/* */

    
    if (0 != (wakeEvents & SYSTEM_EVENT_FLAG_COM_IN))
    {
        iprUsart = IPRD; 
        IPRD |= 0xDDD0;
    }

    if (0 != (wakeEvents & SYSTEM_EVENT_FLAG_SOCKET))
    {
        iprSocket = IPRC;

        IPRC |= 0x0D00;
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
        IPRD = iprUsart;
    }

    if (0 != (wakeEvents & SYSTEM_EVENT_FLAG_SOCKET))
    {
        IPRC = iprSocket;
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


