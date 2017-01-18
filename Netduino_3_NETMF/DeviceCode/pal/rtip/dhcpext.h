/*                                                                        */
/* AUTOIPEXT.H  - RTIP header file                                        */
/*                                                                        */
/*   EBS - RTIP                                                           */
/*                                                                        */
/*   Copyright EBSNet Inc. , 1993                                 */
/*   All rights reserved.                                                 */
/*   This code may not be redistributed in source or linkable object form */
/*   without the consent of its author.                                   */
/*                                                                        */
/*    Module description:                                                 */
/*        This file contains prototypes constants and structure           */
/*        declarations required to build AUTOIP.                          */
/*                                                                        */

#ifndef __DHCPIPEXT__
#define __DHCPIPEXT__ 1

#include "rtip.h"
#include "sock.h"

#if (INCLUDE_DHCP_CLI)

/* ********************************************************************   */
/* TYPES                                                                  */
typedef void (*ARP_CALLBACK_DHCP_FNC)(PIFACE pi, PARPPKT pa);
#define ARP_DHCP_CALLBACK(a, b)          \
    if (arp_callback_dhcp_fnc)           \
        arp_callback_dhcp_fnc(a, b)

/* ********************************************************************   */
/* GLOBAL DATA                                                            */

#ifdef __cplusplus
extern "C" {
#endif

extern ARP_CALLBACK_DHCP_FNC arp_callback_dhcp_fnc;

#ifdef __cplusplus
}
#endif

#endif      // INCLUDE_DHCP_CLI

#if (INCLUDE_DHCP_SRV)
// ********************************************************************
// DHCP SERVER
// ********************************************************************
extern EBS_TIMER    dhcps_timer_info;

#if (POLLOS)
// handle to task to run by poll_os_cycle
extern POLLOS_TASK pollos_dhcps_handle;
extern struct dhcps_context *ctx_pollos;
extern struct dhcps_config *cfg_pollos;
#endif
extern struct dhcps_context *dhcps_ctx;

#endif  // INCLUDE_DHCP_SRV


#endif      /* __DHCPIPEXT__ */

