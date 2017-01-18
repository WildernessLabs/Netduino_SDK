//-----------------------------------------------------------------------------
// Software that is described herein is for illustrative purposes only  
// which provides customers with programming information regarding the  
// products. This software is supplied "AS IS" without any warranties.  
// NXP Semiconductors assumes no responsibility or liability for the 
// use of the software, conveys no license or title under any patent, 
// copyright, or mask work right to the product. NXP Semiconductors 
// reserves the right to make changes in the software without 
// notification. NXP Semiconductors also make no representation or 
// warranty that such application will be suitable for the specified 
// use without further testing or modification. 
//-----------------------------------------------------------------------------

#ifndef _LPC22XX_H_
#define _LPC22XX_H_ 1

#include <cores\arm\include\cpu.h>


extern "C"
{
    void BootstrapCode_Clocks();
    void BootstrapCode_Ext_SRAM();
    void BootstrapCode_Pins();
    void BootstrapCode_Ext_Flash();

}

extern    void Initialize_SyncOn();


struct I2C_CONFIGURATION;

//////////////////////////////////////////////////////////////////////////////
// Interrupt Controller
//
struct LPC22XX_VIC
{
    static const UINT32 c_Base = 0xFFFFF000;

    //--//
    // Priority is not implemented
    //static const UINT32 c_IRQ_Priority_0  =  0;
    //static const UINT32 c_IRQ_Priority_1  =  1;
    //static const UINT32 c_IRQ_Priority_2  =  2;
    //static const UINT32 c_IRQ_Priority_3  =  3;
    //static const UINT32 c_IRQ_Priority_4  =  4;
    //static const UINT32 c_IRQ_Priority_5  =  5;
    //static const UINT32 c_IRQ_Priority_6  =  6;
    //static const UINT32 c_IRQ_Priority_7  =  7;
    //static const UINT32 c_IRQ_Priority_8  =  8;
    //static const UINT32 c_IRQ_Priority_9  =  9;
    //static const UINT32 c_IRQ_Priority_10 = 10;
    //static const UINT32 c_IRQ_Priority_11 = 11;
    //static const UINT32 c_IRQ_Priority_12 = 12;
    //static const UINT32 c_IRQ_Priority_13 = 13;
    //static const UINT32 c_IRQ_Priority_14 = 14;
    //static const UINT32 c_IRQ_Priority_15 = 15;

    static const UINT32 c_IRQ_INDEX_WDT              =  0;
    static const UINT32 c_IRQ_INDEX_SW               =  1;
    static const UINT32 c_IRQ_INDEX_DBG_COM_RX       =  2;
    static const UINT32 c_IRQ_INDEX_DBG_COM_TX       =  3;
    static const UINT32 c_IRQ_INDEX_TIMER0           =  4;
    static const UINT32 c_IRQ_INDEX_TIMER1           =  5;
    static const UINT32 c_IRQ_INDEX_UART0            =  6;
    static const UINT32 c_IRQ_INDEX_UART1            =  7;
    static const UINT32 c_IRQ_INDEX_PWM0             =  8;
    static const UINT32 c_IRQ_INDEX_I2C              =  9;
    static const UINT32 c_IRQ_INDEX_SPI0             = 10;
    static const UINT32 c_IRQ_INDEX_SPI1             = 11;
    static const UINT32 c_IRQ_INDEX_PLL              = 12;
    static const UINT32 c_IRQ_INDEX_RTC              = 13;
    static const UINT32 c_IRQ_INDEX_EINT0            = 14;
    static const UINT32 c_IRQ_INDEX_EINT1            = 15;
    static const UINT32 c_IRQ_INDEX_EINT2            = 16;
    static const UINT32 c_IRQ_INDEX_EINT3            = 17;
    static const UINT32 c_IRQ_INDEX_ADC              = 18;
    static const UINT32 c_IRQ_INDEX_unused19         = 19;
    static const UINT32 c_IRQ_INDEX_unused20         = 20;
    static const UINT32 c_IRQ_INDEX_unused21         = 21;
    static const UINT32 c_IRQ_INDEX_unused22         = 22;
    static const UINT32 c_IRQ_INDEX_unused23         = 23;
    static const UINT32 c_IRQ_INDEX_unused24         = 24;
    static const UINT32 c_IRQ_INDEX_unused25         = 25;
    static const UINT32 c_IRQ_INDEX_unused26         = 26;
    static const UINT32 c_IRQ_INDEX_unused27         = 27;
    static const UINT32 c_IRQ_INDEX_unused28         = 28;
    static const UINT32 c_IRQ_INDEX_unused29         = 29;
    static const UINT32 c_IRQ_INDEX_unused30         = 30;
    static const UINT32 c_IRQ_INDEX_unused31         = 31;
    //--//

    /****/ volatile UINT32 IRQSTATUS;   // Status of enabled IRQ requests
    /****/ volatile UINT32 FIQSTATUS;   // Status of enabled FIQ requests
    /****/ volatile UINT32 RAWINTR;     // Status of interrupt lines irrespective of the classificatino/enabling
    /****/ volatile UINT32 INTRSEL;     // FIQ or IRQ
    /****/ volatile UINT32 INTENABLE;   // Enable interrupt line
    /****/ volatile UINT32 INTENCLR;    // Disable interrupt line
    /****/ volatile UINT32 SOFTINT;     // Force SW interrupt
    /****/ volatile UINT32 SOFTINTCLR;  // Clear SW interrupt
    /****/ volatile UINT32 PROTECTEN;   // 
	/****/ volatile UINT32 Padding0[3]; // Fill address gap
    /****/ volatile UINT32 VECTADDR;    // Address of active vectored interrupt handler
    /****/ volatile UINT32 DEFADDR;     // Address of non vectored interrupt handler

// Vectored interrupts are not used since a wrapper is anyway required to pass parameters
// to the active interrupt handler.
// !!To Do Use vic address registers to identify the interrupt source instead of scanning through the 
// interrupt status register

     volatile UINT32 Padding1[51];

     volatile UINT32 VECTADR0[15];  // Address of vectored interrupt handler for slot 0

    // volatile UINT32 VECTADR1;  // Address of vectored interrupt handler for slot 1
    // volatile UINT32 VECTADR2;  // Address of vectored interrupt handler for slot 2
    // volatile UINT32 VECTADR3;  // Address of vectored interrupt handler for slot 3
    // volatile UINT32 VECTADR4;  // Address of vectored interrupt handler for slot 4
    // volatile UINT32 VECTADR5;  // Address of vectored interrupt handler for slot 5
    // volatile UINT32 VECTADR6;  // Address of vectored interrupt handler for slot 6
    // volatile UINT32 VECTADR7;  // Address of vectored interrupt handler for slot 7
    // volatile UINT32 VECTADR8;  // Address of vectored interrupt handler for slot 8
    // volatile UINT32 VECTADR9;  // Address of vectored interrupt handler for slot 9
    // volatile UINT32 VECTADR10; // Address of vectored interrupt handler for slot 10
    // volatile UINT32 VECTADR11; // Address of vectored interrupt handler for slot 11
    // volatile UINT32 VECTADR12; // Address of vectored interrupt handler for slot 12
    // volatile UINT32 VECTADR13; // Address of vectored interrupt handler for slot 13
    // volatile UINT32 VECTADR14; // Address of vectored interrupt handler for slot 14
    // volatile UINT32 VECTADR15; // Address of vectored interrupt handler for slot 15

     volatile UINT32 Padding2[49];

     volatile UINT32 VECTCNTL0[15];  // Control register for slot 0
    // volatile UINT32 VECTCNTL1;  // Control register for slot 1
    // volatile UINT32 VECTCNTL2;  // Control register for slot 2
    // volatile UINT32 VECTCNTL3;  // Control register for slot 3
    // volatile UINT32 VECTCNTL4;  // Control register for slot 4
    // volatile UINT32 VECTCNTL5;  // Control register for slot 5
    // volatile UINT32 VECTCNTL6;  // Control register for slot 6
    // volatile UINT32 VECTCNTL7;  // Control register for slot 7
    // volatile UINT32 VECTCNTL8;  // Control register for slot 8
    // volatile UINT32 VECTCNTL9;  // Control register for slot 9
    // volatile UINT32 VECTCNTL10; // Control register for slot 10
    // volatile UINT32 VECTCNTL11; // Control register for slot 11
    // volatile UINT32 VECTCNTL12; // Control register for slot 12
    // volatile UINT32 VECTCNTL13; // Control register for slot 13
    // volatile UINT32 VECTCNTL14; // Control register for slot 14
    // volatile UINT32 VECTCNTL15; // Control register for slot 15

    //--//

    static const UINT32 c_MaxInterruptIndex = 32;

    //--//

    BOOL IsInterruptEnabled( UINT32 Irq_Index )
    {
        ASSERT(Irq_Index < 32);

        return (INTENABLE >> Irq_Index) & 0x1;
    }

    BOOL GetInterruptState( UINT32 Irq_Index )
    {
        ASSERT(Irq_Index < 32);
        // Make changes to include the FIQSTATUS if the driver is modifed to support FIQ
        return (IRQSTATUS >> Irq_Index) & 0x1;
    }

    UINT32 NormalInterruptPending()
    {
        UINT32 index;

        for( index = 0; IRQSTATUS && (index < c_MaxInterruptIndex); index++)
        {
            if( (IRQSTATUS >> index) & 0x1 )
            {
                return index;
            }
        }

        return c_MaxInterruptIndex;
    }

    BOOL IsInterruptPending()
    {
        if((NormalInterruptPending() != c_MaxInterruptIndex))
        {
            return TRUE;
        }

        return FALSE;
    }

    void ForceInterrupt( UINT32 Irq_Index )
    {
        SOFTINT |= 1 << Irq_Index;
    }

    void RemoveForcedInterrupt( UINT32 Irq_Index )
    {
        SOFTINTCLR |= 1 << Irq_Index;
    }
};
//
// Interrupt Controller

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// TIMERS
//
struct LPC22XX_TIMER
{
    static const UINT32 c_Base_0 = 0xE0004000;
    static const UINT32 c_Base_1 = 0xE0008000;


    /****/ volatile UINT32 IR;     // Interrupt register
           static const    UINT32 MR0_COMP                   = 0x00000001;
           static const    UINT32 MR0_RESET                  = 0x00000001;

    /****/ volatile UINT32 TCR;    // Timer control register
           static const    UINT32 TCR_TEN                    = 0x00000001;

    /****/ volatile UINT32 TC;     // Timer counter

    /****/ volatile UINT32 PR;     // Pre scale register

    /****/ volatile UINT32 PC;     // Pre scale counter register

    /****/ volatile UINT32 MCR;    // Match control register

    /****/ volatile UINT32 MR0;    // Match 0 register
           static const    UINT32 MR0_IRQEN                  = 0x00000001;
    /****/ volatile UINT32 MR1;    // Match 1 register

    /****/ volatile UINT32 MR2;    // Match 2 register

    /****/ volatile UINT32 MR3;    // Match 3 register

    /****/ volatile UINT32 CCR;    // Capture control register

    /****/ volatile UINT32 CR0;    // Capture 0 register

    /****/ volatile UINT32 CR1;    // Capture 1 register

    /****/ volatile UINT32 CR2;    // Capture 2 register

    /****/ volatile UINT32 CR3;    // Capture 3 register

    /****/ volatile UINT32 EMR;    // External match register
};

//
// TIMER
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// GP I/O
//
struct LPC22XX_GPIO
{
    static const UINT32 c_GPIO_Base = 0xE0028000;

    //--//

    static const UINT32 c_Pin_None  = 0xFFFFFFFF;
    static const UINT32 c_P0_00 =   0; // TxD0     / PWM1     / Reserved
    static const UINT32 c_P0_01 =   1; // RxD0     / PWM3     / EINT0
    static const UINT32 c_P0_02 =   2; // SCL      / CAP0.0   / Reserved
    static const UINT32 c_P0_03 =   3; // SDA      / MAT0.0   / EINT1
    static const UINT32 c_P0_04 =   4; // SCK0     / CAP0.1   / Reserved
    static const UINT32 c_P0_05 =   5; // MISO0    / MAT0.1   / Reserved
    static const UINT32 c_P0_06 =   6; // MOSI0    / CAP0.1   / Reserved
    static const UINT32 c_P0_07 =   7; // SSEL0    / PWM2     / EINT2
    static const UINT32 c_P0_08 =   8; // TxD1     / PWM4     / Reserved
    static const UINT32 c_P0_09 =   9; // RxD1     / PWM6     / EINT3
    static const UINT32 c_P0_10 =  10; // RTS1     / CAPT1.0  / Reserved
    static const UINT32 c_P0_11 =  11; // CTS1     / CAPT1.1  / Reserved
    static const UINT32 c_P0_12 =  12; // DSR1     / MAT1.0   / Reserved
    static const UINT32 c_P0_13 =  13; // DTR1     / MAT1.1   / Reserved
    static const UINT32 c_P0_14 =  14; // CD1      / EINT1    / Reserved
    static const UINT32 c_P0_15 =  15; // RI1      / EINT2    / Reserved
    static const UINT32 c_P0_16 =  16; // EINT0    / MAT0.2   / CAP0.2
    static const UINT32 c_P0_17 =  17; // CAP1.2   / SCK1     / MAT1.2
    static const UINT32 c_P0_18 =  18; // CAP1.3   / MISO1    / MAT1.3
    static const UINT32 c_P0_19 =  19; // MAT1.2   / MOSI1    / MAT1.3
    static const UINT32 c_P0_20 =  20; // MAT1.3   / SSEL1    / EINT3
    static const UINT32 c_P0_21 =  21; // PWM5     / Reserved / CAP1.3
    static const UINT32 c_P0_22 =  22; // Reserved / CAPT0.0  / MAT0.0
    static const UINT32 c_P0_23 =  23; // RD2      / Reserved / Reserved
    static const UINT32 c_P0_24 =  24; // TD2      / Reserved / Reserved
    static const UINT32 c_P0_25 =  25; // RD1      / Reserved / Reserved
// Reserved    static const UINT32 c_P0_26 =  26;
    static const UINT32 c_P0_27 =  27; // AIN0     / CAPT0.1  / MAT0.1
    static const UINT32 c_P0_28 =  28; // AIN1     / CAPT0.2  / MAT0.2
    static const UINT32 c_P0_29 =  29; // AIN2     / CAPT0.3  / MAT0.3
    static const UINT32 c_P0_30 =  30; // AIN3     / EINT3    / CAPT0.0
//  Reserved    static const UINT32 c_P0_31 =  31;
//  Reserved    static const UINT32 c_P1_00 =  32;
//  Reserved    static const UINT32 c_P1_01 =  33;
//  Reserved    static const UINT32 c_P1_02 =  34;
//  Reserved    static const UINT32 c_P1_03 =  35;
//  Reserved    static const UINT32 c_P1_04 =  36;
//  Reserved    static const UINT32 c_P1_05 =  37;
//  Reserved    static const UINT32 c_P1_06 =  38;
//  Reserved    static const UINT32 c_P1_07 =  39;
//  Reserved    static const UINT32 c_P1_08 =  40;
//  Reserved    static const UINT32 c_P1_09 =  41;
//  Reserved    static const UINT32 c_P1_10 =  42;
//  Reserved    static const UINT32 c_P1_11 =  43;
//  Reserved    static const UINT32 c_P1_12 =  44;
//  Reserved    static const UINT32 c_P1_13 =  45;
//  Reserved    static const UINT32 c_P1_14 =  46;
//  Reserved    static const UINT32 c_P1_15 =  47;
// TODO Add support for these pins later
/*    static const UINT32 c_P1_16 =  48;
    static const UINT32 c_P1_17 =  49;
    static const UINT32 c_P1_18 =  50;
    static const UINT32 c_P1_19 =  51;
    static const UINT32 c_P1_20 =  52;
    static const UINT32 c_P1_21 =  53;
    static const UINT32 c_P1_22 =  54;
    static const UINT32 c_P1_23 =  55;
    static const UINT32 c_P1_24 =  56;
    static const UINT32 c_P1_25 =  57;
    static const UINT32 c_P1_26 =  58;
    static const UINT32 c_P1_27 =  59;
    static const UINT32 c_P1_28 =  60;
    static const UINT32 c_P1_29 =  61;
    static const UINT32 c_P1_30 =  62;
    static const UINT32 c_P1_31 =  63;
    static const UINT32 c_P2_00 =  64;
    static const UINT32 c_P2_01 =  65;
    static const UINT32 c_P2_02 =  66;
    static const UINT32 c_P2_03 =  67;
    static const UINT32 c_P2_04 =  68;
    static const UINT32 c_P2_05 =  69;
    static const UINT32 c_P2_06 =  70;
    static const UINT32 c_P2_07 =  71;
    static const UINT32 c_P2_08 =  72;
    static const UINT32 c_P2_09 =  73;
    static const UINT32 c_P2_10 =  74;
    static const UINT32 c_P2_11 =  75;
    static const UINT32 c_P2_12 =  76;
    static const UINT32 c_P2_13 =  77;
    static const UINT32 c_P2_14 =  78;
    static const UINT32 c_P2_15 =  79;
    static const UINT32 c_P2_16 =  80;
    static const UINT32 c_P2_17 =  81;
    static const UINT32 c_P2_18 =  82;
    static const UINT32 c_P2_19 =  83;
    static const UINT32 c_P2_20 =  84;
    static const UINT32 c_P2_21 =  85;
    static const UINT32 c_P2_22 =  86;
    static const UINT32 c_P2_23 =  87;
    static const UINT32 c_P2_24 =  88;
    static const UINT32 c_P2_25 =  89;
    static const UINT32 c_P2_26 =  90;
    static const UINT32 c_P2_27 =  91;
    static const UINT32 c_P2_28 =  92;
    static const UINT32 c_P2_29 =  93;
    static const UINT32 c_P2_30 =  94;
    static const UINT32 c_P2_31 =  95;
    static const UINT32 c_P3_00 =  96;
    static const UINT32 c_P3_01 =  97;
    static const UINT32 c_P3_02 =  98;
    static const UINT32 c_P3_03 =  99;
    static const UINT32 c_P3_04 = 100;
    static const UINT32 c_P3_05 = 101;
    static const UINT32 c_P3_06 = 102;
    static const UINT32 c_P3_07 = 103;
    static const UINT32 c_P3_08 = 104;
    static const UINT32 c_P3_09 = 105;
    static const UINT32 c_P3_10 = 106;
    static const UINT32 c_P3_11 = 107;
    static const UINT32 c_P3_12 = 108;
    static const UINT32 c_P3_13 = 109;
    static const UINT32 c_P3_14 = 110;
    static const UINT32 c_P3_15 = 111;
    static const UINT32 c_P3_16 = 112;
    static const UINT32 c_P3_17 = 113;
    static const UINT32 c_P3_18 = 114;
    static const UINT32 c_P3_19 = 115;
    static const UINT32 c_P3_20 = 116;
    static const UINT32 c_P3_21 = 117;
    static const UINT32 c_P3_22 = 118;
    static const UINT32 c_P3_23 = 119;
    static const UINT32 c_P3_24 = 120;
    static const UINT32 c_P3_25 = 121;
    static const UINT32 c_P3_26 = 122;
    static const UINT32 c_P3_27 = 123;
    static const UINT32 c_P3_28 = 124;
    static const UINT32 c_P3_29 = 125;
    static const UINT32 c_P3_30 = 126;
    static const UINT32 c_P3_31 = 127;
*/
    //--//

    struct 
    {
        /****/ volatile UINT32 IOPIN_PX;            // Pin Value Register
        /****/ volatile UINT32 IOSET_PX;            // Direction Control
        /****/ volatile UINT32 IODIR_PX;            // Data Output Set Register
        /****/ volatile UINT32 IOCLR_PX;            // Data Output Clear Register
    } Regs[4];


    //--//

};
//
// GP I/O
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// UART
//
struct LPC22XX_USART
{
    static const UINT32 c_Base_0        = 0xE000C000;
    static const UINT32 c_Base_1        = 0xE0010000;

    static const UINT32 c_SER1_TXD = LPC22XX_GPIO::c_P0_00;
    static const UINT32 c_SER1_RXD = LPC22XX_GPIO::c_P0_01;

    // Add more pins if UART2 is used in full mode    
    static const UINT32 c_SER2_TXD = LPC22XX_GPIO::c_P0_08;
    static const UINT32 c_SER2_RXD = LPC22XX_GPIO::c_P0_09;
    static const UINT32 c_SER2_RTS = LPC22XX_GPIO::c_P0_10;
    static const UINT32 c_SER2_CTS = LPC22XX_GPIO::c_P0_11;

    static const UINT32 c_ClockRate     =   15000000;        

    static const UINT32 c_MAX_BAUDRATE = c_ClockRate/16; 
    static const UINT32 c_MIN_BAUDRATE = 0;


    //IER
    static const UINT32 UART_IER_RLSIE               = 0x00000004;   //Receive line status interrupt enable. 
    static const UINT32 UART_IER_THREIE              = 0x00000002;   //transmit hold register empty interrupt enable.
    static const UINT32 UART_IER_RDAIE               = 0x00000001;   //Receive data available interrupt enable.
            
    static const UINT32 UART_IER_INTR_ALL_SET        = UART_IER_RLSIE | UART_IER_THREIE | UART_IER_RDAIE;   
    
    //IID   
    static const UINT32 UART_IIR_FMES                = 0x00000080;   //FIFO MODE Enable status.
    static const UINT32 UART_IIR_RFTLS_mask          = 0x00000060;   //RX FIFO threshold level status.
    static const UINT32 UART_IIR_RFTLS_shift         =          5;
    static const UINT32 UART_IIR_IID_mask            = 0x0000000F;   //Interrupt identification.
    // values.            
    static const UINT32 UART_IIR_IID_Irpt_RLS        = 0x00000006;   // Receiver line status interrupt (e.g. overrun error) .
    static const UINT32 UART_IIR_IID_Irpt_RDA        = 0x00000004;   // Receive data ready interrupt.         
    static const UINT32 UART_IIR_IID_Irpt_TOUT       = 0x0000000C;   // Receive FIFO timeout interrupt.               
    static const UINT32 UART_IIR_IID_Irpt_THRE       = 0x00000002;   // Transmitter holding register empty.                       
      
    static const UINT32 UART_IIR_NIP                 = 0x00000001;   //There is no pending interrupt.

    //FCR
    static const UINT32 UART_FCR_RFITL_mask       = 0x000000C0;     // Rx FIFO trigger level
    static const UINT32 UART_FCR_RFITL_shift      =          6;
          
    static const UINT32 UART_FCR_RFITL_01         = 0x00000000;
    static const UINT32 UART_FCR_RFITL_04         = 0x00000001;
    static const UINT32 UART_FCR_RFITL_08         = 0x00000002;
    static const UINT32 UART_FCR_RFITL_14         = 0x00000003;
                                  
    static const UINT32 UART_FCR_TFR              = 0x00000004;     // Tx FIFO reset 
    static const UINT32 UART_FCR_RFR              = 0x00000002;     // Rx FIFO reset          
    static const UINT32 UART_FCR_FME              = 0x00000001;     // FIFO Mode enable                                 


    union 
    {
        struct
        {
            /****/ volatile UINT32 UART_RBR;                             //receive data register 
        } RBR;
        struct
        {
            /****/ volatile UINT32 UART_THR;                            //transmit data register.
            
        } THR; 
        struct
        {
            /****/ volatile UINT32 UART_DLL;                            //Divisor Latch register. (LSB)
        } DLL; 
        
    } SEL1;
    
    union
    {
        struct
        {
            /****/ volatile UINT32 UART_IER;                                //Interrupt enable register
        } IER;
        struct
        {
            /****/ volatile UINT32 UART_DLM;                               //Divisor Latch register.  (MSB)
        } DLM;
    } SEL2;

    union
    {
        struct 
        {
          /****/ volatile UINT32  UART_IIR;                                        //Interrupt identification register.
        } IIR;
        struct
        {
          /****/ volatile UINT32  UART_FCR;
        } FCR;
    } SEL3;
    
    /****/ volatile UINT32 UART_LCR;                                       // line control register.
    //--//  
    static const UINT32 UART_LCR_DLAB                   = 0x00000080;     // Dividor Latch Access bit.     
    static const UINT32 UART_LCR_BCB                    = 0x00000040;     // Break control bit.
    static const UINT32 UART_LCR_SPE                    = 0x00000020;     // Stick parity enable.
    static const UINT32 UART_LCR_EPE                    = 0x00000010;     // Even  parity enable.
    static const UINT32 UART_LCR_PBE                    = 0x00000008;     // Parity bit enable.
    static const UINT32 UART_LCR_NSB_1_STOPBITS         = 0x00000000;     // Number of stop bits (0 - 1 stop bit; 1 - 1.5 stop bits).
    static const UINT32 UART_LCR_NSB_15_STOPBITS        = 0x00000004;     // Number of stop bits (0 - 1 stop bit; 1 - 1.5 stop bits).
    static const UINT32 UART_LCR_WLS_mask               = 0x00000003;     // Word length select.
    static const UINT32 UART_LCR_WLS_shift              =          0;    
    static const UINT32 UART_LCR_WLS_5_BITS             = 0x00000000; 
    static const UINT32 UART_LCR_WLS_6_BITS             = 0x00000001; 
    static const UINT32 UART_LCR_WLS_7_BITS             = 0x00000002; 
    static const UINT32 UART_LCR_WLS_8_BITS             = 0x00000003;             

    /****/ volatile UINT32 UART_MCR;                                       // modem control register. 
        
    /****/ volatile UINT32 UART_LSR;                                       //line status register.
    static const UINT32 UART_LSR_ERR_RX                 = 0x00000080;     //Rx FIFO error 
    static const UINT32 UART_LSR_TE                     = 0x00000040;     //Transmitter Empty.
    static const UINT32 UART_LSR_THRE                   = 0x00000020;     //Transmitter Holding register Empty. 
    static const UINT32 UART_LSR_BII                    = 0x00000010;     //Break interrupt indicator.
    static const UINT32 UART_LSR_FEI                    = 0x00000008;     //Framing Error indicator.
    static const UINT32 UART_LSR_PEI                    = 0x00000004;     //Parity Error indicator.
    static const UINT32 UART_LSR_OEI                    = 0x00000002;     //Overrun Error indicator.
    static const UINT32 UART_LSR_RFDR                   = 0x00000001;     //RX FIFO data ready.
    
    /****/ volatile UINT32 UART_MSR;                                   //Modem status register.
        
    //functions.
    static UINT32 inline getIntNo(int ComPortNum)
    {
        return (LPC22XX_VIC::c_IRQ_INDEX_UART0+ComPortNum);
    }
};
//
// UART
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Pin Connect Block
//
struct LPC22XX_PCB
{
    static const UINT32 c_PCB_Base = 0xE002C000;
    
    struct 
    {
        /****/ volatile UINT32 PINSEL;   // Pin Configuration Register
    } Regs[2];
    
    /****/ volatile UINT32 dummy0[3];    // Filler to align next register address

    /****/ volatile UINT32 PINSEL2;
    
    static const UINT32 FUNC0 = 0x0;
    static const UINT32 FUNC1 = 0x1;
    static const UINT32 FUNC2 = 0x2;
    static const UINT32 FUNC3 = 0x3;
};
//
// Pin Connect Block
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// System Control Block
//
struct LPC22XX_SYSCON
{
    static const UINT32 c_SYSCON_Base = 0xE01FC040;
    
    // Memory Mapping Control
    /****/ volatile UINT32 MEMMAP;        // Memory map control register

    /****/ volatile UINT32 dummy0[15];    // Filler to align next register address

    // Phased Lock Loop
    /****/ volatile UINT32 PLLCON;        // PLL Control Register
    
    /****/ volatile UINT32 PLLCFG;        // PLL Configuration Register
    
    /****/ volatile UINT32 PLLSTAT;       // PLL Status Register

    /****/ volatile UINT32 PLLFEED;       // PLL Feed Register

    /****/ volatile UINT32 dummy1[13];    // Filler to align next register address

    // Power Control
    /****/ volatile UINT32 PCON;          // Power Control Register
    
    /****/ volatile UINT32 PCONP;         // Power Control for Peripherals

    /****/ volatile UINT32 dummy2[15];    // Filler to align next register address

    // VPB Divider
    /****/ volatile UINT32 VPBDIV;        // VPB Divider Control

    /****/ volatile UINT32 dummy3[16];    // Filler to align next register address

    // External Interrupts
    /****/ volatile UINT32 EXTINT;        // External Interrupt Flag Register
    
    /****/ volatile UINT32 EXTWAKE;       // External Interrupt Wakeup Register
    
    /****/ volatile UINT32 EXTMODE;       // External Interrupt Mode Register

    /****/ volatile UINT32 EXTPOLAR;      // External Interrupt Polarity Register
    
};
//
// System Control Block
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// External Memory Controller
//
struct LPC22XX_EMC
{
    static const UINT32 c_EMC_Base = 0xFFE00000;
    
    /****/ volatile UINT32 BCFG0;
    
    /****/ volatile UINT32 BCFG1;
    
    /****/ volatile UINT32 BCFG2;

    /****/ volatile UINT32 BCFG3;
};
//
// External Memory Controller
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// SPI
//
struct LPC22XX_SPI
{
    static const UINT32 c_SPI0_Base     = 0xE0020000;
    static const UINT32 c_SPI1_Base     = 0xE0030000;
    static const UINT32 c_SPI_Clk_KHz   =      15000;

    static const UINT32 c_SPI0 = 0;
    static const UINT32 c_SPI1 = 1;
    static const UINT32 c_MAX_SPI = 2;

    static const UINT32 c_SPI0_SCLK = LPC22XX_GPIO::c_P0_04;
    static const UINT32 c_SPI0_MISO = LPC22XX_GPIO::c_P0_05;
    static const UINT32 c_SPI0_MOSI = LPC22XX_GPIO::c_P0_06;
    static const UINT32 c_SPI0_SSEL = LPC22XX_GPIO::c_P0_07;

    static const UINT32 c_SPI1_SCLK = LPC22XX_GPIO::c_P0_17;
    static const UINT32 c_SPI1_MISO = LPC22XX_GPIO::c_P0_18;
    static const UINT32 c_SPI1_MOSI = LPC22XX_GPIO::c_P0_19;
    static const UINT32 c_SPI1_SSEL = LPC22XX_GPIO::c_P0_20;

    /****/ volatile UINT32 SPCR;
    
//  MSB first always. SPI config structure does not contain a field for the bit order
    static const UINT32 CONTROLREG_MODE_Master     = 0x00000020;
    static const UINT32 CONTROLREG_PHA_1           = 0x00000008;
    static const UINT32 CONTROLREG_POL_1           = 0x00000010;

    /****/ volatile UINT32 SPSR;
    
    static const    UINT32 SPIF_Mask     = 0x00000080;

    /****/ volatile UINT32 SPDR;

    /****/ volatile UINT32 SPCCR;

    /****/ volatile UINT32 SPINT;
        
    static UINT32 ConfigurationToMode( const SPI_CONFIGURATION& Configuration )
    {
        UINT32 Mode;

        
        Mode = CONTROLREG_MODE_Master;

        if(Configuration.MSK_IDLE)
        {
           // Idle is Active high, set 1, 
            Mode |= CONTROLREG_POL_1;   
           
            if(Configuration.MSK_SampleEdge)
            {
                Mode |= CONTROLREG_PHA_1;        // sample on rising edge
            }
            // else phase set to 0
        }
        else
        {
            if(!Configuration.MSK_SampleEdge)
            {
                Mode |= CONTROLREG_PHA_1;        // sample on rising edge
            }
            // else phase set to 0;
        }
        return Mode;
    }
};
//
// SPI
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// I2C
//
struct LPC22XX_I2C
{
    static const UINT32 c_I2C_Base     = 0xE001C000;
    static const UINT32 c_I2C_Clk_KHz  =      15000;

    static const UINT32 c_I2C_SCL = LPC22XX_GPIO::c_P0_02;
    static const UINT32 c_I2C_SDA = LPC22XX_GPIO::c_P0_03;

    /****/ volatile UINT32 I2CONSET;
    static const    UINT32 I2EN  = 0x00000040;
    static const    UINT32 STA   = 0x00000020;
    static const    UINT32 STO   = 0x00000010;
    static const    UINT32 SI    = 0x00000008;
    static const    UINT32 AA    = 0x00000004;
    
    /****/ volatile UINT32 I2STAT;
    
    /****/ volatile UINT32 I2DAT;

    /****/ volatile UINT32 I2ADR;

    /****/ volatile UINT32 I2SCLH;
        
    /****/ volatile UINT32 I2SCLL;
    
    /****/ volatile UINT32 I2CONCLR;
};
//
// I2C
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// WATCHDOG
//
struct LPC22XX_WATCHDOG
{
    static const UINT32 c_WATCHDOG_Base  = 0xE0000000;

    //--//

    /****/ volatile UINT32 WDMOD;
    static const    UINT32 WDMOD__WDEN   = 0x00000001;
    static const    UINT32 WDMOD__WDRESET= 0x00000002;
    static const    UINT32 WDMOD__WDTOF  = 0x00000004;
    static const    UINT32 WDMOD__WDINT  = 0x00000008;

    /****/ volatile UINT32 WDTC;
    static const    UINT32 WDTC_mask     = 0x000000FF;

    /****/ volatile UINT32 WDFEED;
    static const    UINT32 WDFEED_reload_1  = 0x000000AA;
    static const    UINT32 WDFEED_reload_2  = 0x00000055;
    static const    UINT32 WDFEED_mask      = 0x000000FF;

    /****/ volatile UINT32 WDTV;
    static const    UINT32 WDTV_mask     = 0x000000FF;
 
};
//
// WATCHDOG
//////////////////////////////////////////////////////////////////////////////

/**********************************************************************************************************************/
/**********************************************************************************************************************/
/**********************************************************************************************************************/
/**********************************************************************************************************************/
/**********************************************************************************************************************/

struct LPC22XX
{
    static LPC22XX_VIC		& VIC    (         ) { return *(LPC22XX_VIC    *)(size_t)(      LPC22XX_VIC   ::c_Base                                      ); }
    static LPC22XX_GPIO		& GPIO   (         ) { return *(LPC22XX_GPIO   *)(size_t)(      LPC22XX_GPIO  ::c_GPIO_Base                                 ); }
    static LPC22XX_PCB		& PCB    (         ) { return *(LPC22XX_PCB    *)(size_t)(      LPC22XX_PCB   ::c_PCB_Base                                  ); }
    static LPC22XX_SYSCON	& SYSCON (         ) { return *(LPC22XX_SYSCON *)(size_t)(      LPC22XX_SYSCON::c_SYSCON_Base								); }
    static LPC22XX_EMC		& EMC    (         ) { return *(LPC22XX_EMC    *)(size_t)(      LPC22XX_EMC   ::c_EMC_Base                                  ); }
    static LPC22XX_I2C		& I2C    (         ) { return *(LPC22XX_I2C    *)(size_t)(      LPC22XX_I2C   ::c_I2C_Base                                  ); }
    static LPC22XX_WATCHDOG & WTDG	 (		   ) { return *(LPC22XX_WATCHDOG *)(size_t)( LPC22XX_WATCHDOG ::c_WATCHDOG_Base                             ); }
    static LPC22XX_SPI		& SPI    ( int sel ) { if(sel) return *(LPC22XX_SPI    *)(size_t)LPC22XX_SPI	::c_SPI1_Base								 ;
												   else	   return *(LPC22XX_SPI    *)(size_t)LPC22XX_SPI	::c_SPI0_Base								 ; }
    static LPC22XX_USART	& UART   ( int sel ) { if(sel) return *(LPC22XX_USART  *)(size_t)LPC22XX_USART	::c_Base_1									 ;
												   else	   return *(LPC22XX_USART  *)(size_t)LPC22XX_USART	::c_Base_0									 ; }
    static LPC22XX_TIMER	& TIMER  ( int sel ) { if(sel) return *(LPC22XX_TIMER  *)(size_t)LPC22XX_TIMER  ::c_Base_1									 ;
												   else    return *(LPC22XX_TIMER  *)(size_t)LPC22XX_TIMER	::c_Base_0									 ; }

//--//
};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// LPC22XX driver
//

struct LPC22XX_Driver
{
    static const UINT32 c_SystemTime_Timer = 0;

    //--//

    static BOOL Initialize();

    static void Sleep();

    static void Halt();

    static void Reset();

    static void Shutdown();

    static void Hibernate();
};

extern LPC22XX_Driver g_LPC22XX_Driver;

//
// LPC22XX driver
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Interrupt Controller driver
//

struct LPC22XX_VIC_Driver
{
    static const UINT32 c_VECTORING_GUARD = 32;

    //--//

    struct IRQ_VECTORING
    {
        UINT32       Index;
        HAL_CALLBACK Handler;
    };

    //--//

    static IRQ_VECTORING s_IsrTable[];

    //--//

    static void Initialize();

    static BOOL ActivateInterrupt  ( UINT32 Irq_Index, BOOL Fast, HAL_CALLBACK_FPN ISR, void* ISR_Param );
    static BOOL DeactivateInterrupt( UINT32 Irq_Index                                                                   );

    static BOOL InterruptEnable( UINT32 Irq_Index );

    static BOOL InterruptDisable( UINT32 Irq_Index );

    static BOOL InterruptEnableState( UINT32 Irq_Index );

    static BOOL InterruptState( UINT32 Irq_Index );

#if defined(PLATFORM_ARM)
    static void IRQ_Handler();
#endif

    static IRQ_VECTORING* IRQToIRQVector( UINT32 IRQ );

private:
    static void STUB_ISRVector( void* Param );

};

//
// Interrupt Controller driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// TIMER driver
//

struct LPC22XX_TIMER_Driver
{
    const static UINT32 c_MaxTimers = 2;

    //--//

    BOOL m_configured[c_MaxTimers];

    static BOOL   Initialize   ( UINT32 Timer, HAL_CALLBACK_FPN ISR, void* ISR_Param );
    static BOOL   Uninitialize ( UINT32 Timer                                                                                              );
    static UINT32 ReadCounter  ( UINT32 Timer                                                                                              );

    static void EnableCompareInterrupt( UINT32 Timer )
    {
        ASSERT(Timer < c_MaxTimers);

        LPC22XX_TIMER& TIMER = LPC22XX::TIMER( Timer );

        TIMER.MCR |= LPC22XX_TIMER::MR0_IRQEN;
    }

    static void DisableCompareInterrupt( UINT32 Timer )
    {
        ASSERT(Timer < c_MaxTimers);

        LPC22XX_TIMER& TIMER = LPC22XX::TIMER( Timer );

        TIMER.MCR &= ~LPC22XX_TIMER::MR0_IRQEN;
    }

    static void ForceInterrupt( UINT32 Timer )
    {
        ASSERT(Timer < c_MaxTimers);
		
		UINT32 irq;
		
		if(Timer)
		{
			irq = LPC22XX_VIC::c_IRQ_INDEX_TIMER1;
		}
		else
		{
			irq = LPC22XX_VIC::c_IRQ_INDEX_TIMER0;
		}

        LPC22XX::VIC().ForceInterrupt( irq );
    }

    static void SetCompare( UINT32 Timer, UINT32 Compare )
    {
        ASSERT(Timer < c_MaxTimers);

        LPC22XX_TIMER& TIMER = LPC22XX::TIMER( Timer );

        TIMER.MR0 = Compare;
    }

    static BOOL DidCompareHit( UINT32 Timer )
    {
        ASSERT(Timer < c_MaxTimers);

        LPC22XX_TIMER& TIMER = LPC22XX::TIMER( Timer );

        return (TIMER.IR & LPC22XX_TIMER::MR0_COMP) != 0;
    }

    static void ResetCompareHit( UINT32 Timer )
    {
        ASSERT(Timer < c_MaxTimers);

        LPC22XX_TIMER& TIMER = LPC22XX::TIMER( Timer );

        TIMER.IR = LPC22XX_TIMER::MR0_RESET;
    }
};

extern LPC22XX_TIMER_Driver g_LPC22XX_TIMER_Driver;

//
// TIMER driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// System Time driver
//

struct LPC22XX_TIME_Driver
{
    static const UINT32 c_OverflowCheck = 0x80000000;

    UINT64 m_lastRead;
    UINT64 m_nextCompare;

    static BOOL Initialize  ();
    static BOOL Uninitialize();

    static UINT64 CounterValue();

    static void SetCompareValue( UINT64 CompareValue );

    static INT64 TicksToTime( UINT64 Ticks );

    static INT64 CurrentTime();

    static void Sleep_uSec( UINT32 uSec );

    static void Sleep_uSec_Loop( UINT32 uSec );

private:
    static void ISR( void* Param );
};

extern LPC22XX_TIME_Driver g_LPC22XX_TIME_Driver;

//
// System Time driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// UART driver
//

struct LPC22XX_USART_Driver
{
    static const UINT32 c_RefFlagRx = 0x01;
    static const UINT32 c_RefFlagTx = 0x02;

    static const UINT32 c_COM1      = 0;
    static const UINT32 c_COM2      = 1;


    INT8 m_RefFlags[2];

    static BOOL Initialize                        ( int ComPortNum, int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue );
    static BOOL Uninitialize                      ( int ComPortNum               );
    static BOOL TxBufferEmpty                     ( int ComPortNum               );
    static BOOL TxShiftRegisterEmpty              ( int ComPortNum               );
    static void WriteCharToTxBuffer               ( int ComPortNum, UINT8 c      );
    static void TxBufferEmptyInterruptEnable      ( int ComPortNum, BOOL Enable  );
    static BOOL TxBufferEmptyInterruptState       ( int ComPortNum               );
    static void RxBufferFullInterruptEnable       ( int ComPortNum, BOOL Enable  );
    static BOOL RxBufferFullInterruptState        ( int ComPortNum               );
    static BOOL TxHandshakeEnabledState           ( int comPortNum               );
    static void ProtectPins                       ( int ComPortNum, BOOL On      );
    static void GetPins                           (int comPort,  GPIO_PIN& rxPin, GPIO_PIN& txPin,GPIO_PIN& ctsPin, GPIO_PIN& rtsPin );    
    static void BaudrateBoundary                  (int ComPortNum, UINT32& maxBaudrateHz, UINT32& minBaudrateHz);
    static BOOL IsBaudrateSupported               (int ComPortNum, UINT32 & BaudrateHz);        
    
private:
    static void UART_IntHandler                   ( void *     Param             );
    static BOOL IsValidPortNum                    ( int ComPortNum               );
};

extern LPC22XX_USART_Driver g_LPC22XX_USART_Driver;

//
// UART driver
//////////////////////////////////////////////////////////////////////////////

struct LPC22XX_GPIO_Driver
{

    struct PIN_ISR_DESCRIPTOR
    {
        HAL_COMPLETION                 m_completion;
        GPIO_INTERRUPT_SERVICE_ROUTINE m_isr;
        void*                          m_param;
        GPIO_PIN                       m_pin;
        GPIO_INT_EDGE                  m_intEdge;
        UINT8                          m_flags;
        UINT8                          m_status;

        //--//
        
        static const UINT8 c_Flags_Debounce        = 0x01;
        static const UINT8 c_Flags_RequireLowEdge  = 0x02;
        static const UINT8 c_Flags_RequireHighEdge = 0x04;

        static const UINT8 c_Status_AllowLowEdge   = 0x01;
        static const UINT8 c_Status_AllowHighEdge  = 0x02;

        //--//

        static void Fire( void* arg );

        void HandleDebounce( BOOL edge );
    };

    //--//

    static const UINT32 c_MaxPins           =  31;
    static const UINT32 c_MaxPorts          =   1;
    static const UINT32 c_PinsPerPort       =  32;
    static const UINT32 c_PinsPerPCBReg     =  16;

    static const INT32  c_MinDebounceTimeMs =    1; // 1 ms
    static const INT32  c_MaxDebounceTimeMs = 5000; // 5  s

    //--//

    static const UINT8 c_Gpio_Attributes[c_MaxPins];

    // applies to all GPIO
    static const UINT8 c_NumberofResistorMode = 3;
    static const UINT8 c_GPIO_ResistorMode  = (1<<RESISTOR_DISABLED) |(1<<RESISTOR_PULLDOWN) | (1<<RESISTOR_PULLUP) ;

    static const UINT8 c_NumberofInterruptMode = 5;
    static const UINT8 c_GPIO_InterruptMode = (1<<GPIO_INT_EDGE_LOW) | (1<<GPIO_INT_EDGE_HIGH ) | (1<<GPIO_INT_EDGE_BOTH) | (1<<GPIO_INT_LEVEL_HIGH) | (1<<GPIO_INT_LEVEL_LOW);



    PIN_ISR_DESCRIPTOR m_PinIsr            [c_MaxPins ];
    UINT32             m_PinReservationInfo[c_MaxPins];
    UINT32             m_DebounceTicks;
    
    //--//

    static BOOL Initialize  ();

    static BOOL Uninitialize();

    static UINT32 Attributes( GPIO_PIN Pin );

    static void DisablePin( GPIO_PIN Pin, GPIO_RESISTOR ResistorState, UINT32 Direction, GPIO_ALT_MODE AltFunction);

    static void EnableOutputPin( GPIO_PIN Pin, BOOL InitialState );

    static BOOL EnableInputPin( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE ISR, void* ISR_Param, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState );

    static BOOL GetPinState( GPIO_PIN Pin );

    static void SetPinState( GPIO_PIN Pin, BOOL PinState );

    static void ISR( void* Param );

    static void SetResistor( GPIO_PIN Pin, GPIO_RESISTOR ResistorState );

    static BOOL PinIsBusy( GPIO_PIN Pin );

    static BOOL ReservePin( GPIO_PIN Pin, BOOL fReserve );

    static UINT32 GetDebounce();

    static BOOL SetDebounce( INT64 debounceTimeMilliseconds ); 

    static INT32 GetPinCount();
    
    static void GetPinsMap( UINT8* pins, size_t size );
    
    static UINT8 GetSupportedResistorModes( GPIO_PIN pin );

    static UINT8 GetSupportedInterruptModes( GPIO_PIN pin );    

    static void PortConfiguration( UINT32 Port, UINT32 ConfigValue );

    static void PortDirection( UINT32 Port, UINT32 DirValue ); 
    
    static UINT32 GetPinBynIRQ( UINT32 nIRQ );   
    
    static UINT32 GetnIRQByPin( UINT32 Pin ); 
    
    static BOOL   SetnIRQISR  ( GPIO_PIN pin, HAL_CALLBACK_FPN ISR, GPIO_INT_EDGE intEdge, GPIO_RESISTOR resistorState ) ;
    
private:

    static UINT32 PinToPort( GPIO_PIN pin )
    {
        ASSERT(pin < c_MaxPins);

        return pin / c_PinsPerPort;
    }

    static UINT32 PinToBit( GPIO_PIN pin )
    {
        ASSERT(pin < c_MaxPins);

        return pin % c_PinsPerPort;
    }

    static UINT32 PinToPCBReg( GPIO_PIN pin )
    {
        ASSERT(pin < c_MaxPins);

        return pin / c_PinsPerPCBReg;
    }

    static UINT32 PinToPCBRegBit( GPIO_PIN pin )
    {
        ASSERT(pin < c_MaxPins);

        return ((pin % c_PinsPerPCBReg) * 2);
    }

    static void STUB_ISRVector( GPIO_PIN Pin, BOOL PinState, void* Param );

    static void nIRQ_ISR( void* Param );
};

extern LPC22XX_GPIO_Driver g_LPC22XX_GPIO_Driver;

//
// GP I/O driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// SPI  driver
//

struct LPC22XX_SPI_Driver
{
    BOOL m_Enabled[LPC22XX_SPI::c_MAX_SPI];

    static BOOL Initialize  ();
    static void Uninitialize();

    static void GetPins         ( UINT32 spi_mod, GPIO_PIN& msk, GPIO_PIN& miso, GPIO_PIN& mosi );

    static BOOL nWrite16_nRead16( const SPI_CONFIGURATION& Configuration, UINT16* Write16, INT32 WriteCount, UINT16* Read16, INT32 ReadCount, INT32 ReadStartOffset );
    static BOOL nWrite8_nRead8  ( const SPI_CONFIGURATION& Configuration, UINT8*  Write8 , INT32 WriteCount, UINT8*  Read8 , INT32 ReadCount, INT32 ReadStartOffset );

    static BOOL Xaction_Start   ( const SPI_CONFIGURATION& Configuration );
    static BOOL Xaction_Stop    ( const SPI_CONFIGURATION& Configuration );

    static BOOL Xaction_nWrite16_nRead16( SPI_XACTION_16& Transaction );
    static BOOL Xaction_nWrite8_nRead8  ( SPI_XACTION_8&  Transaction );

private:
    static void ISR( void* Param );
};

extern LPC22XX_SPI_Driver g_LPC22XX_SPI_Driver;

//
// SPI driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// I2C driver
//

struct I2C_HAL_XACTION_UNIT;
struct I2C_HAL_XACTION;
struct I2C_CONFIGURATION;

struct LPC22XX_I2C_Driver
{
    BOOL m_initialized;

	//--//
	
    I2C_HAL_XACTION*       m_currentXAction;
    I2C_HAL_XACTION_UNIT*  m_currentXActionUnit;

    //--//

    static const UINT8 c_DirectionWrite = 0x00;
    static const UINT8 c_DirectionRead  = 0x01;

    //--//

    static BOOL  Initialize         (                                              );
    static BOOL  Uninitialize       (                                              );
    static void  MasterXAction_Start( I2C_HAL_XACTION* xAction, bool repeatedStart );
    static void  MasterXAction_Stop (                                              );
    static UINT8 GetClockRate       ( UINT32 rateKhz                               );
    static void  GetPins            ( GPIO_PIN& scl, GPIO_PIN& sda                 );

private:
    static void WriteToSubordinate ( I2C_HAL_XACTION_UNIT* xAction );
    static void ReadFromSubordinate( I2C_HAL_XACTION_UNIT* xAction );
    static void ISR                ( void*            arg          );

    static I2C_CONFIGURATION& GetConfig();
};

extern LPC22XX_I2C_Driver g_LPC22XX_I2C_Driver;

//
// I2C driver
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// Watchdog driver
//

struct LPC22XX_WATCHDOG_Driver
{
    static const UINT32 c_MinCounter  = 256;
    static const UINT32 c_Granularity = TEN_MHZ*4 / SLOW_CLOCKS_PER_SECOND; // us

    //--//

    static HRESULT Enable( UINT32 TimeoutMilliseconds, WATCHDOG_INTERRUPT_CALLBACK callback, void* context );

    static void Disable();

    static void ResetCounter();

    static void ResetCpu();

private:

    static void ISR( void* Param );

    //--//

    WATCHDOG_INTERRUPT_CALLBACK m_callback;
    void*                       m_context;
};

extern LPC22XX_WATCHDOG_Driver g_LPC22XX_WATCHDOG_Driver;

//
// Watchdog driver
//////////////////////////////////////////////////////////////////////////////


struct LPC22XX_SOCKETS_Driver
{
    static BOOL Initialize();
    
    static BOOL Uninitialize();
    
    static SOCK_SOCKET Socket( int family, int type, int protocol );
    
    static int Bind( SOCK_SOCKET socket, const SOCK_sockaddr* address, int addressLen  );

    static int Connect(SOCK_SOCKET socket, const SOCK_sockaddr* address, int addressLen);

    static int Send(SOCK_SOCKET socket, const char* buf, int len, int flags);

    static int Recv(SOCK_SOCKET socket, char* buf, int len, int flags);

    static int Close(SOCK_SOCKET socket);

    static int Listen(SOCK_SOCKET socket, int backlog );

    static SOCK_SOCKET Accept(SOCK_SOCKET socket, SOCK_sockaddr* address, int* addressLen );


    static int Shutdown(SOCK_SOCKET socket, int how );

    static int GetAddrInfo(const char* nodename, 
                  char* servname, 
                  const SOCK_addrinfo* hints, 
                  SOCK_addrinfo** res );

    static void FreeAddrInfo(SOCK_addrinfo* ai );

    static int Ioctl(SOCK_SOCKET socket, int cmd, int* data );

    static int GetLastError();

    static int GetNativeError( int error );

    static int GetNativeSockOption (int level, int optname, int * optval);

    static int GetNativeIPOption (int optname);

    static int Select(int nfds, SOCK_fd_set* readfds, SOCK_fd_set* writefds, SOCK_fd_set* exceptfds, const SOCK_timeval* timeout );

    static int SetSockOpt(SOCK_SOCKET socket, int level, int optname, const char* optval, int  optlen );

    static int GetSockOpt(SOCK_SOCKET socket, int level, int optname, char* optval, int* optlen );

    static int GetPeerName(SOCK_SOCKET socket, SOCK_sockaddr* name, int* namelen );
    
    static int GetSockName(SOCK_SOCKET socket, SOCK_sockaddr* name, int* namelen );

    static int RecvFrom(SOCK_SOCKET s, char* buf, int len, int flags, SOCK_sockaddr* from, int* fromlen );
    
    static int SendTo(SOCK_SOCKET s, const char* buf, int len, int flags, const SOCK_sockaddr* to, int tolen );

    static UINT32 GetAdapterCount();

    static HRESULT LoadAdapterConfiguration( UINT32 interfaceIndex, SOCK_NetworkConfiguration* config );
    
    static HRESULT UpdateAdapterConfiguration( UINT32 interfaceIndex, UINT32 updateFlags, SOCK_NetworkConfiguration* config );

private:

    static BOOL s_fDnsInitialized;
    static BOOL s_fInitialized;

};
//
// SOCKET driver
/////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _LPC22XX_ 1
