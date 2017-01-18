 /*
 | RTPWFILE.H - Runtime Platform Services
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

#ifndef __RTPWFILE_H__
#define __RTPWFILE_H__

#include "rtp.h"
#include "rtpdate.h"

#ifndef __RTPCOMMONFILE_H__
#define __RTPCOMMONFILE_H__

typedef RTP_HANDLE RTP_FILE;

/************************************************************************
 * File System API abstraction layer constants                          *
 ************************************************************************/

/* for the flag argument to rtp_file_open */
#define RTP_FILE_O_APPEND        0x0001  /* All writes will be appended to the file */
#define RTP_FILE_O_RDONLY        0x0002  /* Open a file for reading only */
#define RTP_FILE_O_WRONLY        0x0004  /* Open a file for writing only */
#define RTP_FILE_O_RDWR          0x0008  /* Open a file for reading and writing */
#define RTP_FILE_O_CREAT         0x0010  /* Create a file if it does not exist */
#define RTP_FILE_O_TRUNC         0x0020  /* Truncate a file to 0 bytes after opening */
#define RTP_FILE_O_EXCL          0x0040  /* If creating a file, fail if it already exists */
#define RTP_FILE_O_BINARY        0x0080  /* Open a file in binary mode */
#define RTP_FILE_O_TEXT          0x0100  /* Open a file in text mode */

/* for the mode argument to rtp_file_open */
#define RTP_FILE_S_IWRITE        0x0001  /* Create a file with write permissions */
#define RTP_FILE_S_IREAD         0x0002  /* Create a file with read permissions */
#define RTP_FILE_S_HIDDEN        0x0020  /* Set hidden file attribute on created file */ 
#define RTP_FILE_S_SYSTEM	 0x0040  /* Set system file attribute on created file */ 
#define RTP_FILE_S_ARCHIVE       0x0080  /* Set archive file attribute on created file */ 

/* for attribute member of RTPFSTAT struct */
#define RTP_FILE_ATTRIB_ISDIR    0x01
#define RTP_FILE_ATTRIB_ISVOL    0x02
#define RTP_FILE_ATTRIB_RDONLY   0x04
#define RTP_FILE_ATTRIB_WRONLY   0x08
#define RTP_FILE_ATTRIB_RDWR     0x10
#define RTP_FILE_ATTRIB_HIDDEN   0x20
#define RTP_FILE_ATTRIB_SYSTEM   0x40
#define RTP_FILE_ATTRIB_ARCHIVE  0x80

#endif /* __RTPCOMMONFILE_H__ */

/************************************************************************
 * API functions                                                        *
 ************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

///* Wide-character string (UNICODE) versions of functions above */
int  rtp_wfile_open      (RTP_FILE  * fdPtr, const unsigned short * name, unsigned short flag, unsigned short mode);
int  rtp_wfile_rename    (unsigned short * name, unsigned short * newname);
int  rtp_wfile_delete    (unsigned short * name);
int  rtp_wfile_mkdir     (unsigned short * name);
int  rtp_wfile_rmdir     (unsigned short * name);
int  rtp_wfile_setcwd    (unsigned short * name);
int  rtp_wfile_pwd       (unsigned short * name, long size);
int  rtp_wfile_gfirst    (void ** dirobj, unsigned short * name);
int  rtp_wfile_gnext     (void * dirobj);
int  rtp_wfile_get_name  (void * dirobj, unsigned short * name, int size);
int  rtp_wfile_chmode    (unsigned short * name, unsigned char attributes);
int  rtp_wfile_get_free  (unsigned short * name, unsigned long *total, unsigned long *free,
                          unsigned long *sectors_per_unit, unsigned short *bytes_per_sector);
void rtp_wfile_gdone 	 (void * dirobj);
#ifdef __cplusplus
}
#endif

#endif /*__RTPWFILE_H__*/

/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
