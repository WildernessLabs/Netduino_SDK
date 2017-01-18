#ifndef __RTPFIO_H__
#define __RTPFIO_H__

#include "rtp.h"


/************************************************************************
 * API functions                                                        *
 ************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

int     rtp_stdio_fopen     (void ** rtpfile, const char * fname, const char * mode);
int     rtp_stdio_fclose    (void * rtpfile);
long    rtp_stdio_fread     (void * buffer, unsigned long len, unsigned long n, void * rtpfile);
long    rtp_stdio_fprintf   (void * rtpfile, const char * f, ...);
long    rtp_stdio_fwrite    (void * buffer, unsigned long len, unsigned long n, void * rtpfile);
int     rtp_stdio_fgets     (char * buf, int buflen, void * rtpfile);
long    rtp_stdio_fseek     (void * rtpfile, long offset, int origin);
int     rtp_stdio_fileno    (RTP_HANDLE * handle, void * rtpfile);
int     rtp_stdio_feof      (void * rtpfile);

#ifdef __cplusplus
}
#endif

#endif /*__RTPFIO_H__*/

/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
