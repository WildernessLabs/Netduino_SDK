/*                                                                      */
/* SOCK.H - SOCKET porting layer                                        */
/*                                                                      */
/* EBS - RTIP                                                           */
/*                                                                      */
/* Copyright EBSnet Inc, 1998                                           */
/* All rights reserved.                                                 */
/* This code may not be redistributed in source or linkable object form */
/* without the consent of its author.                                   */
/*                                                                      */

#ifndef __SOCK_H_INCLUDED__
#define __SOCK_H_INCLUDED__

#include "xnconf.h"
#include "rtp.h"

/* ********************************************************************   */
/* CONFIGURATION SECTION                                                  */
/* ********************************************************************   */
/* Specify underlying network stack                                       */
#define INCLUDE_WINSOCK         0
#define INCLUDE_BSDSOCK         0

#define INCLUDE_SUPPORT_SOCKETS 1

#if (!INCLUDE_RTIP)
/* Set to 1 to use dynamic memory routines in bget.c, i.e. bget(), brel() and   */
/* bpool().                                                                     */
/* If this is 0, malloc() and free() will be called for dynamic memory          */
/* requests; see osmalloc.c                                                     */
#define INCLUDE_BGET            0
#define INCLUDE_TCP             1
#endif

/* ********************************************************************   */
#if (INCLUDE_WINSOCK)
#include "string.h"
#include "winsock.h"
#elif (INCLUDE_BSDSOCK)
#include <string.h>
#include <socket.h>
#elif (INCLUDE_RTIP)
#else
#error no network stack!
#endif

#if (INCLUDE_WEB_BROWSER)
#include "webcfg.h"
#include "webc.h"
#endif

#if (INCLUDE_SSL)
#include "vsslapi.h" 
#include "vsslext.h" 
#endif

/* ********************************************************************   */
#define ESOCKNOTSUPPORT     -5

/* ********************************************************************   */
/* NETWORK DIFFERENCES                                                    */
/* ********************************************************************   */
#if (INCLUDE_RTIP)

#ifdef INVALID_SOCKET
#error: Cannot include winsock.h or winsock2.h before socket.h or sock.h
#endif

/* Winsock API compatibility defines   */
typedef int SOCKET;
#define INVALID_SOCKET -1
#define SOCKET_ERROR   -1
#define IS_INVALID_SOCKET(s) (s < 0)
#define GETLASTERROR() xn_getlasterror()
#define WSAGetLastError xn_getlasterror
#define WSAGetLastError xn_getlasterror

#ifdef WSAEWOULDBLOCK
    #undef WSABASEERR
    #undef WSAEWOULDBLOCK
    #undef WSAEINPROGRESS
    #undef WSAEALREADY
    #undef WSAENOTSOCK
    #undef WSAEDESTADDRREQ
    #undef WSAEMSGSIZE
    #undef WSAEPROTOTYPE
    #undef WSAENOPROTOOPT
    #undef WSAEPROTONOSUPPORT
    #undef WSAESOCKTNOSUPPORT
    #undef WSAEOPNOTSUPP
    #undef WSAEPFNOSUPPORT
    #undef WSAEAFNOSUPPORT
    #undef WSAEADDRINUSE
    #undef WSAEADDRNOTAVAIL
    #undef WSAENETDOWN
    #undef WSAENETUNREACH
    #undef WSAENETRESET
    #undef WSAECONNABORTED
    #undef WSAECONNRESET
    #undef WSAENOBUFS
    #undef WSAEISCONN
    #undef WSAENOTCONN
    #undef WSAESHUTDOWN
    #undef WSAETOOMANYREFS
    #undef WSAETIMEDOUT
    #undef WSAECONNREFUSED
    #undef WSAELOOP
    #undef WSAENAMETOOLONG
    #undef WSAEHOSTDOWN
    #undef WSAEHOSTUNREACH
    #undef WSAENOTEMPTY
    #undef WSAEPROCLIM
    #undef WSAEUSERS
    #undef WSAEDQUOT
    #undef WSAESTALE
    #undef WSAEREMOTE
#endif

#define WSABASEERR                 RTIP_ERRNO
#define WSAEWOULDBLOCK             EWOULDBLOCK
#define WSAEINPROGRESS             EINPROGRESS
#define WSAEALREADY                EALREADY
#define WSAENOTSOCK                ENOTSOCK
#define WSAEDESTADDRREQ            EDESTADDRREQ
#define WSAEMSGSIZE                EMSGSIZE
#define WSAEPROTOTYPE              EPROTOTYPE
#define WSAENOPROTOOPT             ENOPROTOOPT
#define WSAEPROTONOSUPPORT         EPROTONOSUPPORT
#define WSAESOCKTNOSUPPORT         ESOCKTNOSUPPORT
#define WSAEOPNOTSUPP              EOPNOTSUPP
#define WSAEPFNOSUPPORT            EPFNOSUPPORT
#define WSAEAFNOSUPPORT            EAFNOSUPPORT
#define WSAEADDRINUSE              EADDRINUSE
#define WSAEADDRNOTAVAIL           EADDRNOTAVAIL
#define WSAENETDOWN                ENETDOWN
#define WSAENETUNREACH             ENETUNREACH
#define WSAENETRESET               ENETRESET
#define WSAECONNABORTED            ECONNABORTED
#define WSAECONNRESET              ECONNRESET
#define WSAENOBUFS                 ENOBUFS
#define WSAEISCONN                 EISCONN
#define WSAENOTCONN                ENOTCONN
#define WSAESHUTDOWN               ESHUTDOWN
#define WSAETOOMANYREFS            ETOOMANYREFS
#define WSAETIMEDOUT               ETIMEDOUT
#define WSAECONNREFUSED            ECONNREFUSED
#define WSAELOOP                   ELOOP
#define WSAENAMETOOLONG            ENAMETOOLONG
#define WSAEHOSTDOWN               EHOSTDOWN
#define WSAEHOSTUNREACH            EHOSTUNREACH
#define WSAENOTEMPTY               ENOTEMPTY
#define WSAEPROCLIM                EPROCLIM
#define WSAEUSERS                  EUSERS
#define WSAEDQUOT                  EDQUOT
#define WSAESTALE                  ESTALE
#define WSAEREMOTE                 EREMOTE

#include "rtipapi.h"
#include "socket.h"

#elif (INCLUDE_WINSOCK)
 #include "winsock.h"
 #define IS_INVALID_SOCKET(s) (s == INVALID_SOCKET)
 typedef struct hostent* PFHOSTENT;
 #define GETLASTERROR()   (WSAGetLastError()?WSAGetLastError()-WSABASEERR:0)
 #define GETLASTFSERROR() (WSAGetLastError())
 #define INCLUDE_DNS 1

#elif (INCLUDE_BSDSOCK)
 #include <socket.h>
 #include <netinet/in.h>
 typedef int SOCKET;
 typedef struct hostent* PFHOSTENT;
 #define INVALID_SOCKET -1
 #define IS_INVALID_SOCKET(s) (s < 0)
 #define GETLASTERROR() (errno)
 typedef struct sockaddr CSOCKADDR;
 typedef CSOCKADDR *PCSOCKADDR;
 typedef struct sockaddr *PSOCKADDR;
 typedef struct linger LINGER;

#elif (defined(MT92101))
#include <psptypes.h>
typedef uint_32 _task_id;           /* what a task_id looks like  */
/*
** FILE STRUCTURE
**
** This structure defines the information kept in order to implement
** ANSI 'C' standard I/O stream.
*/
typedef struct file_struct
{

    /* The address of the Device for this stream   */
    struct io_device_struct _PTR_ DEV_PTR;

    /* Device Driver specific information   */
    pointer     DEV_DATA_PTR;

    /* General control flags for this stream   */
    uint_32     FLAGS;

    /* The current error for this stream   */
    uint_32     ERROR;

    /* The current position in the stream   */
    uint_32     LOCATION;

    /* The current size of the file   */
    uint_32     SIZE;

    /* The following 2 implement undelete   */
    boolean     HAVE_UNGOT_CHARACTER;
    int_32      UNGOT_CHARACTER;

} MYFILE, _PTR_ FILE_PTR;

#include <rtcs.h>
 typedef int SOCKET;
 typedef struct hostent* PFHOSTENT;
 #define INVALID_SOCKET -1
 #define IS_INVALID_SOCKET(s) (s < 0)
 #define GETLASTERROR() (-1)
 typedef struct sockaddr CSOCKADDR;
 typedef CSOCKADDR *PCSOCKADDR;
 typedef struct sockaddr *PSOCKADDR;
 typedef struct linger LINGER;


#else /* No network stack */
 /*typedef int RTP_BOOL;   */
 typedef int SOCKET;
 #define INVALID_SOCKET -1
 #define IS_INVALID_SOCKET(s) (s < 0)
 /*#error: GETLASTERROR needs to be defined   */
 #define INCLUDE_DNS 1
#endif

#if(INCLUDE_WINSOCK)
 typedef LPFD_SET PFDSET;
 #define EWOULDBLOCK (WSAEWOULDBLOCK-WSABASEERR)
 #define EINPROGRESS (WSAEINPROGRESS-WSABASEERR)
#elif (!INCLUDE_RTIP)
 typedef struct fd_set RTP_FAR * PFDSET;
#endif


/* ********************************************************************   */
/* include files                                                          */
/* ********************************************************************   */
#if (!INCLUDE_RTIP)
#include "rtipstub.h"
#endif

/* ********************************************************************   */
/* SOCKET function declarations                                           */
/* ********************************************************************   */
#ifdef __cplusplus
extern "C" {
#endif

int          net_network_init       (void);
int          net_network_close      (void);
SOCKET       net_socket             (int family, int type, int protocol);
int          net_tcp_socket         (SOCKET *psd);
int          net_closesocket        (SOCKET sd);
int          net_bind               (SOCKET sd, RTP_PFUINT8 ip_addr, RTP_UINT16 port);
int          net_connect            (SOCKET sd, RTP_PFUINT8 ip_addr, RTP_UINT16 port, int timeout);
int          net_listen             (SOCKET sd, int backlog);
int          net_accept             (SOCKET sock_master, SOCKET *sock_subordinate);
int          net_recv               (SOCKET sd, RTP_PFCHAR buffer, int size);
int          net_send               (SOCKET sd, RTP_PFCCHAR buffer, int size);
int 		 net_setblocking 		(SOCKET sd, unsigned int onBool);
RTP_BOOL     net_is_blocking        (SOCKET sd);
RTP_BOOL     net_set_hard_close     (SOCKET sd);
RTP_BOOL     net_set_linger         (SOCKET sd, int timeout);
RTP_BOOL     do_read_select         (int socket, long wait);
RTP_BOOL     do_write_select        (int socket_no, long wait);

#define      net_read_select        do_read_select
#define      net_write_select       do_write_select

#if(INCLUDE_SSL)
RTP_BOOL     net_is_secure_socket   (SOCKET sd);
RTP_BOOL     net_set_socket_secure  (SOCKET sd, void *sctx);
RTP_BOOL     net_set_socket_not_secure (SOCKET sd);
#endif /* INCLUDE_SSL */

#ifdef __cplusplus
};
#endif


#endif /* __SOCK_H_INCLUDED__ */

