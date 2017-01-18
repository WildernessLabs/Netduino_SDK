#ifndef _TCP_H
#define _TCP_H

/* ********************************************************************      */

typedef void        (* TCP_INVALIDATE_SOCKETS_FNC)      (RTP_PFUINT8 );
typedef void        (* TC_TCP_ABORT_FNC)                (PTCPPORT, RTP_BOOL);
typedef void        (* TCP_ABORT_FNC)                   (PTCPPORT, RTP_BOOL);
typedef int         (* TCP_PKT_DATA_MAX_FNC)            (PTCPPORT);
typedef int         (* TC_TCP_READ_FNC)                 (PTCPPORT , RTP_PFUINT8 , int , DCU RTP_FAR *, int , long );
typedef int         (* TC_TCP_WRITE_FNC)                (PTCPPORT , RTP_PFUINT8 , int , DCU , int , long );
typedef RTP_BOOL    (* TCP_IS_WRITE_STATE_FNC)          (PTCPPORT );
typedef RTP_BOOL    (* TCP_IS_READ_STATE_FNC)           (PTCPPORT );
typedef PTCPPORT    (* TC_TCP_SOCK_ALLOC_FNC)           (RTP_BOOL);
typedef int         (* TC_TCP_LISTEN_FNC)               (PTCPPORT , int , RTP_BOOL );
typedef int         (* TC_TCP_CONNECT_FNC)              (PTCPPORT ,  RTP_PFUINT8 , RTP_UINT16 );
typedef int         (* TC_TCP_BIND_FNC)                 (PTCPPORT , RTP_PFUINT8 , RTP_UINT16 , RTP_BOOL );
typedef void        (* TC_TCP_PK_PEER_ADDRESS_FNC)      (PTCPPORT , RTP_PFUINT8 , RTP_PFUINT16 );
typedef PTCPPORT    (* TC_TCP_ACCEPT_FNC)               (PTCPPORT );
typedef int         (* TC_TCPSEND_FNC)                  (PIFACE , PTCPPORT , int ,int , RTP_UINT8 );
typedef PTCPPORT    (* TC_FIND_ACCEPT_PORT_FNC)         (PTCPPORT , RTP_BOOL );
typedef void        (* WAKE_UP_FNC)                     (PTCPPORT , RTP_BOOL , int , int );
typedef void        (* TRANS_STATE_FNC)                 (PTCPPORT , int , RTP_BOOL , RTP_BOOL );
typedef int         (* TC_TCP_CLOSE_FNC)                (PTCPPORT );
typedef void        (* TC_TCP_INTERPRET_FNC)            (PIFACE , DCU );
typedef void        (* TC_TCP_ABORT_CONNECTION_FNC)     (PTCPPORT , int , int , RTP_BOOL );
typedef void        (* RETURN_LISTEN_STATE_FNC)         (PTCPPORT );
typedef PTCPPORT    (* TC_FIND_PORT_FNC)                (DCU msg);
typedef RTP_BOOL    (* TC_TCP_TIMEOUT_FNC)              (RTP_BOOL , RTP_BOOL );

typedef struct tcp_registered_functions
{
    TCP_INVALIDATE_SOCKETS_FNC tcp_invalidate_sockets;
    TC_TCP_ABORT_FNC           tc_tcp_abort;
    TCP_ABORT_FNC              tcp_abort;
    TCP_PKT_DATA_MAX_FNC       tcp_pkt_data_max;
    TC_TCP_READ_FNC            tc_tcp_read;
    TC_TCP_WRITE_FNC           tc_tcp_write;
    TCP_IS_WRITE_STATE_FNC     tcp_is_write_state;
    TCP_IS_READ_STATE_FNC      tcp_is_read_state;
    TC_TCP_SOCK_ALLOC_FNC      tc_tcp_sock_alloc;
    TC_TCP_LISTEN_FNC          tc_tcp_listen;
    TC_TCP_CONNECT_FNC         tc_tcp_connect; 
    TC_TCP_BIND_FNC            tc_tcp_bind;
    TC_TCP_PK_PEER_ADDRESS_FNC tc_tcp_pk_peer_address;
    TC_TCP_ACCEPT_FNC          tc_tcp_accept;
    TC_TCPSEND_FNC             tc_tcpsend;
    TC_FIND_ACCEPT_PORT_FNC    tc_find_accept_port;
    WAKE_UP_FNC                wake_up;
    TRANS_STATE_FNC            trans_state;
    TC_TCP_CLOSE_FNC           tc_tcp_close;
    TC_TCP_INTERPRET_FNC       tc_tcp_interpret;
    TC_TCP_ABORT_CONNECTION_FNC tc_tcp_abort_connection;
    RETURN_LISTEN_STATE_FNC    return_listen_state;
    TC_FIND_PORT_FNC           tc_find_port;
    TC_TCP_TIMEOUT_FNC         tc_tcp_timeout;
} TCP_REGISTERED_FUNCTIONS;

/* ********************************************************************      */

#ifdef __cplusplus
extern "C" {
#endif

int     xn_tcp_initialize    ( void );

#ifdef __cplusplus
}
#endif
/* ********************************************************************      */
#endif //#ifndef _TCP_H
