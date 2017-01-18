/********************************************************************************/
/* FileName: SH7264_driver_Keypad.cpp                                           */
/* Function: SH7264 Keypad Functions                                            */
/********************************************************************************/

/*
 *    Copyright (C) 2010 Renesas Electronics America Inc. All rights reserved.
 */
 
#include <tinyhal.h>
#include "SH7264_Keypad.h"
#include <../GPIO/SH7264_GPIO.h>

#define KEY_NUM     16
#define PIN_HIGH    TRUE
#define PIN_LOW     FALSE

//#define IPR14 (*(volatile unsigned short *)(0xFFFE0C10))


SH7264_Keypad_Driver g_SH7264_Keypad_Driver;

BOOL SH7264_Keypad_Driver::Keypad_Initialize()
{   
    GLOBAL_LOCK(irq);   
    PORT.PHCR0.WORD = 0x1111;
    CPU_GPIO_EnableInputPin(SH7264_GPIO::PH00, TRUE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP); 
    CPU_GPIO_EnableInputPin(SH7264_GPIO::PH01, TRUE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP); 
    CPU_GPIO_EnableInputPin(SH7264_GPIO::PH02, TRUE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP); 
    CPU_GPIO_EnableInputPin(SH7264_GPIO::PH03, TRUE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP); 
    
    g_SH7264_Keypad_Driver.Keyscan_result = -1;
    g_SH7264_Keypad_Driver.Edge_flag = 0;
    g_SH7264_Keypad_Driver.Keyscan_Active = 0;
    return TRUE;
}

BOOL SH7264_Keypad_Driver::Keypad_UnInitialize()
{       
    return TRUE;
}

UINT16 SH7264_Keypad_Driver::ADConverter(unsigned char channel)
{
    UINT16 data=0;  

    // setting up for ADC channel 0
    ADC.ADCSR.WORD = CKS_DIV8 | channel;
        
    // set start bit to begin
    ADC.ADCSR.WORD |= ADST;

    while ( !(ADC.ADCSR.WORD & ADF) );

    switch (channel)
    {
        case 0: 
            data = ADC.ADDRA;
            break;
        case 1: 
            data = ADC.ADDRB;
            break;
        case 2: 
            data = ADC.ADDRC;
            break;
        case 3: 
            data = ADC.ADDRD;
            break;
        default:
            hal_printf("Please choose channel 0..3 \r\n");
            break;
    }

    // clear the END Flag to continue on to next conversion
    ADC.ADCSR.WORD = ADC.ADCSR.WORD & ~ADF;

    return data;
}

int SH7264_Keypad_Driver::Key_Press(void)
{
    unsigned short row[4], col, value, i;
    int pin;        
    

    static UINT16 prev_key = -1;

   
    g_SH7264_Keypad_Driver.Keyscan_result = -1;
    
    pin = -1;
    for (col = 0; col<4; col++)
    {   
        value = ADConverter(col);

        row[col] = value < (UINT16)VALUE1 ? 3 :(value < (UINT16)VALUE2 ? 2: (value < (UINT16)VALUE3 ? 1: (value < (UINT16)VALUE4 ? 0: 0xFFFF)));    
    
        if (row[col] == 0xFFFF) continue;

        pin = row[col]+ col*4 + SH7264_GPIO::c_sw_key_offset;
        break;
    }

    if (pin != -1) 
    {
//        g_SH7264_Keypad_Driver.Keyscan_result = key_code_tbl[pin];
        g_SH7264_Keypad_Driver.Keyscan_result = pin;
        g_SH7264_Keypad_Driver.Edge_flag = 1;
        prev_key = g_SH7264_Keypad_Driver.Keyscan_result ;
    }
    else
    {
        if (prev_key != -1)
         {
           g_SH7264_Keypad_Driver.Edge_flag = 0;
           g_SH7264_Keypad_Driver.Keyscan_result = prev_key;
           prev_key = -1;
         }
    }
    return  g_SH7264_Keypad_Driver.Keyscan_result;
}
