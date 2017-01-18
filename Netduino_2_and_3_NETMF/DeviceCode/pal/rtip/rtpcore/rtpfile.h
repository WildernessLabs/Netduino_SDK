 /*
 | RTPFILE.H - Runtime Platform Services
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

#ifndef __RTPFILE_H__
#define __RTPFILE_H__

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
#define RTP_FILE_S_SYSTEM	     0x0040  /* Set system file attribute on created file */ 
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

int  rtp_file_open       (RTP_FILE * fdPtr, const char * name, unsigned short flag, unsigned short mode);
int  rtp_file_close      (RTP_FILE fd);
long rtp_file_read       (RTP_FILE fd, unsigned char * buf, long count);
long rtp_file_write      (RTP_FILE fd, const unsigned char * buf, long count);
long rtp_file_lseek      (RTP_FILE fd, long offset, int origin);
int  rtp_file_truncate   (RTP_FILE fd, long offset);
int  rtp_file_flush      (RTP_FILE fd);
int  rtp_file_rename     (const char * name, char * newname);
int  rtp_file_delete     (const char * name);
int  rtp_file_mkdir      (const char * name);
int  rtp_file_rmdir      (const char * name);
int  rtp_file_setcwd     (const char * name);
int  rtp_file_pwd        (char * name, long size);
int  rtp_file_chmode     (const char * name, unsigned char attributes);
int  rtp_file_get_free   (const char * name, unsigned long * total, unsigned long * free, 
			              unsigned long * sectors_per_unit, unsigned short * bytes_per_sector);
int  rtp_file_set_time   (RTP_FILE fd, RTP_DATE * adate, RTP_DATE * wdate, RTP_DATE * cdate, RTP_DATE * hdate);

#ifdef __cplusplus
}
#endif

#endif /*__RTPFILE_H__*/

/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
