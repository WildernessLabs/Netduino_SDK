////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// This file is part of the Microsoft .NET Micro Framework Porting Kit Code Samples and is unsupported. 
// Copyright (C) Microsoft Corporation. All rights reserved. Use of this sample source code is subject to 
// the terms of the Microsoft license agreement under which you licensed this sample source code. 
// 
// THIS SAMPLE CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// 
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "tinyhal.h"
#include "Keypad.h"
#include <..\GPIO\SH7619_GPIO.h>

//--//

#define KEY_NUM     25
#define SPL_KEY     0x108
#define PIN_HIGH    TRUE
#define PIN_LOW     FALSE

GPIO_KEYPAD_Driver g_GPIO_KEYPAD_Driver;


// Key Number
enum
{
    KEY1 = 1,
    KEY2,
    KEY3,
    KEY4, 
    KEY5,
    KEY6,
    KEY7,
    KEY8,
    KEY9,
    KEY10,
    KEY11,
    KEY12,
    KEY13,
    KEY14,
    KEY15,
    KEY16,
    KEY17,
    KEY18,
    KEY19,
    KEY20,
    KEY21,
    KEY22,
    KEY23,
    KEY24,
    KEY25,
    KEY_SW_SPL
};

// GPIO PINs
//enum
//{
//  PC00 = 0,
//  PC01,
//  PC02,
//  PC03,
//  PC04,
//  PC05,
//  PC06,
//  PC07,
//  PC08,
//  PC09,
//  PC10,
//  PC11,
//  PC12,
//  PC13,
//  PC14,
//  PC15,
//  PC16,
//  PC17,
//  PC18,
//  PC19,
//  PC20
//};

// JJ Todo: fix the HW so that the keypad are in incremental order 
static const key_code_tbl[KEY_NUM]=
{
    KEY1, KEY2, KEY3, KEY4,  KEY5,
    KEY6, KEY7, KEY8, KEY9, KEY10,
    KEY11, KEY12, KEY13, KEY14, KEY15,
    KEY16, KEY17, KEY18, KEY19, KEY20,
    KEY21, KEY22, KEY23, KEY24, KEY25,
};

void GPIO_KEYPAD_Driver::Debounce_Delay (unsigned int time)
{
    unsigned long i;
    
    i = 31250;          // 8 ns/instruction * 4 instructions * loop_count = 1ms = 1000000 ns
    i *= time;
    while (i--);        // loop count 
}

void GPIO_KEYPAD_Driver::KeyPad_Initialize (void)
{   
    // configure the port C pins as input and output according to their wiring setup
    /*          
        PC08 ------K1-------K2-------K3-------K4-------K5
    O               |        |        |        |        |
    U   PC10 ------K6-------K7-------K8-------K9-------K10
    T               |        |        |        |        |
    P   PC09 ------K11------K12------K13------K14------K15
    U               |        |        |        |        |
    T   PC11 ------K16------K17------K18------K19------K20
                    |        |        |        |        |
        PC13 ------K21------K22------K23------K24------K25
                    |        |        |        |        |
                    |        |        |        |        |
                    |        |        |        |        |
        PC00 -------/        |        |        |        |      
                             |        |        |        |
    I   PC01 ----------------/        |        |        |       
    N                                 |        |        |        
    P   PC02 -------------------------/        |        |
    U                                          |        |
    T   PC03 ----------------------------------/        |                          
                                                        |
        PC17 -------------------------------------------/
    */   
    
    //Configure the appropriate port c pins as input/output
    //set PC17 as input
    CPU_GPIO_EnableInputPin(SH7619_GPIO::PC17, TRUE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP); 
    
    //Configure the appropriate port c pins as GPIO
    //Set output = PC13, PC11, PC10, PC09, PC08
    CPU_GPIO_EnableOutputPin(SH7619_GPIO::PC13, TRUE);
    CPU_GPIO_EnableOutputPin(SH7619_GPIO::PC11, TRUE);
    CPU_GPIO_EnableOutputPin(SH7619_GPIO::PC10, TRUE);
    CPU_GPIO_EnableOutputPin(SH7619_GPIO::PC09, TRUE);
    CPU_GPIO_EnableOutputPin(SH7619_GPIO::PC08, TRUE);  
    
    //Set input = PC03, PC02, PC01, PC00
    CPU_GPIO_EnableInputPin(SH7619_GPIO::PC03, TRUE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP);
    CPU_GPIO_EnableInputPin(SH7619_GPIO::PC02, TRUE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP);
    CPU_GPIO_EnableInputPin(SH7619_GPIO::PC01, TRUE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP);
    CPU_GPIO_EnableInputPin(SH7619_GPIO::PC00, TRUE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP); 

    g_GPIO_KEYPAD_Driver.key_data_tmp[0] = 0;
    g_GPIO_KEYPAD_Driver.key_data_tmp[1] = 0;
    g_GPIO_KEYPAD_Driver.Keyscan_result = -1;
    g_GPIO_KEYPAD_Driver.Edge_flag = 0;
    g_GPIO_KEYPAD_Driver.Keyscan_Active = 0;
}

char GPIO_KEYPAD_Driver::KeyPad_Check_Bit(unsigned int keybit)
{
    int i = 0;
    int count = 0;
    int bit_number = -1;
  
    if((keybit & SPL_KEY) == SPL_KEY)
    {
        return KEY_SW_SPL;
    }
  
    for(i=0; i<KEY_NUM; i++)
    {
        if(keybit & 0x00000001)
        {
            count++;
            bit_number = i;
        }
        keybit >>=1;
    }
  
    if(count > 1)
    {
        return -1;  // multi key push error
    }
  
    return (char) key_code_tbl[bit_number];
}

int GPIO_KEYPAD_Driver::KeyPad_Search(void)
{
//  char return_value = -1;
    unsigned short tmp1;
    unsigned short tmp2;
    unsigned int tmp;
    unsigned char key_count;
//  unsigned int key_data_tmp[2];
//  unsigned int key_data_fix;
    char key_code;  
    
    g_GPIO_KEYPAD_Driver.Keyscan_result = -1;
    g_GPIO_KEYPAD_Driver.key_data_tmp[0] = 0;
//  key_data_fix=0;
    
    if (g_GPIO_KEYPAD_Driver.Keyscan_Active != 0){

        for(key_count=1; key_count<6;key_count++){ 
        
            if (key_count!=1) g_GPIO_KEYPAD_Driver.key_data_tmp[0] <<= 5;
            
//          Debounce_Delay (5);                 /* add bit of a delay for key bouce */
            
            tmp1 =  CPU_GPIO_GetPinState(SH7619_GPIO::PC16)
                  | CPU_GPIO_GetPinState(SH7619_GPIO::PC17)
                  | CPU_GPIO_GetPinState(SH7619_GPIO::PC18)
                  | CPU_GPIO_GetPinState(SH7619_GPIO::PC19)
                  | CPU_GPIO_GetPinState(SH7619_GPIO::PC20);
            tmp1 = ~tmp1 & 0x2; 
            
            tmp2 =  CPU_GPIO_GetPinState(SH7619_GPIO::PC00)
                  | CPU_GPIO_GetPinState(SH7619_GPIO::PC01)
                  | CPU_GPIO_GetPinState(SH7619_GPIO::PC02)
                  | CPU_GPIO_GetPinState(SH7619_GPIO::PC03)
                  | CPU_GPIO_GetPinState(SH7619_GPIO::PC04)
                  | CPU_GPIO_GetPinState(SH7619_GPIO::PC05)
                  | CPU_GPIO_GetPinState(SH7619_GPIO::PC06)
                  | CPU_GPIO_GetPinState(SH7619_GPIO::PC07)
                  | CPU_GPIO_GetPinState(SH7619_GPIO::PC08)
                  | CPU_GPIO_GetPinState(SH7619_GPIO::PC09)
                  | CPU_GPIO_GetPinState(SH7619_GPIO::PC10)
                  | CPU_GPIO_GetPinState(SH7619_GPIO::PC11)
                  | CPU_GPIO_GetPinState(SH7619_GPIO::PC12)
                  | CPU_GPIO_GetPinState(SH7619_GPIO::PC13)
                  | CPU_GPIO_GetPinState(SH7619_GPIO::PC14)
                  | CPU_GPIO_GetPinState(SH7619_GPIO::PC15);          
            tmp2 = ~tmp2 & 0xF;
            
            tmp = (unsigned short) tmp1 << 3;
            tmp |= (unsigned short) tmp2;
            
            g_GPIO_KEYPAD_Driver.key_data_tmp[0] |= tmp;
            
            // set all keypad output key pins high  
            CPU_GPIO_SetPinState(SH7619_GPIO::PC08, PIN_HIGH);
            CPU_GPIO_SetPinState(SH7619_GPIO::PC09, PIN_HIGH);
            CPU_GPIO_SetPinState(SH7619_GPIO::PC10, PIN_HIGH);
            CPU_GPIO_SetPinState(SH7619_GPIO::PC11, PIN_HIGH);
            CPU_GPIO_SetPinState(SH7619_GPIO::PC13, PIN_HIGH);
            
        //  key_count++;
            
            switch(key_count)
            {
            case 1:
                // set only port PC11 low
                CPU_GPIO_SetPinState(SH7619_GPIO::PC11, PIN_LOW);
                break;
            case 2:
                // set only port PC09 low
                CPU_GPIO_SetPinState(SH7619_GPIO::PC09, PIN_LOW);
                break;          
            case 3:
                // set only port PC10 low
                CPU_GPIO_SetPinState(SH7619_GPIO::PC10, PIN_LOW);
                break;  
            case 4:
                // set only port PC08 low
                CPU_GPIO_SetPinState(SH7619_GPIO::PC08, PIN_LOW);
                break;
            case 5:
                // set only port PC13 low 
                CPU_GPIO_SetPinState(SH7619_GPIO::PC13, PIN_LOW);
                break;
            }
            
            if (key_count == 5)
            {
                if(g_GPIO_KEYPAD_Driver.key_data_tmp[0] != g_GPIO_KEYPAD_Driver.key_data_tmp[1])                        // keydata cmp 
                {
//                  if(key_data_fix != g_GPIO_KEYPAD_Driver.key_data_tmp[0])
//                  {
                    if(g_GPIO_KEYPAD_Driver.key_data_tmp[0] != 0){
                        key_code = KeyPad_Check_Bit(g_GPIO_KEYPAD_Driver.key_data_tmp[0]);      // key push?     
                        if(key_code >= 0)
                        {
                            g_GPIO_KEYPAD_Driver.Keyscan_result = key_code;
                            g_GPIO_KEYPAD_Driver.Edge_flag = 0;
                        }
                        else
                        {
                            g_GPIO_KEYPAD_Driver.key_data_tmp[0] = 0;  //Multi key push error
                            // key release or another key push 
                        }   
                    } 
                    else{
                        key_code = KeyPad_Check_Bit(g_GPIO_KEYPAD_Driver.key_data_tmp[1]);      // key push?     
                        if(key_code >= 0)
                        {
                            g_GPIO_KEYPAD_Driver.Keyscan_result = key_code;
                            g_GPIO_KEYPAD_Driver.Edge_flag = 1;
                        }
                        else
                        {
                            // key release or another key push 
                        }   
                    }
//                      key_data_fix = g_GPIO_KEYPAD_Driver.key_data_tmp[0];
//                  }
//                  else
//                  {
//                      // repeat key not support ,do nothing
//                  }
                    g_GPIO_KEYPAD_Driver.key_data_tmp[1] = g_GPIO_KEYPAD_Driver.key_data_tmp[0];
                }
                else
                {
//                  g_GPIO_KEYPAD_Driver.key_data_tmp[1] = g_GPIO_KEYPAD_Driver.key_data_tmp[0];
                }
                g_GPIO_KEYPAD_Driver.key_data_tmp[0] = 0;           // keydata clear
//              key_count = 0;                  // reset 
            }
        }
    }   
    return g_GPIO_KEYPAD_Driver.Keyscan_result;
}
