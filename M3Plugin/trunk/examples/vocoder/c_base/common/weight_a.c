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

/* M = LPC order == 10 */
#define M 10

void Weight_Ai (
    Word16 a[],         /* (i)     : a[M+1]  LPC coefficients   (M=10)    */
    const Word16 fac[],       /* (i)     : Spectral expansion factors.          */
    Word16 a_exp[]      /* (o)     : Spectral expanded LPC coefficients   */
)
{
    Word16 i;

    a_exp[0] = a[0];
    for (i = 1; i <= M; i++)
    {
        a_exp[i] = round (L_mult (a[i], fac[i - 1]));
    }

    return;
}
