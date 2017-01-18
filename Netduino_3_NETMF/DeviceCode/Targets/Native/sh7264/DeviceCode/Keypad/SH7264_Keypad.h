/************************************************************/
/*  FileName    : SH7264_Keypad.h                           */
/*  FUNC        : Keypad Register Header File               */
/************************************************************/

/*
 *    Copyright (C) 2010 Renesas Electronics America Inc. All rights reserved.
 */
 
#include <tinyhal.h>

#ifndef _M3AHS64_KEYPAD_H_
#define _M3AHS64_KEYPAD_H_

#define ADF     (1<<15)
#define ADIE    (1<<14)
#define ADST    (1<<13)

#define CKS_DIV4    (0<<6)
#define CKS_DIV8    (1<<6)
#define CKS_DIV16   (2<<6)  
#define CKS_DIV32   (3<<6)

#define MDS_SINGLEMODE  (0<<4)
#define MDS_MULTIMODE   (2<<4)
#define MDS_SCANEMODE   (3<<4)

// #define CH0  0
// #define CH1  1
// #define CH2  2
// #define CH3  3

#define VALUE0  0x0000
#define VALUE1  0x2000
#define VALUE2  0x6000
#define VALUE3  0xA000
#define VALUE4  0xE000

#define VS(x)   (*(volatile unsigned short *)(x))
#define IPR14   VS(0xFFFE0C10)

struct SH7264_Keypad_Driver
{
    static BOOL Keypad_Initialize  ();
    static BOOL Keypad_UnInitialize();
    static unsigned short ADConverter (unsigned char channel);  
    static int Key_Press(void);
    static void Keypad_Test();
    
    char Keyscan_result;
    char Edge_flag;
    char Keyscan_Active;
};

extern SH7264_Keypad_Driver g_SH7264_Keypad_Driver;

#endif //_SH7264_KEYPAD_H_