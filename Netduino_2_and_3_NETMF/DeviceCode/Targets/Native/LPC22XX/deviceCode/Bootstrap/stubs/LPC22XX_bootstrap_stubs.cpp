#include <tinyhal.h>
#include <DeviceCode\LPC22XX.h>


#pragma arm section code = "SectionForBootstrapOperations"

void __section(SectionForBootstrapOperations) BootstrapCode_Clocks()
{
    // Phytec LPC2294 board uses a 10 MHz crystal
    // Set PLL to 60 MHz
    // 10MHz crystal, P = 6, M = 6, cclk = 60MHz
    LPC22XX::SYSCON().PLLCFG  = 0x25;
    LPC22XX::SYSCON().PLLCON  = 0x1;
    LPC22XX::SYSCON().PLLFEED = 0xAA;
    LPC22XX::SYSCON().PLLFEED = 0x55;
   
    // Wait while PLL locks
    while ( (LPC22XX::SYSCON().PLLSTAT & (1<<10)) == 0x0 )
    {
    }
      
    // PLL locked now connect it
    LPC22XX::SYSCON().PLLCON = 0x3;
    LPC22XX::SYSCON().PLLFEED = 0xAA;
    LPC22XX::SYSCON().PLLFEED = 0x55;
}

void __section(SectionForBootstrapOperations) BootstrapCode_Ext_SRAM()
{

// BCFG1 Register Configuration Value:  0x20000C21
// IDCY   :  1  ->  2 idle cycles
// WST1   :  1  ->  4 CCLK cycles
// RBLE   :  1  ->  Memory Bank composed of two 16 bit devices
// WST2   :  1  ->  4 CCLK cycles
// BUSERR :  0  ->  not relevant
// WPERR  :  0  ->  no write protection error
// WP     :  0  ->  bank not write protected
// BM     :  0  ->  no burst rom bank
// MW     :  2  ->  32-bit wide bus
// AT     :  0  ->  always write 0 to this field

    LPC22XX::EMC().BCFG1=0x20000C21;   
}

void __section(SectionForBootstrapOperations) BootstrapCode_Ext_Flash()
{

// BCFG0 Register Configuration Value:  0x20002083

// IDCY   :  1  ->  2 idle cycles
// WST1   :  4  ->  7 CCLK cycles
// RBLE   :  1  ->  Memory Bank composed of two 16 bit devices
// WST2   :  4  ->  7 CCLK cycles
// BUSERR :  0  ->  not relevant
// WPERR  :  0  ->  no write protection error
// WP     :  0  ->  bank not write protected
// BM     :  0  ->  no burst ROM bank
// MW     :  2  ->  32-bit wide bus
// AT     :  0  ->  always write 0 to this field


// ******
// this is longer wait state for the flash transaction that fits most of the phytec boards, for using this setting 
//     LPC22XX::EMC().BCFG0=0x20002481; the Rx Buffer read slower and we have to put larger Rx Buffer ( 512 is too smaller ) that 
// it will hit Rx Buffer overflow and not able to communicate with the tinyclr 2048 rx buffer size is a better choice for that.
// if a shorter wait state, then rx buffer size can be reduce. say   LPC22XX::EMC().BCFG0=0x20001c63; then rx buffer can be 1024


    LPC22XX::EMC().BCFG0=0x20002481;

}

void __section(SectionForBootstrapOperations) BootstrapCode_Pins()
{
//   111 00 1 1 0 00 01 01 0 0 1 00 1 11 10 01 00
    LPC22XX::PCB().PINSEL2=0x0E6149E4;
}

/*
// Copy vector table from External FLASH/RAM to LPC22XX internal RAM
extern UINT32 Load$$ER_IRAM$$Base;
extern UINT32 Image$$ER_IRAM$$Base;
extern UINT32 Image$$ER_IRAM$$Length;

void __section(SectionForBootstrapOperations) PrepareImageRegions_IRAM()
{
    UINT32* src = (UINT32*)&Load$$ER_IRAM$$Base; 
    UINT32* dst = (UINT32*)&Image$$ER_IRAM$$Base;
    UINT32  len = (UINT32 )&Image$$ER_IRAM$$Length; 

    if(dst != src)
    {
        while(len)
        {
            *dst++ = *src++;

            len -= 4;
        }
    }
}

*/


// map the InternalRAM  to address 0
void __section(SectionForBootstrapOperations) Initialize_SyncOn()
{

//    PrepareImageRegions_IRAM();
    LPC22XX::SYSCON().MEMMAP=0x2;
}

#pragma arm section code


