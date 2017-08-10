/*************************************************************************
 *
 *  FILE NAME:   D_GAINS.C
 *
 *  FUNCTIONS DEFINED IN THIS FILE:
 *
 *        d_gain_pitch(), d_gain_code() and gmed5()
 *
 * MA prediction is performed on the innovation energy
 * ( in dB/(20*log10(2)) ) with mean removed.
 *************************************************************************/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "count.h"
#include "sig_proc.h"

#include "gains_tb.h"

#include "cnst.h"
#include "dtx.h"

extern Word16 gain_code_old_rx[4 * DTX_HANGOVER];

/* Static variables for bad frame handling */

/* Variables used by d_gain_pitch: */
Word16 pbuf[5], past_gain_pit, prev_gp;

/* Variables used by d_gain_code: */
Word16 gbuf[5], past_gain_code, prev_gc;

/* Static variables for CNI (used by d_gain_code) */

Word16 gcode0_CN, gain_code_old_CN, gain_code_new_CN, gain_code_muting_CN;

/* Memories of gain dequantization: */

/* past quantized energies.      */
/* initialized to -14.0/constant, constant = 20*Log10(2) */

Word16 past_qua_dec[4];

/* MA prediction coeff   */
Word16 pred_dec[4];

/*************************************************************************
 *
 *  FUNCTION:   gmed5
 *
 *  PURPOSE:    calculates 5-point median.
 *
 *  DESCRIPTION:
 *             
 *************************************************************************/

Word16 gmed5 (        /* out      : index of the median value (0...4) */
    Word16 ind[]      /* in       : Past gain values                  */
)
{
    Word16 i, j, ix = 0, tmp[5];
    Word16 max, tmp2[5];

    for (i = 0; i < 5; i++)
    {
        tmp2[i] = ind[i];                                      move16 (); 
    }

    for (i = 0; i < 5; i++)
    {
        max = -8192;                                           move16 (); 
        for (j = 0; j < 5; j++)
        {
            test (); 
            if (sub (tmp2[j], max) >= 0)
            {
                max = tmp2[j];                                 move16 (); 
                ix = j;                                        move16 (); 
            }
        }
        tmp2[ix] = -16384;                                     move16 (); 
        tmp[i] = ix;                                           move16 (); 
    }

    return (ind[tmp[2]]);
}

/*************************************************************************
 *
 *  FUNCTION:   d_gain_pitch
 *
 *  PURPOSE:  decodes the pitch gain using the received index.
 *
 *  DESCRIPTION:
 *       In case of no frame erasure, the gain is obtained from the
 *       quantization table at the given index; otherwise, a downscaled
 *       past gain is used.
 *
 *************************************************************************/

Word16 d_gain_pitch ( /* out      : quantized pitch gain           */
    Word16 index,     /* in       : index of quantization          */
    Word16 bfi,       /* in       : bad frame indicator (good = 0) */
    Word16 state,
    Word16 prev_bf,
    Word16 rxdtx_ctrl
)
{
    static const Word16 pdown[7] =
    {
        32767, 32112, 32112, 26214,
        9830, 6553, 6553
    };

    Word16 gain, tmp, i;

    test (); 
    if (bfi == 0)
    {
        test (); logic16 (); 
        if ((rxdtx_ctrl & RX_SP_FLAG) != 0)
        {
            gain = shr (qua_gain_pitch[index], 2);             move16 (); 

            test (); 
            if (prev_bf != 0)
            {
                test (); 
                if (sub (gain, prev_gp) > 0)
                {
                    gain = prev_gp;
                }
            }
        }
        else
        {
            gain = 0;                                          move16 (); 
        }
        prev_gp = gain;                                        move16 (); 
    }
    else
    {
        test (); logic16 (); 
        if ((rxdtx_ctrl & RX_SP_FLAG) != 0)
        {
            tmp = gmed5 (pbuf);                                move16 (); 

            test (); 
            if (sub (tmp, past_gain_pit) < 0)
            {
                past_gain_pit = tmp;                           move16 (); 
            }
            gain = mult (pdown[state], past_gain_pit);
        }
        else
        {
            gain = 0;                                          move16 (); 
        }
    }

    past_gain_pit = gain;                                      move16 (); 

    test (); 
    if (sub (past_gain_pit, 4096) > 0)  /* if (past_gain_pit > 1.0) */
    {
        past_gain_pit = 4096;                                  move16 (); 
    }
    for (i = 1; i < 5; i++)
    {
        pbuf[i - 1] = pbuf[i];                                 move16 (); 
    }

    pbuf[4] = past_gain_pit;                                   move16 (); 

    return gain;
}

/*************************************************************************
 *
 *  FUNCTION:  d_gain_code
 *
 *  PURPOSE:  decode the fixed codebook gain using the received index.
 *
 *  DESCRIPTION:
 *      The received index gives the gain correction factor gamma.
 *      The quantized gain is given by   g_q = g0 * gamma
 *      where g0 is the predicted gain.
 *      To find g0, 4th order MA prediction is applied to the mean-removed
 *      innovation energy in dB.
 *      In case of frame erasure, downscaled past gain is used.
 *
 *************************************************************************/

/* average innovation energy.                             */
/* MEAN_ENER = 36.0/constant, constant = 20*Log10(2)      */
#define MEAN_ENER  783741L      /* 36/(20*log10(2))       */

void d_gain_code (
    Word16 index,      /* input : received quantization index */
    Word16 code[],     /* input : innovation codevector       */
    Word16 lcode,      /* input : codevector length           */
    Word16 *gain_code, /* output: decoded innovation gain     */
    Word16 bfi,        /* input : bad frame indicator         */
    Word16 state,
    Word16 prev_bf,
    Word16 rxdtx_ctrl,
    Word16 i_subfr,
    Word16 rx_dtx_state
)
{
    static const Word16 cdown[7] =
    {
        32767, 32112, 32112, 32112,
        32112, 32112, 22937
    };

    Word16 i, tmp;
    Word16 gcode0, exp, frac, av_pred_en;
    Word32 ener, ener_code;

    test (); test (); logic16 (); 
    if (((rxdtx_ctrl & RX_UPD_SID_QUANT_MEM) != 0) && (i_subfr == 0))
    {
        gcode0_CN = update_gcode0_CN (gain_code_old_rx);       move16 (); 
        gcode0_CN = shl (gcode0_CN, 4);
    }

    /* Handle cases of comfort noise fixed codebook gain decoding in
       which past valid SID frames are repeated */

    test (); test (); test (); logic16 (); logic16 (); logic16 (); 
    if (((rxdtx_ctrl & RX_NO_TRANSMISSION) != 0)
        || ((rxdtx_ctrl & RX_INVALID_SID_FRAME) != 0)
        || ((rxdtx_ctrl & RX_LOST_SID_FRAME) != 0))
    {

        test (); logic16 (); 
        if ((rxdtx_ctrl & RX_NO_TRANSMISSION) != 0)
        {
            /* DTX active: no transmission. Interpolate gain values
            in memory */
            test (); 
            if (i_subfr == 0)
            {
                *gain_code = interpolate_CN_param (gain_code_old_CN,
                                            gain_code_new_CN, rx_dtx_state);
                                                               move16 (); 
            }
            else
            {
                *gain_code = prev_gc;                          move16 (); 
            }
        }
        else
        {                       /* Invalid or lost SID frame:
            use gain values from last good SID frame */
            gain_code_old_CN = gain_code_new_CN;               move16 (); 
            *gain_code = gain_code_new_CN;                     move16 (); 

            /* reset table of past quantized energies */
            for (i = 0; i < 4; i++)
            {
                past_qua_dec[i] = -2381;                        move16 (); 
            }
        }

        test (); logic16 (); 
        if ((rxdtx_ctrl & RX_DTX_MUTING) != 0)
        {
            /* attenuate the gain value by 0.75 dB in each subframe */
            /* (total of 3 dB per frame) */
            gain_code_muting_CN = mult (gain_code_muting_CN, 30057);
            *gain_code = gain_code_muting_CN;                  move16 (); 
        }
        else
        {
            /* Prepare for DTX muting by storing last good gain value */
            gain_code_muting_CN = gain_code_new_CN;            move16 (); 
        }

        past_gain_code = *gain_code;                           move16 (); 

        for (i = 1; i < 5; i++)
        {
            gbuf[i - 1] = gbuf[i];                             move16 (); 
        }

        gbuf[4] = past_gain_code;                              move16 (); 
        prev_gc = past_gain_code;                              move16 (); 

        return;
    }

    /*----------------- Test erasure ---------------*/

    test (); 
    if (bfi != 0)
    {
        tmp = gmed5 (gbuf);                                    move16 (); 
        test (); 
        if (sub (tmp, past_gain_code) < 0)
        {
            past_gain_code = tmp;                              move16 (); 
        }
        past_gain_code = mult (past_gain_code, cdown[state]);
        *gain_code = past_gain_code;                           move16 (); 

        av_pred_en = 0;                                        move16 (); 
        for (i = 0; i < 4; i++)
        {
            av_pred_en = add (av_pred_en, past_qua_dec[i]);
        }

        /* av_pred_en = 0.25*av_pred_en - 4/(20Log10(2)) */
        av_pred_en = mult (av_pred_en, 8192);   /*  *= 0.25  */

        /* if (av_pred_en < -14/(20Log10(2))) av_pred_en = .. */
        test (); 
        if (sub (av_pred_en, -2381) < 0)
        {
            av_pred_en = -2381;                                move16 (); 
        }
        for (i = 3; i > 0; i--)
        {
            past_qua_dec[i] = past_qua_dec[i - 1];               move16 (); 
        }
        past_qua_dec[0] = av_pred_en;                           move16 (); 
        for (i = 1; i < 5; i++)
        {
            gbuf[i - 1] = gbuf[i];                             move16 (); 
        }
        gbuf[4] = past_gain_code;                              move16 (); 

        /* Use the most recent comfort noise fixed codebook gain value
           for updating the fixed codebook gain history */
        test ();
        if (gain_code_new_CN == 0)
        {
            tmp = prev_gc;                                     move16 ();
        }
        else
        {
            tmp = gain_code_new_CN;
        }

        update_gain_code_history_rx (tmp, gain_code_old_rx);

        test ();
        if (sub (i_subfr, (3 * L_SUBFR)) == 0)
        {
            gain_code_old_CN = *gain_code;                     move16 (); 
        }
        return;
    }

    test (); logic16 (); 
    if ((rxdtx_ctrl & RX_SP_FLAG) != 0)
    {

        /*-------------- Decode codebook gain ---------------*/

        /*-------------------------------------------------------------------*
         *  energy of code:                                                   *
         *  ~~~~~~~~~~~~~~~                                                   *
         *  ener_code = 10 * Log10(energy/lcode) / constant                   *
         *            = 1/2 * Log2(energy/lcode)                              *
         *                                           constant = 20*Log10(2)   *
         *-------------------------------------------------------------------*/

        /* ener_code = log10(ener_code/lcode) / (20*log10(2)) */
        ener_code = 0;                                         move32 (); 
        for (i = 0; i < lcode; i++)
        {
            ener_code = L_mac (ener_code, code[i], code[i]);
        }
        /* ener_code = ener_code / lcode */
        ener_code = L_mult (round_z (ener_code), 26214);

        /* ener_code = 1/2 * Log2(ener_code) */
        Log2 (ener_code, &exp, &frac);
        ener_code = L_Comp (sub (exp, 30), frac);

        /* predicted energy */

        ener = MEAN_ENER;                                      move32 (); 
        for (i = 0; i < 4; i++)
        {
            ener = L_mac (ener, past_qua_dec[i], pred_dec[i]);
        }

        /*-------------------------------------------------------------------*
         *  predicted codebook gain                                           *
         *  ~~~~~~~~~~~~~~~~~~~~~~~                                           *
         *  gcode0     = Pow10( (ener*constant - ener_code*constant) / 20 )   *
         *             = Pow2(ener-ener_code)                                 *
         *                                           constant = 20*Log10(2)   *
         *-------------------------------------------------------------------*/

        ener = L_shr (L_sub (ener, ener_code), 1);
        L_Extract (ener, &exp, &frac);

        gcode0 = extract_l (Pow2 (exp, frac));  /* predicted gain */

        gcode0 = shl (gcode0, 4);

        *gain_code = mult (qua_gain_code[index], gcode0);      move16 (); 

        test (); 
        if (prev_bf != 0)
        {
            test (); 
            if (sub (*gain_code, prev_gc) > 0)
            {
                *gain_code = prev_gc;     move16 (); 
            }
        }
        /*-------------------------------------------------------------------*
         *  update table of past quantized energies                           *
         *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                           *
         *  past_qua_dec      = 20 * Log10(qua_gain_code) / constant           *
         *                   = Log2(qua_gain_code)                            *
         *                                           constant = 20*Log10(2)   *
         *-------------------------------------------------------------------*/

        for (i = 3; i > 0; i--)
        {
            past_qua_dec[i] = past_qua_dec[i - 1];               move16 (); 
        }
        Log2 (L_deposit_l (qua_gain_code[index]), &exp, &frac);

        past_qua_dec[0] = shr (frac, 5);                        move16 (); 
        past_qua_dec[0] = add (past_qua_dec[0], shl (sub (exp, 11), 10));
        move16 (); 

        update_gain_code_history_rx (*gain_code, gain_code_old_rx);

        if (sub (i_subfr, (3 * L_SUBFR)) == 0)
        {
            gain_code_old_CN = *gain_code;                     move16 (); 
        }
    }
    else
    {
        test (); test (); logic16 (); 
        if (((rxdtx_ctrl & RX_FIRST_SID_UPDATE) != 0) && (i_subfr == 0))
        {
            gain_code_new_CN = mult (gcode0_CN, qua_gain_code[index]);

            /*---------------------------------------------------------------*
             *  reset table of past quantized energies                        *
             *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                        *
             *---------------------------------------------------------------*/

            for (i = 0; i < 4; i++)
            {
                past_qua_dec[i] = -2381;                        move16 (); 
            }
        }
        test (); test (); logic16 (); 
        if (((rxdtx_ctrl & RX_CONT_SID_UPDATE) != 0) && (i_subfr == 0))
        {
            gain_code_old_CN = gain_code_new_CN;               move16 (); 
            gain_code_new_CN = mult (gcode0_CN, qua_gain_code[index]);
                                                               move16 (); 
        }
        test (); 
        if (i_subfr == 0)
        {
            *gain_code = interpolate_CN_param (gain_code_old_CN,
                                               gain_code_new_CN,
                                               rx_dtx_state);  move16 (); 
        }
        else
        {
            *gain_code = prev_gc;                              move16 (); 
        }
    }

    past_gain_code = *gain_code;                               move16 (); 

    for (i = 1; i < 5; i++)
    {
        gbuf[i - 1] = gbuf[i];                                 move16 (); 
    }
    gbuf[4] = past_gain_code;                                  move16 (); 
    prev_gc = past_gain_code;                                  move16 (); 

    return;
}
