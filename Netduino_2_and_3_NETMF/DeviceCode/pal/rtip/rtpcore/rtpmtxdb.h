 /*
 | RTPMTXDB.H - Runtime Platform Services
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

#ifndef __RTPMTXDB_H__
#define __RTPMTXDB_H__

#include "rtp.h"

#ifdef RTP_TRACK_LOCAL_MUTEXES
/************************************************************************
 * If RTP_TRACK_LOCAL_MUTEXES is defined, the debug implementaion of    *
 * the corresponding mutex locking system will be implemented.  This    *
 * provides debug information otherwise not available on most platforms.*
 ************************************************************************/

/* ----------------------------------- */
/*  Number of mutexes to track. If     */
/*  more than this number of mutexes   */
/*  are allocated the excess mutexes   */
/*  will not be added to the track     */
/*  list, and therefore will not be    */
/*  taken into consideration at this   */
/*  layer.                             */
/* ----------------------------------- */
#define RTP_MUTEX_NUM_TO_TRACK          200

/************************************************************************
 * Mutex System Debug API which should be                               *
 * used via the macros that follow.                                     *
 ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
 * Mutex (lock) services                                                *
 ************************************************************************/

int  _rtp_debug_sig_mutex_alloc       (RTP_MUTEX *newMutex, const char *name, const char *file, long line_num); 
void _rtp_debug_sig_mutex_free        (RTP_MUTEX mutexHandle, const char *file, long line_num);
int  _rtp_debug_sig_mutex_claim_timed (RTP_MUTEX mutexHandle, long msecs, const char *file, long line_num);
int  _rtp_debug_sig_mutex_claim       (RTP_MUTEX mutexHandle, const char *file, long line_num);
void _rtp_debug_sig_mutex_release     (RTP_MUTEX mutexHandle, const char *file, long line_num);

void _rtp_debug_sig_mutex_print_log   (void);
void _rtp_debug_sig_mutex_print_stdio (const char* logFile, const char* mode);

#ifdef __cplusplus
}
#endif
  

/************************************************************************
 * If RTP_TRACK_LOCAL_MUTEXES is defined, mutex access will             *
 * be directed to the rtpmtxdb.c debug file and the mutex debug         *
 * utility functions will be defined accordingly.                       *
 ************************************************************************/
#define rtp_sig_mutex_alloc(newMutex, name)             _rtp_debug_sig_mutex_alloc(newMutex, name, __FILE__, __LINE__)
#define rtp_sig_mutex_free(mutexHandle)                 _rtp_debug_sig_mutex_free(mutexHandle, __FILE__, __LINE__)
#define rtp_sig_mutex_claim_timed(mutexHandle, msecs)   _rtp_debug_sig_mutex_claim_timed(mutexHandle, msecs, __FILE__, __LINE__)
#define rtp_sig_mutex_claim(mutexHandle)                _rtp_debug_sig_mutex_claim(mutexHandle, __FILE__, __LINE__)
#define rtp_sig_mutex_release(mutexHandle)              _rtp_debug_sig_mutex_release(mutexHandle, __FILE__, __LINE__)

#define rtp_debug_sig_mutex_snprint( )                  _rtp_debug_sig_mutex_snprint( )
#define rtp_debug_sig_mutex_print_stdio(logFile, mode)  _rtp_debug_sig_mutex_print_stdio(logFile, mode)

#endif /* defined(RTP_TRACK_LOCAL_SEMAPHORES) || defined(RTP_TRACK_LOCAL_MUTEXES) */

#endif /* __RTPMTXDB_H__ */



/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
