////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <machine.h>
#include <smachine.h>


//--//

#define CPUSTB_BASE            0xFFFE0000  

#define VB(x) (*(volatile unsigned char *)(x))
#define VS(x) (*(volatile unsigned short *)(x))

/* Power-Down Modes */
#define STBCR               VB(CPUSTB_BASE+0x0014)
#define STBCR2              VB(CPUSTB_BASE+0x0018)
#define STBCR3              VB(CPUSTB_BASE+0x0408)
#define STBCR4              VB(CPUSTB_BASE+0x040C)
#define STBCR5              VB(CPUSTB_BASE+0x0410)

#define IPR19              VS(0xFFFE0C1A)
#define IPR16              VS(0xFFFE0C14)


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
#if 0
    return_value = g_GPIO_KEYPAD_Driver.KeyPad_Search();
    if(return_value != -1 ) trapa(32);
#endif
/* */

    
    if (0 != (wakeEvents & SYSTEM_EVENT_FLAG_COM_IN))
    {
        iprUsart = IPR16; 
        IPR16 |= 0xDDD0;
    }

    if (0 != (wakeEvents & SYSTEM_EVENT_FLAG_SOCKET))
    {
        iprSocket = IPR19;

        IPR19 |= 0x00D0;
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
        IPR16 = iprUsart;
    }

    if (0 != (wakeEvents & SYSTEM_EVENT_FLAG_SOCKET))
    {
        IPR19 = iprSocket;
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


