

/* DHCPCONF.H  - configuration file

   EBS - RTIP

   Copyright EBSNet Inc. , 1993
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

#ifndef __DHCPCONF__
#define __DHCPCONF__ 1

/* ************************************************************************    */
/* *********************        DHCP CLIENT         ***********************    */
/* ************************************************************************    */

// Include code so DHCP client will request a new IP address if
// lease expires and renews and rebinds fail.  The routine
// cb_dhcp_no_ip will be called if a new IP address is obtained
// or this constant is set to 0 when the lease expires.  It is RFC complient
// to keep this at 1.
#define INCLUDE_DHCP_NEW_LEASE 1

// include code so timeout routine will not block when renewing lease
#define INCLUDE_DHCP_RENEW_NO_BLOCK 1

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING */
/* maximum number of DHCPOFFER messages the client will receive   */
#define _CFG_DHCP_OFFERS  1

/* number of times the client will re-broadcast the DHCPDISOVER message;
   the message is re-broadcast when it receives no responses */
#define _CFG_DHCP_RETRIES 5

/* number of seconds the client will wait for a server response   */
#define _CFG_DHCP_TIMEOUT 8

// Set to 1 to support option to request TFTP IP address.  Cisco DNS
// servers have this option
#define INCLUDE_TFTP_CISCO 1


/* ************************************************************************    */
/* *********************        DHCP SERVER         ***********************    */
/* ************************************************************************    */

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING */
/* Maximum number of "address bindings" in the table,    */
/* dhcps_config.address_pool.                            */
#define _CFG_DHCPS_MAX_BINDINGS        8

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING */
/* Maximum # of IP addresses that are "spanned" in the ranges given in the   */
/* address pool -- this is used for the address state table,                 */
/* dhcps_context.address_states;                                             */
#define _CFG_DHCPS_MAX_ADDRESSES     255

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING */
/* The maximum number of IP addresses that have been OFFERED but not ACKED;   */
/* This specifies the size of dhcps_context.sales_pending.                    */
#define _CFG_DHCPS_MAX_SALES_PENDING   5

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING */
/* The maximum number of IP addresses that been ACKED;     */
/* This specifies the size of dhcps_context.active_leases. */
#define _CFG_DHCPS_MAX_ACTIVE_LEASES  10

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING */
/* Maximum number of DNS servers to configure client with.  This   */
/* specifies the size of dhcps_params.dns_servers.                 */
#define _CFG_DHCPS_MAX_DNS_SERVERS     2

/* Number of seconds for server to keep lease longer than client
   lease time; this is used in case server and clients timers
   are not in sync */
#define _CFG_DHCPS_LEASE_TIME_DIFF    10

/* Number of DHCP IP address requests in case the IP address is already */
/* in use by another host                                               */
#define DHCP_NUM_RETRY              5

/* A window of time to receive an ARP from a maching with the same IP   
   address of our IP address.  If nobody replies within the specified   
   time (_CFG_DHCP_CONFIRM_TIMEOUT), we'll assume nobody has the IP     
   address.  This value is in msecs                                     */
#define DHCP_CONFIRM_TIMEOUT        5000
    
/* This value controls how often to check to see if the ARP reply 
    came back.  This value is in msecs                                  */
#define DHCP_CHECK_FLAG_SLEEP       250
#endif /* __DHCPCONF__ */
