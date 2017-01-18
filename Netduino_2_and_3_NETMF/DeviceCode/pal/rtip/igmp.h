//
// IGMP.H  - IGMP header file				
//
//   EBS - RTIP
//
//   Copyright Peter Van Oudenaren , 1993
//   All rights reserved.
//   This code may not be redistributed in source or linkable object form
//   without the consent of its author.
//
//    Module description:
//        This file contains prototypes for IGMP.
//

#ifndef __IGMP__
#define __IGMP__ 1

/* ************************************************************************    */
/* *********************            IGMP            ***********************    */
/* ************************************************************************    */

/* Maximum delay for sending a IGMP report after receiving an IGMP query;
   If report is sent by another IGMP host before the timer expires the
   timer will be stopped and the report will not be sent */
// Note: This is only used for queries from an IGMP Version 1 router
/* Units: seconds   */
#define _CFG_IGMPV1_MAX_DELAY 10

// Number of sockets per interface record
#define CFG_IGMPV3_IFACE_NUM_SOCKETS 	5

// Number of records an interface contains
#define CFG_IGMPV3_RECORDS_PER_IFACE    5


#define CFG_IGMPV3_NUM_SOCKETS_RECORD	5

// Number of records a socket contains
#define CFG_IGMPV3_RECORDS_PER_SOCKET    5

// ********************************************************************
// IGMP RUN-TIME CONFIGURATION
// ********************************************************************
#if (INCLUDE_RUN_TIME_CONFIG)
typedef struct cfg_igmp_data
{
	int  igmpv1_max_delay;
} CFG_IGMP_DATA;
#endif

#if (INCLUDE_RUN_TIME_CONFIG)
extern CFG_IGMP_DATA RTP_FAR cfg_igmp_data;
#endif

#if (INCLUDE_RUN_TIME_CONFIG)
#define CFG_IGMPV1_MAX_DELAY			cfg_igmp_data.igmpv1_max_delay
#else
#define CFG_IGMPV1_MAX_DELAY			_CFG_IGMPV1_MAX_DELAY
#endif

// ********************************************************************
// IGMP PACKETS
// ********************************************************************
#if (INCLUDE_IGMP)
// igmp protocol frame structure
#define IGMP_HLEN       4
#define IGMP_HLEN_BYTES 8
#define IGMP_TLEN_BYTES (IGMP_HLEN_BYTES+IP_HLEN_BYTES+LL_HLEN_BYTES)

#define IGMP_V1   0x01   // used for type of querier on a given interface
#define IGMP_V2   0x02
#define IGMP_V3   0x03

// Various packet types
// IGMP types (lower 4 bits of igmp_vertype field of IGMP message)
#define IGMP_QUERY      1               // host membership query
#define IGMP_REPORT     2               // host membership report

// IGMP types
#define IGMP123_QUERY  0x11     //host membership query
			   					// group addr = 0 is general query
						   		// group addr filled in is specific query
								// for that group
#define IGMP1_REPORT  0x12 		// Version 1 host membership report for
								// backwards compatibility
							   	// with version 1
#define IGMP2_REPORT  0x16 		// Version 2 host membership report
#define IGMP2_LEAVE   0x17 		// Version 2 host leave group

#define IGMP3_QUERY   0x11       //host membership query
#define IGMP3_REPORT  0x21       //host membership query

#if (INCLUDE_IGMP_V1)
typedef struct _igmp
{
 	RTP_UINT8    igmp_vertype;          	// 4 hi bits == version, lo 4 == type
	RTP_UINT8    igmp_unused;
	RTP_UINT16   igmp_chksum;                  // checksum
	RTP_UINT8    igmp_ip_addr[IP_ALEN];  	// group address
} IGMPV1PKT;

typedef IGMPV1PKT RTP_FAR * PIGMPV1PKT;

#endif		// INCLUDE_IGMP_V1

#if (INCLUDE_IGMP_V2)
typedef struct _igmpv2
{
    RTP_UINT8    igmp_type;            	// just type, version not included anymore
	RTP_UINT8    igmp_max_resp_time;    // Max Response Time
	RTP_UINT16   igmp_chksum;           // checksum
	RTP_UINT8    igmp_ip_addr[IP_ALEN];	// group address
} IGMPV2PKT;

typedef IGMPV2PKT RTP_FAR * PIGMPV2PKT;

#endif		// INCLUDE_IGMP_V2

#if (INCLUDE_IGMP_V2 || INCLUDE_IGMP_V3)
// how long in secs  a host must wait after
// hearing a V1 Query before may send V2 messages
#define VERS1_ROUTER_PRESENT_TMO 400 
#endif

#if (INCLUDE_IGMP_V3)
typedef struct _igmp3pkt_query
{
    RTP_UINT8    igmp_type;            	// just type, version not included anymore
	RTP_UINT8    igmp_max_resp_time;    // Max Response Time
	RTP_UINT16   igmp_chksum;           // checksum
	RTP_UINT8    igmp_ip_addr[IP_ALEN];	// multicast address
	RTP_UINT8	 igmp_qrv;
	RTP_UINT8	 igmp_qqic;
	RTP_UINT16	 igmp_num_src;
	// IGMPV3 SOURCES (unicast addresses) go here
} IGMPV3PKT_QUERY;
typedef IGMPV3PKT_QUERY RTP_FAR * PIGMPV3PKT_QUERY;

#define IGMPV3_REPORT  0x22 		// Version 3 host membership report

typedef struct _igmp3pkt_report
{
    RTP_UINT8    igmp_type;            		// record type
	RTP_UINT8    igmp_reserved1;
	RTP_UINT16   igmp_chksum;
	RTP_UINT16   igmp_reserved2;
	RTP_UINT16   igmp_num_records;
	// IGMP V3 RECORDS go here
} IGMPV3PKT_REPORT;
typedef IGMPV3PKT_REPORT RTP_FAR * PIGMPV3PKT_REPORT;

typedef struct _igmp3_record
{
    RTP_UINT8    igmp_record_type;     		// record type
	RTP_UINT8    igmp_aux_data_len;			// aux data length
	RTP_UINT16   igmp_num_src;          	// number of unicast IP addresses below
	RTP_UINT8    igmp_mcast_addr[IP_ALEN];	// multicast address
	// IP addresses go here
} IGMPV3_RECORD;
typedef IGMPV3_RECORD RTP_FAR * PIGMPV3_RECORD;

// part of interface structure
typedef struct igmpv3_iface_record
{
	RTP_UINT8	mcast_addr[IP_ALEN];		// ip_nulladdr means free entry

	// goes in igmp_record_type in igmp3_record as well
	#define MODE_INVALID            0x00
	#define MODE_IS_INCLUDE  		0x01
	#define MODE_IS_EXCLUDE			0x02
	#define CHANGE_TO_INCLUDE_MODE 	0x03
	#define CHANGE_TO_EXCLUDE_MODE 	0x04
	#define ALLOW_NEW_SOURCES	 	0x05
	#define BLOCK_OLD_SOURCES	 	0x06

	RTP_UINT8	filter_mode;

	int			num_sockets;
	SOCKET   	socket_list[CFG_IGMPV3_IFACE_NUM_SOCKETS];
} IGMPV3_IFACE_RECORD;

typedef struct igmpv3_iface_record RTP_FAR *PIGMPV3_IFACE_RECORD;

// part of UDP socket structure
typedef struct igmpv3_mcast_socket_record
{
	int			iface_no;				// -1 means free entry
	RTP_UINT8	filter_mode;
	// multicast addresses listening to
	RTP_UINT8	mcast_addr[IP_ALEN];

	// unicast addresses for the socket
	RTP_UINT8	 ip_addr_array[CFG_IGMPV3_NUM_SOCKETS_RECORD][IP_ALEN];
} IGMPV3_MCAST_SOCKET_RECORD;

typedef struct igmpv3_mcast_socket_record RTP_FAR *PIGMPV3_MCAST_SOCKET_RECORD;

// ********************************************************************
// FUNCTIONS CALLED BY RTIP

#if (INCLUDE_RUN_TIME_CONFIG)
RTP_BOOL alloc_igmp_data(PIFACE pi);
#endif
void igmp_send_report(PIFACE pi, RTP_PFUINT8 ip_addr);
#if (INCLUDE_IGMP_V2)
void igmp_leave_group(PIFACE pi, RTP_PFUINT8 mcast_ip_addr);
#endif

// ********************************************************************
// IGMP V3 API
// ********************************************************************
void xn_ip_multicast_init(void);
int  xn_ip_multicast_listen(SOCKET sock, int iface_no, RTP_PFUINT8 mcast_addr, 
						    RTP_UINT8 filter_mode, int num_src, 
							RTP_PFUINT8 src_list);

#endif 	// INCLUDE_IGMP_V3
int igmp_iface_open(PIFACE pi);
#endif	// INCLUDE_IGMP

#endif
