/*
|  RTPRAND_SSL.C -
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

/*****************************************************************************/
/* Header files
 *****************************************************************************/

#include "rtprand.h"

/*****************************************************************************/
/* Macros
 *****************************************************************************/

/*****************************************************************************/
/* Types
 *****************************************************************************/

/*****************************************************************************/
/* Function Prototypes 
 *****************************************************************************/
extern "C" {
extern void RAND_seed(const void *buf, int num);
}

#ifndef rtp_srand

/*----------------------------------------------------------------------*
                             ssl_rand_seed
 *----------------------------------------------------------------------*/
/** @memo   Seed the pseudorandom number generator for SSL

    @doc    Seed the pseudorandom number generator.
             
    @return void
 */
void ssl_rand_seed(
  const void *seed, int length
  )
{
    RAND_seed(seed, length);
}
 
#endif
