/*************************************************************************
 *
 *  FUNCTION:  Int_lpc()
 *
 *  PURPOSE:  Interpolates the LSPs and converts to LPC parameters to get
 *            a different LP filter in each subframe.
 *
 *  DESCRIPTION:
 *     The 20 ms speech frame is divided into 4 subframes.
 *     The LSPs are quantized and transmitted at the 2nd and 4th subframes
 *     (twice per frame) and interpolated at the 1st and 3rd subframe.
 *
 *          |------|------|------|------|
 *             sf1    sf2    sf3    sf4
 *       F0            Fm            F1
 *
 *     sf1:   1/2 Fm + 1/2 F0         sf3:   1/2 F1 + 1/2 Fm
 *     sf2:       Fm                  sf4:       F1
 *
 *************************************************************************/
#include "../common/typedef.h"
#include "../common/basic_op.h"
#include "../common/sig_proc.h"

#define M   10                  /* LP order */
#define MP1 11                  /* M+1 */

void Int_lpc (
    Word16 lsp_old[],   /* input : LSP vector at the 4th subframe
                           of past frame    */
    Word16 lsp_mid[],   /* input : LSP vector at the 2nd subframe
                           of present frame */
    Word16 lsp_new[],   /* input : LSP vector at the 4th subframe of
                           present frame */
    Word16 Az[]         /* output: interpolated LP parameters in
                           all subframes */
)
{

    Word16 i;
    Word16 lsp[M];

    /*  lsp[i] = lsp_mid[i] * 0.5 + lsp_old[i] * 0.5 */

    for (i = 0; i < M; i++)
    {
        lsp[i] = add (shr (lsp_mid[i], 1), shr (lsp_old[i], 1));
                                 
    }

    Lsp_Az (lsp, Az);           /* Subframe 1 */
    Az += MP1;                   

    Lsp_Az (lsp_mid, Az);       /* Subframe 2 */
    Az += MP1;                   

    for (i = 0; i < M; i++)
    {
        lsp[i] = add (shr (lsp_mid[i], 1), shr (lsp_new[i], 1));
                                 
    }

    Lsp_Az (lsp, Az);           /* Subframe 3 */
    Az += MP1;                   

    Lsp_Az (lsp_new, Az);       /* Subframe 4 */

    return;
}

/*----------------------------------------------------------------------*
 * Function Int_lpc2()                                                  *
 * ~~~~~~~~~~~~~~~~~~                                                   *
 * Interpolation of the LPC parameters.                                 *
 * Same as the previous function but we do not recompute Az() for       *
 * subframe 2 and 4 because it is already available.                    *
 *----------------------------------------------------------------------*/

void Int_lpc2 (
             Word16 lsp_old[],  /* input : LSP vector at the 4th subframe
                                 of past frame    */
             Word16 lsp_mid[],  /* input : LSP vector at the 2nd subframe
                                 of present frame */
             Word16 lsp_new[],  /* input : LSP vector at the 4th subframe of
                                 present frame */
             Word16 Az[]        /* output: interpolated LP parameters
                                 in subframes 1 and 3 */
)
{

    Word16 i;
    Word16 lsp[M];

    /*  lsp[i] = lsp_mid[i] * 0.5 + lsp_old[i] * 0.5 */

    for (i = 0; i < M; i++)
    {
        lsp[i] = add (shr (lsp_mid[i], 1), shr (lsp_old[i], 1));
                                 
    }
    Lsp_Az (lsp, Az);           /* Subframe 1 */
    Az += MP1 * 2;               

    for (i = 0; i < M; i++)
    {
        lsp[i] = add (shr (lsp_mid[i], 1), shr (lsp_new[i], 1));
                                 
    }
    Lsp_Az (lsp, Az);           /* Subframe 3 */

    return;
}
