/* RTIPEXT.H - RTIP EXTERNAL GLOBAL DATA DECLARATIONS                   */
/*                                                                      */
/* EBS - RTIP                                                           */
/*                                                                      */
/* Copyright Peter Van Oudenaren , 1993                                 */
/* All rights reserved.                                                 */
/* This code may not be redistributed in source or linkable object form */
/* without the consent of its author.                                   */
/*                                                                      */
/*  Module description:                                                 */
/*      This module contains all the extern global data declarations    */
/*      for RTIP.                                                       */

#ifndef __RTIPEXT__
#define __RTIPEXT__ 1

#include "xnconf.h"

#if (RTPLATFORM)
#include "rtp.h"
#include "rtpthrd.h"
#include "rtip.h"     // exception here
#include "rtipdata.h"
#else
#include "rtip.h"
#include "os.h"
#endif

#include "udp.h"
#include "tcp.h"
#ifdef __cplusplus
RTP_EXTERN "C" {
#endif

/* ********************************************************************   */
/* TABLES                                                                 */
/* ********************************************************************   */
#if (INCLUDE_RUN_TIME_CONFIG)
RTP_EXTERN PIFACE        ifaces;                        /* interface structures */
#else
RTP_EXTERN IFACE  RTP_FAR ifaces[CFG_NIFACES];          /* interface structures */
#endif
#if (INCLUDE_ROUTING_TABLE)
#if (INCLUDE_RUN_TIME_CONFIG)
RTP_EXTERN PROUTE rt_table;
#else
RTP_EXTERN ROUTE RTP_FAR rt_table[CFG_RTSIZE];          /* routing table */
#endif
RTP_EXTERN POS_LIST     root_rt_default;                /* default gateways */
#else
RTP_EXTERN RTP_UINT8 RTP_FAR default_gateway_address[IP_ALEN];
#endif

/* ARP CACHE   */
#if (INCLUDE_ARP || INCLUDE_IPV6)
#if (INCLUDE_RUN_TIME_CONFIG)
RTP_EXTERN ARPCACHE RTP_FAR *tc_arpcache;
#else
RTP_EXTERN ARPCACHE RTP_FAR tc_arpcache[CFG_ARPCLEN];
#endif
#endif

/* IP to ethernet translation table;                                    */
/* used for ethernet packets where translation is known by application, */
/* therefore, ARP is not necessary                                      */
#if (INCLUDE_ETH_BUT_NOARP && INCLUDE_RUN_TIME_CONFIG)
RTP_EXTERN IP2ETH RTP_FAR *ip2eth_table;
#elif (INCLUDE_ETH_BUT_NOARP)
RTP_EXTERN IP2ETH RTP_FAR ip2eth_table[CFG_NUM_IP2ETH_ADDR];
#endif

#if (INCLUDE_RARP_SRV)
    /* Table containing IP address an ether address of clients.           */
    /* The rarp server function will look up the IP address based on the  */
    /* RARP requestors etehernet address. The IP address will be returned */
    /* to the client.                                                     */
    /*                                                                    */
    RTP_EXTERN struct rarp_table RTP_FAR rt[RT_TABLE_SIZE];
#endif

/* ********************************************************************   */
/* RTIP API DATA                                                          */
/* ********************************************************************   */
/* See the RTFS/RTIP common section at the end for more                   */

#if (INCLUDE_RARP)
/* depth of calls to xn_rarp; used for reentrancy test   */
RTP_EXTERN int  RTP_FAR rarp_depth;
#endif
#if (INCLUDE_BOOTP)
/* depth of calls to xn_bootp; used for reentrancy test   */
RTP_EXTERN int  RTP_FAR bootp_depth;
#endif

/* default interface for xn_interface_mcast if -1 is passed as the   */
/* interface (the value is set by xn_interface_opt)                  */
RTP_EXTERN int RTP_FAR default_mcast_iface;

/* initialization functions   */
RTP_EXTERN P_INIT_FNCS registered_fncs;

#if (INCLUDE_ERROR_STRING_NUMBER)
RTP_EXTERN char errno_buf[128];
#endif

/* Pointer to device table   */
RTP_EXTERN PDEVTABLE devices_ptr;
#if (!BUILD_NEW_BINARY && !INCLUDE_RUN_TIME_CONFIG)
RTP_EXTERN EDEVTABLE RTP_FAR devices[CFG_NUM_DEVICES];
#endif

/* ********************************************************************   */
/* IP DATA                                                                */
/* ********************************************************************   */
RTP_EXTERN RTP_UINT16  RTP_FAR ipsequence;         /* ip packet numbers start here */
                                        /* and increment   */
RTP_EXTERN RTP_UINT32 RTP_FAR ip_forwarding;        /* 1=can forward; 2=can't forward */
                                        /* can be modified by SNMP   */

/* ********************************************************************   */
/* SOCKET DATA                                                            */
/* ********************************************************************   */

/* global data returned by inet_ntoa                             */
/* NOTE: user must extract the data from ntoa_str before calling */
/*       inet_ntoa again                                         */
RTP_EXTERN char RTP_FAR ntoa_str[IP_STR_LEN];

/* ********************************************************************   */
/* DNS                                                                    */
/* ********************************************************************   */

#if (INCLUDE_DNS)

/* h_table is the user host table. Names which have known IP addresses    */
/* can be added to this table using API calls. This will make the service */
/* of these domain names much faster.  This host table is the first place */
/* that gethostbyname() will look for names. The host_cache is the second */
/* place, and the name server is the third.                               */
RTP_EXTERN struct   hostentext RTP_FAR h_table[CFG_MAX_HT_ENT];

/* host_cache is the internal host cache.                                  */
/* If caching is disabled, every time that gethostbyname()/gethostbyaddr() */
/* is called, it adds an entry to this cache and returns a pointer to the  */
/* cached entry.                                                           */
/* If caching is enabled, gethostbyname()/gethostbyaddr() will look for    */
/* entry in cache and return it if found.  If not found, it performs       */
/* DNS lookup and adds an entry to cache.  If free entry is not found,     */
/* the entry with the smallest ttl is used                                 */
RTP_EXTERN struct   hostentext  RTP_FAR host_cache[CFG_MAX_HC_ENT];

/* host_cache[host_cache_index] is the current cached name entry. When   */
/* gethostbyname() and gethostbyaddr() finds a host, it will cache the   */
/* name at the index+1; xn_add_host_cache_entry also adds entries to     */
/* the host_cache                                                        */
RTP_EXTERN int host_cache_index;                     /* index to host cache */

/* the server_ip_table is a list of IP addresses.  Both gethostbyname and   */
/* gethostbyaddr loop through this list looking for name servers.           */
/* addresses are in network byte order                                      */
RTP_EXTERN RTP_UINT32 RTP_FAR server_ip_table[CFG_MAX_DNS_SRV]; /* 32-bit IP addresses */


#endif
RTP_EXTERN int  RTP_FAR cnt_dns;    /* number of valid DNS servers */
RTP_EXTERN char RTP_FAR my_domain_name[CFG_DNS_NAME_LEN];

/* ********************************************************************   */
/* OS DATA                                                                */
/* ********************************************************************   */

#if (!INCLUDE_MALLOC_DCU_AS_NEEDED)
RTP_EXTERN POS_LIST root_dcu;                           /* head of the free DCU list */
                                            /* Only used at startup   */
RTP_EXTERN int RTP_FAR dcu_size_array[CFG_NUM_FREELISTS]; /* Table of sizes of DCUs */
                                              /* managed by root_dcu_array[]   */
RTP_EXTERN POS_LIST root_dcu_array[CFG_NUM_FREELISTS];/* head of the free DCU list */
                                           /* Used to manage free list by   */
                                           /* one list per dcu size         */
#if (INCLUDE_NO_DCU_BLOCK)
RTP_EXTERN POS_LIST ports_blocked_for_dcu;
#endif
/* lowest number of free packet ever   */
RTP_EXTERN int RTP_FAR lowest_free_packets_array[CFG_NUM_FREELISTS];

/* lowest number of free packets now   */
RTP_EXTERN int RTP_FAR current_free_packets_array[CFG_NUM_FREELISTS];

/* higest number of free packet ever   */
RTP_EXTERN int RTP_FAR highest_free_packets_array[CFG_NUM_FREELISTS];

/* pointers to DCUs and packets   */
RTP_EXTERN EPACKET RTP_FAR *dcu_pool_ptr;
#endif      /* !INCLUDE_MALLOC_DCU_AS_NEEDED */

#if (INCLUDE_MALLOC_DCU_AS_NEEDED)
RTP_EXTERN int RTP_FAR current_alloc_packets;
RTP_EXTERN int RTP_FAR highest_alloc_packets;
#endif  /* INCLUDE_MALLOC_DCU_AS_NEEDED */

/* ********************************************************************   */
/* Pools of memory that are threaded onto freelist by os_memory_init().
   they may be replaced by calling an allocator function instead of
   declaring them externally. The memory doesnt have to be contiguous

   Note: Some architectures limit an array size (intel real mode for example)
   We may want to allocate > a segment's worth so to do so we allocate two
   arrays. More could be added if needed
*/

#if (INCLUDE_MALLOC_DCU_INIT)
RTP_EXTERN RTP_PFUINT8 packet_pool0;
RTP_EXTERN RTP_PFUINT8 packet_pool1;
RTP_EXTERN RTP_PFUINT8 packet_pool2;
RTP_EXTERN RTP_PFUINT8 packet_pool3;
RTP_EXTERN RTP_PFUINT8 packet_pool4;
RTP_EXTERN RTP_PFUINT8 packet_pool5;
#endif


#if (!INCLUDE_MALLOC_DCU_INIT && !INCLUDE_MALLOC_DCU_AS_NEEDED)
/* DCU structures   */
RTP_EXTERN EPACKET RTP_FAR tc_dcu_pool[CFG_NDCUS];

/* DATA AREA FOR DCUs   */
#if (CFG_NUM_PACKETS0)
RTP_EXTERN RTP_UINT16 RTP_FAR packet_pool0[CFG_NUM_PACKETS0]
                         [(CFG_PACKET_SIZE0+CFG_PACKET_ADJ+PKT_GUARD_SIZE)/2];
#endif

#if (CFG_NUM_PACKETS1)
RTP_EXTERN RTP_UINT16 RTP_FAR packet_pool1[CFG_NUM_PACKETS1]
                         [(CFG_PACKET_SIZE1+CFG_PACKET_ADJ+PKT_GUARD_SIZE)/2];
#endif

#if (CFG_NUM_PACKETS2)
RTP_EXTERN RTP_UINT16 RTP_FAR packet_pool2[CFG_NUM_PACKETS2]
                         [(CFG_PACKET_SIZE2+CFG_PACKET_ADJ+PKT_GUARD_SIZE)/2];
#endif

#if (CFG_NUM_PACKETS3)
RTP_EXTERN RTP_UINT16 RTP_FAR packet_pool3[CFG_NUM_PACKETS3]
                         [(CFG_PACKET_SIZE3+CFG_PACKET_ADJ+PKT_GUARD_SIZE)/2];
#endif

#if (CFG_NUM_PACKETS4)
RTP_EXTERN RTP_UINT16 RTP_FAR packet_pool4[CFG_NUM_PACKETS4]
                         [(CFG_PACKET_SIZE4+CFG_PACKET_ADJ+PKT_GUARD_SIZE)/2];
#endif

#if (CFG_NUM_PACKETS5)
RTP_EXTERN RTP_UINT16 RTP_FAR packet_pool5[CFG_NUM_PACKETS5]
                         [(CFG_PACKET_SIZE5+CFG_PACKET_ADJ+PKT_GUARD_SIZE)/2];
#endif

#endif /* !INCLUDE_MALLOC_DCU_INIT && !INCLUDE_MALLOC_DCU_AS_NEEDED */

#if (INCLUDE_MALLOC_DCU_INIT)
    /* total number of packets to allocate (most of these will be attached    */
    /* to DCUs); initialized to CFG_NPACKETS but you should change this value */
    /* before calling xn_rtip_init in order to allocate a different           */
    /* number of packets                                                      */

    /* total number of DCUs to allocate;                            */
    /* initialized to CFG_NDCUS but you should change this value    */
    /* before calling xn_rtip_init in order to allocate a different */
    /* number of DCUs                                               */
    RTP_EXTERN int RTP_FAR total_ndcus;
#endif /* INCLUDE_MALLOC_DCU_INIT */

/* total number of packets and dcus allocated   */
RTP_EXTERN int RTP_FAR ndcus_alloced;

/* ********************************************************************   */
/* POOLS OF PORTS                                                         */

#if (INCLUDE_MALLOC_PORTS)
/* array of allocated port pointers   */
RTP_EXTERN PALLOCED_PORTS_INDEX root_alloced_ports;     
RTP_EXTERN PALLOCED_PORTS_INDEX alloced_ports;
#else
RTP_EXTERN ALLOCED_PORTS_INDEX RTP_FAR alloced_ports[TOTAL_PORTS];
#endif

#if (!INCLUDE_MALLOC_PORTS)
#if (INCLUDE_UDP || INCLUDE_RAW)
    RTP_EXTERN POS_LIST root_udp_port;          /* head of the free UDP port list */
#if (INCLUDE_RUN_TIME_CONFIG)
    RTP_EXTERN PUDPPORT tc_udpp_pool;        /* UDP Ports pointer */
#else
    RTP_EXTERN UDPPORT RTP_FAR tc_udpp_pool[CFG_NUDPPORTS];   /* UDP Ports */
#endif
#endif      /* INCLUDE_UDP || INCLUDE_RAW */

#if (INCLUDE_TCP)
    RTP_EXTERN POS_LIST root_tcp_port;          /* head of the free TCP port list */
#if (INCLUDE_RUN_TIME_CONFIG)
    RTP_EXTERN PTCPPORT tc_tcpp_pool;                       /* TCP ports pointer */
#else
    RTP_EXTERN TCPPORT RTP_FAR tc_tcpp_pool[CFG_NTCPPORTS]; /* TCP ports */
#endif
#endif      /* INCLUDE_TCP */

#endif      /* !INCLUDE_MALLOC_PORTS */

RTP_EXTERN TCP_REGISTERED_FUNCTIONS    tcp_functions;
/* ********************************************************************   */
#if (INCLUDE_SSL && INCLUDE_BGET)
RTP_EXTERN char RTP_FAR ssl_mempool[CFG_SSL_BGET_CORE_SIZE+BGET_OVERHEAD];
#endif  /* (INCLUDE_SSL && INCLUDE_BGET) */

RTP_EXTERN int RTP_FAR lowest_free_packets;     /* lowest number of free packet ever */
RTP_EXTERN int RTP_FAR current_free_packets;    /* lowest number of free packets now */

/* If this value is one or more the device layer ks_invoke_input() accepts   */
/* UDP broadcasts. Otherwise it throws them away. Each port that wishes      */
/* to receive UDP broadcasts increments this variable and decrements         */
/* it when it no longer wants them                                           */
RTP_EXTERN int RTP_FAR allow_udp_broadcasts;

#if (!EBSEXCLUDEOSINT)
/* ********************************************************************   */
/* OSINT DATA                                                             */
/* ********************************************************************   */
/* When interrupt N (n = 0-KS_NUM_INTS) comes through we will call
    rtip_isr_strategy[N](rtip_args[N]);
*/
#if (INCLUDE_SLIP || INCLUDE_CSLIP || INCLUDE_PPP || INCLUDE_TASK_ISRS)
/* If doing the strategy from task level we leave the option to      */
/* call a routine from the interrupt which may have to mask off      */
/* interrupts or do some early processing (this will usually be null */
RTP_EXTERN RTP_IRQ_FN_POINTER rtip_isr_interrupt[KS_NUM_INTS];
RTP_EXTERN PIFACE RTP_FAR rtip_irq_iface[KS_NUM_INTS];
#endif
RTP_EXTERN RTP_IRQ_FN_POINTER   rtip_isr_strategy[KS_NUM_INTS];
RTP_EXTERN void * RTP_FAR       rtip_args[KS_NUM_INTS];
#endif

/* depth of currents calls to interrupt service   */
RTP_EXTERN int RTP_FAR in_irq;

#if (INCLUDE_MODEM)
/* ********************************************************************   */
/* MODEM                                                                  */
/* ********************************************************************   */
/* buffer used to pass input chars from modem to RTIP                     */
RTP_EXTERN RTP_PFCHAR rawdata;

/* default script used to dial a modem where login is required   */
RTP_EXTERN struct al_command RTP_FAR default_script_login[];

/* Pointer to structure; this is done this way due to avoid some compilers   */
/* giving you an error if you RTP_EXTERN an array without its size           */
RTP_EXTERN struct al_command RTP_FAR *default_script_login_ptr;

/* default script used to dial a modem with no login required   */
RTP_EXTERN struct al_command RTP_FAR default_script[];

/* Pointer to structure; this is done this way due to avoid some compilers   */
/* giving you an error if you RTP_EXTERN an array without its size           */
RTP_EXTERN struct al_command RTP_FAR *default_script_ptr;

#endif      /* INCLUDE_MODEM */

#ifdef __cplusplus  /*DM: 9-8-03: added since following variables are declared outside RTP_EXTERN "C" elsewhere */
}
#endif

/* ********************************************************************   */
/* OS PORT DATA                                                           */
/* ********************************************************************   */

/* used to pass task number to tasks spawned   */

#if (INCLUDE_RUN_TIME_CONFIG)
RTP_EXTERN CONTEXT_DATA RTP_FAR *ifaceCtx;
#else
RTP_EXTERN CONTEXT_DATA RTP_FAR ifaceCtx[CFG_NIFACES];
#endif

RTP_EXTERN CONTEXT_DATA RTP_FAR interruptCtx;
RTP_EXTERN CONTEXT_DATA RTP_FAR timerCtx;
RTP_EXTERN CONTEXT_DATA RTP_FAR ripMainCts;

/* Kernel independent  declaration of semaphores and signals.
   The types are defined in os_port.h */
#if (defined(PEGRTIP))
RTP_EXTERN RTP_BOOL hand_kb_to_peg;
RTP_EXTERN RTP_BOOL hand_timer_to_peg;

RTP_EXTERN RTP_SEMAPHORE ks_peg_sig[NUM_PEG_EVENTS];
RTP_EXTERN RTP_MUTEX     ks_peg_sem[NUM_PEG_SEM];
#endif

/* SEMAPHORES   */
RTP_EXTERN RTP_MUTEX criticalsem;
RTP_EXTERN RTP_MUTEX tcpsem;
RTP_EXTERN RTP_MUTEX udpsem;
RTP_EXTERN RTP_MUTEX syslogsem;
RTP_EXTERN RTP_MUTEX tablesem;

#if (RTPX || POLLOS || SH2 || SH3)
extern RTP_UINT32 RTP_FAR timer_tick;
#endif
#ifdef __cplusplus  /*DM: 9-8-03: added since preceding variables are declared outside RTP_EXTERN "C" elsewhere */
RTP_EXTERN "C" {
#endif

// ********************************************************************
// RS232 DATA
// ********************************************************************
#if (INCLUDE_SLIP || INCLUDE_CSLIP || INCLUDE_PPP)
	extern RS232_DATA RTP_FAR glb_rs232_data;
#endif

// *********************************************************************
// PPP
// *********************************************************************
#if (INCLUDE_PPP)
	extern PPP_DATA RTP_FAR glb_ppp_data;
#endif

/* ********************************************************************   */
/* TCP                                                                    */
/* ********************************************************************   */

#if (INCLUDE_TCP)
/* TCP LISTS which include:                                               */
/* offset LISTEN_LIST: Head of list of listener TCP ports                 */
/* offset SOCKET_LIST: Head of list of allocated but not active TCP ports */
/* offset ACTIVE_LIST: Head of list of active TCP ports                   */
/* offset MASTER_LIST: Head of list of master sockets that had a problem  */
/*                     (probably allocating a new socket) during          */
/*                     tcp_interpret; i.e. a SYNC came in and there       */
/*                     was room in the backlog but listen() failed        */
RTP_EXTERN POS_LIST root_tcp_lists[NUM_TCP_LISTS];

RTP_EXTERN RTP_UINT16 RTP_FAR tcp_port_number;       /* port number to use for new sockets */
RTP_EXTERN RTP_BOOL RTP_FAR ka_send_garbage;     /* if set, keepalive will send a garbage */
                                 /* byte   */
RTP_EXTERN long RTP_FAR default_tcp_options; /* default options when allocating a */
                                 /* TCP socket   */
#if (INCLUDE_TCP_OUT_OF_ORDER)
RTP_EXTERN RTP_BOOL RTP_FAR free_ooo_lists;  /* flag for alloc_packet to tell */
                                 /* timeout routine to free all out of   */
                                 /* order packet                         */
#endif
#if (INCLUDE_TCP_PORT_CACHE)
RTP_EXTERN PTCPPORT RTP_FAR cached_tcp_port; /* This value stores the last port */
                                /* we found in tcp_interpret.   */
#endif


#if (INCLUDE_OOO_QUE_LIMIT)
RTP_EXTERN int RTP_FAR num_ooo_que;          /* number of DCUs on the out of order list */
#endif
#endif      /* INCLUDE_TCP */

/* ********************************************************************   */
/* UDP                                                                    */
/* ********************************************************************   */
#if (INCLUDE_UDP)
/* list of heads of UDP socket lists where:                               */
/* offset ACTIVE_LIST: Head of list of active UDP ports                   */
/* offset SOCKET_LIST: Head of list of allocated but not active UDP ports */
RTP_EXTERN POS_LIST root_udp_lists[NUM_UDP_LISTS];

RTP_EXTERN RTP_UINT16 RTP_FAR udp_port_number;      /* port number used by socket to assign */
                                /* unique port numbers to sockets   */
#if (INCLUDE_UDP_PORT_CACHE)
RTP_EXTERN PUDPPORT RTP_FAR cached_udp_port; /* This value stores the last port */
                                /* we found in udp_interpret.        */
                                /* We only save it if port number is */
                                /* unique                            */
#endif
RTP_EXTERN UDP_REGISTERED_FUNCTIONS    udp_functions;
#endif      /* INCLUDE_UDP */

#if (INCLUDE_UDP || INCLUDE_RAW)
RTP_EXTERN long RTP_FAR default_udp_options;      /* default options when allocating a */
                                     /* UDP socket   */
#endif

#if (INCLUDE_RAW)
/* ********************************************************************   */
/* RAW                                                                    */
/* ********************************************************************   */
/* list of heads of TCP socket lists where:                               */
/* offset ACTIVE_LIST: Head of list of active UDP ports                   */
/* offset SOCKET_LIST: Head of list of allocated but not active UDP ports */
RTP_EXTERN POS_LIST root_raw_lists[NUM_UDP_LISTS];
#endif


/* ********************************************************************   */
/* ARP                                                                    */
/* ********************************************************************   */
RTP_EXTERN RTP_UINT32 RTP_FAR arpc_res_tmeout;

/* ********************************************************************   */
/* DEVICE INFORMATION                                                     */
/* ********************************************************************   */

/* size of device table; setup at initialization   */
RTP_EXTERN int RTP_FAR total_devices;

/* total number of valid entries in device table   */
RTP_EXTERN int RTP_FAR valid_device_entries;

/* ********************************************************************   */
/* DRIVERS                                                                */
/* ********************************************************************   */

#if (INCLUDE_MTOKEN)
    RTP_EXTERN MTOKDEVICE RTP_FAR mdevicesoftc[CFG_NUM_MTOKEN];
    RTP_EXTERN MTOK_ADAPTER RTP_FAR madaptersoftc[CFG_NUM_MTOKEN];
#endif

#if (INCLUDE_ED || USE_PCVID_OUTPUT || INCLUDE_LANCE || INCLUDE_PCMCIA || INCLUDE_ERTFS)

#endif

/* ********************************************************************   */
/* FRAGMENT.C                                                             */
/* ********************************************************************   */

#if (INCLUDE_FRAG)
#if (INCLUDE_RUN_TIME_CONFIG)
RTP_EXTERN IP_FRAGLIST RTP_FAR *frag_table;
#else
RTP_EXTERN IP_FRAGLIST RTP_FAR frag_table[CFG_FRAG_TABLE_SIZE];
#endif
RTP_EXTERN RTP_UINT32 RTP_FAR max_frag_size;
#endif

/* ********************************************************************   */
/* TOOLS.C                                                                */
/* ********************************************************************   */




/* ********************************************************************   */
/* OSPORT.C                                                               */
/* ********************************************************************   */

/* set when RTIP has been initialized by xn_rtip_init   */
RTP_EXTERN RTP_BOOL RTP_FAR rtip_initialized;

/* set when RTIP has been exited by xn_rtip_exit   */
RTP_EXTERN RTP_BOOL RTP_FAR rtip_exited;

/* set when resources initialized by xn_rtip_init (by RTIP) or by   */
/* pc_kernel_init (by ERTFS);                                       */
/* possible values are INIT_NOT_DONE, INIT_IN_PROG and INIT_DONE    */
RTP_EXTERN int RTP_FAR resource_initialized;

/* set when task templates and stack pool manager initialized by the   */
/* first call to ks_bind_task_index() or os_spawn_task()               */
RTP_EXTERN RTP_BOOL RTP_FAR kernel_initialized;

/* ********************************************************************   */
/* POLLOS.C                                                               */
/* ********************************************************************   */
#if (POLLOS)
    RTP_EXTERN int RTP_FAR poll_cycle_depth;  /* recurrsive depth of calls to poll_os_cycle() */

#ifdef INCLUDE_POLLOS_COOPERATIVE
	RTP_EXTERN int RTP_FAR pollos_task_id;
	RTP_EXTERN int RTP_FAR poll_cycle_reentry;  /* recurrsive depth of calls to poll_os_cycle() from threads */
#endif
    /* Keep track of when to call the 1 second timer function   */
    RTP_EXTERN RTP_UINT32 RTP_FAR next_sec;

    /* saved value of tick counter from previous call to poll_os_cycle   */
    /* (used for wrap checking)                                          */
    RTP_EXTERN RTP_UINT32 RTP_FAR last_tick;

#if (USE_POLLOS_TASK)
    /* root of tasks to run every second   */
    RTP_EXTERN PPOLLOS_TASK root_pollos_task;
#endif
#endif

#if (INCLUDE_RIP)
/* ********************************************************************   */
/* RIP                                                                    */
/* ********************************************************************   */
RTP_EXTERN RTP_UINT8 rip_buffer[RIP_MAX_PKTLEN];
RTP_EXTERN RTP_UINT8 rip_version_running;
RTP_EXTERN int  rip_udp_socket;
RTP_EXTERN int  rip_secs_to_update;
RTP_EXTERN int  rip_update_flag;
#if (POLLOS)
extern POLLOS_TASK pollos_rip_handle;
#endif
#endif // INCLUDE_RIP


/* ********************************************************************   */
/* TASK                                                                   */
/* ********************************************************************   */
/* Head of running timer chain.
 * The list of running timers is sorted in increasing order of expiration;
 * i.e., the first timer to expire is always at the head of the list.
 */
RTP_EXTERN PTIMER ebs_one_sec_timers;   /* multiple seconds timers */
RTP_EXTERN PTIMER ebs_timers;           /* every time timer task runs */

RTP_EXTERN int RTP_FAR timer_freq;  /* frequency timer task should run; set to CFG_TIMER_FREQ */
                        /* if there are any TCP sockets or 1 sec if no TCP sockets   */
                        /* are allocated                                             */
                        /* UNITS: msecs                                              */

/* the following data needs to be global for POLLOS only since the   */
/* POLLOS version of tc_timer_main is called every interval versus   */
/* looping forever while sleeping every loop                         */
#if (POLLOS)
	extern int  RTP_FAR timer_sec;
	extern int  RTP_FAR timer_to;
#endif
#if (INCLUDE_MEASURE_PERFORMANCE)
/* ********************************************************************   */
/* MEASURE PERFORMANCE PERFORMANCE                                        */
/* ********************************************************************   */
RTP_EXTERN RTP_BOOL do_sock_arp_cache;
RTP_EXTERN RTP_BOOL do_sock_udp_cache;
RTP_EXTERN RTP_BOOL do_sock_tcp_cache;
#endif

/* ********************************************************************   */
/* TASK MANAGEMENT                                                        */
/* ********************************************************************   */
/*  Flag that tells the timer task to call ks_kernel_timer_callback() on  */
/*  the next tick because the kernel layer needs to process a task delete */
/*  operation                                                             */
RTP_EXTERN int RTP_FAR ks_data_run_timer;

/* ********************************************************************   */
/* SERVERS                                                                */
/* ********************************************************************   */
#if (INCLUDE_BGET && INCLUDE_MALLOC)
RTP_EXTERN char RTP_FAR context_core[CFG_CONTEXT_CORE];
#endif

// ********************************************************************
// CALLBACKS - defined in rtipext.h
// ********************************************************************

#if (INCLUDE_SNMP || INCLUDE_DMH_SNMP)
/* ********************************************************************   */
/* MIB variables                                                          */
/* ********************************************************************   */

#if (INCLUDE_TRAPS)
RTP_EXTERN SEND_TRAP_FNC snmp_trap_fnc;
#endif

#if (!USE_OFFSET)
/* data structure containing all the mib variables   */
RTP_EXTERN struct mib_variables RTP_FAR mib_vars;
#endif

/* variables used to calculate MIB variables   */
RTP_EXTERN RTP_UINT32 RTP_FAR sys_up_time;
#endif /* INCLUDE_SNMP */


/* ********************************************************************   */
/* ********************************************************************   */
/* RTIPCONS.C - RTIP CONSTANT GLOBAL DATA                                 */
/* ********************************************************************   */
/* ********************************************************************   */

RTP_EXTERN RTP_CUINT8 RTP_FAR broadaddr[ETH_ALEN];
RTP_EXTERN RTP_CUINT8 RTP_FAR nulladdr[ETH_ALEN] ;

/* ip_ffaddr and ip_nulladdr are delcared in rtipapi.h   */

RTP_EXTERN RTP_CUINT8 RTP_FAR ip_lbmask[IP_ALEN] ;
#if (INCLUDE_IGMP || INCLUDE_IGMP_V2)
/* all hosts group multicast IP address   */
RTP_EXTERN RTP_CUINT8 RTP_FAR ip_igmp_all_hosts[IP_ALEN];
#if (INCLUDE_IGMP_V2)
/* all routers group multicast IP address   */
RTP_EXTERN RTP_CUINT8 RTP_FAR ip_igmp_all_routers[IP_ALEN];
#endif
#endif

#if (INCLUDE_BOOTP)
RTP_EXTERN RTP_CUINT8 RTP_FAR bootp_VM_RFC1048[5];
#endif

/* ********************************************************************   */
/* DEVICES                                                                */
/* ********************************************************************   */

RTP_EXTERN RTP_CONST EDEVTABLE default_device_entry;

/* ********************************************************************   */
/* OSPORT DATA                                                            */
/* ********************************************************************   */

#if (INCLUDE_802_2)
/* ********************************************************************   */
/* 802.2 DATA                                                             */
/* ********************************************************************   */
RTP_EXTERN RTP_CUINT8 llc_snap_data[LLC_SNAP_DATA_LEN];
#endif

/* ********************************************************************   */
/* TCP                                                                    */
/* ********************************************************************   */
#if (INCLUDE_TCP)

/* Array which contains tcp flags which should be sent in output packet     */
/* for each state                                                           */
/* NOTE: when enter FW2 no need to send pkt in response to other sides      */
/*       ack, therefore, for this case msg will not be sent; but while      */
/*       in FW2 can still be getting input pkts, therefore, need to ACK/ack */
/*       them                                                               */
RTP_EXTERN RTP_CUINT8 RTP_FAR tcp_flags_state[14];
#endif

/* ********************************************************************   */
/* UDP                                                                    */
/* ********************************************************************   */

#if (INCLUDE_SLIP || INCLUDE_CSLIP || INCLUDE_LOOP || RTPLATFORM)
/* ********************************************************************   */
/* LOOP, SLIP, CSLIP                                                      */
/* ********************************************************************   */
RTP_EXTERN RTP_CUINT8 RTP_FAR phony_en_addr[ETH_ALEN];
#endif

/* ********************************************************************   */
/* DRIVERS                                                                */
/* ********************************************************************   */

/* ********************************************************************   */
/* SPRINTF, DEBUG data                                                    */
/* ********************************************************************   */

RTP_EXTERN RTP_CCHAR RTP_FAR tc_hmap[17];

/* ********************************************************************   */
/* API data                                                               */
/* ********************************************************************   */
#if (INCLUDE_MODEM)
    RTP_EXTERN RTP_CCHAR RTP_FAR *modem_off_line;
    RTP_EXTERN RTP_CCHAR RTP_FAR *modem_ath0;
    RTP_EXTERN RTP_CCHAR RTP_FAR *modem_ok;
#endif

#if (INCLUDE_API_STR)
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_rarp_name[8];
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_bootp_name[9];
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_ping_name[8];
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_set_ip_name[10];
#if (INCLUDE_SLIP || INCLUDE_CSLIP || INCLUDE_PPP)
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_attach_name[10];
#endif
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_rt_add_name[10];
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_rt_del_name[10];
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_abort_name[9];
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_pkt_alloc_name[13];
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_pkt_data_max_name[16];
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_data_pointer_name[16];
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_pkt_data_size_name[17];
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_pkt_recv_name[29];
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_pkt_send_name[27];
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_interface_open_name[18];
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_interface_opt_name[17];
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_pkt_free_name[12];
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_tcp_is_connect_name[18];
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_tcp_is_read_name[15];
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_tcp_is_write_name[16];
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_getlasterror_name[16];
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_geterror_string_name[19];
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_arp_send_name[12];
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_arp_add_name[11];
    RTP_EXTERN RTP_CCHAR RTP_FAR xn_arp_del_name[11];
    RTP_EXTERN RTP_CCHAR RTP_FAR socket_name[7];
    RTP_EXTERN RTP_CCHAR RTP_FAR listen_name[7];
    RTP_EXTERN RTP_CCHAR RTP_FAR connect_name[8];
    RTP_EXTERN RTP_CCHAR RTP_FAR bind_name[5];
    RTP_EXTERN RTP_CCHAR RTP_FAR accept_name[7];
    RTP_EXTERN RTP_CCHAR RTP_FAR getsockopt_name[11];
    RTP_EXTERN RTP_CCHAR RTP_FAR setsockopt_name[11];
    RTP_EXTERN RTP_CCHAR RTP_FAR ioctlsocket_name[12];
    RTP_EXTERN RTP_CCHAR RTP_FAR select_name[7];
    RTP_EXTERN RTP_CCHAR RTP_FAR shutdown_name[9];
    RTP_EXTERN RTP_CCHAR RTP_FAR closesocket_name[12];
    RTP_EXTERN RTP_CCHAR RTP_FAR recv_name[14];
    RTP_EXTERN RTP_CCHAR RTP_FAR send_name[12];
    RTP_EXTERN RTP_CCHAR RTP_FAR getpeername_name[12];
    RTP_EXTERN RTP_CCHAR RTP_FAR getsockname_name[12];
#endif

/* ********************************************************************   */
/* SOCKET DATA                                                            */
/* ********************************************************************   */

#if (INCLUDE_DB)
/* database for getservbyname() and getservbyport()   */
RTP_EXTERN RTP_CONST struct servent RTP_FAR servs[NUM_SERVENT];

/* database for getprotobynumber() and getprotobyname()   */
RTP_EXTERN RTP_CONST struct protoent RTP_FAR protos[NUM_PROTOENT];
#endif

/* ********************************************************************   */
/* TOOLS.C                                                                */
/* ********************************************************************   */
#if (INCLUDE_ERRNO_STR)

RTP_EXTERN RTP_CONST ERRNO_STRINGS RTP_FAR error_strings[];

/* Pointer to structure; this is done this way due to avoid some compilers   */
/* giving you an error if you RTP_EXTERN an array without its size           */
RTP_EXTERN RTP_CONST ERRNO_STRINGS RTP_FAR *error_strings_ptr;
#endif

RTP_EXTERN RTP_PFCCHAR bad_errno_string;

#ifdef __cplusplus
}
#endif

#endif  /* __RTIPEXT__ */

