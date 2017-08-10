/*---------------------------------------------------------------------*
 * routine preemphasis()                                               *
 * ~~~~~~~~~~~~~~~~~~~~~                                               *
 * Preemphasis: filtering through 1 - g z^-1                           *
 *---------------------------------------------------------------------*/

#include "typedef.h"
#include "basic_op.h"
#include "count.h"

Word16 mem_pre;

void preemphasis (
    Word16 *signal, /* (i/o)   : input signal overwritten by the output */
    Word16 g,       /* (i)     : preemphasis coefficient                */
    Word16 L        /* (i)     : size of filtering                      */
)
{
    Word16 *p1, *p2, temp, i;

    p1 = signal + L - 1;                    move16 (); 
    p2 = p1 - 1;                            move16 (); 
    temp = *p1;                             move16 (); 

    for (i = 0; i <= L - 2; i++)
    {
        *p1 = sub (*p1, mult (g, *p2--));   move16 (); 
        p1--;
    }

    *p1 = sub (*p1, mult (g, mem_pre));     move16 (); 

    mem_pre = temp;                         move16 (); 

    return;
}
