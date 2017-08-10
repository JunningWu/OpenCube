/*************************************************************************
 *
 *  FILE NAME:   pstfilt2.c
 *
 * Performs adaptive postfiltering on the synthesis speech
 *
 *  FUNCTIONS INCLUDED:  Init_Post_Filter()  and Post_Filter()
 *
 *************************************************************************/

#include "typedef.h"
#include "basic_op.h"
#include "sig_proc.h"
#include "count.h"
#include "codec.h"
#include "cnst.h"

/*---------------------------------------------------------------*
 *    Postfilter constant parameters (defined in "cnst.h")       *
 *---------------------------------------------------------------*
 *   L_FRAME     : Frame size.                                   *
 *   L_SUBFR     : Sub-frame size.                               *
 *   M           : LPC order.                                    *
 *   MP1         : LPC order+1                                   *
 *   MU          : Factor for tilt compensation filter           *
 *   AGC_FAC     : Factor for automatic gain control             *
 *---------------------------------------------------------------*/

#define L_H 22  /* size of truncated impulse response of A(z/g1)/A(z/g2) */

/*------------------------------------------------------------*
 *   static vectors                                           *
 *------------------------------------------------------------*/

 /* inverse filtered synthesis */

static Word16 res2[L_SUBFR];

 /* memory of filter 1/A(z/0.75) */

static Word16 mem_syn_pst[M];

 /* Spectral expansion factors */

const Word16 F_gamma3[M] =
{
    22938, 16057, 11240, 7868, 5508,
    3856, 2699, 1889, 1322, 925
};
const Word16 F_gamma4[M] =
{
    24576, 18432, 13824, 10368, 7776,
    5832, 4374, 3281, 2461, 1846
};

/*************************************************************************
 *
 *  FUNCTION:   Init_Post_Filter
 *
 *  PURPOSE: Initializes the postfilter parameters.
 *
 *************************************************************************/

void Init_Post_Filter (void)
{
    Set_zero (mem_syn_pst, M);

    Set_zero (res2, L_SUBFR);

    return;
}

/*************************************************************************
 *  FUNCTION:  Post_Filter()
 *
 *  PURPOSE:  postfiltering of synthesis speech.
 *
 *  DESCRIPTION:
 *      The postfiltering process is described as follows:
 *
 *          - inverse filtering of syn[] through A(z/0.7) to get res2[]
 *          - tilt compensation filtering; 1 - MU*k*z^-1
 *          - synthesis filtering through 1/A(z/0.75)
 *          - adaptive gain control
 *
 *************************************************************************/

void Post_Filter (
    Word16 *syn,    /* in/out: synthesis speech (postfiltered is output)    */
    Word16 *Az_4    /* input: interpolated LPC parameters in all subframes  */
)
{
    /*-------------------------------------------------------------------*
     *           Declaration of parameters                               *
     *-------------------------------------------------------------------*/

    Word16 syn_pst[L_FRAME];    /* post filtered synthesis speech   */
    Word16 Ap3[MP1], Ap4[MP1];  /* bandwidth expanded LP parameters */
    Word16 *Az;                 /* pointer to Az_4:                 */
                                /*  LPC parameters in each subframe */
    Word16 i_subfr;             /* index for beginning of subframe  */
    Word16 h[L_H];

    Word16 i;
    Word16 temp1, temp2;
    Word32 L_tmp;

    /*-----------------------------------------------------*
     * Post filtering                                      *
     *-----------------------------------------------------*/

    Az = Az_4;

    for (i_subfr = 0; i_subfr < L_FRAME; i_subfr += L_SUBFR)
    {
        /* Find weighted filter coefficients Ap3[] and ap[4] */

        Weight_Ai (Az, F_gamma3, Ap3);
        Weight_Ai (Az, F_gamma4, Ap4);

        /* filtering of synthesis speech by A(z/0.7) to find res2[] */

        Residu (Ap3, &syn[i_subfr], res2, L_SUBFR);

        /* tilt compensation filter */

        /* impulse response of A(z/0.7)/A(z/0.75) */

        Copy (Ap3, h, M + 1);
        Set_zero (&h[M + 1], L_H - M - 1);
        Syn_filt (Ap4, h, h, L_H, &h[M + 1], 0);

        /* 1st correlation of h[] */

        L_tmp = L_mult (h[0], h[0]);
        for (i = 1; i < L_H; i++)
        {
            L_tmp = L_mac (L_tmp, h[i], h[i]);
        }
        temp1 = extract_h (L_tmp);

        L_tmp = L_mult (h[0], h[1]);
        for (i = 1; i < L_H - 1; i++)
        {
            L_tmp = L_mac (L_tmp, h[i], h[i + 1]);
        }
        temp2 = extract_h (L_tmp);

        test (); 
        if (temp2 <= 0)
        {
            temp2 = 0;          move16 (); 
        }
        else
        {
            temp2 = mult (temp2, MU);
            temp2 = div_s (temp2, temp1);
        }

        preemphasis (res2, temp2, L_SUBFR);

        /* filtering through  1/A(z/0.75) */

        Syn_filt (Ap4, res2, &syn_pst[i_subfr], L_SUBFR, mem_syn_pst, 1);

        /* scale output to input */

        agc (&syn[i_subfr], &syn_pst[i_subfr], AGC_FAC, L_SUBFR);

        Az += MP1;
    }

    /* update syn[] buffer */

    Copy (&syn[L_FRAME - M], &syn[-M], M);

    /* overwrite synthesis speech by postfiltered synthesis speech */

    Copy (syn_pst, syn, L_FRAME);

    return;
}
