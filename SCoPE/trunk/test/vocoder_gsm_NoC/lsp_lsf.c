/*************************************************************************
 *
 *   FUNCTIONS:  Lsp_lsf and Lsf_lsp
 *
 *   PURPOSE:
 *      Lsp_lsf:   Transformation lsp to lsf
 *      Lsf_lsp:   Transformation lsf to lsp
 *
 *   DESCRIPTION:
 *         lsp[i] = cos(2*pi*lsf[i]) and lsf[i] = arccos(lsp[i])/(2*pi)
 *
 *   The transformation from lsp[i] to lsf[i] and lsf[i] to lsp[i] are
 *   approximated by a look-up table and interpolation.
 *
 *************************************************************************/

#include "typedef.h"
#include "basic_op.h"
#include "count.h"

#include "lsp_lsf.tab"          /* Look-up table for transformations */

void Lsf_lsp (
    Word16 lsf[],       /* (i) : lsf[m] normalized (range: 0.0<=val<=0.5) */
    Word16 lsp[],       /* (o) : lsp[m] (range: -1<=val<1)                */
    Word16 m            /* (i) : LPC order                                */
)
{
    Word16 i, ind, offset;
    Word32 L_tmp;

    for (i = 0; i < m; i++)
    {
        ind = shr (lsf[i], 8);      /* ind    = b8-b15 of lsf[i] */
        offset = lsf[i] & 0x00ff;   logic16 (); /* offset = b0-b7  of lsf[i] */

        /* lsp[i] = table[ind]+ ((table[ind+1]-table[ind])*offset) / 256 */

        L_tmp = L_mult (sub (table[ind + 1], table[ind]), offset);
        lsp[i] = add (table[ind], extract_l (L_shr (L_tmp, 9)));
                                    move16 (); 
    }
    return;
}

void Lsp_lsf (
    Word16 lsp[],       /* (i)  : lsp[m] (range: -1<=val<1)                */
    Word16 lsf[],       /* (o)  : lsf[m] normalized (range: 0.0<=val<=0.5) */
    Word16 m            /* (i)  : LPC order                                */
)
{
    Word16 i, ind;
    Word32 L_tmp;

    ind = 63;                       /* begin at end of table -1 */

    for (i = m - 1; i >= 0; i--)
    {
        /* find value in table that is just greater than lsp[i] */
        test (); 
        while (sub (table[ind], lsp[i]) < 0)
        {
            ind--;
            test (); 
        }

        /* acos(lsp[i])= ind*256 + ( ( lsp[i]-table[ind] ) *
           slope[ind] )/4096 */

        L_tmp = L_mult (sub (lsp[i], table[ind]), slope[ind]);
        /*(lsp[i]-table[ind])*slope[ind])>>12*/
        lsf[i] = round_z (L_shl (L_tmp, 3));      move16 (); 
        lsf[i] = add (lsf[i], shl (ind, 8));    move16 (); 
    }
    return;
}
