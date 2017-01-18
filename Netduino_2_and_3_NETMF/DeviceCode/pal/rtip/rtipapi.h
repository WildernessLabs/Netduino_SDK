/*                                                                        */
/* RTIPAPI.H  - RTIP header file                                          */
/*                                                                        */
/*   EBS - RTIP                                                           */
/*                                                                        */
/*   Copyright Peter Van Oudenaren , 1993                                 */
/*   All rights reserved.                                                 */
/*   This code may not be redistributed in source or linkable object form */
/*   without the consent of its author.                                   */
/*                                                                        */
/*    Module description:                                                 */
/*        This file contains prototypes constants and structure           */
/*        declarations required to build RTIP libraries and applications. */
/*        There should be no need to change values in this file           */
/*        configuration values are in xnconf.h, rtipconf.h etc.           */
/*                                                                        */

#ifndef __XNRTIPAPI__
#define __XNRTIPAPI__ 1

#include "xnconf.h"

#if (RTPLATFORM)
#include "rtp.h"
#endif

//#include "vfconf.h"
#include "rtipconf.h"
//#include "netutil.h"

/* *********************************************************************   */
/* GENERAL DEFINES                                                         */
/* *********************************************************************   */
#define BOOTP_FILE_LEN 128

// I/O ADDRESS (can be different types in different systems so we
// generally define as a RTP_HANDLE and will cast in the porting
// layer
#define IOADDRESS RTP_HANDLE

/* DO NOT CHANGE: change ARGSUSED instead   */
#ifdef ARGSUSED_ASSIGN
#define ARGSUSED_PVOID(x)  x=x;
#define ARGSUSED_INT(x)    x=x;
#else
#ifdef ARGSUSED_FUNC
#define ARGSUSED_PVOID(x)  argsused_pvoid((PFVOID)x);
#define ARGSUSED_INT(x)    argsused_int((int)x);
#endif
#endif

#ifndef ARGSUSED_PVOID
#define ARGSUSED_PVOID(p)
#endif

#ifndef ARGSUSED_INT
#define ARGSUSED_INT(i)
#endif


/* *********************************************************************   */
/* enables using code as DLL                                               */
/* NOTE: the same define is in socket.h                                    */
#define SOCKAPI_
/*#define SOCKAPI_  __stdcall __export   */

/* *********************************************************************   */
#define IP_ALEN     4               /* number of bytes in an IP address */
#define IPV6_ALEN   16              /* IPV6 address length */
#define ETH_ALEN    6               /* number of bytes in an ETHERNET address */

#define IP_LEN      IP_ALEN

#define SETIPADDR(TO, A,B,C,D)                          \
    *TO = A; *(TO+1) = B; *(TO+2) = C; *(TO+3) = D;

/* address addition   */
#define ADDR_ADD(addr1, addr2)  ( (RTP_UINT32)(addr1) + (RTP_UINT32)(addr2) )
/* address difference   */
#define ADDR_DIFF(addr1, addr2) ( (RTP_UINT32)(addr1) - (RTP_UINT32)(addr2) )

// Structure copy causes call to memcpy(), and we can't have that on
// SH1 platform
// tbd
#if (SH1)
#define STRUCT_COPY(x, y) rtp_memcpy((RTP_PFUINT8)&(x), (RTP_PFUINT8)&(y), sizeof(y))
#else
#define STRUCT_COPY(x, y) x = y
#endif

/* *********************************************************************   */
/* POS_LIST                                                                */
/* *********************************************************************   */

#define ZERO_OFFSET     0

typedef struct os_list
{
    struct os_list RTP_FAR *pnext;
    struct os_list RTP_FAR *pprev;
} OS_LIST;
typedef struct os_list RTP_FAR * POS_LIST;   /* head of list */

/* ********************************************************************   */
/* Add-on includes                                                        */
/* ********************************************************************   */
#if (INCLUDE_DHCP_CLI)
    #include "dhcpcapi.h"
#endif
#if (INCLUDE_DHCP_SRV)
    #include "dhcpsapi.h"
#endif

#if (INCLUDE_FTP_SRV)
    #include "ftpapi.h"
#endif

#if (INCLUDE_TELNET_SRV)
    #include "telsapi.h"
#endif

#if (INCLUDE_SNMP)
    #include "snmpapi.h"
#endif      /* end of if SNMP */
#if (INCLUDE_WEB)
    #include "webapi.h"
#endif
#if (INCLUDE_PPP)
    #include "pppapi.h"
#endif

/* ****************************************************************************   */
/* ******                 ERRNO and REPORT_ERROR                          *****   */
/* NOTE: see rtip manual for functions which set errno                            */
/* ****************************************************************************   */

#define RTIP_ERRNO       0      /* allows errno values to be offset */
                                /* NOTE: the errno values in the Reference   */
                                /*       Manual correspond to this value     */
                                /*       being 0                             */

#define EADDRNOTAVAIL    (1+RTIP_ERRNO) /* endpoint address not available */
#define EADDRINUSE       (2+RTIP_ERRNO) /* address in use */
#define EAFNOSUPPORT     (3+RTIP_ERRNO) /* family not supported */
#define EARPFULL         (4+RTIP_ERRNO) /* arp table full */
#define EBADBAUD         (5+RTIP_ERRNO) /* invalid baud rate */
#define EBADCOMMNUM      (6+RTIP_ERRNO) /* invalid comm port number */
#define EBADDEVICE       (7+RTIP_ERRNO) /* invalid device type */
#define EBADIFACE        (8+RTIP_ERRNO) /* invalid interface number */
#define EBADMASK         (9+RTIP_ERRNO) /* invalid mask (ether must not be all fs) */
#define EBADRESP        (10+RTIP_ERRNO) /* invalid ping response */
#define ECONNREFUSED    (11+RTIP_ERRNO) /* endpoint refused connection */
#define EDESTADDREQ     (12+RTIP_ERRNO) /* destination address is required */
#define EDESTUNREACH    (13+RTIP_ERRNO) /* destination unreachable (ICMP) */
#ifndef EFAULT
#define EFAULT          (14+RTIP_ERRNO) /* invalid parameter (pointer is 0) */
#endif
#define EIFACECLOSED    (15+RTIP_ERRNO) /* interface closed */
#define EIFACEFULL      (16+RTIP_ERRNO) /* interface table full */
#define EIFACEOPENFAIL  (17+RTIP_ERRNO) /* interface open failed */
#define EINPROGRESS     (18+RTIP_ERRNO) /* operation would block on */
                                        /* non-blocking socket               */
                                        /* same value as EWOULDBLOCK         */
                                        /* EWOULDBLOCK is defined by BSD     */
                                        /* EINPROGRESS is defined by WINSOCK */

#ifndef EINVAL
#define EINVAL          (19+RTIP_ERRNO) /* invalid function call (parameter) */
#endif
#define EISCONN         (20+RTIP_ERRNO) /* socket is already connected */
#define EMCASTFULL      (21+RTIP_ERRNO) /* multicast table full */
#define EMCASTNOTFOUND  (22+RTIP_ERRNO) /* multicast address not found */
#ifndef EMFILE
#define EMFILE          (23+RTIP_ERRNO) /* out of ports */
#endif
#define ENETDOWN        (24+RTIP_ERRNO) /* network is down (send failed) */
#define ENETUNREACH     (25+RTIP_ERRNO) /* network unreachable (keepalive failed) */
#define ENOPKTS         (26+RTIP_ERRNO) /* out of DCUs (packets) */
#define ENOPROTOOPT     (27+RTIP_ERRNO) /* option parameter is invalid */
#define ENOTCONN        (28+RTIP_ERRNO) /* socket is not connected */
#define ENOTINITIALIZED (29+RTIP_ERRNO) /* RTIP not initialized (i.e. xn_rtip_init */
                                        /* not called)   */
#define ENOTSOCK        (30+RTIP_ERRNO) /* invalid socket descriptor */
#define ENUMDEVICE      (31+RTIP_ERRNO) /* not enough devices (see CFG_NUM_ED, */
                                        /* CFG_NUM_SMCX, NUM_3C, NUM_RS232 etc)   */
#define EOPNOTSUPPORT   (32+RTIP_ERRNO) /* socket type or specified operation not */
                                        /* supported for this function   */
#define EOUTPUTFULL     (33+RTIP_ERRNO) /* send failed due to output list */
                                        /* being full   */
#define EPROBEFAIL      (34+RTIP_ERRNO) /* could not determine device */
#define ERENTRANT       (35+RTIP_ERRNO) /* non-reentrancy error */
#define ERTNOTFOUND     (36+RTIP_ERRNO) /* routing table entry not found */
#define ERTFULL         (37+RTIP_ERRNO) /* routing table full */
#define ERSCINITFAIL    (38+RTIP_ERRNO) /* resource initialization failed */
#define ESHUTDOWN       (39+RTIP_ERRNO) /* illegal operation due to socket shutdown */
#define ETIMEDOUT       (40+RTIP_ERRNO) /* timeout */
#define ETNOSUPPORT     (41+RTIP_ERRNO) /* type not supported (only SOCK_STREAM and */
                                        /* SOCK_DGRAM are supported)   */
#define EWOULDARP       (42+RTIP_ERRNO) /* send needs to ARP but ARP is disabled */
                                        /* (see INCLUDE_ARP)   */
#define EWOULDBLOCK     EINPROGRESS
                                        /* socket non-blocking but function would   */
                                        /* block                                    */
#define EOUTAMEM        (43+RTIP_ERRNO) /* not enough heap memory for allocate request */
                                        /* (rtp_malloc failed)   */
#define ETABLEFULL      (44+RTIP_ERRNO) /* table full (such as ARP cache, device table etc) */
#define EMSGSIZE        (45+RTIP_ERRNO) /* Illegal sized packet */
#define EDEVOPENFAIL    (46+RTIP_ERRNO) /* device open failed */
#define EPFNOSUPPORT    (48+RTIP_ERRNO) /* family not supported */
#define EPROTONOSUPPORT (49+RTIP_ERRNO) /* protocol not supported */
#define EACCES          (50+RTIP_ERRNO) /* access denied on socket (e.g. broadcast attempt */
                                        /* over socket with broadcast disabled) */
#define EOUTADDREC      (51+RTIP_ERRNO) /* no address records available */
#define EBADADDR        (52+RTIP_ERRNO) /* SPRC-SPRC-SPRC */
#define EBADADDRINFO    (53+RTIP_ERRNO) /* SPRC-SPRC-SPRC */
#define ETODNOTSET      (54+RTIP_ERRNO) /* TOD not set */


/* DNS and database   */
#define EHTABLEFULL     (100+RTIP_ERRNO)    /* The global host table is full */
#define EENTRYNOTFOUND  (101+RTIP_ERRNO)    /* The host table entry wasn't found */
#define ETOOMANYSERVERS (102+RTIP_ERRNO)    /* For xn_set_server_list(). Means */
                                            /* # of servers is greater than   */
                                            /* CFG_MAX_DNS_SERVERS            */
#define ENO_RECOVERY    (103+RTIP_ERRNO)    /* DNS server returned error */
#define ENAME_TOO_LONG  (104+RTIP_ERRNO)    /* DNS name is too long */
#define ETRYAGAIN       (105+RTIP_ERRNO)    /* DNS call to socket function failed */
#define ENODATA         (106+RTIP_ERRNO)    /* data not available */
#define ENORESPONSE     (107+RTIP_ERRNO)    /* socket call within DNS failed; probably */
                                            /* No response received from server   */
#define ENOSERVERS      (108+RTIP_ERRNO)    /* No server in server table */

/* MODEM CONTROL   */
#define EMODEMNORING     (109+RTIP_ERRNO) /* No ring received in specified time */
#define EMODEMSENDFAILED (110+RTIP_ERRNO) /* sending char failed */
#define EMODEMBADRESP    (111+RTIP_ERRNO) /* unexpected response from modem */

/* PPP   */
#define EPPPFULL        (200+RTIP_ERRNO)    /* PPP - table full */
#define EPPPNOTOPEN     (201+RTIP_ERRNO)    /* PPP - not open for IP traffic (IPCP not */
                                            /*       open)   */
#define EPPPLINEDOWN    (202+RTIP_ERRNO)    /* PPP - line not up */
#define EPPPNOLINKHDR   (203+RTIP_ERRNO)    /* PPP - link header missing */
#define EPPPBADPKT      (204+RTIP_ERRNO)    /* PPP - bad IP packet */
#define EPPPTIMEDOUT    (205+RTIP_ERRNO)    /* PPP - LCP open timed out */
#define EPPPNOTCLOSED   (206+RTIP_ERRNO)    /* PPP - previous CLOSE not done */

/* SNMP   */
#define ESNMPFOUND      (300+RTIP_ERRNO)    /* SNMP - trap manager already in table */
#define ESNMPFULL       (301+RTIP_ERRNO)    /* SNMP - trap manager table full or */
                                            /*        too many community names   */
#define ESNMPNOTFOUND   (302+RTIP_ERRNO)    /* SNMP - trap manager not found in table */
#define ESNMPSEND       (303+RTIP_ERRNO)    /* SNMP - sending the response to a request */
                                            /*        failed   */
#define ETOOSMALL       (304+RTIP_ERRNO)    /* SNMP - insufficient storage */

/* WEB, FTP, TFTP Servers   */
#define ERTFSINITFAIL   (400+RTIP_ERRNO)    /* WEB - RTFS - pc_memory_init failed */

/* DHCP   */
#define EDHCPSERVNORESP   (500+RTIP_ERRNO) /* DHCP - DHCP server not responding */
#define EDHCPSERVBADRESP  (501+RTIP_ERRNO) /* DHCP - Bad response from server (NI) */
#define EDHCPSERVNOOFFER  (502+RTIP_ERRNO) /* DHCP - no DHCPOFFERS from server met */
                                           /*        user's criteria   */
#define EDHCPSERVNOACK    (503+RTIP_ERRNO) /* DHCP - acknowledgement not received (NI) */
#define EDHCPSERVNOACKNAK (504+RTIP_ERRNO) /* DHCP - nak not received (NI) */
#define EDHCPOPNOTFOUND   (505+RTIP_ERRNO) /* DHCP - option not found in server */
                                           /*        response   */
#define EDHCPOPOVERFLOW   (506+RTIP_ERRNO) /* DHCP - size of option exceeds allocated */
                                           /*        buffer   */
#define EDHCPOPBADLEN     (507+RTIP_ERRNO) /* DHCP - bad length (NI) */
#define EDHCPBADPARAM     (508+RTIP_ERRNO) /* DHCP - bad parmameter (NI) */
#define EDHCPRELEASE      (509+RTIP_ERRNO) // DHCP - failed since address released by xn_release
#define EDHCPBADADDR      (510+RTIP_ERRNO) // DHCP - DHCP returned address in use

/* NFS SERVER   */
#define ENFSEXPORTFOUND     (600+RTIP_ERRNO)  /* NFS - entry already in export table */
#define ENFSEXPORTFULL      (601+RTIP_ERRNO)  /* NFS - export table is full */
                                              /*       (CFG_NS_EXPORT_DIRS)   */
#define ENFSEXPORTNOTFOUND  (602+RTIP_ERRNO)  /* NFS - entry not found export table */
#define ENFSNOBUFS          (603+RTIP_ERRNO)  /* NFS - out of server structures (svcxprt */
                                              /*       or svc_buf (CFG_NSERVERS)   */
#define ENFSNOPROC          (604+RTIP_ERRNO)  /* NFS - out of procedure structures */
                                              /*       (CFG_NUM_PROC)   */
#define ENFSNOPROG          (605+RTIP_ERRNO)  /* NFS - out of program structures */
                                              /*       (CFG_NUM_PROGRAM)   */
#define ENFSPMAPFULL        (606+RTIP_ERRNO)  /* NFS - port map table full (CFG_MAX_PMAP) */
#define ENFSREGERR          (607+RTIP_ERRNO)  /* NFS - program register error */
#define ENFSUSERFOUND       (608+RTIP_ERRNO)  /* NFS - user already in unix user table */
#define ENFSUSERFULL        (609+RTIP_ERRNO)  /* NFS - unix user table is full */
                                              /*       (CFG_NS_UNIX_USERS)   */
#define ENFSUSERNOTFOUND    (610+RTIP_ERRNO)  /* NFS - user not found unix user table */

/* SMTP and POP   */
#define ESRVBADRESP         (700+RTIP_ERRNO)  /* SMTP/POP3 server sent invalid response */
                                              /* or error response   */
#define ESRVDOWN            (701+RTIP_ERRNO)  /* SMTP/POP3 server not responding */

/* TFTP   */
#define EREQNOTSUPPORT      (800+RTIP_ERRNO)    /* TFTP Server - requested from */
                                                /* client unrecognized or not     */
                                                /* supported (i.e. not            */
                                                /* Read request or Write request) */
#define EBADREQ             (801+RTIP_ERRNO)    /* TFTP Server - request */
                                                /* incorrectly formatted   */
#define EMODENOTSUPPORT     (802+RTIP_ERRNO)    /* TFTP Server - mode in request */
                                                /* was unsupported or unrecognized   */
#define EIOERR              (803+RTIP_ERRNO)    /* TFTP Server - file system */
                                                /* error   */
#define ESENDFAILED         (804+RTIP_ERRNO)    /* TFTP Server - send did not */
                                                /* send the requested number   */
                                                /* of bytes                    */
#define ECLIERROR           (805+RTIP_ERRNO)    /* TFTP Server - client sent */
                                                /* an error response   */
#define ETFTPERROR          (806+RTIP_ERRNO)    /* TFTP Client - TTFTPERROR */
                                                /* error response received from   */
                                                /* server                         */

/* FILE I/O LAYER   */
#define EFOPENFAILED        (900+RTIP_ERRNO)    /* File open failed */
#define EFREADFAILED        (901+RTIP_ERRNO)    /* File read failed */
#define EFWRITEFAILED       (902+RTIP_ERRNO)    /* File write failed */
#define EFNODIRENT          (903+RTIP_ERRNO)    /* Out of directory entries */
#define EFINVALIDMODE       (904+RTIP_ERRNO)    /* Invalid mode for operation
                                                   requested */
#define EFNOFILE            (905+RTIP_ERRNO)    /* File does not exist */
#define ENOVNODE            (906+RTIP_ERRNO)    /* vnode does not exist */
#define EFVNODEINITFAIL     (907+RTIP_ERRNO)    /* vnode init failed */
#define EFNOPARENT          (908+RTIP_ERRNO)    /* parent not found */
#define EFNOPATH            (909+RTIP_ERRNO)    /* path not found */
#define EFNOSTATIC          (910+RTIP_ERRNO)    /* STATIC not configured */
#define EFNOSUBDIRS         (911+RTIP_ERRNO)    /* SUBDIRS not configured */
#define EFNOTDIR            (912+RTIP_ERRNO)    /* not a directory */
#define EFNOMTPOINTS        (913+RTIP_ERRNO)    /* no mount points */

/* *********************************************************************   */
/* ***** definition of who called os_alloc_packet (passed as a parameter); */
/* ***** used for tracking packets (see INCLUDE_TRK_PACKETS)               */
/* ***** NOTE: also passed to xn_malloc                                    */
/* *********************************************************************   */
#define ARP_SEND_ALLOC              1        /* arp.c */
#define CAST_ALLOC                  2        /* arp.c */
#define RARP_SEND_ALLOC             3        /* arp.c */
#define DE4X5_ALLOC                 4        /* de4x5.c */
#define DNS_STRING_ALLOC            5        /* dns.c */
#define DNS_SPACE_ALLOC             6        /* dns.c    */
#define DNS_HOST1_ALLOC             7        /* dns.c */
#define DNS_HOST2_ALLOC             8        /* dns.c */
#define DNS_HOST3_ALLOC             9        /* dns.c */
#define FRAG_ALLOC                  10       /* fragment.c */
#define ICMP_ALLOC                  11       /* icmp.c */
#define IGMP_ALLOC                  12       /* igmp.c */
#define DRIVER_ALLOC                13       /* drivers, e.g. ifed.c etc */
#define PPP_ALLOC                   21       /* ppp.c */
#define FSM_TR_ALLOC                22       /* pppfsm.c */
#define FSM_TA_ALLOC                23       /* pppfsm.c */
#define IPCP_MR_ALLOC               24       /* pppipcp.c */
#define IPCP_REPLY_ALLOC            25       /* pppipcp.c */
#define LCP_MR_ALLOC                26       /* ppplcp.c */
#define LCP_REPLY_ALLOC             27       /* ppplcp.c */
#define PAP_MR_ALLOC                28       /* ppppap.c */
#define PAP_REPLY_ALLOC             29       /* ppppap.c */
#define CHAP_MR_ALLOC               30       /* pppchap.c */
#define CHAP_REPLYC_ALLOC           31       /* pppchap.c */
#define CHAP_REPLYR_ALLOC           32       /* pppchap.c */
#define ECHO_ALLOC                  33       /* pppfsm.c */
#define RS232_IN_ALLOC              34       /* rs232.c */
#define RS232_OUT1_ALLOC            35       /* rs232.c */
#define RS232_OUT2_ALLOC            36       /* rs232.c */
#define RS232_BUF_ALLOC             37       /* rs232.c */
#define PING_ALLOC                  39       /* rtipapi.c */
#define USER_ALLOC                  40       /* rtipapi.c */
#define MODEM_ALLOC                 41       /* rtipapi.c */
#define MODEM_ANS_ALLOC             42       /* rtipapi.c */
#define SLIP_ALLOC                  43       /* slip.c */
#define SENDTO_ALLOC                45       /* socket.c */
#define TCP_RESET_ALLOC             46       /* tcp.c */
#define TCP_OUTPUT_ALLOC            47       /* tcp.c */
#define TCP_QUE_IN_ALLOC            48       /* tcp.c */
#define TCP_QUE_OUT_ALLOC           49       /* tcp.c */
#define RAW_RCV                     50       /* udp.c */
#define LINE_INIT1_ALLOC            51       /* rtipapi.c */
#define LINE_INIT2_ALLOC            52       /* rtipapi.c */
#define LINE_PUT_ALLOC              53       /* rtipapi.c */
#define RS232_RAW_ALLOC             54       /* rs232.c */
#define CLOSE_IFACE_ALLOC           58       /* iface.c */
#define DCU_ALLOC_CORE_ALLOC        59       /* os.c */
#define RD_SOLICITE_SEND_ALLOC      60       /* icmp.c */
#define NFS_AUTHU_ALLOC             100      /* ncauth.c */
#define NFS_DYN_ALLOC               101      /* nfs_util.c */
#define NFS_UNIVERSAL_ALLOC         102      /* nsrpc.c */
#define NFS_MNT_ALLOC               103      /* nsmount.c */
#define NFS_UMNT_ALLOC              104      /* nsmount.c */
#define NFS_LOOKUP_ALLOC            105      /* nsnfs.c */
#define NFS_CREATE_ALLOC            106      /* nsnfs.c */
#define NFS_REMOVE_ALLOC            107      /* nsnfs.c */
#define NFS_RENAME_FROM_ALLOC       108      /* nsnfs.c */
#define NFS_RENAME_TO_ALLOC         109      /* nsnfs.c */
#define NFS_MKDIR_ALLOC             110      /* nsnfs.c */
#define NFS_RMDIR_ALLOC             111      /* nsnfs.c */
#define NFS_READDIR_ALLOC           112      /* nsnfs.c */
#define FTP_DAEMON_ALLOC            113      /* ftpsrv.c */
#define SNMP_IO_ALLOC               114      /* snmpapi.c */
#define SNMP_RQD_ALLOC              115      /* snmpapi.c */
#define SNMP_DYN_ALLOC              116      /* snmputil.c */
#define SNMP_SNB2A_ALLOC            117      /* snmputil.c */
#define SNMP_RSLV_ALLOC             118      /* snmputil.c */
#define SNMP_MKSNMP_ALLOC           119      /* snmpproc.c */
#define SNMP_MKTRAP_ALLOC           120      /* snmpproc.c */
#define SNMP_MKTRAPV2_ALLOC         121      /* snmpproc.c */
#define SNMP_MKTRAPV3_ALLOC         122      /* snmpproc.c */
#define WEB_USER_ALLOC              124      /* in webpage directory */
#define WEB_MHDR_ALLOC              125      /* webproc.c */
#define WEB_XFER_ALLOC              126      /* webproc.c */
#define RIP_ALLOC                   127      /* rip.c */
#define SMTP_FORMAT_ALLOC           128      /* smtp.c */
#define SSL_CRYPTO_ALLOC            129      /* crypto/rsa */
#define SLHC_ALLOC1                 201      /* slhc.c */
#define SLHC_ALLOC2                 202      /* slhc.c */
#define DHCPS_ALLOC                 203      /* dhcps.c */
#define TNC_ALLOC                   204      /* telnetc.c */
#define PPPOE_ALLOC                 205      /* pppoe.c */


/* *********************************************************************  */
/* ERROR_REPORT is callback routine to the application to process certain */
/* errors/events (see callback.c)                                         */

/* reasons for ERROR_REPORT   */
#define TCP_RETRANS             1   /* excessive retransmissions */
#define TCP_CONNECTION_CLOSING  2   /* FIN received from otherside */
#define ICMP_QUENCH             3   /* ICMP quench message received */
#define ICMP_UNREACH            4   /* ICMP unreachable message received */
#define ICMP_TIME_EXCEEDED      5   /* ICMP time exceeded message received */
#define ICMP_PARAM_PROBLEM      6   /* ICMP parameter problem message */
                                    /* received   */

/* subreasons for ERROR_REPORT                 */
/* for ICMP, subreason is code in ICMP message */

/* *********************************************************************   */
/* TASKS                                                                   */
/* *********************************************************************   */
/* RTIP's os_spawn_task() calls spanws tasks that ultimately call functions
  of type void func(void). This is a pointer to a function of that type
  and is used in the argument list to os_spawn_task() */
typedef void (*PEBS_TASKENTRY)(void);

/* *********************************************************************    */
/* *********************************************************************   */
/* SNMP                                                                    */
/* *********************************************************************   */
#define USE_OFFSET 0         /* DO NOT CHANGE */
#if (USE_OFFSET)
#define MIB_VAR(X)     (rc->mib_vars.X)
#define MIB_VAR_OFF(X) ((RTP_PFCHAR)STRUCT_OFFSET(rtip_context,mib_vars.X))
#else
#define MIB_VAR(X)     mib_vars.X
#endif

/* *********************************************************************   */
/* INTERFACE OPTIONS                                                       */
/* *********************************************************************   */
#define IO_DEFAULT_MCAST  0x0001    /* default mcast interface to listen */
                                    /* on   */
#define IO_MAX_OUTPUT_QUE 0x0002    /* maximum number of packets which can be */
                                    /* queued on output list     */
                                    /* (-1 = no limit = default) */
#define IO_INPUT_WINDOW   0x0004    /* input window size */
#define IO_OUTPUT_WINDOW  0x0008    /* output window size */
#define IO_MTU            0x0010    /* MTU (max transfer unit) */
#define IO_802_2          0x0020    /* send 802.2 packets over the interface */
#define IO_HARD_CLOSE     0x0040    /* Do a hard interface close; i.e. don't */
                                    /* wait for packets to be xmitted   */

/* *********************************************************************   */
/* BASE 64 ENCODING/DECODING STUFF                                         */
/* *********************************************************************   */
#define BASE64_LINE_LENGTH 76

/* calculates the maximum length of an encoded string given the length   */
/* if the input (i.e. the unencoded string)                              */
/* For example, if you want to encode read_buf which is 512 bytes into   */
/* base64_buf, you would declare the 2 buffers as follows:               */
/*  RTP_UINT8 read_buf[512];                                             */
/*  RTP_UINT8 base64_buf[BASE64_MAX_ENCODED_SIZE(512)];                  */
#define BASE64_MAX_ENCODED_SIZE(input_size)     \
(((((unsigned long)input_size)/3ul+1ul)*4ul)+2*(((((unsigned long)input_size)/\
3ul+1ul)*4ul)/BASE64_LINE_LENGTH+1ul))

typedef struct
{
    RTP_UINT8 quad[4];
    RTP_UINT8 bytes_in_quad;
} base64_decode_context;
typedef base64_decode_context RTP_FAR *PBASE64_DECODE_CONTEXT;

/* *********************************************************************   */
/* ******              DEVICE TABLE                               ******   */
/* *********************************************************************   */
typedef RTP_BOOL (*DEV_OPEN)(void *pi);
typedef void    (*DEV_CLOSE)(void *pi);
typedef int     (*DEV_XMIT)(void *pi, void * msg);
typedef RTP_BOOL (*DEV_XMIT_DONE)(void *pi, void * msg, RTP_BOOL success);
typedef void    (*DEV_PROCESS_INTERRUPTS)(void *pi);
typedef RTP_BOOL (*DEV_STATS)(void *pi);
typedef RTP_BOOL (*DEV_SETMCAST)(void *pi);

/* NOTE: 0 is an invalid device ID   */
#define NE2000_DEVICE           1
#define NE2000_PCMCIA_DEVICE    2
#define SMC8XXX_DEVICE          3
#define EL3_DEVICE              4   /* 3C503 */
#define SMC91C9X_DEVICE         5
#define SMC91C9X_PCMCIA_DEVICE  6
#define DE4X5_DEVICE            7
#define EL9_DEVICE              8   /* 3C509 */
#define EL89_PCMCIA_DEVICE      9   /* 3C589 */
#define LANCE_DEVICE            10  /* LANCE */
#define EEXP_DEVICE             11  /* Intel 82586 - EtherExpress */
#define EEPRO_DEVICE            12  /* Intel 82595 - EtherExpress PRO */
#define PKT_DEVICE              13
#define SLIP_DEVICE             14
#define CSLIP_DEVICE            15
#define PPP_DEVICE              16
#define PPPOE_DEVICE            17
#define ETH360_DEVICE           18  /* Motorola 68EN360 - 68K only */
#define ETH860_DEVICE           19  /* Motorola mpc860 or mpc821 */
#define CS89X0_DEVICE           20  /* Crystal CS89[02]0 (cs89x0.c) */
#define LAN_CS89X0_DEVICE       21  /* Crystal CS89[02]0 (lan8900.c) */
#define MB86964_DEVICE          22  /* Mitsubishi MB86964 */
#define CE3_PCMCIA_DEVICE       23  /* XIRCOM */
#define I82559_DEVICE           24  /* Intel Etherexpress PRO+ and 82559ER */
#define R8139_DEVICE            25  /* Realtek 8139 */
#define TC90X_DEVICE            26  /* 3com 905/90x */
#define N83815_DEVICE           27  /* National 83815 */
#define SBC5307_DEVICE          28 /* Motorola ColdFire 5307 */
#define NIL_ETHER_DEVICE        29  /* used for testing purposes */
#define MTOKEN_DEVICE           30
#define LANCE_ISA_DEVICE        31  /* LANCE */
#define WINETHER_DEVICE         32  /* WINETHER - Raw ether on win32 */
#define LOOP_DEVICE             33
#define PRISM_DEVICE            34  /* Prism based PCI cards */
#define PRISM_PCMCIA_DEVICE     35  /* Prism based PCMCIA cards */
#define DAVICOM_DEVICE          36  /* DM9102A PCI card */
#define SE7709A_DEVICE          37  /* DP83902 on Hitachi SH SolutionEngine board */
#define SMC8041_PCMCIA_DEVICE   38  /* in ne2000.c */
#define ETH5272_DEVICE          39  /* Motorola FEC on ColdFire 5272 */ 
#define ETH5282_DEVICE          40  /* Motorola FEC on ColdFire 5272 */ 
#define ETH405_DEVICE           41  /* IBM 405GP */  
#define TCFE574_PCMCIA_DEVICE   42  /* 3Com 3CFE574 PCMCIA */ 
#define DM9000_DEVICE           43  /* DM9000 ISA Ethernet Controller */
#define WINPCAP_DEVICE          44  /* WINPCAP - Raw ether on win32 */
     
#define LOOP_ETHER_DEVICE       45
#define AX772_DEVICE            46
#define RHINE_DEVICE            47
#define AX172_DEVICE            48
#define ENC28J60_DEVICE         49
#define SH7619_EDMAC_DEVICE    	50

#define FIRST_USER_DEVICE    10000  /* first device ID for driver added */
                                    /* by user   */

#define ETHER_IFACE         1   /* ethernet */
#define RS232_IFACE         2   /* rs232 (SLIP, CSLIP or PPP) */
#define LOOP_IFACE          3   /* loop back */
#define TOKEN_IFACE         4   /* Token Ring */
#define IEEE_802_11_IFACE   5   /* 802.11 */

#define MINOR_0         0   /* first device of same device type */
#define MINOR_1         1   /* second device of same device type */
#define MINOR_2         2   /* third device of same device type */
#define MINOR_3         3   /* fourth device of same device type */

/* *********************************************************************   */
/* ******          interface statistics                           ******   */
/* *********************************************************************   */
/* statistics structure locally by ethernet (ED, SMCX etc) drivers         */
struct ether_statistics
{
    long packets_in;
    long packets_out;
    long bytes_in;
    long bytes_out;
    long errors_in;
    long errors_out;
    long packets_lost;
    long one_collision;             /* one collison occurred, xmit ok */
    long multiple_collisions;       /* more than one collision, xmit ok */
    long owc_collision;             /* out of window collison occurred, */
                                    /* xmit ok   */
    long collision_errors;          /* excessive collisions, xmit aborted */
    long rx_overwrite_errors;       /* ring buffer overflow */
    long rx_frame_errors;           /* packet to large */
    long rx_crc_errors;
    long rx_fifo_errors;            /* rx fifo overrun */
    long rx_other_errors;           /* other errors not counted in SNMP */
                                    /* statistics (includes ring buffer   */
                                    /* corrupt, ring buffer overflow,     */
                                    /* frame alignment error, FIFO        */
                                    /* overrun, missed packet             */
    long tx_carrier_errors;         /* carrier sense errors */
    long tx_fifo_errors;            /* fifo underrun */
    long tx_delayed;                /* xmit delayed due to medium busy */
    long tx_sqe_errors;             /* SQE Error */
    long tx_other_errors;           /* other errors not counted in SNMP */
                                    /* statistics (includes xmit timeout,      */
                                    /* remote transmit DMA failed to complete, */
                                    /* bus error                               */
};
typedef struct ether_statistics RTP_FAR * PETHER_STATS;

struct rs232_statistics
{
    long packets_in;
    long packets_out;
    long bytes_in;
    long bytes_out;
    long errors_in;
    long errors_out;
    long packets_lost;
    long overrun_errors_in;
    long parity_errors_in;
    long framing_errors_in;
};
typedef struct rs232_statistics RTP_FAR * PRS232_STATS;

/* ********                                */
/* Statistics accumulated during operation */
typedef struct _iface_stats
{
    /* DRIVER LAYER   */
    long interface_packets_in;
    long interface_packets_out;
    long interface_bytes_in;
    long interface_bytes_out;
    long interface_errors_in;       /* # pkts dropped by driver due to errors */
    long interface_errors_out;
    long interface_packets_lost;   /* input packets lost (alloc fails etc) */

    /* IP/PROTOCOL LAYER   */
    long rcv_packets;
    long rcv_bytes;
    long rcv_ucast;
    long rcv_nucast;
    long xmit_errors;
    long xmit_packets;
    long xmit_bytes;
    long xmit_ucast;
    long xmit_nucast;
    long rcv_ip_packets;
    long ip_fragments;
    long ip_cksum;
    long ip_dropped;                /* not ours, drop the packet */
    long ip_routed;                 /* # pkts simple router rerouted */
    long ip_options;
    long ip_unknown;                /* unknown protocol (not TCP, UDP or ICMP) */

    /* ARP/RAP LAYER   */
    long rcv_arp_packets;
    long rcv_rarp_packets;
    long unk_packets;               /* not IP, RARP or BOOTP */

    /* TCP LAYER   */
    long rcv_tcp_packets;
    long tcp_cksum;
    long tcp_dropped;               /* no ports found */
    long tcp_retries;               /* # pkts retransmitted */
    long tcp_window_probes;
    long tcp_keepalives;
    long tcp_keepalive_retries;
    long tcp_duplicates;
    long tcp_options;
    long tcp_options_us;            /* unsupported options (ignored) */

    /* UDP LAYER   */
    long rcv_udp_packets;
    long udp_cksum;
    long udp_dropped;               /* no port found */
    long udp_que_full;              /* input queue is full */
    long raw_que_full;              /* input queue is full */
    long udp_sends;
    long udp_send_bytes;
    long raw_sends;
    long rcv_raw_packets;
    long raw_send_bytes;

    /* ICMP LAYER   */
    long icmp_ping_send;
    long icmp_ping_answered;
    long icmp_packets;
    long icmp_cksum;
    long icmp_unreach;
    long icmp_quench;
    long icmp_redir;
    long icmp_echo_requests;
    long icmp_time_exceeded;
    long icmp_param_problem;
    long icmp_not_handled;
} IFACE_STATS;
typedef struct _iface_stats RTP_FAR * PIFACE_STATS;

#if (INCLUDE_TRK_PKTS)
/* *********************************************************************   */
/* ******    DCU STATISTICS                                       ******   */
/* *********************************************************************   */
typedef struct track_dcu
{
    int num_dropped;
#if (INCLUDE_MALLOC_DCU_AS_NEEDED)
    int dcu_cnt;  
#endif

#if (INCLUDE_RUN_TIME_CONFIG)
    RTP_BOOL *dcu_table_ptr;
#else
    RTP_BOOL dcu_table_ptr[CFG_NDCUS];
#endif

	int total_dcu_cnt; 

    int frag_tbl;
    int udp_exch;
    int out_wind;
    int in_wind;
    int ip_exch;
    int ping_exch;
    int input_list;
    int output_list;
    int free_list;
    int arp_cache;
    int rs232_info;
} TRACK_DCU;
typedef struct track_dcu RTP_FAR * PTRACK_DCU;
#endif /* INCLUDE_TRK_PKTS */


/* *********************************************************************   */
/* ******    register initialization routines                     ******   */
/* *********************************************************************   */

typedef void (*REGISTER_PROC)(void);

typedef struct _init_fncs RTP_FAR * P_INIT_FNCS;
typedef struct _init_fncs
{
    REGISTER_PROC init_fnc;
    P_INIT_FNCS   nxt_fnc;
} INIT_FNCS;

/* *********************************************************************   */
/* ******                CONFIGURATION                            ******   */
/* *********************************************************************   */
#if (INCLUDE_RUN_TIME_CONFIG)   /* tbd - spr */
typedef struct cfg_limits_data
{
#if (INCLUDE_MALLOC_PORT_LIMIT)
    int total_sockets;
#endif
    int total_server_threads;
#if (INCLUDE_MALLOC_DCU_AS_NEEDED)
    int total_dcus;
#endif
} CFG_LIMITS_DATA;

RTP_EXTERN CFG_LIMITS_DATA RTP_FAR cfg_limits_data;
RTP_EXTERN CFG_LIMITS_DATA RTP_FAR num_alloced_data;

#endif

typedef struct cfg_ip_data
{
    RTP_UINT32 ip_ttl_val;              /* time-to-live for IP packets */
                                    /* modified by SNMP only   */
#if (INCLUDE_RUN_TIME_CONFIG)
    int   ip_frag_ttl;              /* Time-to-Live for fragment table */
                                    /* entries (in seconds)   */
    int   max_frag;                 /* Maximum fragment size (in bytes) will */
                                    /* process for all protocols              */
                                    /* except ping; size includes protocol    */
                                    /* header; larger packets will be dropped */
    int   max_frag_icmp;            /* Maximum fragment size (in bytes) will */
                                    /* process for ICMP (ping) packets     */
                                    /* except ping; size includes protocol */
    int   frag_table_size;          /* maximum entries in fragmentation */
                                    /* table   */

    int     rtsize;                 /* number of entries in routing table */
#endif
} CFG_IP_DATA;

#if (INCLUDE_RUN_TIME_CONFIG)
#define CFG_IP_TTL          cfg_ip_data.ip_ttl_val
#define CFG_IP_FRAG_TTL     cfg_ip_data.ip_frag_ttl
#define CFG_MAX_FRAG        cfg_ip_data.max_frag
#define CFG_MAX_FRAG_ICMP   cfg_ip_data.max_frag_icmp
#define CFG_FRAG_TABLE_SIZE cfg_ip_data.frag_table_size
#define CFG_RTSIZE          cfg_ip_data.rtsize
#else
#define CFG_IP_TTL          cfg_ip_data.ip_ttl_val
#define CFG_IP_FRAG_TTL     _CFG_IP_FRAG_TTL
#define CFG_MAX_FRAG        _CFG_MAX_FRAG
#define CFG_MAX_FRAG_ICMP   _CFG_MAX_FRAG_ICMP
#define CFG_FRAG_TABLE_SIZE _CFG_FRAG_TABLE_SIZE
#define CFG_RTSIZE          _CFG_RTSIZE
#endif

/* *********************************************************************   */
#if (INCLUDE_RUN_TIME_CONFIG)
typedef struct cfg_iface_data
{
    int nifaces;
    int num_devices;
    int mclistsize;
} CFG_IFACE_DATA;
#endif

#if (INCLUDE_RUN_TIME_CONFIG)
#define CFG_NIFACES         cfg_iface_data.nifaces
#define CFG_NUM_DEVICES     cfg_iface_data.num_devices
#define CFG_MCLISTSIZE      cfg_iface_data.mclistsize
#else
#define CFG_NIFACES         _CFG_NIFACES
#define CFG_NUM_DEVICES     _CFG_NUM_DEVICES
#define CFG_MCLISTSIZE      _CFG_MCLISTSIZE
#endif

/* *********************************************************************   */
#if (INCLUDE_RUN_TIME_CONFIG)
typedef struct cfg_packet_data
{
    int max_packetsize;
    int num_packets0;
    int num_packet_size0;
    int num_packets1;
    int num_packet_size1;
    int num_packets2;
    int num_packet_size2;
    int num_packets3;
    int num_packet_size3;
    int num_packets4;
    int num_packet_size4;
    int num_packets5;
    int num_packet_size5;
    int pkt_lowwater;
    int pkt_lowwater_ooo;
    int pkt_quench;
    int display_if_ndcus;
} CFG_PACKET_DATA;
#endif

#if (INCLUDE_RUN_TIME_CONFIG)
#define CFG_MAX_PACKETSIZE      cfg_packet_data.max_packetsize
#define CFG_NUM_PACKETS0        cfg_packet_data.num_packets0
#define CFG_PACKET_SIZE0        cfg_packet_data.num_packet_size0
#define CFG_NUM_PACKETS1        cfg_packet_data.num_packets1
#define CFG_PACKET_SIZE1        cfg_packet_data.num_packet_size1
#define CFG_NUM_PACKETS2        cfg_packet_data.num_packets2
#define CFG_PACKET_SIZE2        cfg_packet_data.num_packet_size2
#define CFG_NUM_PACKETS3        cfg_packet_data.num_packets3
#define CFG_PACKET_SIZE3        cfg_packet_data.num_packet_size3
#define CFG_NUM_PACKETS4        cfg_packet_data.num_packets4
#define CFG_PACKET_SIZE4        cfg_packet_data.num_packet_size4
#define CFG_NUM_PACKETS5        cfg_packet_data.num_packets5
#define CFG_PACKET_SIZE5        cfg_packet_data.num_packet_size5
#define CFG_PKT_LOWWATER        cfg_packet_data.pkt_lowwater
#define CFG_PKT_LOWWATER_OOO    cfg_packet_data.pkt_lowwater_ooo
#define CFG_PKT_QUENCH          cfg_packet_data.pkt_quench
#define CFG_DISPLAY_IF_NDCUS            cfg_packet_data.pkt_quench
#else
#define CFG_MAX_PACKETSIZE      _CFG_MAX_PACKETSIZE
#define CFG_NUM_PACKETS0        _CFG_NUM_PACKETS0
#define CFG_PACKET_SIZE0        _CFG_PACKET_SIZE0
#define CFG_NUM_PACKETS1        _CFG_NUM_PACKETS1
#define CFG_PACKET_SIZE1        _CFG_PACKET_SIZE1
#define CFG_NUM_PACKETS2        _CFG_NUM_PACKETS2
#define CFG_PACKET_SIZE2        _CFG_PACKET_SIZE2
#define CFG_NUM_PACKETS3        _CFG_NUM_PACKETS3
#define CFG_PACKET_SIZE3        _CFG_PACKET_SIZE3
#define CFG_NUM_PACKETS4        _CFG_NUM_PACKETS4
#define CFG_PACKET_SIZE4        _CFG_PACKET_SIZE4
#define CFG_NUM_PACKETS5        _CFG_NUM_PACKETS5
#define CFG_PACKET_SIZE5        _CFG_PACKET_SIZE5
#define CFG_PKT_LOWWATER        _CFG_PKT_LOWWATER
#define CFG_PKT_LOWWATER_OOO    _CFG_PKT_LOWWATER_OOO
#define CFG_PKT_QUENCH          _CFG_PKT_QUENCH
#define CFG_DISPLAY_IF_NDCUS    _CFG_DISPLAY_IF_NDCUS
#endif

/* *********************************************************************   */
#if (INCLUDE_RUN_TIME_CONFIG)
typedef struct cfg_protocol_data
{
    int  total_tcp_ports;
    int  total_udp_ports;
    int  arpclen;
    int  arpc_req_timeout;
    int  arpc_max_retries;
    int  arpc_res_timeout;
    int  num_ip2eth_addr;
    RTP_UINT16 udp_port_min;
    RTP_UINT16 udp_port_max;
    RTP_UINT16 tcp_port_min;
    RTP_UINT16 tcp_port_max;
    int  timer_freq;
} CFG_PROTOCOL_DATA;
#endif

#if (INCLUDE_MALLOC_PORTS)
#define CFG_NTCPPORTS               cfg_protocol_data.total_tcp_ports
#define CFG_NUDPPORTS               cfg_protocol_data.total_udp_ports
#else
#define CFG_NTCPPORTS               _CFG_NTCPPORTS
#define CFG_NUDPPORTS               _CFG_NUDPPORTS
#endif                              /* _YI_ */

#if (INCLUDE_RUN_TIME_CONFIG)
#define CFG_ARPCLEN                 cfg_protocol_data.arpclen
#define CFG_ARPC_REQ_TIMEOUT        cfg_protocol_data.arpc_req_timeout
#define CFG_ARPC_MAX_RETRIES        cfg_protocol_data.arpc_max_retries
#define CFG_ARPC_RES_TIMEOUT        cfg_protocol_data.arpc_res_timeout
#define CFG_NUM_IP2ETH_ADDR         cfg_protocol_data.num_ip2eth_addr
#define CFG_UDP_PORT_MIN            cfg_protocol_data.udp_port_min
#define CFG_UDP_PORT_MAX            cfg_protocol_data.udp_port_max
#define CFG_TCP_PORT_MIN            cfg_protocol_data.tcp_port_min
#define CFG_TCP_PORT_MAX            cfg_protocol_data.tcp_port_max
#define CFG_TIMER_FREQ              cfg_protocol_data.timer_freq
#define CFG_MAX_DNS_SRV             _CFG_MAX_DNS_SRV
#define CFG_DNS_NAME_LEN            _CFG_DNS_NAME_LEN

#else

#define CFG_ARPCLEN                 _CFG_ARPCLEN
#define CFG_ARPC_REQ_TIMEOUT        _CFG_ARPC_REQ_TIMEOUT
#define CFG_ARPC_MAX_RETRIES        _CFG_ARPC_MAX_RETRIES
#define CFG_ARPC_RES_TIMEOUT        _CFG_ARPC_RES_TIMEOUT
#define CFG_NUM_IP2ETH_ADDR         _CFG_NUM_IP2ETH_ADDR
#define CFG_UDP_PORT_MIN            _CFG_UDP_PORT_MIN
#define CFG_UDP_PORT_MAX            _CFG_UDP_PORT_MAX
#define CFG_TCP_PORT_MIN            _CFG_TCP_PORT_MIN
#define CFG_TCP_PORT_MAX            _CFG_TCP_PORT_MAX
#define CFG_TIMER_FREQ              _CFG_TIMER_FREQ
#define CFG_MAX_DNS_SRV             _CFG_MAX_DNS_SRV
#define CFG_DNS_NAME_LEN            _CFG_DNS_NAME_LEN
#endif

#if (INCLUDE_TCP)
/* ********************************************************************   */
typedef struct cfg_tcp_data
{
    RTP_UINT32 ka_interval;
    int   ka_retry;
    int   ka_tmo;
#if (INCLUDE_RUN_TIME_CONFIG)
    int   timer_freq;
    int   tmo_proc;
    int   max_delay_ack;
    long  maxrto;
    long  minrto;
    long  retrans_tmo;
    long  report_tmo;
    int   win_probe_min;
    int   win_probe_max;
    int   twaittime;
    int   lasttime;
    int   tcp_max_conn;
    RTP_UINT16  no_mss_val;
    RTP_UINT16  mss_gateway;
    int   num_ooo_que;
    int   tcp_send_wait_ack;
#endif
} CFG_TCP_DATA;

#if (INCLUDE_RUN_TIME_CONFIG)
#define CFG_KA_INTERVAL             cfg_tcp_data.ka_interval
#define CFG_KA_RETRY                cfg_tcp_data.ka_retry
#define CFG_KA_TMO                  cfg_tcp_data.ka_tmo
#define CFG_TMO_PROC                cfg_tcp_data.tmo_proc
#define CFG_MAX_DELAY_ACK           cfg_tcp_data.max_delay_ack
#define CFG_MAXRTO                  cfg_tcp_data.maxrto
#define CFG_MINRTO                  cfg_tcp_data.minrto
#define CFG_RETRANS_TMO             cfg_tcp_data.retrans_tmo
#define CFG_REPORT_TMO              cfg_tcp_data.report_tmo
#define CFG_WIN_PROBE_MIN           cfg_tcp_data.win_probe_min
#define CFG_WIN_PROBE_MAX           cfg_tcp_data.win_probe_max
#define CFG_TWAITTIME               cfg_tcp_data.twaittime
#define CFG_LASTTIME                cfg_tcp_data.lasttime
#define CFG_TCP_MAX_CONN            cfg_tcp_data.tcp_max_conn
#define CFG_NO_MSS_VAL              cfg_tcp_data.no_mss_val
#define CFG_MSS_GATEWAY             cfg_tcp_data.mss_gateway
#define CFG_NUM_OOO_QUE             cfg_tcp_data.num_ooo_que
#define CFG_TCP_SEND_WAIT_ACK       cfg_tcp_data.tcp_send_wait_ack
#else
#define CFG_KA_INTERVAL             cfg_tcp_data.ka_interval
#define CFG_KA_RETRY                cfg_tcp_data.ka_retry
#define CFG_KA_TMO                  cfg_tcp_data.ka_tmo
#define CFG_TMO_PROC                _CFG_TMO_PROC
#define CFG_MAX_DELAY_ACK           _CFG_MAX_DELAY_ACK
#define CFG_MAXRTO                  _CFG_MAXRTO
#define CFG_MINRTO                  _CFG_MINRTO
#define CFG_RETRANS_TMO             _CFG_RETRANS_TMO
#define CFG_REPORT_TMO              _CFG_REPORT_TMO
#define CFG_WIN_PROBE_MIN           _CFG_WIN_PROBE_MIN
#define CFG_WIN_PROBE_MAX           _CFG_WIN_PROBE_MAX
#define CFG_TWAITTIME               _CFG_TWAITTIME
#define CFG_LASTTIME                _CFG_LASTTIME
#define CFG_TCP_MAX_CONN            _CFG_TCP_MAX_CONN
#define CFG_NO_MSS_VAL              _CFG_NO_MSS_VAL
#define CFG_MSS_GATEWAY             _CFG_MSS_GATEWAY
#define CFG_NUM_OOO_QUE             _CFG_NUM_OOO_QUE
#define CFG_TCP_SEND_WAIT_ACK       _CFG_TCP_SEND_WAIT_ACK      
#endif
#endif      /* INCLUDE_TCP */

#if (INCLUDE_BOOTP || INCLUDE_RARP)
#if (INCLUDE_RUN_TIME_CONFIG)
typedef struct cfg_rarp_bootp_data
{
    int rarp_tries;     /* How many times to retry rarp before giving up */
    int rarp_tmo;       /* Number of second to wait before retrying a RARP */
                        /* request   */
    int bootp_retries;  /* How many times to retry BOOTP before giving up */
    int bootp_rtsize;   /* Number of routers from BOOTP server which */
                        /* will be processed   */
} CFG_RARP_BOOTP_DATA;
#endif

#if (INCLUDE_RUN_TIME_CONFIG)
#define CFG_RARP_TRIES          cfg_rarp_bootp_data.rarp_tries
#define CFG_RARP_TMO            cfg_rarp_bootp_data.rarp_tmo
#define CFG_BOOTP_RETRIES       cfg_rarp_bootp_data.bootp_retries
#define CFG_BOOTP_RTSIZE        cfg_rarp_bootp_data.bootp_rtsize
#else
#define CFG_RARP_TRIES          _CFG_RARP_TRIES
#define CFG_RARP_TMO            _CFG_RARP_TMO
#define CFG_BOOTP_RETRIES       _CFG_BOOTP_RETRIES
#define CFG_BOOTP_RTSIZE        _CFG_BOOTP_RTSIZE
#endif
#endif /* INCLUDE_BOOTP || INCLUDE_RARP */



#if (INCLUDE_DNS)
/* ********************************************************************   */
#if (INCLUDE_RUN_TIME_CONFIG) /* _YI_ */
typedef struct cfg_dns_data
{
    int min_dns_delay;          /* The minimum (initial) delay between & */
                                /* name server retries.                        */
                                /* Units: seconds                              */
    int max_dns_delay;          /* The maximum delay between retries */
                                /* Units: seconds     */
    int dns_retries;            /* Max # of retries - delay is doubled
                                   for each retry */
} CFG_DNS_DATA;

RTP_EXTERN struct cfg_dns_data RTP_FAR cfg_dns_data;

#define CFG_MIN_DNS_DELAY   cfg_dns_data.min_dns_delay
#define CFG_MAX_DNS_DELAY   cfg_dns_data.max_dns_delay
#define CFG_DNS_RETRIES     cfg_dns_data.dns_retries
#define CFG_MAX_HT_ENT      _CFG_MAX_HT_ENT
#define CFG_MAX_HC_ENT      _CFG_MAX_HC_ENT
#define CFG_MAX_DNS_SRV     _CFG_MAX_DNS_SRV
#define CFG_MAX_HX_ADDRS    _CFG_MAX_HX_ADDRS
#define CFG_MAX_HX_ALIAS    _CFG_MAX_HX_ALIAS

#else
#define CFG_MIN_DNS_DELAY   _CFG_MIN_DNS_DELAY
#define CFG_MAX_DNS_DELAY   _CFG_MAX_DNS_DELAY
#define CFG_DNS_RETRIES     _CFG_DNS_RETRIES
#define CFG_MAX_HT_ENT      _CFG_MAX_HT_ENT
#define CFG_MAX_HC_ENT      _CFG_MAX_HC_ENT
#define CFG_MAX_DNS_SRV     _CFG_MAX_DNS_SRV
#define CFG_MAX_HX_ADDRS    _CFG_MAX_HX_ADDRS
#define CFG_MAX_HX_ALIAS    _CFG_MAX_HX_ALIAS
#endif

#endif      /* INCLUDE_DNS */

#if (INCLUDE_RUN_TIME_CONFIG)
/* ********************************************************************   */
/* CONFIGURATION EXTERNS                                                  */
/* ********************************************************************   */
RTP_EXTERN CFG_PROTOCOL_DATA RTP_FAR cfg_protocol_data;
RTP_EXTERN CFG_IFACE_DATA    RTP_FAR cfg_iface_data;
RTP_EXTERN CFG_PACKET_DATA   RTP_FAR cfg_packet_data;
#if (INCLUDE_BOOTP || INCLUDE_RARP)
RTP_EXTERN CFG_RARP_BOOTP_DATA RTP_FAR cfg_rarp_bootp_data;
#endif
#endif      /* INCLUDE_RUN_TIME_CONFIG */

/* the following have certain entries which are run time configurable   */
/* even if INCLUDE_RUN_TIME_CONFIG is 0                                 */
RTP_EXTERN CFG_IP_DATA RTP_FAR cfg_ip_data;
#if (INCLUDE_TCP)
RTP_EXTERN CFG_TCP_DATA      RTP_FAR cfg_tcp_data;
#endif

#if (INCLUDE_PPP_VANJC || INCLUDE_CSLIP)
/* ********************************************************************   */
#if (INCLUDE_RUN_TIME_CONFIG)
typedef struct cfg_slhc_data
{
    int cslip_slots;                /* number of slots - CSLIP only */
} CFG_SLHC_DATA;

#define CFG_MAX_VJC_SLOTS   _CFG_MAX_VJC_SLOTS
#define CFG_CSLIP_SLOTS     cfg_slhc_data.cslip_slots

RTP_EXTERN struct cfg_slhc_data RTP_FAR cfg_slhc_data;

#else
#define CFG_MAX_VJC_SLOTS   _CFG_MAX_VJC_SLOTS
#define CFG_CSLIP_SLOTS     _CFG_CSLIP_SLOTS
#endif
#endif /* INCLUDE_PPP_VANJC || INCLUDE_CSLIP */

/* ********************************************************************   */
#if (INCLUDE_RUN_TIME_CONFIG)


#define PRIOTASK_NORMAL     threadPriorityMap[CFG_PRIO_INDEX_NORMAL];
#define PRIOTASK_HI         threadPriorityMap[CFG_PRIO_INDEX_HIGH];
#define PRIOTASK_HIGHEST    threadPriorityMap[CFG_PRIO_INDEX_NORMAL];

#define SIZESTACK_TINY	    stackSizeMap[CFG_STACK_SIZE_TINY];
#define SIZESTACK_SMALL	    stackSizeMap[CFG_STACK_SIZE_SMALL];
#define SIZESTACK_NORMAL    stackSizeMap[CFG_STACK_SIZE_NORMAL];
#define SIZESTACK_BIG       stackSizeMap[CFG_STACK_SIZE_BIG];
#define SIZESTACK_HUGE      stackSizeMap[CFG_STACK_SIZE_HUGE]

RTP_EXTERN struct cfg_tasks_data cfg_tasks_data;

#endif

/* *********************************************************************   */
/* ******    parameter, return values etc                         ******   */
/* *********************************************************************   */

#ifdef __cplusplus
RTP_EXTERN "C" {
#endif

RTP_EXTERN RTP_CUINT8 RTP_FAR ip_ffaddr[IP_ALEN] ;
RTP_EXTERN RTP_CUINT8 RTP_FAR ip_nulladdr[IP_ALEN];

#ifdef __cplusplus
}
#endif

#define RT_DEFAULT        ip_nulladdr       /* default gateway */
#define RT_INF            999               /* no timeout for this route */
/* metric   */
#define RTM_INF           16                /* infinite metric */
#define RT_USEIFACEMETRIC 0x80000000L       /* use interface's default metric */

// parameters to xn_rt_del_iface()
#define RT_DEL_ALL          0x01            // delete all routing table entries
                                            // (for the interface)
#define RT_DEL_API          0x02            // delete routing table entries
                                            // added by xn_rt_add,
                                            // xn_ipv6_rt_add or setsockopt
                                            // (for the interface)
#define RT_DEL_SETIP        0x03            // delete routing table entries
                                            // added by xn_set_ip and
                                            // xn_ipv6_set_ip
                                            // (for the interface)
#define RT_DEL_DHCP         0x04            // delete routine table entries
                                            // added by DHCP 
                                            // (for the interface)
#define RT_DEL_SNMP         0x05            // delete routine table entries
                                            // added by SNMP
                                            // (for the interface)
#define RT_DEL_PPP          0x06            // delete routine table entries
                                            // added by PPP 
                                            // (for the interface)
#define RT_DEL_REDIRECT     0x07            // delete routine table entries
                                            // added by REDIRECT
                                            // (for the interface)
#define RT_DEL_RIP          0x08            // delete routine table entries
                                            // added by RIP
                                            // (for the interface)
#define RT_DEL_PROTOCOL     0x09            // delete routine table entries
                                            // added by routing table
                                            // lookup; default gateway
                                            // entries used to add static
                                            // routes; 
                                            // INCLUDE_BUILD_STATIC_ROUTES
                                            // must be 1
#define RT_DEL_BOOTP        0x0a            // delete routine table entries
                                            // added by BOOTP
                                            // (for the interface)
#define RT_DEL_ROUTER_DISC  0x0b            // delete routine table entries
                                            // added by ROUTER DISCOVERY
                                            // (for the interface)
#define RT_DEL_RARP         0x0f            // delete routine table entries
                                            // added by RARP
                                            // (for the interface)

/* parameter to xn_interface_info   */
typedef struct _iface_info
{
    int  device_id;                 /* device type (NE2000_DEVICE etc) */
    RTP_UINT8 my_ip_address[IP_LEN];        /* local ip address */
    RTP_UINT8 my_ethernet_address[ETH_ALEN];    /* local ethernet address */
    RTP_UINT8 fill[2];                  /* alignment */
    RTP_UINT8 ip_mask[IP_ALEN];         /* network mask */
    RTP_UINT8 his_ip_address[IP_ALEN];   /* remote ip address -
                                       valid only for SLIP, CSLIP or PPP */
#if (INCLUDE_BOOTP)
    RTP_UINT8 bootp_ip_address[IP_ALEN]; /* valid if BOOTP was done */
    char bootp_file_name[BOOTP_FILE_LEN];
#endif

    int  mtu;
    int  remote_mtu;                /* remote MTU negotiated */
                                    /* valid only for PPP   */
    int  irq;                       /* interrupt number */
    IOADDRESS ioaddr;               /* I/O address */

    RTP_UINT8   gateways[CFG_N_DEFAULT_GATEWAYS][IP_ALEN];
    int         num_gateways;
    RTP_UINT8   dns_servers[CFG_N_DEFAULT_DNS_SERVERS][IP_ALEN];
    int         num_dns_servers;
    
} IFACE_INFO;
typedef struct _iface_info RTP_FAR * PIFACE_INFO;

/* parameter option_value to xn_ip_set_option for options STRICT ROUTE   */
/* and LOOSE ROUTE                                                       */
typedef struct _route_info
{
    int    route_len;
    RTP_PFUINT8 route_addresses;
} ROUTE_INFO;
typedef struct _route_info RTP_FAR * PROUTE_INFO;

/* parameter to xn_interface_mcast_entry() and xn_snmp_config_trap()   */
#define IPV6_ADDR_FLAG 0x80
#define SET_ENTRY           1
#define ADD_ENTRY           2
#define DELETE_ENTRY        3
#define CLEAR_ENTRY         4
#define SET_IPV6_ENTRY      (IPV6_ADDR_FLAG | SET_ENTRY)
#define ADD_IPV6_ENTRY      (IPV6_ADDR_FLAG | ADD_ENTRY)
#define DELETE_IPV6_ENTRY   (IPV6_ADDR_FLAG | DELETE_ENTRY)
#define CLEAR_IPV6_ENTRY    (IPV6_ADDR_FLAG | CLEAR_ENTRY)


    /* IP OPTIONS   */
#if (INCLUDE_IPV4 && INCLUDE_IP_OPTIONS)
    /* options set by xn_ip_set_option()   */
    #define SO_LOOSE_ROUTE_OPTION  0x01 /* loose source route */
    #define SO_TIMESTAMP_OPTION    0x02 /* timestamp */
    #define SO_RECORD_ROUTE_OPTION 0x04 /* record route */
    #define SO_STRICT_ROUTE_OPTION 0x08 /* strict source route */
#if (INCLUDE_IGMP_V2 || INCLUDE_IGMP_V3)
    /* not set by xn_ip_set_option since per message not per port   */
    /* required for all IGMP V2 messages                            */
    #define RALERT_OPTION 0x10    /*required for IGMP V2 messages */
#endif
#endif

#if (INCLUDE_BOOTP)

typedef struct static_route_t
{
    RTP_UINT8 dest_ip[IP_ALEN];       /* destination addr */
    RTP_UINT8 gw_ip[IP_ALEN];         /* gateway's addr */
} STATIC_ROUTE;


/* We parse the BOOTP packet returned by the server and put it into         */
/* One of these structures. This seperates the parsing and processing       */
/* This is especially important in version 1 because we are adding gateways */
/* and name servers incrementally.                                          */
/* Since it is a big structure we use a packet structure to allocate        */
/* the storage.                                                             */

struct boot_p_results
{
    RTP_BOOL have_mask;             /* True is subnet mask returned */
    RTP_UINT8    my_ip_address[IP_ALEN];     /* local IP address */
    RTP_UINT8    bootp_ip_address[IP_ALEN];  /* local IP address */
    char    bootp_file_name[BOOTP_FILE_LEN];
    RTP_UINT8    my_net_mask[IP_ALEN];       /* Subnet mask */
/*
 * BOOTP needs to support assignment of host name and default
 * domain name.
*/
    RTP_UINT8    my_host_name[CFG_DNS_NAME_LEN];    /* My host name   */
    RTP_UINT8    my_domain_name[CFG_DNS_NAME_LEN];   /* My domain name */
    RTP_UINT8    default_gateway[IP_ALEN];  /* Default gateway */
    int     n_gateways;                /* Number of gateways in vendor */
                                       /* specific   */
    int     n_domservers;
    int     n_static_routes;
#if (INCLUDE_RUN_TIME_CONFIG)
    RTP_PFUINT8  gateways;
    RTP_PFUINT8  domservers;                /* Allow up to CFG_MAX_DNS_SRV */
    STATIC_ROUTE * static_routes;       /* static routes */
#else
    RTP_UINT8    gateways[CFG_BOOTP_RTSIZE*IP_ALEN];
    RTP_UINT8    domservers[CFG_MAX_DNS_SRV*IP_ALEN];   /* Allow up to CFG_MAX_DNS_SRV */
    STATIC_ROUTE static_routes[CFG_BOOTP_RTSIZE];   /* static routes */
#endif
};
typedef struct boot_p_results RTP_FAR * PBOOTR;
#endif      /* INCLUDE_BOOTP */

/* *********************************************************************   */
/* ******        TIMERS                                            *****   */
/* *********************************************************************   */
/* There is one of these structures for each simulated timer.
 * Whenever the timer is running, it is on a linked list
 * pointed to by "Timers".
 *
 * Stopping a timer or letting it expire causes it to be removed
 * from the list. Starting a timer puts it on the list.
 */

typedef struct ebs_timer RTP_FAR *PTIMER;
typedef struct ebs_timer
{
    PTIMER       next;                      /* Linked-list pointer */
    int          duration;                  /* Duration of timer */
    RTP_BOOL is_second;                 /* if one second timer */
    int          expiration;                /* expiration timer - counts down */
    void         (*func) (void RTP_FAR *);  /* Function to call at timer expiration */
    void RTP_FAR *arg;                      /* Arg to pass timeout function */

    #define TIMER_STOP      0
    #define TIMER_RUN       1
    #define TIMER_EXPIRE    2
    RTP_UINT8        state;                 /* Timer state */
} EBS_TIMER;

#if (INCLUDE_POSTMESSAGE)
/* ********************************************************************   */
/* PostMessage                                                            */
/* ********************************************************************   */
#define FD_READ         1
#define FD_WRITE        2
#define FD_ACCEPT       3
#define FD_CONNECT      4
#define FD_CLOSE        5
#define PPP_STATE_TRANS 6
#define PPP_PHASE_TRANS 7
#define PPP_PAP_FAIL    8
#define PPP_CHAP_FAIL   9

#define HWND  int
#define MSGID int
#undef PostMessage
RTP_BOOL PostMessage(HWND, int, int, int);
#endif

/* *********************************************************************   */
/* MODEM                                                                   */
/* *********************************************************************   */

/* constants for autologin commands   */
#define AL_WAIT     1
#define AL_WAITLIST 2
#define AL_BRKERR   3
#define AL_BRKFND   4
#define AL_SEND     5
#define AL_SLEEP    6
#define AL_LABEL    7
#define AL_PRINT    8
#define AL_END      9
#define AL_ENDERR  10
#define AL_NOOP    11

typedef struct al_command
{
    int     command;
    RTP_PFCCHAR str_arg;
    RTP_UINT16      num_arg;
} al_command;
typedef struct al_command RTP_FAR *PAL_COMMAND;

/* *********************************************************************   */
/* PUT HERE BEFORE API since need SOCKET								   */
/* *********************************************************************   */
#include "sock.h"

/* *********************************************************************   */
/* *********************************************************************   */
/* ******           Function prototypes                     ************   */
/* *********************************************************************   */

#ifdef __cplusplus
RTP_EXTERN "C" {
#endif

/* *********************************************************************   */
/* CALLBACKS FUNCTION TYPES                                                */
#if (INCLUDE_PPP && INCLUDE_CHAP)
typedef void (*CB_CHAP_GET_RANDOM_VALUE_FNC)(RTP_PFUINT8 rand_len, RTP_PFUINT8 rand_value);
#endif

#if POLLOS
typedef void (*CB_CHECK_EXIT_FNC)(void);
#ifdef INCLUDE_POLLOS_COOPERATIVE
typedef void (*CB_POLLOS_YIELD_TO_RTOS_FNC)(void);
typedef void (*CB_POLLOS_SIGNAL_TO_RTOS_FNC)(void);
#endif
#endif	// POLLOS

#if (INCLUDE_SLIP || INCLUDE_CSLIP || INCLUDE_PPP)
typedef void (*CB_RAW_MODE_IN_CHAR_FNC)(RTP_UINT8 c);
#endif
#if (INCLUDE_PPP || INCLUDE_SLIP)
typedef void (*CB_RS232_CONNECTION_LOST_FNC)(int iface_no);
#endif
typedef void (*CB_ERROR_REPORT_FNC)(SOCKET socket_no, int reason, int subreason);
#if (INCLUDE_TELNET_SRV)
typedef void (*CB_TELNET_INIT_FNC)(PTELNET_CONTEXT pcontext);
typedef int (*CB_TELNET_INCHAR_FNC)(PTELNET_CONTEXT pcontext, unsigned char c);
#endif
#if (INCLUDE_FTP_SRV)
typedef int (*CB_FTP_CHECK_USER_NAME_FNC)(int sock, char *user);
typedef int (*CB_FTP_CHECK_PASSWORD_FNC)(int sock, char *password);
#endif
#if (INCLUDE_DHCP_CLI)
typedef void (*CB_DHCP_NEW_IP_FNC)(int iface_no);
typedef void (*CB_DHCP_NO_IP_FNC)(int iface_no);
#endif
#if (INCLUDE_AUTOIP)
typedef void (*CB_AUTOIP_NEW_IP_FNC)(int iface_no);
typedef void (*CB_AUTOIP_NO_IP_FNC)(int iface_no);
#endif

/* *********************************************************************   */
typedef struct rtip_callbacks
{
#if (INCLUDE_PPP && INCLUDE_CHAP)
    CB_CHAP_GET_RANDOM_VALUE_FNC cb_chap_get_random_value_fnc;
#endif

    /* output routines for error logging   */
    CB_WR_ERROR_STRING_FNC cb_wr_screen_string_fnc;	/* _YI_ */
    CB_WR_INTERRUPT_STRING_FNC cb_wr_interrupt_string_fnc;

#if POLLOS
    CB_CHECK_EXIT_FNC cb_check_exit_fnc;

#ifdef INCLUDE_POLLOS_COOPERATIVE
    CB_POLLOS_YIELD_TO_RTOS_FNC cb_pollos_yield_to_rtos_fnc;
    CB_POLLOS_SIGNAL_TO_RTOS_FNC cb_pollos_signal_to_rtos_fnc;
#endif
#endif  /* POLLOS */

#if (INCLUDE_SLIP || INCLUDE_CSLIP || INCLUDE_PPP)
    CB_RAW_MODE_IN_CHAR_FNC cb_raw_mode_in_char_fnc;
    CB_RS232_CONNECTION_LOST_FNC cb_rs232_connection_lost_fnc;
#endif

    CB_ERROR_REPORT_FNC cb_error_report_fnc;

#if (INCLUDE_TELNET_SRV)
    CB_TELNET_INIT_FNC cb_telnet_init_fnc;
    CB_TELNET_INCHAR_FNC cb_telnet_inchar_fnc;
#endif

#if (INCLUDE_FTP_SRV)
    CB_FTP_CHECK_USER_NAME_FNC cb_ftp_check_user_name_fnc;
    CB_FTP_CHECK_PASSWORD_FNC  cb_ftp_check_password_fnc;
#endif
#if (INCLUDE_DHCP_CLI)
    CB_DHCP_NEW_IP_FNC cb_dhcp_new_ip_fnc;
    CB_DHCP_NO_IP_FNC  cb_dhcp_no_ip_fnc;
#endif
#if (INCLUDE_AUTOIP)
    CB_AUTOIP_NEW_IP_FNC cb_autoip_new_ip_fnc;
    CB_AUTOIP_NO_IP_FNC  cb_autoip_no_ip_fnc;
#endif
} RTIP_CALLBACKS;

typedef RTIP_CALLBACKS RTP_FAR * PRTIP_CALLBACKS;

/* ********************************************************************   */
/* EXTERN CALLBACKS                                                       */
/* ********************************************************************   */
RTP_EXTERN PRTIP_CALLBACKS rtip_callbacks;

/* *********************************************************************   */
/* ******    REGISTER ADD-ONS                                     ******   */
/* *********************************************************************   */

/* macros to register add-ons   */
#define XN_REGISTER_PKT_DRIVER() xn_register_init_fnc(&pkt_fnc, init_pkt_driver);
#define XN_REGISTER_SNMP()       xn_register_init_fnc(&snmp_fnc, init_snmp);
#define XN_REGISTER_DHCP_CLI()   xn_register_init_fnc(&dhcp_fnc, init_dhcp);
#define XN_REGISTER_FTP_SRV()    xn_register_init_fnc(&ftp_fnc, init_ftp_srv);
#define XN_REGISTER_WEB_SRV()    xn_register_init_fnc(&web_fnc, init_web_srv);
#define XN_REGISTER_TELNET_SRV() xn_register_init_fnc(&telnet_fnc, init_telnet_srv);
#define XN_REGISTER_LOOPBACK()   xn_register_init_fnc(&loop_fnc, init_loopback);

#ifdef __cplusplus
RTP_EXTERN "C" {
#endif

#if (INCLUDE_SNMP)
RTP_EXTERN INIT_FNCS RTP_FAR snmp_fnc;      /* used to register SNMP init fnc */
#endif      /* end of if SNMP */

#if (INCLUDE_DHCP_CLI)
RTP_EXTERN INIT_FNCS RTP_FAR dhcp_fnc;      /* used to register DHCP init fnc */
#endif

#if (INCLUDE_FTP_SRV)
RTP_EXTERN INIT_FNCS RTP_FAR ftp_fnc;       /* used to register FTP init fnc */
#endif

#if (INCLUDE_WEB)
RTP_EXTERN INIT_FNCS RTP_FAR web_fnc;       /* used to register WEB init fnc */
#endif

#if (INCLUDE_TELNET_SRV)
RTP_EXTERN INIT_FNCS RTP_FAR telnet_fnc;        /* used to register TELNET init fnc */
#endif

#if (INCLUDE_PKT)
void init_pkt_driver(void);
RTP_EXTERN INIT_FNCS RTP_FAR pkt_fnc;       /* used to register PKT init fnc */
#endif

#if (INCLUDE_LOOP)
void init_loopback(void);
RTP_EXTERN INIT_FNCS RTP_FAR loop_fnc;      /* used to register LOOPBACK init fnc */
#endif

#ifdef __cplusplus
}
#endif

/* *********************************************************************   */
/* API                                                                     */
/* *********************************************************************   */

/* File VFILE.C   */
int     vf_init(void);

/* *********************************************************************   */
/* File IPMOB.C:                                                           */
int     SOCKAPI_ xn_solicite_routers(int iface_no);

/* *********************************************************************   */
/* File OSPORT.C                                                           */
int     ks_kernel_init(void);
void 	ks_kernel_run(void);


/* *********************************************************************   */
/* File OSTASK.C                                                           */
PRTP_SYSTEM_USER      get_system_user(void);
void 		tc_timer_main(void);
void        tc_ip_process(int * iface_no); 
/* *********************************************************************   */
/* File RTIPAPI.C:                                                         */
void        SOCKAPI_ register_add_ons(void);
void        SOCKAPI_ ebs_set_timer(PTIMER t, int interval, RTP_BOOL is_sec);
void        SOCKAPI_ ebs_start_timer(PTIMER t);
void        SOCKAPI_ ebs_stop_timer(PTIMER timer);

#define xn_ebs_set_timer(T, INTERVAL, IS_SEC) ebs_set_timer(T, INTERVAL, IS_SEC)
#define xn_ebs_start_timer(T)                 ebs_start_timer(T)
#define xn_ebs_stop_timer(T)                  ebs_stop_timer(T)

int         SOCKAPI_ xn_rtip_init(void);
int         SOCKAPI_ xn_device_table_add(int device_id,
                        int minor_number,
                        int iface_type,
                        RTP_PFCHAR device_name,
#if (INCLUDE_SNMP)
                        struct oid media_mib, 
#endif
#if (INCLUDE_SNMP || INCLUDE_DMH_SNMP)
                        RTP_UINT32 speed,
#endif
                        DEV_OPEN dev_open,
                        DEV_CLOSE dev_close,
                        DEV_XMIT dev_xmit,
                        DEV_XMIT_DONE dev_xmit_done,
                        DEV_PROCESS_INTERRUPTS dev_proc_interrupts,
                        DEV_STATS dev_stats,
                        DEV_SETMCAST dev_smcast);

int         SOCKAPI_ xn_rtip_restart(void);
void        SOCKAPI_ xn_register_init_fnc(P_INIT_FNCS init_fnc, REGISTER_PROC fnc);
int         SOCKAPI_ xn_rtip_exit(void);
int         SOCKAPI_ xn_rarp(int iface_no);
#if (INCLUDE_BOOTP)
int         SOCKAPI_ xn_bootp(int iface_no);
int         SOCKAPI_ xn_bootp_res(int iface_no, PBOOTR presults);
#endif
#if (INCLUDE_IPV4 && INCLUDE_PING)
#if (RTIP_VERSION >= 26)
int         SOCKAPI_ _xn_ping(int sequence, int len, int ttl, RTP_PFUINT8 host,
                              int route_type, PROUTE_INFO route_info,
                              long wait_count,
                              long *elapsed_msec, RTP_BOOL send_802_2);
#else
int         SOCKAPI_ xn_ping(int sequence, int len, int ttl, RTP_PFUINT8 host,
                             int route_type, PROUTE_INFO route_info,
                             long wait_count, long *elapsed_msec);
#endif
#endif
int         SOCKAPI_ set_ip(int iface_no, RTP_PFUINT8 local_ip_address, RTP_PFCUINT8 ip_mask, int added_by);
int         SOCKAPI_ xn_set_ip(int iface_no, RTP_PFUINT8 my_ip_address, RTP_PFCUINT8 ip_mask);
int         SOCKAPI_ xn_rt_add(RTP_PFCUINT8 net, RTP_PFCUINT8 mask, RTP_PFCUINT8 gw, 
                               RTP_UINT32 metric, int iface, int ttl);
int         SOCKAPI_ xn_rt_del(RTP_PFUINT8 net, RTP_PFCUINT8 mask);

void        SOCKAPI_ rt_del_iface(int iface_no, int added_by);
#define xn_rt_del_iface(IFACE_NO, ADDED_BY) rt_del_iface(IFACE_NO, ADDED_BY)

int         SOCKAPI_ xn_rt_cycle_gw(void);
int         SOCKAPI_ xn_abort(SOCKET socket, RTP_BOOL tcp_send_reset);
#if (INCLUDE_SLIP || INCLUDE_CSLIP || INCLUDE_PPP)
int         SOCKAPI_ xn_attach(int device_id, int minor_number, RTP_PFUINT8 his_ip_address,
                               int port_com, int baud_rate, char handshake_type,
                               RTP_PFUINT8 input_buffer, int input_buffer_len,
                               RTP_PFUINT8 output_buffer, int output_buffer_len);
#endif
#if (INCLUDE_KEEP_STATS)
int         SOCKAPI_ xn_interface_stats(int interface_no);
#endif

/* modem   */
int         SOCKAPI_ xn_scriptlogin(int iface_no, PAL_COMMAND script,
                                    int num_commands, RTP_PFCHAR term_str);
int         SOCKAPI_ xn_autologin(int iface_no, RTP_PFCHAR setupstr, RTP_PFCHAR phonenum,
                                  RTP_PFCHAR username, RTP_PFCHAR passwd, RTP_PFCHAR term_str);
int         SOCKAPI_ xn_autoanswer(int iface_no, RTP_PFCHAR setupstr, RTP_UINT16 timeout,
                                   RTP_PFCHAR term_str);
int         SOCKAPI_ xn_hangup(int iface_no, RTP_PFCHAR term_str);

int         SOCKAPI_ xn_interface_open_config(int device_id, int minor_number,
                                              IOADDRESS io_address,
                                              int irq_val,
                                              RTP_UINT16 mem_address);
RTP_BOOL     SOCKAPI_ xn_wait_tcp_output(int sec_wait);
int         SOCKAPI_ xn_interface_opt(int iface_no, int option_name,
                                      RTP_PFCCHAR option_value, int optionlen);
RTP_BOOL     SOCKAPI_ xn_tcp_is_write(SOCKET socket);
RTP_BOOL     SOCKAPI_ xn_tcp_is_read(SOCKET socket);
RTP_BOOL     SOCKAPI_ xn_tcp_is_connect(SOCKET socket);
int         SOCKAPI_ xn_getlasterror(void);
RTP_PFCCHAR     SOCKAPI_ xn_geterror_string(int errno_val);
#if (INCLUDE_IPV4 && INCLUDE_IP_OPTIONS)
int         SOCKAPI_ xn_ip_set_option(SOCKET socket, int option_name,
                                      RTP_PFCCHAR option_value, int optionlen);
#endif
#if (INCLUDE_ARP)
#define xn_arp_send(IFACE)          tc_arp_send(IFACE)
int         SOCKAPI_ xn_arp_send(int interface_no);
#endif
#if (INCLUDE_ARP || INCLUDE_ETH_BUT_NOARP)
#define xn_arp_add(IPN, ETHN, TTL)  tc_arp_add(IPN, ETHN, TTL)
#define xn_arp_del(IPN)             tc_arp_del(IPN)
int         SOCKAPI_ xn_arp_add(RTP_PFUINT8 ipn, RTP_PFUINT8 ethn, RTP_UINT32 time_to_live);
int         SOCKAPI_ xn_arp_del(RTP_PFUINT8 ipn);
#endif

int         SOCKAPI_ xn_pkt_data_max(SOCKET socket, RTP_PFUINT8 dest_ip_addr);

#if (INCLUDE_POP3)
int         SOCKAPI_ xn_decode_base64(PBASE64_DECODE_CONTEXT pContext,RTP_PFUINT8 buffer, RTP_PFCHAR input, int input_length);
void        SOCKAPI_ xn_base64_decode_init(PBASE64_DECODE_CONTEXT pContext);
#endif

void        SOCKAPI_ xn_register_callbacks(PRTIP_CALLBACKS rcb);
PRTIP_CALLBACKS SOCKAPI_ xn_callbacks(void);

int         SOCKAPI_ xn_bind_sh7619_edmac(int minor_number);
int         SOCKAPI_ xn_bind_enc28j60(int minor_number);
int         SOCKAPI_ xn_bind_ne2000(int minor_number);
int         SOCKAPI_ xn_bind_ne2000_pcmcia(int minor_number);
int         SOCKAPI_ xn_bind_smc91c9x(int minor_number);
int         SOCKAPI_ xn_bind_smc91c9x_pcmcia(int minor_number);
int         SOCKAPI_ xn_bind_xircom(int minor_number);
int         SOCKAPI_ xn_bind_ed_ne2000(int minor_number);
int         SOCKAPI_ xn_bind_ed_ne2000_pcmcia(int minor_number);
int         SOCKAPI_ xn_bind_3c503(int minor_number);
int         SOCKAPI_ xn_bind_smc8xxx(int minor_number);
int         SOCKAPI_ xn_bind_el3(int minor_number);
int         SOCKAPI_ xn_bind_el89(int minor_number);
int         SOCKAPI_ xn_bind_csx(int minor_number);     /* cs89x0.c */
int         SOCKAPI_ xn_bind_cs(int minor_number);      /* lan8900.c */
int         SOCKAPI_ xn_bind_rtlance(int minor_number);
int         SOCKAPI_ xn_bind_lance_isa(int minor_number);
int         SOCKAPI_ xn_bind_i82559(int minor_number);
int         SOCKAPI_ xn_bind_r8139(int minor_number);
int         SOCKAPI_ xn_bind_tc90x(int minor_number);
int         SOCKAPI_ xn_bind_n83815(int minor_number);
int         SOCKAPI_ xn_bind_davicom(int minor_number);
int         SOCKAPI_ xn_bind_dm9000(int minor_number);
int         SOCKAPI_ xn_bind_eth360(int minor_number);
int         SOCKAPI_ xn_bind_eth860(int minor_number);
/* int         SOCKAPI_ xn_bind_ethfec(int minor_number);   */
int         SOCKAPI_ xn_bind_eth5272(int minor_number);
int         SOCKAPI_ xn_bind_eth5282(int minor_number);
int         SOCKAPI_ xn_bind_winether(int minor_number);
int         SOCKAPI_ xn_bind_winpcap(int minor_number);
int         SOCKAPI_ xn_bind_loop(int minor_number);
int         SOCKAPI_ xn_bind_slip(int minor_number);
int         SOCKAPI_ xn_bind_cslip(int minor_number);
int         SOCKAPI_ xn_bind_tcfe574_pcmcia(int minor_number);  
int         SOCKAPI_ xn_bind_prism(int minor_number);  
int         SOCKAPI_ xn_bind_prism_pcmcia(int minor_number);  

RTP_BOOL SOCKAPI_ wait_pkts_output(RTP_BOOL disable_output, long timeout);
int          SOCKAPI_ tc_interface_close(int iface_no);
int          SOCKAPI_ tc_interface_info(int iface_no, PIFACE_INFO pi_info);
RTP_BOOL SOCKAPI_ tc_interface_mcast(int iface_no, RTP_PFUINT8 mclist,
                               int nipaddrs, int add);

#define      xn_wait_pkts_output(disable_output, timeout) \
                wait_pkts_output((disable_output), (timeout))
#define      xn_interface_close(iface_no)                \
                tc_interface_close((iface_no))
#define      xn_interface_info(iface_no, pi_info)        \
                tc_interface_info((iface_no), (pi_info))
#define      xn_interface_mcast(iface_no, mclist, nipaddrs, add)    \
                tc_interface_mcast((iface_no), (mclist), (nipaddrs), (add))

/* statistics routines   */
int          SOCKAPI_ xn_interface_stats(int interface_no);
RTP_BOOL SOCKAPI_ xn_interface_statistics(int iface_no, PIFACE_STATS stats);

#if (INCLUDE_TRK_PKTS)
/* in OS.C   */
int          SOCKAPI_ xn_stats_gather_packet_info(PTRACK_DCU pdcu_track_info, RTP_BOOL display);
#endif

// in POLLOS.C
#if POLLOS
#define 	xn_pollos_cycle             poll_os_cycle
#endif

#if (RTIP_VERSION >= 26)
#define     xn_ping(SEQ, LEN, TTL, HOST, RT, ROUTE_INFO, WCOUNT, EMSEC) \
                _xn_ping(SEQ, LEN, TTL, HOST, RT, ROUTE_INFO, WCOUNT, EMSEC, RTP_FALSE)
#if (INCLUDE_802_2)
#define     xn_ping_802_2(SEQ, LEN, TTL, HOST, RT, ROUTE_INFO, WCOUNT, EMSEC) \
                _xn_ping(SEQ, LEN, TTL, HOST, RT, ROUTE_INFO, WCOUNT, EMSEC, RTP_TRUE)
#endif
#endif
#define     xn_interface_open(DEVICE, MINOR)  \
                xn_interface_open_config(DEVICE, MINOR, 0, -1, 0)
// TOOLS.C
int			set_errno(int error);

#ifdef __cplusplus
}
#endif

#endif  /* XNRTIPAPI */
