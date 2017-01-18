/*                                                                        */
/* OSLIST.H  - RTIP header file                                           */
/*                                                                        */
/*   EBS - RTIP                                                           */
/*                                                                        */
/*   Copyright EBSNet Inc. , 1993                                 */
/*   All rights reserved.                                                 */
/*   This code may not be redistributed in source or linkable object form */
/*   without the consent of its author.                                   */
/*                                                                        */
/*    Module description:                                                 */
/*        This file contains prototypes constants and structure           */
/*        declarations required to build double linked lists.             */
/*        There should be no need to change values in this file           */
/*        configuration values are in xnconf.h, rtipconf.h, etc.          */
/*                                                                        */

#ifndef __OSLIST__
#define __OSLIST__ 1

/* *********************************************************************   */
/* POS_LIST                                                                */
/* *********************************************************************   */

#define ZERO_OFFSET     0

typedef struct os_list
{
    struct os_list RTP_FAR *pnext;
    struct os_list RTP_FAR *pprev;
} OS_LIST;
typedef struct os_list RTP_FAR * POS_LIST;   /* head of list */

/* ********************************************************************   */
/* macro to access OS_LIST in entry which is at offset instead of         */
/* at the beginning of the structures (the offset are 0 for the first,    */
/* 1 for the second, etc)                                                 */
/* NOTE: offsets are 0, 1, 2 etc                                          */
#define RTIP_POS_ENTRY_OFF(entry, offset) ((POS_LIST)entry + offset)


/* ********************************************************************   */
/* FUNCTION DECLARATIONS                                                  */
/* ********************************************************************   */
POS_LIST    os_list_add_front_off(POS_LIST head, POS_LIST entry, int offset);
POS_LIST    os_list_add_rear_off(POS_LIST head, POS_LIST entry, int offset);
POS_LIST    os_list_add_middle_off(POS_LIST head, POS_LIST entry,
                                   POS_LIST entry_prev, int offset);
POS_LIST    os_list_remove_off(POS_LIST head, POS_LIST entry, int offset);
POS_LIST    os_list_next_entry_off(POS_LIST head, POS_LIST entry, int offset);
POS_LIST    os_list_last_off(POS_LIST head, int offset);
RTP_BOOL 	os_check_list_off(POS_LIST head, POS_LIST entry, int offset);

#endif /* __OSLIST__ */

