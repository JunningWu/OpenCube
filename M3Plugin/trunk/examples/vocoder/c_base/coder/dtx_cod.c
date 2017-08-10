//
// 
//    File Name: dtx_cod.h
// 
//    Purpose:   Contains the CODER implementation for all the functions of DTX.
//               Also contains CODER constants used in DTX functions.
// 
//		TEISA Dpt.
//  University of Cantabria
//  Date: Oct.2003
//  Author: F.Herrera
//  Adaptation of Coder code to SystemC
//
//  dtx for coder
//

#include "../common/typedef.h"
#include "../common/cnst.h"
#include "../common/basic_op.h"
#include "../common/sig_proc.h"

#include "dtx_cod.h"

/*************************************************************************
 *
 *   FUNCTION NAME: aver_lsf_history
 *
 *   PURPOSE: Compute the averaged LSF parameter vector. Computation is
 *            performed by averaging the LSF parameter vectors which exist
 *            in the LSF parameter history, together with the LSF
 *            parameter vectors of the current frame.
 *
 *   INPUTS:      lsf_old[0..DTX_HANGOVER-1][0..M-1]
 *                                   LSF parameter history
 *                lsf1[0..M-1]       LSF vector of the 1st half of the frame
 *                lsf2[0..M-1]       LSF vector of the 2nd half of the frame
 *
 *   OUTPUTS:     lsf_aver[0..M-1]   Averaged LSF parameter vector
 *
 *   RETURN VALUE: none
 *
 *************************************************************************/

void aver_lsf_history (
    Word16 lsf_old[DTX_HANGOVER][M],
    Word16 lsf1[M],
    Word16 lsf2[M],
    Word16 lsf_aver[M]
)
{

    Word16 i, j;
    Word32 L_temp;

    for (j = 0; j < M; j++)
    {
        L_temp = L_mult (0x3fff, lsf1[j]);
        L_temp = L_mac (L_temp, 0x3fff, lsf2[j]);
        L_temp = L_mult (INV_DTX_HANGOVER_P1, extract_h (L_temp));

        for (i = 0; i < DTX_HANGOVER; i++)
        {
            L_temp = L_mac (L_temp, INV_DTX_HANGOVER_P1, lsf_old[i][j]);
        }

        lsf_aver[j] = extract_h (L_temp);               
    }

    return;
}

/*************************************************************************
 *
 *   FUNCTION NAME: compute_CN_excitation_gain
 *
 *   PURPOSE: Compute the unquantized fixed codebook gain. Computation is
 *            based on the energy of the Linear Prediction residual signal.
 *
 *   INPUTS:      res2[0..39]   Linear Prediction residual signal
 *
 *   OUTPUTS:     none
 *
 *   RETURN VALUE: Unquantized fixed codebook gain
 *
 *************************************************************************/

Word16 compute_CN_excitation_gain (
    Word16 res2[L_SUBFR]
)
{

    Word16 i, norm, norm1, temp, overfl;
    Word32 L_temp;

    /* Compute the energy of the LP residual signal */

    norm = 0;                                           
    do
    {
        overfl = 0;                                    

        L_temp = 0L;                                    
        for (i = 0; i < L_SUBFR; i++)
        {
            temp = shr (res2[i], norm);
            L_temp = L_mac (L_temp, temp, temp);
        }

         
        if (L_sub (L_temp, MAX_32) == 0)
        {
            norm = add (norm, 1);
            overfl = 1;                  /* Set the overflow flag */
        }
         
    }
    while (overfl != 0);

    L_temp = L_add (L_temp, 1L);             /* Avoid the case of all zeros */

    /* Take the square root of the obtained energy value (sqroot is a 2nd
       order Taylor series approximation) */

    norm1 = norm_l (L_temp);
	if(norm1<=0)
		temp = extract_h (L_shr (L_temp, -norm1));
	else
		temp = extract_h (L_shl (L_temp, norm1));
    L_temp = L_mult (temp, temp);
    L_temp = L_sub (805306368L, L_shr (L_temp, 3));
    L_temp = L_add (L_temp, L_mult (24576, temp));

    temp = extract_h (L_temp);
      
    if ((norm1 & 0x0001) != 0)
    {
        temp = mult_r (temp, 23170);
        norm1 = sub (norm1, 1);
    }
    /* Divide the result of sqroot operation by sqroot(10) */

    temp = mult_r (temp, 10362);

    /* Re-scale to get the final value */

    norm1 = shr (norm1, 1);
    norm1 = sub (norm1, norm);

     
    if (norm1 >= 0)
    {
        temp = shr (temp, norm1);
    }
    else
    {
        temp = shl (temp, abs_s (norm1));
    }

    return temp;
}

/*************************************************************************
 *
 *   FUNCTION NAME: aver_gain_code_history
 *
 *   PURPOSE: Compute the averaged fixed codebook gain parameter value.
 *            Computation is performed by averaging the fixed codebook
 *            gain parameter values which exist in the fixed codebook
 *            gain parameter history, together with the fixed codebook
 *            gain parameter value of the current subframe.
 *
 *   INPUTS:      CN_excitation_gain
 *                              Unquantized fixed codebook gain value
 *                              of the current subframe
 *                gain_code_old[0..4*DTX_HANGOVER-1]
 *                              fixed codebook gain parameter history
 *
 *   OUTPUTS:     none
 *
 *   RETURN VALUE: Averaged fixed codebook gain value
 *
 *************************************************************************/

Word16 aver_gain_code_history (
    Word16 CN_excitation_gain,
    Word16 gain_code_old[4 * DTX_HANGOVER]
)
{

    Word16 i;
    Word32 L_ret;

    L_ret = L_mult (0x470, CN_excitation_gain);

    for (i = 0; i < (4 * DTX_HANGOVER); i++)
    {
        L_ret = L_mac (L_ret, 0x470, gain_code_old[i]);
    }
    return extract_h (L_ret);
}
