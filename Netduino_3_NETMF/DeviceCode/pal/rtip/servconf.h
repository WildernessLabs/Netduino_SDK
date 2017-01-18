

/* SERVCONF.H  - configuration file

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

#ifndef __SERVCONF__
#define __SERVCONF__ 1

#include "xnconf.h"

#if (INCLUDE_FTP_CLI || INCLUDE_FTP_SERV)
#include "ftpconf.h"
#endif

#if (!INCLUDE_MALLOC_CONTEXT)
/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                      */
/* maximum number of tasks to spawn (should be set to max of   */
/* CFG_WEB_MAX_SPAWN, CFG_TELNET_MAX_SPAWN, CFG_FTP_MAX_SPAWN) */
#define CFG_MAX_CONTEXT 32
#endif

/* ************************************************************************       */
/* ******************       GENERIC SERVER CONFIG       *******************       */
/* ************************************************************************       */
/* Number of second to wait for server read select
   NOTE:  currently only used while waiting for SSL accept
          to be started after returning form standard accept */
#define _CFG_SERV_READ_TMO       10

/* *******************************************************************      */
/* Number of normal (small stacks) that we manage. ie stacks of size
   SIZESTACK_NORMAL. (osport.h). This is only needed if the kernel being
   used requires stacks to be provide by the user (KS_DECLARE_STACK = 1)
   in osport.h. There are two of these provided in case we are running
   in a segmnented architecture and the total stack size does not fit
   in one segment */
/* need 1 for timer, 2 per interface, main, SNMP, DHCP, FTP, TELNET etc   */
/* NOTE: maximum number of stacks for FTP and TELNET would be
         CFG_SPAWN_FTP * CFG_FTP_MAX_SPAWN +
         CFG_SPAWN_TELNET * CFG_TELNET_MAX_SPAWN
         but to cut down on the number of stacks it is defined as * 2 */

#define MAX_STACKS 0    /* declare max possible NORMAL and BIG stacks */

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
#define CFG_N_STACKS_NORMAL_1   6

#if (!MAX_STACKS)
/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
#define CFG_N_STACKS_NORMAL_2   (2*_CFG_NIFACES+                        \
                                 DHCP_STACK +                           \
                                 CFG_SPAWN_FTP * 2 +                    \
                                 CFG_SPAWN_TELNET * 2)
#else
/* maximum possible needed   */
#define CFG_N_STACKS_NORMAL_2   (2*_CFG_NIFACES+                         \
                                 DHCP_STACK +                           \
                                 CFG_SPAWN_FTP * CFG_FTP_MAX_SPAWN +    \
                                 CFG_SPAWN_TELNET * CFG_TELNET_MAX_SPAWN)
#endif

/* Number of big stacks that we manage. ie stacks of size
   SIZESTACK_BIG. (osport.h). This is only needed if the kernel being
   used requires stacks to be provide by the user (KS_DECLARE_STACK = 1)
   in osport.h.
*/
/* NOTE: maximum number of stacks needed for WEB is
   CFG_SPAWN_WEB*CFG_WEB_MAX_SPAWN
   but we cut down on the number of stacks it is defined as * 2 */
#if (!MAX_STACKS)
/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                   */
/* less than maximum possible needed; i.e. uses less memory */
#define CFG_N_STACKS_BIG         (1 + CFG_SPAWN_WEB * 1 +                   \
                                  SNMP_STACK +                              \
                                  NFS_STACK)
#else
/* maximum possible needed   */
#define CFG_N_STACKS_BIG         (1 + CFG_SPAWN_WEB * CFG_WEB_MAX_SPAWN +   \
                                  SNMP_STACK +                              \
                                  NFS_STACK + 2)
#endif

#define CFG_N_STACKS_HUGE         0

#if (!INCLUDE_RTIP && INCLUDE_ERTFS)
#define CFG_N_STACKS_NORMAL_1    6
#define CFG_N_STACKS_NORMAL_2    1
#define CFG_N_STACKS_BIG         1
#define CFG_N_STACKS_HUGE        0
#endif  /* !INCLUDE_RTIP && INCLUDE_ERTFS */

#if (INCLUDE_RTIP || INCLUDE_ERTFS)
/* Derive the total number of stacks     */
#define CFG_N_STACKS_TOTAL (CFG_N_STACKS_NORMAL_1 +  \
                            CFG_N_STACKS_NORMAL_2 +  \
                            CFG_N_STACKS_BIG +       \
                            CFG_N_STACKS_HUGE)
#endif      /* INCLUDE_RTIP || INCLUDE_ERTFS */

#endif /* __SERVCONF__ */

