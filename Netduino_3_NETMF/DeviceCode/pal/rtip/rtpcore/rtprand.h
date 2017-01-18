/*
|  RTPRAND.H - Runtime Platform Pseudo-random number generator
| 
|  EBSnet - 
| 
|   $Author: sarah $
|   $Date: 2005/05/14 02:43:30 $
|   $Name:  $
|   $Revision: 1.1 $
| 
|  Copyright EBSnet Inc. , 2004
|  All rights reserved.
|  This code may not be redistributed in source or linkable object form
|  without the consent of its author.
*/ 

#ifndef __RTPRAND_H__
#define __RTPRAND_H__

/*#define rtp_rand  */
/*#define rtp_srand */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef rtp_rand
int  rtp_rand  (void);
#endif
#ifndef rtp_srand
void rtp_srand (unsigned int seed);
#endif
#ifndef ssl_rand_seed
void ssl_rand_seed(const void *seed, int length);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __RTPRAND_H__ */
