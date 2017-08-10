/*************************************************************************
 *
 *  FUNCTION:  Syn_filt:
 *
 *  PURPOSE:  Perform synthesis filtering through 1/A(z).
 *
 *************************************************************************/

#include "typedef.h"
#include "basic_op.h"


/* M = LPC order == 10 */
#define M 10

void Syn_filt (
    Word16 a[],     /* (i)     : a[M+1] prediction coefficients   (M=10)  */
    Word16 x[],     /* (i)     : input signal                             */
    Word16 y[],     /* (o)     : output signal                            */
    Word16 lg,      /* (i)     : size of filtering                        */
    Word16 mem[],   /* (i/o)   : memory associated with this filtering.   */
    Word16 update   /* (i)     : 0=no update, 1=update of memory.         */
)
{

    Word16 i, j;
    Word32 s;
    Word16 tmp[80];   /* This is usually done by memory allocation (lg+M) */
    Word16 *yy;

    /* Copy mem[] to yy[] */

    yy = tmp;                            

    for (i = 0; i < M; i++)
    {
        *yy++ = mem[i];                  
    } 

    /* Do the filtering. */

    for (i = 0; i < lg; i++)
    {
        s = L_mult (x[i], a[0]);
        for (j = 1; j <= M; j++)
        {
            s = L_msu (s, a[j], yy[-j]);
        }
        s = L_shl (s, 3);
        *yy++ = round (s);               
    }

    for (i = 0; i < lg; i++)
    {
        y[i] = tmp[i + M];               
    }

    /* Update of memory if update==1 */

     
    if (update != 0)
    {
        for (i = 0; i < M; i++)
        {
            mem[i] = y[lg - M + i];      
        }
    }
    return;
}
