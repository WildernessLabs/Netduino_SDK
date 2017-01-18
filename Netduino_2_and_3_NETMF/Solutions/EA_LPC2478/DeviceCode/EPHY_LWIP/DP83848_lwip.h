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

#ifndef _DP83848_LWIP_H_
#define _DP83848_LWIP_H_ 1

/* DP83848C PHY Registers */
#define PHY_REG_BMCR        0x00        /* Basic Mode Control Register       */
#define PHY_REG_BMSR        0x01        /* Basic Mode Status Register        */
#define PHY_REG_IDR1        0x02        /* PHY Identifier 1                  */
#define PHY_REG_IDR2        0x03        /* PHY Identifier 2                  */
#define PHY_REG_STS         0x10        /* Status Register                   */

#define BMCR_RESET			    0x8000
#define BMSR_AUTO_DONE		  0x0020

#define PHY_AUTO_NEG        0x3000      /* Select Auto Negotiation           */

#define STS_LINK_ON         0x1
#define STS_10_MBIT         0x2
#define STS_FULL_DUP        0x4

#define MII_WR_TOUT         0x00050000  /* MII Write timeout count           */
#define MII_RD_TOUT         0x00050000  /* MII Read timeout count            */

#define DP83848C_DEF_ADR    0x0100      /* Default PHY device address        */
#define DP83848C_ID         0x20005C90  /* PHY Identifier                    */

#endif
