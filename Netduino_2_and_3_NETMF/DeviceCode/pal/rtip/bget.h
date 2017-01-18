/*

    Interface definitions for bget.c, the memory management package.

*/

#ifndef __BGET_H__
#define __BGET_H__

#include "rtp.h"
#include "rtptypes.h"

/* type 'bufsize' must be a SIGNED type   */
typedef int bufsize;

#ifdef __cplusplus
extern "C" {
#endif
void        bget_init(void);
void    	bpool   (RTP_PFVOID buffer, bufsize len);
RTP_PFVOID  bget    (bufsize size);
RTP_PFVOID  bgetz   (bufsize size);
RTP_PFVOID  bgetr   (RTP_PFVOID buffer, bufsize newsize);
void    	brel    (RTP_PFVOID buf);
void    	bectl   (int (*compact)(bufsize sizereq, int sequence),
                	RTP_PFVOID (*acquire)(bufsize size),
                	void (*release)(RTP_PFVOID buf), bufsize pool_incr);
void    bstats  	(bufsize *curalloc, bufsize *totfree, bufsize *maxfree,
                	long *nget, long *nrel);
void    bstatse 	(bufsize *pool_incr, long *npool, long *npget,
                	long *nprel, long *ndget, long *ndrel);
void    bufdump 	(RTP_PFVOID buf);
void    bpoold  	(RTP_PFVOID pool, int dumpalloc, int dumpfree);
int     bpoolv  	(RTP_PFVOID pool);

#ifdef __cplusplus
}
#endif

#endif

