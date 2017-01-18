/*
|  RTPRAND.C -
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

/*****************************************************************************/
/* Data
 *****************************************************************************/
static unsigned int guiPNGSeed = 1;             /* initial P.N.G. seed */

/*****************************************************************************/
/* Function Definitions
 *****************************************************************************/

#ifndef rtp_rand
/*----------------------------------------------------------------------*
                             rtp_rand
 *----------------------------------------------------------------------*/
/** @memo   Genearate pseudorandom numbers.

    @doc    Generate pseudorandom numbers and return 
    the generated value as an integer from this 
    linear congruential algorithm using the 
    initial seed or the seed provided by a call
    to rtp_srand.<br><br>
    
    Warning: This random number generator repeats 
    every 2^32 iteration.
             
    @return Random value in the range of 0 to RTP_RAND_MAX.
 */
int rtp_rand (void)
{
#define RTP_RAND_MAX                0x7fff
#define RTP_RAND_POSIX_MULTIPLIER   1103515245L
#define RTP_RAND_POSIX_ADDITIVE     12345L

    guiPNGSeed = guiPNGSeed * RTP_RAND_POSIX_MULTIPLIER + RTP_RAND_POSIX_ADDITIVE;
    return ((unsigned int)(guiPNGSeed / 65536) % (RTP_RAND_MAX + 1));
}
#endif

#ifndef rtp_srand
/*----------------------------------------------------------------------*
                             rtp_srand
 *----------------------------------------------------------------------*/
/** @memo   Seed the pseudorandom number generator.

    @doc    Seed the pseudorandom number generator.
             
    @return void
 */
void rtp_srand (
  unsigned int seed                     /** The value to start the generator. */
  )
{
    guiPNGSeed = seed;
} 

 
#endif
