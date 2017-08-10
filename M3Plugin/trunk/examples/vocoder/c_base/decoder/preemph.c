/*---------------------------------------------------------------------*
 * routine preemphasis()                                               *
 * ~~~~~~~~~~~~~~~~~~~~~                                               *
 * Preemphasis: filtering through 1 - g z^-1                           *
 *---------------------------------------------------------------------*/
#include "../common/typedef.h"
#include "../common/basic_op.h"

void preemphasis (
    Word16 *signal, /* (i/o)   : input signal overwritten by the output */
    Word16 *mem_pre,
    Word16 g,       /* (i)     : preemphasis coefficient                */
    Word16 L        /* (i)     : size of filtering                      */
)
{
#ifdef PERF
	uc_prof_control->add_function_time();
#endif

    Word16 *p1, *p2, temp, i;

    p1 = signal + L - 1;
    p2 = p1 - 1;
    temp = *p1;

    for (i = 0; i <= L - 2; i++)
    {
        *p1 = sub (*p1, mult (g, *p2--));
        p1--;
    }

    *p1 = sub (*p1, mult (g, *mem_pre));

    *mem_pre = temp;

    return;
}
