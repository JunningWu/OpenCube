/*************************************************************************
 *
 *  FUNCTION:   dec_10i40_35bits()
 *
 *  PURPOSE:  Builds the innovative codevector from the received
 *            index of algebraic codebook.
 *
 *   See  c1035pf.c  for more details about the algebraic codebook structure.
 *
 *************************************************************************/

#include "typedef.h"
#include "basic_op.h"
#include "count.h"

#define L_CODE    40            /* codevector length */
#define NB_PULSE  10            /* number of pulses  */
#define NB_TRACK  5             /* number of track */

void dec_10i40_35bits (
    Word16 index[],    /* (i)     : index of 10 pulses (sign+position)       */
    Word16 cod[]       /* (o)     : algebraic (fixed) codebook excitation    */
)
{
    static const Word16 dgray[8] = {0, 1, 3, 2, 5, 6, 4, 7};
    Word16 i, j, pos1, pos2, sign, tmp;

    for (i = 0; i < L_CODE; i++)
    {
        cod[i] = 0;                                     move16 (); 
    }

    /* decode the positions and signs of pulses and build the codeword */

    for (j = 0; j < NB_TRACK; j++)
    {
        /* compute index i */

        tmp = index[j];                                 move16 ();
        i = tmp & 7;                                    logic16 (); 
        i = dgray[i];                                   move16 (); 

        i = extract_l (L_shr (L_mult (i, 5), 1));
        pos1 = add (i, j); /* position of pulse "j" */

        i = shr (tmp, 3) & 1;                           logic16 (); 
        test (); 
        if (i == 0)
        {
            sign = 4096;                                move16 (); /* +1.0 */
        }
        else
        {
            sign = -4096;                               move16 (); /* -1.0 */
        }

        cod[pos1] = sign;                               move16 (); 

        /* compute index i */

        i = index[add (j, 5)] & 7;                      logic16 (); 
        i = dgray[i];                                   move16 (); 
        i = extract_l (L_shr (L_mult (i, 5), 1));

        pos2 = add (i, j);      /* position of pulse "j+5" */

        test (); 
        if (sub (pos2, pos1) < 0)
        {
            sign = negate (sign);
        }
        cod[pos2] = add (cod[pos2], sign);              move16 (); 
    }

    return;
}
