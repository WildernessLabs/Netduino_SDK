

/* XNCONF.H  - configuration file

   EBS - RTIP

   Copyright Peter Van Oudenaren , 1993
   All rights reserved.
   This code may not be redistributed in source or linkable object form
   without the consent of its author.

    Module description:
        This module contains constants that may be change to the modify
        the build characteristics of RTIP.

        NOTE: constants which start with _CFG are runtime configurable;
              (see confdata.c)
              constants which start with CFG are NOT runtime configurable
*/

#ifndef __XNCONF__
#define __XNCONF__ 1

/* ********************************************************************   */
#define RTIP_VERSION                        50  /* RTIP version 5.0 */
#define RTPLATFORM				            1

/* *******************************************************************     */
/* BINARY VERSION CONFIGURATION SECTION                                    */
/* *******************************************************************     */
#define BUILD_NEW_BINARY                    1

#if (BUILD_NEW_BINARY)
#define INCLUDE_RUN_TIME_CONFIG             1       /* DO NOT CHANGE */
#define INCLUDE_MALLOC_PORT_LIMIT           0
#define INCLUDE_MALLOC_CONTEXT              1
#else
#define INCLUDE_RUN_TIME_CONFIG             0
#define INCLUDE_MALLOC_PORT_LIMIT           0
#define INCLUDE_MALLOC_CONTEXT              0
#endif

#if (INCLUDE_RUN_TIME_CONFIG)
#define INCLUDE_MALLOC_PORTS                1       /* DO NOT CHANGE  */
#else                                       
#define INCLUDE_MALLOC_PORTS                0
#endif

/* won't get alloced unless INCLUDE_BGET is on and is needed by RTIP for   */
/* (FTP, TNS or WEB etc)                                                   */
#define CFG_CONTEXT_CORE                    2000

/* Set to attach semaphores and signals to interface structure   */
/* instead of in global variables                                */

/* specify whether to dynamically allocate packets at init time or   */
/* to declare them as global arrays where:                           */
/*   1=malloc packets at initialization time                         */
/*   0=declare packets as global array                               */
#if (BUILD_NEW_BINARY || INCLUDE_RUN_TIME_CONFIG)
#define INCLUDE_MALLOC_DCU_INIT             1
#define INCLUDE_MALLOC_DCU_AS_NEEDED        0   /* WARNING: might not beable to turn on */
                                                /* since alloc packet from ISR   */
#else
#define INCLUDE_MALLOC_DCU_INIT             0
#define INCLUDE_MALLOC_DCU_AS_NEEDED        0   /* DO NOT CHANGE */
#endif

/* configuration parameters which are not run-time configurable   */
#define CONFIG_PARAM_DONE                   0     /* DO NOT CHANGE */

/* *******************************************************************       */
/* START OF ADD-ON CONFIGURATION SECTION                                     */
/* *******************************************************************       */
#undef INCLUDE_RTIP
#undef INCLUDE_ERTFS

#define INCLUDE_RTIP                        1
#define INCLUDE_PPP                         0	// 1
#define INCLUDE_PPPOE                       0	// 1
#define INCLUDE_DMH_SNMP                    0
#define INCLUDE_SNMP                        0	// 1
#define INCLUDE_SNMPV2                      0	// 1
#define INCLUDE_SNMPV3                      0
#define INCLUDE_FTP_CLI                     0
#define INCLUDE_FTP_SRV                     0	// 1
#define INCLUDE_TFTP_CLI                    0	// 1
#define INCLUDE_TFTP_SRV                    0	// 1
#define INCLUDE_TELNET_CLI                  0
#define INCLUDE_TELNET_SRV                  0	// 1
#define INCLUDE_DHCP_CLI                    1   // 1
#define INCLUDE_DHCP_SRV                    0	// 1
#define INCLUDE_SMTP                        0	// 1
#define INCLUDE_POP3                        0	// 0
#define INCLUDE_IMAP                        0
#define INCLUDE_WEB                         0
/* Include support for WEB PUSH                            */
/* NOTE: Netscape support WEB PUSH but not all browsers do */
#define INCLUDE_WEB_PUSH                    0
#define INCLUDE_WEB_BROWSER                 0

/* Set to 1 to include NAT (Network Address Translation) where IP address   */
/* is translated                                                            */
/* NOTE: INCLUDE_ROUTER must also be set                                    */
#define INCLUDE_NAT                         0

/* Set 1 to include Simple Network Time Protocol   */
#define INCLUDE_SNTP                        1

/* Dynamic Configuration of the IPV4 link-layer address (aip*.*)   */
#define INCLUDE_AUTOIP                      0

#define INCLUDE_SSL                         0
#define INCLUDE_CRYPT                       0       /* used by SNMP V3 - tbd */

#define INCLUDE_NFS_SRV                     0
#define INCLUDE_NFS_CLI                     0
#define INCLUDE_SMB_SRV                     0
                                            
#define INCLUDE_ORTFS                       0       /* old version of RTFS */
#define INCLUDE_ERTFS                       0
#define INCLUDE_RTFILES                     0
#define INCLUDE_DOS_FS                      0       /* set for dos file system and dortfdosem */
#define INCLUDE_WIN32_FS                    0       /* DO NOT CHANGE */
#define INCLUDE_ERTFS_PRO                   0       /* If INCLUDE_ERTFS_PRO is set INCLUDE_ERTFS should not be set */
#define ERTFS_PRO_NSEMS                     25      /* reduce according to the ERTFS configuration guide */ 
#define ERTFS_PRO_NSIGS                     4       /* reduce according to the ERTFS configuration guide */ 

/* *******************************************************************       */
/* END OF ADD-ON CONFIGURATION SECTION                                       */
/* *******************************************************************       */

#define INCLUDE_ROUTER_DISCOVERY_HOST       0       /* NI */
#define INCLUDE_ROUTER_DISCOVERY_ROUTER     0       /* NI */
#define INCLUDE_ROUTER_DISCOVERY \
    (INCLUDE_ROUTER_DISCOVERY_HOST || INCLUDE_ROUTER_DISCOVERY_ROUTER)

#define INCLUDE_IP_MOBILITY                 0       /* NI */

/* Dynamic Configuration of the IPV4 link-layer address (apip*.*)   */
#define INCLUDE_APIP                        0              

/* *********************************************************************   */
#define INCLUDE_IPV4                        1   /* DO NOT CHANGE */
#define INCLUDE_IPV6                        0   /* NOT SUPPORTED YET */

/* ************************************************************************ */
/* *********************          SSL               *********************** */
/* ************************************************************************ */
/* This is the only supported secure socket layer by EBS as of yet. */
#define INCLUDE_EBS_SSL                     (1 && INCLUDE_SSL)

#endif /* __XNCONF__ */


