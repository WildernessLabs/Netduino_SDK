/*
 *    Copyright (C) 2010 Renesas Electronics America Inc. All rights reserved.
 */

#include <tinyhal.h>
#include "RSPI.h"
#include "..\..\Include\iodefine.h"


void RSPI_Driver::Protect_Ctrl(unsigned char    proctect_flag)
{
    if( proctect_flag == 0 )
        Write_Status(UNPROTECT_STATUS);     
    else
        Write_Status(PROTECT_STATUS);   
}

void RSPI_Driver::Chip_ID(unsigned char * id)
{
    unsigned char cmd[1];
    cmd[0] = SFLASH_READ_ID;

    Receive_Exe(cmd, 1, id, 3);
}


void RSPI_Driver::Sector_Erase(unsigned long addr)
{
    unsigned char cmd[4];
    //unsigned int addr = sector_num * 0x10000;
    
    cmd[0] = SFLASH_SECTOR_ERASE;
    cmd[1] = (addr >> 16) & 0xff;
    cmd[2] = (addr >>  8) & 0xff;
    cmd[3] =  addr        & 0xff;

    Write_Enable();
    Transmit_Exe(cmd, 4, NULL, 0);
    Busy_Wait();
    Write_Disable();
}

void RSPI_Driver::Byte_Program(unsigned int addr, unsigned char *buf, int size)
{
    unsigned char cmd[4];
    
    cmd[0] = SFLASH_BYTE_PROGRAM;
    cmd[1] = (unsigned char)((addr >> 16) & 0xff);
    cmd[2] = (unsigned char)((addr >>  8) & 0xff);
    cmd[3] = (unsigned char)( addr        & 0xff);
    
    Write_Enable();
    
    Transmit_Exe(cmd, 4, buf, size);
    Busy_Wait();
    Write_Disable();
}

void RSPI_Driver::Byte_Read(unsigned int addr, unsigned char *buf, int size)
{
    unsigned char cmd[4];
    
    cmd[0] = SFLASH_BYTE_READ;
    cmd[1] = (unsigned char)((addr >> 16) & 0xff);
    cmd[2] = (unsigned char)((addr >>  8) & 0xff);
    cmd[3] = (unsigned char)( addr        & 0xff);
    
    Receive_Exe(cmd, 4, buf, size);
}

void RSPI_Driver::Write_Enable()
{
    unsigned char cmd[1];
    
    cmd[0] = SFLASH_WRITE_ENABLE;
    
    Transmit_Exe(cmd, 1, NULL, 0);
}

void RSPI_Driver::Write_Disable()
{
    unsigned char cmd[1];
    
    cmd[0] = SFLASH_WRITE_DISABLE;
    
    Transmit_Exe(cmd, 1, NULL, 0);
}

void RSPI_Driver::Busy_Wait()
{
    while ((Read_Status() & 0x01) != 0) {   /* RDY/BSY */
    }
}

unsigned char RSPI_Driver::Read_Status()
{
    unsigned char buf;
    unsigned char cmd[1];

    cmd[0] = SFLASH_READ_STATUS;
    
    Receive_Exe(cmd, 1, &buf, 1);
    
    return buf;
}

void RSPI_Driver::Write_Status(unsigned char status)
{
    unsigned char cmd[2];

    cmd[0] = SFLASH_WRITE_STATUS;
    cmd[1] = status;    

    Write_Enable(); 
    Transmit_Exe(cmd, 2, NULL, 0);
    Busy_Wait();
    Write_Disable();
}

void RSPI_Driver::RSPI_Initialize()
{
    /* ==== PORT ==== */
    PORT.PFCR3.BIT.PF12MD = 3;  /* PF12:MISO0 */
    PORT.PFCR2.BIT.PF11MD = 3;  /* PF11:MOSI0 */
    PORT.PFCR2.BIT.PF10MD = 3;  /* PF10:SSL00 */
    PORT.PFCR2.BIT.PF9MD  = 3;  /* PF9:RSPCK0 */
    
    /* ==== CPG ==== */
    CPG.STBCR5.BIT.MSTP51 = 0;  /* RSPI0 active */
    
    /* ==== RSPI ==== */
    RSPI0.SPCR.BYTE   = 0x00;   
    RSPI0.SPPCR.BYTE  = 0x30;   
    RSPI0.SPBR.BYTE   = 0x01;   
    RSPI0.SPDCR.BYTE  = 0x20;   
                                
    RSPI0.SPCKD.BYTE  = 0x00;   
    RSPI0.SSLND.BYTE  = 0x00;   
    RSPI0.SPND.BYTE   = 0x00;   
    RSPI0.SPSCR.BYTE  = 0x00;   
    RSPI0.SPCMD0.WORD = 0xE780; 
                                                    
    RSPI0.SPBFCR.BYTE = 0xC0;   
    RSPI0.SPBFCR.BYTE = 0x00;                           
                            
    RSPI0.SSLP.BYTE   = 0x00;   
    RSPI0.SPCR.BYTE   = 0x48;   
}

void RSPI_Driver::Transmit_Exe(unsigned char *cmd, int cmd_size, unsigned char *buff, int buff_size)
{
    unsigned char data;
    unsigned char tmp;                  
    short total_bytes = cmd_size + buff_size;           
    
    RSPI0.SPCMD0.BIT.SSLKP = 1;
    
    while(1)
    {
        if( cmd_size > 0 )
        {
            data = *cmd++;
            cmd_size--;
        }
        else
            data = *buff++;
        
        if(total_bytes > 1)
        {   
            Byte_Transfer(data, &tmp, 1);
            total_bytes--;
        }
        else
            break;
    }
    Wait_TX_End();                  
    RSPI0.SPCMD0.BIT.SSLKP = 0; 
            
    Byte_Transfer(data, &tmp ,1);
}

void RSPI_Driver::Receive_Exe(unsigned char *cmd, int cmd_size, unsigned char *buff, int buff_size)
{
    unsigned char tmp;
    
    RSPI0.SPCMD0.BIT.SSLKP = 1;

    while( cmd_size-- > 0)
        Byte_Transfer(*cmd++, &tmp, 1);

    while( buff_size-- > 1)
        Byte_Transfer(0, buff++, 0);

    Wait_TX_End();                  
    RSPI0.SPCMD0.BIT.SSLKP = 0;         
    Byte_Transfer(0, buff, 0);
}

void RSPI_Driver::Byte_Transfer(unsigned char tx, unsigned char *rx, unsigned char transfer_flag)
{
    while(RSPI0.SPSR.BIT.SPTEF == 0);

    RSPI0.SPDR.BYTE = tx;
    
    while (RSPI0.SPSR.BIT.SPRF == 0);
    
    *rx = RSPI0.SPDR.BYTE;
}

void RSPI_Driver::Wait_TX_End()
{
    while(RSPI0.SPSR.BIT.TEND == 0);    
}