#ifndef _UDP_H
#define _UDP_H

/* ********************************************************************      */

typedef RTP_BOOL (* TC_RAW_INTERPRET_FNC)       (PIFACE, DCU);
typedef void     (* TC_UDP_INTERPRET_FNC)       (PIFACE, DCU);
typedef int      (* TC_UDP_PK_SEND_FNC)         (PIFACE, RTP_PFUINT8, PUDPPORT , DCU, RTP_UINT16, RTP_UINT16, long, int);
typedef int      (* TC_UDP_GET_IFACE_FNC)       (PUDPPORT, RTP_PFUINT8, PIFACE RTP_FAR *, RTP_PFUINT8);
typedef void     (* TC_UDP_PK_PEER_ADDRESS_FNC) (DCU , RTP_PFUINT8 , RTP_PFUINT16 );
typedef int      (* TC_UDP_SET_HEADER_FNC)      (PUDPPORT , DCU , RTP_PFUINT8 , RTP_UINT16 , PIFACE , RTP_BOOL , int , int );
typedef void     (* UDP_CLOSESOCKET_FNC)        (PUDPPORT pu_port);           
typedef int      (* TC_UDP_BIND_FNC)            (PUDPPORT , RTP_PFCUINT8 , RTP_UINT16 );
typedef void     (* TC_UDP_CLOSE_FNC)           (PUDPPORT );
typedef int      (* TC_UDP_CONNECT_FNC)         (PUDPPORT , RTP_PFUINT8 , RTP_UINT16 );
typedef int      (* TC_UDP_READ_FNC)            (SOCKET , PANYPORT , RTP_PFCHAR , int , PSOCKADDR_IN , int , long );
typedef PUDPPORT (* TC_UDP_SOCK_ALLOC_FNC)      (int , int );              
typedef int      (* TC_UDP_WRITE_FNC)           (SOCKET , PANYPORT , RTP_PFUINT8 , int , PCSOCKADDR , int , long );
typedef void     (* UDP_INVALIDATE_SOCKETS_FNC) (RTP_PFUINT8 );
typedef void     (* UDP_FIND_PORT_FNC)          (PIFACE , PUDPPORT *, PUDPPORT *, DCU );

typedef struct udp_registered_functions
{
#if (INCLUDE_RAW)
	TC_RAW_INTERPRET_FNC       tc_raw_interpret;
#endif
	TC_UDP_INTERPRET_FNC       tc_udp_interpret;      
    TC_UDP_PK_SEND_FNC         tc_udp_pk_send;        
    TC_UDP_GET_IFACE_FNC       tc_udp_get_iface;
    TC_UDP_PK_PEER_ADDRESS_FNC tc_udp_pk_peer_address;
    TC_UDP_SET_HEADER_FNC      tc_udp_set_header;
    UDP_CLOSESOCKET_FNC        udp_closesocket;
    TC_UDP_BIND_FNC            tc_udp_bind;
    TC_UDP_CLOSE_FNC           tc_udp_close;
    TC_UDP_CONNECT_FNC         tc_udp_connect;
    TC_UDP_READ_FNC            tc_udp_read;
    TC_UDP_SOCK_ALLOC_FNC      tc_udp_sock_alloc;
    TC_UDP_WRITE_FNC           tc_udp_write;
    UDP_INVALIDATE_SOCKETS_FNC udp_invalidate_sockets;
    UDP_FIND_PORT_FNC          udp_find_port;

}   UDP_REGISTERED_FUNCTIONS;

typedef UDP_REGISTERED_FUNCTIONS * PUDP_REGISTERED_FUNCTIONS;

/* ********************************************************************      */


/* ********************************************************************      */

#ifdef __cplusplus
extern "C" {
#endif

int     xn_udp_initialize    ( void );

#ifdef __cplusplus
}
#endif

/* ********************************************************************      */

#endif /* #ifndef _UDP_H */
