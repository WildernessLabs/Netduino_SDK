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

#ifndef _LPC24XX_PROCESSOR_SELECTOR_H_
#define _LPC24XX_PROCESSOR_SELECTOR_H_ 1

#if !defined(PLATFORM_ARM_LPC24XX)
ERROR - WE SHOULD NOT INCLUDE THIS HEADER IF NOT BUILDING A LPC24xx PLATFORM
#endif


#define PLATFORM_ARM_DEFINED


#include <tinypal.h>
#include <drivers.h>

#include "DeviceCode\LPC24XX.h"
//#include <Cores\Arm\Diagnostics\instrumentation_types.h>

#endif // _LPC24XX_PROCESSOR_SELECTOR_H_
