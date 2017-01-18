 /*
 | RTPWCS.H - Runtime Platform Services
 |
 |   UNIVERSAL CODE - DO NOT CHANGE
 |
 | EBSnet - RT-Platform 
 |
 |  $Author: shane $
 |  $Date: 2004/10/05 18:51:30 $
 |  $Name:  $ 
 |  $Revision: 1.4 $
 |
 | Copyright EBSnet Inc. , 2003
 | All rights reserved.
 | This code may not be redistributed in source or linkable object form
 | without the consent of its author.
 |
 | Module description:
 |  [tbd]
*/

#ifndef __RTPWCS_H__
#define __RTPWCS_H__

/*#define rtp_wcsicmp  */
/*#define rtp_wcsistr  */
/*#define rtp_wcsncmp  */
/*#define rtp_wcsnicmp */
/*#define rtp_wcscat   */
/*#define rtp_wcsncat  */
/*#define rtp_wcschr   */
/*#define rtp_wcscmp   */
/*#define rtp_wcscpy   */
/*#define rtp_wcslen   */
/*#define rtp_wcsncpy  */
/*#define rtp_wcsrchr  */
/*#define rtp_wcsstr   */
/*#define rtp_wcsspn   */
/*#define rtp_wcscspn  */

/************************************************************************
* Configuration Define                                                  *
*************************************************************************/

/************************************************************************
 * ANSI C Standard library unicode utility functions include            *
 *      iswcntrl    iswdigit    iswprint    iswspace    iswupper        *
 *      wcscat      wcschr      wcscmp      wcscpy      wcslen          *
 *      wcsrchr     wcsstr      towlower    towupper                    *
 *                                                                      *
 ************************************************************************/


/************************************************************************/
/*      THERE IS NO NEED TO CHANGE ANYTHING BELOW THIS COMMENT         */
/************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
 * Non-ANSI C Standard library utility function declarations            *
 ************************************************************************/

#ifndef rtp_wcsicmp
int                  rtp_wcsicmp  (const unsigned short *, const unsigned short *);
#endif
#ifndef rtp_wcsistr
unsigned short *     rtp_wcsistr  (const unsigned short *, const unsigned short *);
#endif
#ifndef rtp_wcsncmp
int                  rtp_wcsncmp  (const unsigned short *, const unsigned short *, unsigned int);
#endif
#ifndef rtp_wcsnicmp
int                  rtp_wcsnicmp (const unsigned short *, const unsigned short *, unsigned int);
#endif

/************************************************************************
 * ANSI C Standard library utility function declarations (if needed)    *
 ************************************************************************/
                     
#ifndef rtp_wcscat
unsigned short *     rtp_wcscat   (unsigned short * a, const unsigned short * b);
#endif
#ifndef rtp_wcsncat
unsigned short *     rtp_wcsncat  (unsigned short * a, const unsigned short * b, int n);
#endif
#ifndef rtp_wcschr
unsigned short *     rtp_wcschr   (const unsigned short * str, int find_chr);
#endif
#ifndef rtp_wcscmp
int                  rtp_wcscmp   (const unsigned short * s1, const unsigned short * s2);
#endif
#ifndef rtp_wcscpy
unsigned short *     rtp_wcscpy   (unsigned short * a, const unsigned short * b);
#endif
#ifndef rtp_wcslen
unsigned int         rtp_wcslen   (const unsigned short * string);
#endif
#ifndef rtp_wcsncpy
unsigned short *     rtp_wcsncpy  (unsigned short * a, const unsigned short * b, unsigned int n);
#endif
#ifndef rtp_wcsrchr
unsigned short *     rtp_wcsrchr  (const unsigned short * str, int find_chr);
#endif
#ifndef rtp_wcsstr
unsigned short *     rtp_wcsstr   (const unsigned short * str, const unsigned short * find_str);
#endif
#ifndef rtp_wcsspn
unsigned long        rtp_wcsspn   (const unsigned short *str, const unsigned short *chrs);
#endif
#ifndef rtp_wcscspn
unsigned long        rtp_wcscspn  (const unsigned short *str, const unsigned short *chrs);
#endif

#ifdef __cplusplus
}
#endif

#endif /*__RTPWUTIL_H__*/

/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
