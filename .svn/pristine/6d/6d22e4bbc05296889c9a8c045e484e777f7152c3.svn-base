/*************************************************************************
 *
 *  FUNCTION:  Weight_Ai
 *
 *  PURPOSE: Spectral expansion of LP coefficients.  (order==10)
 *
 *  DESCRIPTION:
 *      a_exp[i] = a[i] * fac[i-1]    ,i=1,10
 *
 *************************************************************************/

#include "typedef.h"
#include "basic_op.h"
#include "count.h"

/* m = LPC order == 10 */
#define m 10

void Weight_Ai (
    Word16 a[],         /* (i)     : a[m+1]  LPC coefficients   (m=10)    */
    const Word16 fac[],       /* (i)     : Spectral expansion factors.          */
    Word16 a_exp[]      /* (o)     : Spectral expanded LPC coefficients   */
)
{
    Word16 i;

    a_exp[0] = a[0];                                    move16 (); 
    for (i = 1; i <= m; i++)
    {
        a_exp[i] = round_z (L_mult (a[i], fac[i - 1]));   move16 (); 
    }

    return;
}
