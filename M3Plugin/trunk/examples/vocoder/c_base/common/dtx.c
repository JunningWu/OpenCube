//
// 
//    File Name: dtx.h
// 
//    Purpose:   Contains the COMMON implementation for functions of DTX.
//               Also contains COMMON constants used in DTX functions.
// 
//		TEISA Dpt.
//  University of Cantabria
//  Date: Oct.2003
//  Author: F.Herrera
//  Adaptation of Coder code to SystemC
//
//  Common part of the dtx code
//

#include "typedef.h"
#include "cnst.h"
#include "basic_op.h"
#include "sig_proc.h"

#include "dtx.h"

/*************************************************************************
 *
 *   FUNCTION NAME: update_lsf_history
 *
 *   PURPOSE: Update the LSF parameter history. The LSF parameters kept
 *            in the buffer are used later for computing the reference
 *            LSF parameter vector and the averaged LSF parameter vector.
 *
 *   INPUTS:      lsf1[0..9]    LSF vector of the 1st half of the frame
 *                lsf2[0..9]    LSF vector of the 2nd half of the frame
 *                lsf_old[0..DTX_HANGOVER-1][0..M-1]
 *                              Old LSF history
 *
 *   OUTPUTS:     lsf_old[0..DTX_HANGOVER-1][0..M-1]
 *                              Updated LSF history
 *
 *   RETURN VALUE: none
 *
 *************************************************************************/

void update_lsf_history (
    Word16 lsf1[M],
    Word16 lsf2[M],
    Word16 lsf_old[DTX_HANGOVER][M]
)
{

    Word16 i, j, temp;

    /* shift LSF data to make room for LSFs from current frame */
    /* This can also be implemented by using circular buffering */

    for (i = DTX_HANGOVER - 1; i > 0; i--)
    {
        for (j = 0; j < M; j++)
        {
            lsf_old[i][j] = lsf_old[i - 1][j];          
        }
    }

    /* Store new LSF data to lsf_old buffer */

    for (i = 0; i < M; i++)
    {
        temp = add (shr (lsf1[i], 1), shr (lsf2[i], 1));
        lsf_old[0][i] = temp;                           
    }

    return;
}

/*************************************************************************
 *
 *   FUNCTION NAME: update_lsf_p_CN
 *
 *   PURPOSE: Update the reference LSF parameter vector. The reference
 *            vector is computed by averaging the quantized LSF parameter
 *            vectors which exist in the LSF parameter history.
 *
 *   INPUTS:      lsf_old[0..DTX_HANGOVER-1][0..M-1]
 *                                 LSF parameter history
 *
 *   OUTPUTS:     lsf_p_CN[0..9]   Computed reference LSF parameter vector
 *
 *   RETURN VALUE: none
 *
 *************************************************************************/

void update_lsf_p_CN (
    Word16 lsf_old[DTX_HANGOVER][M],
    Word16 lsf_p_CN[M]
)
{

    Word16 i, j;
    Word32 L_temp;

    for (j = 0; j < M; j++)
    {
        L_temp = L_mult (INV_DTX_HANGOVER, lsf_old[0][j]);
        for (i = 1; i < DTX_HANGOVER; i++)
        {
            L_temp = L_mac (L_temp, INV_DTX_HANGOVER, lsf_old[i][j]);
        }
        lsf_p_CN[j] = round (L_temp);                   
    }

    return;
}

/*************************************************************************
 *
 *   FUNCTION NAME: update_gcode0_CN
 *
 *   PURPOSE: Update the reference fixed codebook gain parameter value.
 *            The reference value is computed by averaging the quantized
 *            fixed codebook gain parameter values which exist in the
 *            fixed codebook gain parameter history.
 *
 *   INPUTS:      gain_code_old[0..4*DTX_HANGOVER-1]
 *                              fixed codebook gain parameter history
 *
 *   OUTPUTS:     none
 *
 *   RETURN VALUE: Computed reference fixed codebook gain
 *
 *************************************************************************/

Word16 update_gcode0_CN (
    Word16 gain_code_old[4 * DTX_HANGOVER]
)
{

    Word16 i, j;
    Word32 L_temp, L_ret;

    L_ret = 0L;                                         
    for (i = 0; i < DTX_HANGOVER; i++)
    {
        L_temp = L_mult (0x1fff, gain_code_old[4 * i]);
        for (j = 1; j < 4; j++)
        {
            L_temp = L_mac (L_temp, 0x1fff, gain_code_old[4 * i + j]);
        }
        L_ret = L_mac (L_ret, INV_DTX_HANGOVER, extract_h (L_temp));
    }

    return extract_h (L_ret);
}

/*************************************************************************
 *
 *   FUNCTION NAME: build_CN_code
 *
 *   PURPOSE: Compute the comfort noise fixed codebook excitation. The
 *            gains of the pulses are always +/-1.
 *
 *   INPUTS:      *seed         Old CN generator shift register state
 *
 *   OUTPUTS:     cod[0..39]    Generated comfort noise fixed codebook vector
 *                *seed         Updated CN generator shift register state
 *
 *   RETURN VALUE: none
 *
 *************************************************************************/

void build_CN_code (
    Word16 cod[],
    Word32 *seed
)
{

    Word16 i, j, k;

    for (i = 0; i < L_SUBFR; i++)
    {
        cod[i] = 0;                                     
    }

    for (k = 0; k < NB_PULSE; k++)
    {
        i = pseudonoise (seed, 2);      /* generate pulse position */
        i = shr (extract_l (L_mult (i, 10)), 1);
        i = add (i, k);

        j = pseudonoise (seed, 1);      /* generate sign           */

         
        if (j > 0)
        {
            cod[i] = 4096;                              
        }
        else
        {
            cod[i] = -4096;                             
        }
    }

    return;
}

/*************************************************************************
 *
 *   FUNCTION NAME: pseudonoise
 *
 *   PURPOSE: Generate a random integer value to use in comfort noise
 *            generation. The algorithm uses polynomial x^31 + x^3 + 1
 *            (length of PN sequence is 2^31 - 1).
 *
 *   INPUTS:      *shift_reg    Old CN generator shift register state
 *
 *
 *   OUTPUTS:     *shift_reg    Updated CN generator shift register state
 *
 *   RETURN VALUE: Generated random integer value
 *
 *************************************************************************/

Word16 pseudonoise (
    Word32 *shift_reg,
    Word16 no_bits
)
{

    Word16 noise_bits, Sn, i;

    noise_bits = 0;                                     
    for (i = 0; i < no_bits; i++)
    {
        /* State n == 31 */
          
        if ((*shift_reg & 0x00000001L) != 0)
        {
            Sn = 1;                                     
        }
        else
        {
            Sn = 0;                                     
        }

        /* State n == 3 */
          
        if ((*shift_reg & 0x10000000L) != 0)
        {
            Sn = Sn ^ 1;                                
        }
        else
        {
            Sn = Sn ^ 0;                                
        }

        noise_bits = shl (noise_bits, 1);
        noise_bits = noise_bits | (extract_l (*shift_reg) & 1);
          

        *shift_reg = L_shr (*shift_reg, 1);             
          
        if (Sn & 1)
        {
            *shift_reg = *shift_reg | 0x40000000L;       
        }
    }

    return noise_bits;
}
