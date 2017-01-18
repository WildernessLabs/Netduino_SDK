/*
   DHCPAPI.H  - DHCP header file

   EBS - RTIP

   Copyright EBSNet Inc. , 1996
   All rights reserved.
   This code may not be redistributed in source or linkable object form
   without the consent of its author.

   Module description:
    This file contains constants, structures, and function prototypes
    for use in DHCP applications.
*/

#ifndef __DHCPAPI_H__
#define __DHCPAPI_H__

#include "xnconf.h"
#include "dhcpconf.h"
#include "rtipapi.h"

/* TBD _yi_ remove */
#ifndef SOCKET
typedef int SOCKET;
#endif

/****************** CONSTANTS FOR DHCP OPTIONS ************************/
/* NOTE: values for the 'id' field of the struct DHCP_param structure */

#define DHCP_PAD                          0
#define DHCP_END                        255
#define DHCP_SUBNET_MASK                  1
#define DHCP_TIME_OFFSET                  2
#define DHCP_ROUTER_OPTION                3
#define DHCP_TIME_SERVER_OP               4
#define DHCP_NAME_SERVER_OP               5
#define DHCP_DNS_OP                       6
#define DHCP_LOG_SERVER_OP                7
#define DHCP_COOKIE_SERVER_OP             8
#define DHCP_LPR_SERVER_OPTION            9
#define DHCP_IMPRESS_SERVER_OP           10
#define DHCP_RLS_OP                      11
#define DHCP_HOST_NAME_OP                12
#define DHCP_BFS_OP                      13
#define DHCP_MERIT_DUMP                  14
#define DHCP_DOMAIN_NAME                 15
#define DHCP_SWAP_SERVER                 16
#define DHCP_ROOT_PATH                   17
#define DHCP_EXTENSIONS_PATH             18
#define DHCP_IP_FORWARDING               19
#define DHCP_NLSR                        20
#define DHCP_POLICY_FILTER               21
#define DHCP_MDRS                        22
#define DHCP_DEFAULT_IP_TTL              23
#define DHCP_PATH_MTU_AT                 24
#define DHCP_PATH_MTU_PLATEAU            25
#define DHCP_INTERFACE_MTU               26
#define DHCP_ALL_SUBNETS_LOCAL           27
#define DHCP_BROADCAST_ADDRESS           28
#define DHCP_MASK_DISCOVERY              29
#define DHCP_MASK_SUPPLIER               30
#define DHCP_ROUTER_DISCOVERY            31
#define DHCP_RSA_OP                      32
#define DHCP_STATIC_ROUTE_OP             33
#define DHCP_TRAILER_ENCAP_OP            34
#define DHCP_ARP_CT_OP                   35
#define DHCP_EE_OP                       36
#define DHCP_TCP_DEFAULT_TTL             37
#define DHCP_TCP_KA_INTERVAL             38
#define DHCP_TCP_KA_GARBAGE              39
#define DHCP_NISD                        40
#define DHCP_NIS_OP                      41
#define DHCP_NTPS_OP                     42
#define DHCP_VENDOR_SPECIFIC             43
#define DHCP_NET_OVER_TCP_NS             44
#define DHCP_NET_OVER_TCP_DDS            45
#define DHCP_NET_OVER_TCP_NT             46
#define DHCP_NET_OVER_TCP_SCOPE          47
#define DHCP_XWIN_SFS                    48
#define DHCP_XWIN_SDM                    49
#define DHCP_REQ_IP                      50
#define DHCP_IP_LEASE                    51
#define DHCP_OP_OVERLOAD                 52
#define DHCP_MSG_TYPE                    53
#define DHCP_SERVER_ID                   54
#define DHCP_PARAM_REQ_LST               55
#define DHCP_MESSAGE                     56
#define DHCP_MAX_DHCP_MSG_SIZE           57
#define DHCP_RENEWAL_TIME                58
#define DHCP_REBINDING_TIME              59
#define DHCP_CLASS_ID                    60
#define DHCP_CLIENT_ID                   61
#if (INCLUDE_TFTP_CISCO)                
#define DHCP_TFTP_ADDR_CISCO            150      // Cisco option to be TFTP address
#endif

// *********************************************************************
/********** DHCP ARRAY SIZE CONSTANTS **********/
#define DHCP_PKT_OP_SIZE                312
#define DHCP_PKT_SNAME_SIZE              64
#define DHCP_PKT_FILE_SIZE              128
#define DHCP_PKT_CHADDR_SIZE             16

// struct DHCP_packet
typedef struct DHCP_packet
{
    RTP_UINT8  op;
    RTP_UINT8  htype;
    RTP_UINT8  hlen;
    RTP_UINT8  hops;
    RTP_UINT32 xid;
    RTP_UINT16 secs;
    RTP_UINT16 flags;
    RTP_UINT32 ciaddr;                              // address client knows it wants
    RTP_UINT32 yiaddr;
    RTP_UINT32 siaddr;
    RTP_UINT32 giaddr;
    RTP_UINT8  chaddr[DHCP_PKT_CHADDR_SIZE];        // client ether address
    RTP_UINT8  sname[DHCP_PKT_SNAME_SIZE];
    RTP_UINT8  file[DHCP_PKT_FILE_SIZE];
    // create a large array for the options to be formatted into; this
    // should be large enough to hold all the option
    // NOTE: when the packet is sent, this whole buffer is send; 0 option
    //       marks the end of the options so sending a larger packet
    //       than necessary has not caused any problems with servers
    //       However, the code could keep track of the options which
    //       were formatted and send the correct size packet
    RTP_UINT8  options[DHCP_PKT_OP_SIZE];

    RTP_UINT16  options_index;                      // internal
    RTP_UINT8  overload_type;                       // internal
    RTP_UINT8  byte_even;                           // pad to make structure even # of bytes
} DHCP_packet;
typedef struct DHCP_packet RTP_FAR * PFDHCP_packet;

#define DHCP_PKT_EXTRA  4                           // size of internal fields in packet

#if (INCLUDE_DHCP_CLI)                              // exclude rest of file


// *********************************************************************
// ******                CONFIGURATION                            ******
// *********************************************************************
#if (INCLUDE_RUN_TIME_CONFIG)
typedef struct cfg_dhcpc_data
{
#if (CONFIG_PARAM_DONE)
    int dhcp_offers;
#endif
    int dhcp_retries;
    int dhcp_timeout;
} CFG_DHCPC_DATA;

#if (CONFIG_PARAM_DONE)
#define CFG_DHCP_OFFERS     cfg_dhcpc_data.dhcp_offers
#else
#define CFG_DHCP_OFFERS     _CFG_DHCP_OFFERS
#endif
#define CFG_DHCP_RETRIES    cfg_dhcpc_data.dhcp_retries
#define CFG_DHCP_TIMEOUT    cfg_dhcpc_data.dhcp_timeout

extern CFG_DHCPC_DATA RTP_FAR cfg_dhcpc_data;

#else
#define CFG_DHCP_OFFERS     _CFG_DHCP_OFFERS
#define CFG_DHCP_RETRIES    _CFG_DHCP_RETRIES
#define CFG_DHCP_TIMEOUT    _CFG_DHCP_TIMEOUT
#endif

/* ************************************************************************    */
/********** CONSTANTS FOR 'packet_style' in DHCP_session **********/
// for DHCP_MICROSOFT: the REQ_ID option is put in the DHCP request packet
// for DHCP_RFC: the REQ_ID option is not put in the DHCP request packet
#define DHCP_RFC        1
#define DHCP_MICROSOFT  2

/********** CONSTANT for 'prio' field of DHCP_param **********/
#define DHCP_MUST_HAVE  100

// parameter to request from server
typedef struct DHCP_param
{
    RTP_UINT8 id;       // option (DHCP_SUBNET_MASK for example)
    RTP_UINT8 prio;     // 0-99, 100=must have
} DHCP_param;

typedef struct DHCP_param RTP_FAR * PFDHCP_param;

// *********************************************************************
/* parameter for client to send to server   */
typedef struct DHCP_cparam
{
    RTP_UINT8 id;       /* option (HOST_NAME_OP for example) */
    RTP_UINT8 len;      /* length of option */
    RTP_UINT8 fill[2];  // alignment
    void *cpdata;       /* point to option - if string must be null terminated 
                                             for example, domain name */
    void *next;         /* point to next parameter entry */
} DHCP_cparam;

typedef struct DHCP_cparam RTP_FAR * PFDHCP_cparam;

// *********************************************************************
// STRUCT DHCP_conf
typedef struct DHCP_conf
{
    PFDHCP_param plist; // list of parameters to request from server
                        // set to NULL if:
                        // a) you have no parameters to request,
                        // b) you are requesting only the RTIP standard
                        // parameters, and have set request_std_params to
                        // RTP_TRUE.
    int          plist_entries;
    PFDHCP_cparam cplist; /* list of parameters to send to server         */
                        /* set to NULL if:                                */
                        /* a) you have no parameters to send              */
    int          cplist_entries;
    RTP_BOOL request_std_params;    // set to RTP_TRUE to request RTIP standard
                                    // configuration parameters in addition
                                    // to those in plist (if any). Any RTIP
                                    // standard parameters returned by the
                                    // server can be accessed through the
                                    // 'std_params' field of DHCP_session.
    RTP_BOOL apply_std_params;      // if set to RTP_TRUE, any RTIP standard
                                    // parameters returned by the server
                                    // will be applied. 
    RTP_UINT32        lease_time;         // desired ip lease time (units: seconds)
    RTP_UINT8         client_ip_addr[IP_ALEN];  // fill in if known, otherwise you will
                                        // be leased one from server
    RTP_UINT8         req_ip_addr[IP_ALEN];
    RTP_PFUINT8       vendor_specific;
    int               vs_size;
} DHCP_conf;
typedef struct DHCP_conf RTP_FAR * PFDHCP_conf;

// *********************************************************************
// STRUCT DHCP_std_params;
// the pointers are setup to point to the option information; if the
// option is not available the entry is set to 0;
// the types for the options are based upon the types in the message
// received (i.e. 1 byte, 2 bytes or 4 bytes); they are converted
// to the internal RTIP type if they differ
typedef struct DHCP_std_params /* RTIP standard configuration params */
{
    /* Capitalized names in comments correspond to constants for these
       options. */
    RTP_UINT32 *    subnet_mask;        /* DHCP_SUBNET_MASK */
    RTP_UINT32 *    dns_server;         /* DNS */
    int             dns_server_len;     /* length of dns */
    RTP_UINT8 *     domain_name;        /* DHCP_DOMAIN_NAME */
    int             domain_name_len;    /* length of domain name */
    RTP_UINT8 *     be_a_router;        /* ROUTER_OPTION */
    RTP_UINT16 *    mdrs;               /* DHCP_MDRS (maximum datagram reassembly size) */
    RTP_UINT8 *     default_ip_ttl;     /* DHCP_DEFAULT_IP_TTL */
    RTP_UINT16 *    mtu;                /* DHCP_PATH_MTU_AT */
    RTP_UINT32 *    static_route;       /* STATIC_ROUTE_OP */
    int             static_route_len;   /* length of static route information */
    RTP_UINT32 *    router_option;      /* ROUTER_OPTION */
    int             router_option_len;  /* length of router option information */
    RTP_UINT32 *    arp_cache_timeout;  /* ARP_CT_OP */
    RTP_UINT8 *     tcp_default_ttl;    /* TCP_DEFAULT_TTL */
    RTP_UINT32 *    tcp_ka_interval;    /* TCP_KA_INTERVAL */
    RTP_UINT8 *     tcp_ka_garbage;     /* TCP_KA_GARBAGE */
#if (INCLUDE_TFTP_CISCO)
    RTP_UINT32 *    tftp_srv_ip;        /* TFTP_ADDR_CISCO */
#endif
} DHCP_std_params /* RTIP standard configuration params */;

typedef struct DHCP_std_params RTP_FAR * PFDHCP_std_params;

typedef struct DHCP_saved_offer
{
     RTP_UINT8 your_ip_address[IP_ALEN];
} DHCP_saved_offer;


// *********************************************************************
// session structure passed to xn_dhcp
typedef struct DHCP_session
{
    // specify whether to specify the server ID in the DHCP REQUEST;
    // options are:
    //   - DHCP_MICROSOFT: the REQ_ID option is put in the DHCP request packet
    //     (this is RFC complient)
    //   - DHCP_RFC: the REQ_ID option is not put in the DHCP request packet
    //     (this is NOT RFC complient although some servers will not
    //     work if this option is specified)
    int                    packet_style;
    struct DHCP_std_params params;

    RTP_UINT8              client_ip[IP_ALEN];
    RTP_UINT32             lease_time;      // units: seconds

    /* Internal */
    RTP_UINT8              server_ip[IP_ALEN]; // server_id from request chosen
    struct DHCP_packet     outmsg;
    struct DHCP_packet     replies[CFG_DHCP_OFFERS];
    int                    iface_no;
    SOCKET                 xn_dhcp_sock;      // used by xn_dhcp
#if (INCLUDE_DHCP_RENEW_NO_BLOCK)
    SOCKET                 dhcpc_sock;        // used by dhcp_timeout 
#if (RTPLATFORM)
    long                   timeout_secs;
#else
    long                   timeout_ticks;
#endif
    int                    retries;
    RTP_UINT32             start_time;  // seconds for RTPLATFORM
    int                    rindex;
#endif
    struct DHCP_saved_offer       saved_parameters;
} DHCP_session;
typedef struct DHCP_session RTP_FAR * PFDHCP_session;

typedef int (*DHCP_callback_fp)(PFDHCP_packet,int,PFDHCP_conf);

// *********************************************************************
/********** DHCP CLIENT API FUNCTIONS (DHCPAPI.C) **********/

#ifdef __cplusplus
extern "C" {
#endif

// basic API functions
void init_dhcp(void);
void xn_dhcp_init_conf(PFDHCP_conf conf);
int  xn_dhcp(int iface_no,PFDHCP_session session,PFDHCP_conf conf);
int  xn_dhcp_inform(int iface_no, PFDHCP_session session, PFDHCP_conf conf);
int  xn_dhcp_extend_lease(PFDHCP_session session,RTP_UINT32 lease_time);
int  xn_dhcp_release(PFDHCP_session session);

// API functions for advanced use of DHCP:
void xn_dhcp_set_callback(DHCP_callback_fp fp);
int  xn_dhcp_get_op(PFDHCP_session session,RTP_UINT8 op_id,RTP_UINT8 bufsize,RTP_PFVOID vpdest);
void xn_dhcp_set_conf_op(PFDHCP_conf conf,PFDHCP_cparam cpdata);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_DHCP_CLI
#endif  // __DHCPAPI_H__
