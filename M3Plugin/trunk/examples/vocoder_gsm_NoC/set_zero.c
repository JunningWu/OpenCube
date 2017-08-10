/*************************************************************************
 *
 *  FUNCTION:   Set zero()
 *
 *  PURPOSE:  Set vector x[] to zero
 *
 *************************************************************************/

#include "typedef.h"
#include "basic_op.h"
#include "count.h"

void Set_zero (
    Word16 x[],         /* (o)    : vector to clear     */
    Word16 L            /* (i)    : length of vector    */
)
{
    Word16 i;

    for (i = 0; i < L; i++)
    {
        x[i] = 0;               move16 (); 
    }

    return;
}
