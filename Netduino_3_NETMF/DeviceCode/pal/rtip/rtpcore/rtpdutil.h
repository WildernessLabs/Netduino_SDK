 /*
 | RTPDUTIL.H - Runtime Platform Services
 |
 |   UNIVERSAL CODE - DO NOT CHANGE
 |
 | EBSnet - RT-Platform 
 |
 |  $Author: sarah $
 |  $Date: 2005/05/14 02:43:29 $
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

#ifndef __RTPDUTIL_H__
#define __RTPDUTIL_H__

#include "rtpdate.h"


/************************************************************************
 * Type definitions
 ************************************************************************/
#define RTP_MAX_DATE_STR_LEN      32

/************************************************************************
 * RTP_TIMESTAMP - date structure for API accessors/manipulators        *
 ************************************************************************/
 
typedef struct _RTP_TIMESTAMP
{
	short year;
	long  second;
}
RTP_TIMESTAMP;


/************************************************************************
 * API functions
 ************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

unsigned long rtp_get_utc_date             (void);
int           rtp_date_utc_to_gmt          (RTP_DATE * gmtDate, unsigned long utcSec);
void          rtp_date_to_timestamp        (RTP_TIMESTAMP * ptime, RTP_DATE * pdate);
void          rtp_timestamp_to_date        (RTP_DATE * pdate, RTP_TIMESTAMP * ptime);
int           rtp_date_get_timestamp       (RTP_TIMESTAMP * ptime);
int           rtp_date_print_time          (char *str, RTP_TIMESTAMP * ptime, int style);
int           rtp_date_parse_time          (RTP_TIMESTAMP * ptime, const char *str);
int           rtp_date_parse_time_uc       (RTP_TIMESTAMP * ptime, const unsigned short *str);
int           rtp_date_compare_time        (RTP_TIMESTAMP * ptime1, RTP_TIMESTAMP * ptime2);
void          rtp_date_set_time_forever    (RTP_TIMESTAMP * ptime);
void          rtp_date_add_seconds         (RTP_TIMESTAMP * ptime, long seconds);
long          rtp_date_get_seconds_in_year (short year);
long          rtp_date_time_difference     (RTP_TIMESTAMP * pearlier, RTP_TIMESTAMP * plater);
int           rtp_date_get_dayofweek       (long year, long month, long day);
long          rtp_date_get_seconds_by_date (short year, short month, short day, short hour, 
                                            short minute, short second);

#ifdef __cplusplus
}
#endif

#endif /*__RTPDUTIL_H__*/

/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
