 /*
 | RTPENV.H - Runtime Platform Environment Services
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

#ifndef __RTPENV_H__
#define __RTPENV_H__

#include "rtp.h"


/************************************************************************
 * API functions
 ************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

int rtp_setenv (const char * envname, char * value);
int rtp_getenv (const char * envname, char ** value);

#ifdef __cplusplus
}
#endif

#endif /*__RTPENV_H__*/

/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
