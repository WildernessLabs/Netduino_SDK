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
#include "net_decl_lwip.h"
#include "lwip\dhcp.h"

#ifndef _LPC24XX_EMAC_LWIP_DRIVER_H_
#define _LPC24XX_EMAC_LWIP_DRIVER_H_ 1
//**********************************************//


#define LPC24XX_EMAC_DEVICE    FIRST_USER_DEVICE+0 /* LPC23XX/LPC24XX On-chip Ethernet MAC */


//////////////////////////////////////////////////////////////////////////////
// Ethernet driver
// 

struct LPC24XX_EMAC_LWIP_Driver
{
    static int  Open   (   int           );
    static BOOL Close  (   void          );
    static BOOL Bind   (   void          );
};

//
// Ethernet driver
//////////////////////////////////////////////////////////////////////////////

#endif
