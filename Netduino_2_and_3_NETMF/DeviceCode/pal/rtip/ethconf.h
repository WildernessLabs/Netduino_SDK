/* ETHCONF.H  - configuration file for device drivers

   EBS - RTIP

   Copyright Peter Van Oudenaren , 1993
   All rights reserved.
   This code may not be redistributed in source or linkable object form
   without the consent of its author.

    Module description:
        This module contains constants that may be change to the modify
        the build characteristics of drivers for RTIP.

    NOTE: this module needs to be included after ostype.h

*/

#ifndef __ETHCONFH__
#define __ETHCONFH__ 1

#include "xnconf.h"

/* YOU MIGHT WANT TO CHANGE ALL OF THE FOLLOWING */

/* ************************************************************************ */
/* *******************       DEVICES         ****************************** */
/* ************************************************************************ */

/* determines PCMCIA support is included in various drivers.  See the
   device table (chapter 4 of the RTIP Manual) to determine which device drivers
   can support PCMCIA */
/* NOTE: if !AT_MOTHERBOARD, INCLUDE_PCMCIA will be turned off in rtip.h */
/* NOTE: ERTFS might already have turned INCLUDE_PCMCIA on */
#if (RTKERNEL32 && !BUILD_NEW_BINARY)
#define INCLUDE_PCMCIA              1
#elif (RTKERNEL32 || WIN32RTIP || POLLWIN32 || !AT_MOTHERBOARD)
#define INCLUDE_PCMCIA              0
#define INCLUDE_RTKERNEL_PCMCIA     0
#else
#define INCLUDE_PCMCIA      		1
#define INCLUDE_RTKERNEL_PCMCIA     0
#endif

#define INCLUDE_WINPCAP             0    /* WINPCAP driver -  */
/* ************************************************************************    */
/* ******************* DEVICE TIMERS         ******************************    */
/* ************************************************************************    */

/* number of seconds to wait for transmit of ethernet or loopback
   output packet to complete   */
#define CFG_ETHER_XMIT_TIMER 4

/* ************************************************************************    */
/* *******************  DEVICE PARAMETER     ******************************    */
/* ************************************************************************    */
// the following configuration values are only used if no bios exists to
// set up cfg space

// [tbd] MOVE to i82559 pci driver

//#define CFG_82559_PCI_IRQ		0x0a 	 // Default IRQ if not in cfg space
//#define CFG_82559_PCI_IOBASE	0x1000   // Default io base if not in cfg space
//#define CFG_82559_PCI_MEMBASE   0x04000000   /* Default base if not in cfg space */

/* ************************************************************************    */
/* configuration values for R8139 */

// [tbd] MOVE to r8139 driver

//#define CFG_R8139_RX_BUFLEN_IDX	0	/* 0==8K, 1==16K, 2==32K, 3==64K */
//// TBD - should this be a configuration parameter
//#define CFG_R8139_RX_BUFLEN (8192 << CFG_R8139_RX_BUFLEN_IDX)
//
//// the following configuration values are only used if no bios exists to
//// set up cfg space
//#define CFG_R8139_PCI_IRQ       0x0a     /* Default IRQ if not in cfg space */
//#define CFG_R8139_PCI_IOBASE    0x1000   /* Default io base if not in cfg space */

/* ************************************************************************    */
// the following configuration values are only used if no bios exists to
// set up cfg space

// [tbd] MOVE to tc90x driver

//#define CFG_TC90X_PCI_IRQ       0x0a     /* Default IRQ if not in cfg space */
//#define CFG_TC90X_PCI_IOBASE    0x1000   /* Default io base if not in cfg space */
//#define CFG_TC90X_PCI_MEMBASE   0x04000000   /* Default base if not in cfg space */

/* I/O address base and irq for DP83902 on Hitachi SH SolutionEngine board */

// [tbd] MOVE to dp83902 driver

//#if (defined(RTIP_MS7709ASE01))
//#define CFG_ED_SE7709A_IRQ      13
//#define CFG_ED_SE7709A_IOBASE   0xb0000000
//#endif  /* RTIP_MS7709ASE01 */
//#if (defined(RTIP_MS7727SE01))
//#define CFG_ED_SE7709A_IRQ      13
//#define CFG_ED_SE7709A_IOBASE   0xb0000000
//#endif  /* RTIP_MS7727SE01 */
//#if (defined(RTIP_MS7750SE01))
//#define CFG_ED_SE7709A_IRQ      13
//#define CFG_ED_SE7709A_IOBASE   0xb0000000
//#endif  /* RTIP_MS7750SE01 */

/* ************************************************************************    */
/* if LANCE is 32 bit, set to 1
   if LANCE is 16 bit, set to 0 */
// NOTE: INCLUDE_RTLANCE must be set
// NOTE: if protected mode, LANCE can be run in either 32 bit or 16 bit mode
//       if real mode, LANCE must be 16 bit

// [tbd] MOVE to rtlance driver

//#define CFG_AMD_32BIT 1
//
///* if LANCE is PCI-based (Am79C972), CFG_AMD_PCI must be set */
//#if (TARGET_186ES)
//#define CFG_AMD_PCI 0       /* DO NOT CHANGE */
//#elif (TARGET_SC520)
//#define CFG_AMD_PCI 1
//#else
//#define CFG_AMD_PCI 1
//#endif
//
//#define CFG_LANCE_BUS_MASTER 1
//#define CFG_LANCE_SHARED_MEM 0
//#if (CFG_LANCE_SHARED_MEM)
//#	if (IS_MS_PM || IS_BCC_PM || IS_HC_PM)	// protected mode
//#		define CFG_LANCE_SH_MEM_ADDR  (RTP_PFUINT8) 0xD0000
//#	elif (defined(__BORLANDC__) || defined(_MSC_VER) )
//#		define CFG_LANCE_SH_MEM_ADDR (RTP_PFUINT8) 0xD0000000
//#	else
//#		error - Define an address for the shared memory area of the lance card
//#	endif
//#endif
//
///* Use these values for the AM79C972 if probes of the IOBASE and IRQ
//   Registers show that the System BIOS is not assigning these resources
//   to the device.
//   Values set below are used instead of values from the device table
//   or the global configuration values */
//#define CFG_AMD_PCI_IOBASE 	0x1000		/* 1st valid PCI address	*/
//#define CFG_AMD_PCI_IRQ    	0x0A		/* IRQ available to system	*/
//#define CFG_AMD_PCI_MEMBASE 0x04000000  /* Default base if not in cfg space */
//
///* The rtlance driver allows for the "manual" setting of the PHY speed
//     (as opposed to auto-negotiation) when the ethernet interface is an
//     Am79C972.  One of the following must be written into rt_set_phy_speed
//     (defined in rtipdata.c) prior to opening the device. */
//#define PHY_10BT_HD		0
//#define PHY_10BT_FD		1
//#define PHY_100BT_HD	2
//#define PHY_100BT_FD	3
//#define PHY_AUTO		4
//#define CFG_DEFAULT_PHY_SPEED PHY_AUTO
//
///* The rtlance driver allows the Am79C972 to be placed in loopback mode.
//     NO_LOOP indicates that loopback mode is not enabled.
//     EXTERNAL_LOOP indicates that loopback is in the PHY.
//     INTERNAL_LOOP indicates that loopback is in the MII.
//     GPSI loopback is not implemented.	*/
//#define NO_LOOP			0
//#define EXTERNAL_LOOP	1
//#define INTERNAL_LOOP	2
//#define CFG_DEFAULT_LB_MODE NO_LOOP

/* ************************************************************************    */
// [tbd] MOVE to n83815 driver
//#define CFG_N83815_PCI_IRQ      5
//#define CFG_N83815_PCI_IOBASE	0x300

/* ************************************************************************    */
// [tbd] MOVE to prism driver
//#define CFG_PRISM_PCI_IRQ       0x0a

/* ************************************************************************    */
// [tbd] MOVE to davicom driver
//#define CFG_DAVICOM_PCI_IRQ 	5

/* ************************************************************************    */
/* Set to one to hardwire the NE2000 to Board address and IRQ rather
   then extracting from global parms or device table (Saves space on
   little systems) */
// [tbd] MOVE to ne2000 driver
//#define CFG_NE2000_HW    0
//
///* Use these values in hardwired mode (i.e. for the NE2000 driver
//   (INCLUDE_NE2000 is 1 and CFG_NE2000_HW is 1), the configuration
//   values set below are used instead of values from the device table
//   or the global configuration values */
//#define CFG_NE2000_IOBASE 0x300
//#define CFG_NE2000_IRQ    0x05
//
///* If the following is set to 1 a probe for the device is done. Set to
//   0 for a product with ne2000/83902 embedded */
//#if (M5206EC3 || M5407C3) 
//#define CFG_NE2000_PROBE 0  /* we know this board has NE2000 or compatible */
//#else
//#define CFG_NE2000_PROBE 1
//#endif
//
///* Set this if we know it is an  NE1000   */
//#define CFG_FORCE_NE1000 0

/* ************************************************************************    */
/* SBC5307 definitions */
// [tbd] MOVE to sbc5307 driver
//#define CFG_SBC5307_HW       1
//#define CFG_SBC5307_IOBASE   0xFE600300
//#define CFG_SBC5307_IRQ      0x03
//#if (SMX && LH7A400_EVB)
//#define CFG_LAN8900_IRQ      41
//#else
//#define CFG_LAN8900_IRQ      05
//#endif

/* ************************************************************************    */
/* mode to run packet driver n(see INCLUDE_PKT) where:
      4 = accept packets to this board, multicast and broadcast
      3 = accept packets to this board and broadcast
   NOTE: 4 did not work for 3c509 packet driver (3 did work)
*/
#define CFG_PKT_MODE 4

/* ************************************************************************    */
/* *******************          PCI          ******************************    */
/* ************************************************************************    */
#if (defined(SMX) && SMX_VERSION >= 0x353)
/* SMX >= 3.5.3 has its own PCI functions. Macros in pci.h map rtpci_ to them. */
#define INCLUDE_PCI 0
#elif (INCLUDE_RTLANCE && CFG_AMD_PCI)
#define INCLUDE_PCI 1
#elif (INCLUDE_I82559 || INCLUDE_R8139 || INCLUDE_TC90X || INCLUDE_PRISM_PCI || INCLUDE_DAVICOM)
#define INCLUDE_PCI 1
#else
#define INCLUDE_PCI 0
#endif

/* ************************************************************************    */
/* *******************  PACKET ADJUSTMENT    ******************************    */
/* ************************************************************************    */
/* Ethernet data frame size - size of data area in a DCU must have
   extra space for certain drivers/platforms to have room for CRC
   etc etc */
#if (POWERPC || ECOS_POWERPC || INCLUDE_DAVICOM)
#define CFG_PACKET_ADJ 22		/* Must be set for the PPC860T */
#elif (INCLUDE_R8139 || EBSETHFEC)
#define CFG_PACKET_ADJ 22		/* For Motorola FEC controller */
#elif (INCLUDE_I82559)
#define CFG_PACKET_ADJ 16
// INCLUDE_DAVICOM used to be adjusted by 4 
#elif (INCLUDE_TC90X || INCLUDE_RTLANCE || INCLUDE_LANCE || INCLUDE_LANCE_ISA || INCLUDE_DE4X5 || defined(MC68360))
#define CFG_PACKET_ADJ 4
#else
#define CFG_PACKET_ADJ 0
#endif

#if (RTKERNEL32 || RTTARGET)
// for RTKERNEL PM calls malloc routines which are aligned on 16-bit
// boundaries (see PhysMalloc in osmalloc.c) are used
#if (INCLUDE_MALLOC_DCU_INIT)
//#undef CFG_PACKET_ADJ
//#define CFG_PACKET_ADJ 0
#endif
#endif

#if (INCLUDE_SNMP || INCLUDE_DMH_SNMP)
/* ************************************************************************    */
/* **********************      SNMP      **********************************    */
/* ************************************************************************    */
/* speed of the various devices (used by devices table)
   1000000=10BT 10000000=100BT
   Units: bits per second */
#define CFG_SPEED_NE2000    1000000l
#define CFG_SPEED_SMC8X     1000000l
#define CFG_SPEED_EL3       1000000l
#define CFG_SPEED_PKT       1000000l
#define CFG_SPEED_SMC9X     10000000l
#define CFG_SPEED_DE4X5     1000000l
#define CFG_SPEED_EL9       1000000l
#define CFG_SPEED_EL89      1000000l
#define CFG_SPEED_LANCE     10000000l
#define CFG_SPEED_LANCE_ISA 1000000l
#define CFG_SPEED_XIRCOM    1000000l
#define CFG_SPEED_CS89X0    1000000l
#define CFG_SPEED_EEXP      1000000l
#define CFG_SPEED_EEPRO     1000000l
#define CFG_SPEED_I82559    1000000l
#define CFG_SPEED_ETH360    1000000l
#define CFG_SPEED_N83815    1000000l
#define CFG_SPEED_R8139     10000000l
#define CFG_SPEED_TC90X     10000000l
#define CFG_SPEED_DAVICOM   1000000l
#define CFG_SPEED_DM9000    10000000l
#define CFG_SPEED_ETH860    10000000l
#define CFG_SPEED_ETH5272   1000000l 
#define CFG_SPEED_ETH5282   1000000l 
#define CFG_SPEED_SBC5307   1000000l
#define CFG_SPEED_PRISM     1000000l
#define CFG_SPEED_TCFE574   1000000l
#define CFG_SPEED_MTOK      1000000l
#define CFG_SPEED_LOOP      10000000l
#define CFG_SPEED_WINETHER  1000000l
#define CFG_SPEED_WINPCAP  1000000l

/* OID of the various devices (used by devices table)   */
#define CFG_OID_NE2000  	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_SMC8X   	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_EL3     	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_PKT     	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_SMC9X   	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_DE4X5   	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_EL9     	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_EL89    	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_LANCE   	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_XIRCOM  	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_CS89X0  	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_EEXP    	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_EEPRO   	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_I82559  	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_N83815  	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_R8139   	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_TC90X   	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_DAVICOM 	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_DM9000	 	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_ETH360  	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_ETH860  	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_ETH5272     {{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_ETH5282     {{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_SBC5307 	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_PRISM   	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_TCFE574 {{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_MTOK    	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_LOOP    	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_WINETHER	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_WINPCAP	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_SLIP    	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_CSLIP   	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#define CFG_OID_PPP     	{{MIB_OID, 0, 0}, MIB_OID_SIZ + 2}
#endif 	// INCLUDE_SNMP

/* ************************************************************************ */
/* *********************       TOKEN RING           *********************** */
/* ************************************************************************ */

/* number of MADGE token ring adapters supported */
#define CFG_NUM_MTOKEN   1

/* Use these values for the Madge Token ring adapter if probes of the IOBASE
   and IRQ Registers show that the System BIOS is not assigning these
   resources to the device.
   Values set below are used instead of values from the device table
   or the global configuration values */
/* The presto2000 device requires 0x80 bytes of I/O space */
#define CFG_MTOK_PCI_IOBASE 0x1000	/* 1st valid PCI address	*/
#define CFG_MTOK_PCI_IRQ    0x05		/* IRQ available to system	*/

// maximum RIF size for output
#define CFG_MAX_RIF 18				// 2 for header and 16 for 8 routes

#endif		// ETHCONFH


