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

#include <tinyhal.h>
#include "..\LPC22XX.h"

// this routines depends on which platform are using.
extern void BootstrapCode_Clocks();
extern void BootstrapCode_Ext_SRAM();
extern void BootstrapCode_Ext_Flash();
extern void BootstrapCode_Pins();
extern void Initialize_SyncOn();


// Copy vector table from External FLASH/RAM to LPC22XX internal RAM
extern UINT32 Load$$ER_IRAM$$Base;
extern UINT32 Image$$ER_IRAM$$Base;
extern UINT32 Image$$ER_IRAM$$Length;


#pragma arm section code = "SectionForBootstrapOperations"

// Dummy function force code generation for the following bootstrap functions
void __section(SectionForBootstrapOperations) BootstrapCode_Dummy()
{
}

// since Image$$ER_IRAM$$Length is a constant at compile time, that it ignores the "len==0" part, so we 
// have to take care the "len" at the scatterfile, otherwise code will be broken.
void __section(SectionForBootstrapOperations) PrepareImageRegions_IRAM()
{
    UINT32* src = (UINT32*)&Load$$ER_IRAM$$Base; 
    UINT32* dst = (UINT32*)&Image$$ER_IRAM$$Base;

    UINT32  len = (UINT32 )&Image$$ER_IRAM$$Length;     

    if(dst != src)
    {
        while (len)
        {
            *dst++ = *src++;

            len -= 4;
        }
    }
}


extern "C"
{
void __section(SectionForBootstrapOperations) BootstrapCode()
{
//    BootstrapCode_Clocks();
//    BootstrapCode_Pins();
//    BootstrapCode_Ext_SRAM();
//    BootstrapCode_Ext_Flash();
    
    // Implement bootstrap code
    PrepareImageRegions();
    PrepareImageRegions_IRAM(); // extra image copy for Internal RAM
    // Remap Interrupt vectors to internal RAM
    // Vector table is copied by PrepareImageRegions() (HAL\ARM\Configuration\tinyhal.cpp)
//    Initialize_SyncOn();
    
}
}

#pragma arm section code


///////////////////////////////////////////////////////////////////////////////
