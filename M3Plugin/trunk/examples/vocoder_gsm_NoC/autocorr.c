/*************************************************************************
 *
 *  FUNCTION:  autocorr
 *
 *  PURPOSE:   Compute autocorrelations of signal with windowing
 *
 *  DESCRIPTION:
 *       - Windowing of input speech:   s'[n] = s[n] * w[n]
 *       - Autocorrelations of input speech:
 *             r[k] = sum_{i=k}^{239} s'[i]*s'[i-k]    k=0,...,10
 *         The autocorrelations are expressed in normalized double precision
 *         format.
 *
 *************************************************************************/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "count.h"
#include "cnst.h"

Word16 Autocorr (
    Word16 x[],         /* (i)    : Input signal                    */
    Word16 m,           /* (i)    : LPC order                       */
    Word16 r_h[],       /* (o)    : Autocorrelations  (msb)         */
    Word16 r_l[],       /* (o)    : Autocorrelations  (lsb)         */
    const Word16 wind[]       /* (i)    : window for LPC analysis         */
)
{
    Word16 i, j, norm;
    Word16 y[L_WINDOW];
    Word32 sum;
    Word16 overfl, overfl_shft;

    /* Windowing of signal */

    for (i = 0; i < L_WINDOW; i++)
    {
        y[i] = mult_r (x[i], wind[i]); move16 (); 
    }

    /* Compute r[0] and test for overflow */

    overfl_shft = 0;                   move16 (); 

    do
    {
        overfl = 0;                    move16 (); 
        sum = 0L;                      move32 ();

        for (i = 0; i < L_WINDOW; i++)
        {
            sum = L_mac (sum, y[i], y[i]);
        }

        /* If overflow divide y[] by 4 */

        test (); 
        if (L_sub (sum, MAX_32) == 0L)
        {
            overfl_shft = add (overfl_shft, 4);
            overfl = 1;                move16 (); /* Set the overflow flag */

            for (i = 0; i < L_WINDOW; i++)
            {
                y[i] = shr (y[i], 2);  move16 (); 
            }
        }
        test (); 
    }
    while (overfl != 0);

    sum = L_add (sum, 1L);             /* Avoid the case of all zeros */

    /* Normalization of r[0] */

    norm = norm_l (sum);
    sum = L_shl (sum, norm);
    L_Extract (sum, &r_h[0], &r_l[0]); /* Put in DPF format (see oper_32b) */

    /* r[1] to r[m] */

    for (i = 1; i <= m; i++)
    {
        sum = 0;                       move32 (); 

        for (j = 0; j < L_WINDOW - i; j++)
        {
            sum = L_mac (sum, y[j], y[j + i]);
        }

        sum = L_shl (sum, norm);
        L_Extract (sum, &r_h[i], &r_l[i]);
    }

    norm = sub (norm, overfl_shft);

    return norm;
}
