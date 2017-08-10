/*************************************************************************
 *
 *  FUNCTION:   Convolve
 *
 *  PURPOSE:
 *     Perform the convolution between two vectors x[] and h[] and
 *     write the result in the vector y[]. All vectors are of length L
 *     and only the first L samples of the convolution are computed.
 *
 *  DESCRIPTION:
 *     The convolution is given by
 *
 *          y[n] = sum_{i=0}^{n} x[i] h[n-i],        n=0,...,L-1
 *
 *************************************************************************/

#include "typedef.h"
#include "basic_op.h"
#include "count.h"

void Convolve (
    Word16 x[],        /* (i)     : input vector                           */
    Word16 h[],        /* (i)     : impulse response                       */
    Word16 y[],        /* (o)     : output vector                          */
    Word16 L           /* (i)     : vector size                            */
)
{
    Word16 i, n;
    Word32 s;

    for (n = 0; n < L; n++)
    {
        s = 0;                  move32 (); 
        for (i = 0; i <= n; i++)
        {
            s = L_mac (s, x[i], h[n - i]);
        }
        s = L_shl (s, 3);
        y[n] = extract_h (s);   move16 (); 
    }

    return;
}
