 /*
 | RTPDATE.H - Runtime Platform Services
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
 | Copyright EBSnet Inc. , 2003
 | All rights reserved.
 | This code may not be redistributed in source or linkable object form
 | without the consent of its author.
 |
 | Module description:
 |  [tbd]
*/

#ifndef __RTPDATE_H__
#define __RTPDATE_H__

#include "rtp.h"


/************************************************************************
 * Type definitions
 ************************************************************************/

/************************************************************************
 * RTP_DATE - date structure for API accessors/manipulators             *
 ************************************************************************/
 
typedef struct _RTP_DATE
{
	unsigned int  year;           /* year, AD                   */
	unsigned int  month;          /* 1 = January, 12 = December */
	unsigned int  day;            /* 1 = first of the month     */
	unsigned int  hour;           /* 0 = midnight, 12 = noon    */
	unsigned int  minute;         /* minutes past the hour      */
	unsigned int  second;         /* seconds in minute          */
	unsigned int  msec;           /* milliseconds in second     */

	/* These two fields help  */
	/* interpret the absolute */
	/* time meaning of the    */
	/* above values.          */
	unsigned int  dlsTime;        /* boolean; daylight savings time is in effect                      */
	int           tzOffset;       /* signed int; difference in seconds imposed by timezone (from GMT) */
}
RTP_DATE;

/************************************************************************
 * API functions
 ************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

int           rtp_set_date     (RTP_DATE * date);
int           rtp_get_date     (RTP_DATE * date);

#ifdef __cplusplus
}
#endif

#endif /*__RTPDATE_H__*/

/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
