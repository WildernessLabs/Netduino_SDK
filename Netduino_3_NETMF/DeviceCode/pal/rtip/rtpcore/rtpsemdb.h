 /*
 | RTPSEMDB.H - Runtime Platform Services
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

#ifndef __RTPSEMDB_H__
#define __RTPSEMDB_H__

#include "rtp.h"

#ifdef RTP_TRACK_LOCAL_SEMAPHORES
/************************************************************************
 * If RTP_TRACK_LOCAL_SEMAPHORES is defined, the debug implementaion    *
 * of the corresponding semaphore system will be implemented.  This     *
 * provides debug information otherwise not available on most platforms.*
 ************************************************************************/
 
/* ----------------------------------- */
/*  Number of semaphores to track. If  */
/*  more than this number of semaphores*/
/*  are allocated the excess semaphores*/
/*  will not be added to the track     */
/*  list, and therefore will not be    */
/*  taken into consideration at this   */
/*  layer.                             */
/* ----------------------------------- */
#define RTP_SEMAPHORE_NUM_TO_TRACK      200

/************************************************************************
 * Semaphore System Debug API which should be                           *
 * used via the macros that follow.                                     *
 ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
 * Semaphore (signalling) services                                      *
 ************************************************************************/

int  _rtp_debug_sig_semaphore_alloc       (RTP_SEMAPHORE *newSem, const char *name, const char *file, long line_num);
void _rtp_debug_sig_semaphore_free        (RTP_SEMAPHORE semHandle, const char *file, long line_num);
int  _rtp_debug_sig_semaphore_wait_timed  (RTP_SEMAPHORE semHandle, long msecs, const char *file, long line_num);
void _rtp_debug_sig_semaphore_clear       (RTP_SEMAPHORE semHandle, const char *file, long line_num);
int  _rtp_debug_sig_semaphore_wait        (RTP_SEMAPHORE semHandle, const char *file, long line_num);
void _rtp_debug_sig_semaphore_signal      (RTP_SEMAPHORE semHandle, const char *file, long line_num);
void _rtp_debug_sig_semaphore_signal_isr  (RTP_SEMAPHORE semHandle, const char *file, long line_num);
                                          
void _rtp_debug_sig_semaphore_print_log   (void);
void _rtp_debug_sig_semaphore_print_stdio (const char* logFile, const char* mode);

#ifdef __cplusplus
}
#endif

/************************************************************************
 * If RTP_TRACK_LOCAL_SEMAPHORES is defined, semaphore access will      *
 * be directed to the rtpsemdb.c debug file and the semaphore debug     *
 * utility functions will be defined accordingly.                       *
 ************************************************************************/
#define rtp_sig_semaphore_alloc(newSem, name)              _rtp_debug_sig_semaphore_alloc(newSem, name, __FILE__, __LINE__)          
#define rtp_sig_semaphore_free(semHandle)                  _rtp_debug_sig_semaphore_free(semHandle, __FILE__, __LINE__)              
#define rtp_sig_semaphore_wait_timed(semHandle, msecs)     _rtp_debug_sig_semaphore_wait_timed(semHandle, msecs, __FILE__, __LINE__) 
#define rtp_sig_semaphore_clear(semHandle)                 _rtp_debug_sig_semaphore_clear(semHandle, __FILE__, __LINE__)             
#define rtp_sig_semaphore_wait(semHandle)                  _rtp_debug_sig_semaphore_wait(semHandle, __FILE__, __LINE__)              
#define rtp_sig_semaphore_signal(semHandle)                _rtp_debug_sig_semaphore_signal(semHandle, __FILE__, __LINE__)            
#define rtp_sig_semaphore_signal_isr(semHandle)            _rtp_debug_sig_semaphore_signal_isr(semHandle, __FILE__, __LINE__)        
                                                           
#define rtp_debug_sig_semaphore_print_log( )               _rtp_debug_sig_semaphore_print_log( )
#define rtp_debug_sig_semaphore_print_stdio(logFile, mode) _rtp_debug_sig_semaphore_print_stdio(logFile, mode)


#endif /* RTP_TRACK_LOCAL_SEMAPHORES */

#endif /* __RTPSEMDB_H__ */



/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
