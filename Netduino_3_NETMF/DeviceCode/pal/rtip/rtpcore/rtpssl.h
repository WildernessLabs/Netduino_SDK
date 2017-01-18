 /*
 | RTPSSL.H - Runtime Platform Services
 |
 |   UNIVERSAL CODE - DO NOT CHANGE
 |
 | EBSnet - RT-Platform 
 |
 |  $Author: sarah $
 |  $Date: 2005/05/14 02:43:30 $
 |  $Name:  $
 |  $Revision: 1.1 $
 |
 | Copyright EBSnet Inc. , 2003
 | All rights reserved.
 | This code may not be redistributed in source or linkable object form
 | without the consent of its author.
 |
 | Module description:
 |  [tbd]
*/

#ifndef __RTPSSL_H__
#define __RTPSSL_H__

#include "rtp.h"
#include "rtpnet.h"

/* DO NOT CHANGE ANY CONSTANTS */

typedef RTP_HANDLE RTP_SSL_SESSION;
typedef RTP_HANDLE RTP_SSL_CONTEXT;

/* for the sslMode argument to rtp_ssl_init_context */

#define RTP_SSL_MODE_SERVER                   0x01
#define RTP_SSL_MODE_CLIENT                   0x02
#define RTP_SSL_MODE_SSL2                     0x04
#define RTP_SSL_MODE_SSL3                     0x08
#define RTP_SSL_MODE_TLS1                     0x10

/* for the verifyMode argument to rtp_ssl_init_context */

#define RTP_SSL_VERIFY_NONE                   0x01
#define RTP_SSL_VERIFY_PEER                   0x02
#define RTP_SSL_VERIFY_FAIL_IF_NO_PEER_CERT   0x04
/* RTP_SSL_VERIFY_CLIENT_ONCE is only for servers */
#define RTP_SSL_VERIFY_CLIENT_ONCE            0x08


#define RTP_SSL_ERR_NONE                       0
#define RTP_SSL_ERR_FAILED                    -1
#define RTP_SSL_ERR_WANT_READ                 -2
#define RTP_SSL_ERR_WANT_WRITE                -3

/************************************************************************
 * Secure Socket Layer API                                              *
 ************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

int  rtp_ssl_init_context  (RTP_SSL_CONTEXT* sslContext, unsigned int sslMode, unsigned int verifyMode);
int  rtp_ssl_init_client   (RTP_SSL_SESSION* sslSession, RTP_SOCKET sockHandle, RTP_SSL_CONTEXT sslContext);
int  rtp_ssl_init_server   (RTP_SSL_SESSION* sslSession, RTP_SOCKET sockHandle, RTP_SSL_CONTEXT sslContext);
int  rtp_ssl_connect       (RTP_SSL_SESSION sslStream);
int  rtp_ssl_accept        (RTP_SSL_SESSION sslStream);
long rtp_ssl_send          (RTP_SSL_SESSION sslStream, const unsigned char* buffer, long len, unsigned int boolBlocking);
long rtp_ssl_recv          (RTP_SSL_SESSION sslStream, unsigned char* buffer, long len, unsigned int boolBlocking);
int  rtp_ssl_close_session (RTP_SSL_SESSION sslStream);
void rtp_ssl_close_context (RTP_SSL_CONTEXT sslContext);

#ifdef __cplusplus
}
#endif

#endif /* __RTPSSL_H__ */

/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
