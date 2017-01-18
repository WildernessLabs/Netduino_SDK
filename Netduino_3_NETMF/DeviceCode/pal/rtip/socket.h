/*
 * Copyright (c) 1982,1985,1986,1988 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *  This product includes software developed by the University of
 *  California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *  @(#)socket.h    7.13 (Berkeley) 4/20/91
 */

/*
 * Definitions related to sockets: types, address families, options.
 */

#ifndef __SOCKET__
#define __SOCKET__ 1

#ifdef __cplusplus
RTP_EXTERN "C" {
#endif

#ifdef _WINSOCKAPI_
#error: Cannot include winsock.h or winsock2.h before socket.h->
        define WIN32_LEAN_AND_MEAN before including windows.h
#endif
#ifdef _WINSOCK2API_
#error: Cannot include winsock.h or winsock2.h before socket.h->
        define WIN32_LEAN_AND_MEAN before including windows.h
#endif

#define _WINSOCKAPI_
#define _WINSOCK2API_

/* *********************************************************************   */
#include "xnconf.h"
#include "rtipconf.h"
#include "rtipapi.h"


/* *********************************************************************   */
/* enables using code as DLL                                               */
/* NOTE: the same define is in rtip.h                                      */
#define SOCKAPI_
/*#define SOCKAPI_  __stdcall __export   */

/* *********************************************************************   */
/* Socket Types                                                            */
#define SOCK_STREAM     1       /* stream socket */
#define SOCK_DGRAM      2       /* datagram socket */
#define SOCK_RAW        3       /* raw-protocol interface */
#define SOCK_RDM        4       /* reliably-delivered message */
#define SOCK_SEQPACKET  5       /* sequenced packet stream */

/* *********************************************************************   */
/* Option flags per-socket. - NOTE: options in effect for a socket         */
/* are stored in options field of ANYPORT structure (see rtip.h)           */
/* NOTE: socket defines the socket option as an int; for machines with     */
/*       16-bit ints there are not enough bits to represent all the        */
/*       options as individual bits; therefore, options are stored         */
/*       internally as a long and some options have more than one          */
/*       bit set in the option value (see SO_xx and SOO_xx)                */
#define SO_NAGLE                0x0001     /* NAGLE algorithm enabled/disabled */
#define SO_TCP_STREAM           0x0002     /* do not send unless can send MSS */
#define SO_REUSEADDR            0x0004     /* allow local address reuse */
#define SO_KEEPALIVE            0x0008     /* keep connections alive */
#define SO_MAX_UDP_QUE          0x0010     /* max number of input UDP pkt to que */
#define SO_UDPCKSUM_IN          0x0020     /* check UDP checksum on input pkts */
#define SO_UDPCKSUM_OUT         0x0040     /* generate UDP checksum output pkts */
#define SO_LINGER               0x0080     /* linger on close if data present */
#define SO_TCP_NO_COPY          0x0100     /* TCP is in packet mode vs window mode */
#define SO_REUSESOCK            0x0200     /* allow reuse of socket in TWAIT state */
#define SO_DELAYED_ACK          0x0400     /* delay sending ACK */
#define SO_IP_TTL               0x0800     /* IP time-to-live */
#define SO_MCAST_LOOP           0x1000     /* send multicast to loopback if */
#define SO_SELECT_SIZE          0x2000     /* TCP write select: wake up when */
                                           /*   specifed room in window   */
#define IO_BLOCK_OPT            0x4000     /* blocking mode: set by ioctlsocket() */
                                           /* listening to dest address (UDP only)   */
#if (INCLUDE_TCP_TIMESTAMP)
#define SO_TCP_TIMESTAMP        0x8000     /* TCP timestamp option */
#endif

#if (INCLUDE_802_2)
#define SO_802_2                0x1001     /* 802.2 socket - passed to setsockopt() */
#define SOO_802_2               0x10000L   /* 802.2 socket - stored internally */
#endif                          
                                
#define SO_TOS                  0x1002     /* TOS Byte value in IP Header */
#define SOO_TOS                 0x20000L   /* TOS - stored internally */

#define SO_FREE_WITH_INPUT      0x1004     /* Free TCP socket even if data in */
                                           /* input window   */
#define SOO_FREE_WITH_INPUT     0x40000L

#if (INCLUDE_SSL)                    /* __st__ 2002.08.02 */
#define SO_SECURE_SOCKET        0x1008   /* Use secure socket to transmit data */
#define SOO_SECURE_SOCKET       0x80000L /* SECURE SOCKET - stored internally */
#endif

#define SO_RCV_TIMEO            0x1010         /* Set receive timeout */
#define SOO_RCV_TIMEO           0x00100000L    /* Set receive timeout - stored internally */
                                
#define SO_SEND_TIMEO           0x1011         /* Set send timeout */
#define SOO_SEND_TIMEO          0x00200000L    /* Set send timeout - stored internally */
                                
#define SO_ERROR                0x1012         /* get and clear errno */

#define SO_INPUT_WINDOW         0x1013            // TCP input window size
#define SO_OUTPUT_WINDOW        0x1014            // TCP output window size
#define SO_RCVBUF               SO_INPUT_WINDOW   // define BSD socket option
#define SO_SNDBUF               SO_OUTPUT_WINDOW  // define BSD socket option
#define SO_RECEIVE_BUFFER       0x1015            // UDP buffer size
#define SOO_RECEIVE_BUFFER      0x00000001L       // ip_option2_flags
#define SO_SEND_BUFFER          0x1016            // UDP buffer size
#define SOO_SEND_BUFFER         0x00000002L       // ip_option2_flags

#define SO_TYPE                 0x1018            // type of socket
#define SO_ACCEPTCON            0x1019            // can socket accept connection

#define IP_DONTFRAG             0x1023            // Don't Fragment Packet
#define IPO_DONTFRAG            0x40000000L

#define IP_BROADCAST            0x1024            // Allow send and recv broadcast
#define IPO_BROADCAST           0x80000000L
/* *********************************************************************   */
/* Options for multicast  - level will always be IPPROTO_IP                */
/* constants taken from Berkeley values                                    */
#define IP_MULTICAST_IF          9   /* set/get IP multicast interface */
#define IP_MULTICAST_TTL        10   /* set/get IP multicast TTL */
#define IP_MULTICAST_LOOP       11   /* set/get IP multicast loopback */
#define IP_ADD_MEMBERSHIP       12   /* join multicast group */
#define IP_DROP_MEMBERSHIP      13   /* leave multicast group */

/* NOTE: 0xE0000000 is invalid   */
#define MIN_VALID_MCADDR        0xE0000001UL
#define MAX_VALID_MCADDR        0xEFFFFFFFUL

/* *********************************************************************   */
/*
 * Structure used for manipulating linger option.
 */
typedef struct linger
{
    int l_onoff;        /* option on/off */
    int l_linger;       /* linger time */
} LINGER;
typedef struct linger RTP_FAR * PLINGER;

/*
 * Level number for (get/set)sockopt() to apply to socket itself.
*/
#define SOL_SOCKET              0x7fff      /* options for socket level */

/* *********************************************************************   */
/*
 * Address families.
 */
#define AF_UNSPEC       0       /* unspecified */
#define AF_UNIX         1       /* local to host (pipes, portals) */
#define AF_INET         2       /* internetwork: UDP, TCP, etc. */
#define AF_IMPLINK      3       /* arpanet imp addresses */
#define AF_PUP          4       /* pup protocols: e.g. BSP */
#define AF_CHAOS        5       /* mit CHAOS protocols */
#define AF_NS           6       /* XEROX NS protocols */
#define AF_ISO          7       /* ISO protocols */
#define AF_OSI          AF_ISO
#define AF_ECMA         8       /* european computer manufacturers */
#define AF_DATAKIT      9       /* datakit protocols */
#define AF_CCITT        10      /* CCITT protocols, X.25 etc */
#define AF_SNA          11      /* IBM SNA */
#define AF_DECnet       12      /* DECnet */
#define AF_DLI          13      /* DEC Direct data link interface */
#define AF_LAT          14      /* LAT */
#define AF_HYLINK       15      /* NSC Hyperchannel */
#define AF_APPLETALK    16      /* Apple Talk */
#define AF_ROUTE        17      /* Internal Routing Protocol */
#define AF_LINK         18      /* Link layer interface */
#define pseudo_AF_XTP   19      /* eXpress Transfer Protocol (no AF) */
#define AF_INET6        20      /* IPv6: UDP, TCP, etc. */

#define AF_MAX                  21

#define INADDR_ANY              0x00000000UL
#define INADDR_BROADCAST        0xffffffffUL
#define INADDR_NONE             0xffffffffUL

/* *********************************************************************   */
/* protocols                                                               */
#define IPROTO_TCP              6
#define IPROTO_UDP              17
#define IPROTO_IP               8
#define IPPROTO_TCP             6
#define IPPROTO_UDP             17
#define IPPROTO_IP              8

/* *********************************************************************   */
struct in_addr
{
    union
    {
        struct
        {
            RTP_UINT8 s_b1;
            RTP_UINT8 s_b2;
            RTP_UINT8 s_b3;
            RTP_UINT8 s_b4;
        } s_un_b;
        struct
        {
            RTP_UINT16 s_w1;
            RTP_UINT16 s_w2;
        } s_un_w;
        RTP_UINT32 S_addr;
    } s_un;
};

#if !(CFG_BSD_44)
#define s_addr  S_addr
#else
#define s_addr  s_un.S_addr
#endif

/*
 * Structure used by kernel to store most addresses.
 */
typedef struct sockaddr
{
    RTP_UINT8   sa_len;         /* total length */
    RTP_UINT8   sa_family;      /* address family */
    char    sa_data[14];    /* actually longer; address value */
} sockaddr;
typedef struct sockaddr RTP_FAR * PSOCKADDR;
typedef const struct sockaddr RTP_FAR * PCSOCKADDR;

/*
 * Structure used by kernel to store most
 * addresses.
 */
#define SIN_ZERO_SIZE           8

typedef struct sockaddr_in
{
    RTP_UINT16  sin_family;                 /* address family */
    RTP_UINT16  sin_port;                   /* port number */
#if !(CFG_BSD_44)
    RTP_UINT32  sin_addr;                   /* IP address */
#else
    struct in_addr sin_addr;            /* IP address - UNIX/Winsock compliant! */
#endif
    char    sin_zero[SIN_ZERO_SIZE];    /* unused (set to 0) */
} sockaddr_in;
typedef struct sockaddr_in RTP_FAR * PSOCKADDR_IN;

/* *********************************************************************   */
/*
 * Structure used for IP add and drop membership options
 */
typedef struct ip_mreq
{
    struct in_addr imr_multiaddr;  /* multicast group to join */
    struct in_addr imr_interface;  /* local IP address for this interface */
} IP_MREQ;

typedef struct ip_mreq RTP_FAR *PIPMREQ;

typedef struct in_addr RTP_FAR *PINADDR;

/* *********************************************************************   */
/*
 * Protocol families, same as address families for now.
 */
#define PF_UNSPEC               AF_UNSPEC
#define PF_UNIX                 AF_UNIX
#define PF_INET                 AF_INET
#define PF_IMPLINK              AF_IMPLINK
#define PF_PUP                  AF_PUP
#define PF_CHAOS                AF_CHAOS
#define PF_NS                   AF_NS
#define PF_ISO                  AF_ISO
#define PF_OSI                  AF_ISO
#define PF_ECMA                 AF_ECMA
#define PF_DATAKIT              AF_DATAKIT
#define PF_CCITT                AF_CCITT
#define PF_SNA                  AF_SNA
#define PF_DECnet               AF_DECnet
#define PF_DLI                  AF_DLI
#define PF_LAT                  AF_LAT
#define PF_HYLINK               AF_HYLINK
#define PF_APPLETALK            AF_APPLETALK
#define PF_ROUTE                AF_ROUTE
#define PF_LINK                 AF_LINK
#define PF_XTP                  pseudo_AF_XTP   /* really just proto family, no AF */
#define PF_INET6                AF_INET6

#define PF_MAX                  AF_MAX

/* *********************************************************************       */
/* values for flag parameter (MSG_PEEK, MSG_QUEUE and MSG_WAITALL are the only */
/* flags supported and they are supported for TCP only)                        */
#define MSG_OOB                 0x1         /* process out-of-band data */
#define MSG_PEEK                0x2         /* peek at incoming message */
#define MSG_DONTROUTE           0x4     /* send without using routing tables */
#define MSG_EOR                 0x8         /* data completes record */
#define MSG_TRUNC               0x10        /* data discarded before delivery */
#define MSG_CTRUNC              0x20        /* control data lost before delivery */
#define MSG_WAITALL             0x40        /* wait for full request or error */
#define MSG_QUEUE               0x80        /* just queue the buffer; dont send it */

/* ********************************************************************   */
/* select                                                                 */

/* max number of select requests; set to max number of ports but can   */
/* be cut down to conserve space in fd_set                             */
#if (INCLUDE_MALLOC_PORTS || INCLUDE_RUN_TIME_CONFIG || RTPLATFORM)
#define FD_SETSIZE              64          /* Match for Microsoft _YI_ */
#else
#define FD_SETSIZE              TOTAL_PORTS
#endif

#if !defined(__Itime) && ! defined(KROS)     /* POWERPC diab includes struct timeval in time.h */
typedef struct timeval
{
    long tv_sec;
    long tv_usec;
} timeval;
#endif
typedef struct timeval RTP_FAR * PTIMEVAL;
typedef const struct timeval RTP_FAR * PCTIMEVAL;

#ifdef __GNUC__
	// Using GCC library, the following symbols are already defined in types.h 
	// We need to undefine them before
	#ifdef fd_set
	#undef fd_set
	#endif

	#ifdef FD_SET
	#undef FD_SET
	#endif

	#ifdef FD_CLR
	#undef FD_CLR
	#endif

	#ifdef FD_ISSET
	#undef FD_ISSET
	#endif

	#ifdef FD_ZERO
	#undef FD_ZERO
	#endif
#endif

typedef struct fd_set
{
    int num_sockets;
    int sockets[FD_SETSIZE];
} fd_set;
typedef struct fd_set RTP_FAR * PFDSET;

#define FD_SET(socket, fd)                                         \
    if (((PFDSET)fd)->num_sockets < FD_SETSIZE)                    \
    {                                                              \
        ((PFDSET)fd)->sockets[((PFDSET)fd)->num_sockets] = socket; \
        ((PFDSET)fd)->num_sockets++;                               \
    }

#define FD_CLR(socket, fd)  proc_fd_clr(socket, (PFDSET)fd)

#define FD_ISSET(socket, fd) proc_fd_isset(socket, (PFDSET)(fd))

#define FD_ZERO(fd) proc_fd_zero((PFDSET)fd)

/* ********************************************************************   */
/* ioctlsocket command parameter values                                   */
#define FIONBIO                 1l
#define FIONREAD                2l
#define FIONWRITE               3l

/* ********************************************************************   */
/* DNS and database stuff                                                 */
#define DNS_PORT                53          /* Well known DNS port */
#define DOMSIZE                 512         /* maximum domain message size to mess with */
#define DRCODE                  0x000F      /* response code, see below */
#define DQR                     0x8000      /* query=0, response=1 */
#define DIN                     1           /* ARPA internet class */
#define DRD                     0x0100      /* Recursion desired */

/* QTYPE field of DNS query (follows the QNAME field)   */
#define DTYPEPTR                12          /* a domain name ptr */
#define DTYPEA                  1           /* host address resource record (RR) */

/* ********************************************************************   */
/* DATABASE                                                               */

struct  protoent
{
    RTP_PFCCHAR             p_name;        /* official protocol name */
    RTP_PFCCHAR RTP_FAR *   p_aliases;        /* alias list */
    short                   p_proto;       /* protocol # */
    short                   fill;          /* alignment */
    RTP_PFCCHAR             alias;         /* extended part for actual string */
};
typedef struct protoent RTP_FAR *PFPROTOENT;
#define NUM_PROTOENT 20             /* number of protent entries */


struct servent
{
    RTP_PFCCHAR             s_name;         /* official name of the service */
    RTP_PFCCHAR *           s_aliases;      /* alternative names (NI) */
    int                     s_port;         /* port number in network byte order */
    RTP_PFCCHAR             s_proto;        /* name of the protocol */
};
typedef struct servent RTP_FAR *PFSERVENT;

#define NUM_SERVENT 135             /* number of servent entries */

#if (INCLUDE_DNS)
/* format of host table, host cache and structure returned by gethostbyname()   */
/* and gethostbyaddr()                                                          */
struct hostentext
{
    RTP_PFCHAR              h_name;                         /* official name of host */
    RTP_PFCHAR              h_aliases[CFG_MAX_HX_ALIAS];    /* alias list */
    RTP_UINT16              h_addrtype;                     /* host address type */
    RTP_UINT16              h_length;                       /* length of address */
    RTP_PFCHAR              h_addr_list[CFG_MAX_HX_ADDRS];  /* list of addresses */
    RTP_UINT32              h_ttl;                          /* Time to live for entry. */

    /* Host table part   */
    char                    sz_name[CFG_DNS_NAME_LEN];
    struct in_addr          ip_addr;
    char                    alias[CFG_DNS_NAME_LEN];
};
#define h_addr  h_addr_list[0]              /* address, for backward compat */
typedef struct hostentext RTP_FAR *PFHOSTENT;
#define hostent hostentext

/* format of DNS Request/Response header; the header is followed
   by Question section */
struct dhead
{
    RTP_UINT16 ident;
    #define  DNS_RSP_NO_ERR     0   /* successful */
    #define  DNS_RSP_FMT_ERR    1   /* unable to interpret inquery (internal */
                                    /* DNS error - should never happen)   */
    #define  DNS_RSP_SRV_FAIL   2   /* unsuccessful due to problem with */
                                    /* name server   */
    #define  DNS_RSP_NAME_ERR   3   /* requested name does not exist */
    #define  DNS_RSP_REFUSED    5   /* server refused due to policy reasons */
    RTP_UINT16 flags;
    RTP_UINT16 qdcount;
    RTP_UINT16 ancount;
    RTP_UINT16 nscount;
    RTP_UINT16 arcount;
};

struct useek
{
    struct dhead h;
    RTP_UINT8         x[DOMSIZE];       /* Question */
};
typedef struct useek RTP_FAR * PFUSEEK;


struct rrpart
{
    RTP_UINT16  rtype;
    RTP_UINT16  rclass;
    RTP_UINT32 rttl;
    RTP_UINT16  rdlength;
    RTP_UINT8  rdata[DOMSIZE];
};
typedef struct rrpart RTP_FAR *PFRRPART;
#endif /* INCLUDE_DNS */

/* ********************************************************************   */
/* FUNCTION DECLARATIONS                                                  */
/* ********************************************************************   */

#if (!BUILD_NEW_BINARY)
#define ntohs(x) net2hs((RTP_UINT16)(x))
#define htons(x) hs2net((RTP_UINT16)(x))
#define htonl(x) hl2net((RTP_UINT32)(x))
#define ntohl(x) net2hl((RTP_UINT32)(x))
#else
RTP_UINT16 ntohs(RTP_UINT16 x);
RTP_UINT16 htons(RTP_UINT16 x);
RTP_UINT32 htonl(RTP_UINT32 x);
RTP_UINT32 ntohl(RTP_UINT32 x);
#endif

RTP_UINT16       net2hs(RTP_UINT16 w);
RTP_UINT16       hs2net(RTP_UINT16 w);
RTP_UINT32       net2hl(RTP_UINT32 l);
RTP_UINT32       hl2net(RTP_UINT32 l);

/* ********************************************************************   */
/* SELECT function declarations                                           */
void SOCKAPI_       proc_fd_clr(int socket, PFDSET fd);
int  SOCKAPI_       proc_fd_isset(int socket, PFDSET fd);
void SOCKAPI_       proc_fd_zero(PFDSET fd);

/* ********************************************************************   */
/* SOCKET function declarations                                           */
SOCKET SOCKAPI_     socket(int, int, int);
int  SOCKAPI_       accept(int, PSOCKADDR, RTP_PFINT);
int  SOCKAPI_       bind(int, PCSOCKADDR, int);
int  SOCKAPI_       closesocket(SOCKET socket);
int  SOCKAPI_       connect(SOCKET, PCSOCKADDR, int);
int  SOCKAPI_       ioctlsocket(SOCKET socket, long cmd, unsigned long RTP_FAR *argp);
int  SOCKAPI_       getpeername(SOCKET, PSOCKADDR, RTP_PFINT);
int  SOCKAPI_       getsockname(SOCKET, PSOCKADDR, RTP_PFINT);
int  SOCKAPI_       getsockopt(SOCKET socket, int level, int option_name,
                                RTP_PFCHAR option_value, RTP_PFINT optionlen);
int  SOCKAPI_       listen(SOCKET, int);
int  SOCKAPI_       recv(SOCKET, RTP_PFCHAR, int, int);
int  SOCKAPI_       recvfrom(SOCKET, RTP_PFCHAR, int, int, PSOCKADDR, RTP_PFINT);
/*int    recvmsg(int, struct msghdr *, int);   */
int  SOCKAPI_       select(int fd, PFDSET fread, PFDSET fwrite, PFDSET fexception,
                     PCTIMEVAL timeout);
int  SOCKAPI_       sendto(SOCKET, RTP_PFCCHAR, int, int, PCSOCKADDR, int);

/*   int SOCKAPI_ send(int, RTP_PFCCHAR, int, int);   */
#define send(socket, buffer, buf_len, flags)  \
    sendto((socket), (buffer), (buf_len), (flags), (PSOCKADDR)0, 0)

/*int    sendmsg (SOCKET, KS_CONSTANT struct msghdr *, int);   */
int  SOCKAPI_ setsockopt(SOCKET, int, int, RTP_PFCCHAR, int);
int  SOCKAPI_ shutdown(SOCKET, int);

unsigned long SOCKAPI_ inet_addr(RTP_PFCCHAR string);
RTP_PFCHAR        SOCKAPI_ inet_ntoa(struct in_addr in);

#if (INCLUDE_DNS)
PFPROTOENT  SOCKAPI_ getprotobynumber(int proto);
PFPROTOENT  SOCKAPI_ getprotobyname(RTP_PFCHAR name);
PFSERVENT   SOCKAPI_ getservbyname(RTP_PFCHAR name, RTP_PFCHAR proto);
PFSERVENT   SOCKAPI_ getservbyport(int port, RTP_PFCHAR proto);

PFHOSTENT SOCKAPI_ gethostbyname(RTP_PFCHAR name);
PFHOSTENT SOCKAPI_ gethostbyaddr(RTP_PFCHAR addr, int len, int type);

#if (INCLUDE_DNS_CACHE)
int  SOCKAPI_ xn_clear_host_cache(void);
#endif

int    SOCKAPI_ xn_add_dns_server(RTP_PFCUINT8 addr);
int    SOCKAPI_ xn_set_server_list(RTP_UINT32 list[], int num_elements);
int    SOCKAPI_ xn_add_host_table_entry(RTP_PFCHAR name, RTP_UINT32 ipaddr);
int    SOCKAPI_ xn_delete_host_table_entry(RTP_PFCHAR name);
int    SOCKAPI_ xn_has_dns_name(void);

#endif  /* INCLUDE_DNS */

int        SOCKAPI_ xn_has_domain_name(void);
RTP_PFCHAR SOCKAPI_ xn_get_domain_name(void);
int    SOCKAPI_ xn_set_domain_name(RTP_PFCCHAR name);

/*
 * <name> may be valid decimal dotted IP number of DNS resolvable name.
 *
 * Return converted IP number in 'ip_addr' and RTP_TRUE as function result.
 *
 * Return 'ip_ffaddr'/'ip_nulladdr' (255.255.255.255/0.0.0.0) in ip_addr and RTP_FALSE as function result on error!
 *
 * NOTE: Errors are:
 * - IP numbers 255.255.255.255 and 0.0.0.0 and anything that leads to these two
 * - 'name' cannot be resolved by DNS for any reason
 * - 'name' is not a decimal dotted number that matches IP address specifications (format: ddd.ddd.ddd.ddd)
 *          where 'ddd' may be any decimal number from 0..255
 *
 * Examples of unaccepted (--> 'faulty') inputs:
 *
 *   xn_name2ip_addr(xxxx, "255.255.255.255")           --> RTP_FALSE [255.255.255.255]
 *   xn_name2ip_addr(xxxx, "0.0.0.0")                   --> RTP_FALSE [0.0.0.0]
 *   xn_name2ip_addr(xxxx, "1000.0.0.0")                --> RTP_FALSE [255.255.255.255]
 *   xn_name2ip_addr(xxxx, "totally.unknown.dns.entry") --> RTP_FALSE [255.255.255.255]
 */
int    SOCKAPI_ xn_name2ip_addr(RTP_PFUINT8 ip_addr, RTP_PFCCHAR name);


#ifdef __cplusplus
}
#endif

#endif /* __SOCKET__ */

