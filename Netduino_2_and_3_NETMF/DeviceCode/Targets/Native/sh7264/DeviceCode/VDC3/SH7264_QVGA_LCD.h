/************************************************************/
/*  FileName    : SH7264_QVGA_LCD.h             */
/*  FUNC        : SH7264 QVGA LCD Register Header File  */
/************************************************************/

/*
 *    Copyright (C) 2010 Renesas Electronics America Inc. All rights reserved.
 */
 
#ifndef _SH7264_QVGA_LCD_H_
#define _SH7264_QVGA_LCD_H_

//#include "..\Include\iodefine.h"


struct SH7264_QVGA_LCD_Driver
{
    static void QVGA_LCD_DAC_RSPI_Init( void );
    static int QVGA_LCD_DAC_RSPI_Write_Dev_Reg( unsigned char subaddress, unsigned char data );
    static void QVGA_LCD_DAC_Delay_ms(unsigned long units);
    static void QVGA_LCD_DAC_Initialize( void );
};

#endif //SH7264_QVGA_LCD_H_
