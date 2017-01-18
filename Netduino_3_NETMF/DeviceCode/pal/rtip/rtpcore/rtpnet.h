 /*
 | RTPNET.H - Runtime Platform Services
 |
 |   UNIVERSAL CODE - DO NOT CHANGE
 |
 | EBSnet - RT-Platform
 |
 |  $Author: vmalaiya $
 |  $Date: 2004/12/09 21:05:04 $
 |  $Name:  $
 |  $Revision: 1.3 $
 |
 | Copyright EBSnet Inc. , 2003
 | All rights reserved.
 | This code may not be redistributed in source or linkable object form
 | without the consent of its author.
 |
 | Module description:
 |  [tbd]
*/

#ifndef __RTPNET_H__
#define __RTPNET_H__

#include "rtp.h"
#include "rtip.h"

typedef RTP_HANDLE RTP_SOCKET;

/************************************************************************
 * Defines
 ************************************************************************/
#define RTP_FD_SET_MAX                       64
                                             
#define RTP_NET_TYPE_IPV4                    4
#define RTP_NET_TYPE_IPV6                    6
#define RTP_NET_IP_ALEN                      4

#define RTP_NET_AF_INET                 AF_INET                                             
#define RTP_NET_SOCK_STREAM             SOCK_STREAM
#define RTP_NET_SOCK_DGRAM              SOCK_DGRAM
#define RTP_NET_SOCK_RAW                SOCK_RAW
#define RTP_NET_SOCK_RDM                SOCK_RDM
#define RTP_NET_SOCK_SEQPACKET          SOCK_SEQPACKET
                                             
#define RTP_NET_EADDRNOTAVAIL           EADDRNOTAVAIL   
#define RTP_NET_EADDRINUSE              EADDRINUSE      
#define RTP_NET_EAFNOSUPPORT            EAFNOSUPPORT    
#define RTP_NET_EARPFULL                EARPFULL        
#define RTP_NET_EBADBAUD                EBADBAUD        
#define RTP_NET_EBADCOMMNUM             EBADCOMMNUM     
#define RTP_NET_EBADDEVICE              EBADDEVICE      
#define RTP_NET_EBADIFACE               EBADIFACE       
#define RTP_NET_EBADMASK                EBADMASK        
#define RTP_NET_EBADRESP                EBADRESP        
#define RTP_NET_ECONNREFUSED            ECONNREFUSED    
#define RTP_NET_EDESTADDREQ             EDESTADDREQ     
#define RTP_NET_EDESTUNREACH            EDESTUNREACH    
#define RTP_NET_EFAULT                  EFAULT          
#define RTP_NET_EIFACECLOSED            EIFACECLOSED    
#define RTP_NET_EIFACEFULL              EIFACEFULL      
#define RTP_NET_EIFACEOPENFAIL          EIFACEOPENFAIL  
#define RTP_NET_EINPROGRESS             EINPROGRESS     
#define RTP_NET_EINVAL                  EINVAL          
#define RTP_NET_EISCONN                 EISCONN         
#define RTP_NET_EMCASTFULL              EMCASTFULL      
#define RTP_NET_EMCASTNOTFOUND          EMCASTNOTFOUND  
#define RTP_NET_EMFILE                  EMFILE          
#define RTP_NET_ENETDOWN                ENETDOWN        
#define RTP_NET_ENETUNREACH             ENETUNREACH     
#define RTP_NET_ENOPKTS                 ENOPKTS         
#define RTP_NET_ENOPROTOOPT             ENOPROTOOPT     
#define RTP_NET_ENOTCONN                ENOTCONN        
#define RTP_NET_ENOTINITIALIZED         ENOTINITIALIZED 
#define RTP_NET_ENOTSOCK                ENOTSOCK        
#define RTP_NET_ENUMDEVICE              ENUMDEVICE      
#define RTP_NET_EOPNOTSUPPORT           EOPNOTSUPPORT   
#define RTP_NET_EOUTPUTFULL             EOUTPUTFULL     
#define RTP_NET_EPROBEFAIL              EPROBEFAIL      
#define RTP_NET_ERENTRANT               ERENTRANT       
#define RTP_NET_ERTNOTFOUND             ERTNOTFOUND     
#define RTP_NET_ERTFULL                 ERTFULL         
#define RTP_NET_ERSCINITFAIL            ERSCINITFAIL    
#define RTP_NET_ESHUTDOWN               ESHUTDOWN       
#define RTP_NET_ETIMEDOUT               ETIMEDOUT       
#define RTP_NET_ETNOSUPPORT             ETNOSUPPORT     
#define RTP_NET_EWOULDARP               EWOULDARP       
#define RTP_NET_EWOULDBLOCK             EWOULDBLOCK     
#define RTP_NET_EOUTAMEM                EOUTAMEM        
#define RTP_NET_ETABLEFULL              ETABLEFULL      
#define RTP_NET_EMSGSIZE                EMSGSIZE        
#define RTP_NET_EDEVOPENFAIL            EDEVOPENFAIL    
#define RTP_NET_EPFNOSUPPORT            EPFNOSUPPORT    
#define RTP_NET_EPROTONOSUPPORT         EPROTONOSUPPORT 
#define RTP_NET_EHTABLEFULL             EHTABLEFULL     
#define RTP_NET_EENTRYNOTFOUND          EENTRYNOTFOUND  
#define RTP_NET_ETOOMANYSERVERS         ETOOMANYSERVERS 
#define RTP_NET_ENO_RECOVERY            ENO_RECOVERY    
#define RTP_NET_ENAME_TOO_LONG          ENAME_TOO_LONG  
#define RTP_NET_ETRYAGAIN               ETRYAGAIN       
#define RTP_NET_ENODATA                 ENODATA         
#define RTP_NET_ENORESPONSE             ENORESPONSE     
#define RTP_NET_ENOSERVERS              ENOSERVERS      
#define RTP_NET_EDHCPSERVNORESP         EDHCPSERVNORESP   
#define RTP_NET_EDHCPSERVBADRESP        EDHCPSERVBADRESP  
#define RTP_NET_EDHCPSERVNOOFFER        EDHCPSERVNOOFFER  
#define RTP_NET_EDHCPSERVNOACK          EDHCPSERVNOACK    
#define RTP_NET_EDHCPSERVNOACKNAK       EDHCPSERVNOACKNAK 
#define RTP_NET_EDHCPOPNOTFOUND         EDHCPOPNOTFOUND   
#define RTP_NET_EDHCPOPOVERFLOW         EDHCPOPOVERFLOW   
#define RTP_NET_EDHCPOPBADLEN           EDHCPOPBADLEN     
#define RTP_NET_EDHCPBADPARAM           EDHCPBADPARAM  
#define RTP_NET_EACCES                  EACCES   


/* IP options */
#define RTP_NET_IP_OPTS_MULTICAST_IF    IP_MULTICAST_IF    
#define RTP_NET_IP_OPTS_MULTICAST_TTL   IP_MULTICAST_TTL   
#define RTP_NET_IP_OPTS_MULTICAST_LOOP  IP_MULTICAST_LOOP  
#define RTP_NET_IP_OPTS_ADD_MEMBERSHIP  IP_ADD_MEMBERSHIP  
#define RTP_NET_IP_OPTS_DROP_MEMBERSHIP IP_DROP_MEMBERSHIP 
#define RTP_NET_IP_OPTS_BROADCAST       IP_BROADCAST
#define RTP_NET_IP_OPTS_DONTFRAG        IP_DONTFRAG

/* Socket Options */
#define RTP_NET_SO_NAGLE                SO_NAGLE        /* NAGLE algorithm enabled/disabled */
#define RTP_NET_SO_TCP_STREAM           SO_TCP_STREAM   /* do not send unless can send MSS */
#define RTP_NET_SO_REUSEADDR            SO_REUSEADDR    /* allow local address reuse */
#define RTP_NET_SO_KEEPALIVE            SO_KEEPALIVE    /* keep connections alive */
#define RTP_NET_SO_MAX_UDP_QUE          SO_MAX_UDP_QUE  /* max number of input UDP pkt to que */
#define RTP_NET_SO_UDPCKSUM_IN          SO_UDPCKSUM_IN  /* check UDP checksum on input pkts */
#define RTP_NET_SO_UDPCKSUM_OUT         SO_UDPCKSUM_OUT /* generate UDP checksum output pkts */
#define RTP_NET_SO_LINGER               SO_LINGER       /* linger on close if data present */
#define RTP_NET_SO_TCP_NO_COPY          SO_TCP_NO_COPY  /* TCP is in packet mode vs window mode */
#define RTP_NET_SO_REUSESOCK            SO_REUSESOCK    /* allow reuse of socket in TWAIT state */
#define RTP_NET_SO_DELAYED_ACK          SO_DELAYED_ACK  /* delay sending ACK */
#define RTP_NET_SO_IP_TTL               SO_IP_TTL       /* IP time-to-live */
#define RTP_NET_SO_MCAST_LOOP           SO_MCAST_LOOP   /* send multicast to loopback if */
#define RTP_NET_SO_SELECT_SIZE          SO_SELECT_SIZE  /* TCP write select: wake up when */
                                                        /* specifed room in window   */
#define RTP_NET_SO_TYPE                 SO_TYPE         /* type of socket */                                                        
#define RTP_NET_SO_ACCEPTCON            SO_ACCEPTCON    /* can socket accept connection */
#define RTP_NET_IO_BLOCK_OPT            IO_BLOCK_OPT    /* blocking mode: set by ioctlsocket() */
                                                        /* listening to dest address (UDP only)   */

#if (INCLUDE_TCP_TIMESTAMP)
#define RTP_NET_SO_TCP_TIMESTAMP        SO_TCP_TIMESTAMP /* TCP timestamp option */
#endif

#if (INCLUDE_802_2)
#define RTP_NET_SO_802_2                SO_802_2         /* 802.2 socket - passed to setsockopt() */
#endif                          
                                
#define RTP_NET_SO_TOS                  SO_TOS                 /* TOS Byte value in IP Header */
#define RTP_NET_SO_FREE_WITH_INPUT      SO_FREE_WITH_INPUT     /* Free TCP socket even if data in */
                                
#if (INCLUDE_SSL)                    
#define RTP_SO_SECURE_SOCKET            SO_SECURE_SOCKET        /* Use secure socket to transmit data */
#endif

#define RTP_NET_SO_RCV_TIMEO            SO_RCV_TIMEO            /* Set receive timeout */
#define RTP_NET_SO_SEND_TIMEO           SO_SEND_TIMEO           /* Set send timeout */
#define RTP_NET_SO_ERROR                SO_ERROR                /* get and clear errno */

#define RTP_NET_SO_RCVBUF               SO_RCVBUF
#define RTP_NET_SO_SNDBUF               SO_SNDBUF
#define RTP_NET_SO_RECEIVE_BUFFER       SO_RECEIVE_BUFFER
#define RTP_NET_SO_SEND_BUFFER          SO_SEND_BUFFER

#define RTP_NET_SO_IP_DONTFRAG          IP_DONTFRAG

/* IP option levels */
#define RTP_NET_SOL_SOCKET              SOL_SOCKET 
#define RTP_NET_IPROTO_TCP              IPROTO_TCP  
#define RTP_NET_IPROTO_UDP              IPROTO_UDP  
#define RTP_NET_IPROTO_IP               IPROTO_IP   
#define RTP_NET_IPPROTO_TCP             IPPROTO_TCP 
#define RTP_NET_IPPROTO_UDP             IPPROTO_UDP 
#define RTP_NET_IPPROTO_IP              IPPROTO_IP  

#define RTP_NET_MSG_OOB                 MSG_OOB            /* process out-of-band data */
#define RTP_NET_MSG_PEEK                MSG_PEEK           /* peek at incoming message */
#define RTP_NET_MSG_DONTROUTE           MSG_DONTROUTE      /* send without using routing tables */
#define RTP_NET_MSG_EOR                 MSG_EOR            /* data completes record */
#define RTP_NET_MSG_TRUNC               MSG_TRUNC          /* data discarded before delivery */
#define RTP_NET_MSG_CTRUNC              MSG_CTRUNC         /* control data lost before delivery */
#define RTP_NET_MSG_WAITALL             MSG_WAITALL        /* wait for full request or error */
#define RTP_NET_MSG_QUEUE               MSG_QUEUE          /* just queue the buffer; dont send it */

/************************************************************************
 * Typedefs
 ************************************************************************/
/************************************************************************
 * RTP_FD_SET - socket handle list for rtp_net_select                   *
 ************************************************************************/
typedef struct s_RTP_FD_SET
{
    int                fdCount;
    RTP_SOCKET         fdArray[RTP_FD_SET_MAX];
}
RTP_FD_SET;

/************************************************************************
 * RTP_NET_ADDR - network address structure                             *
 ************************************************************************/
typedef struct s_RTP_NET_ADDR
{
	unsigned char      ipAddr[RTP_NET_IP_ALEN];
	int                port;
	int                type;
}
RTP_NET_ADDR;

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
 * Initialize/Shutdown the API                                          *
 ************************************************************************/

int rtp_init_completions     (void);
int rtp_uninit_completions   (void);
int  rtp_net_init            (void);
void rtp_net_exit            (void);
void rtp_net_restart         (void);
int  rtp_net_set_ip          (int ifaceNumber, RTP_PFUINT8 myIpAddress, RTP_PFUINT8 myBroadCastAddress);
/************************************************************************
 * Sockets API                                                          *
 ************************************************************************/

/* general sockets calls */
int      rtp_net_socket          (RTP_SOCKET *sockHandle, int family, int type, int protocol);
int      rtp_net_bind            (RTP_SOCKET sockHandle, unsigned char *ipAddr, int port, int type);
int      rtp_net_listen          (RTP_SOCKET sockHandle, int queueSize);
int      rtp_net_getpeername     (RTP_SOCKET sockHandle, unsigned char *ipAddr, int *port, int *type);
int      rtp_net_getsockname     (RTP_SOCKET sockHandle, unsigned char *ipAddr, int *port, int *type);
int      rtp_net_gethostbyname   (unsigned char *ipAddr, int *type, char *name);
int      rtp_net_accept          (RTP_SOCKET *connectSock, RTP_SOCKET serverSock,
                                  unsigned char *ipAddr, int *port, int *type);
int      rtp_net_connect         (RTP_SOCKET sockHandle, unsigned char *ipAddr, int port, int type);
unsigned rtp_net_is_connected    (RTP_SOCKET sockHandle);
int      rtp_net_write_select    (RTP_SOCKET sockHandle, long msecTimeout);
int      rtp_net_read_select     (RTP_SOCKET sockHandle, long msecTimeout);
long     rtp_net_send            (RTP_SOCKET sockHandle, const unsigned char *buffer, long size, int flags);
long     rtp_net_recv            (RTP_SOCKET sockHandle, unsigned char *buffer, long size, int flags);
long     rtp_net_sendto          (RTP_SOCKET sockHandle, const unsigned char *buffer, long size,
                                  unsigned char *ipAddr, int port, int type, int flags);
long     rtp_net_recvfrom        (RTP_SOCKET sockHandle, unsigned char *buffer, long size,
                                  unsigned char *ipAddr, int *port, int *type, int flags);
int      rtp_net_closesocket     (RTP_SOCKET sockHandle);
int      rtp_net_shutdown        (RTP_SOCKET sockHandle, int how);
int      rtp_net_getlasterror    (void);

/* socket options */
int      rtp_net_setsockoopt     (RTP_HANDLE sockHandle, int level, int optionName, 
                                    RTP_PFCCHAR optionVal, int length);
int      rtp_net_getsockopt      (RTP_HANDLE sockHandle, int level, int optionName, 
                                    RTP_PFCHAR opttionVal, int* optlen );                                   
int      rtp_net_getntoread      (RTP_SOCKET sockHandle, unsigned long * nToRead);
int      rtp_net_setblocking     (RTP_SOCKET sockHandle, unsigned int onBool);
int      rtp_net_setnagle        (RTP_SOCKET sockHandle, unsigned int onBool);
int      rtp_net_setlinger       (RTP_SOCKET sockHandle, unsigned int onBool, long msecTimeout);
int      rtp_net_setreusesock    (RTP_SOCKET sockHandle, unsigned int onBool);
int      rtp_net_setreuseaddr    (RTP_SOCKET sockHandle, unsigned int onBool);
int      rtp_net_settcpnocopy    (RTP_SOCKET sockHandle, unsigned int onBool);
int      rtp_net_setkeepalive    (RTP_SOCKET sockHandle, unsigned int onBool);
int      rtp_net_setmembership   (RTP_SOCKET sockHandle, unsigned char * ipAddr,
                                  int type, unsigned int onBool);
int      rtp_net_setmcastttl     (RTP_SOCKET sockHandle, int ttl);
int      rtp_net_setbroadcast    (RTP_SOCKET sockHandle, unsigned int onBool);

/* utility functions */
#if (!INCLUDE_RTIP)
short    rtp_net_htons           (short i);
short    rtp_net_ntohs           (short i);
long     rtp_net_htonl           (long i);
long     rtp_net_ntohl           (long i);
#endif /* #if (!INCLUDE_RTIP) */ 

int      rtp_net_ip_to_str       (char *str, unsigned char *ipAddr, int type);
int      rtp_net_str_to_ip       (unsigned char *ipAddr, char *str, int *type);
int      rtp_net_getifaceaddr    (unsigned char *localAddr, unsigned char *remoteAddr,
                                  int remotePort, int remoteType);

/* FD_SET functions */
void     rtp_fd_zero             (RTP_FD_SET  *list);
void     rtp_fd_set              (RTP_FD_SET  *list, RTP_SOCKET fd);
void     rtp_fd_clr              (RTP_FD_SET  *list, RTP_SOCKET fd);
int      rtp_fd_isset            (RTP_FD_SET  *list, RTP_SOCKET fd);

int      rtp_net_select          (RTP_FD_SET  *readList, RTP_FD_SET  *writeList, RTP_FD_SET  *errorList, struct timeval * msecTimeout);

void     rtp_net_invoke_input    (PIFACE pi, DCU msg, int length);
void     rtp_net_invoke_output   (PIFACE pi, int xmit_complete_cnt);
#ifdef __cplusplus
}
#endif

#endif /* __RTPNET_H__ */

/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
