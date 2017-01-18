/*
 *    Copyright (C) 2010 Renesas Electronics America Inc  All rights reserved.
 */
 
#include <tinyhal.h>
#include "SH7264_QVGA_LCD.h"
/**********************************************************************/
/*                                                                    */
/* QVGA_LCD_DAC_RSPI_Init                                                     */
/*                                                                    */
/**********************************************************************/

void SH7264_QVGA_LCD_Driver::QVGA_LCD_DAC_RSPI_Init( void )
{
    /* Select RSPI channel 1 pins */
    PORT.PGCR5.BIT.PG23MD = 1;
    PORT.PGCR5.BIT.PG22MD = 1;
    PORT.PGCR5.BIT.PG21MD = 1;
        
    /* Disable Module */
    RSPI1.SPCR.BIT.SPE = 0;
    
    /* Set RSPI Registers */
    RSPI1.SPPCR.BIT.MOIFE = 0;  /* MOSI Idle output value is the MOIFV bit */
    RSPI1.SPPCR.BIT.MOIFV = 0;  /* MOSI Idle output value = 0 */
    
    RSPI1.SSLP.BIT.SSL0P = 0;   /* SSL signal 0-active */

    RSPI1.SPDCR.BIT.TXDMY = 0;  /* Disbale dummy data transmis */   
    RSPI1.SPDCR.BIT.SPLW = 3;   /* SPDR is accessed in long */

    RSPI1.SPCMD0.BIT.SCKDEN = 1;/* An RSPCK delay equal to SPCKD settings */
    RSPI1.SPCMD0.BIT.SLNDEN = 1;/* An SSL negation delay equal to SSLND settings */
    RSPI1.SPCMD0.BIT.SPNDEN = 1;/* A next-access delay equal to SPND settings */
    RSPI1.SPCMD0.BIT.LSBF = 0;  /* MSB first */
    RSPI1.SPCMD0.BIT.SPB = 0xf; /* 16 bits */
    RSPI1.SPCMD0.BIT.SSLKP = 0; /* Keeps the SSL signal level until the beginning of the next access */
    RSPI1.SPCMD0.BIT.BRDV = 3;  /* Select the base bit rate divided by 8 */
    RSPI1.SPCMD0.BIT.CPOL = 1;  /* RSPCK = 0 when idle */
    RSPI1.SPCMD0.BIT.CPHA = 1;  /* Data variation on even edge, data sampling on
                                   odd edge */  
    RSPI1.SPSCR.BYTE = 0; /* 0 -> 0 -> ... */
    
    RSPI1.SPCKD.BYTE = 0;       /* 1 RSPCK */

    RSPI1.SSLND.BYTE = 0;       /* 1 RSPCK */

    RSPI1.SPND.BYTE = 0;        /* 1 RSPCK + 2 B0 */

    RSPI1.SPBR.BYTE = 5;        /* Bit Rate = 750kbps */
    
    RSPI1.SPCR.BIT.MSTR = 1;    /* Master mode */
    RSPI1.SPCR.BIT.FWMM = 1;    /* Enables minimum transfer wait mode */
}

/**********************************************************************/
/*                                                                    */
/* QVGA_LCD_DAC_RSPI_Write_Dev_Reg                                             */
/*                                                                    */
/**********************************************************************/
int SH7264_QVGA_LCD_Driver::QVGA_LCD_DAC_RSPI_Write_Dev_Reg( unsigned char subaddress, unsigned char data )
{   
    unsigned long l, i;

    /*=== send sub-address ===*/
    RSPI1.SPSR.BIT.MODF = 0;    /* Clears Errors */
    
    /* Reset Transmit Buffer */
    RSPI1.SPBFCR.BIT.TXRST = 1;
    RSPI1.SPBFCR.BIT.RXRST = 1;
    
    for (i=0; i<0x1000; i++);
    
    RSPI1.SPBFCR.BIT.TXRST = 0;
    RSPI1.SPBFCR.BIT.RXRST = 0;
    RSPI1.SPBFCR.BIT.RXTRG = 1;//4bytes
    RSPI1.SPBFCR.BIT.TXTRG = 3;//0bytes
    
    while ( RSPI1.SPSR.BIT.SPTEF == 0 );
            
    RSPI1.SPCR.BIT.SPE = 0;     /* Disable */

//  RSPI1.SPDR.LONG = ((0xA0|subaddress)<<24) | (data<<16);
    RSPI1.SPDR.LONG = ((0x70)<<24) | (subaddress<<16);
    
    RSPI1.SPCR.BIT.SPE = 1;     /* Enable */
    
    while( RSPI1.SPSR.BIT.TEND == 0 );  

    while(RSPI1.SPSR.BIT.SPRF == 1 )
        l = RSPI1.SPDR.LONG;

    /*=== send data ===*/

    RSPI1.SPSR.BIT.MODF = 0;    /* Clears Errors */
    
    /* Reset Transmit Buffer */
    RSPI1.SPBFCR.BIT.TXRST = 1;
    RSPI1.SPBFCR.BIT.RXRST = 1;
    
    for (i=0; i<0x1000; i++);
    
    RSPI1.SPBFCR.BIT.TXRST = 0;
    RSPI1.SPBFCR.BIT.RXRST = 0;
    RSPI1.SPBFCR.BIT.RXTRG = 1;//4bytes
    RSPI1.SPBFCR.BIT.TXTRG = 3;//0bytes
    
    while ( RSPI1.SPSR.BIT.SPTEF == 0 );
            
    RSPI1.SPCR.BIT.SPE = 0;     /* Disable */

    RSPI1.SPDR.LONG = ((0x72)<<24) | (data<<16);
    
    RSPI1.SPCR.BIT.SPE = 1;     /* Enable */
    
    while( RSPI1.SPSR.BIT.TEND == 0 );  

    while(RSPI1.SPSR.BIT.SPRF == 1 )
        l = RSPI1.SPDR.LONG;
        
    return 0;
}


/* Delay in units of milliseconds */
void SH7264_QVGA_LCD_Driver::QVGA_LCD_DAC_Delay_ms(unsigned long units)
{
    register unsigned long counter = units * 1200;

    /* Generate delay by executing nop instructions */
    while (counter--)
    {
        while(0);
    }
}   

/**********************************************************************/
/*                                                                    */
/* QVGA_LCD_DAC_RSPI_Initialize                                                      */
/*                                                                    */
/**********************************************************************/
void SH7264_QVGA_LCD_Driver::QVGA_LCD_DAC_Initialize( void ) 
{
    QVGA_LCD_DAC_RSPI_Init();

    QVGA_LCD_DAC_Delay_ms(1000);

    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x00, 0x01);

    QVGA_LCD_DAC_Delay_ms(10);
    
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x05, 0x8b);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x50, 0x00);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x51, 0x00);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x40, 0x00);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x41, 0x00);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x42, 0x00);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x40, 0x10);

    QVGA_LCD_DAC_Delay_ms(100);

    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x40, 0x02);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x41, 0x50);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x42, 0x04);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x40, 0x12);

    QVGA_LCD_DAC_Delay_ms(100);

    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x40, 0x03);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x41, 0x66);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x42, 0x0f);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x40, 0x13);

    QVGA_LCD_DAC_Delay_ms(100);

    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x40, 0x01);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x41, 0x91);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x42, 0x03);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x40, 0x11);

    QVGA_LCD_DAC_Delay_ms(100);

    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x40, 0x06);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x41, 0xce);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x42, 0x00);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x40, 0x16);

    QVGA_LCD_DAC_Delay_ms(100);

    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x50, 0x34);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x51, 0x00);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x40, 0x00);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x41, 0x02);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x42, 0x04);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x40, 0x10);

    QVGA_LCD_DAC_Delay_ms(20);

    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x40, 0x00);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x41, 0x0b);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x42, 0x14);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x40, 0x10);

    QVGA_LCD_DAC_Delay_ms(20);

    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x03, 0x00);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x09, 0xf4);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x0a, 0x00);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x0b, 0x00);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x0c, 0x00);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x0d, 0x00);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x0e, 0x00);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x34, 0x00);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x35, 0x00);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x36, 0x00);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x37, 0x00);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x04, 0x01);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x60, 0x74);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x61, 0x51);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x62, 0x05);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x63, 0x23);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x64, 0x33);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x65, 0x15);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x66, 0x03);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x67, 0x06);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x68, 0x00);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x69, 0x62);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x6a, 0x30);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x6b, 0x13);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x6c, 0x35);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x6d, 0x23);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x6e, 0x03);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x6f, 0x05);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x70, 0x00);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x71, 0x06);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x40, 0x00);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x41, 0x8b);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x42, 0x14);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x40, 0x10);

    QVGA_LCD_DAC_Delay_ms(100);

    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x23, 0x36);
    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x04, 0x03);

    QVGA_LCD_DAC_Delay_ms(1);

    QVGA_LCD_DAC_RSPI_Write_Dev_Reg(0x06, 0x11);
}
