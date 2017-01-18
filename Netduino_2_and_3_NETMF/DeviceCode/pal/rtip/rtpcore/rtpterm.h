 /*
 | RTPTERM.H - Runtime Platform Services
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

#ifndef __RTPTERM_H__
#define __RTPTERM_H__

#include "rtp.h"


/************************************************************************
 * Terminal API                                                         *
 ************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TERMINALH__
int     rtp_term_init           (void);
int     rtp_term_kbhit          (void);
int     rtp_term_getch          (void);
void    rtp_term_putc           (char ch);
void    rtp_term_puts           (const char * string);
int     rtp_term_cputs          (const char * string);
int     rtp_term_gets           (char * string);
int     rtp_term_promptstring   (char * string, unsigned int handle_arrows);

int     rtp_term_up_arrow       (void);
int     rtp_term_down_arrow     (void);
int     rtp_term_left_arrow     (void);
int     rtp_term_right_arrow    (void);
int     rtp_term_escape_key     (void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __RTPTERM_H__ */

/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
