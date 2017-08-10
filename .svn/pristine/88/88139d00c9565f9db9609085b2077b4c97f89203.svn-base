/*************************************************************************
 *
 *  FUNCTION:  Lag_window()
 *
 *  PURPOSE:  Lag windowing of autocorrelations.
 *
 *  DESCRIPTION:
 *         r[i] = r[i]*lag_wind[i],   i=1,...,10
 *
 *     r[i] and lag_wind[i] are in special double precision format.
 *     See "oper_32b.c" for the format.
 *
 *************************************************************************/

#include "../common/typedef.h"
#include "../common/basic_op.h"
#include "../common/oper_32b.h"

#include "lag_wind.tab"

void Lag_window (
    Word16 m,           /* (i)     : LPC order                        */
    Word16 r_h[],       /* (i/o)   : Autocorrelations  (msb)          */
    Word16 r_l[]        /* (i/o)   : Autocorrelations  (lsb)          */
)
{

    Word16 i;
    Word32 x;

    for (i = 1; i <= m; i++)
    {
        x = Mpy_32 (r_h[i], r_l[i], lag_h[i - 1], lag_l[i - 1]);
        L_Extract (x, &r_h[i], &r_l[i]);
    }
    return;
}
