////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Portions Copyright (c) Secret Labs LLC.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _NET_DECL_LWIP_H_
#define _NET_DECL_LWIP_H_

#ifndef TCPIP_LWIP
#include <lwip_selector.h>
#endif 

#ifndef TCPIP_LWIP
#error Only include net_decl_lwip.h when using LWIP stack
#endif

#include "network_defines_lwip.h"

#define NO_SYS 0
#define ERRNO  1


#ifdef PLATFORM_DEPENDENT__MEM_SIZE
#define MEM_SIZE PLATFORM_DEPENDENT__MEM_SIZE
#else
#define MEM_SIZE MEM_SIZE__default
#endif

#ifdef PLATFORM_DEPENDENT__MEMP_NUM_PBUF
#define MEMP_NUM_PBUF PLATFORM_DEPENDENT__MEMP_NUM_PBUF
#else
#define MEMP_NUM_PBUF MEMP_NUM_PBUF__default
#endif

#ifdef PLATFORM_DEPENDENT__MEMP_NUM_UDP_PCB
#define MEMP_NUM_UDP_PCB PLATFORM_DEPENDENT__MEMP_NUM_UDP_PCB
#else
#define MEMP_NUM_UDP_PCB MEMP_NUM_UDP_PCB__default
#endif

#ifdef PLATFORM_DEPENDENT__MEMP_NUM_TCP_PCB
#define MEMP_NUM_TCP_PCB PLATFORM_DEPENDENT__MEMP_NUM_TCP_PCB
#else
#define MEMP_NUM_TCP_PCB MEMP_NUM_TCP_PCB__default
#endif

#ifdef PLATFORM_DEPENDENT__MEMP_NUM_TCP_PCB_LISTEN
#define MEMP_NUM_TCP_PCB_LISTEN PLATFORM_DEPENDENT__MEMP_NUM_TCP_PCB_LISTEN
#else
#define MEMP_NUM_TCP_PCB_LISTEN MEMP_NUM_TCP_PCB_LISTEN__default
#endif

#ifdef PLATFORM_DEPENDENT__MEMP_NUM_TCP_SEG
#define MEMP_NUM_TCP_SEG PLATFORM_DEPENDENT__MEMP_NUM_TCP_SEG
#else
#define MEMP_NUM_TCP_SEG MEMP_NUM_TCP_SEG__default
#endif

#ifdef PLATFORM_DEPENDENT__MEMP_NUM_SYS_TIMEOUT
#define MEMP_NUM_SYS_TIMEOUT PLATFORM_DEPENDENT__MEMP_NUM_SYS_TIMEOUT
#else
#define MEMP_NUM_SYS_TIMEOUT MEMP_NUM_SYS_TIMEOUT__default
#endif

#ifdef PLATFORM_DEPENDENT__MEMP_NUM_NETBUF
#define MEMP_NUM_NETBUF PLATFORM_DEPENDENT__MEMP_NUM_NETBUF
#else
#define MEMP_NUM_NETBUF MEMP_NUM_NETBUF__default
#endif

#ifdef PLATFORM_DEPENDENT__MEMP_NUM_NETCONN
#define MEMP_NUM_NETCONN PLATFORM_DEPENDENT__MEMP_NUM_NETCONN
#else
#define MEMP_NUM_NETCONN MEMP_NUM_NETCONN__default
#endif

#ifdef PLATFORM_DEPENDENT__PBUF_POOL_SIZE
#define PBUF_POOL_SIZE PLATFORM_DEPENDENT__PBUF_POOL_SIZE
#else
#define PBUF_POOL_SIZE PBUF_POOL_SIZE__default
#endif

#ifdef PLATFORM_DEPENDENT__PBUF_POOL_BUFSIZE
#define PBUF_POOL_BUFSIZE PLATFORM_DEPENDENT__PBUF_POOL_BUFSIZE
#else
#define PBUF_POOL_BUFSIZE PBUF_POOL_BUFSIZE__default
#endif

#ifdef PLATFORM_DEPENDENT__TCP_MSS
#define TCP_MSS PLATFORM_DEPENDENT__TCP_MSS
#else
#define TCP_MSS TCP_MSS__default
#endif

#ifdef PLATFORM_DEPENDENT__TCP_SND_BUF
#define TCP_SND_BUF PLATFORM_DEPENDENT__TCP_SND_BUF
#else
#define TCP_SND_BUF TCP_SND_BUF__default
#endif

#ifdef PLATFORM_DEPENDENT__TCP_SND_QUEUELEN
#define TCP_SND_QUEUELEN PLATFORM_DEPENDENT__TCP_SND_QUEUELEN
#else
#define TCP_SND_QUEUELEN TCP_SND_QUEUELEN__default
#endif

#ifdef PLATFORM_DEPENDENT__TCP_WND
#define TCP_WND PLATFORM_DEPENDENT__TCP_WND
#else
#define TCP_WND TCP_WND__default
#endif

#ifdef PLATFORM_DEPENDENT__TCP_SNDLOWAT
#define TCP_SNDLOWAT PLATFORM_DEPENDENT__TCP_SNDLOWAT
#else
#define TCP_SNDLOWAT TCP_SNDLOWAT__default
#endif

//--//

/* LWIP options that are the same for all configurations */

/* MEM_ALIGNMENT: should be set to the alignment of the CPU for which
   lwIP is compiled. 4 byte alignment -> define MEM_ALIGNMENT to 4, 2
   byte alignment -> define MEM_ALIGNMENT to 2. */
#define MEM_ALIGNMENT           4

/* These two control whether reclaimer functions should be compiled
   in. Should always be turned on (1). */
#define MEM_RECLAIM             1
#define MEMP_RECLAIM            1

/* PBUF_LINK_HLEN: the number of bytes that should be allocated for a
   link level header. */
#define PBUF_LINK_HLEN          16

/* ---------- TCP options ---------- */
#define LWIP_TCP                1
#define TCP_TTL                 255

/* Controls if TCP should queue segments that arrive out of
   order. Define to 0 if your device is low on memory. */
#define TCP_QUEUE_OOSEQ         1

/* Maximum number of retransmissions of data segments. */
#define TCP_MAXRTX              12

/* Maximum number of retransmissions of SYN segments. */
#define TCP_SYNMAXRTX           4

/* ---------- ARP options ---------- */
#define ARP_TABLE_SIZE          10
#define ARP_QUEUEING            1

/* ---------- IP options ---------- */
/* Define IP_FORWARD to 1 if you wish to have the ability to forward
   IP packets across network interfaces. If you are going to run lwIP
   on a device with only one network interface, define this to 0. */
#if defined(PLATFORM_ARM_NetduinoPlus2) || defined(PLATFORM_ARM_Netduino3Ethernet)
#define IP_FORWARD              0
#else
#define IP_FORWARD              1
#endif

/* If defined to 1, IP options are allowed (but not parsed). If
   defined to 0, all packets with IP options are dropped. */
#define IP_OPTIONS              1

/* IP reassembly and segmentation.These are orthogonal even
 * if they both deal with IP fragments */
#define IP_REASSEMBLY           1
#define IP_FRAG                 1

/* ---------- ICMP options ---------- */
#define ICMP_TTL                255

/* ---------- DHCP options ---------- */
/* Define LWIP_DHCP to 1 if you want DHCP configuration of
   interfaces. */
#define LWIP_DHCP               1

/* 1 if you want to do an ARP check on the offered address
   (recommended). */
#if defined(PLATFORM_ARM_NetduinoPlus2) || defined(PLATFORM_ARM_Netduino3Ethernet)
#define DHCP_DOES_ARP_CHECK     1
#else
#define DHCP_DOES_ARP_CHECK     0
#endif

/* ---------- UDP options ---------- */
#define LWIP_UDP                1
#define UDP_TTL                 255

/* ---------- Statistics options ---------- */
#define LWIP_STATS              0

/** SYS_LIGHTWEIGHT_PROT
 * define SYS_LIGHTWEIGHT_PROT in lwipopts.h if you want inter-task
 * protection for certain critical regions during buffer allocation
 * and deallocation and memory allocation and deallocation.
 */
#define SYS_LIGHTWEIGHT_PROT    1

#define LWIP_COMPAT_SOCKETS     1

#define LWIP_PROVIDE_ERRNO      1

/* ---------- SNMP options ---------- */
#define LWIP_SNMP               0 /*LwIP 1.2.0*/
#if defined(PLATFORM_ARM_NetduinoPlus2) || defined(PLATFORM_ARM_Netduino3Ethernet)
#define LWIP_IGMP               0 /*LwIP 1.2.0*/
#else
#define LWIP_IGMP               1 /*LwIP 1.2.0*/
#endif

// thread priorities are in VDK terms - 1 is highest, 30 is lowest
#define TCPIP_THREAD_PRIO       5
#define DEFAULT_THREAD_PRIO     10
#define LOW_THREAD_PRIO         29

#define OPENSSL_SMALL_FOOTPRINT 1

//--// RAM size estimate macro

#if 0 // TODO - implement similar for LWIP
#define NETWORK_RAM_SIZE_ESTIMATE() (\
    _NETWORK_SIZEOF_NONE + \
    NETWORK_MULTICAST_LIST_SIZE        * (_NETWORK_SIZEOF_MCLISTSIZE) + \
    NETWORK_PACKET_POOL_0__NUM_PACKETS * (NETWORK_PACKET_POOL_0__PACKET_SIZE + _NETWORK_SIZEOF_DCU) + \
    NETWORK_PACKET_POOL_1__NUM_PACKETS * (NETWORK_PACKET_POOL_1__PACKET_SIZE + _NETWORK_SIZEOF_DCU) + \
    NETWORK_PACKET_POOL_2__NUM_PACKETS * (NETWORK_PACKET_POOL_2__PACKET_SIZE + _NETWORK_SIZEOF_DCU) + \
    NETWORK_PACKET_POOL_3__NUM_PACKETS * (NETWORK_PACKET_POOL_3__PACKET_SIZE + _NETWORK_SIZEOF_DCU) + \
    NETWORK_PACKET_POOL_4__NUM_PACKETS * (NETWORK_PACKET_POOL_4__PACKET_SIZE + _NETWORK_SIZEOF_DCU) + \
    NETWORK_PACKET_POOL_5__NUM_PACKETS * (NETWORK_PACKET_POOL_5__PACKET_SIZE + _NETWORK_SIZEOF_DCU) + \
    _NETWORK_TOTAL_PACKET_COUNT        * _NETWORK_SIZEOF_PACKET_OVERHEAD     + \
    NETWORK_NUM_IFACES                 * _NETWORK_SIZEOF_IFACE        + \
    NETWORK_ROUTINGTABLE_SIZE          * _NETWORK_SIZEOF_ROUTINGTABLE + \
    NETWORK_ARP_NUM_TABLE_ENTRIES      * _NETWORK_SIZEOF_ARP_ENTRY    + \
    NETWORK_TCP_NUM_PORTS__SUPPORTED   * _NETWORK_SIZEOF_TCPPORT      + \
    NETWORK_UDP_NUM_PORTS__SUPPORTED   * _NETWORK_SIZEOF_UDPPORT      + \
    NETWORK_FRAG_TABLE_SIZE            * _NETWORK_SIZEOF_FRAG_ENTRY   + \
    NETWORK_NAT_NUM_ENTRIES            * _NETWORK_SIZEOF_NAT_ENTRY )    \

typedef char NETWORK_COMPILE_TIME_ASSERT[NETWORK_MEMORY_POOL__SIZE - NETWORK_RAM_SIZE_ESTIMATE()];
#endif

#endif //_NET_DECL_LWIP_H_

