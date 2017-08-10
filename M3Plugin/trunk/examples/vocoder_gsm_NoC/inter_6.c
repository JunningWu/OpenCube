/*************************************************************************
 *
 *  FUNCTION:  Interpol_6()
 *
 *  PURPOSE:  Interpolating the normalized correlation with 1/6 resolution.
 *
 *************************************************************************/

#include "typedef.h"
#include "basic_op.h"
#include "count.h"

#define UP_SAMP      6
#define L_INTERPOL   4
#define FIR_SIZE     (UP_SAMP*L_INTERPOL+1)

/* 1/6 resolution interpolation filter  (-3 dB at 3600 Hz) */

static const Word16 inter_6[FIR_SIZE] =
{
    29519,
    28316, 24906, 19838, 13896, 7945, 2755,
    -1127, -3459, -4304, -3969, -2899, -1561,
    -336, 534, 970, 1023, 823, 516,
    220, 0, -131, -194, -215, 0
};

Word16 Interpol_6 (  /* (o)  : interpolated value  */
    Word16 *x,       /* (i)  : input vector        */
    Word16 frac      /* (i)  : fraction            */
)
{
    Word16 i, k;
    Word16 *x1, *x2;
    const Word16 *c1, *c2;
    Word32 s;

    test (); 
    if (frac < 0)
    {
        frac = add (frac, UP_SAMP);
        x--;
    }
    x1 = &x[0];                         move16 (); 
    x2 = &x[1];                         move16 (); 
    c1 = &inter_6[frac];                move16 (); 
    c2 = &inter_6[sub (UP_SAMP, frac)]; move16 (); 

    s = 0;                              move32 (); 
    for (i = 0, k = 0; i < L_INTERPOL; i++, k += UP_SAMP)
    {
        s = L_mac (s, x1[-i], c1[k]);
        s = L_mac (s, x2[i], c2[k]);
    }

    return round_z (s);
}
