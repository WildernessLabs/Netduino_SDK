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
#include "..\LPC24XX.h"

// these routines depend on which platform is being used
extern void Initialize_SyncOn();
extern void BootstrapCode_Clocks();
extern void BootstrapCode_Pins();
extern void BootstrapCode_Ext_SRAM();
extern void BootstrapCode_Ext_SDRAM();
extern void BootstrapCode_Ext_Flash();



#pragma arm section code = "SectionForBootstrapOperations"

// Dummy function force code generation for the following bootstrap functions
//void __section(SectionForBootstrapOperations) BootstrapCode_Dummy()
//{
//}

extern "C"
{
void __section(SectionForBootstrapOperations) BootstrapCode()
{
    // Clocks, Pins and Memory init done in board specefic bootstrap code
    // See LPC24XX_prestackinit.s
    
    // Implement bootstrap code
    PrepareImageRegions();
}
}

#pragma arm section code


///////////////////////////////////////////////////////////////////////////////
