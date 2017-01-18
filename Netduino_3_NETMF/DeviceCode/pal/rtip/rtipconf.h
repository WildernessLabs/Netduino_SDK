

/* RTIPCONF.H  - configuration file

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

#ifndef __RTIPCONF__
#define __RTIPCONF__ 1

#define INCLUDE_MULTICAST                   1

/* number of multicast addresses a socket can listen to   */
#define CFG_SOCK_MAX_MEMBERSHIP             5

/* ************************************************************************       */
/* *******************     LARGE/SMALL CONFIGURATION     ******************       */
/* ************************************************************************       */

/* Set this define to 1 to turn on all the options.               */
/* Set this define to 0 for a small configuration where           */
/* certain features such as RARP, BOOTP, MODEM, IGMP, RAW socket, */
/* all device drivers except NE2000 etc. are turned off           */
#if (BUILD_NEW_BINARY)
#define LARGE_CONFIG                        1
#else                                       
#define LARGE_CONFIG                        0
#endif

/* Set to 0 to get really small configuration; no UDP, no DNS   */
#if (LARGE_CONFIG)
#define SMALL_CONFIG                        0     /* DO NOT CHANGE */
#else                                       
#define SMALL_CONFIG                        1
#endif

/* ************************************************************************       */
/* *******************        INCLUDE Protocols      **********************       */
/* ************************************************************************       */

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                                      */
/* Set to 1 if you will use tcp. If you don't (udp/raw only) set to zero       */
/* to save code space and resources                                            */
#define INCLUDE_TCP                         1

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                                      */
/* Set to 1 if you will use udp. If you don't (tcp/raw only) set to zero       */
/* to save code space and resources                                            */
#define INCLUDE_UDP                         1

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                               */
/* Set to 1 if you will use raw sockets. If you don't set to zero       */
/* to save code space and resources;
   NOTE: if this is set, INCLUDE_UDP must also be set */
#define INCLUDE_RAW                         1

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                                        */
/* Set to 1 if you will include calls to xn_ping(). If not set this to zero      */
/* to save code space and resources                                              */
#define INCLUDE_PING                        1

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Set to 1 if you do need ICMP support (except ping which is controlled
   by INCLUDE_PING. If not set this to zero to save code space and resources */
#define INCLUDE_ICMP                        1

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Set to 1 if you do need IGMP support (host side only, i.e. RTIP
   is not a multicast router
   NOTE: set to 0 if INCLUDE_IGMP_V2 is set to 1 */
#define INCLUDE_IGMP_V1                     1

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Set to 1 if you need IGMP Version 2 support (host side only)
   NOTE: if set to 1, set INCLUDE_IGMP to 0 */
#define INCLUDE_IGMP_V2                     LARGE_CONFIG

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Set to 1 if you need IGMP Version 3 support (host side only)
   NOTE: if set to 1, set INCLUDE_IGMP and INCLUDE_IGMP_V2 to 0 */
#define INCLUDE_IGMP_V3                     0       /* NI */

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                                     */
/* Set to 1 to include a rarp server 0 otherwise. The rarp server is in       */
/* arp.c It must be generalized                                               */
#define INCLUDE_RARP_SRV                    LARGE_CONFIG

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                                   */
/* Set to 1 if you will call xn_rarp(). Set to zero to save code space      */
#define INCLUDE_RARP                        LARGE_CONFIG

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                                    */
/* Set to 1 if you will call xn_bootp(). Set to zero to save code space      */
#define INCLUDE_BOOTP                       LARGE_CONFIG

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                              */
/* Set to 1 to include support for 802.2, i.e. LLC and SNAP headers    */
/* Support for 802.2 entails processing of 802.2 input packets and if
   the socket option (see setsockopt) or interface option
   (see xn_interfacea_opt) is set, sending 802 */
#define INCLUDE_802_2                       0

/* Set to 1 to include support for Token Ring   */
#define INCLUDE_TOKEN                       0

/* Set to 1 to include support for callback due to events needed   */
/* for WINSOCK implementation                                      */
#define INCLUDE_POSTMESSAGE                 0


/* ************************************************************************             */
/* *********************         MISC               ***********************             */
/* ************************************************************************             */
/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                                               */
/* Set to 1 if you will need fragmentation support. Set to zero to save code space      */
#define INCLUDE_FRAG                        1

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Set to 1 if you need select (and xn_tcp_is_connect, xn_tcp_is_read or
   xn_tcp_is_write) */
#define INCLUDE_SELECT                      1

/* Set to 1 to gather statistics about interfaces and ports     */
/* Statistics are stored in the interface structure and are useful for
   debugging applications. This may be turned of for a production system if
   the statistics won't be used. */
/* NOTE: the statistics are used by SNMP also, therefore, if SNMP is on
         do not turn this off */
#if (INCLUDE_SNMP || INCLUDE_DMH_SNMP || INCLUDE_WEB)
#define INCLUDE_KEEP_STATS                  1
#else
#define INCLUDE_KEEP_STATS                  0
#endif

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Set to 1 to include DNS functions        */
#if (!INCLUDE_UDP)
#define INCLUDE_DNS                         0       /* DO NOT CHANGE */
#else
#define INCLUDE_DNS                         1
#endif

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                         */
/* Set to 1 to include DNS cache, i.e. cache results of lookup    */
#define INCLUDE_DNS_CACHE                   1

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Set to 1 to include database functions, i.e. getprotobynumber(),
   getprotobyname(), getservbyname() and getservbyport() */
#define INCLUDE_DB                          LARGE_CONFIG

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                      */
/* Set to 1 to include packet API, i.e. xn_pkt_send() etc      */
#define INCLUDE_PKT_API                     1   /* LARGE_CONFIG  */

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Set to 1 to enable ARP code; ARP is only needed for ethernet
   interfaces;
   NOTE: if using SNMP, INCLUDE_ARP must be set */
/* NOTE: if this is set, INCLUDE_ETH_BUT_NOARP must be off   */
#define INCLUDE_ARP                         1

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                           */
/* Set to 1 to include an IP to ETHERNET translation table; this is */
/* used for ETHERNET packet where the translation is known by the   */
/* application, therefore, it is not necessary to ARP; the table    */
/* must be set up by calls to xn_arp_add()                          */
/* NOTE: if this is set, INCLUDE_ARP must be off                    */
/* NOTE: if set RTIP will still respond to ARP requests             */
#define INCLUDE_ETH_BUT_NOARP               0

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Set to 1 to enable routing table code; routing table code allows
   determination of which interface a packet is sent out on based upon
   the IP address sending to; This should only be turned of if there
   is only one interface, i.e. CFG_NIFACES is 1;
   NOTE: if using SNMP, INCLUDE_ROUTING_TABLE must be set */
#define INCLUDE_ROUTING_TABLE               1

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Set to 1 to enable a time-to-live timer in routing table entries; if
   this is set, the ttl value is set by a parameter in xn_rt_add.  If
   this is set to 0, routing table entries do not expire and are only
   removed by calls to xn_rt_del */
#define INCLUDE_RT_TTL                      1

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Set to 1 to enable code which locks a routing table entry while a
   TCP socket which uses the entry is connected. Turning this off save
   a small amount of code but if a routing table entry is deleted which
   is being used by a TCP connection, sends will fail */
#define INCLUDE_RT_LOCK                     1

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Set to 1 to build static routes when default gateway is chosen,
   NOTE: this enables ICMP redirects to effect sending to a better gateway */
#define INCLUDE_BUILD_STATIC_ROUTES         LARGE_CONFIG

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Set to 1 to enable routing of packets, i.e. forwarding packets not destined
   for our IP address; the routing table is used to determine where to
   forward the packet
   NOTE: INCLUDE_ROUTING_TABLE must also be set to 1
   NOTE: RTIP provides a very simple router, we are not a "router" */
#define INCLUDE_ROUTER                      0    /* SPR */

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Set to 1 to include code to act as a PROXY ARP; this is valid only
   if INCLUDE_ROUTER is set to 1 */
#define INCLUDE_PROXY_ARP                   0

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                    */
/* Set these to enable RIP, version 1 or versions 1 and 2    */
#define INCLUDE_RIPV1                       0
#define INCLUDE_RIPV2                       0
#define INCLUDE_RIP_LISTEN_ONLY             0

/* ************************************************************************    */
/* **************** ROUTER DISCOVERY (AGENT/ROUTER) ***********************    */
/* ************************************************************************    */
/* The IP destination address to be used for multicast                         */
/* Router Advertisements sent from the interface.  The                         */
/* only permissible values are the all-systems multicast                       */
/* address, 224.0.0.1, or the limited-broadcast address,                       */
/* 255.255.255.255.  (The all-systems address is preferred                     */
/* wherever possible, i.e., on any link where all                              */
/* listening hosts support IP multicast.)                                      */
/*                                                                             */
/* Default: 224.0.0.1 if the router supports IP multicast                      */
/* on the interface, else 255.255.255.255                                      */
#define CFG_RD_AD_ADDRESS                   {224, 0, 0, 1}    /* {255, 255, 255, 255} */

/* The maximum time allowed between sending multicast      */
/* Router Advertisements from the interface, in seconds.   */
/* Must be no less than 4 seconds and no greater than 1800 */
/* seconds.                                                */
/* Units: seconds                                          */
#define CFG_RD_MAX_ADD_INTERVAL             600

/* The minimum time allowed between sending unsolicited    */
/* multicast Router Advertisements from the interface, in  */
/* seconds.  Must be no less than 3 seconds and no greater */
/* than CFG_RD_MAX_ADD_INTERVAL.                           */
/*                                                         */
/* Default: 0.75 * CFG_RD_MAX_ADD_INTERVAL                 */
/* Units: seconds                                          */
#define CFG_RD_MIN_ADD_INTERVAL             450

/* The value to be placed in the Lifetime field of Router   */
/* Advertisements sent from the interface, in seconds.      */
/* Must be no less than CFG_RD_MAX_ADD_INTERVAL and no      */
/* greater than 9000 seconds.                               */
/*                                                          */
/* Default: 3 * CFG_RD_MAX_ADD_INTERVAL                     */
/* Units: seconds                                           */
#define CFG_RD_ADD_LIFETIME                 1800


/* ************************************************************************    */
/* *********************      ERRNO/API             ***********************    */
/* ************************************************************************    */

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Set to 1 to enable strings associated with the API calls (used for debugging
   purposes).  The strings can be used for debugging purposes to track
   calls to the API layer, i.e. every api routine calls the macro
   RTIP_API_ENTER passing a unique parameter.  The macro which is defined in
   rtip.h calls the diagnostic routing RTP_DEBUG_LOG to print the name of the
   api.  (See the Users/Porting Manual for more details on RTP_DEBUG_LOG).
   The strings are defined in rtipcons.c */
#define INCLUDE_API_STR                     0

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Set to 1 to include various error checking code such as parameter checking
   and if system has been initialized.  This should be set to 1 for
   development but can be set to 0 to save space for production */
#define INCLUDE_ERROR_CHECKING              1

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Set to 1 to enable the routine geterror_string() which converts an
   errno value to a string.  A large array of data strings describing
   the errno values will be included if this is set.  The strings are
   define in rtipcons.c */
#define INCLUDE_ERRNO_STR                   1

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* set to non-zero if you want NON-thread-safe unknown errno's to be printed as
   numbers within the xn_geterror_string() returned string.
   NOTE that, though the returned string is NOT threaqd-safe, is constructed in such
   a way that you'll always hit a sentinel (NUL-RTP_UINT8) at the end of the valid buffer.
   I.e. your software will NOT crash; only receive 'patched', i.e. incorrect unknown
   errno numbers when multiple tasks access xn_geterror_string() with unknown errno's
   at the same time.
 */
#define INCLUDE_ERROR_STRING_NUMBER         1

/* ************************************************************************    */
/* *********************     TCP ALGORITHMS         ***********************    */
/* ************************************************************************    */

/* when send is in blocking mode, send will wait until all bytes are
   acked (versus waiting until all are queued) */
#define _CFG_TCP_SEND_WAIT_ACK              1

/* Set to 1 to enable FAST RETRANSMIT in TCP; fast retransmit will
   retransmit packets if too many duplicate acks are received */
#define INCLUDE_FAST_RETRANS                1

/* Set to 1 to enable Congestion avoidance and SLOW START in TCP   */
#define INCLUDE_CWND_SLOW                   1

/* Set to 1 to enable round-trip time calculations for TCP; if this
   is not calculated CFG_MINRTO is used to determine intervals
   to retransmit data */
#define INCLUDE_TCP_RTT                     1

/* Set to 1 to enable code to do out of order processing, i.e. if set
   out of order packets are keep and processed when they are no longer
   out of order; if turned off, out of order packets are dropped */
#define INCLUDE_TCP_OUT_OF_ORDER            1

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                      */
/* Set to 1 to include code to support NO COPY mode for TCP    */
#define INCLUDE_TCP_NO_COPY                 1

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                   */
/* Set to 1 to include code to support COPY mode for TCP    */
#define INCLUDE_TCP_COPY                    1

/* Set to 1 to include code to perform fast processing of TCP input
   packets; this code will perform delayed ack process but none of
   the other TCP algorithms such as congestion fast retransmit,
   out of order processing, etc.  This code not handle packets for
   sockets in COPY mode or packets which have RESET, SYNC or FIN bits set */
#define INCLUDE_FAST_TCP                    0

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                                   */
/* Use set sockopt(SO_LINGER=off, linger value >0) to set maximum amount of */
/* time to wait until remote closes                                         */
#define INCLUDE_TCP_REMOTE_CLOSE            1

/* ************************************************************************    */
/* *********************       OPTIONS              ***********************    */
/* ************************************************************************    */
/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                                      */
/* Set to 1 to include support for IP options                                  */
#define INCLUDE_IP_OPTIONS                  1

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                  */
/* Set to 1 to include code to support timestamp option    */
#define INCLUDE_TCP_TIMESTAMP               1

/* ************************************************************************    */
/* *********************     PERFORMANCE            ***********************    */
/* ************************************************************************    */

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Set to 1 to include code in UDP that will speed up matching incoming
   packets with ports. It will speed up only if the port number of the socket
   in the previous packet is the same as the port number in the current packet
   and there is only one udp socket bound to that port number */
#define INCLUDE_UDP_PORT_CACHE              LARGE_CONFIG

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Set to 1 to include code in UDP that will speed up transmission by
   cacheing routing table information in udp sockets.
   Recommended setting 1 */
#define INCLUDE_UDP_IFACE_CACHE             LARGE_CONFIG

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* This configuration flag is meaningfull only if INCLUDE_UDP_IFACE_CACHE
   is 1. If INCLUDE_SYNCHRONIZE_UDP_IFACE_CACHE is 1 then code is included to
   make sure that if a routing table entry changes then all routes cached
   at the socket level will be cleared. This feature may be disabled to
   save code space if you are sure that routes will not dynamically change
   Recommended setting 1 */
#define INCLUDE_SYNCHRONIZE_UDP_IFACE_CACHE LARGE_CONFIG

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Set to 1 to include code that will speed up transmission by
   cacheing arp table information in tcp and udp sockets.
   Recommended setting 1 */
#define INCLUDE_SOCK_ARP_CACHE              LARGE_CONFIG

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Set this value to 1 if you are using UDP_PORT_CACHE and you can guarantee
   that your application will never bind the same socket number to more than
   one udp port. If this value is set to one some code is excluded but
   udp port cacheing is unreliable if more than one socket has the same
   port number.
   The recommended value is zero unless you are in a deeply embedded system
   and are very clear on the usage of this flag */
#define GUARANTEE_UNIQUE_UDP_PORT_NUMBERS   0

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Set to 1 to include code in TCP that will speed up matching incoming
   packets with ports. This is a straight forewared algortithm and should
   be left on */
#define INCLUDE_TCP_PORT_CACHE              LARGE_CONFIG

/* ************************************************************************    */
/* *********************     DIAGNOSTICS            ***********************    */
/* ************************************************************************    */

/* Set to one to enable PERFORMANCE measuring features. This should normally
   be set to zero */
#define INCLUDE_MEASURE_PERFORMANCE         0

/* Set to 1 to include execution profiling code   */
/* Note: Only currently works for NET186 board    */
#define INCLUDE_PROFILING                   0

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Set to 1 to include debug code to track DCUs; if this is set the
   routine display_packet_info() is available in os.c which will
   scan all RTIP internal exchanges, lists, TCP windows etc and
   to gather statistics; this routine will call RTP_DEBUG_ERROR to prints
   the results;
   if DEBUG_TRACK in os.c is set to 1 and INCLUDE_TRK_PKTS is set,
   the routine display_packet_info() will be called if call to allocate
   a packet fails;
   display_packet_info() calls RTP_DEBUG_ERROR() to display the summary */
/* NOTE: check related values in os.c, i.e. TRACK_WAIT, DEBUG_TRACK   */
#if (BUILD_NEW_BINARY)
#define INCLUDE_TRK_PKTS                    0
#else                                       
#define INCLUDE_TRK_PKTS                    0
#endif

/* call routine to track packets if cannot             */
/* allocate a packet; this will display a report       */
/* (via RTP_DEBUG_ERROR) showing what exchanges, lists */
/* etc the DCUs are on                                 */
/* NOTE: INCLUDE_TRK_PKTS in rtipconf.h must also      */
/*       be on                                         */
#define _CFG_DISPLAY_IF_NDCUS               1

/* *********************************************************************   */
/* ******        GUARD PACKET MACROS                               *****   */
/* *********************************************************************   */

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                                      */
/* set to 1 to include code to guard the end of PACKET which is connected to   */
/* DCU; a fixed pattern will be stored at the end of the packet and each time  */
/* a DCU is freed the guard will be checked to ensure it has not been changed; */
/* if turned on set a break point at break_guard()                             */
/* which will be called if the guard is corrupted                              */
#define INCLUDE_GUARD_PACKET                0

/* ********************************************************************   */
/* MALLOC                                                                 */
/* ********************************************************************   */

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                  */
/* track calls to bget and brel check for invalid frees    */
#define INCLUDE_DEBUG_MALLOC                0

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING          */
/* check for overwritting end of dynamic memory    */
#define INCLUDE_GUARD_MALLOC                0

/* number of malloc's to track   */
#if (INCLUDE_SSL)
#define CFG_DEBUG_MALLOC_SIZE               8192
#else                                       
#define CFG_DEBUG_MALLOC_SIZE               4096
#endif

/* ************************************************************************    */
/* *********************      SECURE SOCKETS: SSL      ********************    */
/* ************************************************************************    */

/* set to a suitable 'number of bytes' to reserve for SSL related run-time
   storage allocation. Don't worry about this one if INCLUDE_BGET is NOT set.
 */
#define CFG_SSL_BGET_CORE_SIZE              (256L * 1024L)

/* Set to 1 to include code to test any cryptographic routines      */
/* Set in ssl environment makefiles                                 */
#if (!defined(INCLUDE_TEST_CRYPT))
#define INCLUDE_TEST_CRYPT                  0
#endif

/* ************************************************************************       */
/* ***************** SLIP/CSLIP and MODEM *********************************       */
/* ************************************************************************       */

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING */
/* Set to 1 to include SLIP   */
#define INCLUDE_SLIP                        0	// LARGE_CONFIG

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING */
/* Set to 1 to include CSLIP   */
#define INCLUDE_CSLIP                       0 // LARGE_CONFIG

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* modem support included                   */
#if (INCLUDE_PPP || INCLUDE_SLIP || INCLUDE_CSLIP)
#define INCLUDE_MODEM                       LARGE_CONFIG
#else                                       
#define INCLUDE_MODEM                       0     /* DO NOT CHANGE */
#endif

/* ************************************************************************       */
/* *******************  NUMBER DEVICES   **********************************       */
/* ************************************************************************       */
/* number of comm ports supported (supports COMM1-COMM4)                          */
#define CFG_COMM_NUM                        4

/* number of rs232 lines supported     */
#define CFG_NUM_RS232                       1

/* Size of the interem buffer used when PPP and SLIP prepare for transmit     */
#define CFG_RS232_XMBUFSIZE                 256

/* number of PPP connections supported - must be less than or equal to
   CFG_NUM_RS232 */
#define CFG_NUM_PPP                         1

/* ************************************************************************          */
/* ******************* DEVICE TIMERS         ******************************          */
/* ************************************************************************          */

/* number of seconds to wait for transmit of SLIP or PPP output packet
   to complete   */
#define CFG_RS232_XMIT_TIMER 6

/* ************************************************************************          */
/* ******************        TOD                        *******************          */
/* ************************************************************************          */

/* Interval in which time of day should be updated.  This is used if      */
/* KS_SUPPORTS_TIME is 0                                                  */
/* Units: seconds                                                         */
#define _CFG_TOD_FREQ 2

/* ************************************************************************          */
/* ******************* VANJ COMPRESSION (CSLIP/PPP) ***********************          */
/* ************************************************************************          */

/* Set to 1 to include Van Jacobson Compression for PPP     */
#if (INCLUDE_PPP)
#define INCLUDE_PPP_VANJC                   1
#else                                       
#define INCLUDE_PPP_VANJC                   0        /* DO NOT CHANGE */
#endif

#define INCLUDE_CSLIP_XMIT                  1        /* compress output packets - CSLIP only */
                                            
#define _CFG_MAX_VJC_SLOTS                  16       /* maximum number of slots (0-255 where
                                                        0 means no compression */
#define _CFG_CSLIP_SLOTS                    8       /* number of slots - CSLIP only */

/* ************************************************************************       */
/* ****************************       DCUS     ****************************       */
/* ************************************************************************       */

/* NOTE: Memory packet pool is not used if INCLUDE_MALLOC_DCU_AS_NEEDED is 1   */
/*       in which case DCUs are allocated dynamically                          */

/* set to block at API layer if low on DCUs   */

#if (BUILD_NEW_BINARY || INCLUDE_MALLOC_DCU_AS_NEEDED)
#define INCLUDE_NO_DCU_BLOCK                0  /* DO NOT CHANGE: not supported */
#else                                       
#define INCLUDE_NO_DCU_BLOCK                0
#endif

/* threshold for blocking waiting for DCUs   */
/* used if INCLUDE_NO_DCU_BLOCK is turned on */
#define CFG_DCU_BLOCK                       2   /* block if hit this threshold */
#define CFG_DCU_RESUME                      5   /* resume (signal) if hit this threshold */

/* Ethernet data frame size - size of data area in a DCU
   NOTE: DCUs are also used for DNS buffers, therefore, the buffer within
         the DCU should never be less than CFG_DNS_NAME_LEN or 260 bytes
   NOTE: the actual size of the packets are CFG_PACKETSIZEx+CFG_PACKET_ADJ
         which is defined in ethconf.h */

#if (INCLUDE_TOKEN)
#define _CFG_MAX_PACKETSIZE                 2096 	// mtu=2044+14(mtok)+18(rif)+8(LLC,SNAP)+
									                //     4(trailer)+8(alignment)
#else
#define _CFG_MAX_PACKETSIZE                 1514
#endif


/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                                             */
/* Memory packet pool configuration.                                                  */
/*                                                                                    */
/* Network packets vary in size. Many small control packets are sent and              */
/* received such as ARP requests, TCP acknowledgement packets and small               */
/* application specific packets. These packets are typically less then                */
/* 128 bytes in length. Medium sized packets are sent and received                    */
/* frequently as well. Examples of applications that use these packets                */
/* are web page downloads, mail downloads and FTP control socket traffic.             */
/* These packets often carry a payload of 512 bytes plus framing overhead.            */
/* Then there are large packets that consume the maximum link layer frame             */
/* size. These large packets are often used during bulk transfer operations           */
/* such as FTP.                                                                       */
/*                                                                                    */
/* To handle multiple packet sizes RTIP uses multiple packet pools containing         */
/* packets of different sizes. We default to four pools. One for small                */
/* packets <= 128 bytes,  one for medium packets <= 256, one for medium               */
/* packets <= 596 bytes and one for large packets <= 1518 bytes.                      */
/*                                                                                    */
/* When a packet allocation is requested we allocate it from the pool that            */
/* contains the smallest packet that will fit our requirement. If that pool           */
/* is full we look to the larger pools until we find space.                           */
/*                                                                                    */
/* We provide a template for allocating six packet pools of the various               */
/* sizes. Note: we only utilize four of the pools but having six pools                */
/* available allows you to add finer granularity memory allocation without            */
/* having to resort to modifying source code.                                         */
/*                                                                                    */
/* To configure the packet buffers you must modify several defines                    */
/* in rtipconf.h There are six pairs of defines that define the layout.               */
/* Each pair defines the number of packets and the packet size of the                 */
/* pool where the pool is zero to five.                                               */
/*                                                                                    */
/* #define CFG_NUM_PACKETSv YY                                                        */
/* #define CFG_PACKET_SIZEv XX                                                        */
/*                                                                                    */
/* Where v is the pool number (0 to 5), YY is the number of packets and               */
/* XX is the packet size.                                                             */
/*                                                                                    */
/* Notes:                                                                             */
/* 1. (YY * XX) may not exceed 64K. If you wish to allocate more than                 */
/*    64 K of data to a specific packet size then add another pool containing         */
/*    packets of the same size.  THIS RESTRICTION IS FOR REAL MODE ONLY               */
/* 2. The packet sizes must be specified in ascending order. Packets will             */
/*    be allocated from the first pool that contains packets >= the required          */
/*    size so in order to allocate the smallest packet possible the pools must        */
/*    be declared in ascending order.                                                 */
/* 3. The code expects there will be packets of size CFG_MAX_PACKETSIZE.              */
/*    therefore, if you want to cut down on the size of the packets                   */
/*    do not just modify CFG_PACKET_SIZEx but also modify CFG_MAX_PACKETSIZE          */
/* 4. In our sample we only use four of the possible six pools. If you add            */
/*    entries then you must insert them into the list in ascending order and          */
/*    will have to move our assignments up. For example if you add a pool of          */
/*    20 packates of size 1200 bytes you must move the 1518 RTP_UINT8 pool            */
/*    configuration up and put the 1200 RTP_UINT8 configuration in its place.         */
/* 5. Packets should never be smaller than ETHER_MIN_LEN if you are                   */
/*    using an ethernet driver (see tc_interface_send in iface.c)                     */
/*                                                                                    */
/* Eg:                                                                                */
/* Change the configuration block from:                                               */
/*                                                                                    */
/* #define CFG_NUM_PACKETS3  10                                                       */
/* #define CFG_PACKET_SIZE3  1518                                                     */
/*                                                                                    */
/* #define CFG_NUM_PACKETS4  0                                                        */
/* #define CFG_PACKET_SIZE4  0                                                        */
/*                                                                                    */
/* To:                                                                                */
/*                                                                                    */
/* #define CFG_NUM_PACKETS3  20                                                       */
/* #define CFG_PACKET_SIZE3  1200                                                     */
/*                                                                                    */
/* #define CFG_NUM_PACKETS4  10                                                       */
/* #define CFG_PACKET_SIZE4  1518                                                     */
/*                                                                                    */
/*                                                                                    */
/*                                                                                    */
/*                                                                                    */


#define CFG_NUM_FREELISTS  6     /* Set for up to six free lists. Do not
                                   reduce. If you increase it you must add
                                   declarations to rtipdata.c and you must
                                   modify the subroutine os_init_packet_pools()
                                   in os.c */

#define _CFG_NUM_PACKETS0  10    /* allocate room for 10 128 RTP_UINT8 packets */
#define _CFG_PACKET_SIZE0  128   /* This utilizes 1280 bytes */

#define _CFG_NUM_PACKETS1  20    /* allocate room for  50 128 RTP_UINT8 packets */
#define _CFG_PACKET_SIZE1  256   /* This utilizes 2560 bytes */

#define _CFG_NUM_PACKETS2  10    /* Allocate room for  20 596 RTP_UINT8 packets */
#define _CFG_PACKET_SIZE2  512   /* This utilizes 11920 bytes */

#define _CFG_NUM_PACKETS3  20    /* Allocate room for 20 RTP_UINT8 packets */
#define _CFG_PACKET_SIZE3  _CFG_MAX_PACKETSIZE

#if (INCLUDE_MTOKEN)
#define _CFG_NUM_PACKETS4  25    /*  Allocate room for 15 RTP_UINT8 packets */
#else
#define _CFG_NUM_PACKETS4  15    /*  Allocate room for 15 RTP_UINT8 packets */
#endif
#define _CFG_PACKET_SIZE4  _CFG_MAX_PACKETSIZE

#if (INCLUDE_MTOKEN)
#define _CFG_NUM_PACKETS5  20    /*  Place holder. Assign segment size and */
#define _CFG_PACKET_SIZE5  _CFG_MAX_PACKETSIZE
#else
#define _CFG_NUM_PACKETS5  0     /*  Place holder. Assign segment size and */
#define _CFG_PACKET_SIZE5  0     /*  packet size to create more segments */
#endif

/* Lowwater mark where allocation of packets for input will fail.
   This allows sending of packets to succeed even if input packets are
   being blasted in faster than they can be handled.  This number should
   be less than CFG_DCUS. */
#define _CFG_PKT_LOWWATER                   2

/* Lowwater mark for packets where all the DCUs in the TCP out-of-order
   lists will be freed.  The packets will be freed by the timer task.
   This number should be less than CFG_DCUS but greater that
   CFG_PKT_LOWWATER. */
#define _CFG_PKT_LOWWATER_OOO               5

/* Lowwater mark where source quench messages will be sent for input packets -
   this should be less than CFG_DCUS and greater than CFG_PKT_LOWWATER;
   INCLUDE_ICMP must also be set */
#define _CFG_PKT_QUENCH                     10

/* ************************************************************************       */
/* *******************       POOLS and TABLE SIZES     ********************       */
/* ************************************************************************       */

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Number of ports (sockets) available to UDP (minimize to save space);
   UDP sockets and resources are preallocated. Each UDPPORT uses
   approximately 58 bytes, One exchange, one semaphore and
   one message. We pre-allocate 4 structures. It should be set to at
   least one. UDP ports are allocated from far memory. */
#define _CFG_NUDPPORTS                      64

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Number of ports (sockets) available to TCP (minimize to save space)
   Ignored if INCLUDE_TCP is zero;
   TCP sockets and resources are preallocated and are in far memory */
#define _CFG_NTCPPORTS                      64

/* Set to number of concurent calls to select can be made for a socket   */
#define CFG_NUM_SELECT_P_SOCK               5

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Size of the ARP cache per interface.  The arp cache  is used
   for address resolution (IP to ethernet address) by hosts on the net.
   Each entry has an exchange consisting of all packets waiting for the
   same address to be resolved */
/* NOTE: used if INCLUDE_ARP is set   */
#define _CFG_ARPCLEN                        5

/* Retry timer for Arp requests. A timeout value of ARP_TTL_INF (0xfffffffful)   */
/* never expires                                                                 */
/* Units: Seconds                                                                */
/* The default value is 1 second.                                                */
#define _CFG_ARPC_REQ_TIMEOUT               2   

/* Number of times to retry ARP request   */
/* The default value is 4 times           */
#define _CFG_ARPC_MAX_RETRIES               4  

/* Number of seconds Arp Resolutions are stale   */
/* This value might be changed by DHCP           */
/* Units: seconds                                */
/* Default: 10 minutes i.e. 600 secs             */
#define _CFG_ARPC_RES_TIMEOUT               600 

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                               */
/* Number of entries IP to ethernet translation table;                  */
/* used for ethernet packets where translation is known by application, */
/* therefore, ARP is not necessary                                      */
/* NOTE: used if INCLUDE_ETH_BUT_NOARP is set                           */
#define _CFG_NUM_IP2ETH_ADDR                5


/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Number of interfaces allowed to be open at one time. An interface
   structure (see IFACE in rtip.h) is allocated for each open device).
   Each interface requires two TASKs plus CFG_ARPCLEN exchanges for
   queueing packets for output once an address is resolved.
   Each interface uses 82 plus 16 * (CFG_ARPCLEN).  If INCLUDE_KEEP_STATS is
   one an additional approx 160 bytes is used for statistics.
   Interface structures are in far memory */
#if (BUILD_NEW_BINARY && RTKERNEL32)
#define _CFG_NIFACES                        2
#elif (!INCLUDE_ROUTING_TABLE)
#define _CFG_NIFACES                        2
#else
#define _CFG_NIFACES 	                    2
#endif

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                                  */
/* number of entries in device table; if run-time configuration is turned  */
/* on, total_devices can be modified before calling xn_device_table_add to */
/* modify the size of the device table                                     */
#define _CFG_NUM_DEVICES                    4

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* maximum number of multicast addresses which each interface can be
   listening for;
   NOTE: if INCLUDE_ICMP is 1, an extra entry is needed for the
         ALL HOSTS GROUP which is automatically added */
#define _CFG_MCLISTSIZE                     5

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Size of routing table                    */
#define _CFG_RTSIZE                         10

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING   */
/* Size of fragment table                   */
#define _CFG_FRAG_TABLE_SIZE                6

/* Time-to-Live for fragment table entries (in seconds)     */
#define _CFG_IP_FRAG_TTL                    100

/* IP time to live - this value is used unless otherwise specified;
   useful to limit lifetime of segments and to terminate routing loops;
   only gateways will discard packets when TTL is exceeded;
   Note: the value used in output packets can be changed by SNMP or DHPC
         client */
/* Units: seconds although each gateway reduces it by one     */
#define _CFG_IP_TTL                         60

/* Maximum fragment size (in bytes) will process for all protocols
   except ping; size includes protocol header;
   larger packets will be dropped */
#if (INCLUDE_NFS)
#define _CFG_MAX_FRAG                       8192     /* Max data transfer size for NFS  */
#else                                       
#define _CFG_MAX_FRAG                       6000
#endif

/* Maximum fragment size (in bytes) will process for ICMP packets;
   size includes protocol header; larger packets will be dropped */
#define _CFG_MAX_FRAG_ICMP                  2000

/* ************************************************************************       */
/* ***************** MTU/WINDOW SIZE **************************************       */
/* ************************************************************************       */

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                           */
/* MTU/MSS/WINDOW SIZE defines used in device table entries         */
/* NOTE: window size allows 4 max packets per window;               */
/*       you may want to change this to increase performance        */
/*       but if you increase it too much you                        */
/*       might need more DCU (see CFG_NUM_PACKETSx and CFG_NDCUS in */
/*       rtipconf.h)                                                */
/* NOTE: the maximum window size allowed if 0xffff                  */

/* NOTE: these values will be set in the device table for ETHERNET        */
/*       and RS232 devices when the device is opened; however, connecting */
/*       via PPP or using DHCP may change the values                      */
/*       The default for PPP is 1500, so if it is not negotiated it       */
/*       will be changed to 1500; in order to set negotiation values for  */
/*       PPP use xn_lcp_want_mru()                                        */

#define CFG_ETHER_MAX_MSS                   1460
#define CFG_ETHER_MAX_MTU                   1500
#define CFG_ETHER_MAX_WIN_IN                (CFG_ETHER_MAX_MSS*8)
#define CFG_ETHER_MAX_WIN_OUT               (CFG_ETHER_MAX_MSS*8)
/*#define ETHER_MAX_WIN_IN  (0x37ff)   */
/*#define ETHER_MAX_WIN_OUT (0x37ff)   */

#if (0)		// test purposes
#define CFG_RS232_MAX_MSS                   216         	// was 216 but max value seems better
#define CFG_RS232_MAX_MTU                   256         	// was 256 but max value seems better
#define CFG_RS232_MAX_WIN_IN                (CFG_RS232_MAX_MSS*4)
#define CFG_RS232_MAX_WIN_OUT               (CFG_RS232_MAX_MSS*4)
#else                                       
#define CFG_RS232_MAX_MSS                   CFG_ETHER_MAX_MSS    	// was 216 but max value seems better
#define CFG_RS232_MAX_MTU                   CFG_ETHER_MAX_MTU    	// was 256 but max value seems better
#define CFG_RS232_MAX_WIN_IN                (CFG_RS232_MAX_MSS*4)
#define CFG_RS232_MAX_WIN_OUT               (CFG_RS232_MAX_MSS*4)
#endif

#if (INCLUDE_TOKEN)
#define TOK_MAX_MTU                         (2044+LLC_SNAP_HLEN_BYTES)
#define TOK_MAX_MSS                         (TOK_MAX_MTU-TCP_HLEN_BYTES)
#define TOK_MAX_WIN_IN                      (TOK_MAX_MSS*4)
#define TOK_MAX_WIN_OUT                     (TOK_MAX_MSS*4)
#endif

/*   *********************************************************************       */
/*   ******               TCP and UDP port numbers            ************       */
/*   *********************************************************************       */

/* mimimum and maximum range of port numbers assigned to TCP and UDP          */
/* sockets (assigned when socket is called)                                   */
/* NOTE: calling bind will overrule the unique random port number assigned    */
/*       by socket                                                            */
/* NOTE: reserved ports:              1-1023                                  */
/*       ports assigned by socket: 1024-5000                                  */
/*       ports for bind:           5001-0xffff                                */
/* NOTE: bind does not check range (it only checks if address is in use)      */
/*       but using the range 5001-0xffff ensures no conflict with reserved    */
/*       ports or ports assigned by socket                                    */
/* NOTE: NFS assigns UDP ports in the range RESV_PORT_FIRST-RESV_PORT_LAST    */
/*       (600-1023)                                                           */
#define _CFG_UDP_PORT_MIN                   1024
#define _CFG_UDP_PORT_MAX                   5000
#define _CFG_TCP_PORT_MIN                   1024
#define _CFG_TCP_PORT_MAX                   5000

/*   *********************************************************************       */
/*   ******           TCP Compile Time Tuning Constants       ************       */
/*   ******           (don't change lightly)                  ************       */
/*   *********************************************************************       */

/* Send a keep alive packet if socket is quiescent for > CFG_KA_INTERVAL seconds      */
/* Units: seconds                                                                     */
#define _CFG_KA_INTERVAL                    7200

/* Retransmit keep alive packet if still have not heard from other side      */
/* after CFG_KA_RETRY seconds                                                */
/* Units: seconds                                                            */
#define _CFG_KA_RETRY                       75

/* Close the connection if do not hear from other side for CFG_KA_TMO
   seconds even though sent keepalive packets.
   Note: CFG_KA_TMO starts counting only after CFG_KA_INTERVAL seconds
   have expired if keepalive packets are sent, i.e. CFG_KA_TMO/CFG_KA_RETRY
   keepalive packets will have been sent before. */
/* Units: seconds     */
#define _CFG_KA_TMO                         750

/* Interval between updating timers - must be less time than the timeout
   and delayed ack processing; the smaller this number the more acurate
   the delayed ack and timeout processing will be; this number will be
   converted to ticks and rounded up. */
/* Units: msec     */
/* JRT - changed from 110 to 20 to be in sync with timer completion task */
#define _CFG_TIMER_FREQ                     20
#if (defined(CMX_PORT))
#undef CFG_TIMER_FREQ
#define CFG_TIMER_FREQ                      CMX_CFG_TIMER_FREQ
#endif

/* Interval to perform TCP timeout processing            */
/* Units: msec                                           */
/* Range: RTP_UINT16                                     */
#define _CFG_TMO_PROC                       500

/* Interval to send delayed TCP ACKS            */
/* Units: msec                                  */
/* Range: RTP_UINT16                            */
#define _CFG_MAX_DELAY_ACK                  200

/* As packets are acked a round trip time estimate is calculated. Packets
   are resent if not acked within this time. The estimated round trip is
   constantly adjusted. These two values put limits on the estimate. */
/* Units: msec                             */
/* Range: RTP_UINT16 (see port->rto)       */
#define _CFG_MAXRTO                         60000l
#define _CFG_MINRTO                         6000l

/* Amount of time TCP will retransmit packets before giving up -
   default is 3 minutes */
/* Units: msec             */
/* Range: RTP_UINT32       */
#define _CFG_RETRANS_TMO                    180000l

/* Amount of time TCP will retransmit packets before reporting error to
   application - default is 3*CFG_MINRTO */
/* Units: msec             */
/* Range: RTP_UINT16       */
#define _CFG_REPORT_TMO                     18000l

/* The minimum interval to send a window probe      */
/* Units: sec                                       */
#define _CFG_WIN_PROBE_MIN                  5

/* The maximum interval to send a window probe      */
/* Units: sec                                       */
#define _CFG_WIN_PROBE_MAX                  60

/* Waits this long between completing a close handshake and releasing the
   socket (ticks), i.e. after we initiate a close and the other side
   has responded with FIN|ACK, the socket will be released after
   CFG_TWAITTIME seconds */
/* Units: seconds            */
/* Range: RTP_UINT16         */
#define _CFG_TWAITTIME                      120

/* How long a CLOSE connection non initiater should wait for ACK of the final
   FIN it sent in close before closing (aborting) the socket, i.e.
   after we have received a FIN (close) request from the other side we
   send FIN|ACK. The other side should respond by ACKING the message. When
   he does we release the socket.  If he does respond within LASTTIME
   ticks we close the socket. The default value is set to 4000 (approx. 3.75)
   minutes. */
/* Units: seconds            */
/* Range: RTP_UINT16         */
#define _CFG_LASTTIME                       220

/* Maximum number of outstanding connection requests allowed for a port;
   used to limit the backlogsize parameter of xn_listen */
#define _CFG_TCP_MAX_CONN                   10

/* maximum segment size to use if input SYNC message does not contain a
   MSS option; RFC 1122 says to use 536 */
#define _CFG_NO_MSS_VAL                     536     /* equivalent to MTU of 576 */

/* maximum segment size to limit sends to if need to send over a gateway -
   RFC 1122 says to use 536 */
#define _CFG_MSS_GATEWAY                    536   /* equivalent to MTU of 576 */

/* total number of DCUs which may be queued on all the TCP out-of-order
   lists; valid only if INCLUDE_OOO_QUE is set */
#define INCLUDE_OOO_QUE_LIMIT               1   /* tbd - move? */
#define _CFG_NUM_OOO_QUE                    8

/* *********************************************************************       */
/* ******                    RARP and BOOTP                 ************       */
/* *********************************************************************       */

/*   How many times to retry rarp before giving up     */
#define _CFG_RARP_TRIES                     10

/* Number of second to wait before retrying a RARP request     */
#define _CFG_RARP_TMO                       30

/* How many times to retry BOOTP before giving up     */
#define _CFG_BOOTP_RETRIES                  4

/* Number of routers from BOOTP server which will be processed   */
#define _CFG_BOOTP_RTSIZE                   5

/* Number of gateway servers that can be stored in the interface info structure   */
#define CFG_N_DEFAULT_GATEWAYS              1

/* ************************************************************************       */
/* *******************     DNS and DATABASE  ******************************       */
/* ************************************************************************       */

#define _CFG_MIN_DNS_DELAY                  10     /* The minimum (initial) delay between & */
                                                   /* name server retries.                        */
                                                   /* Units: seconds                              */
#define _CFG_MAX_DNS_DELAY                  60     /* The maximum delay between retries */
                                                   /* Units: seconds     */
#define _CFG_DNS_RETRIES                    3      /* Max # of retries - delay is doubled
                                                      for each retry */
#define _CFG_DNS_NAME_LEN                   100    /* Maximum length of a hostname */
                                                   /* NOTE: Needs to be a multiple of   */
                                                   /*       4 for alignment issues      */
#define _CFG_MAX_HT_ENT                     6      /* Maximum # of host table entries */
#define _CFG_MAX_HC_ENT                     6      /* Maximum # of host cache entries */
#define _CFG_MAX_DNS_SRV                    5      /* Maximum # of name servers  */
#define _CFG_MAX_HX_ADDRS                   3      /* Size of array h_addr_list in */
                                                   /* hostentext (extended hostent)                 */
                                                   /*  structure.                                   */
#define _CFG_MAX_HX_ALIAS                   1      /* Size of h_aliases in hostentext               */
#define  CFG_N_DEFAULT_DNS_SERVERS          2      /* Size of dns servers for the interface info    */

/* ************************************************************************          */
/* *********************     PRINTF & FLOATING POINT    *******************          */
/* ************************************************************************          */

/*
 * Set to non-zero value if you want tc/tm_[*]printf functions to support
 * floating point values.
 * NOTE: for this, the RTL sprintf() function is called. So if your RTL
 *       sprintf() doesn't support floating point values, you should add
 *       your own code instead.
 * NOTE2: set this value only to non-zero if your APP requires this type
 *        of floating-point displaying support!
 * NOTE3: SSL/CRYPTO: DES and other (speed measurement) test routines use
 *        floating point printf to report results.
 *        This setting is also used to control availability of 'floating
 *        point printing' in 'openssl/bio/b_print.c' as it has the same
 *        purpose.
 */
#define INCLUDE_PRINTF_FLOAT_SUPPORT        0

/*#if (INCLUDE_TEST_CRYPT) && (INCLUDE_CRYPT || INCLUDE_SSL))   */
/*#undef INCLUDE_rtp_printf_FLOAT_SUPPORT                       */
/*#define INCLUDE_rtp_printf_FLOAT_SUPPORT     1                */
/*#endif                                                        */

/* set to the radix (10 or 16 are preferred) of the numbers of any MAC address
   being printed by the tc_*printf() function series.
 */
#define CFG_PRINTF_DUMP_MAC_ADDRESS_RADIX    16

/* ************************************************************************          */
/* *********************     MOUSE                  ***********************          */
/* ************************************************************************          */

/* set to a non-zero value if you want to include the RTIP mouse_*() driver routines   */
#define INCLUDE_MOUSE                       0      /* NOT SUPPORTED YET */

/* ************************************************************************          */
/* *********************     Compatibility          ***********************          */
/* ************************************************************************          */

/* set to a non-zero value if you want to use the new UNIX/Winsock compatible
   'struct sockaddr_in' and 'struct in_addr' definitions. This is mandatory
   for SSL and most of the examples, as now you can copy UNIX-style code, like

      server.sin_addr.s_addr = htonl(ipaddress);

   instead of

      tc_mv4((RTP_PFUINT8)&server.sin_addr, &ip_addr, IP_LEN);

   Set to ZERO(0) for unconditional RTIP 2.x backwards compatibility.
 */
#if (INCLUDE_SSL)
#define CFG_BSD_44                          1 /* DO NOT CHANGE */
#else                                       
#define CFG_BSD_44                          1
#endif

/* ************************************************************************          */
/* *********************     TASK MANAGER           ***********************          */
/* ************************************************************************          */

/* Configure the task manager section in osport.c     */

/* The total maximum number of tasks we wish to manage. Each task that
   uses the package needs to have one of task control blocks. For internally
   spawned tasks it contains information used for spawning the task, its
   priority, its stack size and its entry point. All tasks have a user
   structure that contains task specific data such as errno values current
   working directory (if rtfs is included) and user defined void data pointers
   Used if USE_RTIP_TASKING_PACKAGE (in osport.h) is set to 1 */
#define CFG_NUM_TASK_CONTROL_BLOCKS         30

/* The first of the task control blocks to be available for storing
   task specific data for tasks that were spawned externally and then
   called the api. There are
    CFG_NUM_TASK_CONTROL_BLOCKS - CFG_FIRST_EXTERNAL_TASK of these
   available (ie: this many externally spawned tasks may use the
   api and have unique contexts.
*/
#define CFG_FIRST_EXTERNAL_TASK             25 /* Here and above are never spawned */

/* *********************************************************************       */
/* ******                     MD5                           ************       */
/* *********************************************************************       */
#if (INCLUDE_SNMPV3 && INCLUDE_SNMPV3_AUTHENTICATE)
#define INCLUDE_RSA_MD5                     0
#define INCLUDE_SSL_MD5                     1
#elif (INCLUDE_SMB_SRV || INCLUDE_SSL)
#define INCLUDE_RSA_MD5                     0
#define INCLUDE_SSL_MD5                     1
#else                                       
#define INCLUDE_RSA_MD5                     1
#define INCLUDE_SSL_MD5                     0
#endif

/* ************************************************************************          */
/* *********************            STACKS          ***********************          */
/* ************************************************************************          */

/* IGNORE THIS SECTION IF KS_DECLARE_STACK is 0 (see osport.h)   */

#if (INCLUDE_SNMP || INCLUDE_DMH_SNMP)
#define SNMP_STACK                          1
#else                                       
#define SNMP_STACK                          0
#endif

#if (INCLUDE_NFS_SRV || INCLUDE_NFS_CLI)
/* client for nfsdemo; server for daemon   */
#define NFS_STACK                           1
#else                                       
#define NFS_STACK                           0
#endif

#if (INCLUDE_DHCP_SRV)
#define DHCP_STACK                          1
#else                                       
#define DHCP_STACK                          0
#endif


/* ************************************************************************          */
/* THE REST OF THE FILE IS NOT CONFIGURATION DATA                                    */
/* set defines for offsets in alloced ports to check                                 */
/* SPR: move this to rtip.h but socket.h uses TOTAL_PORTS                            */
/* ************************************************************************          */
#if (INCLUDE_MALLOC_PORTS)
#define FIRST_TCP_PORT                      0
#else                                       
#define FIRST_TCP_PORT                      CFG_NUDPPORTS
#endif                                      
#define FIRST_UDP_PORT                      0
#define TOTAL_TCP_PORTS                     (CFG_NUDPPORTS+CFG_NTCPPORTS)
#define TOTAL_UDP_PORTS                     CFG_NUDPPORTS
#define TOTAL_PORTS                         (CFG_NTCPPORTS+CFG_NUDPPORTS)
#define IS_TCP(PORT)                        (1)
#define IS_UDP(PORT)                        (1)


/* ************************************************************************         */
#if (INCLUDE_RIPV1 || INCLUDE_RIPV2)
#define INCLUDE_RIP                         1
#else                                       
#define INCLUDE_RIP                         0
#endif

/* ************************************************************************         */
/* Number of DCUs; DCUs are the internal RTIP data structure which
   contains information about packets received, about to be sent etc, etc;
   the TCP windows and other internal lists consist of linked lists of
   DCUs; DCUs contain a pointer to the data area whose size is defined
   by CFG_MAX_PACKETSIZE */
#if (INCLUDE_MALLOC_DCU_INIT || INCLUDE_MALLOC_DCU_AS_NEEDED)
#define CFG_NDCUS    (CFG_NUM_PACKETS0+CFG_NUM_PACKETS1+CFG_NUM_PACKETS2+\
                      CFG_NUM_PACKETS3+CFG_NUM_PACKETS4+CFG_NUM_PACKETS5)
#else
#define CFG_NDCUS    (_CFG_NUM_PACKETS0+_CFG_NUM_PACKETS1+_CFG_NUM_PACKETS2+\
                      _CFG_NUM_PACKETS3+_CFG_NUM_PACKETS4+_CFG_NUM_PACKETS5)
#endif
/* ************************************************************************         */
/* sanity checks                                                                    */
#if (INCLUDE_SNMPV2 || INCLUDE_SNMPV3)
#undef  INCLUDE_SNMP
#define INCLUDE_SNMP                    1
#endif

#if (INCLUDE_SNMPV3)
#undef  INCLUDE_SNMPV2
#define INCLUDE_SNMPV2                  1
#endif

#if (!INCLUDE_SNMP)
#undef  INCLUDE_SNMPV2
#define INCLUDE_SNMPV2                  0
#undef  INCLUDE_SNMPV3                  
#define INCLUDE_SNMPV3                  0
#endif

#if (!INCLUDE_SNMPV2)
#undef  INCLUDE_SNMPV2_AUTHENTICATE
#define INCLUDE_SNMPV2_AUTHENTICATE     0
#endif

/* integrity checks: - SNMPv3? then also SNMPv2!   */
#if (INCLUDE_SNMP && INCLUDE_SNMPV3)
  #undef INCLUDE_SNMPV2
  #undef INCLUDE_SNMPV2_AUTHENTICATE
/*  #undef INCLUDE_CRYPT   */

  #define INCLUDE_SNMPV2                1
  #define INCLUDE_SNMPV2_AUTHENTICATE   1
/*  #define INCLUDE_CRYPT                 1   */

#elif (INCLUDE_SNMP && INCLUDE_SNMPV2 && INCLUDE_SNMPV2_AUTHENTICATE != 0)
/*  #undef INCLUDE_CRYPT                    */
/*  #define INCLUDE_CRYPT                 1 */

#elif (!INCLUDE_SNMP && !INCLUDE_SSL)
/*  #undef INCLUDE_CRYPT                    */
/*  #define INCLUDE_CRYPT                 0 */

#elif (INCLUDE_PPP && INCLUDE_CHAP)
/*  #undef INCLUDE_CRYPT                    */
/*  #define INCLUDE_CRYPT                 1 */
#endif

/* ************************************************************************         */
#if (INCLUDE_RIP)
#undef  INCLUDE_ROUTING_TABLE
#define INCLUDE_ROUTING_TABLE           1
#undef  INCLUDE_RT_TTL                  
#define INCLUDE_RT_TTL                  1
#endif

#if (INCLUDE_WINETHER)
/* ************************************************************************         */
#undef INCLUDE_SLIP
#undef INCLUDE_CSLIP
#undef INCLUDE_PPP
#undef INCLUDE_MODEM

#define INCLUDE_SLIP                    0   /* DO NOT CHANGE */
#define INCLUDE_CSLIP                   0   /* DO NOT CHANGE */
#define INCLUDE_PPP                     0   /* DO NOT CHANGE */
#define INCLUDE_MODEM                   0   /* DO NOT CHANGE */
#endif

#if (defined(EMBOS_GNU))
/* ************************************************************************      */
#if (INCLUDE_SLIP || INCLUDE_CSLIP || defined(INCLUDE_PPP))
#define NEW_UARTPORT                    1
#endif  /* end of (INCLUDE_SLIP || INCLUDE_CSLIP || INCLUDE_PPP) */
#endif

// ********************************************************************
#if (defined(MC68360) || POWERPC)
#if (INCLUDE_SLIP || INCLUDE_CSLIP || INCLUDE_PPP)
#define EBSSMCUART    	                1       /* Use Static Memory Controller channel for UART, driver in eth360.c */
#else                                   
#define EBSSMCUART    	                0       /* Use Static Memory Controller channel for UART, driver in eth360.c */
#endif
#endif


#endif /* __RTIPCONF__ */

