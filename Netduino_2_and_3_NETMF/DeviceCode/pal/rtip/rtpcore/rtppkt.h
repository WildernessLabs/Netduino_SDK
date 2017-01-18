 /*
 | RTPPKT.H - Runtime Platform Environment Services
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
 | Copyright EBSnet Inc. , 2004
 | All rights reserved.
 | This code may not be redistributed in source or linkable object form
 | without the consent of its author.
 |
 | Module description:
 |  [tbd]
*/

#ifndef __RTPPKT_H__
#define __RTPPKT_H__

#include "rtp.h"
#include "rtpsignl.h"

typedef RTP_HANDLE RTP_PKT_DEVICE;

typedef short RTP_PKT_ALLOC_REQUEST;

typedef struct s_RTP_PKT_HEAD RTP_PKT_HEAD;
typedef struct s_RTP_PKT_PART RTP_PKT_PART;

typedef int  (*RTP_PKT_CALLBACK) (void* callbackData, RTP_PKT_HEAD* packet);

typedef enum e_RTP_PKT_STATUS
{
	RTP_PKT_STATUS_ALLOC,
	RTP_PKT_STATUS_RECV,
	RTP_PKT_STATUS_SENDING,
	RTP_PKT_STATUS_QUEUED_FOR_SEND,
	RTP_PKT_STATUS_SENT,
	RTP_PKT_STATUS_ABORTED,
	RTP_PKT_STATUS_SEND_ERROR
}
RTP_PKT_STATUS;

struct s_RTP_PKT_PART
{
	const unsigned char* data;          /* DO NOT CHANGE the pointer location; only the data it points to! */
	long                 size;
	RTP_PKT_PART*        next;
};

struct s_RTP_PKT_HEAD
{
	RTP_PKT_HEAD*     next;
	RTP_PKT_HEAD*     prev;
	RTP_PKT_DEVICE    device;        /* this value is read-only! */
	unsigned char*    payloadData;   /* DO NOT CHANGE the pointer location; only the data it points to! */
	long              payloadSize;
	RTP_PKT_PART*     nextPart;
	unsigned char*    dstAddr;       /* DO NOT CHANGE the pointer location; only the data it points to! */
	unsigned char*    srcAddr;       /* DO NOT CHANGE the pointer location; only the data it points to! */
	unsigned char*    protocol;
	RTP_PKT_CALLBACK  sentCallbackFn;
	void*             sentCallbackData;

	/* the fields below this line MUST NOT be accessed by the application!!! */
	unsigned char*    buffer;
	long              bufferSize;
	RTP_PKT_STATUS    status;        /* 0 == allocated by alloc
	                                    1 == allocated by recv
	                                    2 == pending free when sent
	                                    3 == pending delay free
	                                    4 == sent ok to free */
};


#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
 * API functions
 ************************************************************************/

int            rtp_pkt_init                     (void);

void           rtp_pkt_exit                     (void);

int            rtp_pkt_open_device              (RTP_PKT_DEVICE* handle,
                                                 const char* type,
                                                 int number,
                                                 unsigned promiscuous);

void           rtp_pkt_close_device             (RTP_PKT_DEVICE handle);

int            rtp_pkt_set_recv_callback        (RTP_PKT_DEVICE   handle,
                                                 RTP_PKT_CALLBACK recvCallback,
                                                 void*            recvData);

long           rtp_pkt_send                     (RTP_PKT_HEAD* packet);

int            rtp_pkt_abort_send               (RTP_PKT_HEAD* packet);

int            rtp_pkt_recv                     (RTP_PKT_HEAD** packet,
                                                 long msecTimeout);

int            rtp_pkt_get_status               (RTP_PKT_HEAD* packet);

unsigned short rtp_pkt_calculate_ip_checksum    (const unsigned char* buffer,
                                                 unsigned short length,
                                                 unsigned long partialChecksum);

unsigned char  rtp_pkt_get_address_length       (RTP_PKT_DEVICE handle);

unsigned char  rtp_pkt_get_protocol_length      (RTP_PKT_DEVICE handle);

int            rtp_pkt_get_local_address        (RTP_PKT_DEVICE handle,
                                                 unsigned char* outBuffer);

int            rtp_pkt_get_broadcast_address    (RTP_PKT_DEVICE handle,
                                                 unsigned char* outBuffer);

long           rtp_pkt_get_device_mtu           (RTP_PKT_DEVICE handle);

unsigned       rtp_pkt_address_compare          (RTP_PKT_DEVICE handle,
                                                 unsigned char* addrOne,
                                                 unsigned char* addrTwo);

int            rtp_pkt_get_protocol_by_name     (RTP_PKT_DEVICE handle,
                                                 unsigned char* outBuffer,
                                                 const char* protocolName);

unsigned       rtp_pkt_compare_protocol_by_name (RTP_PKT_DEVICE handle,
                                                 unsigned char* inBuffer,
                                                 const char* protocolName);

RTP_PKT_HEAD*  rtp_pkt_alloc                    (RTP_PKT_DEVICE device,
                                                 long minPayloadSize);

int            rtp_pkt_alloc_async              (RTP_PKT_DEVICE device,
                                                 long minPayloadSize,
                                                 RTP_PKT_CALLBACK allocCallback,
                                                 void* allocData,
                                                 RTP_PKT_ALLOC_REQUEST* allocRequest,
                                                 int priority);

int            rtp_pkt_abort_alloc              (RTP_PKT_ALLOC_REQUEST* allocRequest);

RTP_PKT_PART*  rtp_pkt_alloc_part               (const unsigned char* data,
                                                 long size);

void           rtp_pkt_free                     (RTP_PKT_HEAD* packet);

void           rtp_pkt_free_part                (RTP_PKT_PART* part);

int            rtp_pkt_free_callback            (void* callbackData,
                                                 RTP_PKT_HEAD* packet);

#ifdef __cplusplus
}
#endif

#endif /*__RTPPKT_H__*/

/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
