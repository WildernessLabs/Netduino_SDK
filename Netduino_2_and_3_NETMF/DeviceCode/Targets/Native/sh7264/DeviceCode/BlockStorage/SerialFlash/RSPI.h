/*
 *    Copyright (C) 2010 Renesas Electronics America Inc. All rights reserved.
 */
 
#ifndef __RSPI_DRIVER_H__
#define __RSPI_DRIVER_H__	

#define DMAC_CHCR_TE     ( 0x1 <<  1 )
#define DMAC_CHCR_DE     ( 0x1 <<  0 )

//======== Serial Flash Commands ==========//
#define	SFLASH_READ_ID			0x9F
#define SFLASH_CHIP_ERASE		0xc7
#define SFLASH_SECTOR_ERASE		0xd8
#define SFLASH_BYTE_PROGRAM		0x02
#define SFLASH_BYTE_READ		0x03
#define SFLASH_WRITE_ENABLE		0x06
#define SFLASH_WRITE_DISABLE	0x04
#define SFLASH_READ_STATUS		0x05
#define SFLASH_WRITE_STATUS		0x01
#define UNPROTECT_STATUS		0x00
#define PROTECT_STATUS			0x3C
#define	SFLASH_DUAL_READ		0x0B

struct RSPI_Driver {

	static void RSPI_Initialize(void);
	static void Protect_Ctrl(unsigned char	proctect_flag);
	static void Chip_ID(unsigned char * id);
	static void Sector_Erase(unsigned long addr);
	static void Byte_Program(unsigned int addr, unsigned char *buf, int size);
	static void Byte_Read(unsigned int addr, unsigned char *buf, int size);
	static void Write_Enable();
	static void Write_Disable();
	static void Busy_Wait();
	static unsigned char Read_Status();
	static void Write_Status(unsigned char status);
	static void Transmit_Exe(unsigned char *cmd, int cmd_size, unsigned char *buff, int buff_size);
	static void Receive_Exe(unsigned char *cmd, int cmd_size, unsigned char *buff, int buff_size);
	static void Byte_Transfer(unsigned char tx, unsigned char *rx, unsigned char transfer_flag);
	static void Wait_TX_End();
};

#endif /* _SERIAL_FLASH_H_ */
