///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Nativesample application 
//
// Copyright (C) 2001-2009 Microsoft Corporation. All rights reserved. 
// This file is part of the Microsoft .NET Micro Framework Porting Kit Code Samples and is unsupported. 
// Use of this sample source code is subject to the terms of the Microsoft license agreement under which 
// you licensed this sample source code. 
// 
// THIS SAMPLE CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// 
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *    Copyright (C) 2010 Renesas Electronics America Inc. All rights reserved.
 */
 
#include <tinyhal.h>
#include <Tests.h>
#include "nativesample.h"

#define BUFF_SIZE 256

extern struct BlockStorageDevice  g_INTERNALFLASH;
extern struct IBlockStorageDevice SH7216_INTERNAL_FLASH_DeviceTable;
extern struct BLOCK_CONFIG        g_INTERNALFLASH_Config;


BlockStorageDevice  * pBlockStorageDevice = &g_INTERNALFLASH;
BLOCK_CONFIG        * pBLOCK_CONFIG         = &g_INTERNALFLASH_Config;


//--//

HAL_DECLARE_NULL_HEAP();


void io_output_flash(unsigned long address, unsigned int length)
{
	unsigned int i = 0;
	unsigned char out_char;
	char c[2];
	char ASCII_lookup[17] = {"0123456789ABCDEF"};
	// Print out message	
	c[1] = '\0';
	for(i=0;i<length;i++)
	{
		// New line ever 32 bytes
		if( (i!=0) && (i&0x1F) == 0)
		{
			hal_printf("\n");// Insert newline
			hal_printf("\r");
		}
		
		out_char = 	*(char *)address;
		out_char = out_char >> 4;		// upper nibble
		c[0] = ASCII_lookup[out_char];
		//hal_printf("%s", ASCII_lookup[out_char]);
		hal_printf(c);

		out_char = 	*(char *)address;
		out_char &= 0x0F;		// lower nibble

		c[0] = ASCII_lookup[out_char];
		//hal_printf("%s", ASCII_lookup[out_char]);
		hal_printf(c);
	
		address++;		// Next data location
	}

    /* ---- wait 1-bit period ---- */
    for(i=0; i<0x6000; i++);        /* 1-bit period  */
	hal_printf("\n");
	
    /* ---- wait 1-bit period ---- */
    for(i=0; i<0x6000; i++);        /* 1-bit period  */	
	hal_printf("\r");
}


void ApplicationEntryPoint()
{
	char Data[1], previousChar = '\0';
	int ret = 0, TestItem = 0;
	BOOL validCommand = FALSE;
	UINT32 sleep = 100;
	Data[1] = '\0';
	BYTE Buff[BUFF_SIZE];

   do
    {
        lcd_printf( "\n\n\n\n   SH7216 - ROK571267C001BR\n" );
        lcd_printf( "Renesas Electronics America Inc." );

		//Serial TEST
		hal_printf( "\r\nSH7216 .NET Micro Framework\r\n" );
		hal_printf( "Renesas Electronics America Inc.\r\n" );		
		hal_printf( ">" );

		NATIVE_PROFILE_PAL_EVENTS();
		
		do
		{
			UINT32 Events = Events_WaitForEvents( SYSTEM_EVENT_FLAG_COM_IN, sleep);

			if(Events & SYSTEM_EVENT_FLAG_COM_IN)			
			{
				Events_Clear( SYSTEM_EVENT_FLAG_COM_IN );
				while((ret = DebuggerPort_Read( HalSystemConfig.DebugTextPort, Data, 1 )) > 0)
				{					
					switch(TestItem)
					{
						case 0:		//None
							if(Data[0] == 0xD)				//0xD: ENTER key
							{								
								hal_printf( "\r\n" );
								if(validCommand)
								{
									TestItem = 1;			//Enter test
									validCommand = FALSE;
									previousChar = '\0';
									hal_printf( "\r\nTest Menu:\r\n" );
									hal_printf( "	[F]lash memory Tests\r\n" );
									hal_printf( "	E[x]it\r\n" );
									hal_printf( "Your choice: " );
								}
								else if(previousChar == 'I')
								{
									hal_printf("** Invalid Selection\r\n>");
									previousChar = '\0';
								}
								else
									hal_printf( ">" );
							}
							else
							{
								hal_printf( Data );
								if(Data[0] == 'd' || Data[0] == 'D')
								{
									if(previousChar == '\0')
										previousChar = 'd';
									else
									{
										validCommand = FALSE;
										previousChar = 'I';		//Invalid character input
									}
								}
								else if(Data[0] == 't' || Data[0] == 'T')
								{
									if(previousChar == 'd')
									{
										previousChar = 't';
										validCommand = TRUE;
									}
									else
									{
										validCommand = FALSE;
										previousChar = 'I';		//Invalid character input
									}
								}
								else
								{
									validCommand = FALSE;
									previousChar = 'I';			//Invalid character input
								}
							}
							break;
						case 1:		//Enter test
							hal_printf( Data );
							switch(Data[0])
							{
								case 'X':
								case 'x':
									TestItem = 0;
									hal_printf( "\r\n\n>" );
									break;
								case 'F':
                                case 'f':
                                    TestItem = 2;
                                    hal_printf( "\r\n\nFlash memory Menu:\r\n" );
                                    hal_printf( "   [1]Erase block\r\n" );
                                    hal_printf( "   [2]write block\r\n" );
									hal_printf( "   [3]Erase FLD block\r\n" );
                                    hal_printf( "   [4]write FLD block\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;   
								default:
									hal_printf( "\r\n\nTest Menu:\r\n" );
									hal_printf( "	[F]lash memory Tests\r\n" );
									hal_printf( "	E[x]it\r\n" );
									hal_printf( "Your choice: " );
									break;
							}
							break;
							case 2:     //Flash memory test
                            hal_printf( Data );
                            hal_printf( "\r\n" );
                            switch(Data[0])
                            {
                                case 'X':
                                case 'x':
                                    TestItem = 1;
                                    hal_printf( "\r\nTest Menu:\r\n" );
									hal_printf( "	[F]lash memory Tests\r\n" );
									hal_printf( "	E[x]it\r\n" );
									hal_printf( "Your choice: " );
                                    break;
                                case '1':
									hal_printf("Start Flash Erase block Test\n\r");
									/* Show Initial value of Flash */
									hal_printf("Initial Value\n\r");
									hal_printf("Data at 0xC0000\n\r");
									io_output_flash(0xC0000, 512);
									SH7216_INTERNAL_FLASH_DeviceTable.InitializeDevice(pBLOCK_CONFIG);
									SH7216_INTERNAL_FLASH_DeviceTable.EraseBlock(pBLOCK_CONFIG,0xC0000);
									hal_printf("End Flash Erase block Test\n\r");
									/* Show Initial value of Flash */
									hal_printf("Final Value\n\r");
									hal_printf("Data at 0xC0000\n\r");
									io_output_flash(0xC0000, 512);
                                    
									hal_printf( "\r\n\nFlash memory Menu:\r\n" );
                                    hal_printf( "   [1]Erase block\r\n" );
                                    hal_printf( "   [2]write block\r\n" );
									hal_printf( "   [3]Erase FLD block\r\n" );
                                    hal_printf( "   [4]write FLD block\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '2':
									for(int i=0; i<BUFF_SIZE; i++) Buff[i]=i;
									hal_printf("Start Flash Write block Test\n\r");
									/* Show Initial value of Flash */
									hal_printf("Initial Value\n\r");
									hal_printf("Data at 0xC0000\n\r");
									io_output_flash(0xC0000, 512);
									SH7216_INTERNAL_FLASH_DeviceTable.InitializeDevice(pBLOCK_CONFIG);
									SH7216_INTERNAL_FLASH_DeviceTable.Write(pBLOCK_CONFIG,0xC0000,BUFF_SIZE,Buff,0x0);
                                    hal_printf("End Flash Write block Test\n\r");
									/* Show Initial value of Flash */
									hal_printf("Initial Value\n\r");
									hal_printf("Data at 0xC0000\n\r");
									io_output_flash(0xC0000, 512);
									
									hal_printf( "\r\n\nFlash memory Menu:\r\n" );
                                    hal_printf( "   [1]Erase block\r\n" );
                                    hal_printf( "   [2]write block\r\n" );
									hal_printf( "   [3]Erase FLD block\r\n" );
                                    hal_printf( "   [4]write FLD block\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
								case '3':
									hal_printf("Start Flash Erase block Test\n\r");
									/* Show Initial value of Flash */
									hal_printf("Initial Value\n\r");
									hal_printf("Data at 0x80104000\n\r");
									io_output_flash(0x80104000, 512);
									SH7216_INTERNAL_FLASH_DeviceTable.InitializeDevice(pBLOCK_CONFIG);
									SH7216_INTERNAL_FLASH_DeviceTable.EraseBlock(pBLOCK_CONFIG,0x80104000);
									hal_printf("End Flash Erase block Test\n\r");
									/* Show Initial value of Flash */
									hal_printf("Final Value\n\r");
									hal_printf("Data at 0x80104000\n\r");
									io_output_flash(0x80104000, 512);
                                    
									hal_printf( "\r\n\nFlash memory Menu:\r\n" );
                                    hal_printf( "   [1]Erase block\r\n" );
                                    hal_printf( "   [2]write block\r\n" );
									hal_printf( "   [3]Erase FLD block\r\n" );
                                    hal_printf( "   [4]write FLD block\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '4':
									for(int i=0; i<BUFF_SIZE; i++) Buff[i]=i;
									hal_printf("Start Flash Write block Test\n\r");
									/* Show Initial value of Flash */
									hal_printf("Initial Value\n\r");
									hal_printf("Data at 0x80104000\n\r");
									io_output_flash(0x80104000, 512);
									SH7216_INTERNAL_FLASH_DeviceTable.InitializeDevice(pBLOCK_CONFIG);
									SH7216_INTERNAL_FLASH_DeviceTable.Write(pBLOCK_CONFIG,0x80104000,BUFF_SIZE,Buff,0x0);
                                    hal_printf("End Flash Write block Test\n\r");
									/* Show Initial value of Flash */
									hal_printf("Initial Value\n\r");
									hal_printf("Data at 0x80104000\n\r");
									io_output_flash(0x80104000, 512);
									
									hal_printf( "\r\n\nFlash memory Menu:\r\n" );
                                    hal_printf( "   [1]Erase block\r\n" );
                                    hal_printf( "   [2]write block\r\n" );
									hal_printf( "   [3]Erase FLD block\r\n" );
                                    hal_printf( "   [4]write FLD block\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                default:
                                    hal_printf( "\r\n\nFlash memory Menu:\r\n" );
                                    hal_printf( "   [1]Erase block\r\n" );
                                    hal_printf( "   [2]write block\r\n" );
									hal_printf( "   [3]Erase FLD block\r\n" );
                                    hal_printf( "   [4]write FLD block\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                            }
                            break;
					}
				}
			}			
		} while(TRUE);
    } while(FALSE); // run only once!

}