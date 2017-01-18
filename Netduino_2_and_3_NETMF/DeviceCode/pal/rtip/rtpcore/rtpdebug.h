 /*
 | RTPDEBUG.H - Runtime Platform Debug Services
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

#ifndef __RTPDEBUG_H__
#define __RTPDEBUG_H__

#include "rtp.h"


/************************************************************************
 * If RTP_DEBUG is defined, the debug implementaion of the macro        *
 * will be implemented.  This provides debug information otherwise not  *
 * available.                                                           *
 ************************************************************************/
 
/* ---------------------------------------------- */
/* Enable this flag to include debug information. */
/* ---------------------------------------------- */
/* #ifndef RTP_DEBUG                              */
/* #define RTP_DEBUG                              */
/* #endif                                         */

#define RTP_DEBUG_MULTITHREAD 0		// SPR - why not defined

/************************************************************************
 * Debug System API
 ************************************************************************/

/************************************************************************
 * Debug System API which should be
 * used via the macros indicated below.
 ************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

void _rtp_debug_output_str (
	char* msg,
	const char *file,
	long line_num
	);
	
void _rtp_debug_output_int (
	long val
	);
 
#ifdef __cplusplus
}
#endif


#ifdef RTP_DEBUG
#define RTP_DEBUG_OUTPUT_STR(msg)  _rtp_debug_output_str(msg, __FILE__, __LINE__)
#define RTP_DEBUG_OUTPUT_INT(val)  _rtp_debug_output_int(val)
#else
#define RTP_DEBUG_OUTPUT_STR(msg)
#define RTP_DEBUG_OUTPUT_INT(msg)
#endif /* RTP_DEBUG */


#endif /* __RTPDEBUG_H__ */

/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
