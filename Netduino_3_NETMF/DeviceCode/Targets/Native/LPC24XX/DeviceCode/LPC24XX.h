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

#ifndef _LPC24XX_H_
#define _LPC24XX_H_ 1

#include <cores\arm\include\cpu.h>

//////////////////////////////////////////////////////////////////////////////
// Interrupt Controller
//
struct LPC24XX_VIC
{
    static const UINT32 c_VIC_Base = 0xFFFFF000;

    //--//
    // Priority Levels, 0 = Highest priority
    static const UINT32 c_IRQ_Priority_0  =  0;
    static const UINT32 c_IRQ_Priority_1  =  1;
    static const UINT32 c_IRQ_Priority_2  =  2;
    static const UINT32 c_IRQ_Priority_3  =  3;
    static const UINT32 c_IRQ_Priority_4  =  4;
    static const UINT32 c_IRQ_Priority_5  =  5;
    static const UINT32 c_IRQ_Priority_6  =  6;
    static const UINT32 c_IRQ_Priority_7  =  7;
    static const UINT32 c_IRQ_Priority_8  =  8;
    static const UINT32 c_IRQ_Priority_9  =  9;
    static const UINT32 c_IRQ_Priority_10 = 10;
    static const UINT32 c_IRQ_Priority_11 = 11;
    static const UINT32 c_IRQ_Priority_12 = 12;
    static const UINT32 c_IRQ_Priority_13 = 13;
    static const UINT32 c_IRQ_Priority_14 = 14;
    static const UINT32 c_IRQ_Priority_15 = 15;

    // Interrupt slot assignments
    static const UINT32 c_IRQ_INDEX_WDT              =  0;
    static const UINT32 c_IRQ_INDEX_SW               =  1;
    static const UINT32 c_IRQ_INDEX_DBG_COM_RX       =  2;
    static const UINT32 c_IRQ_INDEX_DBG_COM_TX       =  3;
    static const UINT32 c_IRQ_INDEX_TIMER0           =  4;
    static const UINT32 c_IRQ_INDEX_TIMER1           =  5;
    static const UINT32 c_IRQ_INDEX_UART0            =  6;
    static const UINT32 c_IRQ_INDEX_UART1            =  7;
    static const UINT32 c_IRQ_INDEX_PWM_0_1          =  8;
    static const UINT32 c_IRQ_INDEX_I2C0             =  9;
    static const UINT32 c_IRQ_INDEX_SPI_SSP0         = 10;
    static const UINT32 c_IRQ_INDEX_SSP1             = 11;
    static const UINT32 c_IRQ_INDEX_PLL              = 12;
    static const UINT32 c_IRQ_INDEX_RTC              = 13;
    static const UINT32 c_IRQ_INDEX_EINT0            = 14;
    static const UINT32 c_IRQ_INDEX_EINT1            = 15;
    static const UINT32 c_IRQ_INDEX_EINT2_LCD        = 16;
    static const UINT32 c_IRQ_INDEX_EINT3            = 17;
    static const UINT32 c_IRQ_INDEX_ADC0             = 18;
    static const UINT32 c_IRQ_INDEX_I2C1             = 19;
    static const UINT32 c_IRQ_INDEX_BOD              = 20;
    static const UINT32 c_IRQ_INDEX_EMAC             = 21;
    static const UINT32 c_IRQ_INDEX_USB              = 22;
    static const UINT32 c_IRQ_INDEX_CAN              = 23;
    static const UINT32 c_IRQ_INDEX_SD               = 24;
    static const UINT32 c_IRQ_INDEX_DMA              = 25;
    static const UINT32 c_IRQ_INDEX_TIMER2           = 26;
    static const UINT32 c_IRQ_INDEX_TIMER3           = 27;
    static const UINT32 c_IRQ_INDEX_UART2            = 28;
    static const UINT32 c_IRQ_INDEX_UART3            = 29;
    static const UINT32 c_IRQ_INDEX_I2C2             = 30;
    static const UINT32 c_IRQ_INDEX_I2S              = 31;
    //--//

    /****/ volatile UINT32 IRQSTATUS;   // Status of enabled IRQ requests
    /****/ volatile UINT32 FIQSTATUS;   // Status of enabled FIQ requests
    /****/ volatile UINT32 RAWINTR;     // Status of interrupt lines irrespective of the classificatino/enabling
    /****/ volatile UINT32 INTRSEL;     // FIQ or IRQ
    /****/ volatile UINT32 INTENABLE;   // Enable interrupt line
    /****/ volatile UINT32 INTENCLR;    // Disable interrupt line
    /****/ volatile UINT32 SOFTINT;     // Force SW interrupt
    /****/ volatile UINT32 SOFTINTCLR;  // Clear SW interrupt
    /****/ volatile UINT32 PROTECTEN;   // VIC registers can only be accessed in privileged mode
      /****/ volatile UINT32 PRIORITYMASK;// Mask interrupt priority level

     volatile UINT32 Padding0[54];

     volatile UINT32 VECTADDR[32];      // Address of vectored interrupt handler for slot 0 - 31

     volatile UINT32 Padding1[32];

     volatile UINT32 VECTPRIORITY[32];  // Priority control register for slot 0 - 31

     volatile UINT32 Padding2[800];

     volatile UINT32 ADDRESS;           // Address of the current Active interrupt

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

        for( index = 0; index < c_MaxInterruptIndex; index++)
        {
            if( (IRQSTATUS >> index) & 0x1 )
            {
                return index;
            }
        }

        return c_MaxInterruptIndex;
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
struct LPC24XX_TIMER
{

    static const UINT32 c_Timer_0 = 0;
    static const UINT32 c_Timer_1 = 1;
    static const UINT32 c_Timer_2 = 2;
    static const UINT32 c_Timer_3 = 3;
    static const UINT32 c_MaxTimer_no = 3;

    static const UINT32 c_TimerBase_0 = 0xE0004000;
    static const UINT32 c_TimerBase_1 = 0xE0008000;
    static const UINT32 c_TimerBase_2 = 0xE0070000;
    static const UINT32 c_TimerBase_3 = 0xE0074000;


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
    //functions.
    static UINT32 inline getIntNo(int Timer)
    {
        switch (Timer)
        {
        case  c_Timer_0:
            return LPC24XX_VIC::c_IRQ_INDEX_TIMER0;
            break;
        case  c_Timer_1:
            return LPC24XX_VIC::c_IRQ_INDEX_TIMER1;
            break;
        case  c_Timer_2:
            return LPC24XX_VIC::c_IRQ_INDEX_TIMER2;
            break;
        case  c_Timer_3:
            return LPC24XX_VIC::c_IRQ_INDEX_TIMER3;
            break;
        default:
            ASSERT(TRUE);
            return(Timer);            
            break;
        }
    }
};
//
// TIMER
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// GP I/O
//
struct LPC24XX_GPIO
{
    // Use FAST IO for all ports
    static const UINT32 c_GPIO_Base = 0x3FFFC000;

    //--//

    static const UINT32 c_Pin_None  = 0xFFFFFFFF;
// Controlled by PINSEL0
    static const UINT32 c_P0_00 =   0; // RD1          / TXD3         / SDA1
    static const UINT32 c_P0_01 =   1; // TD1          / RXD3         / SCL1
    static const UINT32 c_P0_02 =   2; // TXD0         / Reserved     / Reserved
    static const UINT32 c_P0_03 =   3; // RXD0         / Reserved     / Reserved
    static const UINT32 c_P0_04 =   4; // I2SRX_CLK    / RD2          / CAP2[0]
                                        // LCDVD[0]
    static const UINT32 c_P0_05 =   5; // I2SRX_WS     / TD2          / CAP2[1]
                                        // LCDVD[1]
    static const UINT32 c_P0_06 =   6; // I2SRX_SDA    / SSEL1        / MAT2[0]
                                        // LCDVD[8]
    static const UINT32 c_P0_07 =   7; // I2STX_CLK    / SCK1         / MAT2[1]
                                        // LCDVD[9]
    static const UINT32 c_P0_08 =   8; // I2STX_WS     / MISO1        / MAT2[2]
                                        // LCDVD[16]
    static const UINT32 c_P0_09 =   9; // I2STX_SDA    / MOSI1        / MAT2[3]
                                        // LCDVD[17]
    static const UINT32 c_P0_10 =  10; // TXD2         / SDA2         / MAT3[0]
    static const UINT32 c_P0_11 =  11; // RXD2         / SCL2         / MAT3[1]
    static const UINT32 c_P0_12 =  12; // USB_PPWR2    / MISO1        / AD0[6]
    static const UINT32 c_P0_13 =  13; // USB_UP_LED2  / MOSI1        / AD0[7]
    static const UINT32 c_P0_14 =  14; // USB_HSTEN2   / USB_CONNECT2 / SSEL1
    static const UINT32 c_P0_15 =  15; // TXD1         / SCK0         / SCK
// Controlled by PINSEL1
    static const UINT32 c_P0_16 =  16; // RXD1         / SSEL0        / SSEL
    static const UINT32 c_P0_17 =  17; // CTS1         / MISO0        / MISO
    static const UINT32 c_P0_18 =  18; // DCD1         / MOSI0        / MOSI
    static const UINT32 c_P0_19 =  19; // DSR1         / MCICLK       / SDA1
    static const UINT32 c_P0_20 =  20; // DTR1         / MCICMD       / SCL1
    static const UINT32 c_P0_21 =  21; // RI1          / MCIPWR       / RD1
    static const UINT32 c_P0_22 =  22; // RTS1         / MCIDAT0      / TD1
    static const UINT32 c_P0_23 =  23; // AD0[0]       / I2SRX_CLK    / CAP3[0]
    static const UINT32 c_P0_24 =  24; // AD0[1]       / I2SRX_WS     / CAP3[1]
    static const UINT32 c_P0_25 =  25; // AD0[2]       / I2SRX_SDA    / TXD3
    static const UINT32 c_P0_26 =  26; // AD0[3]       / AOUT         / RXD3
    static const UINT32 c_P0_27 =  27; // SDA0         / Reserved     / Reserved
    static const UINT32 c_P0_28 =  28; // SCL0         / Reserved     / Reserved
    static const UINT32 c_P0_29 =  29; // USB_D+1      / Reserved     / Reserved
    static const UINT32 c_P0_30 =  30; // USB_D-1      / Reserved     / Reserved
    static const UINT32 c_P0_31 =  31; // USB_D+2      / Reserved     / Reserved
// Controlled by PINSEL2
    static const UINT32 c_P1_00 =  32; // ENET_TXD0    / Reserved     / Reserved
    static const UINT32 c_P1_01 =  33; // ENET_TXD1    / Reserved     / Reserved
    static const UINT32 c_P1_02 =  34; // ENET_TXD2    / MCICLK       / PWM0[1]
    static const UINT32 c_P1_03 =  35; // ENET_TXD3    / MCICMD       / PWM0[2]
    static const UINT32 c_P1_04 =  36; // ENET_TX_EN   / Reserved     / Reserved
    static const UINT32 c_P1_05 =  37; // ENET_TX_ER   / MCIPWR       / PWM0[3]
    static const UINT32 c_P1_06 =  38; // ENET_TX_CLK  / MCIDAT0      / PWM0[4]
    static const UINT32 c_P1_07 =  39; // ENET_COL     / MCIDAT1      / PWM0[5]
    static const UINT32 c_P1_08 =  40; // ENET_CRS_DV  / Reserved     / Reserved
                                        // ENET_CRS
    static const UINT32 c_P1_09 =  41; // ENET_RXD0    / Reserved     / Reserved
    static const UINT32 c_P1_10 =  42; // ENET_RXD1    / Reserved     / Reserved
    static const UINT32 c_P1_11 =  43; // ENET_RXD2    / MCIDAT2      / PWM0[6]
    static const UINT32 c_P1_12 =  44; // ENET_RXD3    / MCIDAT3      / PCAP0[0]
    static const UINT32 c_P1_13 =  45; // ENET_RX_DV   / Reserved     / Reserved
    static const UINT32 c_P1_14 =  46; // ENET_RX_ER   / Reserved     / Reserved
    static const UINT32 c_P1_15 =  47; // ENET_REF_CLK / Reserved     / Reserved
                                        // ENET_CRS
// Controlled by PINSEL3
    static const UINT32 c_P1_16 =  48; // ENET_MDC     / Reserved     / Reserved
    static const UINT32 c_P1_17 =  49; // ENET_MDIO    / Reserved     / Reserved
    static const UINT32 c_P1_18 =  50; // USB_UP_LED1  / PWM1[1]      / CAP1[0]
    static const UINT32 c_P1_19 =  51; // USB_TX_E1    / USB_PPWR1    / CAP1[1]    
    static const UINT32 c_P1_20 =  52; // USB_TX_DP1   / PWM1[2]      / SCK0
                                        // LCDVD[6]
                                        // LCDVD[10]
    static const UINT32 c_P1_21 =  53; // USB_TX_DM1   / PWM1[3]      / SSEL0
                                        // LCDVD[7]
                                        // LCDVD[11]
    static const UINT32 c_P1_22 =  54; // USB_RCV1     / USB_PWRD1    / MAT1[0]
                                        // LCDVD[8]
                                        // LCDVD[12]
    static const UINT32 c_P1_23 =  55; // USB_RX_DP1   / PWM1[4]      / MISO0
                                        // LCDVD[9]
                                        // LCDVD[13]
    static const UINT32 c_P1_24 =  56; // USB_RX_DM1   / PWM1[5]      / MOSI0
                                        // LCDVD[10]
                                        // LCDVD[14]
    static const UINT32 c_P1_25 =  57; // USB_LS1      / USB_HSTEN1   / MAT1[1]
                                        // LCDVD[11]
                                        // LCDVD[15]
    static const UINT32 c_P1_26 =  58; // USB_SSPND1   / PWM1[6]      / CAP0[0]
                                        // LCDVD[12]
                                        // LCDVD[20]
    static const UINT32 c_P1_27 =  59; // USB_INT1     / USB_OVRCR1   / CAP0[1]
                                        // LCDVD[13]
                                        // LCDVD[21]
    static const UINT32 c_P1_28 =  60; // USB_SCL1     / PCAP1[0]     / MAT0[0]
                                        // LCDVD[14]
                                        // LCDVD[22]
    static const UINT32 c_P1_29 =  61; // USB_SDA1     / PCAP1[1]     / MAT0[1]
                                        // LCDVD[15]
                                        // LCDVD[23]
    static const UINT32 c_P1_30 =  62; // USB_PWRD2    / VBUS         / AD0[4]
    static const UINT32 c_P1_31 =  63; // USB_OVRCR2   / SCK1         / AD0[5]
// Controlled by PINSEL4
    static const UINT32 c_P2_00 =  64; // PWM1[1]      / TXD1         / TRACECLK
                                        //                             / LCDPWR
    static const UINT32 c_P2_01 =  65; // PWM1[2]      / RXD1         / PIPESTAT0
                                        //                             / LCDLE
    static const UINT32 c_P2_02 =  66; // PWM1[3]      / CTS1         / PIPESTAT1
                                        //                             / LCDDCLK
    static const UINT32 c_P2_03 =  67; // PWM1[4]      / DCD1         / PIPESTAT2
                                        //                             / LCDFP
    static const UINT32 c_P2_04 =  68; // PWM1[5]      / DSR11        / TRACESYNC
                                        //                             / LCDENAB
                                        //                             / LCDM
    static const UINT32 c_P2_05 =  69; // PWM1[6]      / DTR1         / TRACEPKT0
                                        //                             / LCDLP
    static const UINT32 c_P2_06 =  70; // PCAP1[0]     / RI1          / TRACEPKT1
                                        //                             / LCDVD[0]
                                        //                             / LCDVD[4]
    static const UINT32 c_P2_07 =  71; // RD2          / RTS1         / TRACEPKT2
                                        //                             / LCDVD[1]
                                        //                             / LCDVD[5]
    static const UINT32 c_P2_08 =  72; // TD2          / TXD2         / TRACEPKT3
                                        //                             / LCDVD[2]
                                        //                             / LCDVD[6]
    static const UINT32 c_P2_09 =  73; // USB_CONNECT1 / RXD2         / EXTIN0
                                        //                             / LCDVD[3]
                                        //                             / LCDVD[7]
    static const UINT32 c_P2_10 =  74; // EINT0        / Reserved     / Reserved
    static const UINT32 c_P2_11 =  75; // EINT1        / MCIDAT1      / I2STX_CLK
    static const UINT32 c_P2_12 =  76; // EINT2        / MCIDAT2      / I2STX_WS
                                        // LCDVD[4]
                                        // LCDVD[3]
                                        // LCDVD[8]
                                        // LCDVD[18]
    static const UINT32 c_P2_13 =  77; // EINT3        / MCIDAT3      / I2STX_SDA
                                        // LCDVD[5]
                                        // LCDVD[9]
                                        // LCDVD[19]
    static const UINT32 c_P2_14 =  78; // CS2          / CAP2[0]      / SDA1
    static const UINT32 c_P2_15 =  79; // CS3          / CAP2[1]      / SCL1
// Controlled by PINSEL5
    static const UINT32 c_P2_16 =  80; // CAS          / Reserved     / Reserved
    static const UINT32 c_P2_17 =  81; // RAS          / Reserved     / Reserved
    static const UINT32 c_P2_18 =  82; // CLKOUT0      / Reserved     / Reserved
    static const UINT32 c_P2_19 =  83; // CLKOUT1      / Reserved     / Reserved
    static const UINT32 c_P2_20 =  84; // DYCS0        / Reserved     / Reserved
    static const UINT32 c_P2_21 =  85; // DYCS1        / Reserved     / Reserved
    static const UINT32 c_P2_22 =  86; // DYCS2        / CAP3[0]      / SCK0
    static const UINT32 c_P2_23 =  87; // DYCS3        / CAP3[1]      / SSEL0
    static const UINT32 c_P2_24 =  88; // CKEOUT0      / Reserved     / Reserved
    static const UINT32 c_P2_25 =  89; // CKEOUT1      / Reserved     / Reserved
    static const UINT32 c_P2_26 =  90; // CKEOUT2      / MAT3[0]      / MISO0
    static const UINT32 c_P2_27 =  91; // CKEOUT3      / MAT3[1]      / MOSI0
    static const UINT32 c_P2_28 =  92; // DQMOUT0      / Reserved     / Reserved
    static const UINT32 c_P2_29 =  93; // DQMOUT1      / Reserved     / Reserved
    static const UINT32 c_P2_30 =  94; // DQMOUT2      / MAT3[2]      / SDA2
    static const UINT32 c_P2_31 =  95; // DQMOUT3      / MAT3[3]      / SCL2
// Controlled by PINSEL6
    static const UINT32 c_P3_00 =  96; // D0           / Reserved     / Reserved
    static const UINT32 c_P3_01 =  97; // D1           / Reserved     / Reserved
    static const UINT32 c_P3_02 =  98; // D2           / Reserved     / Reserved
    static const UINT32 c_P3_03 =  99; // D3           / Reserved     / Reserved
    static const UINT32 c_P3_04 = 100; // D4           / Reserved     / Reserved
    static const UINT32 c_P3_05 = 101; // D5           / Reserved     / Reserved
    static const UINT32 c_P3_06 = 102; // D6           / Reserved     / Reserved
    static const UINT32 c_P3_07 = 103; // D7           / Reserved     / Reserved
    static const UINT32 c_P3_08 = 104; // D8           / Reserved     / Reserved
    static const UINT32 c_P3_09 = 105; // D9           / Reserved     / Reserved
    static const UINT32 c_P3_10 = 106; // D10          / Reserved     / Reserved
    static const UINT32 c_P3_11 = 107; // D11          / Reserved     / Reserved
    static const UINT32 c_P3_12 = 108; // D12          / Reserved     / Reserved
    static const UINT32 c_P3_13 = 109; // D13          / Reserved     / Reserved
    static const UINT32 c_P3_14 = 110; // D14          / Reserved     / Reserved
    static const UINT32 c_P3_15 = 111; // D15          / Reserved     / Reserved
// Controlled by PINSEL7
    static const UINT32 c_P3_16 = 112; // D16          / PWM0[1]      / TXD1
    static const UINT32 c_P3_17 = 113; // D17          / PWM0[2]      / RXD1
    static const UINT32 c_P3_18 = 114; // D18          / PWM0[3]      / CTS1
    static const UINT32 c_P3_19 = 115; // D19          / PWM0[4]      / DCD1
    static const UINT32 c_P3_20 = 116; // D20          / PWM0[5]      / DSR1
    static const UINT32 c_P3_21 = 117; // D21          / PWM0[6]      / DR1
    static const UINT32 c_P3_22 = 118; // D22          / PCAP0[0]     / RI1
    static const UINT32 c_P3_23 = 119; // D23          / CAP0[0]      / PCAP1[0]
    static const UINT32 c_P3_24 = 120; // D24          / CAP0[1]      / PWM1[1] 
    static const UINT32 c_P3_25 = 121; // D25          / MAT0[0]      / PWM1[2] 
    static const UINT32 c_P3_26 = 122; // D26          / MAT0[1]      / PWM1[3] 
    static const UINT32 c_P3_27 = 123; // D27          / CAP1[0]      / PWM1[4] 
    static const UINT32 c_P3_28 = 124; // D28          / CAP1[1]      / PWM1[5] 
    static const UINT32 c_P3_29 = 125; // D29          / MAT1[0]      / PWM1[6] 
    static const UINT32 c_P3_30 = 126; // D30          / MAT1[1]      / RTS1
    static const UINT32 c_P3_31 = 127; // D31          / MAT1[2]      / Reserved
// Controlled by PINSEL8
    static const UINT32 c_P4_00 = 128; // A0           / Reserved     / Reserved
    static const UINT32 c_P4_01 = 129; // A1           / Reserved     / Reserved
    static const UINT32 c_P4_02 = 130; // A2           / Reserved     / Reserved
    static const UINT32 c_P4_03 = 131; // A3           / Reserved     / Reserved
    static const UINT32 c_P4_04 = 132; // A4           / Reserved     / Reserved
    static const UINT32 c_P4_05 = 133; // A5           / Reserved     / Reserved
    static const UINT32 c_P4_06 = 134; // A6           / Reserved     / Reserved
    static const UINT32 c_P4_07 = 135; // A7           / Reserved     / Reserved
    static const UINT32 c_P4_08 = 136; // A8           / Reserved     / Reserved
    static const UINT32 c_P4_09 = 137; // A9           / Reserved     / Reserved
    static const UINT32 c_P4_10 = 138; // A10          / Reserved     / Reserved
    static const UINT32 c_P4_11 = 139; // A11          / Reserved     / Reserved
    static const UINT32 c_P4_12 = 140; // A12          / Reserved     / Reserved
    static const UINT32 c_P4_13 = 141; // A13          / Reserved     / Reserved
    static const UINT32 c_P4_14 = 142; // A14          / Reserved     / Reserved
    static const UINT32 c_P4_15 = 143; // A15          / Reserved     / Reserved
// Controlled by PINSEL9                  
    static const UINT32 c_P4_16 = 144; // A16          / Reserved     / Reserved 
    static const UINT32 c_P4_17 = 145; // A17          / Reserved     / Reserved 
    static const UINT32 c_P4_18 = 146; // A18          / Reserved     / Reserved 
    static const UINT32 c_P4_19 = 147; // A19          / Reserved     / Reserved 
    static const UINT32 c_P4_20 = 148; // A20          / SDA2         / SCK1     
    static const UINT32 c_P4_21 = 149; // A21          / SCL2         / SSEL1    
    static const UINT32 c_P4_22 = 150; // A22          / TXD2         / MISO1    
    static const UINT32 c_P4_23 = 151; // A23          / RXD2         / MOSI1    
    static const UINT32 c_P4_24 = 152; // OE           / Reserved     / Reserved 
    static const UINT32 c_P4_25 = 153; // WE           / Reserved     / Reserved 
    static const UINT32 c_P4_26 = 154; // BLS0         / Reserved     / Reserved 
    static const UINT32 c_P4_27 = 155; // BLS1         / Reserved     / Reserved 
    static const UINT32 c_P4_28 = 156; // BLS2         / MAT2[0]      / TXD3 00  
                                        //                             / LCDVD[6]   
                                        //                             / LCDVD[10]   
                                        //                             / LCDVD[2] 
    static const UINT32 c_P4_29 = 157; // BLS3         / MAT2[1]      / RXD3     
                                        //                             / LCDVD[7]    
                                        //                             / LCDVD[11]     
                                        //                             / LCDVD[3]       
    static const UINT32 c_P4_30 = 158; // CS0          / Reserved     / Reserved 
    static const UINT32 c_P4_31 = 159; // CS1          / Reserved     / Reserved 
                                                                          
    //--//

    struct 
    {
        /****/ volatile UINT32 FIODIR_PX;            // Direction Control
        /****/ volatile UINT32 dummy1;               // filler to align address
        /****/ volatile UINT32 dummy2;               // filler to align address
        /****/ volatile UINT32 dummy3;               // filler to align address
        /****/ volatile UINT32 FIOMASK_PX;           // Pin Mask Register
        /****/ volatile UINT32 FIOPIN_PX;            // Pin Value Register
        /****/ volatile UINT32 FIOSET_PX;            // Data Output Set Register
        /****/ volatile UINT32 FIOCLR_PX;            // Data Output Clear Register
    } Regs[5];

};
    //--//

struct LPC24XX_GPIOIRQ
{
    
    // GPIO IRQ registers for P0 & P2

    static const UINT32 c_GPIOIRQ_Base = 0xE0028080;
    
    //GPIO Interrupt registers (port 0 & 2 only can generate IRQs)
    
    //IRQ Overall Status
    UINT32 volatile IOIntStatus;    // 0xE0028080
    
    ////////////
    // PORT 0 //
    ////////////
    
    //IRQ Status
    UINT32 volatile IO0IntStatR;    // 0xE0028084
    UINT32 volatile IO0IntStatF;    // 0xE0028088
    
    //IRQ Clear
    UINT32 volatile IO0IntClr;        // 0xE002808C
    
    //IRQ Enable
    UINT32 volatile IO0IntEnR;        // 0xE0028090
    UINT32 volatile IO0IntEnF;        // 0xE0028094
    
    //////////////////
    // DUMMY PORT 1 //
    //////////////////
    
    UINT32 P1_generates_no_irqs[3]; // 0xE00280{98-9C-A0} (why did they leave just 3 empty regs?)
    
    ////////////
    // PORT 2 //
    ////////////
    
    //IRQ Status
    UINT32 volatile IO2IntStatR;    // 0xE00280A4
    UINT32 volatile IO2IntStatF;    // 0xE00280A8
    
    //IRQ Clear
    UINT32 volatile IO2IntClr;        // 0xE00280AC
    
    //IRQ Enable
    UINT32 volatile IO2IntEnR;        // 0xE00280B0
    UINT32 volatile IO2IntEnF;        // 0xE00280B4
    
    
};
//
// GP I/O
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// UART
//

struct LPC24XX_USART
{
    static const UINT32 c_Uart_0        = 0;
    static const UINT32 c_Uart_1        = 1;
    static const UINT32 c_Uart_2        = 2;
    static const UINT32 c_Uart_3        = 3;
    static const UINT32 c_MaxUart_no     = 3;    

    static const UINT32 c_UartBase_0    = 0xE000C000;
    static const UINT32 c_UartBase_1    = 0xE0010000;
    static const UINT32 c_UartBase_2    = 0xE0078000;
    static const UINT32 c_UartBase_3    = 0xE007C000;
    
    // UART0 pins
    static const UINT32 c_SER1_TXD = LPC24XX_GPIO::c_P0_02;
    static const UINT32 c_SER1_RXD = LPC24XX_GPIO::c_P0_03;

    // Add more pins if UART1 is used in full mode    
    static const UINT32 c_SER2_TXD = LPC24XX_GPIO::c_P3_16;
    static const UINT32 c_SER2_RXD = LPC24XX_GPIO::c_P3_17;

    // UART2 pins
    static const UINT32 c_SER3_TXD = LPC24XX_GPIO::c_P0_10;
    static const UINT32 c_SER3_RXD = LPC24XX_GPIO::c_P0_11;

    // UART3 pins
    static const UINT32 c_SER4_TXD = LPC24XX_GPIO::c_P0_00;
    static const UINT32 c_SER4_RXD = LPC24XX_GPIO::c_P0_01;


    static const UINT32 c_ClockRate     =   LPC24XX_UART_PCLK;        

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
    
    /****/ volatile UINT32 UART_LCR;                                   // line control register.
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

    /****/ volatile UINT32 UART_MCR_UART1_ONLY;                        // modem control register. 
        
    /****/ volatile UINT32 UART_LSR;                                   //line status register.
    static const UINT32 UART_LSR_ERR_RX                 = 0x00000080;     //Rx FIFO error 
    static const UINT32 UART_LSR_TE                     = 0x00000040;     //Transmitter Empty.
    static const UINT32 UART_LSR_THRE                   = 0x00000020;     //Transmitter Holding register Empty. 
    static const UINT32 UART_LSR_BII                    = 0x00000010;     //Break interrupt indicator.
    static const UINT32 UART_LSR_FEI                    = 0x00000008;     //Framing Error indicator.
    static const UINT32 UART_LSR_PEI                    = 0x00000004;     //Parity Error indicator.
    static const UINT32 UART_LSR_OEI                    = 0x00000002;     //Overrun Error indicator.
    static const UINT32 UART_LSR_RFDR                   = 0x00000001;     //RX FIFO data ready.
    
    /****/ volatile UINT32 UART_MSR_UART1_ONLY;                        //Modem status register.

    /****/ volatile UINT32 UART_SCR;                                   //Scratch pad register.

    /****/ volatile UINT32 UART_ACR;                                   //Autobaud control register.
    static const UINT32 UART_ACR_START                   = 0x00000001;     // Start bit
    static const UINT32 UART_ACR_MODE1                   = 0x00000002;     // Mode 1
    static const UINT32 UART_ACR_AUTO_RESTART            = 0x00000004;     // Auto Restart
    static const UINT32 UART_ACR_AUTOBAUD_INT_CLR        = 0x00000100;     // Autobaud interrupt clear
    static const UINT32 UART_ACR_TIMEOUT_INT_CLR         = 0x00000200;     // Autobaud timeout interrupt clear

    /****/ volatile UINT32 UART_ICR;                                   //IrDA control register.

    /****/ volatile UINT32 UART_FDR;                                   //Fractional divider register.
    static const UINT32 UART_FDR_DIVADDVAL_mask          = 0x0000000F;    
    static const UINT32 UART_FDR_DIVADDVAL_shift         = 0x00000000;    
    static const UINT32 UART_FDR_MULVAL_mask             = 0x000000F0;    
    static const UINT32 UART_FDR_MULVAL_shift            = 0x00000004;    

    /****/ volatile UINT32 PADDING_3;                                    

    /****/ volatile UINT32 UART_TER;                                   //Transmit Enable register.
    static const UINT32 UART_TER_TXEN                    = 0x00000080;  //TX Enable bit
          
    //functions.
    static UINT32 inline getIntNo(int ComPortNum)
    {
        switch (ComPortNum)
        {
        case c_Uart_0:
            return LPC24XX_VIC::c_IRQ_INDEX_UART0;
            break;
        case c_Uart_1:
            return LPC24XX_VIC::c_IRQ_INDEX_UART1;
            break;
        case c_Uart_2:
            return LPC24XX_VIC::c_IRQ_INDEX_UART2;
            break;
        case c_Uart_3:
            return LPC24XX_VIC::c_IRQ_INDEX_UART3;
            break;
        default:
            ASSERT(TRUE);
            return(ComPortNum);
            break;
        }
    }
};
//
// UART
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Pin Connect Block
//
struct LPC24XX_PCB
{
    static const UINT32 c_PCB_Base = 0xE002C000;
    
    struct 
    {
        /****/ volatile UINT32 PINSEL;    // Pin Configuration Register
    } Regs[10];
    static const UINT32 PINSEL2_PWM0_GPIO_P1_2  = (0x11 << 4);
    static const UINT32 PINSEL7_PWM1_GPIO_P3_24 = (0x11 << 4);  
    
    /****/ volatile UINT32 PINSEL10;      // ETM Interface Pin control

    /****/ volatile UINT32 PINSEL11;      // LCD function and mode control
    
    /****/ volatile UINT32 dummy[4];      // Filler to align address

    struct 
    {
        /****/ volatile UINT32 PINMODE;   // Pin Mode Register
    } Regsm[10];
    
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
struct LPC24XX_SYSCON
{
    static const UINT32 c_SYSCON_Base = 0xE01FC000;

    // MAM Registers
    /****/ volatile UINT32 MAMCR;         // MAM Control register
    /****/ volatile UINT32 MAMTIM;        // MAM Timing register

    /****/ volatile UINT32 dummy0[14];    // Filler to align next register address
    
    // Memory Mapping Control
    /****/ volatile UINT32 MEMMAP;        // Memory map control register

    /****/ volatile UINT32 dummy1[15];    // Filler to align next register address

    // Phased Lock Loop
    /****/ volatile UINT32 PLLCON;        // PLL Control Register
    static const UINT32 PLLE = 0x1;       // Enable PLL
    static const UINT32 PLLC = 0x2;       // Connect PLL
    
    /****/ volatile UINT32 PLLCFG;        // PLL Configuration Register

    /****/ volatile UINT32 PLLSTAT;       // PLL Status Register
    static const UINT32 ENBLD = (0x1<<24);  // PLL is enabled
    static const UINT32 CNCTD = (0x1<<25);  // PLL is connected
    static const UINT32 LOCKD = (0x1<<26);  // PLL is locked

    /****/ volatile UINT32 PLLFEED;       // PLL Feed Register

    /****/ volatile UINT32 dummy2[12];    // Filler to align next register address

    // Power Control
    /****/ volatile UINT32 PCON;          // Power Control Register
    /****/ volatile UINT32 PCONP;         // Power Control for Peripherals
    static const UINT32 ENABLE_ENET  = 0x40000000;
    static const UINT32 ENABLE_LCD   = 0x00100000;
    static const UINT32 ENABLE_PWM0  = 0x00000020;
    static const UINT32 ENABLE_PWM1  = 0x00000040;
    /****/ volatile UINT32 dummy3[15];    // Filler to align next register address

    // Clock Control
    /****/ volatile UINT32 CCLKCFG;        // CPU Clock Configuration Register
    /****/ volatile UINT32 USBCLKCFG;      // USB Clock Configuration Register

    /****/ volatile UINT32 CLKSRCSEL;      // Clock Source Select Register
    static const UINT32 IRC = 0x0;         // Internal RC Oscillator
    static const UINT32 OSC = 0x1;         // Main Oscillator
    static const UINT32 RTC = 0x2;         // RTC Oscillator

    /****/ volatile UINT32 dummy4[12];    // Filler to align next register address
    
    // External Interrupts
    /****/ volatile UINT32 EXTINT;        // External Interrupt Flag Register
    /****/ volatile UINT32 EXTWAKE;       // External Interrupt Wakeup Register
    /****/ volatile UINT32 EXTMODE;       // External Interrupt Mode Register
    /****/ volatile UINT32 EXTPOLAR;      // External Interrupt Polarity Register

    /****/ volatile UINT32 dummy5[12];    // Filler to align next register address

    /****/ volatile UINT32 RSID;          // Reset Source Identification Register    

    /****/ volatile UINT32 dummy6[7];     // Filler to align next register address

    // System control and status
    /****/ volatile UINT32 SCS;           // System Control and Status
    static const UINT32 HS_IO = (0x1<<0); // Enable High speed gpio on port 0 and 1  
    static const UINT32 OSCEN = (0x1<<5); // Oscillator enable
    static const UINT32 READY = (0x1<<6); // Oscillator is ready to use
    

    // IRC trim
    /****/ volatile UINT32 IRCTRIM;       // IRC Trim Register
    // Peripheral clock control
    /****/ volatile UINT32 PCLKSEL0;      // Peripheral Clock Selection register 0
    static const UINT32 PCLK_CLK_DIV_1       = 0x01;
    static const UINT32 PCLK_CLK_DIV_2       = 0x02;
    static const UINT32 PCLK_CLK_DIV_4       = 0x00;
    static const UINT32 PCLK_CLK_DIV_8       = 0x03;

    static const UINT32 PCLK_UART0_SHIFT     = 6;    
    static const UINT32 PCLK_UART0_MASK      = (0x03ul << PCLK_UART0_SHIFT);
    static const UINT32 PCLK_UART1_SHIFT     = 8;    
    static const UINT32 PCLK_UART1_MASK      = (0x03ul << PCLK_UART0_SHIFT);


    /****/ volatile UINT32 PCLKSEL1;      // Peripheral Clock Selection register 1    
    static const UINT32 PCLK_PWM0_SHIFT     = 10;    
    static const UINT32 PCLK_PWM0_MASK      = (0x03 << PCLK_PWM0_SHIFT);
    static const UINT32 PCLK_PWM0_CLK_DIV_1 = (0x01 << PCLK_PWM0_SHIFT);
    static const UINT32 PCLK_PWM0_CLK_DIV_2 = (0x02 << PCLK_PWM0_SHIFT);
    static const UINT32 PCLK_PWM0_CLK_DIV_4 = (0x00 << PCLK_PWM0_SHIFT);
    static const UINT32 PCLK_PWM0_CLK_DIV_8 = (0x03 << PCLK_PWM0_SHIFT);

    static const UINT32 PCLK_PWM1_SHIFT     = 12;    
    static const UINT32 PCLK_PWM1_MASK      = (0x03 << PCLK_PWM1_SHIFT);
    static const UINT32 PCLK_PWM1_CLK_DIV_1 = (0x01 << PCLK_PWM1_SHIFT);
    static const UINT32 PCLK_PWM1_CLK_DIV_2 = (0x02 << PCLK_PWM1_SHIFT);
    static const UINT32 PCLK_PWM1_CLK_DIV_4 = (0x00 << PCLK_PWM1_SHIFT);
    static const UINT32 PCLK_PWM1_CLK_DIV_8 = (0x03 << PCLK_PWM1_SHIFT);

    static const UINT32 PCLK_UART2_SHIFT     = 16;    
    static const UINT32 PCLK_UART2_MASK      = (0x03ul << PCLK_UART0_SHIFT);
    static const UINT32 PCLK_UART3_SHIFT     = 18;    
    static const UINT32 PCLK_UART3_MASK      = (0x03ul << PCLK_UART0_SHIFT);



    /****/ volatile UINT32 dummy7[2];     // Filler to align next register address   
    //LCD clock Divider
     /****/ volatile UINT32 LCD_CFG;       // Filler to align next register address
};
//
// System Control Block
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// External Memory Controller
//
struct LPC24XX_EMC
{
    static const UINT32 c_EMC_Base = 0xFFE08000;
    
    /****/ volatile UINT32 CONTROL;       // Controls operation of the memory controller. 0x1 0x3 R/W
    /****/ volatile UINT32 STATUS;        // PROVIDES  status information. - 0x5 RO
    /****/ volatile UINT32 CONFIG;        // CONFIGUREs operation of the memory controller - 0x0 R/W

    /****/ volatile UINT32 dummy0[5];     // Filler to align next register address

    /****/ volatile UINT32 DCONTROL;      // Controls Dynamic memory operation. - 0x006 R/W
    /****/ volatile UINT32 DREFRESH;      // Configures Dynamic memory refresh operation. - 0x0 R/W
    /****/ volatile UINT32 DREADCONFIG;   // Configures the Dynamic memory read strategy. - 0x0 R/W

    /****/ volatile UINT32 dummy1;        // Filler to align next register address

    /****/ volatile UINT32 DRP;           // Selects the precharge command period. - 0x0F R/W
    /****/ volatile UINT32 DRAS;          // Selects the active to precharge command period. - 0xF R/W
    /****/ volatile UINT32 DSREX;         // Selects the self-refresh exit time. - 0xF R/W
    /****/ volatile UINT32 DAPR;          // Selects the last-data-out to active command time. - 0xF R/W
    /****/ volatile UINT32 DDAL;          // Selects the data-in to active command time. - 0xF R/W
    /****/ volatile UINT32 DWR;           // Selects the write recovery time. - 0xF R/W
    /****/ volatile UINT32 DRC;           // Selects the active to active command period. - 0x1F R/W
    /****/ volatile UINT32 DRFC;          // Selects the auto-refresh period. - 0x1F R/W
    /****/ volatile UINT32 DXSR;          // Selects the exit self-refresh to active command time. - 0x1F R/W
    /****/ volatile UINT32 DRRD;          // Selects the active bank A to active bank B latency. - 0xF R/W
    /****/ volatile UINT32 DMRD;          // Selects the load mode register to active command time. - 0xF R/W

    /****/ volatile UINT32 dummy2[9];     // Filler to align next register address

    /****/ volatile UINT32 SEXTENDEDWAIT; // Selects time for long Static memory read and write transfers. - 0x0 R/W

    /****/ volatile UINT32 dummy3[31];    // Filler to align next register address

    /****/ volatile UINT32 DCONFIG0;      // Selects the configuration information for Dynamic memory chip select 0.- 0x0 R/W
    /****/ volatile UINT32 DRASCAS0;      // Selects the RAS and CAS latencies for Dynamic memory chip select 0.- 0x303 R/W

    /****/ volatile UINT32 dummy4[6];     // Filler to align next register address

    /****/ volatile UINT32 DCONFIG1;      // Selects the configuration information for Dynamic memory chip select 1.- 0x0 R/W
    /****/ volatile UINT32 DRASCAS1;      // Selects the RAS and CAS latencies for Dynamic memory chip select 1.- 0x303 R/W

    /****/ volatile UINT32 dummy5[6];     // Filler to align next register address

    /****/ volatile UINT32 DCONFIG2;      // Selects the configuration information for Dynamic memory chip select 2. - 0x0 R/W 
    /****/ volatile UINT32 DRASCAS2;      // Selects the RAS and CAS latencies for Dynamic memorychip select 2. - 0x303 R/W

    /****/ volatile UINT32 dummy6[6];     // Filler to align next register address

    /****/ volatile UINT32 DCONFIG3;      // Selects the configuration information for Dynamic memory chip select 3. - 0x0 R/W
    /****/ volatile UINT32 DRASCAS3;      // Selects the RAS and CAS latencies for Dynamic memory chip select 3. - 0x303 R/W

    /****/ volatile UINT32 dummy7[38];    // Filler to align next register address

    /****/ volatile UINT32 SCONFIG0;      // Selects the memory configuration for Static chip select 0. - 0x0 R/W
    /****/ volatile UINT32 SWAITWEN0;     // Selects the delay from chip select 0 to write enable. - 0x0 R/W
    /****/ volatile UINT32 SWAITOEN0;     // Selects the delay from chip select 0 or address change, whichever is later, to output enable. - 0x0 R/W
    /****/ volatile UINT32 SWAITRD0;      // Selects the delay from chip select 0 to a read access. - 0x1F R/W
    /****/ volatile UINT32 SWAITPAGE0;    // Selects the delay for asynchronous page mode sequential accesses for chip select 0. - 0x1F R/W
    /****/ volatile UINT32 SWAITWR0;      // Selects the delay from chip select 0 to a write access. - 0x1F R/W
    /****/ volatile UINT32 SWAITTURN0;    // Selects the number of bus turnaround cycles for chip select 0. - 0xF R/W

    /****/ volatile UINT32 dummy8;        // Filler to align next register address
    /****/ volatile UINT32 SCONFIG1;      // Selects the memory configuration for Static chip select 1. - 0x0 R/W
    /****/ volatile UINT32 SWAITWEN1;     // Selects the delay from chip select 1 to write enable. - 0x0 R/W
    /****/ volatile UINT32 SWAITOEN1;     // Selects the delay from chip select 1 or address change, whichever is later, to output enable. - 0x0 R/W
    /****/ volatile UINT32 SWAITRD1;      // Selects the delay from chip select 1 to a read access. - 0x1F R/W
    /****/ volatile UINT32 SWAITPAGE1;    // Selects the delay for asynchronous page mode sequential accesses for chip select 1. - 0x1F R/W
    /****/ volatile UINT32 SWAITWR1;      // Selects the delay from chip select 1 to a write access. - 0x1F R/W
    /****/ volatile UINT32 SWAITTURN1;    // Selects the number of bus turnaround cycles for chip select 1. - 0xF R/W

    /****/ volatile UINT32 dummy9;        // Filler to align next register address
    /****/ volatile UINT32 SCONFIG2;      // Selects the memory configuration for Static chip select 2. - 0x0 R/W
    /****/ volatile UINT32 SWAITWEN2;     // Selects the delay from chip select 2 to write enable. - 0x0 R/W
    /****/ volatile UINT32 SWAITOEN2;     // Selects the delay from chip select 2 or address change, whichever is later, to output enable. - 0x0 R/W
    /****/ volatile UINT32 SWAITRD2;      // Selects the delay from chip select 2 to a read access. - 0x1F R/W
    /****/ volatile UINT32 SWAITPAGE2;    // Selects the delay for asynchronous page mode sequential accesses for chip select 2. - 0x1F R/W
    /****/ volatile UINT32 SWAITWR2;      // Selects the delay from chip select 2 to a write access. - 0x1F R/W
    /****/ volatile UINT32 SWAITTURN2;    // Selects the number of bus turnaround cycles for chip select 2. - 0xF R/W

    /****/ volatile UINT32 dummy10;        // Filler to align next register address
    /****/ volatile UINT32 SCONFIG3;      // Selects the memory configuration for Static chip select 3. - 0x0 R/W
    /****/ volatile UINT32 SWAITWEN3;     // Selects the delay from chip select 3 to write enable. - 0x0 R/W
    /****/ volatile UINT32 SWAITOEN3;     // Selects the delay from chip select 3 or address change, whichever is later, to output enable. - 0x0 R/W
    /****/ volatile UINT32 SWAITRD3;      // Selects the delay from chip select 3 to a read access. - 0x1F R/W
    /****/ volatile UINT32 SWAITPAGE3;    // Selects the delay for asynchronous page mode sequential accesses for chip select 3. - 0x1F R/W
    /****/ volatile UINT32 SWAITWR3;      // Selects the delay from chip select 3 to a write access. - 0x1F R/W
    /****/ volatile UINT32 SWAITTURN3;    // Selects the number of bus turnaround cycles for chip select 3. - 0xF R/W
};
//
// External Memory Controller
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// SPI
//
struct LPC24XX_SPI
{
    static const UINT32 c_SPI0_Base     = 0xE0020000;
    static const UINT32 c_SPI_Clk_KHz   = LPC24XX_SPI_PCLK_KHZ;

    static const UINT32 c_SPI0 = 0;
    static const UINT32 c_MAX_SPI = 1;

    static const UINT32 c_SPI0_SCLK = LPC24XX_GPIO::c_P0_15;
    static const UINT32 c_SPI0_MISO = LPC24XX_GPIO::c_P0_17;
    static const UINT32 c_SPI0_MOSI = LPC24XX_GPIO::c_P0_18;

    /****/ volatile UINT32 SPCR;
    
//  MSB first always. SPI config structure does not contain a field for the bit order
    static const UINT32 CONTROLREG_BitEnable     = 0x00000004;
    static const UINT32 CONTROLREG_MODE_Master   = 0x00000020;
    static const UINT32 CONTROLREG_PHA_1         = 0x00000008;
    static const UINT32 CONTROLREG_POL_1         = 0x00000010;

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
struct LPC24XX_I2C
{
    static const UINT32 c_I2C_Base     = 0xE001C000;
    static const UINT32 c_I2C_Clk_KHz  = LPC24XX_I2C_PCLK_KHZ;

    static const UINT32 c_I2C_SDA = LPC24XX_GPIO::c_P0_27;
    static const UINT32 c_I2C_SCL = LPC24XX_GPIO::c_P0_28;

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
struct LPC24XX_WATCHDOG
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


//////////////////////////////////////////////////////////////////////////////
// DAC
//
struct LPC24XX_DAC
{
    /****/ volatile UINT32 DACR;
    
    static const UINT32 c_DAC_Base        = 0xE006C000;
    static const UINT32 c_DAC_AOUT        = LPC24XX_GPIO::c_P0_26;
    
    static const UINT32 Timer    = LPC24XX_TIMER::c_Timer_1;
    
    //to zero out reserved bits and bias bit
    static const UINT32 VALUE_MASK        = 0x0000FFC0;
    // 0x0001 0000 for maximum output current.
    // 0x0000 0000 for faster settle time, but half max output
    static const UINT32 TRADE_SPEED_FOR_POWER = 0x00010000;

};

//
// DAC
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// PWM
//
struct LPC24XX_PWM
{
    static const UINT32 c_PWM_Base_0        = 0xE0014000;
    static const UINT32 c_PWM_Base_1        = 0xE0018000;

    //--//

    /****/ volatile UINT32 IR;
    
    /****/ volatile UINT32 TCR;
    static const UINT32    TCR_COUNTER_ENABLE = 0x00000001;
    static const UINT32    TCR_COUNTER_RESET  = 0x00000002;
    static const UINT32    TCR_PWM_ENABLE     = 0x00000008;

    /****/ volatile UINT32 TC;

    /****/ volatile UINT32 PR;

    /****/ volatile UINT32 PC;

    /****/ volatile UINT32 MCR;
    static const UINT32    MCR_MR0_INTERRUPT = 0x00000001;
    static const UINT32    MCR_MR0_RESET_TC  = 0x00000002;
    static const UINT32    MCR_MR0_STOP_TC   = 0x00000004;
        
    /****/ volatile UINT32 MR0;

    /****/ volatile UINT32 MR1;

    /****/ volatile UINT32 MR2;

    /****/ volatile UINT32 MR3;

    /****/ volatile UINT32 CCR;

    /****/ volatile UINT32 CR0;

    /****/ volatile UINT32 CR1;

    /****/ volatile UINT32 dummy1[3];

    /****/ volatile UINT32 MR4;

    /****/ volatile UINT32 MR5;

    /****/ volatile UINT32 MR6;

    /****/ volatile UINT32 PCR;
    static const UINT32    PCR_ENABLE_1 = 0x00000200;
    static const UINT32    PCR_ENABLE_2 = 0x00000400;

    /****/ volatile UINT32 LER;
    static const UINT32    LER_ENABLE_0     = 0x00000001;
    static const UINT32    LER_ENABLE_1     = 0x00000002;
    static const UINT32    LER_ENABLE_2     = 0x00000004;
    static const UINT32    LER_ENABLE_3     = 0x00000008;
    static const UINT32    LER_ENABLE_4     = 0x00000010;
    static const UINT32    LER_ENABLE_5     = 0x00000020;
    static const UINT32    LER_ENABLE_6     = 0x00000040;

    /****/ volatile UINT32 dummy2[7];

    /****/ volatile UINT32 CTCR;
    
};

//
// PWM
//////////////////////////////////////////////////////////////////////////////

/**********************************************************************************************************************/
/**********************************************************************************************************************/
/**********************************************************************************************************************/
/**********************************************************************************************************************/
/**********************************************************************************************************************/

struct LPC24XX
{
    static LPC24XX_VIC    & VIC    (         ) { return *(LPC24XX_VIC*     )(size_t)( LPC24XX_VIC     ::c_VIC_Base      ); }
    static LPC24XX_GPIO   & GPIO   (         ) { return *(LPC24XX_GPIO*    )(size_t)( LPC24XX_GPIO    ::c_GPIO_Base     ); }
    static LPC24XX_GPIOIRQ& GPIOIRQ(         ) { return *(LPC24XX_GPIOIRQ* )(size_t)( LPC24XX_GPIOIRQ ::c_GPIOIRQ_Base  ); }
    static LPC24XX_PCB    & PCB    (         ) { return *(LPC24XX_PCB*     )(size_t)( LPC24XX_PCB     ::c_PCB_Base      ); }
    static LPC24XX_SYSCON & SYSCON (         ) { return *(LPC24XX_SYSCON*  )(size_t)( LPC24XX_SYSCON  ::c_SYSCON_Base   ); }
    static LPC24XX_EMC    & EMC    (         ) { return *(LPC24XX_EMC*     )(size_t)( LPC24XX_EMC     ::c_EMC_Base      ); }
    static LPC24XX_SPI    & SPI    ( int sel ) { return *(LPC24XX_SPI*     )(size_t)( LPC24XX_SPI     ::c_SPI0_Base     ); }
    static LPC24XX_I2C    & I2C    (         ) { return *(LPC24XX_I2C*     )(size_t)( LPC24XX_I2C     ::c_I2C_Base      ); }
    static LPC24XX_WATCHDOG & WTDG (         ) { return *(LPC24XX_WATCHDOG*)(size_t)( LPC24XX_WATCHDOG::c_WATCHDOG_Base ); }
    static LPC24XX_DAC      & DAC  (         ) { return *(LPC24XX_DAC*     )(size_t)( LPC24XX_DAC     ::c_DAC_Base      ); }
    static LPC24XX_PWM      & PWM  ( int sel ) 
    { 
        if(sel == PWM_CHANNEL_0)
        {
            return *(LPC24XX_PWM*)(size_t)LPC24XX_PWM::c_PWM_Base_0;
        }
        else 
        {
            return *(LPC24XX_PWM*)(size_t)LPC24XX_PWM::c_PWM_Base_1;
        }
    }
    
   
    static LPC24XX_TIMER  & TIMER( int sel )
    { 
        ASSERT(sel <=LPC24XX_TIMER::c_MaxTimer_no)

        if (sel == LPC24XX_TIMER::c_Timer_0)
             return *(LPC24XX_TIMER  *)(size_t)LPC24XX_TIMER::c_TimerBase_0;
        else if (sel == LPC24XX_TIMER::c_Timer_1)
             return *(LPC24XX_TIMER  *)(size_t)LPC24XX_TIMER::c_TimerBase_1;
        else if (sel == LPC24XX_TIMER::c_Timer_2)
             return *(LPC24XX_TIMER  *)(size_t)LPC24XX_TIMER::c_TimerBase_2;
        else 
            return *(LPC24XX_TIMER  *)(size_t)LPC24XX_TIMER::c_TimerBase_3;
    }

    static LPC24XX_USART  & UART( int sel )
    { 
        ASSERT(sel <=LPC24XX_USART::c_MaxUart_no)
        if (sel == LPC24XX_USART::c_Uart_0)
             return *(LPC24XX_USART  *)(size_t)LPC24XX_USART::c_UartBase_0;
        else if (sel == LPC24XX_USART::c_Uart_1)
             return *(LPC24XX_USART  *)(size_t)LPC24XX_USART::c_UartBase_1;
        else if (sel == LPC24XX_USART::c_Uart_2)
             return *(LPC24XX_USART  *)(size_t)LPC24XX_USART::c_UartBase_2;
        else 
             return *(LPC24XX_USART  *)(size_t)LPC24XX_USART::c_UartBase_3;
    }
//--//
};

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// LPC24XX driver
//

struct LPC24XX_Driver
{
    static const UINT32 c_SystemTime_Timer         = LPC24XX_TIMER::c_Timer_0;

    BOOL             m_PowerOnReset;

    //--//

    static BOOL Initialize();

    static void Pause();

    static void Sleep();

    static void Halt();

    static void Reset();

    static void Shutdown();

    static void Hibernate();

    static BOOL PowerOnReset();

    static BOOL PowerOnReset_STUB();

    static void DisplayProcessorInformation();
};

extern LPC24XX_Driver g_LPC24XX_Driver;

//
// LPC24XX driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Interrupt Controller driver
//

struct LPC24XX_VIC_Driver
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
    static BOOL DeactivateInterrupt( UINT32 Irq_Index );

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

struct LPC24XX_TIMER_Driver
{
    const static UINT32 c_MaxTimers = 4;

    //--//

    BOOL m_configured[c_MaxTimers];

    static BOOL   Initialize   ( UINT32 Timer, HAL_CALLBACK_FPN ISR, void* ISR_Param );
    static BOOL   Uninitialize ( UINT32 Timer                                                                                              );
    static UINT32 ReadCounter  ( UINT32 Timer                                                                                              );

    static void EnableCompareInterrupt( UINT32 Timer )
    {
        ASSERT(Timer < c_MaxTimers);

        LPC24XX_TIMER& TIMER = LPC24XX::TIMER( Timer );

        TIMER.MCR |= LPC24XX_TIMER::MR0_IRQEN;
    }

    static void DisableCompareInterrupt( UINT32 Timer )
    {
        ASSERT(Timer < c_MaxTimers);

        LPC24XX_TIMER& TIMER = LPC24XX::TIMER( Timer );

        TIMER.MCR &= ~LPC24XX_TIMER::MR0_IRQEN;
    }

    static void ForceInterrupt( UINT32 Timer )
    {
        ASSERT(Timer < c_MaxTimers);

        LPC24XX::VIC().ForceInterrupt( LPC24XX_TIMER::getIntNo(Timer) );
    }

    static void SetCompare( UINT32 Timer, UINT32 Compare )
    {
        ASSERT(Timer < c_MaxTimers);

        LPC24XX_TIMER& TIMER = LPC24XX::TIMER( Timer );

        TIMER.MR0 = Compare;
    }

    static BOOL DidCompareHit( UINT32 Timer )
    {
        ASSERT(Timer < c_MaxTimers);

        LPC24XX_TIMER& TIMER = LPC24XX::TIMER( Timer );

        return (TIMER.IR & LPC24XX_TIMER::MR0_COMP) != 0;
    }

    static void ResetCompareHit( UINT32 Timer )
    {
        ASSERT(Timer < c_MaxTimers);

        LPC24XX_TIMER& TIMER = LPC24XX::TIMER( Timer );

        TIMER.IR = LPC24XX_TIMER::MR0_RESET;
    }
};

extern LPC24XX_TIMER_Driver g_LPC24XX_TIMER_Driver;

//
// TIMER driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// System Time driver
//

struct LPC24XX_TIME_Driver
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

extern LPC24XX_TIME_Driver g_LPC24XX_TIME_Driver;

//
// System Time driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// UART driver
//

struct LPC24XX_USART_Driver
{
    static const UINT32 c_RefFlagRx = 0x01;
    static const UINT32 c_RefFlagTx = 0x02;

    static const UINT32 c_COM1      = 0;
    static const UINT32 c_COM2      = 1;
    static const UINT32 c_COM3      = 2;
    static const UINT32 c_COM4      = 3;


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

extern LPC24XX_USART_Driver g_LPC24XX_USART_Driver;

//
// UART driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// GPIO driver
//

struct LPC24XX_GPIO_Driver
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
        UINT32                        m_lastExecTicks;

        //--//
        
        static const UINT8 c_Flags_Debounce        = 0x01;
        
        // we are not using these
        
        /*static const UINT8 c_Flags_RequireLowEdge  = 0x02;
        static const UINT8 c_Flags_RequireHighEdge = 0x04;

        static const UINT8 c_Status_AllowLowEdge   = 0x01;
        static const UINT8 c_Status_AllowHighEdge  = 0x02;*/

        //--//

        static void Fire( void* arg );

        void HandleDebounce( BOOL edge );
    };

    //--//

    static const UINT32 c_MaxPins           =  160;
    static const UINT32 c_MaxPorts          =   5;
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
    
    static void GPIO_ISR( void* Param );

/*    static void PortConfiguration( UINT32 Port, UINT32 ConfigValue );

    static void PortDirection( UINT32 Port, UINT32 DirValue ); 
    
    static UINT32 GetPinBynIRQ( UINT32 nIRQ );   
    
    static UINT32 GetnIRQByPin( UINT32 Pin ); 
    
    static BOOL   SetnIRQISR  ( GPIO_PIN pin, HAL_CALLBACK_FPN ISR, GPIO_INT_EDGE intEdge, GPIO_RESISTOR resistorState ) ;*/
    
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

    //static void nIRQ_ISR( void* Param );
};

extern LPC24XX_GPIO_Driver g_LPC24XX_GPIO_Driver;

//
// GP I/O driver
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// SPI  driver
//

struct LPC24XX_SPI_Driver
{
    BOOL m_Enabled[LPC24XX_SPI::c_MAX_SPI];

    static BOOL Initialize  ();
    static void Uninitialize();

    static void GetPins         ( UINT32 spi_mod, GPIO_PIN& msk, GPIO_PIN& miso, GPIO_PIN& mosi );

    static BOOL nWrite16_nRead16( const SPI_CONFIGURATION& Configuration, UINT16* Write16, INT32 WriteCount, UINT16* Read16, INT32 ReadCount, INT32 ReadStartOffset );
    static BOOL nWrite8_nRead8  ( const SPI_CONFIGURATION& Configuration, UINT8*  Write8 , INT32 WriteCount, UINT8*  Read8 , INT32 ReadCount, INT32 ReadStartOffset );

    static BOOL Xaction_Start( const SPI_CONFIGURATION& Configuration );
    static BOOL Xaction_Stop ( const SPI_CONFIGURATION& Configuration );

    static BOOL Xaction_nWrite16_nRead16( SPI_XACTION_16& Transaction );
    static BOOL Xaction_nWrite8_nRead8  ( SPI_XACTION_8&  Transaction );

private:
    static void ISR( void* Param );
};

extern LPC24XX_SPI_Driver g_LPC24XX_SPI_Driver;

//
// SPI driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// I2C driver
//

struct I2C_HAL_XACTION_UNIT;
struct I2C_HAL_XACTION;
struct I2C_CONFIGURATION;

struct LPC24XX_I2C_Driver
{
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

extern LPC24XX_I2C_Driver g_LPC24XX_I2C_Driver;

//
// I2C driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Watchdog driver
//

struct LPC24XX_WATCHDOG_Driver


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

extern LPC24XX_WATCHDOG_Driver g_LPC24XX_WATCHDOG_Driver;


//
// Watchdog driver
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// DAC driver
//


/* To allow efficient integration with the Helix decoder, the DAC buffer is divided into N
 * FRAME buffers, each as large as the maximum allowed MP3 frame size. A frame can be shorter for lower quality
 * files, therefore we keep the size of each framebuffer in an array.
 * The ISR will jump to the next FRAME buffer if it reaches the end of this one.
 * You always add a FRAME at a time.
 * The driver expects 16bit SIGNED INTEGERS as samples.
 */
 
 //Maximum size in bytes of a decoded MP3 Frame
#define MAX_DECODED_FRAME_SIZE            0x900
 
#define DAC_FRAME_BUFFERS_NUM             16
//a sample is 2 bytes
#define DAC_FRAME_BUFFER_SIZE_SAMPLES     MAX_DECODED_FRAME_SIZE/2

struct LPC24XX_DAC_Driver
{
    static void Initialize  ( UINT32 SampleFrequencyHz );
    
    static void Uninitialize();

    //enable timer interrupts for output
    static BOOL On();
    
    //disable timer interrupts
    static BOOL Off();
    
    //give info on whether the DAC is or is not converting samples
    static BOOL IsEnabled();
    
    //Samples left in the buffer
    static UINT32 GetBufferLevel();
    
    //Max SampleBuffer Capacity (in 16bit samples)
    static UINT32 GetBufferCapacity();
    
    //FRAMES left in the buffer
    static UINT32 GetFramesLeft();
    
    //Max SampleBuffer Capacity (in FRAMES)
    static UINT32 GetBufferFrameCapacity();
    
    //return false if there was not enough space left
    static BOOL AddFrame(short const * const Samples, UINT32 const SamplesNum);



    short SamplesBuffer[DAC_FRAME_BUFFERS_NUM*DAC_FRAME_BUFFER_SIZE_SAMPLES];
    UINT16 SamplesInFrame[DAC_FRAME_BUFFERS_NUM];
    UINT32 nextFrameWrite;
    UINT32 nextFrameRead;
    UINT32 nextSampleRead;
    UINT32 SampleCount;
    UINT32 FrameCount;
    UINT32 SampleTimeInCycles;
    
    //output next sample
    static void ISR( void* Param );
};

extern LPC24XX_DAC_Driver g_LPC24XX_DAC_Driver;


//
// DAC driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// SOCKET driver
// 

struct LPC24XX_SOCKETS_Driver
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

    static int GetNativeSockOption (int level, int optname);

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
};

//
// SOCKET driver
/////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////
// PWM driver
//

struct LPC24XX_PWM_Driver
{
    // for this specific processor, we will use the channel as a synomim for controller, and proceed from there    
    
    static const UINT32 c_Channels = 2;

    static const GPIO_PIN c_Channel_0_Pin1 = LPC24XX_GPIO::c_P1_02; 
    static const GPIO_PIN c_Channel_0_Pin2 = LPC24XX_GPIO::c_P1_03; // use this for channel 0
    static const GPIO_PIN c_Channel_0_Pin3 = LPC24XX_GPIO::c_P1_05; 
    static const GPIO_PIN c_Channel_0_Pin4 = LPC24XX_GPIO::c_P1_06; 
    static const GPIO_PIN c_Channel_0_Pin5 = LPC24XX_GPIO::c_P1_07; 
    static const GPIO_PIN c_Channel_0_Pin6 = LPC24XX_GPIO::c_P1_11; 
    
    static const GPIO_PIN c_Channel_1_Pin1 = LPC24XX_GPIO::c_P1_18; // use this for channel 1
    static const GPIO_PIN c_Channel_1_Pin2 = LPC24XX_GPIO::c_P1_20; 
    static const GPIO_PIN c_Channel_1_Pin3 = LPC24XX_GPIO::c_P1_21; 
    static const GPIO_PIN c_Channel_1_Pin4 = LPC24XX_GPIO::c_P1_23; 
    static const GPIO_PIN c_Channel_1_Pin5 = LPC24XX_GPIO::c_P1_24; 
    static const GPIO_PIN c_Channel_1_Pin6 = LPC24XX_GPIO::c_P1_26; 

    //-//
    
    static BOOL     Initialize        ( PWM_CHANNEL channel );
    static BOOL     Uninitialize      ( PWM_CHANNEL channel );
    static BOOL     ApplyConfiguration( PWM_CHANNEL channel, GPIO_PIN pin, UINT32& period, UINT32& duration, PWM_SCALE_FACTOR& scale, BOOL invert );
    static BOOL     Start             ( PWM_CHANNEL channel, GPIO_PIN pin );
    static void     Stop              ( PWM_CHANNEL channel, GPIO_PIN pin );
    static BOOL     Start             ( PWM_CHANNEL* channel, GPIO_PIN* pin, UINT32 count );
    static void     Stop              ( PWM_CHANNEL* channel, GPIO_PIN* pin, UINT32 count );
    static UINT32   Channels          ( );
    static GPIO_PIN GetPinForChannel  ( PWM_CHANNEL channel );

    //--//

    static void EnablePin( PWM_CHANNEL channel, GPIO_PIN pin );
    static void DisablePin( PWM_CHANNEL channel, GPIO_PIN pin );
};

//
// PWM driver
/////////////////////////////////////////////////////////////////////////////////////////////////


#endif // _LPC24XX_H_ 1
