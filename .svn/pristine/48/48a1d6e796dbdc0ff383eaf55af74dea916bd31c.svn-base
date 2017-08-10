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
#include "../common/typedef.h"
#include "../common/cnst.h"
#include "../common/basic_op.h"
#include "../common/oper_32b.h"
#include "../common/sig_proc.h"
#include "../common/gains_tb.h"

#include "dtx_dec.h"

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
#ifdef PERF
	uc_prof_control->add_function_time();
#endif

    Word16 i, j, ix = 0, tmp[5];
    Word16 max, tmp2[5];

    for (i = 0; i < 5; i++)
    {
        tmp2[i] = ind[i];
    }

    for (i = 0; i < 5; i++)
    {
        max = -8192;
        for (j = 0; j < 5; j++)
        { 
            if (sub (tmp2[j], max) >= 0)
            {
                max = tmp2[j];
                ix = j;
            }
        }
        tmp2[ix] = -16384;
        tmp[i] = ix;
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
    Word16 rxdtx_ctrl,
    Word16 pbuf[5],
    Word16 *past_gain_pit,
    Word16 *prev_gp    
)
{
#ifdef PERF
	uc_prof_control->add_function_time();
#endif

    static const Word16 pdown[7] =
    {
        32767, 32112, 32112, 26214,
        9830, 6553, 6553
    };

    Word16 gain, tmp, i;
 
    if (bfi == 0)
    {
        if ((rxdtx_ctrl & RX_SP_FLAG) != 0)
        {
            gain = shr (qua_gain_pitch[index], 2);

            if (prev_bf != 0)
            {
                if (sub (gain, *prev_gp) > 0)
                {
                    gain = *prev_gp;
                }
            }
        }
        else
        {
            gain = 0;
        }
        *prev_gp = gain;
    }
    else
    {
        if ((rxdtx_ctrl & RX_SP_FLAG) != 0)
        {
            tmp = gmed5 (pbuf);

            if (sub (tmp, *past_gain_pit) < 0)
            {
                *past_gain_pit = tmp;
            }
            gain = mult (pdown[state], *past_gain_pit);
        }
        else
        {
            gain = 0;
        }
    }

    *past_gain_pit = gain;

    if (sub (*past_gain_pit, 4096) > 0)  /* if (*past_gain_pit > 1.0) */
    {
        *past_gain_pit = 4096;
    }
    for (i = 1; i < 5; i++)
    {
        pbuf[i - 1] = pbuf[i];
    }

    pbuf[4] = *past_gain_pit;

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
    Word16 rx_dtx_state,
    Word16 gain_code_old_rx[4 * DTX_HANGOVER],    
    Word16 *buf_p_rx,
    Word16 gbuf[5],
    Word16 *past_gain_code,
    Word16 *prev_gc,
    Word16 *gcode0_CN,
    Word16 *gain_code_old_CN,
    Word16 *gain_code_new_CN,
    Word16 gain_code_muting_CN,    
    Word16 past_qua_en[4],
    Word16 pred[4]       
)
{
#ifdef PERF
	uc_prof_control->add_function_time();
#endif

    static const Word16 cdown[7] =
    {
        32767, 32112, 32112, 32112,
        32112, 32112, 22937
    };

    Word16 i, tmp;
    Word16 gcode0, exp, frac, av_pred_en;
    Word32 ener, ener_code;

    if (((rxdtx_ctrl & RX_UPD_SID_QUANT_MEM) != 0) && (i_subfr == 0))
    {
        *gcode0_CN = update_gcode0_CN (gain_code_old_rx);
        *gcode0_CN = shl (*gcode0_CN, 4);
    }

    /* Handle cases of comfort noise fixed codebook gain decoding in
       which past valid SID frames are repeated */

    if (((rxdtx_ctrl & RX_NO_TRANSMISSION) != 0)
        || ((rxdtx_ctrl & RX_INVALID_SID_FRAME) != 0)
        || ((rxdtx_ctrl & RX_LOST_SID_FRAME) != 0))
    {

        if ((rxdtx_ctrl & RX_NO_TRANSMISSION) != 0)
        {
            /* DTX active: no transmission. Interpolate gain values
            in memory */
            if (i_subfr == 0)
            {
                *gain_code = interpolate_CN_param (*gain_code_old_CN,
                                            *gain_code_new_CN, rx_dtx_state);
            }
            else
            {
                *gain_code = *prev_gc;
            }
        }
        else
        {                       /* Invalid or lost SID frame:
            use gain values from last good SID frame */
            *gain_code_old_CN = *gain_code_new_CN;
            *gain_code = *gain_code_new_CN; 

            /* reset table of past quantized energies */
            for (i = 0; i < 4; i++)
            {
                past_qua_en[i] = -2381;
            }
        }

        if ((rxdtx_ctrl & RX_DTX_MUTING) != 0)
        {
            /* attenuate the gain value by 0.75 dB in each subframe */
            /* (total of 3 dB per frame) */
            gain_code_muting_CN = mult (gain_code_muting_CN, 30057);
            *gain_code = gain_code_muting_CN;
        }
        else
        {
            /* Prepare for DTX muting by storing last good gain value */
            gain_code_muting_CN = *gain_code_new_CN;
        }

        *past_gain_code = *gain_code;

        for (i = 1; i < 5; i++)
        {
            gbuf[i - 1] = gbuf[i];
        }

        gbuf[4] = *past_gain_code;
        *prev_gc = *past_gain_code;

        return;
    }

    /*----------------- Test erasure ---------------*/

    if (bfi != 0)
    {
        tmp = gmed5 (gbuf);
        if (sub (tmp, *past_gain_code) < 0)
        {
            *past_gain_code = tmp;
        }
        *past_gain_code = mult (*past_gain_code, cdown[state]);
        *gain_code = *past_gain_code;

        av_pred_en = 0;
        for (i = 0; i < 4; i++)
        {
            av_pred_en = add (av_pred_en, past_qua_en[i]);
        }

        /* av_pred_en = 0.25*av_pred_en - 4/(20Log10(2)) */
        av_pred_en = mult (av_pred_en, 8192);   /*  *= 0.25  */

        /* if (av_pred_en < -14/(20Log10(2))) av_pred_en = .. */
        if (sub (av_pred_en, -2381) < 0)
        {
            av_pred_en = -2381;
        }
        for (i = 3; i > 0; i--)
        {
            past_qua_en[i] = past_qua_en[i - 1];
        }
        past_qua_en[0] = av_pred_en; 
        for (i = 1; i < 5; i++)
        {
            gbuf[i - 1] = gbuf[i];
        }
        gbuf[4] = *past_gain_code;

        /* Use the most recent comfort noise fixed codebook gain value
           for updating the fixed codebook gain history */
        if (*gain_code_new_CN == 0)
        {
            tmp = *prev_gc;
        }
        else
        {
            tmp = *gain_code_new_CN;
        }

        update_gain_code_history_rx (tmp, gain_code_old_rx,buf_p_rx);

        if (sub (i_subfr, (3 * L_SUBFR)) == 0)
        {
            *gain_code_old_CN = *gain_code;
        }
        return;
    }

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
        ener_code = 0;
        for (i = 0; i < lcode; i++)
        {
            ener_code = L_mac (ener_code, code[i], code[i]);
        }
        /* ener_code = ener_code / lcode */
        ener_code = L_mult (round (ener_code), 26214);

        /* ener_code = 1/2 * Log2(ener_code) */
        Log2 (ener_code, &exp, &frac);
        ener_code = L_Comp (sub (exp, 30), frac);

        /* predicted energy */

        ener = MEAN_ENER;
        for (i = 0; i < 4; i++)
        {
            ener = L_mac (ener, past_qua_en[i], pred[i]);
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

        *gain_code = mult (qua_gain_code[index], gcode0);

        if (prev_bf != 0)
        {
            if (sub (*gain_code, *prev_gc) > 0)
            {
                *gain_code = *prev_gc;
            }
        }
        /*-------------------------------------------------------------------*
         *  update table of past quantized energies                           *
         *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                           *
         *  past_qua_en      = 20 * Log10(qua_gain_code) / constant           *
         *                   = Log2(qua_gain_code)                            *
         *                                           constant = 20*Log10(2)   *
         *-------------------------------------------------------------------*/

        for (i = 3; i > 0; i--)
        {
            past_qua_en[i] = past_qua_en[i - 1];
        }
        Log2 (L_deposit_l (qua_gain_code[index]), &exp, &frac);

        past_qua_en[0] = shr (frac, 5);
        past_qua_en[0] = add (past_qua_en[0], shl (sub (exp, 11), 10));

        update_gain_code_history_rx (*gain_code, gain_code_old_rx, buf_p_rx);

        if (sub (i_subfr, (3 * L_SUBFR)) == 0)
        {
            *gain_code_old_CN = *gain_code;
        }
    }
    else
    {
        if (((rxdtx_ctrl & RX_FIRST_SID_UPDATE) != 0) && (i_subfr == 0))
        {
            *gain_code_new_CN = mult (*gcode0_CN, qua_gain_code[index]);

            /*---------------------------------------------------------------*
             *  reset table of past quantized energies                        *
             *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                        *
             *---------------------------------------------------------------*/

            for (i = 0; i < 4; i++)
            {
                past_qua_en[i] = -2381;
            }
        }
        if (((rxdtx_ctrl & RX_CONT_SID_UPDATE) != 0) && (i_subfr == 0))
        {
            *gain_code_old_CN = *gain_code_new_CN;
            *gain_code_new_CN = mult (*gcode0_CN, qua_gain_code[index]);
        }
		
        if (i_subfr == 0)
        {
            *gain_code = interpolate_CN_param (*gain_code_old_CN,
                                               *gain_code_new_CN,
                                               rx_dtx_state);
        }
        else
        {
            *gain_code = *prev_gc;
        }
    }

    *past_gain_code = *gain_code;

    for (i = 1; i < 5; i++)
    {
        gbuf[i - 1] = gbuf[i];
    }
    gbuf[4] = *past_gain_code;
    *prev_gc = *past_gain_code;

    return;
}
