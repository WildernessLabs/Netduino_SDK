/*
|  RTPTOTC.H - Translate old tc_... functions to RTPlatform functions
|
|  EBSnet -
|
|   $Author: shane $
|   $Date: 2004/11/09 22:03:27 $
|   $Name:  $
|   $Revision: 1.2 $
|
|  Copyright EBSnet Inc. , 2004
|  All rights reserved.
|  This code may not be redistributed in source or linkable object form
|  without the consent of its author.
*/

#ifndef __RTPTOTC_H__
#define __RTPTOTC_H__

#include "rtptypes.h"

#define tc_hatoi(A)            rtp_hatoi(A)
#define tc_itoa(A,B,C)         rtp_itoa(A,B,C)
#define tc_ltoa(A,B,C)         rtp_ltoa(A,B,C)
#define tc_ultoa(A,B,C)        rtp_ultoa(A,B,C)
#define tc_lrotl(A,B)          rtp_lrotl(A,B)
#define tc_lrotr(A,B)          rtp_lrotr(A,B)
#define tc_stricmp(A,B)        rtp_stricmp(A,B)
#define tc_stristr(A,B)        rtp_stristr(A,B)
#define tc_strncmp(A,B,C)      rtp_strncmp(A,B,C)
#define tc_strnicmp(A,B,C)     rtp_strnicmp(A,B,C)
#define tc_memcat(A,B,C,D)     rtp_memcat(A,B,C,D)
#define tc_atoi(A)             rtp_atoi(A)
#define tc_atol(A)             rtp_atol(A)
#define tc_isalnum(A)          rtp_isalnum(A)
#define tc_iscntrl(A)          rtp_iscntrl(A)
#define tc_isdigit(A)          rtp_isdigit(A)
#define tc_isprint(A)          rtp_isprint(A)
#define tc_isspace(A)          rtp_isspace(A)
#define tc_isupper(A)          rtp_isupper(A)
#define tc_isxdigit(A)         rtp_isxdigit(A)
#define tc_memchr(A,B,C)       rtp_memchr(A,B,C)
#define tc_memcmp(A,B,C)       rtp_memcmp(A,B,C)
#define tc_memcpy(A,B,C)       rtp_memcpy(A,B,C)
#ifndef tc_mv4
#define tc_mv4(A,B,C)          rtp_memcpy(A,B,C)
#endif
#define tc_movebytes(A,B,C)    rtp_memcpy(A,B,C)
#define tc_memmove(A,B,C)      rtp_memmove(A,B,C)
#define tc_memset(A,B,C)       rtp_memset(A,B,C)
#define tc_rand()              rtp_rand()
#define tc_srand(A)            rtp_srand(A)
#define tc_strcat(A,B)         rtp_strcat(A,B)
#define tc_strncat(A,B,C)      rtp_strncat(A,B,C)
#define tc_strchr(A,B)         rtp_strchr(A,B)
#define tc_strchrs(A,B)        rtp_strpbrk(A,B)
#define tc_strpbrk(A,B)        rtp_strpbrk(A,B)
#define tc_strcmp(A,B)         rtp_strcmp(A,B)
#define tc_strcpy(A,B)         rtp_strcpy(A,B)
#define tc_strlen(A)           rtp_strlen(A)
#define tc_strncpy(A,B,C)      rtp_strncpy(A,B,C)
#define tc_strrchr(A,B)        rtp_strrchr(A,B)
#define tc_strstr(A,B)         rtp_strstr(A,B)
#define tc_strspn(A,B)         rtp_strspn(A,B)
#define tc_strcspn(A,B)        rtp_strcspn(A,B)
#define tc_strtol(A,B,C)       rtp_strtol(A,B,C)
#define tc_strtoul(A,B,C)      rtp_strtoul(A,B,C)
#define tc_tolower(A)          rtp_tolower(A)
#define tc_toupper(A)          rtp_toupper(A)
#define tc_qsort(A,B,C,D)      rtp_qsort(A,B,C,D)
#define tc_bsearch(A,B,C,D,E)  rtp_bsearch(A,B,C,D,E)
#define tc_itow(A,B,C)         rtp_itow(A,B,C)
#define tc_ltow(A,B,C)         rtp_ltow(A,B,C)
#define tc_ultow(A,B,C)        rtp_ultow(A,B,C)
#define tc_wcsicmp(A,B)        rtp_wcsicmp(A,B)
#define tc_wcsistr(A,B)        rtp_wcsistr(A,B)
#define tc_wcsncmp(A,B,C)      rtp_wcsncmp(A,B,C)
#define tc_wcsnicmp(A,B,C)     rtp_wcsnicmp(A,B,C)
#define tc_wtoi(A)             rtp_wtoi(A)
#define tc_wtol(A)             rtp_wtol(A)
#define tc_iswalnum(A)         rtp_iswalnum(A)
#define tc_iswcntrl(A)         rtp_iswcntrl(A)
#define tc_iswdigit(A)         rtp_iswdigit(A)
#define tc_iswprint(A)         rtp_iswprint(A)
#define tc_iswspace(A)         rtp_iswspace(A)
#define tc_iswupper(A)         rtp_iswupper(A)
#define tc_iswxdigit(A)        rtp_iswxdigit(A)
#define tc_wcscat(A,B)         rtp_wcscat(A,B)
#define tc_wcschr(A,B)         rtp_wcschr(A,B)
#define tc_wcscmp(A,B)         rtp_wcscmp(A,B)
#define tc_wcscpy(A,B)         rtp_wcscpy(A,B)
#define tc_wcslen(A)           rtp_wcslen(A)
#define tc_wcsncpy(A,B,C)      rtp_wcsncpy(A,B,C)
#define tc_wcsrchr(A,B)        rtp_wcsrchr(A,B)
#define tc_wcsstr(A,B)         rtp_wcsstr(A,B)
#define tc_wcstol(A,B,C)       rtp_wcstol(A,B,C)
#define tc_wcstoul(A,B,C)      rtp_wcstoul(A,B,C)
#define tc_towlower(A)         rtp_towlower(A)
#define tc_towupper(A)         rtp_towupper(A)
#define tc_sprintf             rtp_sprintf
#define tc_comparen(A,B,N)     (!rtp_memcmp(A,B,N))

#endif /* __RTPTOTC_H__ */
