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
#include "dhcpcapi.h"

#ifndef _LPC24XX_EMAC_DRIVER_H_
#define _LPC24XX_EMAC_DRIVER_H_ 1
//**********************************************//


#define LPC24XX_EMAC_DEVICE    FIRST_USER_DEVICE+0 /* LPC23XX/LPC24XX On-chip Ethernet MAC */


//////////////////////////////////////////////////////////////////////////////
// Ethernet driver
// 

struct LPC24XX_EMAC_Driver
{
    int          m_interfaceNumber;
    DHCP_session m_currentDhcpSession;

    //--//
    
    static int  Open   (   void          );
    static BOOL Close  (   void          );
    static BOOL Bind   (   void          );
};

//
// Ethernet driver
//////////////////////////////////////////////////////////////////////////////

#endif
