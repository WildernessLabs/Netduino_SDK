/*
|  RTPWCHAR.H - 
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

#ifndef __RTPWCHAR_H__
#define __RTPWCHAR_H__

/*#define rtp_iswalnum   */
/*#define rtp_iswcntrl   */
/*#define rtp_iswdigit   */
/*#define rtp_iswprint   */
/*#define rtp_iswspace   */
/*#define rtp_iswupper   */
/*#define rtp_iswxdigit  */
/*#define rtp_towlower   */
/*#define rtp_towupper   */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef rtp_iswalnum
int     rtp_iswalnum (int chr);
#endif
#ifndef rtp_iswcntrl
int     rtp_iswcntrl (int ch);
#endif
#ifndef rtp_iswdigit
int     rtp_iswdigit (int ch);
#endif
#ifndef rtp_iswprint
int     rtp_iswprint (int ch);
#endif
#ifndef rtp_iswspace
int     rtp_iswspace (int ch);
#endif
#ifndef rtp_iswupper
int     rtp_iswupper (int ch);
#endif
#ifndef rtp_iswxdigit
int     rtp_iswxdigit(int chr);
#endif

#ifndef rtp_towlower
int     rtp_towlower  (int c);
#endif
#ifndef rtp_towupper
int     rtp_towupper  (int c);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __RTPWCHAR_H__ */
