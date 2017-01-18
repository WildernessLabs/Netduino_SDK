 /*
 | RTPEXIT.H - Runtime Platform Services
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

#ifndef __RTPEXIT_H__
#define __RTPEXIT_H__

#include "rtp.h"

/************************************************************************
 * Type definitions
 ************************************************************************/

/************************************************************************
 * Kernel API                                                           *
 ************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

void rtp_abort (void);
void rtp_exit  (int exitvalue);

#ifdef __cplusplus
}
#endif

#endif /* __RTPEXIT_H__ */

/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
