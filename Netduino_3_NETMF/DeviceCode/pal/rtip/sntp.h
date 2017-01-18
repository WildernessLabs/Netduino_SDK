//
// SNTP.H  - SNTP header file
//
//   EBS - RTIP
//
//   Copyright EBSNet Inc. , 1993
//   All rights reserved.
//   This code may not be redistributed in source or linkable object form
//   without the consent of its author.
//
//    Module description:
//        This file contains prototypes constants and structure
//        declarations required to build SNTP,
//        Simple Network Time Protocol
//
#ifndef __SNTP__
#define __SNTP__ 1

// ********************************************************************
#include "timeapi.h"

// ********************************************************************
#define SNTP_TIME_OUT            -3
#define SNTP_WANT_READ_WRITE     -2
#define SNTP_ERROR               -1

// ********************************************************************
#define NTP_PORT 123    // source and destination ports

// ********************************************************************
typedef struct ntp_time_format
{
    RTP_UINT32 seconds;
    RTP_UINT32 frac_sec;
} NTP_TIME_FORMAT;
typedef struct ntp_time_format RTP_FAR * PNTP_TIME_FORMAT;

typedef struct ntp_pkt
{
// mode bits
#define NTP_MODE_MASK 0x07
#define NTP_SYM_ACTIVE  1
#define NTP_SYM_PASSIVE 2
#define NTP_REQUEST     3
#define NTP_RESPONSE    4
#define NTP_BROADCAST   5

// VN - version bits
#define NTP_VERSION_MASK    0x38

    RTP_UINT8   li_vn_mode; // LI (2 bits), VN (3 bits), mode (3 bits)
    RTP_UINT8   stratum;
    RTP_UINT8   poll;
    RTP_UINT8   precision;
    RTP_UINT32  root_delay;
    RTP_UINT32  root_dispersion;
    char    ref_id[4];                  // ids the reference source such as
                                        // LOCL, PPS, WWVB, GPS, GOES etc.
                                        // set by server:
    NTP_TIME_FORMAT ref_timestamp;      // reference timestamp
                                        // set by client
    NTP_TIME_FORMAT orig_timestamp;     // origional timestamp
                                        // set by client
    NTP_TIME_FORMAT rcv_timestamp;      // request received timestamp;
                                        // set by server
    NTP_TIME_FORMAT transmit_timestamp;// time reply departed server;
                                        // set by server
//  RTP_UINT32  key_id;
//  RTP_UINT8    digest[128];               // optional ?
} NTP_PKT;
typedef struct ntp_pkt RTP_FAR * PNTP_PKT;

// ********************************************************************
// data passed between timout routine
// ********************************************************************
typedef struct sntp_timeout_data
{
    int             sntp_socket;
    RTP_UINT8       sntp_ip_addr[CFG_SNTP_NUM_SERVERS*IP_ALEN];
    int             sntp_num_servers;   // number of servers in sntp_ip_addr
    int             sntp_elapsed_sec;   // ctr for delay between requests
    int             sntp_select_timer;
    int             sntp_step_timeout;
    NTP_TIME_FORMAT current_timestamp;  // local time
    int             version;
    int             mode;               // NTP_REQUEST or NTP_SYM_PASSIVE
    RTP_UINT32           ticks_when_sent;
} SNTP_TIMEOUT_DATA;
typedef struct sntp_timeout_data RTP_FAR * PSNTP_TIMEOUT_DATA;

// ********************************************************************
// enables using code as DLL
// NOTE: the same define is in rtip.h
#define SOCKAPI_
//#define SOCKAPI_  __stdcall __export

// ********************************************************************
int sntp_request_time(RTP_PFUINT8 ip_sntp_server, int *socket_no, 
                  PNTP_TIME_FORMAT current_timestamp,
                  int version, int mode, RTP_UINT32 *ticks_when_sent,
                  PEBSTIME res_ebs_tod);
int sntp_process_result(int socket_no, PNTP_TIME_FORMAT current_timestamp,
                        int version, RTP_UINT32 ticks_when_sent,
                        PEBSTIME res_ebs_tod);

#endif  // __SNTP__

