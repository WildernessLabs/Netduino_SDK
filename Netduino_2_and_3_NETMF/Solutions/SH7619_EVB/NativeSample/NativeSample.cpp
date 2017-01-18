////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////#include <tinyhal.h>

/*
 *    Copyright (C) 2009-2010 Renesas Electronics America Inc. All rights reserved.
 */
 
#include <tinyhal.h>
#include <Tests.h>
#include "nativesample.h"
#include "..\..\..\DeviceCode\Targets\Native\sh2\DeviceCode\sh7619\KeyPad\Keypad.h"
#include "lcdTest.h"

//--//

extern void  Network_Interface_Process_Packet();
extern int   Network_Interface_ARP_Request(unsigned long dest_ip);
extern BOOL  Network_Interface_ARP_Ack();
extern BOOL  Network_Interface_ICMP_Echo(unsigned long dest_ip);
extern BOOL  Network_Interface_ICMP_Ack();
extern void  Network_Interface_IP_Address();
extern void  Network_Interface_EtherC_Registers();
extern void  Network_Interface_EDMAC_Registers();

extern struct BlockStorageDevice  g_S29GL064A_16_BS;
extern struct IBlockStorageDevice g_AM29DL_16_BS_DeviceTable;
extern struct BLOCK_CONFIG        g_S29GL064A_16_BS_Config;


#if defined(SH7619_EVB_8M_FLASH)
extern struct BlockStorageDevice  g_S29GL064A_16_BS;
extern struct IBlockStorageDevice g_AM29DL_16_BS_DeviceTable;
extern struct BLOCK_CONFIG        g_S29GL064A_16_BS_Config;

BlockStorageDevice  * pBlockStorageDevice = &g_S29GL064A_16_BS;
BLOCK_CONFIG        * pBLOCK_CONFIG         = &g_S29GL064A_16_BS_Config;

#else
extern struct BlockStorageDevice  g_S29GL032A_16_BS;
extern struct IBlockStorageDevice g_AM29DL_16_BS_DeviceTable;
extern struct BLOCK_CONFIG        g_S29GL032A_16_BS_Config;

BlockStorageDevice  * pBlockStorageDevice = &g_S29GL032A_16_BS;
BLOCK_CONFIG        * pBLOCK_CONFIG         = &g_S29GL032A_16_BS_Config;

#endif


HAL_DECLARE_NULL_HEAP();

//--//

void KeyPad_Test()
{
    NATIVE_PROFILE_PAL_BUTTONS();
    int return_value;
    
    hal_printf("\r\n\nKeyscan Test (press 'Stop' to exit)\r\n");
    
    g_GPIO_KEYPAD_Driver.KeyPad_Initialize();
        
    /* start keyscan */
    /* NOTE: enter "Stop" to exit the loop */
    while (1)
    {
        return_value = g_GPIO_KEYPAD_Driver.KeyPad_Search();
        
        if ( return_value != -1 ) 
        {           
            switch ( return_value ) 
            {   
                case 1:
                    hal_printf("UP\r\n");
                    break;
                case 2:
                    hal_printf("DOWN\r\n");
                    break;
                case 3:
                    hal_printf("LEFT\r\n");
                    break;
                case 4:
                    hal_printf("RIGHT\r\n");
                    break;
                case 5:
                    hal_printf("OK\r\n");
                    break;
                case 6:
                    hal_printf("Fn\r\n");
                    break;
                case 7:
                    hal_printf("C\r\n");
                    break;
                case 8:
                    hal_printf("Dial\r\n");
                    break;
                case 9:
                    hal_printf("Hang Up\r\n");
                    break;
                case 10:
                    hal_printf("1\r\n");                    
                    break;
                case 11:
                    hal_printf("2\r\n");
                    break;
                case 12:
                    hal_printf("3\r\n");
                    break;
                case 13:
                    hal_printf("4\r\n");
                    break;
                case 14:
                    hal_printf("5\r\n");
                    break;
                case 15:
                    hal_printf("6\r\n");
                    break;
                case 16:
                    hal_printf("7\r\n");
                    break;
                case 17:
                    hal_printf("8\r\n");
                    break;
                case 18:
                    hal_printf("9\r\n");
                    break;
                case 19:
                    hal_printf("*\r\n");
                    break;
                case 20:
                    hal_printf("0\r\n");
                    break;
                case 21:
                    hal_printf("#\r\n");
                    break;
                case 22:
                    hal_printf("Prev\r\n");
                    break;
                case 23:
                    hal_printf("Stop\r\n");
                    hal_printf("Exit Keypad Test");
                    return;                 
                    break;
                case 24:
                    hal_printf("Play\r\n");
                    break;
                case 25:
                    hal_printf("Next\r\n");
                    break;                  
            }       
        }
        else
        {
            //hal_printf("ERROR!!\r\n");
        }  
    }
}



//--//
/*
long Char_To_Num(char c)
{
    switch(c)
    {
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        default:
            return -1;
    }
}

BOOL String_To_Num(char* str, int len, unsigned long *num)
{
    int i;
    unsigned long tmp;
    long val;
    
    tmp = 0;
    for(i = 0; i < len; i++)
    {
        if((val = Char_To_Num(str[i])) < 0)
            return FALSE;
        tmp *= 10;
        tmp += (unsigned long)val;
    }
    
    *num = tmp;
    
    return TRUE;
}

unsigned long Is_IP_Address(char ip[15], int length)
{
    int i, j, pos;
    char ip_part[4][15];
    int len[4];
    unsigned long long_ip, tmp;
    
    if(length > 15 || length < 7)
        return 0;
        
    for(i = 0; i < length; i++)
        if((ip[i] < '0' || ip[i] > '9') && ip[i] != '.')
            break;
    if(i < length)
        return 0;

    pos = 0;
    i = 0;
    while(pos < length)
    {
        if(i >= 4) 
            return 0;
            
        for(j = 0; j < 3; j++)
        {
            if((ip[pos] != '.') && (ip[pos] != '\0'))
                ip_part[i][j] = ip[pos];
            else
                break;
            pos++;
        }
        
        if((ip[pos] != '.') && (ip[pos] != '\0'))
            return 0;
        len[i] = j;
        ip_part[i][j] = '\0';
        pos++;
        i++;
    }
    
    long_ip = 0;
    for(i = 0; i < 4; i++)
    {
        if(String_To_Num(ip_part[i], len[i], &tmp) == FALSE)
            return 0;
        if( tmp < 0 || tmp > 255)
            return 0;
        long_ip += (tmp << (8 * (3 - i)));      
    }
    return long_ip;
}

void Get_IP_String(char ip[15], int *length)
{
    UINT32 Events;
    
    char c[1];
    char* Data = ip;
    int len = 0, ret;
    
    c[1] = '\0';
    
    while(1)
    {
        Events = Events_WaitForEvents( SYSTEM_EVENT_FLAG_COM_IN | SYSTEM_EVENT_FLAG_ETHER, 50 );
        
        if(Events & SYSTEM_EVENT_FLAG_ETHER)            
        {           
            Events_Clear( SYSTEM_EVENT_FLAG_ETHER );
            Network_Interface_Process_Packet();
        }

        if(Events & SYSTEM_EVENT_FLAG_COM_IN)
        {
            Events_Clear( SYSTEM_EVENT_FLAG_COM_IN );
            while((ret = DebuggerPort_Read( HalSystemConfig.stdio, c, 1 )) > 0)
            {
                if(c[0] == 0xD)             //0xD: ENTER key
                {
                    Data[len] = '\0';
                    *length = len;
                    return;
                }
                else if(len < 15)
                {
                    Data[len++] = c[0];
                    hal_printf( c );
                }
            }
        }
    }
}

int Ether_Ping(unsigned long dest_ip, int n)
{
    int ret=-1;
    long i;
    UINT32 Events;
    
    hal_printf( "\r\nPinging %d.%d.%d.%d with 32 bytes of data:\r\n\n",
                    0x000000FF & (dest_ip >> 24), 0x000000FF & (dest_ip >> 16),
                    0x000000FF & (dest_ip >> 8), 0x000000FF & dest_ip);
    
    while( n )
    {
        if( (ret = Network_Interface_ARP_Request(dest_ip)) == 0 )
        {
            hal_printf( "Hardware error.\r\n" );
        }
        else
        {
            i = 100;
            while( i-- )
            {
                Events = Events_WaitForEvents( SYSTEM_EVENT_FLAG_ETHER, 50 );
                if(Events & SYSTEM_EVENT_FLAG_ETHER)
                {
                    Network_Interface_Process_Packet();
                    Events_Clear( SYSTEM_EVENT_FLAG_ETHER );
                    if(Network_Interface_ARP_Ack() == TRUE)
                    {
                        break;
                    }
                }
            }
            
            if( i <= 0 )
            {
                hal_printf( "Request timed out.\r\n" );
            }
            else
                break;
            n--;
        }
    }
    
    if( n <= 0 )
        return 0;

    while( n )
    {
        Network_Interface_ICMP_Echo(dest_ip);
        
        i = 100;
        while( i-- )
        {
            Events = Events_WaitForEvents( SYSTEM_EVENT_FLAG_ETHER, 50 );
            if(Events & SYSTEM_EVENT_FLAG_ETHER)
            {
                Network_Interface_Process_Packet();
                Events_Clear( SYSTEM_EVENT_FLAG_ETHER );                
                if(Network_Interface_ICMP_Ack() == TRUE)
                {
                    break;
                }
            }
        }
        
        if (i <= 0)
        {
            hal_printf( "Request timed out.\r\n" );
        }
        else
        {
            hal_printf( "Reply from %d.%d.%d.%d: bytes=32 TTL=128\r\n", 
                        0x000000FF & (dest_ip >> 24), 0x000000FF & (dest_ip >> 16),
                        0x000000FF & (dest_ip >> 8), 0x000000FF & dest_ip);
            for(i = 0; i < 5000000; i++);       //Delay
        }
        n--;
    }
    
    return 1;
}*/

void ApplicationEntryPoint()
{
    char Data[1], previousChar = '\0';
    int ret = 0, TestItem = 0;
    BOOL validCommand = TRUE;
	
	MEMORY_MAPPED_NOR_BLOCK_CONFIG* config;
	
//    BlockStorageDevice *device;
//    ByteAddress datByteAddress;

//    BlockStorageList::FindDeviceForPhysicalAddress( &device, 0, datByteAddress );
//    const BlockDeviceInfo * deviceInfo=device->GetDeviceInfo();


    
    Data[1] = '\0';
    char ip[15];
    int length;
    unsigned long dest_ip=0;
    UINT32 sleep = 100;

//    TimedEvents eventsTest;
 //   UART        usartTest  ( COMTestPort, 9600, USART_PARITY_NONE, 8, USART_STOP_BITS_ONE, USART_FLOW_NONE );
//    GPIO        gpioTest   ( GPIOTestPin );

    do
    {
        lcd_printf( "\n\n\n\n\n\n\n\n   SH7619 EVB\n" );
        lcd_printf( " Renesas America       Inc." );
                
        //Serial TEST
        hal_printf( "\r\nSH7619 EVB .NET Micro Framework NativeSample\r\n" );
        hal_printf( "Renesas Electronics America Inc.\r\n" );
        hal_printf( "61 MHz Clk,Big-endian\r\n" );      
        hal_printf( ">" );
        
        NATIVE_PROFILE_PAL_EVENTS();
        do
        {
            UINT32 Events = Events_WaitForEvents( SYSTEM_EVENT_FLAG_COM_IN | SYSTEM_EVENT_FLAG_ETHER, sleep );

            // if(Events == 0)
            // {
                // hal_printf("Slept %d sec\r\n", sleep / 1000);
                // sleep += 1000; 
                // continue;
            // }
            
            if(Events & SYSTEM_EVENT_FLAG_ETHER)            
            {
                //Network_Interface_Process_Packet();
				//hal_printf("ETHER event\r\n");
                Events_Clear( SYSTEM_EVENT_FLAG_ETHER );
            }
                        
            if(Events & SYSTEM_EVENT_FLAG_COM_IN)           
            {
                Events_Clear( SYSTEM_EVENT_FLAG_COM_IN );
                while((ret = DebuggerPort_Read( HalSystemConfig.DebugTextPort, Data, 1 )) > 0)
                {                   
                    switch(TestItem)
                    {
                        case 0:     //None
                            if(Data[0] == 0xD)              //0xD: ENTER key
                            {                               
                                hal_printf( "\r\n" );
                                if(validCommand)
                                {
                                    TestItem = 1;           //Enter test
                                    validCommand = FALSE;
                                    previousChar = '\0';
                                    hal_printf( "\r\nTest Menu:\r\n" );
                                    hal_printf( "   [L]CD Tests\r\n" );
                                    hal_printf( "   [K]eypad Tests\r\n" );
                                    hal_printf( "   [N]etwork Tests\r\n" );
                                    hal_printf( "   [F]lash memory Tests\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
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
                                        previousChar = 'I';     //Invalid character input
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
                                        previousChar = 'I';     //Invalid character input
                                    }
                                }
                                else
                                {
                                    validCommand = FALSE;
                                    previousChar = 'I';         //Invalid character input
                                }
                            }
                            break;
                        case 1:     //Enter test
                            hal_printf( Data );
                            switch(Data[0])
                            {
                                case 'X':
                                case 'x':
                                    TestItem = 0;
                                    hal_printf( "\r\n\n>" );
                                    break;
                                case 'L':
                                case 'l':
                                    TestItem = 5;
                                    hal_printf( "\r\n\nLCD Menu:\r\n" );
                                    hal_printf( "   1: Display colorful rectangles\r\n" );
                                    hal_printf( "   2: Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: Colorful solid rectangle\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case 'K':
                                case 'k':
                                    KeyPad_Test();                                  
                                    break;
                                case 'N':
                                case 'n':
                                    TestItem = 7;
                                    hal_printf( "\r\n\nEthernet Menu:\r\n" );
                                    hal_printf( "   [1]Read IP Address\r\n" );
                                    hal_printf( "   [2]Read EtherC Registers\r\n" );
                                    hal_printf( "   [3]Read E-DMAC Registers\r\n" );
                                    hal_printf( "   [4]Ping Test\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;                              
                                case 'F':
                                case 'f':
                                    TestItem = 8;
                                    hal_printf( "\r\n\nFlash memory Menu:\r\n" );
                                    hal_printf( "   [1]Erase block\r\n" );
                                    hal_printf( "   [2]write block\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;                              
                                default:
                                    hal_printf( "\r\n\nTest Menu:\r\n" );
                                    hal_printf( "   [L]CD Tests\r\n" );
                                    hal_printf( "   [K]eypad Tests\r\n" );
                                    hal_printf( "   [N]etwork Tests\r\n" );
                                    hal_printf( "   [F]lash memory Tests\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                            }
                            break;
                        case 5:     //LCD test
                            hal_printf( Data );
                            switch(Data[0])
                            {
                                case 'X':
                                case 'x':
                                    TestItem = 1;
                                    hal_printf( "\r\n\nTest Menu:\r\n" );
                                    hal_printf( "   [L]CD Tests\r\n" );
                                    hal_printf( "   [K]eypad Tests\r\n" );
                                    hal_printf( "   [N]etwork Tests\r\n" );
                                    hal_printf( "   [F]lash memory Tests\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '1':
                                    LCDTest_Driver::LCD_Sample1();
                                    hal_printf( "\r\n\nLCD Menu:\r\n" );
                                    hal_printf( "   1: Display colorful rectangles\r\n" );
                                    hal_printf( "   2: Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: Colorful solid rectangle\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '2':
                                    LCDTest_Driver::LCD_Sample2(0);
                                    hal_printf( "\r\n\nLCD Menu:\r\n" );
                                    hal_printf( "   1: Display colorful rectangles\r\n" );
                                    hal_printf( "   2: Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: Colorful solid rectangle\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '3':
                                    LCDTest_Driver::LCD_Sample2(1);
                                    hal_printf( "\r\n\nLCD Menu:\r\n" );
                                    hal_printf( "   1: Display colorful rectangles\r\n" );
                                    hal_printf( "   2: Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: Colorful solid rectangle\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                default:
                                    hal_printf( "\r\n\nLCD Menu:\r\n" );
                                    hal_printf( "   1: Display colorful rectangles\r\n" );
                                    hal_printf( "   2: Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: Colorful solid rectangle\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                            }
                            break;
                        case 7:     //Ethernet test
                            hal_printf( Data );
                            hal_printf( "\r\n" );
                            switch(Data[0])
                            {
                                case 'X':
                                case 'x':
                                    TestItem = 1;
                                    hal_printf( "\r\n\nTest Menu:\r\n" );
                                    hal_printf( "   [L]CD Tests\r\n" );
                                    hal_printf( "   [K]eypad Tests\r\n" );
                                    hal_printf( "   [N]etwork Tests\r\n" );
                                    hal_printf( "   [F]lash memory Tests\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '1':
                                    //Network_Interface_IP_Address();                                 
                                    hal_printf( "\r\n\nEthernet Menu:\r\n" );
                                    hal_printf( "   [1]Read IP Address\r\n" );
                                    hal_printf( "   [2]Read EtherC Registers\r\n" );
                                    hal_printf( "   [3]Read E-DMAC Registers\r\n" );
                                    hal_printf( "   [4]Ping Test\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '2':
                                    //Network_Interface_EtherC_Registers();
                                    hal_printf( "\r\n\nEthernet Menu:\r\n" );
                                    hal_printf( "   [1]Read IP Address\r\n" );
                                    hal_printf( "   [2]Read EtherC Registers\r\n" );
                                    hal_printf( "   [3]Read E-DMAC Registers\r\n" );
                                    hal_printf( "   [4]Ping Test\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '3':
                                    //Network_Interface_EDMAC_Registers();
                                    hal_printf( "\r\n\nEthernet Menu:\r\n" );
                                    hal_printf( "   [1]Read IP Address\r\n" );
                                    hal_printf( "   [2]Read EtherC Registers\r\n" );
                                    hal_printf( "   [3]Read E-DMAC Registers\r\n" );
                                    hal_printf( "   [4]Ping Test\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;                              
                                case '4':
                                    //do
                                    //{
                                    //    hal_printf( "\r\nEnter dest IP: " );
                                    //    Get_IP_String(ip, &length);
                                    //    if( (dest_ip = Is_IP_Address(ip, length)) == 0)
                                    //        hal_printf( "\r\n*** Invalid IP Address!\r\n" );
                                    //}while(dest_ip == 0);
                                    //Ether_Ping(dest_ip, 4); 
                                    hal_printf( "\r\n\nEthernet Menu:\r\n" );
                                    hal_printf( "   [1]Read IP Address\r\n" );
                                    hal_printf( "   [2]Read EtherC Registers\r\n" );
                                    hal_printf( "   [3]Read E-DMAC Registers\r\n" );
                                    hal_printf( "   [4]Ping Test\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;                          
                                default:
                                    hal_printf( "\r\n\nEthernet Menu:\r\n" );
                                    hal_printf( "   [1]Read IP Address\r\n" );
                                    hal_printf( "   [2]Read EtherC Registers\r\n" );
                                    hal_printf( "   [3]Read E-DMAC Registers\r\n" );
                                    hal_printf( "   [4]Ping Test\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                            }
                            break;
                        case 8:     //Flash memory test
                            hal_printf( Data );
                            hal_printf( "\r\n" );
                            switch(Data[0])
                            {
                                case 'X':
                                case 'x':
                                    TestItem = 1;
                                    hal_printf( "\r\n\nTest Menu:\r\n" );
                                    hal_printf( "   [L]CD Tests\r\n" );
                                    hal_printf( "   [K]eypad Tests\r\n" );
                                    hal_printf( "   [N]etwork Tests\r\n" );
                                    hal_printf( "   [F]lash memory Tests\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '1':
									g_AM29DL_16_BS_DeviceTable.InitializeDevice(pBLOCK_CONFIG);
									g_AM29DL_16_BS_DeviceTable.EraseBlock(pBLOCK_CONFIG,0xa0080000);
                                    hal_printf( "\r\n\nFlash memory Menu:\r\n" );
                                    hal_printf( "   [1]Erase block\r\n" );
                                    hal_printf( "   [2]write block\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '2':
									BYTE Buff[0x10];
									for(int i=0; i<0x10; i++) Buff[i]=i;
									g_AM29DL_16_BS_DeviceTable.InitializeDevice(pBLOCK_CONFIG);
									g_AM29DL_16_BS_DeviceTable.Write(pBLOCK_CONFIG,0xa0080000,0x10,Buff,0x0);
                                    hal_printf( "\r\n\nFlash memory Menu:\r\n" );
                                    hal_printf( "   [1]Erase block\r\n" );
                                    hal_printf( "   [2]write block\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                default:
                                    hal_printf( "\r\n\nFlash memory Menu:\r\n" );
                                    hal_printf( "   [1]Erase block\r\n" );
                                    hal_printf( "   [2]write block\r\n" );
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
