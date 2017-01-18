 /*
 | RTPSIGNL.H - Runtime Platform Services
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

#ifndef __RTPSIGNL_H__
#define __RTPSIGNL_H__

#include "rtp.h" /* _yi_ */

/************************************************************************
 * Portable signalling services including semaphores (counting semaphores
 * or event queues) and mutexes (resource locks)
 ************************************************************************/

/************************************************************************
 * If RTP_TRACK_LOCAL_SEMAPHORES or RTP_TRACK_LOCAL_MUTEXES is turned   *
 * on, the debug implementaion of the corresponding signalling system   *
 * will be implemented.  This provides debug information otherwise not  *
 * available on most platforms.                                         *
 ************************************************************************/
 
/* ----------------------------------------------- */
/* Enable this flag to include semaphore tracking. */
/* ----------------------------------------------- */
/* #define RTP_TRACK_LOCAL_SEMAPHORES */

/* ----------------------------------------------- */
/* Enable this flag to include mutex tracking.     */
/* ----------------------------------------------- */
/* #define RTP_TRACK_LOCAL_MUTEXES    */

typedef RTP_HANDLE RTP_SEMAPHORE;
typedef RTP_HANDLE RTP_SIGNAL_CONTEXTP;
typedef RTP_HANDLE RTP_MUTEX;
typedef RTP_HANDLE RTP_MUTEX_CONTEXTP;

/************************************************************************
* Type definitions
*************************************************************************/

/************************************************************************
 * Signaling System API which should be                                 *
 * used via the macros that follow.                                     *
 ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
 * Semaphore (signalling) services                                      *
 ************************************************************************/
int  rtp_sig_semaphore_init        (int nSem, RTP_SIGNAL_CONTEXTP ctxBlock);
void rtp_sig_semaphore_shutdown    (void);
int  _rtp_sig_semaphore_alloc      (RTP_SEMAPHORE *newSem, const char *name);
void _rtp_sig_semaphore_free       (RTP_SEMAPHORE semHandle);
int  _rtp_sig_semaphore_wait_timed (RTP_SEMAPHORE semHandle, long msecs);
void _rtp_sig_semaphore_clear      (RTP_SEMAPHORE semHandle);
int  _rtp_sig_semaphore_wait       (RTP_SEMAPHORE semHandle);
void _rtp_sig_semaphore_signal     (RTP_SEMAPHORE semHandle);
void _rtp_sig_semaphore_signal_isr (RTP_SEMAPHORE semHandle);

/************************************************************************
 * Mutex (lock) services                                                *
 ************************************************************************/

int  rtp_sig_mutex_init         (int nMutex, RTP_MUTEX_CONTEXTP ctxBlock);
void rtp_sig_mutex_shutdown     (void);
int  _rtp_sig_mutex_alloc       (RTP_MUTEX *newMutex, const char *name); 
void _rtp_sig_mutex_free        (RTP_MUTEX mutexHandle);
int  _rtp_sig_mutex_claim_timed (RTP_MUTEX mutexHandle, long msecs);
int  _rtp_sig_mutex_claim       (RTP_MUTEX mutexHandle);
void _rtp_sig_mutex_release     (RTP_MUTEX mutexHandle);

#ifdef __cplusplus
}
#endif

#ifdef RTP_TRACK_LOCAL_SEMAPHORES
#include "rtpsemdb.h"
#endif

  #ifndef RTP_TRACK_LOCAL_SEMAPHORES
	/************************************************************************
	 * If RTP_TRACK_LOCAL_SEMAPHORES is not defined, semaphore access will  *
	 * be directed to the rtpsignl.c porting file and the semaphore debug   *
	 * utility functions will be defined to nothing to remove their use.    *
	 ************************************************************************/
	#define rtp_sig_semaphore_alloc(newSem, name)           _rtp_sig_semaphore_alloc(newSem, name)          
	#define rtp_sig_semaphore_free(semHandle)               _rtp_sig_semaphore_free(semHandle)              
	#define rtp_sig_semaphore_wait_timed(semHandle, msecs)  _rtp_sig_semaphore_wait_timed(semHandle, msecs) 
	#define rtp_sig_semaphore_clear(semHandle)              _rtp_sig_semaphore_clear(semHandle)             
	#define rtp_sig_semaphore_wait(semHandle)               _rtp_sig_semaphore_wait(semHandle)              
	#define rtp_sig_semaphore_signal(semHandle)             _rtp_sig_semaphore_signal(semHandle)            
	#define rtp_sig_semaphore_signal_isr(semHandle)         _rtp_sig_semaphore_signal_isr(semHandle)        
	
	#define rtp_debug_sig_semaphore_print_log( )
	#define rtp_debug_sig_semaphore_print_stdio(logFile, mode)
  #endif /* RTP_TRACK_LOCAL_SEMAPHORES */

#ifdef RTP_TRACK_LOCAL_MUTEXES
#include "rtpmtxdb.h"
#endif

  #ifndef RTP_TRACK_LOCAL_MUTEXES
	/************************************************************************
	 * If RTP_TRACK_LOCAL_MUTEXES is not defined, mutex access will         *
	 * be directed to the rtpsignl.c porting file and the mutex debug       *
	 * utility functions will be defined to nothing to remove their use.    *
	 ************************************************************************/
	#define rtp_sig_mutex_alloc(newMutex, name)             _rtp_sig_mutex_alloc(newMutex, name)
	#define rtp_sig_mutex_free(mutexHandle)                 _rtp_sig_mutex_free(mutexHandle)
	#define rtp_sig_mutex_claim_timed(mutexHandle, msecs)   _rtp_sig_mutex_claim_timed(mutexHandle, msecs)
	#define rtp_sig_mutex_claim(mutexHandle)                _rtp_sig_mutex_claim(mutexHandle)
	#define rtp_sig_mutex_release(mutexHandle)              _rtp_sig_mutex_release(mutexHandle)
	
	#define rtp_debug_sig_mutex_print_log( )
	#define rtp_debug_sig_mutex_print_stdio(logFile, mode)
  #endif /* RTP_TRACK_LOCAL_MUTEXES */

#endif /* __RTPSIGNL_H__ */

/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
