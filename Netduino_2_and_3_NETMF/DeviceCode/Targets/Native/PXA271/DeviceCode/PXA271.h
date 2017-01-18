////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PXA271_H_
#define _PXA271_H_ 1

#include <cores\arm\include\cpu.h>


/////////////////////////////////////////////////////////////////////////////////////////////////
extern "C"
{
void CPWAIT(int reg);
void CPU_CPWAIT();

void BOOT_CPWAIT();
void BOOT_FlushCaches();

}

/////////////////////////////////////////////////////////////////////////////////////////////////
// LAYOUT SECTION
//
//
//////////////////////////////////////////////////////////////////////////////
// Power Manager
//
struct PXA271_POWER_MANAGER
{
   static const UINT32 c_Base = 0x40F00000;

    /****/ volatile UINT32 PMCR;                                   // power manager control register
    static const    UINT32 PMCR__INTRS                 = 0x00000020; // Interrupt Status
    static const    UINT32 PMCR__IAS                   = 0x00000010; // Interrupt/Abort Select
    static const    UINT32 PMCR__VIDAS                 = 0x00000008; // Imprecise-Data-Abort Status for nVdd Fault
    static const    UINT32 PMCR__VIDAE                 = 0x00000004; // Imprecise-Data Abort Enable for nVdd Fault
    static const    UINT32 PMCR__BIDAS                 = 0x00000002; // Imprecise-Data Abort Status for nBatt Fault
    static const    UINT32 PMCR__BIDAE                 = 0x00000001; // Imprecise-Data Abort Enable for nBatt Fault

    /****/ volatile UINT32 PSSR;                                   // Power Manager Sleep Status Register
    static const    UINT32 PSSR__OTGPH                 = 0x00000040; // OTG periphaeral control hold
    static const    UINT32 PSSR__RDH                   = 0x00000020; // Read disable hold
    static const    UINT32 PSSR__PH                    = 0x00000010; // Peripheral Control Hold
    static const    UINT32 PSSR__STS                   = 0x00000008; // Standby Mode Status
    static const    UINT32 PSSR__VFS                   = 0x00000004; // VCC Fault Status
    static const    UINT32 PSSR__BFS                   = 0x00000002; // Battery Fault Status
    static const    UINT32 PSSR__SSS                   = 0x00000001; // Software Sleep Status

    /****/ volatile UINT32 PSPR;                                  // Power Manager Scratch Pad Register
    static const    UINT32 PSPR__MASK                  = 0xFFFFFFFF; // Scatch Pad Register 32 bit

    /****/ volatile UINT32 PWER;                                  // Power Manager Wake=up Enable Register 
    static const    UINT32 PWER__WERTC                 = 0x80000000; // Wake-up enable for RTC standby, sleep, or deep-sleep mode
    static const    UINT32 PWER__WEP                   = 0x40000000; // Wkae-up enable for PI power Domain Standby or sleep mode
    static const    UINT32 PWER__WEUSBH2               = 0x10000000; // Wake-up enable for USB host port 2 Standby or sleep mode
    static const    UINT32 PWER__WEUSBH1               = 0x08000000; // Wake-up enable for USB host port 1 Standby or sleep mode
    static const    UINT32 PWER__WEUSBB                = 0x04000000; // Wake-up enable for USB client port Standby or sleep mode
    static const    UINT32 PWER__WBB                   = 0x02000000; // Wake-up enable for a risigin edge from MSL (GPIO<83>) for standby or sleep mode
    static const    UINT32 PWER__WE35                  = 0x01000000; // Wake-up Enable for standby or sleep mode
    static const    UINT32 PWER__WEUSIM                = 0x00800000; // Wake-up enable or rising or falling edge from UDET <GPIO<16>) for standby or sleep mode
    static const    UINT32 PWER__WEMUX3_MASK           = 0x00180000; // Wake=up enable for GPIO 13 or GPIO 113 for Standby and sleep
    static const    UINT32 PWER__WEMUX3_SHIFT          =         19; // 
    static const    UINT32 PWER__WEMUX3_NO_WAKEUP_ENABLED    =    0; // 
    static const    UINT32 PWER__WEMUX3_ENA_WKUP_DUE_GPIO13  =    1; // 
    static const    UINT32 PWER__WEMUX3_ENA_WKUP_DUE_GPIO113 =    2; // 
    static const    UINT32 PWER__WEMUX2_MASK           = 0x00070000; // Wake=up enable for GPIO 36,38,40,53 for Standby and sleep
    static const    UINT32 PWER__WEMUX2_SHIFT          =         16; // 
    static const    UINT32 PWER__WEMUX2_NO_WAKEUP_ENABLED=        0; // 
    static const    UINT32 PWER__WEMUX2_ENA_WKUP_DUE_GPIO38 =     1; // 
    static const    UINT32 PWER__WEMUX2_ENA_WKUP_DUE_GPIO53 =     2; // 
    static const    UINT32 PWER__WEMUX2_ENA_WKUP_DUE_GPIO40 =     3; // 
    static const    UINT32 PWER__WEMUX2_ENA_WKUP_DUE_GPIO36 =     4; // 
    static const    UINT32 PWER__WE_MASK               = 0x0000FE00; // Wake-up enable n for standby or sleep n=9 to 15
    static const    UINT32 PWER__WE_SHIFT              =          9; // 
    static const    UINT32 PWER__WE4                   = 0x00000010; // Wake-up enable for standby or sleep modes due to GPIO 4 edge
    static const    UINT32 PWER__WE3                   = 0x00000008; // Wake-up enable for standby or sleep modes due to GPIO 3 edge
    static const    UINT32 PWER__WEN_MASK              = 0x00000003; // Wake-up enable for standby or sleep modes due to GPIO 0,1 edge
    static const    UINT32 PWER__WEN_SHIFT             =          0; // 

    /****/ volatile UINT32 PRER;                                  // Power Manager rising edge detect enable register
    static const    UINT32 PRER__RE35                  = 0x01000000; // Standby or sleep =ode rising edge wake-up enable due to GPIO 35
    static const    UINT32 PRER__REN_MASK              = 0x0000FE00; // Standby or sleep mode rising edge wake-up enable due to GPIO 9-15
    static const    UINT32 PRER__REN_SHIFT             =          9; //
    static const    UINT32 PRER__RE4                   = 0x00000010; // Standby or sleep mode rising edge wake-up enable due to GPIO 4 edge
    static const    UINT32 PRER__RE3                   = 0x00000008; // Standby or sleep mode rising edge wake-up enable due to GPIO 3 edge
    static const    UINT32 PRER__REN_MASK2             = 0x00000003; // Standby or sleep mode rising edge wake-up enable due to GPIO 0,1 edge
    static const    UINT32 PRER__REN_SHIFT2            =          0; // 

    /****/ volatile UINT32 PFER;                                 // Power Manager falling edge detect enable register
    static const    UINT32 PFER__FE35                  = 0x01000000; // Standby or sleep =ode falling edge wake-up enable due to GPIO 35
    static const    UINT32 PFER__FEN_MASK              = 0x0000FE00; // Standby or sleep mode falling edge wake-up enable due to GPIO 9-15
    static const    UINT32 PFER__FEN_SHIFT             =          9; //
    static const    UINT32 PFER__FE4                   = 0x00000010; // Standby or sleep mode falling edge wake-up enable due to GPIO 4 edge
    static const    UINT32 PFER__FE3                   = 0x00000008; // Standby or sleep mode falling edge wake-up enable due to GPIO 3 edge
    static const    UINT32 PFER__FEN_MASK2             = 0x00000003; // Standby or sleep mode falling edge wake-up enable due to GPIO 0,1 edge
    static const    UINT32 PFER__FEN_SHIFT2            =          0; // 

    /****/ volatile UINT32 PEDR;                                 // Power Manager edge detect status register
    static const    UINT32 PEDR__EDRTC                 = 0x80000000; // Standby or sleep mode or deep-sleep wake up from RTC
    static const    UINT32 PEDR__EDP1                  = 0x40000000; // Standby or sleep wake up from PI power domain
    static const    UINT32 PEDR__EDUSBH2               = 0x10000000; // Standby or sleep wake-up from USB host port 2
    static const    UINT32 PEDR__EDUSBH1               = 0x08000000; // Standby or sleep wake-up from USB host port 1
    static const    UINT32 PEDR__EDUSBC                = 0x04000000; // Standby or sleep wake-up from USB client port 
    static const    UINT32 PEDR__EDBB                  = 0x02000000; // Standby or sleep wake-up from MSL
    static const    UINT32 PEDR__ED35                  = 0x01000000; // Standby or sleep wake-up from Edge detect on GPIO35
    static const    UINT32 PEDR__EDMUX3                = 0x00100000; // Standby or sleep edge detect status base on PWER[WEMUX3]
    static const    UINT32 PEDR__EDMUX2                = 0x00020000; // Standby or sleep edge detect status base on PWER[WEMUX2]
    static const    UINT32 PEDR__EDN_MASK              = 0x0000FE00; // Standby or sleep edge detect status base on GPIO 9-15
    static const    UINT32 PEDR__EDN_SHIFT             =          9; //
    static const    UINT32 PEDR__ED4                   = 0x00000010; // Standby or sleep edge detect status base on GPIO 4 edge
    static const    UINT32 PEDR__ED3                   = 0x00000008; // Standby or sleep edge detect status base on GPIO 3 edge
    static const    UINT32 PEDR__EDN_MASK2             = 0x00000003; // Standby or sleep edge detect status base on  GPIO 0,1 edge
    static const    UINT32 PEDR__EDN_SHIFT2            =          0; // 

    /****/ volatile UINT32 PCFR;                                 // Power Manager general configruation register
    static const    UINT32 PCFR__RO                    = 0x00008000; // RDH override 
    static const    UINT32 PCFR__PO                    = 0x00004000; // PH Override
    static const    UINT32 PCFR__GPIOD                 = 0x00001000; // GPIO nReset _out Disable
    static const    UINT32 PCFR__L1_EN                 = 0x00000800; // Sleep mode sleep/deep sleep linear regulator enable
    static const    UINT32 PCFR__FVC                   = 0x00000400; // Frequency/voltage change
    static const    UINT32 PCFR__DC_EN                 = 0x00000080; // Sleep/Deep-Sleep DC_DC converter enable
    static const    UINT32 PCFR__P12C_EN               = 0x00000040; // power manager i2c enable
    static const    UINT32 PCFR__GPR_EN                = 0x00000010; // nReset gpio pin enable
    static const    UINT32 PCFR__FS                    = 0x00000004; // float static chip select(CS5-1)during sleep mode
    static const    UINT32 PCFR__FP                    = 0x00000002; // float PC card pins during sleep or deep sleep mode
    static const    UINT32 PCFR__OPDE                  = 0x00000001; // 13 MHz Processor oscillator power down enable

    /****/ volatile UINT32 PGSR[4];                              // Power Manager general configruation register, for GPIO-=120

    /****/ volatile UINT32 RCSR;                                 // Reset Controller Status register
    static const    UINT32 RCSR__GPR                   = 0x00000008; // GPIO Reset
    static const    UINT32 RCSR__SMR                   = 0x00000004; // Sleep-exit from sleep or deep-sleep mode
    static const    UINT32 RCSR__WDR                   = 0x00000002; // watchdog reset
    static const    UINT32 RCSR__HWR                   = 0x00000001; // Hardware/power on reset

    /****/ volatile UINT32 PSLR;                                 // Power Manager Sleep configuration Register
    static const    UINT32 PSLR__SYS_DEL_MASK          = 0xF0000000; // External High-voltage Power domains ramp Delay
    static const    UINT32 PSLR__SYS_DEL_SHIFT         =         28; // 
    static const    UINT32 PSLR__PWR_DEL_MASK          = 0x0F000000; // External low-voltage Power domains ramp delay
    static const    UINT32 PSLR__PWR_DEL_SHIFT         =         24; //
    static const    UINT32 PSLR__PSSD                  = 0x00800000; // Sleep-mode shorten wake=up delay disable
    static const    UINT32 PSLR__IVF                   = 0x00400000; // Ignore nVDD_FAULT in sleep mode and deep-sleep mode
    static const    UINT32 PSLR__SL_ROD                = 0x00100000; // sleep-mode/deep sleep mode nRESET_OUT disable
    static const    UINT32 PSLR__SL_R3                 = 0x00000800; // sleep mode unit retention-internal SRAM bank3
    static const    UINT32 PSLR__SL_R2                 = 0x00000400; // sleep mode unit retention-internal SRAM bank2
    static const    UINT32 PSLR__SL_R1                 = 0x00000200; // sleep mode unit retention-internal SRAM bank1
    static const    UINT32 PSLR__SL_R0                 = 0x00000100; // sleep mode unit retention-internal SRAM bank0
    static const    UINT32 PSLR__SL_PI_MASK            = 0x0000000C; // Sleep or deep sleep mode unit retention -PI power domain
    static const    UINT32 PSLR__SL_PI_SHIFT           =          2; // 
    static const    UINT32 PSLR__SL_PI_POWER_OFF       =          0; //
    static const    UINT32 PSLR__SL_PI_POWER_RETAINS   =          1; //
    static const    UINT32 PSLR__SL_PI_POWER_ACTIVE    =          2; // Do not use this setting when deep sleep

    /****/ volatile UINT32 PSTR;                                 // Power Manager Standby configuration Register
    static const    UINT32 PSTR__ST_R3                 = 0x00000800; // Standby mode retent state- Internal SRAM bank3
    static const    UINT32 PSTR__ST_R2                 = 0x00000400; // Standby mode retent state- Internal SRAM bank2
    static const    UINT32 PSTR__ST_R1                 = 0x00000200; // Standby mode retent state- Internal SRAM bank1
    static const    UINT32 PSTR__ST_R0                 = 0x00000100; // Standby mode retent state- Internal SRAM bank0
    static const    UINT32 PSTR__ST_PI_MASK            = 0x0000000C; // Standby mode retent state-PI power domain
    static const    UINT32 PSTR__ST_PI_SHIFT           =          2; // 
    static const    UINT32 PSTR__ST_PI_POWER_OFF       =          0; //
    static const    UINT32 PSTR__ST_PI_POWER_RETAINS   =          1; //
    static const    UINT32 PSTR__ST_PI_POWER_ACTIVE    =          2; // Do not use this setting when deep sleep

    /****/ volatile UINT32 Padding1;                             // 

    /****/ volatile UINT32 PVCR;                                 // Power Manager Voltage change control register
    static const    UINT32 PVCR__RD_PTR_MASK           = 0x01F00000; // A programmed vlaue of N indicateds that the voltage-change sequencer is pointing to register PCMD[n]
    static const    UINT32 PVCR__RD_PTR_SHIFT          =         20; //
    static const    UINT32 PVCR__VCSA                  = 0x00000400; // Voltage-Chagne sequecner active
    static const    UINT32 PVCR__CMD_DELAY_MASK        = 0x00000F80; //  Command delay in 2^n
    static const    UINT32 PVCR__CMD_DELAY_SHIFT       =          7;
    static const    UINT32 PVCR__SLV_ADDR_MASK         = 0x0000007F; // Subordinate address, the 7 bit address of the external regulators's I2C module
    static const    UINT32 PVCR__SLV_ADDR_SHIFT        =          0; // 

    /****/ volatile UINT32 Padding2[2];                          // 
    
    /****/ volatile UINT32 PUCR;                                 // Power Manager  USIM Card Control Status Register
    static const    UINT32 PUCR__UDETS                 = 0x00000020; //  USIM Detect Status
    static const    UINT32 PUCR__USIM115               = 0x00000008; // Allow UVS or UEN functinoality for GPIO15
    static const    UINT32 PUCR__USIM114               = 0x00000004; // Allow UVS or UEN functinoality for GPIO14
    static const    UINT32 PUCR__EN_UDET               = 0x00000001; //  Enable USIM Card detect

    /****/ volatile UINT32 PKWR;                                 // Power Manager Keyboard wake-up Enable register
    static const    UINT32 PKWR__WEN_MASK              = 0x000FFFFF; // Standby or sleep mode  wkae-up enable due to high level of GPIO n
    static const    UINT32 PKWR__WEN_SHIFT             =          0;

    /****/ volatile UINT32 PKSR;                                 // Power Manager Keyboard level detect status register
    static const    UINT32 PKSR__LDN_MASK              = 0x000FFFFF; // Standby or sleep mode level detect status due to high level of GPIO n
    static const    UINT32 PKWR__LDN_SHIFT             =          0;

    /****/ volatile UINT32 Padding3[9];                          // 

    /****/ volatile UINT32 PCMDX[32];                            // Power Manager  I2c Command register file for PCMD0-PCMD31
    static const    UINT32 PCMDX__MBC                  = 0x00001000; // Multi-byte command
    static const    UINT32 PCMDX__DELAY_CMD_EXE        = 0x00000800; // command delay execution
    static const    UINT32 PCMDX__LC                   = 0x00000400; // Last command
    static const    UINT32 PCMDX__SQC_MASK             = 0x00000300; // Sequecence configuration
    static const    UINT32 PCMDX__SQC_SHIFT            =          8; // 
    static const    UINT32 PCMDX__CMD_DATA_MASK        = 0x000000FF; // Power manager I2C Command Data send to external regulator
    static const    UINT32 PCMDX__CMD_DATA_SHIFT       =         0; // 
};
// Power Manager
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//CLOCK manager

struct PXA271_CLOCK_MANAGER
{
   static const UINT32 c_Base = 0x41300000;

    /****/ volatile UINT32 CCCR;                                 // Core CLock configuration Register 
    static const    UINT32 CCCR__CPDIS                 = 0x80000000; // Core PLL Output Disable/Enable
    static const    UINT32 CCCR__PPDIS                 = 0x40000000; // peripheral PLL Output disable/enable
    static const    UINT32 CCCR__LCD_26                = 0x08000000; // LCD CLock freq in deeep-idle or 13M mode 0-13M, 1-26M
    static const    UINT32 CCCR__PLL_EARLY_EN          = 0x04000000; // Early PLL enable
    static const    UINT32 CCCR__A                     = 0x02000000; // Alternative Setting for memory controller clock
    static const    UINT32 CCCR__2N_MASK               = 0x00000780; // Turbo-mode-to-run-mode  ratio, N
    static const    UINT32 CCCR__2N_SHIFT              =          7; // 
    static const    UINT32 CCCR__L_MASK                = 0x0000001F; // Run-mode-to-Oscillator  ratio, L
    static const    UINT32 CCCR__L_SHIFT               =          0; //

    /****/ volatile UINT32 CKEN;                                 // Clock enable Register 
    static const    UINT32 CKEN__31                    = 0x80000000; // AC97 controller configruation
    static const    UINT32 CKEN__N_MASK                = 0x01FFFFFF; // Clock enable for CKEN0-CKEN24
    static const    UINT32 CKEN__N_SHIFT               =          0; // 
    static const    UINT32 CKEN__TPM_UNIT_CLK          = 0x02000000; // TPM
    static const    UINT32 CKEN__CLK_CAP_INTERFACE     = 0x01000000; // clock capture interface clock enable
    static const    UINT32 CKEN__SSP1                  = 0x00800000; // SSP1 Untie Clock enable
    static const    UINT32 CKEN__MEMORY_CNTRL          = 0x00400000; // Memory controller
    static const    UINT32 CKEN__MEMORY_STICK_HOST     = 0x00200000; // Memory Stick Host
    static const    UINT32 CKEN__INTERNAL_MEMORY_CLK   = 0x00100000; // Internal Memory clock
    static const    UINT32 CKEN__KEYBRD_INTERFACE_CLK  = 0x00080000; // Keyboard interface clock enable
    static const    UINT32 CKEN__USIM_UNIT_CLK         = 0x00040000; // USIM Unit Clock
    static const    UINT32 CKEN__MSL_INTERFACE_CLK     = 0x00020000; // MSL interface clock
    static const    UINT32 CKEN__LCD_CTRL_CLK          = 0x00010000; // LCD controller clock
    static const    UINT32 CKEN__PWR_MNG_I2C_CLK       = 0x00008000; // Power manager I2C clock
    static const    UINT32 CKEN__I2C_CLK               = 0x00004000; // I2C clock
    static const    UINT32 CKEN__INFRARED_PORT         = 0x00002000; // Infra red port clock
    static const    UINT32 CKEN__MMC_CTRL              = 0x00001000; // MMC Controller clock
    static const    UINT32 CKEN__USB_CLIENT_48MHZ      = 0x00000800; // USB client unit clock- 48MHz
    static const    UINT32 CKEN__USB_HOST              = 0x00000400; // USB host unit clock
    static const    UINT32 CKEN__OS_TIMER              = 0x00000200; // OS timer unit clock
    static const    UINT32 CKEN__I2S                   = 0x00000100; // I2S unit clock
    static const    UINT32 CKEN__BTUART                = 0x00000080; // BTUART unit clock
    static const    UINT32 CKEN__FFUART                = 0x00000040; // FFUART unit clock
    static const    UINT32 CKEN__STUART                = 0x00000020; // STUART unit clock
    static const    UINT32 CKEN__SSP3                  = 0x00000010; // SSP3 unit
    static const    UINT32 CKEN__SSP2                  = 0x00000008; // SSP2 unit
    static const    UINT32 CKEN__AC97                  = 0x00000004; // AC97 controller clock
    static const    UINT32 CKEN__PWM_MASK              = 0x00000003; // PWM0-PWM3
    static const    UINT32 CKEN__PWM_SHIFT             =          0; // 

    /****/ volatile UINT32 OSCC;                                 // Oscillator configuration Register
    static const    UINT32 OSCC__OSD_MASK              = 0x00000060; // Processor (13MHz ) Oscillator Stabilization Delay
    static const    UINT32 OSCC__OSD_SHIFT             =          5; // 
    static const    UINT32 OSCC__OSD_5MS               =          0; // delay ~5 ms
    static const    UINT32 OSCC__OSD_3MS               =          1; // delay ~3ms
    static const    UINT32 OSCC__OSD_375US             =          2; // delay 375us
    static const    UINT32 OSCC__OSD_1US               =          3; // delay 1us
    static const    UINT32 OSCC__CRI                   = 0x00000010; // Clock request inoput ( external Processor Oscillator) status
    static const    UINT32 OSCC__PIO_EN                = 0x00000008; // 13MHz Processor Oscillator output enable 
    static const    UINT32 OSCC__TOUT_EN               = 0x00000004; // Timekeeping (32.768KHz) Oscillator output enable
    static const    UINT32 OSCC__OON                   = 0x00000002; // Timekeeping (32.768KHz) Oscillator on
    static const    UINT32 OSCC__OOK                   = 0x00000001; // Timekeeping (32.768KHz) Oscillator ok (read)

    /****/ volatile UINT32 CCSR;                                 // Core CLock Status Register, read only
    static const    UINT32 CCSR__CPDIS_S               = 0x80000000; // Core PLL output disable status
    static const    UINT32 CCSR__PPDIS_S               = 0x40000000; // peripheral PLL output disable status
    static const    UINT32 CCSR__CPLCK                 = 0x20000000; // Core PLL lock
    static const    UINT32 CCSR__PPLCK                 = 0x10000000; // Peripheral PLL lock
    static const    UINT32 CCSR__2N_S_MASK             = 0x00000280; // Turbo-mode-to-run-mode ratio (N) status
    static const    UINT32 CCSR__2N_S_SHIFT            =          7; // 
    static const    UINT32 CCSR__L_S_MASK              = 0x0000001F; // Run-mode to 13 MHz Processor Oscillator Ratio (L) Status

};
//CLOCK manager
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// DMA
struct PXA271_DMA
{
   static const UINT32 c_Base = 0x40000000;
    /****/ volatile UINT32 DCSR[32];                             // DMA Control/status register for channel 0-31
    static const    UINT32 DCSR__FBPOL1                = 0x00010000; // Fly-by DMA Davl<1> Polarity
    static const    UINT32 DCSR__FBPOL0                = 0x00000001; // Fly-by DMA Davl<0> Polarity

    /****/ volatile UINT32 Padding1[8];                          // 

    /****/ volatile UINT32 DALGN;                                // DMA Alignment register
    static const    UINT32 DALGN_MASK                  = 0xFFFFFFFF; // 

    /****/ volatile UINT32 DPCSR;                                // DMA programmed I/O Control status Register 
    static const    UINT32 DPCSR__BRGSPLIT             = 0x80000000; // active posted writes and split reads
    static const    UINT32 DPCSR__BRGBUSY              = 0x00000001; // Bridge Busy status

    /****/ volatile UINT32 Padding2[14];                         // 

    /****/ volatile UINT32 DRQSR[3];                             // DMA DREQ0-2 status register
    static const    UINT32 DRQSR__CLR                  = 0x00001000; // Clear Pending Request
    static const    UINT32 DRQSR__REQPEND_MASK         = 0x0000001F; // Request Pending
    static const    UINT32 DRQSR__REQPEND_SHIFT        =          0; // 

    /****/ volatile UINT32 Padding3;                             // 

    /****/ volatile UINT32 DINT;                                 // DMA Interrupt register
    static const    UINT32 DINT__MASK                  = 0xFFFFFFFF; // Channel interrupt for ch0-ch31, 0-no interrupt

    /****/ volatile UINT32 Padding4[3];                          // 

    /****/ volatile UINT32 DRCMR[64];                            // Request to channel map register for device
    static const    UINT32 DRCMR__MAPVLD               = 0x00000080; // Map valid channel
    static const    UINT32 DRCMR__CHLNUM_MASK          = 0x0000001F; // Channel number
    static const    UINT32 DRCMR__CHLNUM_SHIFT         =          0; // 

    struct CHANNEL
    {

        /****/ volatile UINT32 DDADR;                            // DMA Decriptor address registers 
        static const    UINT32 DDADR__DADDR_MASK       = 0xFFFFFFF0; // Descriptor address
        static const    UINT32 DDADR__DADDR_SHIFT      =          4; // 
        static const    UINT32 DDADR__BREN             = 0x00000002; // enable descriptor branching
        static const    UINT32 DRCMR__STOP             = 0x00000001; // Stop channel

        /****/ volatile UINT32 DSADR;                            // DMA Source address registers 
        static const    UINT32 DSADR__SRCADDR_MASK     = 0xFFFFFFFF; // 

        /****/ volatile UINT32 DTADR;                            // DMA Target address register
        static const    UINT32 DTADR__MASK             = 0xFFFFFFFF; // 

        /****/ volatile UINT32 DCMD;                             // DMA command registers 
        static const    UINT32 DCMD__INCSRADDR         = 0x80000000; // 
        static const    UINT32 DCMD__INCTRGADDR        = 0x40000000; // 
        static const    UINT32 DCMD__FLOWSRC           = 0x20000000; // 
        static const    UINT32 DCMD__FLOWTRG           = 0x10000000; // 
        static const    UINT32 DCMD__CMDPEN            = 0x02000000; // 
        static const    UINT32 DCMD__ADDRMODE          = 0x00800000; // 
        static const    UINT32 DCMD__STARTIRQEN        = 0x00400000; // 
        static const    UINT32 DCMD__ENDIRQEN          = 0x00200000; // 
        static const    UINT32 DCMD__FLYBYS            = 0x00100000; // 
        static const    UINT32 DCMD__FLYBYT            = 0x00080000; // 
        static const    UINT32 DCMD__SIZE_MASK         = 0x00030000; // 
        static const    UINT32 DCMD__SIZE_SHIFT        =         16; //
        static const    UINT32 DCMD__WIDTH_MASK        = 0x0000C000; // 
        static const    UINT32 DCMD__WIDTH_SHIFT       =         14; // 
        static const    UINT32 DCMD__LEN_MASK          = 0x000001FF; // 
    };

    static const UINT32 c_Channel =          32;

    CHANNEL              Channel[c_Channel];

};

// DMA
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//SDRAM +MEMORY
struct PXA271_MEMORY
{
   static const UINT32 c_Base = 0x48000000;
    /****/ volatile UINT32 MDCMFG;                               // SDRAM configuration register
    static const    UINT32 MDCMFG__MDENX               = 0x80000000; // SDRAM 1 GB memory map enable
    static const    UINT32 MDCMFG__DCACX2              = 0x40000000; // extra column addressing
    static const    UINT32 MDCMFG__DSA1110_2           = 0x20000000; // SA-1110 addressing mode compatibility
    static const    UINT32 MDCMFG__SETALWAYS2          = 0x08000000; // Set reserved bit always "1"
    static const    UINT32 MDCMFG__DADDR2              = 0x04000000; // Alternate addressing mode
    static const    UINT32 MDCMFG__DTC2_MASK           = 0x03000000; // AC timing parameters for SDRAM
    static const    UINT32 MDCMFG__DTC2_SHIFT          =         24; //
    static const    UINT32 MDCMFG__DNB2                = 0x00800000; // Number of banks in partition pair 2/3
    static const    UINT32 MDCMFG__DRAC2_MASK          = 0x00600000; // SDRAM Row address bit count for partition pair 2/3
    static const    UINT32 MDCMFG__DRAC2_SHIFT         =         21; //
    static const    UINT32 MDCMFG__DRAC2_ROWADDR_11    =          0; //
    static const    UINT32 MDCMFG__DRAC2_ROWADDR_12    =          1; //
    static const    UINT32 MDCMFG__DRAC2_ROWADDR_13    =          2; //
    static const    UINT32 MDCMFG__DCAC2_MASK          = 0x00180000; // SDRAM Column address bit count for partition pair 2/3
    static const    UINT32 MDCMFG__DCAC2_SHIFT         =         19; // Use with DCACX2
    static const    UINT32 MDCMFG__DCAC2_COLADDR_8     =          0; // DCACX=0
    static const    UINT32 MDCMFG__DCAC2_COLADDR_9     =          1; // DCACX=0
    static const    UINT32 MDCMFG__DCAC2_COLADDR_10    =          2; // DCACX=0
    static const    UINT32 MDCMFG__DCAC2_COLADDR_11    =          3; // DCACX=0
    static const    UINT32 MDCMFG__DCAC2_COLADDR_12    =          4; // has to set DCACX2 to 1,  DCAC2 = 0
    static const    UINT32 MDCMFG__DWID2               = 0x00040000; // SDRAM data bus width for partition pair 2/3
    static const    UINT32 MDCMFG__DE3                 = 0x00020000; // SDRAM enable for partition 3
    static const    UINT32 MDCMFG__DE2                 = 0x00010000; // SDRAM enable for partition 2
    static const    UINT32 MDCMFG__STACK1              = 0x00008000; // STACK 1
    static const    UINT32 MDCMFG__DCACX0              = 0x00004000; // Extra column addressing
    static const    UINT32 MDCMFG__STACK0              = 0x00002000; // Stack 0
    static const    UINT32 MDCMFG__DSA1110_0           = 0x00001000; // SA-1110 addressing mode compatibility
    static const    UINT32 MDCMFG__SETALWAYS0          = 0x00000800; // Set reserved bit always "1"
    static const    UINT32 MDCMFG__DADDR0              = 0x00000400; // Alternate addressing mode
    static const    UINT32 MDCMFG__DTC0_MASK           = 0x00000300; // AC timing parameters for SDRAM
    static const    UINT32 MDCMFG__DTC0_SHIFT          =          8; //
    static const    UINT32 MDCMFG__DTC0_00             =          0; //
    static const    UINT32 MDCMFG__DTC0_01             =          1; //
    static const    UINT32 MDCMFG__DTC0_02             =          2; //
    static const    UINT32 MDCMFG__DTC0_03             =          3; //
    static const    UINT32 MDCMFG__DNB0                = 0x00000080; // Number of banks in partition pair 2/3
    static const    UINT32 MDCMFG__DRAC0_MASK          = 0x00000060; // SDRAM Row address bit count for partition pair 2/3
    static const    UINT32 MDCMFG__DRAC0_SHIFT         =          5; //
    static const    UINT32 MDCMFG__DRAC0_ROWADDR_11    =          0; //
    static const    UINT32 MDCMFG__DRAC0_ROWADDR_12    =          1; //
    static const    UINT32 MDCMFG__DRAC0_ROWADDR_13    =          2; //
    static const    UINT32 MDCMFG__DCAC0_MASK          = 0x00000018; // SDRAM Column address bit count for partition pair 2/3
    static const    UINT32 MDCMFG__DCAC0_SHIFT         =          3; // Use with DCACX2
    static const    UINT32 MDCMFG__DCAC0_COLADDR_8     =          0; // DCACX=0
    static const    UINT32 MDCMFG__DCAC0_COLADDR_9     =          1; // DCACX=0
    static const    UINT32 MDCMFG__DCAC0_COLADDR_10    =          2; // DCACX=0
    static const    UINT32 MDCMFG__DCAC0_COLADDR_11    =          3; // DCACX=0
    static const    UINT32 MDCMFG__DCAC0_COLADDR_12    =          4; // has to set DCACX2 to 1,  DCAC2 = 0
    static const    UINT32 MDCMFG__DWID0               = 0x00000004; // SDRAM data bus width for partition pair 0/1
    static const    UINT32 MDCMFG__DE1                 = 0x00000002; // SDRAM enable for partition 1
    static const    UINT32 MDCMFG__DE0                 = 0x00000001; // SDRAM enable for partition 0

    /****/ volatile UINT32 MDREFR;                               // SDRAM refresh control register
    static const    UINT32 MDREFR__ALTREFA             = 0x80000000; // Exiting alternate bus master mode refresh control
    static const    UINT32 MDREFR__ALTREFB             = 0x40000000; // Entering alternate bus master mode refresh control
    static const    UINT32 MDREFR__K0DB4               = 0x20000000; // Sync static memory clock pin0 SDCLK<0> and SDCLK<3>
    static const    UINT32 MDREFR__K2FREE              = 0x02000000; // SDCLK<2> free-running
    static const    UINT32 MDREFR__K1FREE              = 0x01000000; // SDCLK<1> free-running
    static const    UINT32 MDREFR__K0FREE              = 0x00800000; // SDCLK<0> free-running
    static const    UINT32 MDREFR__SLFRSH              = 0x00400000; // SDRAM self refresh control/status
    static const    UINT32 MDREFR__APD                 = 0x00100000; // SDRAM/sync static memory auto-power-down enable
    static const    UINT32 MDREFR__K2DB2               = 0x00080000; // SDRAM clock pin 2 SDCLK<2> divide-by-2 control/status
    static const    UINT32 MDREFR__K2RUN               = 0x00040000; // SDRAM clock pin 2 SDCLK<2> run control/status
    static const    UINT32 MDREFR__K1DB2               = 0x00020000; // SDRAM clock pin 1 SDCLK<1> divide-by-2 control/status
    static const    UINT32 MDREFR__K1RUN               = 0x00010000; // SDRAM clock pin 1 SDCLK<1> divide-by-2 control/status
    static const    UINT32 MDREFR__E1PIN               = 0x00008000; // SDRAM clock enable pin 1 (SDCKE) levle control status
    static const    UINT32 MDREFR__K0DB2               = 0x00004000; // Sync static memory clock pin 0(SDCLK<0> and SDCLK<3>) divide by-2 ctrl status
    static const    UINT32 MDREFR__K0RUN               = 0x00002000; // Sync static memory clock pin 0 (SDCLK<0> and SDCLK<3> run ctrl status
    static const    UINT32 MDREFR__DRI_MASK            = 0x00000FFF; // SDRAM refresh interval for all partitions
    static const    UINT32 MDREFR__DRI_SHIFT           =          0; //

    /****/ volatile UINT32 MSC[3];                               // Static memory control register 0-2
    static const    UINT32 MSC__RBUFF_135              = 0x80000000; // return data buffer vs streaming behavior
    static const    UINT32 MSC__RRR_135_MASK           = 0x70000000; // ROM/SRAM recovery time
    static const    UINT32 MSC__RRR_135_SHIFT          =         28; //
    static const    UINT32 MSC__RDN_135_MASK           = 0x0F000000; // ROM Delay next access
    static const    UINT32 MSC__RDN_135_SHIFT          =         24; // 
    static const    UINT32 MSC__RDF_135_MASK           = 0x00F00000; // ROM Delay first access
    static const    UINT32 MSC__RDF_135_SHIFT          =         20; // 
    static const    UINT32 MSC__RBW_135                = 0x00080000; // ROM bus width
    static const    UINT32 MSC__RT_135_MASK            = 0x00070000; // ROM Delay first access
    static const    UINT32 MSC__RT_135_SHIFT           =         16; // 
    static const    UINT32 MSC__RBUFF_024              = 0x00008000; // return data buffer vs streaming behavior
    static const    UINT32 MSC__RRR_024_MASK           = 0x00007000; // ROM/SRAM recovery time
    static const    UINT32 MSC__RRR_024_SHIFT          =         12; //
    static const    UINT32 MSC__RDN_024_MASK           = 0x00000F00; // ROM Delay next access
    static const    UINT32 MSC__RDN_024_SHIFT          =          8; // 
    static const    UINT32 MSC__RDF_024_MASK           = 0x000000F0; // ROM Delay first access
    static const    UINT32 MSC__RDF_024_SHIFT          =          4; // 
    static const    UINT32 MSC__RBW_024                = 0x00000008; // ROM bus width
    static const    UINT32 MSC__RT_024_MASK            = 0x00000007; // ROM Delay first access
    static const    UINT32 MSC__RT_024_SHIFT           =          0; // 
    static const    UINT32 MSC__RT_024_ROM_SYNFLASH    =          0; // sync flash or non-burst rom or non-burst flash
    static const    UINT32 MSC__RT_024_SRAM            =          1; // SRAM
    static const    UINT32 MSC__RT_024_4_ROM           =          2; // burst of 4 rom or syn flash
    static const    UINT32 MSC__RT_024_8_ROM           =          3; // burst of 8 rom or syn flash
    static const    UINT32 MSC__RT_024_VLO             =          4; // Vraible -latency I/o (VLIO)


    /****/ volatile UINT32 MECER;                                // Expansion memory (PCcard/Compact flash)bus configuration register
    static const    UINT32 MECER__CIT                  = 0x00000002; // PC card is presnet
    static const    UINT32 MECER__NOS                  = 0x00000001; // PC card interface number of socket

    /****/ volatile UINT32 Padding1;                             // 

    /****/ volatile UINT32 SXCNFG;                               //  Synch static memory register
    static const    UINT32 SXCNFG__SXCLEXT2            = 0x80000000; // Synch flash memory CAS latency extension for SXCL2 
    static const    UINT32 SXCNFG__SXTP2_MASK          = 0x30000000; // SX memory type for partition pair 2/3
    static const    UINT32 SXCNFG__SXTP2_SHIFT         =         28; //
    static const    UINT32 SXCNFG__SXCL2_MASK          = 0x001C0000; // CAS latency for SZ memory partition pair 2/3
    static const    UINT32 SXCNFG__SXCL2_SHIFT         =         18; // 
    static const    UINT32 SXCNFG__SXEN3               = 0x00020000; // SX memory partition 3 enable
    static const    UINT32 SXCNFG__SXEN2               = 0x00010000; // SX memory partition 2 enable
    static const    UINT32 SXCNFG__SXCLEXT0            = 0x00008000; // Synch flash memory CAS latency extension for SXCL0
    static const    UINT32 SXCNFG__SXTP0_MASK          = 0x00003000; // SX memory type for partition pair 0/1
    static const    UINT32 SXCNFG__SXTP0_SHIFT         =         12; //
    static const    UINT32 SXCNFG__SXCL0_MASK          = 0x0000001C; // CAS latency for SZ memory partition pair 0/1
    static const    UINT32 SXCNFG__SXCL0_SHIFT         =          2; // 
    static const    UINT32 SXCNFG__SXEN1               = 0x00000002; // SX memory partition 1 enable
    static const    UINT32 SXCNFG__SXEN0               = 0x00000001; // SX memory partition 0 enable
    static const    UINT32 SXCNFG__RESERVE_BITS        = 0x40004000; // SX memory partition 0 enable

    /****/ volatile UINT32 Padding2[2];                          // 

    /****/ volatile UINT32 MCMEM[2];                             //  Expansion memory PC Card interface comon memory space socket timing configuration register
    static const    UINT32 MCMEM__HOLD_MASK            = 0x000FC000; // MCMEMx address hold
    static const    UINT32 MCMEM__HOLD_SHIFT           =         14; // 
    static const    UINT32 MCMEM__ASSt_MASK            = 0x00000F80; // MCMEMx command assert
    static const    UINT32 MCMEM__HOLD_SHIFT2          =         7; //
    static const    UINT32 MCMEM__SET_MASK             = 0x0000007F; // MCMEMx address setup
    static const    UINT32 MCMEM__SET_SHIFT            =          0; // 

    /****/ volatile UINT32 MCATT[2];                             //  Expansion memory PC Card interface attribute space socket timing configuration register
    static const    UINT32 MCATT__HOLD_MASK            = 0x000FC000; // MCATTx address hold
    static const    UINT32 MCATT__HOLD_SHIFT           =         14; // 
    static const    UINT32 MCATT__ASSt_MASK            = 0x00000F80; // MMCATTx command assert
    static const    UINT32 MCATT__HOLD_SHIFT2          =         7; //
    static const    UINT32 MCATT__SET_MASK             = 0x0000007F; // MCATTx address setup
    static const    UINT32 MCATT__SET_SHIFT            =          0; // 

    /****/ volatile UINT32 MCIO[2];                              // Expansion memory PC Card interface IO space socket timing configuration register
    static const    UINT32 MCIO__HOLD_MASK             = 0x000FC000; // MCATTx address hold
    static const    UINT32 MCIO__HOLD_SHIFT            =         14; // 
    static const    UINT32 MCIO__ASSt_MASK             = 0x00000F80; // MMCATTx command assert
    static const    UINT32 MCIO__HOLD_SHIFT2           =         7; //
    static const    UINT32 MCIO__SET_MASK              = 0x0000007F; // MCATTx address setup
    static const    UINT32 MCIO__SET_SHIFT             =          0; // 

    /****/ volatile UINT32 MDMRS;                                //  SDRAM mode register set configuration register
    static const    UINT32 MDMRS__MDMRS2_MASK          = 0x7F800000; // SDRAM MRS bit represent the value driver onto SDRAM address
    static const    UINT32 MDMRS__MDMRS2_SHIFT         =         23; // 
    static const    UINT32 MDMRS__MDCL2_MASK           = 0x00700000; // SDRAM MRS bits representing CAS latency
    static const    UINT32 MDMRS__MDCL2_SHIFT          =         20; // 
    static const    UINT32 MDMRS__MDADD2               = 0x00080000; // SDRAM MRS bit represnting burst type
    static const    UINT32 MDMRS__MDBL2_MASK           = 0x00070000; // SDRAM MRS bits representing burst length
    static const    UINT32 MDMRS__MDBL2_SHIFT          =         16; //
    static const    UINT32 MDMRS__K1FREE               = 0x00100000; // SDCLK<1> free-running
    static const    UINT32 MDMRS__MDMRS0_MASK          = 0x00007F80; // SDRAM MRS bit represent the value driver onto SDRAM address
    static const    UINT32 MDMRS__MDMRS0_SHIFT         =          7; // 
    static const    UINT32 MDMRS__MDCL0_MASK           = 0x00000070; // SDRAM MRS bits representing CAS latency
    static const    UINT32 MDMRS__MDCL0_SHIFT          =          4; // 
    static const    UINT32 MDMRS__MDADD0               = 0x00000008; // SDRAM MRS bit represnting burst type
    static const    UINT32 MDMRS__MDBL0_MASK           = 0x00000007; // SDRAM MRS bits representing burst length
    static const    UINT32 MDMRS__MDBL0_SHIFT          =          0; //

    /****/ volatile UINT32 BOOT_DEF;                             //  Boot time default configuration register (read only)
    static const    UINT32 BOOT_DEF__PKG_TYPE          = 0x00000008; // 0-reserved, 1=32bit package
    static const    UINT32 BOOT_DEF__BOOT_SEL          = 0x00000001; // Boot select 0=Boot_SEL is low(32bit memory), 1=BOOT_SEL is high (16bit memory)

    /****/ volatile UINT32 ARB_CNTL;                             //  Arbiter control register

    /****/ volatile UINT32 BSCNTR0;                              // System memory buffer strength control register 0
    static const    UINT32 BSCNTR0__CKE1BS_MASK        = 0xF0000000; // SDCKE buffer strength control register
    static const    UINT32 BSCNTR0__CKE1BS_SHIFT       =         28; // 
    static const    UINT32 BSCNTR0__CLK2BS_MASK        = 0x0F000000; // SDCLK <2> buffer strength control register
    static const    UINT32 BSCNTR0__CLK2BS_SHIFT       =         24; // 
    static const    UINT32 BSCNTR0__CLK1BS_MASK        = 0x00F00000; // SDCLK <1> buffer strength control register
    static const    UINT32 BSCNTR0__CLK1BS_SHIFT       =         20; // 
    static const    UINT32 BSCNTR0__CLK0BS_MASK        = 0x000F0000; // SDCLK <0> buffer strength control register
    static const    UINT32 BSCNTR0__CLK0BS_SHIFT       =         16; // 
    static const    UINT32 BSCNTR0__RASBS_MASK         = 0x0000F000; // SDRAS buffer strength control register
    static const    UINT32 BSCNTR0__RASBS_SHIFT        =         12; // 
    static const    UINT32 BSCNTR0__CASBS_MASK         = 0x00000F00; // SDCAS buffer strength control register
    static const    UINT32 BSCNTR0__CASBS_SHIFT        =          8; // 
    static const    UINT32 BSCNTR0__MDHBS_MASK         = 0x000000F0; // MD<31:16> buffer strength control register
    static const    UINT32 BSCNTR0__MDHBS_SHIFT        =          4; // 
    static const    UINT32 BSCNTR0__MDLBS_MASK         = 0x0000000F; // MD<15:0> buffer strength control register
    static const    UINT32 BSCNTR0__MDLBS_SHIFT        =          0; // 

    /****/ volatile UINT32 BSCNTR1;                              // System memory buffer strength control register 1
    static const    UINT32 BSCNTR1__DQM32BS_MASK       = 0xF0000000; // DQM<3:2> buffer strength control register
    static const    UINT32 BSCNTR1__DQM32BS_SHIFT      =         28; // 
    static const    UINT32 BSCNTR1__DQM10BS_MASK       = 0x0F000000; // DQM<1:0> buffer strength control register
    static const    UINT32 BSCNTR1__DQM10BS_SHIFT      =         24; // 
    static const    UINT32 BSCNTR1__SDCS32BS_MASK      = 0x00F00000; // SDCS <3:2> buffer strength control register
    static const    UINT32 BSCNTR1__SDCS32BS_SHIFT     =         20; // 
    static const    UINT32 BSCNTR1__SDCS10BS_MASK      = 0x000F0000; // SDCS <1:0> buffer strength control register
    static const    UINT32 BSCNTR1__SDCS10BS_SHIFT     =         16; // 
    static const    UINT32 BSCNTR1__WEBS_MASK          = 0x0000F000; // WE buffer strength control register
    static const    UINT32 BSCNTR1__WEBS_SHIFT         =         12; // 
    static const    UINT32 BSCNTR1__OEBS_MASK          = 0x00000F00; // OE buffer strength control register
    static const    UINT32 BSCNTR1__OEBS_SHIFT         =          8; // 
    static const    UINT32 BSCNTR1__SDCAS_DELAY_MASK   = 0x000000F0; // SDCAS return signal timing delay
    static const    UINT32 BSCNTR1__SDCAS_DELAY_SHIFT  =          4; // 
    static const    UINT32 BSCNTR1__RDnWRBS_MASK       = 0x0000000F; // RdnWR buffer strength control register
    static const    UINT32 BSCNTR1__RDnWRBS_SHIFT      =          0; // 

    /****/ volatile UINT32 LCDBSCNTR;                            // LCD buffer strength control register

    /****/ volatile UINT32 MDMRSLP;                              // Special low power SDRAM mode register set configuration register
    static const    UINT32 MDMRSLP__MDLPEN2            = 0x80000000; // LOW-POWERenable for partition 2/3
    static const    UINT32 MDMRSLP__MDMRSLP2_MASK      = 0x7FFF0000; // Low power MRS value for partition 2/3 
    static const    UINT32 MDMRSLP__MDMRSLP2_SHIFT     =         16; //
    static const    UINT32 MDMRSLP__MDLPEN0            = 0x00008000; // LOW-POWERenable for partition 0/1
    static const    UINT32 MDMRSLP__MDMRSLP0_MASK      = 0x00007FFF; // Low power MRS value for partition 0/1 
    static const    UINT32 MDMRSLP__MDMRSLP0_SHIFT     =          0; //

    /****/ volatile UINT32 BSCNTR2;                              // System memory buffer strength control register 2
    static const    UINT32 BSCNTR2__CS5BS_MASK         = 0xF0000000; // CS<5> buffer strength control register
    static const    UINT32 BSCNTR2__CS5BS_SHIFT        =         28; // 
    static const    UINT32 BSCNTR2__CS4BS_MASK         = 0x0F000000; // CS<4> buffer strength control register
    static const    UINT32 BSCNTR2__CS4BS_SHIFT        =         24; // 
    static const    UINT32 BSCNTR2__CS3BS_MASK         = 0x00F00000; // CS <3> buffer strength control register
    static const    UINT32 BSCNTR2__CS3BS_SHIFT        =         20; // 
    static const    UINT32 BSCNTR2__CS2BS_MASK         = 0x000F0000; // CS <2> buffer strength control register
    static const    UINT32 BSCNTR2__CS2BS_SHIFT        =         16; // 
    static const    UINT32 BSCNTR2__CS1BS_MASK         = 0x0000F000; // CS<1> buffer strength control register
    static const    UINT32 BSCNTR2__CS1BS_SHIFT        =         12; // 
    static const    UINT32 BSCNTR2__CS0BS_MASK         = 0x00000F00; // CS<0> buffer strength control register
    static const    UINT32 BSCNTR2__CS0BS_SHIFT        =          8; // 
    static const    UINT32 BSCNTR2__CLK3BS_MASK        = 0x000000F0; // SDCLK<3> buffer strength control register
    static const    UINT32 BSCNTR2__CLK3BS_SHIFT       =          4; // 
    static const    UINT32 BSCNTR2__MA25BS_MASK        = 0x0000000F; // MA<25> buffer strength control register
    static const    UINT32 BSCNTR2__MA25BS_SHIFT       =          0; // 

    /****/ volatile UINT32 BSCNTR3;                              // System memory buffer strength control register 3
    static const    UINT32 BSCNTR3__MA24BS_MASK        = 0xF0000000; // MA<24> buffer strength control register
    static const    UINT32 BSCNTR3__MA24BS_SHIFT       =         28; // 
    static const    UINT32 BSCNTR3__MA23BS_MASK        = 0x0F000000; // MA<23> buffer strength control register
    static const    UINT32 BSCNTR3__MA23BS_SHIFT       =         24; // 
    static const    UINT32 BSCNTR3__MA22BS_MASK        = 0x00F00000; // MA <22> buffer strength control register
    static const    UINT32 BSCNTR3__MA22BS_SHIFT       =         20; // 
    static const    UINT32 BSCNTR3__MA21BS_MASK        = 0x000F0000; // MA <21> buffer strength control register
    static const    UINT32 BSCNTR3__MA21BS_SHIFT       =         16; // 
    static const    UINT32 BSCNTR3__MA2010BS_MASK      = 0x0000F000; // MA<20:10> buffer strength control register
    static const    UINT32 BSCNTR3__MA2010BS_SHIFT     =         12; // 
    static const    UINT32 BSCNTR3__MA92BS_MASK        = 0x00000F00; // MA<9:2> buffer strength control register
    static const    UINT32 BSCNTR3__MA92BS_SHIFT       =          8; // 
    static const    UINT32 BSCNTR3__MA1BS_MASK         = 0x000000F0; // MA<1> buffer strength control register
    static const    UINT32 BSCNTR3__MA1BS_SHIFT        =          4; // 
    static const    UINT32 BSCNTR3__MA0BS_MASK         = 0x0000000F; // MA<0> buffer strength control register
    static const    UINT32 BSCNTR3__MA0BS_SHIFT        =          0; // 

    /****/ volatile UINT32 SA1110;                               //  SA-1110 compatibility mode for static memory register
    static const    UINT32 SA1110__SXSTACK_MASK        = 0x00003000; // stacked flash option
    static const    UINT32 SA1110__SXSTACK_SHIFT       =         12; //
    static const    UINT32 SA1110__SXSTACK_NO_FLASH    =          0;
    static const    UINT32 SA1110__SXSTACK_nCS0        =          1;
    static const    UINT32 SA1110__SXSTACK_nCS1        =          2;
    static const    UINT32 SA1110__SXSTACK_nCS0_1      =          3;
    static const    UINT32 SA1110__SXENX               = 0x00000100; // large memory support
    static const    UINT32 SA1110__SA110_5             = 0x00000020; // SA-110 compatibility mode for static memory partition 5
    static const    UINT32 SA1110__SA110_4             = 0x00000010; // SA-110 compatibility mode for static memory partition 4
    static const    UINT32 SA1110__SA110_3             = 0x00000008; // SA-110 compatibility mode for static memory partition 3
    static const    UINT32 SA1110__SA110_2             = 0x00000004; // SA-110 compatibility mode for static memory partition 2
    static const    UINT32 SA1110__SA110_1             = 0x00000002; // SA-110 compatibility mode for static memory partition 1
    static const    UINT32 SA1110__SA110_0             = 0x00000001; // SA-110 compatibility mode for static memory partition 0

};
//SDRAM +MEMORY
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//LCD controller
//
struct PXA271_LCDC
{
   static const UINT32 c_Base = 0x44000000;
    /****/ volatile UINT32 LCCR0;                                // LCD Controller COntrol register 0
    static const    UINT32 LCCR0__LDDALT               = 0x04000000; // LDD Alternate mapping control bit
    static const    UINT32 LCCR0__OUC                  = 0x02000000; // Overlay underlay control bit
    static const    UINT32 LCCR0__CMDIM                = 0x01000000; // LCD command interrupt mask
    static const    UINT32 LCCR0__RDSTM                = 0x00800000; // LCD Read status interrupt mask
    static const    UINT32 LCCR0__LCDT                 = 0x00400000; // LCD panel type
    static const    UINT32 LCCR0__OUM                  = 0x00200000; // OUtput FIFO underrun mask
    static const    UINT32 LCCR0__BSM0                 = 0x00100000; // branch status mask
    static const    UINT32 LCCR0__PPD_MASK             = 0x000FF000; // Palette DMA Request delay
    static const    UINT32 LCCR0__PPD_SHIFT            =         12; //
    static const    UINT32 LCCR0__QDM                  = 0x00000800; // LCD quick disable mask
    static const    UINT32 LCCR0__DIS                  = 0x00000400; // LCD disable
    static const    UINT32 LCCR0__DPD                  = 0x00000200; // double -pixel data pin mode
    static const    UINT32 LCCR0__PAS                  = 0x00000080; // Passive/active display select
    static const    UINT32 LCCR0__EOFM0                = 0x00000040; // end of frame mask for channel 0 and channel 1
    static const    UINT32 LCCR0__IUM                  = 0x00000020; // input FIFO underrun mask
    static const    UINT32 LCCR0__SOFM0                = 0x00000010; // start of frame mask for channel0 and for channel 1
    static const    UINT32 LCCR0__LDM                  = 0x00000008; // LCD disable done mask
    static const    UINT32 LCCR0__SDS                  = 0x00000004; // singles-scan/dual scan display select
    static const    UINT32 LCCR0__CMS                  = 0x00000002; // color/monochrome select
    static const    UINT32 LCCR0__ENB                  = 0x00000001; // LCD Controller enable

    /****/ volatile UINT32 LCCR1;                                // LCD controller control Register 1
    static const    UINT32 LCCR1__BLW_MASK             = 0xFF000000; // Beginning -of-line pixel wait count
    static const    UINT32 LCCR1__BLW_SHIFT            =         24; //
    static const    UINT32 LCCR1__ELW_MASK             = 0x00FF0000; // End-of-line pixel clock wait count
    static const    UINT32 LCCR1__ELW_SHIFT            =         16; //
    static const    UINT32 LCCR1__HSW_MASK             = 0x0000FC00; // Horizontal sync pulse width
    static const    UINT32 LCCR1__HSW_SHIFT            =         10; //
    static const    UINT32 LCCR1__PPL_MASK             = 0x000003FF; // Pixels per line for base frame
    static const    UINT32 LCCR1__PPL_SHIFT            =          0; // 

    /****/ volatile UINT32 LCCR2;                                // LCD controller control Register 2
    static const    UINT32 LCCR2__BFW_MASK             = 0xFF000000; // Beginning-of-frame line clock wait count
    static const    UINT32 LCCR2__BFW_SHIFT            =         24; //
    static const    UINT32 LCCR2__EFW_MASK             = 0x00FF0000; // End-of-frame line clock wait count
    static const    UINT32 LCCR2__EFW_SHIFT            =         16; //
    static const    UINT32 LCCR2__VSW_MASK             = 0x0000FC00; // Veritcal sync pulse width
    static const    UINT32 LCCR2__VSW_SHIFT            =         10; //
    static const    UINT32 LCCR2__LPP_MASK             = 0x000003FF; // Lines per panel for base frame
    static const    UINT32 LCCR2__LPP_SHIFT            =          0; // 


    /****/ volatile UINT32 LCCR3;                                // LCD controller control Register 3
    static const    UINT32 LCCR3__PDFOR_MASK           = 0xC0000000; // Pixel data format
    static const    UINT32 LCCR3__PDFOR_SHIFT          =         30; //
    static const    UINT32 LCCR3__BPP3                 = 0x20000000; // Bits per pixel
    static const    UINT32 LCCR3__DPC                  = 0x08000000; // Double pixel clock mode
    static const    UINT32 LCCR3__BPP                  = 0x07000000; // Bits per pixel
    static const    UINT32 LCCR3__OEP                  = 0x00800000; // output enable polarity
    static const    UINT32 LCCR3__PCP                  = 0x00400000; // Pixel clock polarity
    static const    UINT32 LCCR3__HSP                  = 0x00200000; // Horizontal sync polarity
    static const    UINT32 LCCR3__VSP                  = 0x00100000; // Vertical sync polarity
    static const    UINT32 LCCR3__API_MASK             = 0x000F0000; // AC bias pin transitions per interrupt
    static const    UINT32 LCCR3__API_SHIFT            =         16; //
    static const    UINT32 LCCR3__ACW_MASK             = 0x0000FF00; // AC bias pin frequency
    static const    UINT32 LCCR3__ACW_SHIFT            =          8; //
    static const    UINT32 LCCR3__PCD_MASK             = 0x000000FF; // Pixel clock divisor
    static const    UINT32 LCCR3__PCD_SHIFT            =          0; // 

    /****/ volatile UINT32 LCCR4;                                // LCD controller control Register 4
    static const    UINT32 LCCR4__PCDDIV               = 0x80000000; // PCD Divisor selection
    static const    UINT32 LCCR4__13M_PCD_EN           = 0x02000000; // 13M mode Pixel clock divisor enable
    static const    UINT32 LCCR4__13M_PCD_VAL_MASK     = 0x01FE0000; // 13M mode pixel clock divisor value
    static const    UINT32 LCCR4__13M_PCD_VAL_SHIFT    =         17; // 
    static const    UINT32 LCCR4__PAL_FOR_MASK         = 0x00018000; // Paletter Data format
    static const    UINT32 LCCR4__PAL_FOR_SHIFT        =         15; // 
    static const    UINT32 LCCR4__K3_MASK              = 0x000001C0; // Multiplication constant for green for half transparency
    static const    UINT32 LCCR4__K3_SHIFT             =          6; // 
    static const    UINT32 LCCR4__K2_MASK              = 0x00000038; // Multiplication constant for blue for half transparency
    static const    UINT32 LCCR4__K2_SHIFT             =          3; // 
    static const    UINT32 LCCR4__K1_MASK              = 0x00000007; // Multiplication constant for red for half transparency
    static const    UINT32 LCCR4__K1_SHIFT             =          0; // 

    /****/ volatile UINT32 LCCR5;                                // LCD controller control Register 5
    static const    UINT32 LCCR5__IUM6                 = 0x20000000; // Input FIFO underrun mask for cmd data
    static const    UINT32 LCCR5__IUM5                 = 0x10000000; // Input FIFO underrun mask for cursor
    static const    UINT32 LCCR5__IUM4                 = 0x08000000; // Input FIFO underrun mask for overlay2
    static const    UINT32 LCCR5__IUM3                 = 0x04000000; // Input FIFO underrun mask for overlay2
    static const    UINT32 LCCR5__IUM2                 = 0x02000000; // Input FIFO underrun mask for overlay2
    static const    UINT32 LCCR5__IUM1                 = 0x01000000; // Input FIFO underrun mask for overlay1 (when enabled)
    static const    UINT32 LCCR5__BSM6                 = 0x00200000; // Branch mask for overlay  (DMA ch 6 )
    static const    UINT32 LCCR5__BSM5                 = 0x00100000; // Branch mask for overlay  (DMA ch 5)
    static const    UINT32 LCCR5__BSM4                 = 0x00080000; // Branch mask for overlay  (DMA ch 4)
    static const    UINT32 LCCR5__BSM3                 = 0x00040000; // Branch mask for overlay  (DMA ch 3)
    static const    UINT32 LCCR5__BSM2                 = 0x00020000; // Branch mask for overlay  (DMA ch 2)
    static const    UINT32 LCCR5__BSM1                 = 0x00010000; // Branch mask for overlay  (DMA ch 1)
    static const    UINT32 LCCR5__EOFM6                = 0x00002000; // end of frame mask for command data (DMA ch 6)
    static const    UINT32 LCCR5__EOFM5                = 0x00001000; // end of frame mask for cursor (DMA ch 5)
    static const    UINT32 LCCR5__EOFM4                = 0x00000800; // end of frame mask for overlay 2 (DMA ch 4)
    static const    UINT32 LCCR5__EOFM3                = 0x00000400; // end of frame mask for overlay 2 (DMA ch 3)
    static const    UINT32 LCCR5__EOFM2                = 0x00000200; // end of frame mask for overlay 2 (DMA ch 2)
    static const    UINT32 LCCR5__EOFM1                = 0x00000100; // end of frame mask for overlay 1 (DMA ch 1)
    static const    UINT32 LCCR5__SOFM6                = 0x00000020; // start of frame mask for command data (DMA ch 6)
    static const    UINT32 LCCR5__SOFM5                = 0x00000010; // start of frame mask for cursor (DMA ch 5)
    static const    UINT32 LCCR5__SOFM4                = 0x00000008; // start of frame mask for overlay 2 (DMA ch 4)
    static const    UINT32 LCCR5__SOFM3                = 0x00000004; // start of frame mask for overlay 2 (DMA ch 3)
    static const    UINT32 LCCR5__SOFM2                = 0x00000002; // start of frame mask for overlay 2 (DMA ch 2)
    static const    UINT32 LCCR5__SOFM1                = 0x00000001; // start of frame mask for overlay 1 (DMA ch 1)

    /****/ volatile UINT32 Padding1[2];                          // 

    /****/ volatile UINT32 FBR[5];                               // DMA frame branch Register 0-4 
    static const    UINT32 FBR__SRCADDR_MASK           = 0xFFFFFFF0; // frame branch address
    static const    UINT32 FBR__SRCADD_SHIFT           =          4; // 
    static const    UINT32 FBR__BINT                   = 0x00000002; // Branch interrupt
    static const    UINT32 FBR__BRA                    = 0x00000001; // brmacj after finishing the current frame 

    /****/ volatile UINT32 LCSR1;                                //  LCD controller status Register 1
    static const    UINT32 LCSR1__IU6                  = 0x20000000; // Input FIFO underrun for ch6
    static const    UINT32 LCSR1__IU5                  = 0x10000000; // Input FIFO underrun for ch5
    static const    UINT32 LCSR1__IU4                  = 0x08000000; // Input FIFO underrun for ch4
    static const    UINT32 LCSR1__IU3                  = 0x04000000; // Input FIFO underrun for ch3
    static const    UINT32 LCSR1__IU2                  = 0x02000000; // Input FIFO underrun for ch2
    static const    UINT32 LCSR1__IU1                  = 0x01000000; // Input FIFO underrun for ch1
    static const    UINT32 LCSR1__BS6                  = 0x00200000; // Branch status for ch 6 (Command register)
    static const    UINT32 LCSR1__BS5                  = 0x00100000; // Branch status for ch 5 (Hardware cursor)
    static const    UINT32 LCSR1__BS4                  = 0x00080000; // Branch status for ch 4 (overlay 2)
    static const    UINT32 LCSR1__BS3                  = 0x00040000; // Branch status for ch 3 (overlay 2)
    static const    UINT32 LCSR1__BS2                  = 0x00020000; // Branch status for ch 2 (overlay 2)
    static const    UINT32 LCSR1__BS1                  = 0x00010000; // Branch status for ch 1 (overlay 1)
    static const    UINT32 LCSR1__EOF6                 = 0x00002000; // end of frame for command data (DMA ch 6)
    static const    UINT32 LCSR1__EOF5                 = 0x00001000; // end of frame for cursor (DMA ch 5)
    static const    UINT32 LCSR1__EOF4                 = 0x00000800; // end of frame for overlay 2 (DMA ch 4)
    static const    UINT32 LCSR1__EOF3                 = 0x00000400; // end of frame for overlay 2 (DMA ch 3)
    static const    UINT32 LCSR1__EOF2                 = 0x00000200; // end of frame for overlay 2 (DMA ch 2)
    static const    UINT32 LCSR1__EOF1                 = 0x00000100; // end of frame for overlay 1 (DMA ch 1)
    static const    UINT32 LCSR1__SOF6                 = 0x00000020; // start of frame for command data (DMA ch 6)
    static const    UINT32 LCSR1__SOF5                 = 0x00000010; // start of frame for cursor (DMA ch 5)
    static const    UINT32 LCSR1__SOF4                 = 0x00000008; // start of frame for overlay 2 (DMA ch 4)
    static const    UINT32 LCSR1__SOF3                 = 0x00000004; // start of frame for overlay 2 (DMA ch 3)
    static const    UINT32 LCSR1__SOF2                 = 0x00000002; // start of frame for overlay 2 (DMA ch 2)
    static const    UINT32 LCSR1__SOF1                 = 0x00000001; // start of frame for overlay 1 (DMA ch 1)

    /****/ volatile UINT32 LCSR0;                                // LCD controller status Register 0
    static const    UINT32 LCSR0__BER_CH_MASK          = 0x07000000; // bus error ch number
    static const    UINT32 LCSR0__BER_CH_SHIFT         =         28; // 
    static const    UINT32 LCSR0__CMD_INT              = 0x00001000; // Read status
    static const    UINT32 LCSR0__RD_ST                = 0x00000800; // read status
    static const    UINT32 LCSR0__SINT                 = 0x00000400; // subsequent interrupt status
    static const    UINT32 LCSR0__BS0                  = 0x00000200; // branch status for base
    static const    UINT32 LCSR0__EOF0                 = 0x00000100; // end of frame status for base
    static const    UINT32 LCSR0__QD                   = 0x00000080; // LCD quick disable status
    static const    UINT32 LCSR0__OU                   = 0x00000040; // output FIFO underrun
    static const    UINT32 LCSR0__IU1                  = 0x00000020; // input FIFO underrun for ch1
    static const    UINT32 LCSR0__IU0                  = 0x00000010; // input FIFO underrun for ch0
    static const    UINT32 LCSR0__ABC                  = 0x00000008; // AC Bias count status
    static const    UINT32 LCSR0__BER                  = 0x00000004; // bus error status
    static const    UINT32 LCSR0__SOF0                 = 0x00000002; // start of frame status for base(ch0)
    static const    UINT32 LCSR0__LDD                  = 0x00000001; // LCD disable done flag

    /****/ volatile UINT32 LIIDR;                                // LCD controller interrupt ID Register 
    static const    UINT32 LIIDR__IFRAMEID_MASK        = 0xFFFFFFFF; // 
    static const    UINT32 LIIDR__IFRAMEID_SHIFT       =          0; // 

    /****/ volatile UINT32 TRGBR;                                // TMED RGB seed Register 
    static const    UINT32 TRGBR__TBS_MASK             = 0x00FF0000; // TMED Blue seed value
    static const    UINT32 TRGBR__TBS_SHIFT            =         16; // 
    static const    UINT32 TRGBR__TGS_MASK             = 0x0000FF00; // TMED green seed value
    static const    UINT32 TRGBR__TGS_SHIFT            =          8; // 
    static const    UINT32 TRGBR__TRS_MASK             = 0x000000FF; // TMED red seed value
    static const    UINT32 TRGBR__TRS_SHIFT            =          0; // 


    /****/ volatile UINT32 TCR;                                  // TMED control Register 
    static const    UINT32 TCR__TED                    = 0x00004000; // TMED energy distribution select
    static const    UINT32 TCR__TSCS_MASK              = 0x00003000; // TMED shades per color select
    static const    UINT32 TCR__TSCS_SHIFT             =         12; //
    static const    UINT32 TCR__THBS_MASK              = 0x00000F00; // TMED horizontal beat suppression
    static const    UINT32 TCR__THBS_SHIFT             =          8; //
    static const    UINT32 TCR__TVBS_MASK              = 0x000000F0; // TMED vertical beat suppression
    static const    UINT32 TCR__TVBS_SHIFT             =          4; //
    static const    UINT32 TCR__TM1EN                  = 0x00000008; // TMED method 1 enable
    static const    UINT32 TCR__TM2EN                  = 0x00000004; // TMED method 2 enable
    static const    UINT32 TCR__TM1S                   = 0x00000002; // TMED method 1 select
    static const    UINT32 TCR__TM2S                   = 0x00000001; // TMED method 2 select

    /****/ volatile UINT32 Padding2[2];                          // 

    /****/ volatile UINT32 OVL1C1;                               // Overlay 1 control Register 1
    static const    UINT32 OVL1C1__O1EN                = 0x80000000; // ENABLE bit for overlay 1
    static const    UINT32 OVL1C1__BPP1_MASK           = 0x00F00000; // bits per pixel for overlay 1
    static const    UINT32 OVL1C1__BPP1_SHIFT          =         20; // 
    static const    UINT32 OVL1C1__LPO1_MASK           = 0x000FFC00; // Number of lines for overlay 1
    static const    UINT32 OVL1C1__LPO1_SHIFT          =         10;
    static const    UINT32 OVL1C1__PPL1_MASK           = 0x000003FF; // Pixels per line for overlay 1 frame
    static const    UINT32 OVL1C1__PPL1_SHIFT          =          0; // 

    /****/ volatile UINT32 Padding3[3];                          // 

    /****/ volatile UINT32 OVL1C2;                               // Overlay 1 control Register 2
    static const    UINT32 OVL1C2__O1YPOS_MASK         = 0x000FFC00; // Vertical position of the upper left most piexel of overlay 1 window
    static const    UINT32 OVL1C2__O1YPOS_SHIFT        =         10; // 
    static const    UINT32 OVL1C2__LPO1_MASK           = 0x000FFC00; // Number of lines for overlay 1
    static const    UINT32 OVL1C2__O1XPOS_MASK         = 0x000003FF; // horizontal position of the upper left most piexel of overlay 1 window
    static const    UINT32 OVL1C2__O1XPOS_SHIFT        =          0; // 

    /****/ volatile UINT32 Padding4[3];                          // 

    /****/ volatile UINT32 OVL2C1;                               // Overlay 2 control Register 1
    static const    UINT32 OVL2C1__O2EN                = 0x80000000; // ENABLE bit for overlay 2
    static const    UINT32 OVL2C1__BPP2_MASK           = 0x00F00000; // bits per pixel for overlay 2
    static const    UINT32 OVL2C1__BPP2_SHIFT          =         20; // 
    static const    UINT32 OVL2C1__LPO2_MASK           = 0x000FFC00; // Number of lines for overlay 2
    static const    UINT32 OVL2C1__LPO2_SHIFT          =         10;
    static const    UINT32 OVL2C1__PPL2_MASK           = 0x000003FF; // Pixels per line for overlay 2 frame
    static const    UINT32 OVL2C1__PPL2_SHIFT          =          0; // 

    /****/ volatile UINT32 Padding5[3];                              // 

    /****/ volatile UINT32 OVL2C2;                               // Overlay 2 control Register 2
    static const    UINT32 OVL2C2__FOR_MASK            = 0x00700000; // PIXEL format
    static const    UINT32 OVL2C2__FOR_SHIFT           =         20; // 
    static const    UINT32 OVL2C2__O2YPOS_MASK         = 0x000FFC00; // Vertical position of the upper left most piexel of overlay 2 window
    static const    UINT32 OVL2C2__O2YPOS_SHIFT        =         10; // 
    static const    UINT32 OVL2C2__O2XPOS_MASK         = 0x000003FF; // horizontal position of the upper left most piexel of overlay 2 window
    static const    UINT32 OVL2C2__O2XPOS_SHIFT        =          0; // 

    /****/ volatile UINT32 Padding6[3];                          // 

    /****/ volatile UINT32 CCR;                                  // Command cursor Register
    static const    UINT32 CCR__CEN                    = 0x80000000; // Cusor enable
    static const    UINT32 CCR__CYPOS_MASK             = 0x1FF80000; // vertical pos of cursor
    static const    UINT32 CCR__CYPOS_SHIFT            =         14; // 
    static const    UINT32 CCR__CXPOS_MASK             = 0x00007FE0; // Horizontal pos of cursor
    static const    UINT32 CCR__CXPOS_SHIFT            =          5; // 
    static const    UINT32 CCR__CURMS_MASK             = 0x00000007; // Cursor mode select
    static const    UINT32 CCR__CURMS_SHIFT            =          0; // 

    /****/ volatile UINT32 Padding7[3];                          // 

    /****/ volatile UINT32 CMDCR;                                // Command control Register
    static const    UINT32 CMDCR__SYNC_CNT_MASK        = 0x000000FF; // Sync count
    static const    UINT32 CMDCR__SYNC_CNT_SHIFT       =         0; // 

    /****/ volatile UINT32 Padding8[2];                          // 

    /****/ volatile UINT32 PRSR;                                 // Panel read status register
    static const    UINT32 PRSR__CON_NT                = 0x00000400; // continue to next cmd
    static const    UINT32 PRSR__ST_OK                 = 0x00000200; // staus ok
    static const    UINT32 PRSR__A0                    = 0x00000100; // read data source
    static const    UINT32 PRSR__DATA_MASK             = 0x000000FF; // Panel data
    static const    UINT32 PRSR__DATA_SHIFT            =         0; // 

    /****/ volatile UINT32 FBR56[2];                             // DMA frame branch Register 5,6 
    static const    UINT32 FBR56__SRCADDR_MASK         = 0xFFFFFFF0; // frame branch address
    static const    UINT32 FBR56__SRCADD_SHIFT         =          4; // 
    static const    UINT32 FBR56__BINT                 = 0x00000002; // Branch interrupt
    static const    UINT32 FBR56__BRA                  = 0x00000001; // brmacj after finishing the current frame 

    /****/ volatile UINT32 Padding9[58];                         // 

    struct DMA_CH
    {

        /****/ volatile UINT32 FDADR;                            // DMA Frame Decriptor address registers 
        static const    UINT32 FDADR__DADDR_MASK       = 0xFFFFFFF0; // Descriptor address
        static const    UINT32 DDADR__DADDR_SHIFT      =          4; // 

        /****/ volatile UINT32 FSADR;                            // DMA Frane Source address registers 
        static const    UINT32 FSADR__SRCADDR_MASK     = 0xFFFFFFF0; // 
        static const    UINT32 FSADR__SRCADDR_SHIFT    =          4; // 

        /****/ volatile UINT32 FIDR;                             // DMA Frane ID registers 
        static const    UINT32 FIDR__FRAME_ID_MASK     = 0xFFFFFFF0; // Frane ID
        static const    UINT32 FIDR__FRAME_ID_SHIFT    =          4; // 

        /****/ volatile UINT32 LDCMD;                            // LCD DMA command registers 
        static const    UINT32 LDCMD__PAL              = 0x04000000; // Paletter buffer
        static const    UINT32 LDCMD__SOFINT           = 0x00400000; // start of frame interrupt
        static const    UINT32 LDCMD__EOFINT           = 0x00200000; // 
        static const    UINT32 LDCMD__LENGTH_MASK      = 0x001FFFFC; // The length of transfer in bytes
        static const    UINT32 LDCMD__LENGTH_SHIFT     =          2; //
    };

    static const UINT32 c_Channel =          7;
    DMA_CH              Channel[c_Channel];
};
//
//LCD controller
///////////////////////////////////////////////////////////////////////////////













///// TOBE CONTINUE ...........PPPPPPPPPPPPPPPPP...................//
//////////////////////////////////////////////////////////////////////////////
// System Control
//
struct PXA271_SC
{
    static const UINT32 c_Base = 0x0021B800;


    /****/ volatile UINT32 Padding1;

};
//
// System Control
//////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////
// PWM
//
struct PXA271_PWM
{
// 16bit
    static const UINT32 c_Base_0 =0x40B00000;   //pwm 0
    static const UINT32 c_Base_1 =0x40C00000;   //pwm 1
    static const UINT32 c_Base_2 =0x40B00010;   //pwm 2
    static const UINT32 c_Base_3 =0x40C00010;   //pwm 3

// PWM base= c_Base_0 + c_Base_offset1*(PWMx & 1)+ c_Base_offset2*(PWMx >>1)
    static const UINT32 c_Base_offset1  =0x00100000;   //
    static const UINT32 c_Base_offset2  =0x00000010;   //
    //--//
    
//  static const UINT32 c_PWM_OUTPUT = PXA271_GPIO::c_Port_A_02;
 
    //--//

    /****/ volatile UINT32 PWMCR;                       // PWM control register
    static const    UINT32 PWMCR__SD                = 0x00000040; // PWM shutdown mode
    static const    UINT32 PWMCR__SD_GRACEFUL_SHUTDN= 0xFFFFFFBF; // Finished current cycle before shutdown
    static const    UINT32 PWMCR__SD_ABRUPT_SHUTDN  = 0x00000040; // PWM shutdown immediately
    
    static const    UINT32 PWMCR__PRESCALE_MASK     = 0x0000003F; // The scaled counter clock frequecny 

    /****/ volatile UINT32 PWMDCR;                      // PWM Duty Cycle register
    static const    UINT32 PWMDCR__FD               = 0x00000400; // Full duty cycle
    static const    UINT32 PWMDCR__FD_ASSERTED      = 0x00000400; // PWM always asserted
    static const    UINT32 PWMDCR__FD_DEASSERTED    = 0xFFFFFBFF; // PWM depends on the duty cycle
    static const    UINT32 PWMDCR__DCYCLE_MASK      = 0x000003FF; // duty cycle of PWM_OUT


    /****/ volatile UINT32 PWMPCR;                      // PWM Period register
    static const    UINT32 PWMP__PERIOD_MASK        = 0x000003FF; // Period Value
    
};
//
// PWM
//////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// SDRAM Control
//

//////////////////////////////////////////////////////////////////////////////
// Interrupt Controller
//
struct PXA271_AITC
{
    static const UINT32 c_Base = 0x40D00000;        // common interrupt registers

//    static const UINT32 c_Base0 = 0x40D00000;        // interrupt 0-31
//    static const UINT32 c_Base1 = 0x40D0009C;        // interrupt 32-39

    //--//

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
    static const UINT32 c_IRQ_Priority_16 = 16;
    static const UINT32 c_IRQ_Priority_17 = 17;
    static const UINT32 c_IRQ_Priority_18 = 18;
    static const UINT32 c_IRQ_Priority_19 = 19;
    static const UINT32 c_IRQ_Priority_20 = 20;
    static const UINT32 c_IRQ_Priority_21 = 21;
    static const UINT32 c_IRQ_Priority_22 = 22;
    static const UINT32 c_IRQ_Priority_23 = 23;
    static const UINT32 c_IRQ_Priority_24 = 24;
    static const UINT32 c_IRQ_Priority_25 = 25;
    static const UINT32 c_IRQ_Priority_26 = 26;
    static const UINT32 c_IRQ_Priority_27 = 27;
    static const UINT32 c_IRQ_Priority_28 = 28;
    static const UINT32 c_IRQ_Priority_29 = 29;
    static const UINT32 c_IRQ_Priority_30 = 30;
    static const UINT32 c_IRQ_Priority_31 = 31;
    static const UINT32 c_IRQ_Priority_32 = 32;
    static const UINT32 c_IRQ_Priority_33 = 33;
    static const UINT32 c_IRQ_Priority_34 = 34;

    static const UINT32 c_IRQ_INDEX_SSP_3            =  0;
    static const UINT32 c_IRQ_INDEX_MSL              =  1;
    static const UINT32 c_IRQ_INDEX_USB_HOST_2       =  2;
    static const UINT32 c_IRQ_INDEX_USB_HOST_1       =  3;
    static const UINT32 c_IRQ_INDEX_KEYPAD_CTRL      =  4;
    static const UINT32 c_IRQ_INDEX_MEMORY_STICK     =  5;
    static const UINT32 c_IRQ_INDEX_PWR_I2C          =  6;
    static const UINT32 c_IRQ_INDEX_OS_TIMER         =  7;
    static const UINT32 c_IRQ_INDEX_GPIO0            =  8;
    static const UINT32 c_IRQ_INDEX_GPIO1            =  9;
    static const UINT32 c_IRQ_INDEX_GPIOx            = 10;
    static const UINT32 c_IRQ_INDEX_USB_CLIENT       = 11;
    static const UINT32 c_IRQ_INDEX_PMU              = 12;
    static const UINT32 c_IRQ_INDEX_I2S              = 13;
    static const UINT32 c_IRQ_INDEX_AC97             = 14;
    static const UINT32 c_IRQ_INDEX_USIM             = 15;
    static const UINT32 c_IRQ_INDEX_SSP_2            = 16;
    static const UINT32 c_IRQ_INDEX_LCD              = 17;
    static const UINT32 c_IRQ_INDEX_I2C              = 18;
    static const UINT32 c_IRQ_INDEX_INFRA_RED_COM    = 19;
    static const UINT32 c_IRQ_INDEX_STUART           = 20;
    static const UINT32 c_IRQ_INDEX_BTUART           = 21;
    static const UINT32 c_IRQ_INDEX_FFUART           = 22;
    static const UINT32 c_IRQ_INDEX_FLASH_CARD       = 23;
    static const UINT32 c_IRQ_INDEX_SSP_1            = 24;
    static const UINT32 c_IRQ_INDEX_DMA_CTRL         = 25;
    static const UINT32 c_IRQ_INDEX_OS_TIMER0        = 26;
    static const UINT32 c_IRQ_INDEX_OS_TIMER1        = 27;
    static const UINT32 c_IRQ_INDEX_OS_TIMER2        = 28;
    static const UINT32 c_IRQ_INDEX_OS_TIMER3        = 29;
    static const UINT32 c_IRQ_INDEX_RTC_1HZ_TIC      = 30;
    static const UINT32 c_IRQ_INDEX_RTC_ALARM        = 31;
    static const UINT32 c_IRQ_INDEX_TRUSTED_PLFM     = 32;
    static const UINT32 c_IRQ_INDEX_QK_CAP           = 33;
 
 
    //--//

    /****/ volatile UINT32 ICIP;                              // Interrupt controller IRQ Pending register
    static const    UINT32 ICIP__INTERRUPT_PENDING     = 0x00000001;

    /****/ volatile UINT32 ICMR;                              // Interrupt controller mask register
    static const    UINT32 ICMR__INT_NOTMASKED         =          1;

    /****/ volatile UINT32 ICLR;                              // Interrupt controller lever register
    static const    UINT32 ICLR__IRQ                   =          0; //The corresponding bit creates an IRQ 
    static const    UINT32 ICLR__FIQ                   =          1; //The corresponding bit creates an FIQ 

    /****/ volatile UINT32 ICFP;                              // Interrupt controller FIQ Pending register
    static const    UINT32 ICFP__PENDING_FIQ           =          1; //The corresponding bit creates an FIQ 

    /****/ volatile UINT32 ICPR;                              // Interrupt controller pending register
    static const    UINT32 ICPR__INT_PENDING           =          1; //The corresponding bit has an interrupt occured

    /****/ volatile UINT32 ICCR;                              // Interrupt controller control register
    static const    UINT32 ICCR__DIM                   = 0x00000001; // Disable interrupt mode
    static const    UINT32 ICCR__DIM_ANY               = 0x00000000; // Any interrupt in ICPR brings the processor out of idle
    static const    UINT32 ICCR__DIM_OTHER             = 0x00000001; // Only unmasked interrupt in ICMR brings the processor out of idle

    /****/ volatile UINT32 ICHP;                              // Interrupt controller highest prioirty register
    static const    UINT32 ICHP__VAL_IRQ               = 0x80000000; // Valid IRQ 
    static const    UINT32 ICHP__IRQ_MASK              = 0x003F0000; // Current IRQ highest priority field
    static const    UINT32 ICHP__IRQ_SHIFT             =         16; // 
    static const    UINT32 ICHP__VAL_FIQ               = 0x00008000; // Valid FIQ
    static const    UINT32 ICHP__FIQ_MASK              = 0x0000003F; // Current FIQ highest priority field
    static const    UINT32 ICHP__FIQ_SHIFT             =          0; // 

    /****/ volatile UINT32 IPR[32];                           // Interrupt controller priority register
    static const    UINT32 IPR__VAL                    = 0x80000000; // Valid bit
    static const    UINT32 IPR__PID_MASK               = 0x0000003F; // 
    static const    UINT32 IPR__PID_SHIFT              =          0; // 


    /****/ volatile UINT32 ICIP2;                              // Interrupt controller IRQ Pending register
    static const    UINT32 ICIP2__INTERRUPT_PENDING    = 0x00000001;

    /****/ volatile UINT32 ICMR2;                              // Interrupt controller mask register
    static const    UINT32 ICMR2__INT_NOTMASKED        =          1;

    /****/ volatile UINT32 ICLR2;                              // Interrupt controller lever register
    static const    UINT32 ICLR2__IRQ                  =          0; //The corresponding bit creates an IRQ 
    static const    UINT32 ICLR2__FIQ                  =          1; //The corresponding bit creates an FIQ 

    /****/ volatile UINT32 ICFP2;                              // Interrupt controller FIQ Pending register
    static const    UINT32 ICFP2__PENDING_FIQ          =          1; //The corresponding bit creates an FIQ 

    /****/ volatile UINT32 ICPR2;                              // Interrupt controller pending register
    static const    UINT32 ICPR2__INT_PENDING          =          1; //The corresponding bit has an interrupt occured

    /****/ volatile UINT32 IPR2[8];                           // Interrupt controller priority register
    static const    UINT32 IPR2__VAL                   = 0x80000000; // Valid bit
    static const    UINT32 IPR2__PID_MASK              = 0x0000003F; // 
    static const    UINT32 IPR2__PID_SHIFT             =         31; // 


    static const UINT32 c_MaxInterruptIndex = 34;

    //--//

    void DisableInterrupt( UINT32 Irq_Index )
    {
        ASSERT(Irq_Index < c_MaxInterruptIndex);

        volatile UINT32* IntEnable = (Irq_Index < 32) ? &ICMR : &ICMR2;

        Irq_Index %= 32;

       *IntEnable &= ~(0x1 << Irq_Index);
    }

    void EnableInterrupt( UINT32 Irq_Index )
    {
        ASSERT(Irq_Index < c_MaxInterruptIndex);
        volatile UINT32* IntEnable = (Irq_Index < 32) ? &ICMR : &ICMR2;

        Irq_Index %= 32;

       *IntEnable |= 0x1 << Irq_Index;
    }



    BOOL IsInterruptEnabled( UINT32 Irq_Index )
    {
        ASSERT(Irq_Index < c_MaxInterruptIndex);

        // Note that if DIM is cleared AND the processor is in the idle state,
        // then the interrupt is enabled even if it is masked.
        
        volatile UINT32* IntEnable = (Irq_Index < 32) ? &ICMR : &ICMR2;

        Irq_Index %= 32;

        return (*IntEnable >> Irq_Index) & 0x1;
    }

// ASSUME we are ignoring masked interrupts
    BOOL GetInterruptState( UINT32 Irq_Index )
    {
        ASSERT(Irq_Index < c_MaxInterruptIndex);

        volatile UINT32* IntSrc = (Irq_Index < 32) ? &ICIP : &ICIP2;

        Irq_Index %= 32;

        return (*IntSrc >> Irq_Index) & 0x1;
    }

    UINT32 NormalInterruptPending()
    {
        UINT32 index;
        if (ICHP & ICHP__VAL_IRQ)
        {
            return index = ((ICHP & ICHP__IRQ_MASK) >> ICHP__IRQ_SHIFT);
        }
        return c_MaxInterruptIndex;
    }

    UINT32 FastInterruptPending()
    {
        UINT32 index;

        if (ICHP & ICHP__VAL_FIQ)
        {
            return index = ((ICHP & ICHP__FIQ_MASK) >> ICHP__FIQ_SHIFT);
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

// assume Fast = 1 =>FIQ
    void SetType( UINT32 Irq_Index, BOOL Fast )
    {
        ASSERT(Irq_Index < c_MaxInterruptIndex);

        volatile UINT32* INTTYPE = (Irq_Index < 32) ? &ICLR : &ICLR2;

        if(Fast) *INTTYPE |=  (1 << Irq_Index);
        else     *INTTYPE &= ~(1 << Irq_Index);
    }

    void SetPriority( UINT32 Irq_Index, UINT32 priority )
    {
        ASSERT(Irq_Index < c_MaxInterruptIndex);
        ASSERT(priority < c_MaxInterruptIndex);

// should disable the interrupt before change the priority. It is expected the caller to disable interupt
        
        volatile UINT32* NIPRIORITY = (priority < 32) ? &(IPR[priority]) : &(IPR2[priority-32]);
        *NIPRIORITY = (Irq_Index | IPR__VAL);
    }

    UINT32 GetPriority( UINT32 Irq_Index )
    {
        ASSERT(Irq_Index < c_MaxInterruptIndex);
        int i;

        Irq_Index |= IPR__VAL;      // Compare only works if register value is valid

        for(i = 0; i < 32; i++)
        {
            if(Irq_Index == IPR[i])
            {
                return i;
            }
        }
        for( ; i < c_MaxInterruptIndex; i++)
        {
            if(Irq_Index == IPR2[i-32])
            {
                return i;
            }
        }
        return c_MaxInterruptIndex; 
    }

    void ForceInterrupt( UINT32 Irq_Index )
    {
        // TBD
        ASSERT(FALSE);      // Interrupts may not be forced for this processor (at this time?)
    }

    void RemoveForcedInterrupt( UINT32 Irq_Index )
    {
        // TBD
    }

    
};
//
// Interrupt Controller
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// GP I/O
//
struct PXA271_GPIO
{
    static const UINT32 c_Base        = 0x40E00000;

    //--//

    static const UINT32 c_Pin_None  = 0xFFFFFFFF;
    static const UINT32 c_Port_00 =   0;
    static const UINT32 c_Port_01 =   1;
//  static const UINT32 c_Port_02 =   2;  this is NA, it is SYS_EN
    static const UINT32 c_Port_03 =   3;
    static const UINT32 c_Port_04 =   4;
//  static const UINT32 c_Port_05 =   5;  this is NA, it is PWR_CAP
//  static const UINT32 c_Port_06 =   6;  this is NA, it is PWR_CAP
//  static const UINT32 c_Port_07 =   7;  this is NA, it is PWR_CAP
//  static const UINT32 c_Port_08 =   8;  this is NA, it is PWR_CAP
    static const UINT32 c_Port_09 =   9;
    static const UINT32 c_Port_10 =  10;
    static const UINT32 c_Port_11 =  11;
    static const UINT32 c_Port_12 =  12;
    static const UINT32 c_Port_13 =  13;
    static const UINT32 c_Port_14 =  14;
    static const UINT32 c_Port_15 =  15;
    static const UINT32 c_Port_16 =  16;
    static const UINT32 c_Port_17 =  17;
    static const UINT32 c_Port_18 =  18;
    static const UINT32 c_Port_19 =  19;
    static const UINT32 c_Port_20 =  20;
    static const UINT32 c_Port_21 =  21;
    static const UINT32 c_Port_22 =  22;
    static const UINT32 c_Port_23 =  23;
    static const UINT32 c_Port_24 =  24;
    static const UINT32 c_Port_25 =  25;
    static const UINT32 c_Port_26 =  26;
    static const UINT32 c_Port_27 =  27;
    static const UINT32 c_Port_28 =  28;
    static const UINT32 c_Port_29 =  29;
    static const UINT32 c_Port_30 =  30;
    static const UINT32 c_Port_31 =  31;
    static const UINT32 c_Port_32 =  32;
    static const UINT32 c_Port_33 =  33;
    static const UINT32 c_Port_34 =  34;
    static const UINT32 c_Port_35 =  35;
    static const UINT32 c_Port_36 =  36;
    static const UINT32 c_Port_37 =  37;
    static const UINT32 c_Port_38 =  38;
    static const UINT32 c_Port_39 =  39;
    static const UINT32 c_Port_40 =  40;
    static const UINT32 c_Port_41 =  41;
    static const UINT32 c_Port_42 =  42;
    static const UINT32 c_Port_43 =  43;
    static const UINT32 c_Port_44 =  44;
    static const UINT32 c_Port_45 =  45;
    static const UINT32 c_Port_46 =  46;
    static const UINT32 c_Port_47 =  47;
    static const UINT32 c_Port_48 =  48;
    static const UINT32 c_Port_49 =  49;
    static const UINT32 c_Port_50 =  50;
    static const UINT32 c_Port_51 =  51;
    static const UINT32 c_Port_52 =  52;
    static const UINT32 c_Port_53 =  53;
    static const UINT32 c_Port_54 =  54;
    static const UINT32 c_Port_55 =  55;
    static const UINT32 c_Port_56 =  56;
    static const UINT32 c_Port_57 =  57;
    static const UINT32 c_Port_58 =  58;
    static const UINT32 c_Port_59 =  59;
    static const UINT32 c_Port_60 =  60;
    static const UINT32 c_Port_61 =  61;
    static const UINT32 c_Port_62 =  62;
    static const UINT32 c_Port_63 =  63;
    static const UINT32 c_Port_64 =  64;
    static const UINT32 c_Port_65 =  65;
    static const UINT32 c_Port_66 =  66;
    static const UINT32 c_Port_67 =  67;
    static const UINT32 c_Port_68 =  68;
    static const UINT32 c_Port_69 =  69;
    static const UINT32 c_Port_70 =  70;
    static const UINT32 c_Port_71 =  71;
    static const UINT32 c_Port_72 =  72;
    static const UINT32 c_Port_73 =  73;
    static const UINT32 c_Port_74 =  74;
    static const UINT32 c_Port_75 =  75;
    static const UINT32 c_Port_76 =  76;
    static const UINT32 c_Port_77 =  77;
    static const UINT32 c_Port_78 =  78;
    static const UINT32 c_Port_79 =  79;
    static const UINT32 c_Port_80 =  80;
    static const UINT32 c_Port_81 =  81;
    static const UINT32 c_Port_82 =  82;
    static const UINT32 c_Port_83 =  83;
    static const UINT32 c_Port_84 =  84;
    static const UINT32 c_Port_85 =  85;
    static const UINT32 c_Port_86 =  86;
    static const UINT32 c_Port_87 =  87;
    static const UINT32 c_Port_88 =  88;
    static const UINT32 c_Port_89 =  89;
    static const UINT32 c_Port_90 =  90;
    static const UINT32 c_Port_91 =  91;
    static const UINT32 c_Port_92 =  92;
    static const UINT32 c_Port_93 =  93;
    static const UINT32 c_Port_94 =  94;
    static const UINT32 c_Port_95 =  95;
    static const UINT32 c_Port_96 =  96;
    static const UINT32 c_Port_97 =  97;
    static const UINT32 c_Port_98 =  98;
    static const UINT32 c_Port_99 =  99;
    static const UINT32 c_Port_100= 100;
    static const UINT32 c_Port_101= 101;
    static const UINT32 c_Port_102= 102;
    static const UINT32 c_Port_103= 103;
    static const UINT32 c_Port_104= 104;
    static const UINT32 c_Port_105= 105;
    static const UINT32 c_Port_106= 106;
    static const UINT32 c_Port_107= 107;
    static const UINT32 c_Port_108= 108;
    static const UINT32 c_Port_109= 109;
    static const UINT32 c_Port_110= 110;
    static const UINT32 c_Port_111= 111;
    static const UINT32 c_Port_112= 112;
    static const UINT32 c_Port_113= 113;
    static const UINT32 c_Port_114= 114;
    static const UINT32 c_Port_115= 115;
    static const UINT32 c_Port_116= 116;
    static const UINT32 c_Port_117= 117;
    static const UINT32 c_Port_118= 118;
    static const UINT32 c_Port_119= 119;
    static const UINT32 c_Port_120= 120;
    //--//

    /****/ volatile UINT32  GPLR[3];                     // GPIO Pin-Level register (read only)
    inline volatile UINT32& rGPLR(UINT32 pin)            { return (pin < 96) ? GPLR[pin/32] : GPLR3; }

    /****/ volatile UINT32  GPDR[3];                     // GPIO Pin Direction register
    inline volatile UINT32& rGPDR(UINT32 pin)            { return (pin < 96) ? GPDR[pin/32] : GPDR3; }
    static const    UINT32  GPDR__DIR_IN                 = 0;    // Input port
    static const    UINT32  GPDR__DIR_OUT                = 1;    // Output Port

    /****/ volatile UINT32  GPSR[3];                     // GPIO Output set register
    inline volatile UINT32& rGPSR(UINT32 pin)            { return (pin < 96) ? GPSR[pin/32] : GPSR3; }
    static const    UINT32  GPSR__SET                    = 1;    // Set port to Hi state if it is output port

    /****/ volatile UINT32  GPCR[3];                     // GPIO Output clear register
    inline volatile UINT32& rGPCR(UINT32 pin)            { return (pin < 96) ? GPCR[pin/32] : GPCR3; }
    static const    UINT32  GPCR__CLR                    = 1;    // set port to Lo state if it is output port

    /****/ volatile UINT32  GRER[3];                     // GPIO rising edge detect enable register
    inline volatile UINT32& rGRER(UINT32 pin)            { return (pin < 96) ? GRER[pin/32] : GRER3; }
    static const    UINT32  GRER__SET_DETECT_RISING      = 1;    // Set detect rising edge

    /****/ volatile UINT32  GFER[3];                     // GPIO rising edge detect enable register
    inline volatile UINT32& rGFER(UINT32 pin)            { return (pin < 96) ? GFER[pin/32] : GFER3; }
    static const    UINT32  GFER__SET_DETECT_FALL        = 1;    // Set detect falling edge

    /****/ volatile UINT32  GEDR[3];                     // GPIO edge detect Status register
    inline volatile UINT32& rGEDR(UINT32 pin)            { return (pin < 96) ? GEDR[pin/32] : GEDR3; }
    static const    UINT32  GEDR__EDGE_DETECTED          = 1;    // edge detected if set
    static const    UINT32  GEDR__CLR_DETECT_RISING      = 1;    // write to clear detect edge

    /****/ volatile UINT32  GAFR[8];                     // GPIO Alternate function register.
    inline volatile UINT32& rGAFR(UINT32 pin)            { return GAFR[pin/16]; }
    static const    UINT32 GAFR__IO                     = 0;    // set the pin to GPIO
    static const    UINT32 GAFR__ALT_1                  = 1;    // set the pin to Alternate function1
    static const    UINT32 GAFR__ALT_2                  = 2;    // set the pin to Alternate function2
    static const    UINT32 GAFR__ALT_3                  = 3;    // set the pin to Alternate function3

    /****/ volatile UINT32 Padding0[35];                // Addresses 0x40E0_0074 - 0x40E0_00FF unused

    //--//

    /****/ volatile UINT32 GPLR3;                       // GPIO 96-120 Pin-Level register (read only)

    /****/ volatile UINT32 Padding1[2];                 // Addresses 0x40E0_0104 - 0x40E0_010B unused

    /****/ volatile UINT32 GPDR3;                       // GPIO 96-120 Pin Direction register

    /****/ volatile UINT32 Padding2[2];                 // Addresses 0x40E0_0110 - 0x40E0_0117 unused

    /****/ volatile UINT32 GPSR3;                       // GPIO 96-120 Output set register

    /****/ volatile UINT32 Padding3[2];                 // Addresses 0x40E0_011C - 0x40E0_0123 unused

    /****/ volatile UINT32 GPCR3;                       // GPIO 96-120 Output clear register

    /****/ volatile UINT32 Padding4[2];                 // Addresses 0x40E0_0128 - 0x40E0_012F unused

    /****/ volatile UINT32 GRER3;                       // GPIO 96-120 rising edge detect enable register

    /****/ volatile UINT32 Padding5[2];                 // Addresses 0x40E0_0134 - 0x40E0_013B unused

    /****/ volatile UINT32 GFER3;                       // GPIO96-120 falling edge detect enable register

    /****/ volatile UINT32 Padding6[2];                 // Addresses 0x40E0_0140 - 0x40E0_0147 unused

    /****/ volatile UINT32 GEDR3;                       // GPIO 96-120 edge detect Status register


    static const    UINT32 GPIO_INT_EDGE_FALLING        = 0x00000001;
    static const    UINT32 GPIO_INT_EDGE_RISING         = 0x00000002;
    
    //--//

    void SetInterruptMode( UINT32 pin, UINT32 intMode )
    {
        UINT32           bitMask  = 1 << (pin % 32);
        volatile UINT32& GFERport = rGFER(pin);
        volatile UINT32& GRERport = rGRER(pin);

        if(intMode & GPIO_INT_EDGE_FALLING)
            GFERport |=  bitMask;
        else
            GFERport &= ~bitMask;

        if(intMode & GPIO_INT_EDGE_RISING)
            GRERport |=  bitMask;
        else
            GRERport &= ~bitMask;
    }

    UINT32 GetInterruptMode( UINT32 pin )
    {
        UINT32 mode     = 0;
        UINT32 bitMask  = 1 << (pin % 32);
        
        if(rGFER(pin) & bitMask)
            mode |= GPIO_INT_EDGE_FALLING;

        if(rGRER(pin) & bitMask)
            mode |= GPIO_INT_EDGE_RISING;
        
        return mode;
    }

    void SetPinFunction( UINT32 pin, UINT32 function )
    {
        UINT32           shift = (pin % 16) * 2;
        UINT32           mask = 3 << shift;
        volatile UINT32& GAFRport = rGAFR(pin);

        GAFRport = (GAFRport & ~mask) | (function << shift);
    }
    
};
//
// GP I/O
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// TIMERs and Watchdog
//
struct PXA271_TIMER
{
    // NOTE:
    // The 12 timers below are divided into two types: PXA250 compatible timers and ones designed
    // for the PXA271.  The 4 PXA250 compatible timers consist of only one count register that is
    // connected to four compare or "match" registers.  The 8 PXA271 timers consist of eight counters
    // and eight corresponding match registers.
    // Also note that the Watchdog is defined to use timer 3; one of the PXA250 compatible timers.
    
    static const UINT32 c_Base = 0x40A00000;

    /****/ volatile UINT32 OSMR0[4];                                        // PXA250 compatible match registers (match with OSCR0)

    /****/ volatile UINT32 OSCR0;                                           // PXA250 compatible count register

    // PXA250 compatible and PXA271 Timer shared registers

    /****/ volatile UINT32 OSSR;                                            // Status register - cleared by writing 1 to bit

    /****/ volatile UINT32 OWER;                                            // Watchdog enable register
    static const    UINT32 OWER__WME                  = 0x01;               // Enable watchdog on Timer 3 - NOTE: Cannot be cleared once set!

    /****/ volatile UINT32 OIER;                                            // Interrupt enable register

    // PXA271 Timer Counter, Match and Control registers

    /****/ volatile UINT32 OSNR;                                            // Snapshot register

    /****/ volatile UINT32 Padding1[7];
    
    /****/ volatile UINT32 OSCR[8];                                         // PXA271 Timer Count registers

    /****/ volatile UINT32 Padding2[8];

    /****/ volatile UINT32 OSMR[8];                                         // PXA271 Timer Match registers

    /****/ volatile UINT32 Padding3[8];

    /****/ volatile UINT32 OMCR[8];                                         // PXA271 Control registers
    static const    UINT32 OMCR__SNAP                 = 0x200;              // Snapshot mode (only available for Timers 9 & 11)
    static const    UINT32 OMCR__MATCHx               = 0x80;               // Matches its own count register
    static const    UINT32 OMCR__PER                  = 0x40;               // Timer is periodic (does not stop on match)
    static const    UINT32 OMCR__XSYNC_NONE           = 0;                  // No synchronization
    static const    UINT32 OMCR__XSYNC_x0             = 0x10;               // Count reset on rising edge of EXT_SYNC<0>
    static const    UINT32 OMCR__XSYNC_x1             = 0x20;               // Count reset on rising edge of EXT_SYNC<1>
    static const    UINT32 OMCR__ROM                  = 0x08;               // Reset on match
    static const    UINT32 OMCR__CRES_DIS             = 0;                  // Counter is disabled
    static const    UINT32 OMCR__CRES_C32             = 0x01;               // 32768 Hz clock used
    static const    UINT32 OMCR__CRES_1mS             = 0x02;               // 1 KHz Hz clock used (derived from 32768 Hz clock with uneven intervals)
    static const    UINT32 OMCR__CRES_1S              = 0x03;               // 1 Hz clock used (derived from 32768 Hz clock)
    static const    UINT32 OMCR__CRES_1uS             = 0x04;               // 1 MHz clock used (derived from 13 MHz clock)
    static const    UINT32 OMCR__CRES_Xclk            = 0x05;               // External clock used
    static const    UINT32 OMCR__CRES_SSP1            = 0x06;               // SSP1 frame detect rate (only available for Timers 8-11)
    static const    UINT32 OMCR__CRES_SSP2            = 0x07;               // SSP1 frame detect rate (only available for Timers 8-11)
    static const    UINT32 OMCR__CRES_SSP3            = 0x100;              // SSP1 frame detect rate (only available for Timers 8-11)
    static const    UINT32 OMCR__CRES_UDC             = 0x101;              // UDC frame detect rate (only available for Timers 8-11)

};
//
// TIMER
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// USART
//
struct PXA271_USART
{
    static const    UINT32 c_STUART     = 0;
    static const    UINT32 c_BTUART     = 1;
    static const    UINT32 c_FFUART     = 2;


    static const    UINT32 c_Base_FF    = 0x40100000;
    static const    UINT32 c_Base_BT    = 0x40200000;
    static const    UINT32 c_Base_ST    = 0x40700000;

    // max 921600bps
    static const UINT32 c_MAX_BAUDRATE = 115200; 
    static const UINT32 c_MIN_BAUDRATE = 300;
    //--//

    static const UINT32 c_STD_TXD       = PXA271_GPIO::c_Port_47;
    static const UINT32 c_STD_RXD       = PXA271_GPIO::c_Port_46;
    static const UINT32 c_BT_CTS        = PXA271_GPIO::c_Port_44;
    static const UINT32 c_BT_RTS        = PXA271_GPIO::c_Port_45;
    static const UINT32 c_BT_TXD        = PXA271_GPIO::c_Port_43;
    static const UINT32 c_BT_RXD        = PXA271_GPIO::c_Port_42;
    static const UINT32 c_FF_CTS        = PXA271_GPIO::c_Port_100;
    static const UINT32 c_FF_RTS        = PXA271_GPIO::c_Port_98;
    static const UINT32 c_FF_TXD        = PXA271_GPIO::c_Port_99;
    static const UINT32 c_FF_RXD        = PXA271_GPIO::c_Port_96;

    //--//

    /****/ union
    {
           volatile UINT32 RBR;                         // Rx Buffer Register (Read only and enabled by LCR[DLAB] = 0)
           volatile UINT32 THR;                         // Transmit Holding Register  (Write only & enabled by LCR[DLAB] = 0)
           volatile UINT32 DLL;                         // Low order Divisor Register  (Enabled by LCR[DLAB] = 1)
    };
    static const    UINT32 RBR__DATA_MASK8              = 0x000000FF;   // 8-bit data mask
    
    /****/ union
    {
           volatile UINT32 IER;                         // Interrupt Enable Register (Enabled by LCR[DLAB] = 0)
           volatile UINT32 DLH;                         // High order Divisor Register  (Enabled by LCR[DLAB] = 1)
    };
    static const    UINT32 IER__DMAE                    = 0x00000080;   // Enable DMA requests
    static const    UINT32 IER__UUE                     = 0x00000040;   // USART unit enable
    static const    UINT32 IER__NRZE                    = 0x00000020;   // Enable NRZ coding
    static const    UINT32 IER__RTOIE                   = 0x00000010;   // Enable Receiver Timeout interrupt
    static const    UINT32 IER__MIE                     = 0x00000008;   // Enable Modem status interrupt
    static const    UINT32 IER__RLSE                    = 0x00000004;   // Enable Receiver line status interrupt
    static const    UINT32 IER__TIE                     = 0x00000002;   // Enable Transmit Data interrupt
    static const    UINT32 IER__RAVIE                   = 0x00000001;   // Enable Receive Data available interrupt
    
    /****/ union
    {
           volatile UINT32 IIR;                         // Interrupt ID Register  (Read only)
           volatile UINT32 FCR;                         // FIFO Control Register  (Write only)
    };
    static const    UINT32 IIR__FIFOES                  = 0x000000C0;   // Enable FIFO mode
    static const    UINT32 IIR__EOC                     = 0x00000020;   // End of DMA descriptor chain reached
    static const    UINT32 IIR__ABL                     = 0x00000010;   // Auto baud has locked in the rate
    static const    UINT32 IIR__TOD                     = 0x00000008;   // Timeout interrupt is pending
    static const    UINT32 IIR__IID_MASK                = 0x00000007;   // Encoded interrupt source mask
    static const    UINT32 IIR__IID_MODEM_STATUS        = 0x00000000;
    static const    UINT32 IIR__IID_TRANSMIT_FIFO       = 0x00000002;
    static const    UINT32 IIR__IID_RECEIVE_DATA        = 0x00000004;
    static const    UINT32 IIR__IID_RECEIVE_ERROR       = 0x00000006;
    static const    UINT32 IIR__NIP                     = 0x00000001;   // No interrupt is pending (negative logic)

    static const    UINT32 FCR__ITL_1                   = 0x00000000;   // Receive interrupt trip at 1 character
    static const    UINT32 FCR__ITL_8                   = 0x00000040;   // Receive interrupt trip at 8 characters
    static const    UINT32 FCR__ITL_16                  = 0x00000080;   // Receive interrupt trip at 16 characters
    static const    UINT32 FCR__ITL_32                  = 0x000000C0;   // Receive interrupt trip at 32 characters
    static const    UINT32 FCR__BUS_8                   = 0x00000000;   // Peripheral bus to 8 bit
    static const    UINT32 FCR__BUS_32                  = 0x00000020;   // Peripheral bus to 32 bit
    static const    UINT32 FCR__TRAIL                   = 0x00000010;   // Trailing bytes are removed by DMA controller
    static const    UINT32 FCR__TIL_HALF_EMPTY          = 0x00000000;   // Trip Transmit interrupt/DMA when FIFO is half empty
    static const    UINT32 FCR__TIL_EMPTY               = 0x00000008;   // Trip Transmit interrupt/DMA when FIFO is empty
    static const    UINT32 FCR__RESETTF                 = 0x00000004;   // Reset Transmit FIFO
    static const    UINT32 FCR__RESETRF                 = 0x00000002;   // Reset Receive FIFO
    static const    UINT32 FCR__TRFIFOE                 = 0x00000001;   // Transmit & Receive FIFO enable
    
    /****/ volatile UINT32 LCR;                         // Line Control register
    static const    UINT32 LCR__DLAB                    = 0x00000080;   // Access divisor latch registers
    static const    UINT32 LCR__SB                      = 0x00000040;   // Send a break
    static const    UINT32 LCR__STKYP                   = 0x00000020;   // Sticky parity (keep parity bit constant)
    static const    UINT32 LCR__EPS                     = 0x00000010;   // Select even parity
    static const    UINT32 LCR__PEN                     = 0x00000008;   // Enable parity
    static const    UINT32 LCR__STB_1                   = 0x00000000;   // One stop bit
    static const    UINT32 LCR__STB_2                   = 0x00000004;   // Two stop bits
    static const    UINT32 LCR__WLS_7                   = 0x00000002;   // Select 7 bit character size
    static const    UINT32 LCR__WLS_8                   = 0x00000003;   // Select 8 bit character size
    
    /****/ volatile UINT32 MCR;                         // Modem Control register
    static const    UINT32 MCR__AFE                     = 0x00000020;   // Auto handshake (RTS/CTS) enable
    static const    UINT32 MCR__LOOP                    = 0x00000010;   // Enable Loopback mode
    static const    UINT32 MCR__OUT2                    = 0x00000008;   // Enable interrupts to AITC (also Loopback DCD state)
    static const    UINT32 MCR__OUT1                    = 0x00000004;   // Loopback mode RI state
    static const    UINT32 MCR__RTS                     = 0x00000002;   // Enable RTS line (also enable auto RTS)
    static const    UINT32 MCR__DTS                     = 0x00000001;   // Enable DTR line
    
    /****/ volatile UINT32 LSR;                         // Line Status register
    static const    UINT32 LSR__FIFOE                   = 0x00000080;   // A character in the FIFO has an error
    static const    UINT32 LSR__TEMT                    = 0x00000040;   // Transmit FIFO is empty
    static const    UINT32 LSR__TDRQ                    = 0x00000020;   // Transmit FIFO data request
    static const    UINT32 LSR__BI                      = 0x00000010;   // Break signal received
    static const    UINT32 LSR__FE                      = 0x00000008;   // Framing error detected
    static const    UINT32 LSR__PE                      = 0x00000004;   // Parity error detected
    static const    UINT32 LSR__OE                      = 0x00000002;   // Receiver over-run occurred
    static const    UINT32 LSR__DR                      = 0x00000001;   // Received data is ready
    static const    UINT32 LSR__ERRORS                  = 0x0000008E;   // Mask for all error bits
    
    /****/ volatile UINT32 MSR;                         // Modem Status register
    static const    UINT32 MSR__DCD                     = 0x00000080;   // State of the DCD input (1 = active)
    static const    UINT32 MSR__RI                      = 0x00000040;   // State of the RI input (1 = active)
    static const    UINT32 MSR__DSR                     = 0x00000020;   // State of the DSR input (1 = active)
    static const    UINT32 MSR__CTS                     = 0x00000010;   // State of the CTS input (1 = active)
    static const    UINT32 MSR__DDCD                    = 0x00000008;   // Set on change of DCD state
    static const    UINT32 MSR__TERI                    = 0x00000004;   // Set on trailing edge of RI input
    static const    UINT32 MSR__DDSR                    = 0x00000002;   // Set on change of DSR input
    static const    UINT32 MSR__DCTS                    = 0x00000001;   // Set on change of CTS input
    
    /****/ volatile UINT32 SPR;                         // Scratch Pad register (for 16550A compatability only - does nothing)
    
    /****/ volatile UINT32 ISR;                         // Infrared Select register
    
    /****/ volatile UINT32 FOR;                         // Receive FIFO Occupancy register
    static const    UINT32 FOR__MASK                    = 0x0000003F;   // Mask for # characters in Receive FIFO
    
    /****/ volatile UINT32 ABR;                         // Auto Baud Control register
    static const    UINT32 ABR__ABT                     = 0x00000008;   // Use table to calculate baud rate
    static const    UINT32 ABR__ABUP                    = 0x00000004;   // UART programs divisor latch values
    static const    UINT32 ABR__ABLIE                   = 0x00000002;   // Enable Autobaud Lock interrupt
    static const    UINT32 ABR__ABE                     = 0x00000001;   // Autobaud enable
    static const    UINT32 ABR__AB_DISABLE              = 0x00000000;   // Disable Autobaud
    
    /****/ volatile UINT32 ACR;                         // Auto Baud Count register
    static const    UINT32 ACR_MASK                     = 0x0000FFFF;   // Autobaud count mask

};
//
// USART
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// USB Client port
// 
struct PXA271_USB
{

    static const    UINT32 c_Base                       = 0x040600000;  // Base address for all USB Client port registers
    static const    UINT32 c_MaxEndpoints               = 24;           // Including Endpoint 0

    static const    UINT32 c_USBC_GPIOX_EN              = PXA271_GPIO::c_Port_88;       // Must be raised high to signal host
    static const    UINT32 c_USBC_GPION_DET             = PXA271_GPIO::c_Port_13;       // Goes high while host supplies power to board


    /****/ volatile UINT32 UDCCR;                       // UDC Control register
    static const    UINT32 UDCCR__OEN                   = 0x80000000;   // "On The Go" enable (R/W)
    static const    UINT32 UDCCR__AALTHNP               = 0x40000000;   // B-device connected to A-device with HNP capable alternate port (R only)
    static const    UINT32 UDCCR__AHNP                  = 0x20000000;   // B-device connected to HNP capable A-device (R only)
    static const    UINT32 UDCCR__BHNP                  = 0x10000000;   // B-device HNP enabled by host controller (R only)
    static const    UINT32 UDCCR__DWRE                  = 0x00010000;   // Remote wake up feature has been enabled by host (R only)
    static const    UINT32 UDCCR__ACN_mask              = 0x00001800;   // Mask for Active Configuration Number (R only)
    static const    UINT32 UDCCR__ACN_shift             = 11;           // Shift for Active Configuration Number (R only)
    static const    UINT32 UDCCR__AIN_mask              = 0x00000700;   // Mask for Active UDC Interface Number (R only)
    static const    UINT32 UDCCR__AIN_shift             = 8;            // Shift for Active UDC Interface Number (R only)
    static const    UINT32 UDCCR__AAISN_mask            = 0x000000E0;   // Mask for Active UDC Alternate Interface Setting Number (R only)
    static const    UINT32 UDCCR__AAISN_shift           = 5;            // Shift for Active UDC Alternate Interface Setting Number (R only)
    static const    UINT32 UDCCR__SMAC                  = 0x00000010;   // Activate endpoint memory allocation (R/W) (can't deactivate)
    static const    UINT32 UDCCR__EMCE                  = 0x00000008;   // Endpoint memory allocation error (write 1 to clear this)
    static const    UINT32 UDCCR__UDR                   = 0x00000004;   // Force UDC out of suspend state
    static const    UINT32 UDCCR__UDA                   = 0x00000002;   // UDC is currently active on USB bus (R only)
    static const    UINT32 UDCCR__UDE                   = 0x00000001;   // Enable UDC (use USBC_P & USBC_N) (R/W)

    /****/ volatile UINT32 UDCICR0;                     // UDC Interrupt Control register 0
    static const    UINT32 UDCICR__NONE                 = 0;            // No Endpoint interrupts enabled
    static const    UINT32 UDCICR__PCKT                 = 1;            // Endpoint packet complete interrupt enabled
    static const    UINT32 UDCICR__FIFO_ERR             = 2;            // Endpoint FIFO Error interrupt enabled
    static const    UINT32 UDCICR__BOTH                 = 3;            // Both Endpoint Packet complete and Endpoint FIFO Error interrupts enabled
    /****/ volatile UINT32 UDCICR1;                     // UDC Interrupt Control register 1
    static const    UINT32 UDCICR1__IECC                = 0x80000000;   // Interrupt Enable—Configuration Change (R/W)
    static const    UINT32 UDCICR1__IESOF               = 0x40000000;   // Interrupt Enable—Start of Frame (R/W)
    static const    UINT32 UDCICR1__IERU                = 0x20000000;   // Interrupt Enable—Resume (R/W)
    static const    UINT32 UDCICR1__IESU                = 0x10000000;   // Interrupt Enable-Suspend (R/W)
    static const    UINT32 UDCICR1__IERS                = 0x08000000;   // Interrupt Enable-Reset (R/W)

    /****/ volatile UINT32 UDCISR0;                     // UDC Interrupt Status register 0
    /****/ volatile UINT32 UDCISR1;                     // UDC Interrupt Status register 1
    static const    UINT32 UDCISR1__IRCC                = 0x80000000;   // Configuration Change interrupt occurred (write 1 to clear)
    static const    UINT32 UDCISR1__IRSOF               = 0x40000000;   // Start of Frame interrupt occurred (write 1 to clear)
    static const    UINT32 UDCISR1__IRRU                = 0x20000000;   // Resume interrupt occurred (write 1 to clear)
    static const    UINT32 UDCISR1__IRSU                = 0x10000000;   // Suspend interrupt occurred (write 1 to clear)
    static const    UINT32 UDCISR1__IRRS                = 0x08000000;   // Reset interrupt occurred (write 1 to clear)

    /****/ volatile UINT32 UDCFNR;                      // UDC Frame Number register
    static const    UINT32 UDCFNR__FN_mask              = 0x000007FF;   // Mask for frame number associated with last received SOF (R only)

    /****/ volatile UINT32 UDCOTGICR;                   // UDC OTG Interrupt Control register
    static const    UINT32 UDCOTGICR__IESF              = 0x01000000;   // Interrupt Enable - OTG SET_FEATURE Command Received
    static const    UINT32 UDCOTGICR__IEXR              = 0x00020000;   // Interrupt Enable - External Transceiver Interrupt Rising-Edge Interrupt Enable
    static const    UINT32 UDCOTGICR__IEXF              = 0x00010000;   // Interrupt Enable - External Transceiver Interrupt Falling-Edge Interrupt Enable
    static const    UINT32 UDCOTGICR__IEVV40R           = 0x00000200;   // Interrupt Enable - OTG Vbus Valid 4.0-V Rising-Edge Interrupt Enable
    static const    UINT32 UDCOTGICR__IEVV40F           = 0x00000100;   // Interrupt Enable - OTG Vbus Valid 4.0-V Falling-Edge Interrupt Enable
    static const    UINT32 UDCOTGICR__IEVV44R           = 0x00000080;   // Interrupt Enable - OTG Vbus Valid 4.4-V Rising-Edge Interrupt Enable
    static const    UINT32 UDCOTGICR__IEVV44F           = 0x00000040;   // Interrupt Enable - OTG Vbus Valid 4.4-V Falling-Edge Interrupt Enable
    static const    UINT32 UDCOTGICR__IESVR             = 0x00000020;   // Interrupt Enable - OTG Session Valid Rising-Edge Interrupt Enable
    static const    UINT32 UDCOTGICR__IESVF             = 0x00000010;   // Interrupt Enable - OTG Session Valid Falling-Edge Interrupt Enable
    static const    UINT32 UDCOTGICR__IESDR             = 0x00000008;   // Interrupt Enable - OTG A-Device SRP Detect Rising-Edge Interrupt Enable
    static const    UINT32 UDCOTGICR__IESDF             = 0x00000004;   // Interrupt Enable - OTG A-Device SRP Detect Falling-Edge Interrupt Enable
    static const    UINT32 UDCOTGICR__IEIDR             = 0x00000002;   // Interrupt Enable - OTG ID Change Rising-Edge Interrupt Enable
    static const    UINT32 UDCOTGICR__IEIDF             = 0x00000001;   // Interrupt Enable - OTG ID Change Falling-Edge Interrupt Enable

    /****/ volatile UINT32 UDCOTGISR;                   // UDC OTG Interrupt Status register
    static const    UINT32 UDCOTGISR__IRSF              = 0x01000000;   // OTG SET_FEATURE Command Received (write 1 to clear)
    static const    UINT32 UDCOTGISR__IRXR              = 0x00020000;   // External Transceiver Interrupt Rising-Edge Interrupt Enable (write 1 to clear)
    static const    UINT32 UDCOTGISR__IRXF              = 0x00010000;   // External Transceiver Interrupt Falling-Edge Interrupt Enable (write 1 to clear)
    static const    UINT32 UDCOTGISR__IRVV40R           = 0x00000200;   // OTG Vbus Valid 4.0-V Rising-Edge Interrupt Enable (write 1 to clear)
    static const    UINT32 UDCOTGISR__IRVV40F           = 0x00000100;   // OTG Vbus Valid 4.0-V Falling-Edge Interrupt Enable (write 1 to clear)
    static const    UINT32 UDCOTGISR__IRVV44R           = 0x00000080;   // OTG Vbus Valid 4.4-V Rising-Edge Interrupt Enable (write 1 to clear)
    static const    UINT32 UDCOTGISR__IRVV44F           = 0x00000040;   // OTG Vbus Valid 4.4-V Falling-Edge Interrupt Enable (write 1 to clear)
    static const    UINT32 UDCOTGISR__IRSVR             = 0x00000020;   // OTG Session Valid Rising-Edge Interrupt Enable (write 1 to clear)
    static const    UINT32 UDCOTGISR__IRSVF             = 0x00000010;   // OTG Session Valid Falling-Edge Interrupt Enable (write 1 to clear)
    static const    UINT32 UDCOTGISR__IRSDR             = 0x00000008;   // OTG A-Device SRP Detect Rising-Edge Interrupt Enable (write 1 to clear)
    static const    UINT32 UDCOTGISR__IRSDF             = 0x00000004;   // OTG A-Device SRP Detect Falling-Edge Interrupt Enable (write 1 to clear)
    static const    UINT32 UDCOTGISR__IRIDR             = 0x00000002;   // OTG ID Change Rising-Edge Interrupt Enable (write 1 to clear)
    static const    UINT32 UDCOTGISR__IRIDF             = 0x00000001;   // OTG ID Change Falling-Edge Interrupt Enable (write 1 to clear)

    /****/ volatile UINT32 UP2OCR;                      // USB Port 2 Output Control register
    static const    UINT32 UP2OCR__SEOS_OFF             = 0x00000000;   // Single ended output off - no output selected
    static const    UINT32 UP20CR__SEOS_NU              = 0x01000000;   // Single ended output not used
    static const    UINT32 UP2OCR__SEOS_DGPIOX          = 0x02000000;   // Device controller single-ended operation using GPIO pads with an external transceiver
    static const    UINT32 UP2OCR__SEOS_HGPIOX          = 0x03000000;   // Host controller single-ended operation using GPIO pads with an external transceiver.
    static const    UINT32 UP2OCR__SEOS_DXOTG           = 0x04000000;   // Device controller with external OTG transceiver
    static const    UINT32 UP2OCR__SEOS_HXOTG           = 0x05000000;   // Host controller with external OTG transceiver
    static const    UINT32 UP2OCR__SEOS_DXCP            = 0x06000000;   // Device controller with external charge pump
    static const    UINT32 UP2OCR__SEOS_HXCP            = 0x07000000;   // Host controller with external charge pump
    static const    UINT32 UP2OCR__HXOE                 = 0x00020000;   // On-chip host controller transceiver enabled
    static const    UINT32 UP2OCR__HXS                  = 0x00010000;   // Output signals from USB host controller
    static const    UINT32 UP2OCR__IDON                 = 0x00000400;   // OTG ID Read Enable
    static const    UINT32 UP2OCR__EXSUS                = 0x00000200;   // External Transceiver Suspend Enable
    static const    UINT32 UP2OCR__EXSP                 = 0x00000100;   // External Transceiver Speed Control
    static const    UINT32 UP2OCR__DMSTATE              = 0x00000080;   // Host Port 2 D- line after two 26MHz clock delay or approximately one 12MHz USB clock (R only)
    static const    UINT32 UP2OCR__VPMBlockEnbN         = 0x00000040;   // USB_P3_6 (VPO) and USB_P3_4 (VMO) are not ignored by USB Host Port 3 when USB_P3_2 (OE_n) is deasserted
    static const    UINT32 UP2OCR__DPSTATE              = 0x00000020;   // Host Port 2 D+ line after two 26MHz clock delay or approximately one 12MHz USB clock (R only)
    static const    UINT32 UP2OCR__DPPUE                = 0x00000010;   // Host Port 2 Transceiver D+ Pull Up Enable
    static const    UINT32 UP2OCR__DMPDE                = 0x00000008;   // Host Port 2 Transceiver D– Pull Down Enable
    static const    UINT32 UP2OCR__DPPDE                = 0x00000004;   // Host Port 2 Transceiver D+ Pull Down Enable
    static const    UINT32 UP2OCR__CPVPE                = 0x00000002;   // Charge Pump Vbus Pulse Enable
    static const    UINT32 UP2OCR__CPVEN                = 0x00000001;   // Charge Pump Vbus Enable

    /****/ volatile UINT32 UP3OCR;                      // USB Port 3 Output Control register
    static const    UINT32 UP3OCR__CFG_XHC              = 0x00000000;   // Host controller port 3 transceiver with external host controller
    static const    UINT32 UP3OCR__CFG_NU               = 0x00000001;   // Not used
    static const    UINT32 UP3OCR__CFG_XDC              = 0x00000002;   // Host controller port 3 transceiver with external device controller
    static const    UINT32 UP3OCR__CFG_SEO              = 0x00000003;   // Force SEO mode

    /****/ volatile UINT32 Padding1[54];
    
    /****/ volatile UINT32 UDCCSRx[c_MaxEndpoints];     // UDC Control/Status register
    static const    UINT32 UDCCSR__ACM                  = 0x00000200;   // (EP0 only) Require AREN=1 to ACK to SET_CONFIGURATION and SET_INTERFACE (R/W)
    static const    UINT32 UDCCSR__DPE                  = 0x00000200;   // (not EP0) OUT Data packet error (R only)
    static const    UINT32 UDCCSR__AREN                 = 0x00000100;   // (EP0 only) Send ACK response to SET_CONFIGURATION and SET_INTERFACE commands (Write 1 to set)
    static const    UINT32 UDCCSR__FEF                  = 0x00000100;   // (not EP0) Flush endpoint transmit/receive FIFO (Write 1 to set)
    static const    UINT32 UDCCSR__SA                   = 0x00000080;   // (EP0 only) Setup command received (Write 1 to clear)
    static const    UINT32 UDCCSR__SP                   = 0x00000080;   // (not EP0) IN: short packet ready to send (Write 1 to set), OUT: Short packet received (R only)
    static const    UINT32 UDCCSR__RNE                  = 0x00000040;   // (EP0 only) Receive FIFO not empty (R only)
    static const    UINT32 UDCCSR__BNx                  = 0x00000040;   // (not EP0) IN: BNF Buffer not full, OUT: BNE Buffer not empty (R only)
    static const    UINT32 UDCCSR__FST                  = 0x00000020;   // Force Stall handshake (Write 1 to set)
    static const    UINT32 UDCCSR__SST                  = 0x00000010;   // A Stall needed to be sent (Write 1 to clear)
    static const    UINT32 UDCCSR__DME                  = 0x00000008;   // DMA request enable (R/W)
    static const    UINT32 UDCCSR__FTF                  = 0x00000004;   // (EP0 only) Flush transmit FIFO (Write 1 to set)
    static const    UINT32 UDCCSR__TRN                  = 0x00000004;   // (not EP0) Tx/Rx NAK - IN: Data request with empty FIFO, OUT: Data received, but FIFO is full (Write 1 to clear)
    static const    UINT32 UDCCSR__IPR                  = 0x00000002;   // (EP0 only) IN packet ready (Write 1 to set)
    static const    UINT32 UDCCSR__PC                   = 0x00000002;   // (not EP0) Packet correctly complete IN: packet sent with no errors, OUT: packet received with no errors (Write 1 to clear)
    static const    UINT32 UDCCSR__OPC                  = 0x00000001;   // (EP0 only) OUT packet received (Write 1 to clear - only after all bytes read from FIFO)
    static const    UINT32 UDCCSR__FS                   = 0x00000001;   // (not EP0) FIFO needs service IN: FIFO has room for at least one packet, OUT: FIFO holds at least 1 complete packet (R only)

    /****/ volatile UINT32 Padding2[40];

    /****/ volatile UINT32 UDCBCRx[c_MaxEndpoints];     // UDC Byte Count register
    static const    UINT32 UDCBCR_mask                  = 0x000003FF;   // Number of characters remaining in IN FIFO (not available for OUT Endpoints)

    /****/ volatile UINT32 Padding3[40];

    /****/ volatile UINT32 UDCDRx[c_MaxEndpoints];      // UDC Data register

    /****/ volatile UINT32 Padding4[40];

    /****/ volatile UINT32 UDCCRx[c_MaxEndpoints];      // UDC Configuration register - NOTE: No Configuration register for endpoint 0!! Also, becomes R only after UDCCR[UDE] = 1
    static const    UINT32 UDCCR__CN_mask               = 0x06000000;   // Mask for the configuration number of this endpoint
    static const    UINT32 UDCCR__CN_shift              = 25;           // Shift for the configuration number of this endpoint
    static const    UINT32 UDCCR__IN_mask               = 0x01C00000;   // Mask for the interface number of this endpoint
    static const    UINT32 UDCCR__IN_shift              = 22;           // Shift for the interface number of this endpoint
    static const    UINT32 UDCCR__AISN_mask             = 0x00380000;   // Mask for the Alternate interface setting number of this endpoint
    static const    UINT32 UDCCR__AISN_shift            = 19;           // Shift for the Alternate interface setting number of this endpoint
    static const    UINT32 UDCCR__EN_mask               = 0x00078000;   // Mask for the Endpoint number of this endpoint
    static const    UINT32 UDCCR__EN_shift              = 15;           // Shift for the Endpoint number of this endpoint
    static const    UINT32 UDCCR__ET_NU                 = 0x00000000;   // Endpoint Type:  Not used
    static const    UINT32 UDCCR__ET_ISO                = 0x00002000;   // Endpoint Type:  Isochronous
    static const    UINT32 UDCCR__ET_BULK               = 0x00004000;   // Endpoint Type:  Bulk
    static const    UINT32 UDCCR__ET_INT                = 0x00006000;   // Endpoint Type:  Interrupt
    static const    UINT32 UDCCR__ED_OUT                = 0x00000000;   // Endpoint direction: OUT
    static const    UINT32 UDCCR__ED_IN                 = 0x00001000;   // Endpoint direction: IN
    static const    UINT32 UDCCR__MPS_mask              = 0x00000FFC;   // Mask for Endpoint Maximum Packet Size
    static const    UINT32 UDCCR__MPS_shift             = 2;            // Shift for Endpoint Maximum Packet Size
    static const    UINT32 UDCCR__DE                    = 0x00000002;   // Enable Double Buffering
    static const    UINT32 UDCCR__EE                    = 0x00000001;   // Enable Endpoint

    // Note that endpoint memory has 1K 32-bit words available; the words cannot
    // be broken up between separate endpoints.  This is true also of double buffering.
    union EndpointConfiguration
    {
        struct
        {
            unsigned EE   :  1;      // Endpoint enable (1 = enable)
            unsigned DE   :  1;      // Double buffer enable (1 = double buffered)
            unsigned MPS  : 10;      // Maximum packet size (iso=1-1023, blk=8,16,32,64, int=1-64
            unsigned ED   :  1;      // Endpoint direction (1 = IN)
            unsigned ET   :  2;      // Endpoint type (1=iso, 2=blk, 3=int)
            unsigned EN   :  4;      // Endpoint number (1-15)
            unsigned AISN :  3;      // Alternate Interface number
            unsigned IN   :  3;      // Interface number
            unsigned CN   :  2;      // Configuration number
        } bits;
        UINT32 word;
    };

    
    void Set_Interrupt(int EPn, UINT32 interrupt)
    {
        UINT32 mask = 3;
        int    shift;

        ASSERT(EPn < c_MaxEndpoints);

        if(EPn < 16)
        {
            shift = EPn * 2;
            mask <<= shift;
            UDCICR0 = (UDCICR0 & ~mask) | (interrupt << shift);
        }
        else
        {
            shift = (EPn - 16) * 2;
            mask <<= shift;
            UDCICR1 = (UDCICR1 & ~mask) | (interrupt << shift);
        }
    }

};
//
// USB
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// I2C
//
struct PXA271_I2C
{
    static const UINT32 c_Base = 0x40301680;

    //--//

    static const UINT32 c_I2C_SDA                       = PXA271_GPIO::c_Port_118;
    static const UINT32 c_I2C_SCL                       = PXA271_GPIO::c_Port_117;

    //--//

    /****/ volatile UINT32 IBMR;                        // I2C Bus Monitor Register
    static const    UINT32 IBMR_SDA                     = 0x00000001; // SDA pin value
    static const    UINT32 IBMR_SCL                     = 0x00000002; // SCL pin value
    
    /****/ volatile UINT32 Padding1;

    /****/ volatile UINT32 IDBR;                        // Data buffer register  
    static const    UINT32 IDBR_DATA_MASK               = 0x000000FF; // Data buffer
    
    /****/ volatile UINT32 Padding2;
    
    /****/ volatile UINT32 ICR;                         // Control register
    static const    UINT32 ICR__FM                      = 0x00008000; // Fast Mode
    static const    UINT32 ICR__FM_100KPS               =          0; // 
    static const    UINT32 ICR__FM_400KPS               =          1; //
    static const    UINT32 ICR__UR                      = 0x00004000; // UNIT RESET
    static const    UINT32 ICR__UR_NO_RESET             =          0; // 
    static const    UINT32 ICR__UR_RESET                =          1; //
    static const    UINT32 ICR__SADIE                   = 0x00002000; // Subordinate ADDRESS DETECTED INTERUPT ENABLE
    static const    UINT32 ICR__ALDIE                   = 0x00001000; // ARBITRATION LOSS DETECTED INTERRUPT ENABLE
    static const    UINT32 ICR__SSDIE                   = 0x00000800; // Subordinate STOP DETECTED INTERRUPT ENABLE
    static const    UINT32 ICR__BEIE                    = 0x00000400; // BUS ERROR INTERRUPT ENABLE
    static const    UINT32 ICR__DRFIE                   = 0x00000200; // DBR RECEIVE INTERRUPT ENABLE
    static const    UINT32 ICR__ITEIE                   = 0x00000100; // IDBR TRANSIT EMPTY INTERRUPT ENABLE
    static const    UINT32 ICR__GCD                     = 0x00000080; // GENERAL CALL DISABLE
    static const    UINT32 ICR__IUE                     = 0x00000040; // I2C UNIT ENABLE
    static const    UINT32 ICR__SCLEA                   = 0x00000020; // SCL ENABLE
    static const    UINT32 ICR__MA                      = 0x00000010; // MASTER ABORT
    static const    UINT32 ICR__TB                      = 0x00000008; // TRANSFER BYTE
    static const    UINT32 ICR__ACKNAK                  = 0x00000004; // POS/NEG ACK
    static const    UINT32 ICR__STOP                    = 0x00000002; // STOP
    static const    UINT32 ICR__START                   = 0x00000001; // START

    /****/ volatile UINT32 Padding3;

    /****/ volatile UINT32 ISR;                         // Status register
    static const    UINT32 ISR__BED                     = 0x00000400; // BUS ERROR DETECTED
    static const    UINT32 ISR__SAD                     = 0x00000200; // Subordinate ADDRESS DETECTED
    static const    UINT32 ISR__GCAD                    = 0x00000100; // GENERAL CALL ADDRESS DETECTED
    static const    UINT32 ISR__IRF                     = 0x00000080; // IDBR RECEIVE FULL
    static const    UINT32 ISR__ITE                     = 0x00000040; // IDBR TRANSMIT EMPTY
    static const    UINT32 ISR__ALD                     = 0x00000020; // ARBITRATION LOSS DETECTED
    static const    UINT32 ISR__SSD                     = 0x00000010; // Subordinate STOP DETECTED
    static const    UINT32 ISR__IBB                     = 0x00000008; // I2C BUS BUSY
    static const    UINT32 ISR__UB                      = 0x00000004; // UNIT BUSY
    static const    UINT32 ISR__ACKNAK                  = 0x00000002; // ACK/NAK STATUS
    static const    UINT32 ISR__RWM                     = 0x00000001; // READ/WRITE MODE

    /****/ volatile UINT32 Padding4;
    
    /****/ volatile UINT32 ISAR;                             // Subordinate address  register
    static const    UINT32 ISAR__SL_ADDR_MASK           = 0x0000007F;
 
};
//
// I2C
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// SPI / Motorola
//
struct PXA271_SPI
{
    static const UINT32 c_SPI1    = 0;
    static const UINT32 c_SPI2    = 1;
    static const UINT32 c_SPI3    = 2;    
    static const UINT32 c_MAX_SPI = 3;

    
    static const UINT32 c_Base1    = 0x41000000;//SSP1 reg base
    static const UINT32 c_Base2    = 0x41700000;//SSP2 reg base   
    static const UINT32 c_Base3    = 0x41900000;//SSP3 reg base   
    
    static const UINT32 c_SSP1_CLK    = PXA271_GPIO::c_Port_23; //msk
    static const UINT32 c_SSP1_TXD    = PXA271_GPIO::c_Port_25; //miso
    static const UINT32 c_SSP1_SFRM   = PXA271_GPIO::c_Port_24;
    static const UINT32 c_SSP1_RXD    = PXA271_GPIO::c_Port_26; //mosi
    
    static const UINT32 c_SSP2_CLK    = PXA271_GPIO::c_Port_36;
    static const UINT32 c_SSP2_TXD    = PXA271_GPIO::c_Port_38;
    static const UINT32 c_SSP2_SFRM   = PXA271_GPIO::c_Port_37;
    static const UINT32 c_SSP2_RXD    = PXA271_GPIO::c_Port_11;

    static const UINT32 c_SSP3_CLK    = PXA271_GPIO::c_Port_34;
    static const UINT32 c_SSP3_TXD    = PXA271_GPIO::c_Port_35;
    static const UINT32 c_SSP3_SFRM   = PXA271_GPIO::c_Port_39;
    static const UINT32 c_SSP3_RXD    = PXA271_GPIO::c_Port_41;    
    
    //--//
    volatile UINT32 SSP_SSCR0;
    
    static const    UINT32 SSP_SSCR0__MOD    = 0x00000000;//Normal mode
    __inline static UINT32 SSP_SSCR0__SCR__set( UINT32 d ) { return  (d & 0xFFF) << 8; }//clock rate  
    static const    UINT32 SSP_SSCR0__SSE    = 0x00000080;//SSP port operation Enable
    static const    UINT32 SSP_SSCR0__FRF    = 0x00000000;//Motorola SPI
    __inline static UINT32 SSP_SSCR0_DSS__set( UINT32 d ) { return  (d & 0xF) << 0; }//Data Size select    

    /****/ volatile UINT32 SSP_SSCR1;
    
    static const    UINT32 SSP_SSCR1__TTE       = 0x40000000;//TXD tristate Enable
    static const    UINT32 SSP_SSCR1__SCLKDIR   = 0x00000000;//Port is in Master mode
    static const    UINT32 SSP_SSCR1__SFRMDIR   = 0x00000000;//Port is in Master mode
    static const    UINT32 SSP_SSCR1__SPOL      = 0x00000000;//Motorola SPi Polarity
    static const    UINT32 SSP_SSCR1__SPOH      = 0x00000008;//Motorola SPi Polarity
    static const    UINT32 SSP_SSCR1__SPHL      = 0x00000000;//Motorola SPi Phase
    static const    UINT32 SSP_SSCR1__SPHH      = 0x00000010;//Motorola SPi Phase          
    

    /****/ volatile UINT32 SSP_SSSR;
    
    static const    UINT32 SSP_SSSR__TNF    = 0x00000004;//TX FIFO not Full
    static const    UINT32 SSP_SSSR__RNE    = 0x00000008;//RX FIFO is not empty
    static const    UINT32 SSP_SSSR__BSY    = 0x00000010;//Busy
    static const    UINT32 SSP_SSSR__TFL    = 0x00000F00;//TXFIFO level
    static const    UINT32 SSP_SSSR__RFL    = 0x0000F000;//RXFIFO level
   
    /*************/volatile UINT32 Padding; 
    
    /****/ volatile UINT32 SSP_SSDR;//data register    

    // __inline Helpers for this processor core

    __inline BOOL TransmitFifoNotEmpty()
    {
        // from spec: When the value of 0x0 is read, the TX FIFO is either empty or full and programmers must refer to the TNF bit.
        return ((SSP_SSSR & SSP_SSSR__TFL) != 0 || (SSP_SSSR & SSP_SSSR__TNF) == 0);
    }


    __inline BOOL ReceiveFifoEmpty()
    {
        return (SSP_SSSR & SSP_SSSR__RNE) == 0;
    }


    __inline BOOL ShiftBufferEmpty()
    {
        return (SSP_SSSR & SSP_SSSR__BSY) == 0;
    }
     
    __inline static UINT32 ConvertClockRateToDivisor(UINT32 Clock_RateKHz)
    {
        #define SSP_CLOCK_KHZ  13000
        
        if(Clock_RateKHz >= SSP_CLOCK_KHZ)   return 1;
        else  if(Clock_RateKHz <= (SSP_CLOCK_KHZ/4096))   return 4096;
        else return(((SSP_CLOCK_KHZ+Clock_RateKHz-1)/Clock_RateKHz));
    }

    __inline static UINT32 ConfigurationControlReg0( const SPI_CONFIGURATION& Configuration )
    {
        UINT32 Mode;

        Mode = SSP_SSCR0__MOD;
        Mode |= SSP_SSCR0__FRF;   

        if(Configuration.MD_16bits)
        {
                  
         Mode |= SSP_SSCR0_DSS__set(15);
              
        }
        else
        {
                 
         Mode |= SSP_SSCR0_DSS__set(7);
            
        }
        
        Mode |= SSP_SSCR0__SCR__set((ConvertClockRateToDivisor(Configuration.Clock_RateKHz)-1));     
       
        return Mode;   
    }
 

    // only used in 1 place in RTM builds, so inline this for quicker access
    __inline static UINT32 ConfigurationControlReg1( const SPI_CONFIGURATION& Configuration )
    {
        UINT32 Mode;

        Mode = 0x00000000;
       
        if(Configuration.MSK_IDLE==FALSE)
        {
            if(Configuration.MSK_SampleEdge == FALSE)                 
                Mode |= SSP_SSCR1__SPHH;                  
        }
        else
        { 
            Mode |= SSP_SSCR1__SPOH; 
            if(Configuration.MSK_SampleEdge == TRUE)                 
                Mode |= SSP_SSCR1__SPHH;        
        }
       
        Mode |= SSP_SSCR1__SCLKDIR;
        Mode |= SSP_SSCR1__SFRMDIR;
        Mode |= SSP_SSCR1__TTE;  
       
        return Mode;
    }
};
//
// SPI / Microwire

/**********************************************************************************************************************/
/**********************************************************************************************************************/
/**********************************************************************************************************************/
/**********************************************************************************************************************/
/**********************************************************************************************************************/

struct PXA271
{
    static const UINT32 c_UncachableMask = 0x80000000;

    static PXA271_POWER_MANAGER & PWRMNGR (          ) { return *(PXA271_POWER_MANAGER*)(size_t)(    PXA271_POWER_MANAGER::c_Base                                  ); }
    static PXA271_CLOCK_MANAGER & CLKMNGR (          ) { return *(PXA271_CLOCK_MANAGER*)(size_t)(    PXA271_CLOCK_MANAGER::c_Base                                  ); }
    static PXA271_DMA           & DMA     (          ) { return *(PXA271_DMA     *)(size_t)(        PXA271_DMA     ::c_Base                                      ); }
    static PXA271_MEMORY        & MEMORY  (          ) { return *(PXA271_MEMORY  *)(size_t)(        PXA271_MEMORY  ::c_Base                                      ); }
    static PXA271_LCDC          & LCDC    (          ) { return *(PXA271_LCDC    *)(size_t)(        PXA271_LCDC    ::c_Base                                      ); }
    static PXA271_PWM           & PWM     ( int  sel ) { return *(PXA271_PWM     *)(size_t)(        PXA271_PWM     ::c_Base_0 + (sel&1)*PXA271_PWM ::c_Base_offset1 + (sel>>1)*PXA271_PWM::c_Base_offset2); }
    static PXA271_AITC          & AITC    (          ) { return *(PXA271_AITC    *)(size_t)(        PXA271_AITC    ::c_Base                                      ); }
    static PXA271_GPIO          & GPIO    (          ) { return *(PXA271_GPIO    *)(size_t)(        PXA271_GPIO    ::c_Base                                      ); }
    static PXA271_I2C           & I2C     (          ) { return *(PXA271_I2C     *)(size_t)(        PXA271_I2C     ::c_Base                                      ); }
    static PXA271_USB           & USB     (          ) { return *(PXA271_USB     *)(size_t)(        PXA271_USB     ::c_Base                                      ); }
    static PXA271_TIMER         & TIMER   (          ) ;
//  static PXA271_WATCHDOG      & WTDG    (          ) { return *(PXA271_WATCHDOG*)(size_t)(        PXA271_WATCHDOG::c_Base                                      ); }
    static PXA271_USART         & USART   ( int  sel ) {	   if(sel == PXA271_USART::c_FFUART) return *(PXA271_USART   *)(size_t)PXA271_USART::c_Base_FF;
														  else if(sel == PXA271_USART::c_BTUART) return *(PXA271_USART   *)(size_t)PXA271_USART::c_Base_BT;
														  else									 return *(PXA271_USART   *)(size_t)PXA271_USART::c_Base_ST;			}
    
	static PXA271_SPI           & SPI     ( int  sel ) {	   if(sel == PXA271_SPI::c_SPI1)	return *(PXA271_SPI   *)(size_t)PXA271_SPI::c_Base1;
														  else if(sel == PXA271_SPI::c_SPI2)	return *(PXA271_SPI   *)(size_t)PXA271_SPI::c_Base2;
														  else									return *(PXA271_SPI   *)(size_t)PXA271_SPI::c_Base3; 			    }

//
/// To be removed

//    static PXA271_EIM     & EIM  (         ) { return *(PXA271_EIM     *)(size_t)(      PXA271_EIM     ::c_Base                                      ); }
//    static PXA271_SC      & SC   (         ) { return *(PXA271_SC      *)(size_t)(      PXA271_SC      ::c_Base                                      ); }
//    static PXA271_SDRAM   & SDRAM(         ) { return *(PXA271_SDRAM   *)(size_t)(      PXA271_SDRAM   ::c_Base                                      ); }
//    static PXA271_CMU     & CMU  (         ) { return *(PXA271_CMU     *)(size_t)(      PXA271_CMU     ::c_Base                                      ); }



    //--//

    static void Debug_Emit( char c )
    {
        PXA271_USART& USART = PXA271::USART( 0 );
    }

    static void Debug_NewLine()
    {
        PXA271_USART& USART = PXA271::USART( 0 );

    }

    static void Debug_EmitHEX( UINT32 value );

    //--//

    static void Debug_Emit2( char c )
    {
    }

    static void Debug_NewLine2()
    {
    }

    static void Debug_EmitHEX2( UINT32 value );
};

//
//
//
// LAYOUT SECTION
/////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////
// DRIVER SECTION
//
//


/////////////////////////////////////////////////////////////////////////////////////////////////
// Cache
//
//
#if defined(PLATFORM_ARM_PXA271)

template <typename T> void CPU_InvalidateAddress( T* address )
{
    UINT32 reg = 0;

#ifdef __GNUC__
	asm("MOV	%0, #0" : "=r" (reg));
	asm("MCR	p15, 0, %0, c7, c10, 4" :: "r" (reg));
#else
    __asm
    {
        mov     reg, #0
        mcr     p15, 0, reg, c7, c10, 4           // Drain Write Buffers.
    }
#endif

    CPU_CPWAIT();

#ifdef __GNUC__
	asm("MCR	p15, 0, %0, c7, c5, 1" :: "r" (address));
#else
    __asm
    {
        mcr     p15, 0, address, c7, c5, 1        // Invalidate DCache.
    }
#endif

    CPU_CPWAIT();

#ifdef __GNUC__
	asm("MCR	p15, 0, %0, c7, c6, 1" :: "r" (address));
#else
    __asm
    {
        mcr     p15, 0, address, c7, c6, 1        // Invalidate DCache.
    }
#endif

    CPU_CPWAIT();
}

#endif
//////////////////////////////////////////////////////////////////////////////
// Clock Management Unit driver
//

struct PXA271_Clock_Driver
{
    static const UINT32 PERCLK1 = 0;
    static const UINT32 PERCLK2 = 1;
    static const UINT32 PERCLK3 = 2;

    static void ClockDivisor( UINT32 peripheral, UINT32 divisor );

    static UINT32 ReadClockDivisor( UINT32 peripheral );
};

//
// Clock Management Unit driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Watchdog driver
//

struct PXA271_WATCHDOG_Driver
{
    static const UINT32 c_CountsPerMillisecond  = SYSTEM_CLOCK_HZ / 1000;
    static const UINT32 c_MaxMilliseconds       = (UINT32)0xFFFFF000 / c_CountsPerMillisecond;
    static const BOOL   c_WatchdogUsesInterrupt = TRUE;

    //--//

    static HRESULT Enable( UINT32 TimeoutMilliseconds, WATCHDOG_INTERRUPT_CALLBACK callback, void* context );

    static void Disable();

    static void ResetCounter();

    static void ResetCpu();

private:

    UINT32                      m_ResetCount;
    WATCHDOG_INTERRUPT_CALLBACK m_callback;
    void*                       m_context;

    //--//
    
    static void ISR( void* Param );
};

extern PXA271_WATCHDOG_Driver g_PXA271_WATCHDOG_Driver;

//
// Watchdog driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PXA271 driver
//

struct PXA271_Driver
{
    static const UINT32 c_SystemTime_Timer         = 0;
    static const UINT32 c_Watchdog_Timer           = 3;
    static const UINT32 c_PerformanceCounter_Timer = 4;

#if !defined(BUILD_RTM)
    volatile UINT32* m_PerformanceCounter;
#endif

    //--//

    static BOOL Initialize();

    static void Pause();

    static void Sleep();

    static void Halt();

    static void Reset();

    static void Shutdown();

    static void Hibernate();

#if !defined(BUILD_RTM)
    static void   PerformanceCounter_Initialize  ();
    static void   PerformanceCounter_Uninitialize();
    static UINT32 PerformanceCounter             ();
#endif
};

extern PXA271_Driver g_PXA271_Driver;

//
// PXA271 driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Interrupt Controller driver
//

struct PXA271_AITC_Driver
{
    static const UINT32 c_VECTORING_GUARD = 34;

    //--//

    struct IRQ_VECTORING
    {
        UINT32       Priority;
        HAL_CALLBACK Handler;
    };

    //--//

    static IRQ_VECTORING s_IsrTable[];

    //--//

    static void Initialize();

    static BOOL ActivateInterrupt  ( UINT32 Irq_Index, BOOL Fast, HAL_CALLBACK_FPN ISR, void* ISR_Param);
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
// GP I/O driver
//

struct PXA271_GPIO_Driver
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

    static const UINT32 c_MaxPins           = 121;
    static const UINT32 c_MaxPorts          =   4;
    static const UINT32 c_PinsPerPort       =  32;

    static const INT32  c_MinDebounceTimeMs =    1; // 1 ms
    static const INT32  c_MaxDebounceTimeMs = 5000; // 5  s

    //--//

    static const UINT8 c_Gpio_Attributes[c_MaxPins];

    // applies to all GPIO pins
    static const UINT8 c_NumberofResistorMode = 2;
    static const UINT8 c_GPIO_ResistorMode=(1<<RESISTOR_DISABLED) | (1<<RESISTOR_PULLUP);  

    // applies to all GPIO pins
    static const UINT8 c_NumberofInterruptMode = 5;
    static const UINT8 c_GPIO_InterruptMode=(1<<GPIO_INT_EDGE_LOW) | (1<<GPIO_INT_EDGE_HIGH ) | (1<<GPIO_INT_EDGE_BOTH) | (1<<GPIO_INT_LEVEL_HIGH) | (1<<GPIO_INT_LEVEL_LOW);
    

    PIN_ISR_DESCRIPTOR m_PinIsr            [c_MaxPins ];
    UINT32             m_PinReservationInfo[c_MaxPorts];
    UINT32             m_DebounceTicks;

    //--//

    static BOOL Initialize  ();

    static BOOL Uninitialize();

    static UINT32 Attributes( GPIO_PIN Pin );

    static void DisablePin( GPIO_PIN Pin, GPIO_RESISTOR ResistorState, UINT32 direction, GPIO_ALT_MODE function );

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

    static void GetPinsMap( UINT8* pins, size_t size );
    
    static UINT8 GetSupportedResistorModes( GPIO_PIN pin );

    static UINT8 GetSupportedInterruptModes( GPIO_PIN pin );    
    
private:

    static UINT32 PinToBit( GPIO_PIN pin )
    {
        ASSERT(pin < c_MaxPins);

        return pin % c_PinsPerPort;
    }

    static void STUB_GPIOISRVector( GPIO_PIN Pin, BOOL PinState, void* Param );
};

extern PXA271_GPIO_Driver g_PXA271_GPIO_Driver;

//
// GP I/O driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// USART driver
//

struct PXA271_USART_Driver
{
    static const UINT32 c_RefFlagRx = 0x01;
    static const UINT32 c_RefFlagTx = 0x02;

    //--//

    INT8 m_RefFlags[3];

    //--//

    static BOOL Initialize( int comPort, int baudRate, int Parity, int DataBits, int StopBits, int FlowValue );

    static BOOL Uninitialize( int comPort );

    static BOOL TxBufferEmpty( int comPort );

    static BOOL TxShiftRegisterEmpty( int comPort );

    static void WriteCharToTxBuffer( int comPort,  UINT8 c );

    static void TxBufferEmptyInterruptEnable( int comPort, BOOL enable );

    static BOOL TxBufferEmptyInterruptState( int comPort );

    static void RxBufferFullInterruptEnable( int comPort, BOOL enable );

    static BOOL RxBufferFullInterruptState( int comPort );

    static BOOL TxHandshakeEnabledState( int comPort );

    static void ProtectPins( int comPort, BOOL on );

    static void GetPins(int comPort, GPIO_PIN& rxPin, GPIO_PIN& txPin,GPIO_PIN& ctsPin, GPIO_PIN& rtsPin );    

    static void BaudrateBoundary( int ComPortNum, UINT32& maxBaudrateHz, UINT32& minBaudrateHz );

    static BOOL IsBaudrateSupported( int ComPortNum, UINT32 & BaudrateHz );    

private:

    static void USART_ISR( void* Param );

    static void RefFlags( int ComPortNum, INT8 RefFlags, BOOL Add );
};

extern PXA271_USART_Driver g_PXA271_USART_Driver;

//
// USART driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// TIMER driver
//

struct PXA271_TIMER_Driver
{
    const static UINT32 c_MaxTimers          = 12;
    static const UINT32 c_FirstPXA271_Timer  = 4;          // PXA250 compatible timers are 0-3, PXA271 Timers are 4-11

    //--//

    static BOOL   Initialize  ( UINT32 Timer, BOOL FreeRunning, UINT32 ClkSource, UINT32 externalSync, HAL_CALLBACK_FPN ISR, void* ISR_Param );
    static BOOL   Uninitialize( UINT32 Timer                                                                                                 );
    static UINT32 ReadCounter ( UINT32 Timer                                                                                                 );
    
    static void EnableCompareInterrupt( UINT32 Timer )
    {
        ASSERT(Timer < c_MaxTimers);

        PXA271_TIMER& TIMER = PXA271::TIMER();

        TIMER.OIER |= (1 << Timer);
    }

    static void DisableCompareInterrupt( UINT32 Timer )
    {
        ASSERT(Timer < c_MaxTimers);

        PXA271_TIMER& TIMER = PXA271::TIMER();

        TIMER.OIER &= ~(1 << Timer);
    }

    static void ForceInterrupt( UINT32 Timer )
    {
        ASSERT(Timer < c_MaxTimers);
        ASSERT_IRQ_MUST_BE_OFF();       // Should be called only by a routine that has interrupts already turned off

        PXA271_TIMER& TIMER = PXA271::TIMER();

        // Interrupts in general can't be forced for the PXA271, but a timer interrupt can be
        // forced by writing the current counter value to the match register.  This write is
        // performed twice in the code below in case the counter changed between the first
        // read and write.  Note that interrupts are turned off in order to insure that the
        // two writes are performed within one Timer clock period.
        if( Timer < c_FirstPXA271_Timer )     // If one of the PXA250 compatible timers, clock period is about 307 nS
        {
            TIMER.OSMR0[Timer] = TIMER.OSCR0 + 4;
        }
        else        // Likely clocked by 32.768 KHz; a clock period of over 30 uS
        {
            TIMER.OSMR[Timer-c_FirstPXA271_Timer] = TIMER.OSCR[Timer-c_FirstPXA271_Timer] + 4;
        }
    }

    static void SetCompare( UINT32 Timer, UINT32 Compare )
    {
        ASSERT(Timer < c_MaxTimers);

        PXA271_TIMER& TIMER = PXA271::TIMER();

        if( Timer < c_FirstPXA271_Timer )     // If one of the PXA250 compatible timers
            TIMER.OSMR0[Timer] = Compare;
        else
            TIMER.OSMR[Timer-c_FirstPXA271_Timer] = Compare;
    }

    static UINT32 GetCompare( UINT32 Timer )
    {
        ASSERT(Timer < c_MaxTimers);

        PXA271_TIMER& TIMER = PXA271::TIMER();

        if( Timer < c_FirstPXA271_Timer )     // If one of the PXA250 compatible timers
            return TIMER.OSMR0[Timer];
        else
            return TIMER.OSMR[Timer-c_FirstPXA271_Timer];
    }

    static void SetCounter( UINT32 Timer, UINT32 Count )
    {
        ASSERT(Timer < c_MaxTimers);

        PXA271_TIMER& TIMER = PXA271::TIMER();

        if( Timer < c_FirstPXA271_Timer)      // If one of the PXA250 compatible timers
            TIMER.OSCR0 = Count;           // There is only one count register for the first four timers
        else
            TIMER.OSCR[Timer-c_FirstPXA271_Timer] = Count;
    }

    static BOOL DidCompareHit( UINT32 Timer )
    {
        ASSERT(Timer < c_MaxTimers);

        PXA271_TIMER& TIMER = PXA271::TIMER();

        return (TIMER.OSSR & (1 << Timer)) ? true : false;
    }

    static void ResetCompareHit( UINT32 Timer )
    {
        ASSERT(Timer < c_MaxTimers);

        PXA271_TIMER& TIMER = PXA271::TIMER();

        TIMER.OSSR = 1 << Timer;        // Must write a 1 to clear the bit; zeros affect nothing
    }
    
private:
    struct Descriptors
    {
        HAL_CALLBACK_FPN isr;
        void* arg;
        BOOL configured;
    };
    Descriptors m_descriptors[c_MaxTimers];

    static void ISR4_11( void* param );
};

extern PXA271_TIMER_Driver g_PXA271_TIMER_Driver;

//
// TIMER driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// System Time driver
//

struct PXA271_TIME_Driver
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

extern PXA271_TIME_Driver g_PXA271_TIME_Driver;

//
// System Time driver
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// SPI / Microwire driver
//

struct PXA271_SPI_Driver
{
    struct SPI_PORT_INFO
    {
        UINT32 CLK_pin;
        UINT32 TXD_Pin;
        UINT32 RXD_Pin;
        UINT32 ClockIndex; 
        BOOL m_Enabled;    
    };
    
    static struct SPI_PORT_INFO s_All_SPI_port[3];
    
    static BOOL Initialize  ();
    static void Uninitialize();

    static void GetPins( UINT32 spi_mod, GPIO_PIN& msk, GPIO_PIN& miso, GPIO_PIN& mosi );

    static BOOL nWrite16_nRead16( const SPI_CONFIGURATION& Configuration, UINT16* Write16, INT32 WriteCount, UINT16* Read16, INT32 ReadCount, INT32 ReadStartOffset );
    static BOOL nWrite8_nRead8  ( const SPI_CONFIGURATION& Configuration, UINT8*  Write8 , INT32 WriteCount, UINT8*  Read8 , INT32 ReadCount, INT32 ReadStartOffset );

    static BOOL Xaction_Start( const SPI_CONFIGURATION& Configuration );
    static BOOL Xaction_Stop ( const SPI_CONFIGURATION& Configuration );

    static BOOL Xaction_nWrite16_nRead16( SPI_XACTION_16& Transaction );
    static BOOL Xaction_nWrite8_nRead8  ( SPI_XACTION_8&  Transaction );

private:
    static void ISR( void* Param );
};

extern PXA271_SPI_Driver g_PXA271_SPI_Driver;

//
// SPI / Microwire driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// I2C driver
//

struct I2C_HAL_XACTION_UNIT;
struct I2C_HAL_XACTION;
struct I2C_CONFIGURATION;

struct PXA271_I2C_Driver
{
    BOOL m_initialized;

	//--//
	
    I2C_HAL_XACTION*       m_currentXAction;
    I2C_HAL_XACTION_UNIT*  m_currentXActionUnit;

    //--//

    static const UINT8 c_DirectionWrite = 0x00;
    static const UINT8 c_DirectionRead  = 0x01;
    static const UINT8 c_AddressCycle   = 0x01;

    //--//

    static BOOL  Initialize         (                                                      );
    static BOOL  Uninitialize       (                                                      );
    static void  MasterXAction_Start( I2C_HAL_XACTION* xAction, bool repeatedStart = false );
    static void  MasterXAction_Stop (                                                      );
    static UINT8 GetClockRate       ( UINT32 rateKhz                                       );
    static void  GetPins            ( GPIO_PIN& scl, GPIO_PIN& sda                         );

private:
    static BOOL IsBusBusy          (                               );
    static void WriteToSubordinate ( I2C_HAL_XACTION_UNIT* xAction );
    static void ReadFromSubordinate( I2C_HAL_XACTION_UNIT* xAction );
    static void ISR                ( void*            arg          );
};

extern PXA271_I2C_Driver g_PXA271_I2C_Driver;

//
// I2C driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// External Bus Interface Unit
//

struct PXA271_EIM_Driver
{
    static void ConfigMemoryBlock( const CPU_MEMORY_CONFIG& CPUMemoryConfig);

    static BOOL Memory_ReadOnly( const CPU_MEMORY_CONFIG& CPUMemoryConfig, BOOL ReadOnly );

private:
    static void MemoryConfig_To_Control( const CPU_MEMORY_CONFIG& CPUMemoryConfig, UINT32& CS_L, UINT32& CS_U );
};

//
// External Bus Interface Unit
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// USB driver
//

#include <..\pal\com\usb\USB.h>

struct PXA271_USB_Driver
{
    static const UINT32 c_Used_Endpoints           = 7;
    static const UINT32 c_default_ctrl_packet_size = 16;

    USB_CONTROLLER_STATE*   pUsbControllerState;
    
#if defined(USB_REMOTE_WAKEUP)
    #define USB_MAX_REMOTE_WKUP_RETRY 5

    HAL_COMPLETION          RemoteWKUP10msCompletion;
    HAL_COMPLETION          RemoteWKUP100msEOPCompletion;
    UINT32                  RemoteWkUpRetry;
#endif

    UINT8                   ControlPacketBuffer[c_default_ctrl_packet_size];
    UINT16                  EndpointStatus[c_Used_Endpoints];
    BOOL                    TxRunning [USB_MAX_QUEUES];
    BOOL                    TxNeedZLPS[USB_MAX_QUEUES];

    UINT8                   PreviousDeviceState;
    UINT8                   RxExpectedToggle[USB_MAX_QUEUES];
    BOOL                    PinsProtected;
    BOOL                    FirstDescriptorPacket;

#if defined(USB_METRIC_COUNTING)
    USB_PERFORMANCE_METRICS PerfMetrics;
#endif

    //--//

    static USB_CONTROLLER_STATE * GetState( int Controller );

    static HRESULT Initialize( int Controller );

    static HRESULT Uninitialize( int Controller );

    static BOOL StartOutput( USB_CONTROLLER_STATE* State, int endpoint );

    static BOOL RxEnable( USB_CONTROLLER_STATE* State, int endpoint );

    static BOOL GetInterruptState();

    static BOOL ProtectPins( int Controller, BOOL On );

private:
    static void ClearTxQueue( USB_CONTROLLER_STATE* State, int endpoint );

    static void StartHardware();

    static void StopHardware ();

    static void TxPacket( USB_CONTROLLER_STATE* State, int endpoint );

    static void ControlNext();

    static void SuspendEvent();
    static void ResumeEvent ();
    static void ResetEvent  ();

    static void Global_ISR  ( void*  Param );
    static void EP0_ISR     ( UINT32 Param );    
    static void EP_TxISR    ( UINT32 Param );    
    static void EP_RxISR    ( UINT32 Param );    

#if defined(USB_REMOTE_WAKEUP)
    static void RemoteWkUp_ISR    ( void* Param );
    static void RemoteWkUp_Process( void* Param );
#endif
};

extern PXA271_USB_Driver g_PXA271_USB_Driver;


//
// USB DRIVER
///////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// DMA driver
//

struct PXA271_DMA_Driver
{
    static const int c_DummyDMAChannel    = 10;
    static const int c_MemCpy2DChannel    =  9;
    static const int c_MemCpyChannelStart =  8; // We start at high numbers because the higher the channel number the higher the priority
    static const int c_MemCpyChannelEnd   =  0;

    static const UINT32 c_MemCpy2DWaitTimeOut = 100000; // cycles

    //--//

    static BOOL Initialize  ();
    static BOOL Uninitialize();

    static void MemCpy  ( void* dst, void* src, UINT32 size, BOOL async );
    static void MemCpy2D( void* dst, void* src, UINT32 width, UINT32 height, UINT32 dataWidth, BOOL async );

    static void StartDummyDMA();

private:
    static void* TranslateAddress( void* virtualAddress );
};

extern PXA271_DMA_Driver g_PXA271_DMA_Driver;

//
// DMA driver
///////////////////////////////////////////////////////////////////////////////

//
//
// DRIVER SECTION
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// CONFIG SECTION
//
//

///////////////////////////////////////////////////////////////////////////////
// USART SECTION
//

struct PXA271_USART_CONFIG
{
    HAL_DRIVER_CONFIG_HEADER Header;

    //--//

    UINT8         TxProtectInput;
    UINT8         RxProtectInput;
    UINT8         CTSProtectInput;
    UINT8         RTSProtectInput;
    GPIO_RESISTOR TxProtectResistor;
    GPIO_RESISTOR RxProtectResistor;
    GPIO_RESISTOR CTSProtectResistor;
    GPIO_RESISTOR RTSProtectResistor;
    UINT8         TxProtectOutputValue;
    UINT8         RxProtectOutputValue;
    UINT8         CTSProtectOutputValue;
    UINT8         RTSProtectOutputValue;

    //--//

    static LPCSTR GetDriverName() { return "PXA271_USART"; }
};

extern PXA271_USART_CONFIG g_PXA271_USART_Config;
//
// USART SECTION
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// USB SECTION
//

struct PXA271_USB_CONFIG
{
    HAL_DRIVER_CONFIG_HEADER Header;

    //--//

    static LPCSTR GetDriverName() { return "PXA271_USB"; }
};

extern PXA271_USB_CONFIG* const g_pPXA271_USB_Config;

//
// USB SECTION
///////////////////////////////////////////////////////////////////////////////
//
//
// CONFIG SECTION
/////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _PXA271_H_ 1
