 /*
 | RTPNET.C - Runtime Platform Network Services
 |
 |   PORTED TO THE EBSnet Inc RTIP TCP/IP PLATFORM
 |
 | EBSnet - RT-Platform
 |
 |  $Author:  $
 |  $Date: 2005/02/23 16:20:00 $
 |  $Name:  $
 |  $Revision: 1.14 $
 |
 | Copyright EBSnet Inc. , 2003
 | All rights reserved.
 | This code may not be redistributed in source or linkable object form
 | without the consent of its author.
 |
 | Module description:
 |  [tbd]
*/



/************************************************************************
* Headers
************************************************************************/
#include "rtp.h"
#include "rtpnet.h"
#include "rtpmem.h"
#include "rtipext.h"
#include "rtpstr.h"
#include "rtpdebug.h"
#include "rtpscnv.h"
#include "rtpprint.h"
#include "net_decl.h"
#include "xnconf.h"
#include "rtipconf.h"
#include "rtipapi.h"
#include "socket.h"
#include "pollos.h"
#include "bget.h"
#include "rtpnet.h"
#include "dhcpcapi.h"
#include "rtpprint.h"
#include "rtip.h"
#include "udp.h"

#include <tinyhal.h>
#include <AsyncProcCalls_decl.h>

/************************************************************************
* ONLY HERE FOR TESTING - MUST REMOVE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
************************************************************************/
#ifdef RTP_DEBUG
void cb_wr_dbstring(int a, int b, int c, int d, char *e)
{
    NATIVE_PROFILE_PAL_NETWORK();
    RTP_DEBUG_OUTPUT_STR(e);
    RTP_DEBUG_OUTPUT_STR(".\n");
}
#endif

/************************************************************************
* Defines
************************************************************************/

/************************************************************************
* Compile Time Possible Porting Errors
************************************************************************/
#if (RTP_FD_SET_MAX > FD_SETSIZE)
#error RTP_FD_SET_MAX SHOULD NEVER BE LARGER THAN THE NATIVE FD_SETSIZE
#error         Adjustments should be made to RTP_FD_SET_MAX in rtpnet.h
#endif

/************************************************************************
* Types
************************************************************************/

/************************************************************************
* Data
************************************************************************/
//RTP_EXTERN CONTEXT_DATA RTP_FAR *ifaceCtx;
static UINT32              MemoryPool[NETWORK_MEMORY_POOL__SIZE/(sizeof(UINT32))];
extern HAL_CONTINUATION    IPTaskContinuation;
extern HAL_CONTINUATION    TimerTaskContinuation;
extern HAL_CONTINUATION    DHCPTaskContinuation;
extern HAL_COMPLETION      TimerTaskCompletion;

#if (INCLUDE_RUN_TIME_CONFIG)
RTP_THREAD  *ifaceThread;
RTP_THREAD  *interruptThread;
int         *ifaceIndexes;
#else
RTP_THREAD  ifaceThread[CFG_NIFACES];
RTP_THREAD  interruptThread[CFG_NIFACES];
int         ifaceIndexes[CFG_NIFACES];
#endif

RTP_THREAD  timerThread;

/************************************************************************
* Macros
************************************************************************/

/************************************************************************
* Function Prototypes
************************************************************************/
extern "C" {
extern void rtp_thrd_timer_continuation(void* arg);
extern void rtp_thrd_timer_completion(void* arg);
extern void rtp_thrd_ip_process(void* arg);
extern void rtp_thrd_ip_continuation(int index);
extern void rtp_tc_interrupt_process(void* arg);
#if (INCLUDE_RUN_TIME_CONFIG)
RTP_EXTERN INIT_FNCS RTP_FAR loop_fnc;      /* used to register LOOPBACK init fnc */
RTP_EXTERN INIT_FNCS RTP_FAR dhcp_fnc;      /* used to register DHCP init fnc */
#endif
}

#if (INCLUDE_RUN_TIME_CONFIG)
#if defined(NETWORK_USE_LOOPBACK)
extern void init_loopback(void);
#endif
#endif

/************************************************************************
* Function Bodies
************************************************************************/

// Helper class for dealing with copy of potentially mis-aligned addresses
template <class D,class S> __inline void CopyIPv4Addr(D* dest, S* source)
{
    NATIVE_PROFILE_PAL_NETWORK();
    ((unsigned char *)dest)[0] = ((unsigned char*)source)[0];
    ((unsigned char *)dest)[1] = ((unsigned char*)source)[1];
    ((unsigned char *)dest)[2] = ((unsigned char*)source)[2];
    ((unsigned char *)dest)[3] = ((unsigned char*)source)[3];
}


int rtp_init_completions()
{
    NATIVE_PROFILE_PAL_NETWORK();
    int ifaceCounter;

    GLOBAL_LOCK(irq);
    
#if (INCLUDE_RUN_TIME_CONFIG)
    ifaceThread = (RTP_THREAD  *) rtp_malloc(sizeof(RTP_THREAD) * CFG_NIFACES);
    interruptThread = (RTP_THREAD  *) rtp_malloc(sizeof(RTP_THREAD) * CFG_NIFACES);
    ifaceIndexes = (int *) rtp_malloc(sizeof(int) * CFG_NIFACES);
    
    if (((int) ifaceThread == -1) || ((int) interruptThread == -1) || ((int)ifaceIndexes == -1))
    {
        return(RTP_FALSE);
    }
#endif

    /* start the IP tasks   */
    for (ifaceCounter = 0; ifaceCounter < CFG_NIFACES; ifaceCounter++)
    {
        ifaceIndexes[ifaceCounter] = ifaceCounter;
    
        if (rtp_thread_spawn (
                &(ifaceThread[ifaceCounter]),
                rtp_thrd_ip_process,
                "ip interpret", 
                CFG_STACK_SIZE_NORMAL, 
                CFG_PRIO_INDEX_HIGHEST, 
                (void*)&(ifaceIndexes[ifaceCounter])) == -1)
        {
           
            RTP_DEBUG_ERROR("ks_resource_init - spawn IP task failed - IP task no = ", EBS_INT1, ifaceCounter, 0);
                
            return(RTP_FALSE);
        }
        
        /* spawn interrupt processing tasks   */
        if (rtp_thread_spawn (
                &(interruptThread[ifaceCounter]),
                (RTP_ENTRY_POINT_FN)rtp_tc_interrupt_process,
                "interrupt task", 
                CFG_STACK_SIZE_NORMAL, 
                CFG_PRIO_INDEX_HIGHEST, 
                (void*)&(ifaceIndexes[ifaceCounter])
            ) == -1)
        {
            RTP_DEBUG_ERROR("ks_resource_init - spawn INTERRUPT task failed - INTERRUPT task no = ", EBS_INT1, ifaceCounter, 0);

            return(RTP_FALSE);
        }
    }

    
    /* start the timer user-mode completion task   */
    if (rtp_thread_spawn (
            &timerThread,
            rtp_thrd_timer_completion,
            "timer comp", 
            CFG_STACK_SIZE_NORMAL, 
            CFG_PRIO_INDEX_HIGH, 
            0
        ) == -1)
    {
        RTP_DEBUG_ERROR("ks_resource_init - spawn timer task failed", NOVAR, 0, 0);

        return(RTP_FALSE);
    }
    
    /* start the timer user-mode completion task   */
    if (rtp_thread_spawn (
            &timerThread,
            rtp_thrd_timer_continuation,
            "timer cont", 
            CFG_STACK_SIZE_NORMAL, 
            CFG_PRIO_INDEX_HIGH, 
            0
        ) == -1)
    {
        RTP_DEBUG_ERROR("ks_resource_init - spawn timer task failed", NOVAR, 0, 0);

        return(RTP_FALSE);
    }


    return RTP_TRUE;
}


int rtp_uninit_completions()
{
    NATIVE_PROFILE_PAL_NETWORK();
    int ifaceCounter;

    GLOBAL_LOCK(irq);

    for (ifaceCounter = 0; ifaceCounter < CFG_NIFACES; ifaceCounter++)
    {
        /* stop the IP tasks   */
        if (rtp_thread_kill (
                &(ifaceThread[ifaceIndexes[ifaceCounter]]),
                rtp_thrd_ip_process,
                "ip interpret", 
                CFG_STACK_SIZE_NORMAL, 
                CFG_PRIO_INDEX_HIGHEST, 
                (void*)&(ifaceIndexes[ifaceCounter])) == -1)
        {
           
            RTP_DEBUG_ERROR("ks_resource_init - spawn IP task failed - IP task no = ", EBS_INT1, ifaceCounter, 0);
                
            return(RTP_FALSE);
        }
        
        /* stop interrupt processing tasks   */
        if (rtp_thread_kill (
                &(interruptThread[ifaceIndexes[ifaceCounter]]),
                (RTP_ENTRY_POINT_FN)rtp_tc_interrupt_process,
                "interrupt task", 
                CFG_STACK_SIZE_NORMAL, 
                CFG_PRIO_INDEX_HIGHEST, 
                (void*)&(ifaceIndexes[ifaceCounter])
            ) == -1)
        {
            RTP_DEBUG_ERROR("ks_resource_init - spawn INTERRUPT task failed - INTERRUPT task no = ", EBS_INT1, ifaceCounter, 0);

            return(RTP_FALSE);
        }
    }

    
    /* stop the timer user-mode completion task   */
    if (rtp_thread_kill (
            &timerThread,
            rtp_thrd_timer_completion,
            "timer comp", 
            CFG_STACK_SIZE_NORMAL, 
            CFG_PRIO_INDEX_HIGH, 
            0
        ) == -1)
    {
        RTP_DEBUG_ERROR("ks_resource_init - spawn timer task failed", NOVAR, 0, 0);

        return(RTP_FALSE);
    }
    
    /* start the timer user-mode completion task   */
    if (rtp_thread_kill (
            &timerThread,
            rtp_thrd_timer_continuation,
            "timer cont", 
            CFG_STACK_SIZE_NORMAL, 
            CFG_PRIO_INDEX_HIGH, 
            0
        ) == -1)
    {
        RTP_DEBUG_ERROR("ks_resource_init - spawn timer task failed", NOVAR, 0, 0);

        return(RTP_FALSE);
    }
    
#if (INCLUDE_RUN_TIME_CONFIG)
    rtp_free( ifaceThread );
    rtp_free( interruptThread );
    rtp_free( ifaceIndexes );
#endif

    return RTP_TRUE;
}

/*----------------------------------------------------------------------*
                              rtp_net_init
 *----------------------------------------------------------------------*/
int rtp_net_init (void)
{
    NATIVE_PROFILE_PAL_NETWORK();
    // no interrupts while initiliazing the stack
    GLOBAL_LOCK(irq); 
    
    int retVal;

    /*  These functions are usually registered by the stack in the
        normal build (INCLUDE_RUN_TIME_CONFIG = 0).  If INCLUDE_RUN_TIME_CONFIG is
        turned on, each add-on needs to be registered by the application
     */
#if (INCLUDE_RUN_TIME_CONFIG)
#if defined(NETWORK_USE_LOOPBACK)
    XN_REGISTER_LOOPBACK()
#endif
#if defined(NETWORK_USE_DHCP)
    XN_REGISTER_DHCP_CLI()
#endif
#endif
   
    bget_init();
    if(NULL == rtp_memset(MemoryPool, 0, NETWORK_MEMORY_POOL__SIZE*sizeof (unsigned char ))) return RTP_NET_EBADIFACE;
    bpool(&MemoryPool[0], NETWORK_MEMORY_POOL__SIZE);

    rtp_kern_init();
    
    rtp_threads_init();

    // Setup DHCP parameters
    cfg_dhcpc_data.dhcp_retries     = NETWORK_DHCP_RETRIES;
    cfg_dhcpc_data.dhcp_timeout     = NETWORK_DHCP_TIMEOUT;

    // Setup Interface parameters
    cfg_iface_data.mclistsize       = NETWORK_MULTICAST_LIST_SIZE;
    
#if defined(NETWORK_USE_LOOPBACK)
    cfg_iface_data.nifaces          = NETWORK_NUM_IFACES + 1;
#else
    cfg_iface_data.nifaces          = NETWORK_NUM_IFACES;
#endif
    
    cfg_iface_data.num_devices      = NETWORK_NUM_DEVICES;

    // Setup IGMP parameters
    cfg_igmp_data.igmpv1_max_delay  = NETWORK_IGMPV1_MAX_DELAY;

    // Setup IP parameters
    cfg_ip_data.frag_table_size     = NETWORK_FRAG_TABLE_SIZE;
    cfg_ip_data.rtsize              = NETWORK_ROUTINGTABLE_SIZE;

    // Setup DNS parameters
    cfg_dns_data.min_dns_delay      = NETWORK_DNS_MIN_DELAY;    
    cfg_dns_data.max_dns_delay      = NETWORK_DNS_MAX_DELAY;
    cfg_dns_data.dns_retries        = NETWORK_DNS_RETRIES;

    // Setup ARP parameters
    cfg_protocol_data.arpclen          = NETWORK_ARP_NUM_TABLE_ENTRIES;
    cfg_protocol_data.arpc_max_retries = NETWORK_ARP_MAX_RETRIES;
    cfg_protocol_data.arpc_req_timeout = NETWORK_ARP_REQ_TIMEOUT;
    cfg_protocol_data.arpc_res_timeout = NETWORK_ARP_RES_TIMEOUT;
    cfg_protocol_data.total_tcp_ports  = NETWORK_TCP_NUM_PORTS__SUPPORTED;
    cfg_protocol_data.total_udp_ports  = NETWORK_UDP_NUM_PORTS__SUPPORTED;
    cfg_protocol_data.timer_freq       = 20;

    // Setup Packet parameters
    cfg_packet_data.max_packetsize   = NETWORK_MAX_PACKETSIZE;
    cfg_packet_data.num_packets0     = NETWORK_PACKET_POOL_0__NUM_PACKETS;
    cfg_packet_data.num_packet_size0 = NETWORK_PACKET_POOL_0__PACKET_SIZE;
    cfg_packet_data.num_packets1     = NETWORK_PACKET_POOL_1__NUM_PACKETS;
    cfg_packet_data.num_packet_size1 = NETWORK_PACKET_POOL_1__PACKET_SIZE;
    cfg_packet_data.num_packets2     = NETWORK_PACKET_POOL_2__NUM_PACKETS;
    cfg_packet_data.num_packet_size2 = NETWORK_PACKET_POOL_2__PACKET_SIZE;
    cfg_packet_data.num_packets3     = NETWORK_PACKET_POOL_3__NUM_PACKETS;
    cfg_packet_data.num_packet_size3 = NETWORK_PACKET_POOL_3__PACKET_SIZE;
    cfg_packet_data.num_packets4     = NETWORK_PACKET_POOL_4__NUM_PACKETS;
    cfg_packet_data.num_packet_size4 = NETWORK_PACKET_POOL_4__PACKET_SIZE;
    cfg_packet_data.num_packets5     = NETWORK_PACKET_POOL_5__NUM_PACKETS;
    cfg_packet_data.num_packet_size5 = NETWORK_PACKET_POOL_5__PACKET_SIZE;


    // TODO: Add NAT parameters when EBSNet makes them available
     
    retVal = xn_rtip_init();
    
    if (retVal != -1)
    {
        retVal = rtp_init_completions() == RTP_TRUE? 0: -1;   
    }
    
    xn_udp_initialize();
    xn_tcp_initialize();

    debug_printf("Estimated Network RAM usage: %d (bytes)\r\n", NETWORK_RAM_SIZE_ESTIMATE());

    return (retVal);
}


/*----------------------------------------------------------------------*
                              rtp_net_exit
 *----------------------------------------------------------------------*/
void rtp_net_exit (void)
{
    NATIVE_PROFILE_PAL_NETWORK();
    GLOBAL_LOCK(irq); // no interrupts whuile initilizing the stack
    
    rtp_uninit_completions();
    
    xn_rtip_exit();

    rtp_threads_shutdown();

    rtp_kern_exit( 0 );
}


/*----------------------------------------------------------------------*
                        rtp_net_socket
 *----------------------------------------------------------------------*/
int rtp_net_socket (RTP_HANDLE  *sockHandle, int family, int type, int protocol)
{
    NATIVE_PROFILE_PAL_NETWORK();
    int sock;
    
    RTP_DEBUG_ERROR("SOCKET", 0, 0, 0);
#ifdef RTP_DEBUG
    int result;
    /* ----------------------------------- */
    /*  Clear the error state by setting   */
    /*  to 0.                              */
    /* ----------------------------------- */
    
#endif

    if (!family )
    {   
        *sockHandle = ((RTP_HANDLE)-1);
        set_errno(RTP_NET_EPFNOSUPPORT);
        return (-1);
    }
    
    if (!type)
    {
        *sockHandle = ((RTP_HANDLE)-1);
        set_errno(RTP_NET_EPROTONOSUPPORT);
        return (-1);
    }
    
    if (! (  (type == RTP_NET_SOCK_RAW) 
            || (type == RTP_NET_SOCK_STREAM && protocol == 6)
            || (type == RTP_NET_SOCK_DGRAM && protocol == 17)
           ) 
        )   
    {
        *sockHandle = ((RTP_HANDLE)-1);
        set_errno(RTP_NET_EPROTONOSUPPORT);
        return (-1);
    }
    
    sock = socket(family, type, protocol);

    if (sock < 0)
    {
        *sockHandle = ((RTP_HANDLE)-1);
        
        
#ifdef RTP_DEBUG
        result = xn_getlasterror();
        RTP_DEBUG_OUTPUT_STR("rtp_net_socket_datagram: error returned ");
        RTP_DEBUG_OUTPUT_INT(result);
        RTP_DEBUG_OUTPUT_STR(".\n");
#endif
        return (-1);
    }

    *sockHandle = (RTP_HANDLE) sock;

    return (0);
}

/*----------------------------------------------------------------------*
                            rtp_net_bind
 *----------------------------------------------------------------------*/
int rtp_net_bind (RTP_HANDLE sockHandle, unsigned char *ipAddr, int port, int type)
{
    NATIVE_PROFILE_PAL_NETWORK();
    int result;

    struct sockaddr_in sin;
    unsigned long in_addr = 0;

    rtp_memset(&sin, 0, sizeof (sin));

    if (ipAddr)
    {
        if (type == RTP_NET_TYPE_IPV4)
        {
            CopyIPv4Addr( &in_addr, ipAddr );
        }
        else
        {
            /* ----------------------------------- */
            /* RTP_NET_TYPE_IPV6 not yet supported */
            /* ----------------------------------- */
            return (-1);
        }
    }
    else
    {
        in_addr = INADDR_ANY;
    }

    sin.sin_family      = AF_INET;
    sin.sin_addr.s_addr = in_addr;
    sin.sin_port        = (unsigned short)port;

    
    if (bind ((int) sockHandle, (PSOCKADDR) &sin, sizeof (sin)) != 0)
    {
    	result = xn_getlasterror();
        if (result == EADDRINUSE)
        {
#ifdef RTP_DEBUG
			RTP_DEBUG_OUTPUT_STR("rtp_net_bind: non-fatal error returned ");
			RTP_DEBUG_OUTPUT_INT(result);
			RTP_DEBUG_OUTPUT_STR(".\n");
#endif
            return (-1);
        }
        else
        {
#ifdef RTP_DEBUG
			RTP_DEBUG_OUTPUT_STR("rtp_net_bind: error returned ");
			RTP_DEBUG_OUTPUT_INT(result);
			RTP_DEBUG_OUTPUT_STR(".\n");
#endif
        	return (-1);
        }
    }

    return (0);
}


/*----------------------------------------------------------------------*
                            rtp_net_listen
 *----------------------------------------------------------------------*/
int rtp_net_listen (RTP_HANDLE sockHandle, int queueSize)
{
    NATIVE_PROFILE_PAL_NETWORK();
#ifdef RTP_DEBUG
    int result;
    /* ----------------------------------- */
    /*  Clear the error state by setting   */
    /*  to 0.                              */
    /* ----------------------------------- */
    
#endif

    if (listen ((int) sockHandle, queueSize) != 0)
    {
#ifdef RTP_DEBUG
        result = xn_getlasterror();
        RTP_DEBUG_OUTPUT_STR("rtp_net_listen: error returned ");
        RTP_DEBUG_OUTPUT_INT(result);
        RTP_DEBUG_OUTPUT_STR(".\n");
#endif
        return (-1);
    }

    return (0);
}


/*----------------------------------------------------------------------*
                          rtp_net_getpeername
 *----------------------------------------------------------------------*/
int rtp_net_getpeername (RTP_HANDLE sockHandle, unsigned char *ipAddr, int *port, int *type)
{
    NATIVE_PROFILE_PAL_NETWORK();
    struct sockaddr_in peerAddr;
    int peerLen;
#ifdef RTP_DEBUG
    int result;
#endif

    peerLen = sizeof (peerAddr);
    rtp_memset(&peerAddr, 0, peerLen);

#ifdef RTP_DEBUG
    /* ----------------------------------- */
    /*  Clear the error state by setting   */
    /*  to 0.                              */
    /* ----------------------------------- */
    
#endif

    if (getpeername ((int) sockHandle, (PSOCKADDR) &peerAddr, (RTP_PFINT) &peerLen) != 0)
    {
#ifdef RTP_DEBUG
        result = xn_getlasterror();
        RTP_DEBUG_OUTPUT_STR("rtp_net_getpeername: error returned ");
        RTP_DEBUG_OUTPUT_INT(result);
        RTP_DEBUG_OUTPUT_STR(".\n");
#endif
        return (-1);
    }

    if (ipAddr)
    {
        CopyIPv4Addr( ipAddr, &(peerAddr.sin_addr.s_addr) );

        // make sure loopback is 127.0.0.1 (address is filtered by mask to 127.0.0.0).
        if(ipAddr[0] == 127 && ipAddr[1] == 0 && ipAddr[2] == 0 && ipAddr[3] == 0)
        {
            ipAddr[3] = 1;
        }
    }
    
    if (type) *type = RTP_NET_TYPE_IPV4;
    if (port) *port = peerAddr.sin_port;

    return (0);
}


/*----------------------------------------------------------------------*
                          rtp_net_getsockname
 *----------------------------------------------------------------------*/
int rtp_net_getsockname (RTP_HANDLE sockHandle, unsigned char *ipAddr, int *port, int *type)
{
    NATIVE_PROFILE_PAL_NETWORK();
    struct sockaddr_in localAddr;
    int localLen;
#ifdef RTP_DEBUG
    int result;
#endif

    localLen = sizeof (localAddr);
    rtp_memset(&localAddr, 0, localLen);

#ifdef RTP_DEBUG
    /* ----------------------------------- */
    /*  Clear the error state by setting   */
    /*  to 0.                              */
    /* ----------------------------------- */
    
#endif

    if (getsockname ((int) sockHandle, (PSOCKADDR) &localAddr, (RTP_PFINT) &localLen) != 0)
    {
#ifdef RTP_DEBUG
        result = xn_getlasterror();
        RTP_DEBUG_OUTPUT_STR("rtp_net_getsockname: error returned ");
        RTP_DEBUG_OUTPUT_INT(result);
        RTP_DEBUG_OUTPUT_STR(".\n");
#endif
        return (-1);
    }

    if (ipAddr)
    {
        CopyIPv4Addr( ipAddr, &(localAddr.sin_addr.s_addr) );

        // make sure loopback is 127.0.0.1 (address is filtered by mask to 127.0.0.0).
        if(ipAddr[0] == 127 && ipAddr[1] == 0 && ipAddr[2] == 0 && ipAddr[3] == 0)
        {
            ipAddr[3] = 1;
        }
    }

    if (type) *type   = RTP_NET_TYPE_IPV4;
    if (port) *port   = localAddr.sin_port;
    
    return (0);
}


/*----------------------------------------------------------------------*
                          rtp_net_gethostbyname
 *----------------------------------------------------------------------*/
int rtp_net_gethostbyname (unsigned char *ipAddr, int *type, char *name)
{
    NATIVE_PROFILE_PAL_NETWORK();
#if (INCLUDE_DNS)
#ifdef RTP_DEBUG
    int result;
#endif
    PFHOSTENT hp = 0;

#ifdef RTP_DEBUG
    /* ----------------------------------- */
    /*  Clear the error state by setting   */
    /*  to 0.                              */
    /* ----------------------------------- */
    
#endif

    hp = (PFHOSTENT)gethostbyname(name);
    if (!hp)
    {
#ifdef RTP_DEBUG
        result = xn_getlasterror();
        RTP_DEBUG_OUTPUT_STR("rtp_net_gethostbyname: error returned ");
        RTP_DEBUG_OUTPUT_INT(result);
        RTP_DEBUG_OUTPUT_STR(".\n");
#endif
        return (-1);
    }

    if (hp->h_addrtype != AF_INET)
    {
#ifdef RTP_DEBUG
        set_errno (EAFNOSUPPORT);
        result = xn_getlasterror();
        RTP_DEBUG_OUTPUT_STR("rtp_net_gethostbyname: error returned ");
        RTP_DEBUG_OUTPUT_INT(result);
        RTP_DEBUG_OUTPUT_STR(".\n");
#endif
        return (-1);
    }

    if (ipAddr) 
    {
        CopyIPv4Addr( ipAddr, hp->h_addr_list[0] );

        // make sure loopback is 127.0.0.1 (address is filtered by mask to 127.0.0.0).
        if(ipAddr[0] == 127 && ipAddr[1] == 0 && ipAddr[2] == 0 && ipAddr[3] == 0)
        {
            ipAddr[3] = 1;
        }
    }
    
    if (type) *type = RTP_NET_TYPE_IPV4;

    return (0);
#else

    rtp_not_yet_implemented();
    return (-1);
#endif /* INCLUDE_DNS */
}


/*----------------------------------------------------------------------*
                           rtp_net_accept
 *----------------------------------------------------------------------*/
int rtp_net_accept (RTP_HANDLE *connectSock, RTP_HANDLE serverSock,
                    unsigned char *ipAddr, int *port, int *type)
{
    struct sockaddr_in  clientAddr;
    int                 clientLen;
    int                 conSocket;
    int                 result;

    clientLen = sizeof (clientAddr);
    rtp_memset(&clientAddr, 0, clientLen);

    conSocket = accept ((int) serverSock, (PSOCKADDR) &clientAddr, (RTP_PFINT) &clientLen);

    if (conSocket < 0)
    {
        *connectSock = ((RTP_HANDLE)-1);
        result = xn_getlasterror();
        if ((result == EINPROGRESS) || (result == EWOULDBLOCK))
        {
#ifdef RTP_DEBUG
			RTP_DEBUG_OUTPUT_STR("rtp_net_accept: non-fatal error returned ");
			RTP_DEBUG_OUTPUT_INT(result);
			RTP_DEBUG_OUTPUT_STR(".\n");
#endif
            return (-1);
        }
#ifdef RTP_DEBUG
		RTP_DEBUG_OUTPUT_STR("rtp_net_accept: error returned ");
		RTP_DEBUG_OUTPUT_INT(result);
		RTP_DEBUG_OUTPUT_STR(".\n");
#endif
        return (-1);
    }

    *connectSock = (RTP_HANDLE)conSocket;

    if (ipAddr)
    {
        CopyIPv4Addr( ipAddr, &(clientAddr.sin_addr.s_addr) );

        // make sure loopback is 127.0.0.1 (address is filtered by mask to 127.0.0.0).
        if(ipAddr[0] == 127 && ipAddr[1] == 0 && ipAddr[2] == 0 && ipAddr[3] == 0)
        {
            ipAddr[3] = 1;
        }
    }
    if (type) *type = RTP_NET_TYPE_IPV4;
    if (port) *port = clientAddr.sin_port;

    return (0);
}


/*----------------------------------------------------------------------*
                          rtp_net_connect
 *----------------------------------------------------------------------*/
int rtp_net_connect (RTP_HANDLE sockHandle,
                     unsigned char *ipAddr,
                     int port, int type)
{
    int result;
    int sinLen;
    struct sockaddr_in sin;
    unsigned long in_addr = 0;

    RTP_DEBUG_ERROR("CONNECT", 0, 0, 0);

    sinLen = sizeof (sin);
    rtp_memset(&sin, 0, sinLen);

    if (ipAddr)
    {
        if (type != RTP_NET_TYPE_IPV4)
        {
            return (-1);
        }
        else
        {
            CopyIPv4Addr( &in_addr, ipAddr );
        }
    }
    else
    {
        /* invalid address */
        return (-1);
    }

    sin.sin_family      = AF_INET;
    sin.sin_addr.s_addr = in_addr;
    sin.sin_port        = (unsigned short)port;

    
    if (connect ((int) sockHandle, (PSOCKADDR) &sin, sinLen) != 0)
    {
        result = xn_getlasterror();
        if ((result == EINPROGRESS) ||
            (result == EWOULDBLOCK))
        {
#ifdef RTP_DEBUG
			RTP_DEBUG_OUTPUT_STR("rtp_net_connect: non-fatal error returned ");
			RTP_DEBUG_OUTPUT_INT(result);
			RTP_DEBUG_OUTPUT_STR(".\n");
#endif
            return (-1);
        }
#ifdef RTP_DEBUG
		RTP_DEBUG_OUTPUT_STR("rtp_net_connect: error returned ");
		RTP_DEBUG_OUTPUT_INT(result);
		RTP_DEBUG_OUTPUT_STR(".\n");
#endif
        return (-1);
    }

    return (0);
}


/*----------------------------------------------------------------------*
                         rtp_net_is_connected
 *----------------------------------------------------------------------*/
unsigned rtp_net_is_connected (RTP_SOCKET sockHandle)
{
    NATIVE_PROFILE_PAL_NETWORK();
    struct sockaddr_in peerAddr;
    int peerLen;

    peerLen = sizeof (peerAddr);
    rtp_memset(&peerAddr, 0, peerLen);

    return (getpeername ((int)sockHandle, (PSOCKADDR)&peerAddr, (RTP_PFINT)&peerLen) == 0);
}


/*----------------------------------------------------------------------*
                        rtp_net_write_select
 *----------------------------------------------------------------------*/
int rtp_net_write_select (RTP_HANDLE sockHandle, long msecTimeout)
{
    NATIVE_PROFILE_PAL_NETWORK();
struct timeval selectTime;
fd_set write_set;
int result;

    /* ----------------------------------- */
    /*              write list             */
    /* ----------------------------------- */
    FD_ZERO(&write_set);
    FD_SET(sockHandle, &write_set);

#ifdef RTP_DEBUG
    /* ----------------------------------- */
    /*  Clear the error state by setting   */
    /*  to 0.                              */
    /* ----------------------------------- */
    set_errno (0);
#endif

    if (msecTimeout >= 0)
    {
        selectTime.tv_sec = msecTimeout / 1000;
        selectTime.tv_usec = (msecTimeout % 1000) * 1000;
        result = select(1, (PFDSET) 0, (PFDSET) &write_set, (PFDSET) 0, (PCTIMEVAL) &selectTime);
    }
    else
    {
        result = select(1, (PFDSET) 0, (PFDSET) &write_set, (PFDSET) 0, (PCTIMEVAL) NULL);
    }

 /*   if (result == 1)
 |    {
 |        if (!xn_tcp_is_write(sockHandle))
 |        {
 |            result = (-1);
 |        }
 |    }
*/
    /* if an error or if it timed out */
    if (result <= 0)
    {
#ifdef RTP_DEBUG
        result = xn_getlasterror();
        RTP_DEBUG_OUTPUT_STR("rtp_net_write_select: error returned ");
        RTP_DEBUG_OUTPUT_INT(result);
        RTP_DEBUG_OUTPUT_STR(".\n");
#endif
        return (-1);
    }

    return (0);
}


/*----------------------------------------------------------------------*
                        rtp_net_read_select
 *----------------------------------------------------------------------*/
int rtp_net_read_select (RTP_HANDLE sockHandle, long msecTimeout)
{
    NATIVE_PROFILE_PAL_NETWORK();
struct timeval selectTime;
fd_set read_set;
int result;

    /* ----------------------------------- */
    /*              read list              */
    /* ----------------------------------- */
    FD_ZERO(&read_set);
    FD_SET(sockHandle, &read_set);

#ifdef RTP_DEBUG
    /* ----------------------------------- */
    /*  Clear the error state by setting   */
    /*  to 0.                              */
    /* ----------------------------------- */
    set_errno (0);
#endif

    if (msecTimeout >= 0)
    {
        selectTime.tv_sec = msecTimeout / 1000;
        selectTime.tv_usec = (msecTimeout % 1000) * 1000;
        result = select(1, (PFDSET) &read_set, (PFDSET) 0, (PFDSET) 0, (PCTIMEVAL) &selectTime);
    }
    else
    {
        result = select(1, (PFDSET) &read_set, (PFDSET) 0, (PFDSET) 0, (PCTIMEVAL) NULL);
    }

 /*   if (result == 1)
 |    {
 |        if (!xn_tcp_is_read(sockHandle))
 |        {
 |            result = (-1);
 |        }
 |    }
*/
    /* if an error or if it timed out */
    if (result <= 0)
    {
#ifdef RTP_DEBUG
        result = xn_getlasterror();
        RTP_DEBUG_OUTPUT_STR("rtp_net_read_select: error returned ");
        RTP_DEBUG_OUTPUT_INT(result);
        RTP_DEBUG_OUTPUT_STR(".\n");
#endif
        return (-1);
    }

    return (0);
}


/*----------------------------------------------------------------------*
                           rtp_net_send
 *----------------------------------------------------------------------*/
long rtp_net_send (RTP_HANDLE sockHandle, const unsigned char * buffer, long size, int flags)
{
    NATIVE_PROFILE_PAL_NETWORK();
    long result;
    PANYPORT pport;
    
    pport = api_sock_to_port(sockHandle);
    if (!pport)
        return(set_errno(ENOTSOCK));
    
    if ( (flags != 0) && 
           ((pport->port_type == TCPPORTTYPE) && (flags != MSG_QUEUE)) )
    {
        set_errno(EOPNOTSUPPORT);
        return -1;
    }
    
    result = (long) send((int) sockHandle, (RTP_CONST char *) buffer, (int) size, flags);

    if (result < 0)
    {
        result = (long) xn_getlasterror();
        if ((result == EINPROGRESS) || (result == EWOULDBLOCK))
        {
			RTP_DEBUG_OUTPUT_STR("rtp_net_send: non-fatal error returned ");
			RTP_DEBUG_OUTPUT_INT(result);
			RTP_DEBUG_OUTPUT_STR(".\n");
            return (-1);
        }
		RTP_DEBUG_OUTPUT_STR("rtp_net_send: error returned ");
		RTP_DEBUG_OUTPUT_INT(result);
		RTP_DEBUG_OUTPUT_STR(".\n");
        return (-1);
    }
    return (result);
}


/*----------------------------------------------------------------------*
                           rtp_net_recv
 *----------------------------------------------------------------------*/
long rtp_net_recv (RTP_HANDLE sockHandle, unsigned char * buffer, long size, int flag)
{
    NATIVE_PROFILE_PAL_NETWORK();
    long        result;
    PANYPORT    pport;
    
    pport = api_sock_to_port(sockHandle);
    if (!pport)
        return(set_errno(ENOTSOCK));
    
    if ( (flag != 0) &&  (flag != MSG_PEEK) )
    {
        set_errno(EOPNOTSUPPORT);
        return -1;
    }
     
    result = (long) recv((int) sockHandle, (char *) buffer, (int) size, flag);

    if (result < 0)
    {
        result = (long) xn_getlasterror();
        if ((result == EINPROGRESS) || (result == EWOULDBLOCK))
        {
        	RTP_DEBUG_OUTPUT_STR("rtp_net_recv: non-fatal error returned ");
        	RTP_DEBUG_OUTPUT_INT(result);
        	RTP_DEBUG_OUTPUT_STR(".\n");
            return (-1);
        }
        RTP_DEBUG_OUTPUT_STR("rtp_net_recv: error returned ");
        RTP_DEBUG_OUTPUT_INT(result);
        RTP_DEBUG_OUTPUT_STR(".\n");
        return (-1);
    }
    return (result);
}


/*----------------------------------------------------------------------*
                           rtp_net_sendto
 *----------------------------------------------------------------------*/
long rtp_net_sendto (RTP_HANDLE sockHandle,
                     const unsigned char * buffer, long size,
                     unsigned char * ipAddr, int port, int type, 
                     int flags)
{
    long        result;
    int         sinLen;
    struct sockaddr_in sin;
    unsigned long in_addr = 0;
    PANYPORT    pport;
    
    
    pport = api_sock_to_port(sockHandle);
    if (!pport)
        return(set_errno(ENOTSOCK));
        
    
    sinLen = sizeof (sin);
    rtp_memset(&sin, 0, sinLen);

    
    if ( (flags != 0) && 
           ((pport->port_type == TCPPORTTYPE) && (flags != MSG_QUEUE)) )
    {
        set_errno(EOPNOTSUPPORT);
        return -1;
    }
    
    if (ipAddr)
    {
        if (type != RTP_NET_TYPE_IPV4)
        {
            set_errno(ENOTSOCK);
            return (-1);
        }

        CopyIPv4Addr( &in_addr, ipAddr );
    }
    else
    {
        /* invalid address */
        set_errno(EINVAL);
        return (-1);
    }

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = in_addr;
    sin.sin_port = (unsigned short)port;

    /* ----------------------------------- */
    /*  Clear the error state by setting   */
    /*  to 0.                              */
    /* ----------------------------------- */
    

    result = (long) sendto((int) sockHandle, (RTP_PFCCHAR) buffer, (int) size, flags, (PCSOCKADDR)&sin, sinLen);

    if (result < 0)
    {
        result = (long) xn_getlasterror();
        if ((result == EINPROGRESS) || (result == EWOULDBLOCK))
        {
        	RTP_DEBUG_OUTPUT_STR("rtp_net_sendto: non-fatal error returned ");
        	RTP_DEBUG_OUTPUT_INT(result);
        	RTP_DEBUG_OUTPUT_STR(".\n");
            return (-1);
        }
        RTP_DEBUG_OUTPUT_STR("rtp_net_sendto: error returned ");
        RTP_DEBUG_OUTPUT_INT(result);
        RTP_DEBUG_OUTPUT_STR(".\n");
        return (-1);
    }
    return (result);
}


/*----------------------------------------------------------------------*
                           rtp_net_recvfrom
 *----------------------------------------------------------------------*/
long rtp_net_recvfrom (RTP_HANDLE sockHandle,
                       unsigned char *buffer, long size,
                       unsigned char *ipAddr, int *port, int *type, int flags)
{
    long        result;
    int         remoteLen;
    struct sockaddr_in remote;
    PANYPORT    pport;
    
    pport = api_sock_to_port(sockHandle);
    
    if (!pport)
        return(set_errno(ENOTSOCK));
    
    if ( (flags != 0) &&  (flags != MSG_PEEK) )
    {
        set_errno(EOPNOTSUPPORT);
        return -1;
    }
    
    remoteLen = sizeof (remote);
    rtp_memset (&remote, 0, remoteLen);

    result = (long) recvfrom ((int) sockHandle, (RTP_PFCHAR) buffer, (int) size, flags, (PSOCKADDR) &remote, (RTP_PFINT) &remoteLen);

    if (result < 0)
    {
        result = (long) xn_getlasterror();
        if ((result == EINPROGRESS) || (result == EWOULDBLOCK))
        {
        	RTP_DEBUG_OUTPUT_STR("rtp_net_recvfrom: non-fatal error returned ");
        	RTP_DEBUG_OUTPUT_INT(result);
        	RTP_DEBUG_OUTPUT_STR(".\n");
            return (-1);
        }
        RTP_DEBUG_OUTPUT_STR("rtp_net_recvfrom: error returned ");
        RTP_DEBUG_OUTPUT_INT(result);
        RTP_DEBUG_OUTPUT_STR(".\n");
        return (-1);
    }

    if (ipAddr)
    {
        CopyIPv4Addr( ipAddr, &(remote.sin_addr.s_addr) );

        // make sure loopback is 127.0.0.1 (address is filtered by mask to 127.0.0.0).
        if(ipAddr[0] == 127 && ipAddr[1] == 0 && ipAddr[2] == 0 && ipAddr[3] == 0)
        {
            ipAddr[3] = 1;
        }
    }
    if (type) *type = RTP_NET_TYPE_IPV4;
    if (port) *port = remote.sin_port;

    return (result);
}


/*----------------------------------------------------------------------*
                          rtp_net_closesocket
 *----------------------------------------------------------------------*/
int rtp_net_closesocket (RTP_HANDLE sockHandle)
{
    NATIVE_PROFILE_PAL_NETWORK();
#ifdef RTP_DEBUG
    int result;
    /* ----------------------------------- */
    /*  Clear the error state by setting   */
    /*  to 0.                              */
    /* ----------------------------------- */
    set_errno (0);
#endif

    if (closesocket((int) sockHandle) != 0)
    {
#ifdef RTP_DEBUG
        result = xn_getlasterror();
        RTP_DEBUG_OUTPUT_STR("rtp_net_closesocket: error returned ");
        RTP_DEBUG_OUTPUT_INT(result);
        RTP_DEBUG_OUTPUT_STR(".\n");
#endif
        return (-1);
    }
    return (0);
}


/*----------------------------------------------------------------------*
                            rtp_net_shutdown
 *----------------------------------------------------------------------*/
int rtp_net_shutdown (RTP_HANDLE sockHandle, int how)
{
    NATIVE_PROFILE_PAL_NETWORK();
#ifdef RTP_DEBUG
    int result;
    /* ----------------------------------- */
    /*  Clear the error state by setting   */
    /*  to 0.                              */
    /* ----------------------------------- */
    set_errno (0);
#endif

    if (shutdown((int) sockHandle, how) != 0)
    {
#ifdef RTP_DEBUG
        result = xn_getlasterror();
        RTP_DEBUG_OUTPUT_STR("rtp_net_shutdown: error returned ");
        RTP_DEBUG_OUTPUT_INT(result);
        RTP_DEBUG_OUTPUT_STR(".\n");
#endif
        return (-1);
    }
    return (0);
}

/*----------------------------------------------------------------------*
                           rtp_net_getlasterror
 *----------------------------------------------------------------------*/
int      rtp_net_getlasterror    (void)
{
    NATIVE_PROFILE_PAL_NETWORK();
    int ret;
    ret = xn_getlasterror();
    
#ifdef RTP_DEBUG
    
#endif
    return(ret);
}
/*----------------------------------------------------------------------*
                           rtp_net_getntoread
 *----------------------------------------------------------------------*/
int rtp_net_getntoread (RTP_HANDLE sockHandle, unsigned long * nToRead)
{
    NATIVE_PROFILE_PAL_NETWORK();
    unsigned long arg;

#ifdef RTP_DEBUG
    int result;
    /* ----------------------------------- */
    /*  Clear the error state by setting   */
    /*  to 0.                              */
    /* ----------------------------------- */
    set_errno (0);
#endif

    if (ioctlsocket((int) sockHandle, FIONREAD, &arg) != 0)
    {
#ifdef RTP_DEBUG
        result = xn_getlasterror();
        RTP_DEBUG_OUTPUT_STR("rtp_net_getntoread: error returned ");
        RTP_DEBUG_OUTPUT_INT(result);
        RTP_DEBUG_OUTPUT_STR(".\n");
#endif
        return (-1);
    }
    *nToRead = arg;
    return (0);
}

/*----------------------------------------------------------------------*
                           rtp_net_setsockoopt
 *----------------------------------------------------------------------*/
int      rtp_net_setsockoopt (RTP_HANDLE sockHandle, int level, int optionName, 
                                    RTP_PFCCHAR optionVal, int length)
{
    int     ret;
    LINGER  linger;
    RTP_UINT32 win_size32;
    RTP_UINT16 win_size16;
    struct timeval timeout;
    int millisecs;
    PANYPORT pport;
    
    if (    (level == RTP_NET_SOL_SOCKET) &&
            (optionName == RTP_NET_IP_OPTS_BROADCAST)  )
    {
        pport = api_sock_to_port(sockHandle);
        if (pport && pport->port_type == TCPPORTTYPE)
        {
            set_errno(SOCK_ENOPROTOOPT);
            return SOCK_SOCKET_ERROR;
        }
    }
    
    if (    (level == RTP_NET_SOL_SOCKET) &&
            (optionName == RTP_NET_SO_LINGER) )
    {
        if(length != sizeof(int)) 
        {
            set_errno(EFAULT);
            return SOCK_SOCKET_ERROR;
        }
        
        int value = *((int*)optionVal);

        if(value == -1) 
        {
            // graceful block
            linger.l_onoff  = 1;
            linger.l_linger = 1;
        }
        else if(value == -2)
        {
            // Linger OFF
            // graceful no block
            linger.l_onoff  = 0;
            linger.l_linger = 1;
        }
        else
        {
            // graceful block OR
            // hard block (with value 0)
            // Linger ON with timeout
            linger.l_onoff = 1;
            linger.l_linger = value;            
        }

        optionVal = (RTP_PFCCHAR)&linger;
        length = sizeof(LINGER);
    }
    /* JRT */
    else if ( (optionName == RTP_NET_SO_RECEIVE_BUFFER) ||
              (optionName == RTP_NET_SO_SEND_BUFFER) )
    {
        if (length != 4)
        {
            set_errno(EFAULT);
            return (-1);
        }

        pport = api_sock_to_port(sockHandle);
        if (!pport)
              return(set_errno(ENOTSOCK));
              
        rtp_memcpy(&win_size32, optionVal, 4);
    
        if (pport->port_type == TCPPORTTYPE)
        {
            if (win_size32 > 0xFFFF)
            {
                set_errno(EFAULT);
                return (-1);
            }
        
            win_size16 = (RTP_UINT16) win_size32;
            length = sizeof(RTP_UINT16);
            optionVal = (RTP_PFCCHAR)&win_size16;
            level = RTP_NET_SOL_SOCKET;
        }
        else if (pport->port_type == UDPPORTTYPE)
        {
            length = sizeof(RTP_UINT32);
            optionVal = (RTP_PFCCHAR)&win_size32;
            level = RTP_NET_SOL_SOCKET;
        }
    }
    /* JRT */
    else if ( (optionName == RTP_NET_SO_RCV_TIMEO) ||
              (optionName == RTP_NET_SO_SEND_TIMEO) )
    {
        length = sizeof(struct timeval);
        
        millisecs       = (*(int*)optionVal) % 1000;
        timeout.tv_usec = millisecs * 1000;
        timeout.tv_sec  = (*(int*)optionVal) / 1000;

        optionVal = (RTP_PFCCHAR)&timeout;
    } 
    else if ((level == RTP_NET_IPROTO_TCP) &&
            (optionName == RTP_NET_SO_NAGLE) )
    {
        int set_nagle = 0;
        
        if (!(*optionVal))
        {
            set_nagle = 1;        
        }

        optionVal = (RTP_PFCCHAR)&set_nagle;
    }
    
    ret = setsockopt(sockHandle, level, optionName, optionVal, length);
    
    return ret;   
    
}

/*----------------------------------------------------------------------*
                           rtp_net_getsockopt
 *----------------------------------------------------------------------*/
int      rtp_net_getsockopt  (RTP_HANDLE sockHandle, int level, int optionName, 
                              RTP_PFCHAR optionVal, int* optlen )
{    
    int ret = SOCK_SOCKET_ERROR;
    int optionLength;
    PANYPORT pport;
    
    if (    (level == RTP_NET_SOL_SOCKET) &&
            (optionName == RTP_NET_IP_OPTS_BROADCAST)  )
    {
        pport = api_sock_to_port(sockHandle);
        if (pport && pport->port_type == TCPPORTTYPE)
        {
            set_errno(SOCK_ENOPROTOOPT);
            return SOCK_SOCKET_ERROR;
        }
    }
    
    if ( (level      == RTP_NET_SOL_SOCKET) &&
         (optionName == RTP_NET_SO_LINGER))
    {
        /* JRT - linger not a pointer */
        LINGER      linger;

        optionLength = sizeof(LINGER);
        ret = getsockopt(sockHandle, level, optionName, (RTP_PFCHAR) &linger, &optionLength);

        *((int*)optionVal) = linger.l_linger;
        *optlen = sizeof(int);
    }         
    else if ( (optionName == RTP_NET_SO_RCV_TIMEO) ||
              (optionName == RTP_NET_SO_SEND_TIMEO) )
    {
        /* JRT - SO_RCV_TIMEO, SO_SEND_TIMEO take timeval struct */
        struct      timeval timeout;

        optionLength = sizeof(struct timeval);
        ret = getsockopt(sockHandle, level, optionName, (RTP_PFCHAR) &timeout, &optionLength);

        /* Convert the timeval values to milliseconds */
        *(int*)optionVal =  (int)((timeout.tv_sec * 1000) + (timeout.tv_usec / 1000));
    } 
    /* JRT */
    else if ( (optionName == RTP_NET_SO_RECEIVE_BUFFER) ||
              (optionName == RTP_NET_SO_SEND_BUFFER) )
    {        
        if (*optlen != 4)
        {
            set_errno(EFAULT);
            return (-1);
        }

        RTP_PFCHAR window_size;
        RTP_UINT16 window_size16;
 
        pport = api_sock_to_port(sockHandle);
        if (!pport)
              return(set_errno(ENOTSOCK));       
        
        if (pport->port_type == TCPPORTTYPE)
        {
            optionLength = sizeof(RTP_UINT16);
            window_size  = (RTP_PFCHAR)&window_size16;
        }
        else //if (pport->port_type == UDPPORTTYPE)
        {
            optionLength = sizeof(RTP_UINT32);
            window_size  = (RTP_PFCHAR)optionVal;
        }
        

        level = RTP_NET_SOL_SOCKET;
            
        ret = getsockopt(sockHandle, level, optionName, window_size, &optionLength);

        if (pport->port_type == TCPPORTTYPE)
        {
            *(RTP_UINT32*)optionVal = window_size16;
        }
        
    } 
    else if ((level == RTP_NET_IPROTO_TCP) &&
             (optionName == RTP_NET_SO_NAGLE) )
    {
        ret = getsockopt(sockHandle, level, optionName, optionVal, &optionLength);
  
        *optionVal = !(*optionVal);
    }             
    else
    {
        optionLength = *optlen;
        ret = getsockopt(sockHandle, level, optionName, optionVal, &optionLength);
    }
    
    return ret;
}
/*----------------------------------------------------------------------*
                           rtp_net_setblocking
 *----------------------------------------------------------------------*/
int rtp_net_setblocking (RTP_HANDLE sockHandle, unsigned int onBool)
{
    NATIVE_PROFILE_PAL_NETWORK();
    unsigned long arg;

#ifdef RTP_DEBUG
    
#endif

    arg = (unsigned long)(!onBool);
    if (ioctlsocket((int) sockHandle, FIONBIO, &arg) != 0)
    {
#ifdef RTP_DEBUG
        result = xn_getlasterror();
        RTP_DEBUG_OUTPUT_STR("rtp_net_setblocking: error returned ");
        RTP_DEBUG_OUTPUT_INT(result);
        RTP_DEBUG_OUTPUT_STR(".\n");
#endif
        return (-1);
    }
    return (0);
}


/*----------------------------------------------------------------------*
                            rtp_net_setnagle
 *----------------------------------------------------------------------*/
int rtp_net_setnagle (RTP_HANDLE sockHandle, unsigned int onBool)
{
    NATIVE_PROFILE_PAL_NETWORK();
    int option;

#ifdef RTP_DEBUG
    int result;
    /* ----------------------------------- */
    /*  Clear the error state by setting   */
    /*  to 0.                              */
    /* ----------------------------------- */
    set_errno (0);
#endif

    option = (int) onBool;
    if (setsockopt((int) sockHandle, SOL_SOCKET,
                    SO_NAGLE, (RTP_PFCCHAR) &option,
                    sizeof (int)) != 0)
    {
#ifdef RTP_DEBUG
        result = xn_getlasterror();
        RTP_DEBUG_OUTPUT_STR("rtp_net_setnagle: error returned ");
        RTP_DEBUG_OUTPUT_INT(result);
        RTP_DEBUG_OUTPUT_STR(".\n");
#endif
        return (-1);
    }
    return (0);
}


/*----------------------------------------------------------------------*
                          rtp_net_setlinger
 *----------------------------------------------------------------------*/
int rtp_net_setlinger (RTP_HANDLE sockHandle, unsigned int onBool, long msecTimeout)
{
    NATIVE_PROFILE_PAL_NETWORK();
    LINGER arg;
#ifdef RTP_DEBUG
    int result;
#endif

    arg.l_onoff = onBool;
    arg.l_linger = 0;
    if (arg.l_onoff)
    {
	    if (msecTimeout > 0)
	    {
	        arg.l_linger = (int) (msecTimeout / 1000);
	    }
    }

#ifdef RTP_DEBUG
    /* ----------------------------------- */
    /*  Clear the error state by setting   */
    /*  to 0.                              */
    /* ----------------------------------- */
    set_errno (0);
#endif

    if (setsockopt((int) sockHandle, SOL_SOCKET,
                    SO_LINGER, (RTP_PFCCHAR) &arg,
                    sizeof (struct linger)) != 0)
    {
#ifdef RTP_DEBUG
        result = xn_getlasterror();
        RTP_DEBUG_OUTPUT_STR("rtp_net_setlinger: error returned ");
        RTP_DEBUG_OUTPUT_INT(result);
        RTP_DEBUG_OUTPUT_STR(".\n");
#endif
        return (-1);
    }
    return (0);
}


/*----------------------------------------------------------------------*
                           rtp_net_setreusesock
 *----------------------------------------------------------------------*/
int rtp_net_setreusesock (RTP_HANDLE sockHandle, unsigned int onBool)
{
    NATIVE_PROFILE_PAL_NETWORK();

#ifdef RTP_DEBUG
    int result;
    /* ----------------------------------- */
    /*  Clear the error state by setting   */
    /*  to 0.                              */
    /* ----------------------------------- */
    set_errno (0);
#endif

    if ( setsockopt((int) sockHandle,
                    SOL_SOCKET,
                    SO_REUSESOCK,
                    (RTP_PFCCHAR) &onBool,
                    sizeof (int)) != 0 )
    {
#ifdef RTP_DEBUG
        result = xn_getlasterror();
        RTP_DEBUG_OUTPUT_STR("rtp_net_setreusesock: error returned ");
        RTP_DEBUG_OUTPUT_INT(result);
        RTP_DEBUG_OUTPUT_STR(".\n");
#endif
        return (-1);
    }
    return (0);
}


/*----------------------------------------------------------------------*
                           rtp_net_setreuseaddr
 *----------------------------------------------------------------------*/
int rtp_net_setreuseaddr (RTP_HANDLE sockHandle, unsigned int onBool)
{
    NATIVE_PROFILE_PAL_NETWORK();

#ifdef RTP_DEBUG
    int result;
    /* ----------------------------------- */
    /*  Clear the error state by setting   */
    /*  to 0.                              */
    /* ----------------------------------- */
    set_errno (0);
#endif

    if ( setsockopt((int) sockHandle,
                    SOL_SOCKET,
                    SO_REUSEADDR,
                    (RTP_PFCCHAR) &onBool,
                    sizeof (int)) != 0 )
    {
#ifdef RTP_DEBUG
        result = xn_getlasterror();
        RTP_DEBUG_OUTPUT_STR("rtp_net_setreuseaddr: error returned ");
        RTP_DEBUG_OUTPUT_INT(result);
        RTP_DEBUG_OUTPUT_STR(".\n");
#endif
        return (-1);
    }
    return (0);
}

/*----------------------------------------------------------------------*
                           rtp_net_settcpnocopy
 *----------------------------------------------------------------------*/
int rtp_net_settcpnocopy (RTP_HANDLE sockHandle, unsigned int onBool)
{
    NATIVE_PROFILE_PAL_NETWORK();

#ifdef RTP_DEBUG
    int result;
    /* ----------------------------------- */
    /*  Clear the error state by setting   */
    /*  to 0.                              */
    /* ----------------------------------- */
    set_errno (0);
#endif

    if ( setsockopt((int) sockHandle,
                    SOL_SOCKET,
                    SO_TCP_NO_COPY,
                    (RTP_PFCCHAR) &onBool,
                    sizeof (int)) != 0 )
    {
#ifdef RTP_DEBUG
        result = xn_getlasterror();
        RTP_DEBUG_OUTPUT_STR("rtp_net_settcpnocopy: error returned ");
        RTP_DEBUG_OUTPUT_INT(result);
        RTP_DEBUG_OUTPUT_STR(".\n");
#endif
        return (-1);
    }
    return (0);
}


/*----------------------------------------------------------------------*
                         rtp_net_setkeepalive
 *----------------------------------------------------------------------*/
int rtp_net_setkeepalive (RTP_HANDLE sockHandle, unsigned int onBool)
{
    NATIVE_PROFILE_PAL_NETWORK();

#ifdef RTP_DEBUG
    int result;
    /* ----------------------------------- */
    /*  Clear the error state by setting   */
    /*  to 0.                              */
    /* ----------------------------------- */
    set_errno (0);
#endif

    if (setsockopt((int) sockHandle, SOL_SOCKET,
                    SO_KEEPALIVE, (RTP_PFCCHAR)&onBool,
                    sizeof (int)) != 0)
    {
#ifdef RTP_DEBUG
        result = xn_getlasterror();
        RTP_DEBUG_OUTPUT_STR("rtp_net_setkeepalive: error returned ");
        RTP_DEBUG_OUTPUT_INT(result);
        RTP_DEBUG_OUTPUT_STR(".\n");
#endif
        return (-1);
    }
    return (0);
}


/*----------------------------------------------------------------------*
                         rtp_net_setmembership
 *----------------------------------------------------------------------*/
int rtp_net_setmembership (RTP_HANDLE sockHandle, unsigned char * ipAddr, int type, unsigned int onBool)
{
    NATIVE_PROFILE_PAL_NETWORK();
    struct sockaddr_in sin;
    struct sockaddr_in localAddr;
    int localLen;
    int result;
    struct ip_mreq mcreq;
    unsigned long in_addr = 0;

    localLen = sizeof (localAddr);
    rtp_memset(&localAddr, 0, localLen);
    rtp_memset(&sin, 0, sizeof (sin));

    if (ipAddr)
    {
        if (type == RTP_NET_TYPE_IPV4)
        {
            CopyIPv4Addr( &in_addr, ipAddr );
        }
        else
        {
            /* ----------------------------------- */
            /* RTP_NET_TYPE_IPV6 not yet supported */
            /* ----------------------------------- */
            return (-1);
        }
    }
    else
    {
        in_addr = INADDR_ANY;
    }

#ifdef RTP_DEBUG
    /* ----------------------------------- */
    /*  Clear the error state by setting   */
    /*  to 0.                              */
    /* ----------------------------------- */
    set_errno (0);
#endif

    if (getsockname ((int) sockHandle, (PSOCKADDR) &localAddr, (RTP_PFINT) &localLen) != 0)
    {
#ifdef RTP_DEBUG
        result = xn_getlasterror();
        RTP_DEBUG_OUTPUT_STR("rtp_net_setmembership: error returned ");
        RTP_DEBUG_OUTPUT_INT(result);
        RTP_DEBUG_OUTPUT_STR(".\n");
#endif
        return (-1);
    }

    rtp_memmove(&mcreq.imr_multiaddr.s_addr, &in_addr, IP_ALEN);
    rtp_memmove(&mcreq.imr_interface.s_addr, &localAddr.sin_addr.s_addr, IP_ALEN);

    if (onBool)
    {
        result = setsockopt((int) sockHandle, IPPROTO_IP,
                            IP_ADD_MEMBERSHIP, (RTP_PFCCHAR)&mcreq,
                            sizeof (mcreq));
    }
    else
    {
        result = setsockopt((int) sockHandle, IPPROTO_IP,
                            IP_DROP_MEMBERSHIP, (RTP_PFCCHAR)&mcreq,
                            sizeof (mcreq));
    }

    if (result != 0)
    {
#ifdef RTP_DEBUG
        result = xn_getlasterror();
        RTP_DEBUG_OUTPUT_STR("rtp_net_setmembership: error returned ");
        RTP_DEBUG_OUTPUT_INT(result);
        RTP_DEBUG_OUTPUT_STR(".\n");
#endif
        return (-1);
    }
    return (0);
}


/*----------------------------------------------------------------------*
                           rtp_net_setbroadcast
 *----------------------------------------------------------------------*/
int rtp_net_setbroadcast (RTP_HANDLE sockHandle, unsigned int onBool)
{
    NATIVE_PROFILE_PAL_NETWORK();
    if (!onBool)
    {
    	RTP_DEBUG_OUTPUT_STR("rtp_net_setbroadcast: broadcasts cannot be turned off.\n");
    	return (-1);
    }
    return (0);
}


/*----------------------------------------------------------------------*
                         rtp_net_setmcastttl
 *----------------------------------------------------------------------*/
int  rtp_net_setmcastttl(RTP_HANDLE sockHandle, int ttl)
{
    NATIVE_PROFILE_PAL_NETWORK();
    int result;

#ifdef RTP_DEBUG
    /* ----------------------------------- */
    /*  Clear the error state by setting   */
    /*  to 0.                              */
    /* ----------------------------------- */
    set_errno (0);
#endif

    result = setsockopt( sockHandle, IPPROTO_IP,
                IP_MULTICAST_TTL, (char *) &ttl, sizeof (int));
    if (result != 0)
    {
#ifdef RTP_DEBUG
        result = xn_getlasterror();
        RTP_DEBUG_OUTPUT_STR("rtp_net_setmcastttl: error returned ");
        RTP_DEBUG_OUTPUT_INT(result);
        RTP_DEBUG_OUTPUT_STR(".\n");
#endif
        return (-1);
    }
    return (0);
}

#if (!INCLUDE_RTIP)
/*----------------------------------------------------------------------*
                            rtp_net_htons
 *----------------------------------------------------------------------*/
short rtp_net_htons (short i)
{
    NATIVE_PROFILE_PAL_NETWORK();
    return (htons(i));
}


/*----------------------------------------------------------------------*
                            rtp_net_ntohs
 *----------------------------------------------------------------------*/
short rtp_net_ntohs (short i)
{
    NATIVE_PROFILE_PAL_NETWORK();
    return (ntohs(i));
}


/*----------------------------------------------------------------------*
                            rtp_net_htonl
 *----------------------------------------------------------------------*/
long rtp_net_htonl (long i)
{
    NATIVE_PROFILE_PAL_NETWORK();
    return (htonl(i));
}


/*----------------------------------------------------------------------*
                            rtp_net_ntohl
 *----------------------------------------------------------------------*/
long rtp_net_ntohl (long i)
{
    NATIVE_PROFILE_PAL_NETWORK();
    return (ntohl(i));
}

#endif //#if (!INCLUDE_RTIP) /* _YI_ 9/7/2005 */

/*----------------------------------------------------------------------*
                           rtp_net_ip_to_str
 *----------------------------------------------------------------------*/
int rtp_net_ip_to_str (char *str, unsigned char *ipAddr, int type)
{
    NATIVE_PROFILE_PAL_NETWORK();
    int n;

    str[0] = '\0';
    if (type != RTP_NET_TYPE_IPV4)
    {
        return (-1);
    }

    for (n=0; n<4; n++)
    {
        rtp_itoa(ipAddr[n], (char *) &(str[rtp_strlen(str)]), 10);
        if (n<3)
        {
            rtp_strcat((char *) str, ".");
        }
    }

    return (rtp_strlen((const char *) str));
}


/*----------------------------------------------------------------------*
                           rtp_net_str_to_ip
 *----------------------------------------------------------------------*/
int rtp_net_str_to_ip (unsigned char *ipAddr, char *str, int *type)
{
    NATIVE_PROFILE_PAL_NETWORK();
    char * ptr;
    char savech;
    int nbytes;
    int n;

    *type = RTP_NET_TYPE_IPV4;
    nbytes = 4;

    for (n=0; n<nbytes; n++)
    {
        ptr = str;
        while (*ptr != '.' && *ptr != '\0')
        {
            ptr++;
        }

        savech = *ptr;
        *ptr = '\0';
        ipAddr[n] =  (unsigned char)rtp_atoi((const char *) str);
        if (savech == '\0')
        {
            break;
        }
        *ptr = savech;

        str = ptr + 1;
    }

    return (0);
}


/*----------------------------------------------------------------------*
                                rtp_fd_zero
 *----------------------------------------------------------------------*/
void rtp_fd_zero (RTP_FD_SET  *list)
{
    NATIVE_PROFILE_PAL_NETWORK();
    list->fdCount = 0;
}


/*----------------------------------------------------------------------*
                                rtp_fd_set
 *----------------------------------------------------------------------*/
void rtp_fd_set (RTP_FD_SET  *list, RTP_HANDLE fd)
{
    NATIVE_PROFILE_PAL_NETWORK();
int limit;


    limit = (int) (RTP_FD_SET_MAX > FD_SETSIZE ? FD_SETSIZE : RTP_FD_SET_MAX);

    if (list->fdCount < limit)
    {
        if (!rtp_fd_isset(list, fd))
        {
            list->fdArray[list->fdCount] = fd;
            list->fdCount++;
        }
    }
}


/*----------------------------------------------------------------------*
                                rtp_fd_clr
 *----------------------------------------------------------------------*/
void rtp_fd_clr (RTP_FD_SET  *list, RTP_HANDLE fd)
{
    NATIVE_PROFILE_PAL_NETWORK();
int n;
int limit;


    limit = (int) (RTP_FD_SET_MAX > FD_SETSIZE ? FD_SETSIZE : RTP_FD_SET_MAX);

    if (list->fdCount > limit)
    {
        list->fdCount = limit;
    }

    for (n = 0; n < list->fdCount; n++)
    {
        if (list->fdArray[n] == fd)
        {
            int i;

            for (i = n; i < list->fdCount - 1; i++)
            {
                list->fdArray[i] = list->fdArray[i+1];
            }

            list->fdCount--;
            break;
        }
    }
}


/*----------------------------------------------------------------------*
                               rtp_fd_isset
 *----------------------------------------------------------------------*/
int rtp_fd_isset (RTP_FD_SET  *list, RTP_HANDLE fd)
{
    NATIVE_PROFILE_PAL_NETWORK();
int n;
int limit;


    limit = (int) (RTP_FD_SET_MAX > FD_SETSIZE ? FD_SETSIZE : RTP_FD_SET_MAX);

    if (list->fdCount > limit)
    {
        list->fdCount = limit;
    }

    for (n = 0; n < list->fdCount; n++)
    {
        if (list->fdArray[n] == fd)
        {
            return (1);
        }
    }

    return (0);
}


/*----------------------------------------------------------------------*
                              rtp_net_select
 *----------------------------------------------------------------------*/
int rtp_net_select (RTP_FD_SET  *readList,
                    RTP_FD_SET  *writeList,
                    RTP_FD_SET  *errorList,
                    struct timeval *timeout)
{
    int result;


    result = select(1, (PFDSET) readList, (PFDSET) writeList, (PFDSET) errorList, (PCTIMEVAL) timeout);
    return (result);
}

/*----------------------------------------------------------------------*
                           rtp_net_getifaceaddr
 *----------------------------------------------------------------------*/
int rtp_net_getifaceaddr (unsigned char *localAddr, unsigned char *remoteAddr,
                          int remotePort, int remoteType)
{
	RTP_SOCKET tempSock;
	int localPort;
	int localType;

	if (rtp_net_socket(&tempSock, RTP_NET_TYPE_IPV4, RTP_NET_SOCK_DGRAM, 0) >= 0)
	{
		/* determine the local IP address that is receiving this request by
		   creating a temporary UDP socket and connecting it back to the
		   sender; we then query the IP address of the temp socket using
		   getsockname. */
		if (rtp_net_connect(tempSock, remoteAddr, remotePort, remoteType) >= 0)		
        {
            if (rtp_net_getsockname(tempSock, localAddr, &localPort, &localType) >= 0)
            {
                rtp_net_closesocket(tempSock);
		        return (0);
            }
        }
        rtp_net_closesocket(tempSock);		
	}
	return (-1);
}

/*----------------------------------------------------------------------*
                           rtp_net_set_ip
 *----------------------------------------------------------------------*/
int rtp_net_set_ip(int ifaceNumber, RTP_PFUINT8 myIpAddress, RTP_PFUINT8 myBroadCastAddress)
{
    NATIVE_PROFILE_PAL_NETWORK();
    return (xn_set_ip(ifaceNumber,
                   myIpAddress,
                   myBroadCastAddress));
}
/*----------------------------------------------------------------------*
                           rtp_net_invoke_input
 *----------------------------------------------------------------------*/
void rtp_net_invoke_input(PIFACE pi, DCU msg, int length)
{
    NATIVE_PROFILE_PAL_NETWORK();
    ks_invoke_input(pi, msg, length);
    rtp_thrd_ip_continuation(pi->ctrl.index);
}

/*----------------------------------------------------------------------*
                           rtp_net_invoke_output
 *----------------------------------------------------------------------*/
void rtp_net_invoke_output(PIFACE pi, int xmit_complete_cnt)
{
    NATIVE_PROFILE_PAL_NETWORK();
    ks_invoke_output(pi, xmit_complete_cnt);
    rtp_thrd_ip_continuation(pi->ctrl.index);
}

/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
