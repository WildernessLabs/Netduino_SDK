//-----------------------------------------------------------------------------
// Copyright 2007 NXP Semiconductors Limited,
// 1109 Mckay Drive San Jose CA 95131 USA
// All rights are reserved. Reproduction in whole or in part is prohibited
// without the prior written consent of the copyright owner.
//-----------------------------------------------------------------------------

#ifndef _PLATFORM_LPC22XX_SELECTOR_H_
#define _PLATFORM_LPC22XX_SELECTOR_H_ 1

#if defined(PLATFORM_ARM_LPC22XX)

#define PLATFORM_ARM_DEFINED

#include <tinypal.h>
#include <drivers.h>

#include "DeviceCode\LPC22XX.h"
#include <Cores\arm\Include\cpu.h>

//
//  Constants that span multiple platforms go here
//

#else  // !PLATFORM_ARM_LPC22XX

ERROR - WE SHOULD NOT INCLUDE THIS HEADER IF NOT BUILDING A LPC22XX PLATFORM

#endif // PLATFORM_ARM_LPC22XX


#endif // _PLATFORM_LPC22XX_SELECTOR_H_
