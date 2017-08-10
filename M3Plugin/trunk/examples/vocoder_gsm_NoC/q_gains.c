/*--------------------------------------------------------------------------*
 * Function q_gain_pitch(), q_gain_code()                                  *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                                  *
 * Scalar quantization of the pitch gain and the innovative codebook gain.  *
 *                                                                          *
 * MA prediction is performed on the innovation energy                      *
 * (in dB/(20*log10(2))) with mean removed.                                 *
 *-------------------------------------------------------------------------*/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "count.h"
#include "sig_proc.h"

#include "gains_tb.h"

#include "cnst.h"
#include "dtx.h"

/* past quantized energies.                               */
/* initialized to -14.0/constant, constant = 20*Log10(2)   */
Word16 past_qua_en[4];

/* MA prediction coeff */
Word16 pred[4];

extern Word16 CN_excitation_gain, gain_code_old_tx[4 * DTX_HANGOVER];

Word16 q_gain_pitch (   /* Return index of quantization */
    Word16 *gain        /* (i)  :  Pitch gain to quantize  */
)
{
    Word16 i, index, gain_q14, err, err_min;

    gain_q14 = shl (*gain, 2);

    err_min = abs_s (sub (gain_q14, qua_gain_pitch[0]));
    index = 0;                                  move16 (); 

    for (i = 1; i < NB_QUA_PITCH; i++)
    {
        err = abs_s (sub (gain_q14, qua_gain_pitch[i]));

        test (); 
        if (sub (err, err_min) < 0)
        {
            err_min = err;                      move16 (); 
            index = i;                          move16 (); 
        }
    }

    *gain = shr (qua_gain_pitch[index], 2);     move16 (); 

    return index;
}

/* average innovation energy.                             */
/* MEAN_ENER  = 36.0/constant, constant = 20*Log10(2)     */

#define MEAN_ENER  783741L      /* 36/(20*log10(2))       */

Word16 q_gain_code (    /* Return quantization index                  */
    Word16 code[],      /* (i)      : fixed codebook excitation       */
    Word16 lcode,       /* (i)      : codevector size                 */
    Word16 *gain,       /* (i/o)    : quantized fixed codebook gain   */
    Word16 txdtx_ctrl,
    Word16 i_subfr
)
{
    Word16 i, index;
    Word16 gcode0, err, err_min, exp, frac;
    Word32 ener, ener_code;
    Word16 aver_gain;
    static Word16 gcode0_CN;

    logic16 (); test (); 
    if ((txdtx_ctrl & TX_SP_FLAG) != 0)
    {

        /*-------------------------------------------------------------------*
         *  energy of code:                                                   *
         *  ~~~~~~~~~~~~~~~                                                   *
         *  ener_code(Q17) = 10 * Log10(energy/lcode) / constant              *
         *                 = 1/2 * Log2(energy/lcode)                         *
         *                                           constant = 20*Log10(2)   *
         *-------------------------------------------------------------------*/

        /* ener_code = log10(ener_code/lcode) / (20*log10(2))       */
        ener_code = 0;                          move32 (); 
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

        ener = MEAN_ENER;                       move32 (); 
        for (i = 0; i < 4; i++)
        {
            ener = L_mac (ener, past_qua_en[i], pred[i]);
        }

        /*-------------------------------------------------------------------*
         *  predicted codebook gain                                           *
         *  ~~~~~~~~~~~~~~~~~~~~~~~                                           *
         *  gcode0(Qx) = Pow10( (ener*constant - ener_code*constant) / 20 )   *
         *             = Pow2(ener-ener_code)                                 *
         *                                           constant = 20*Log10(2)   *
         *-------------------------------------------------------------------*/

        ener = L_shr (L_sub (ener, ener_code), 1);
        L_Extract (ener, &exp, &frac);

        gcode0 = extract_l (Pow2 (exp, frac));  /* predicted gain */

        gcode0 = shl (gcode0, 4);

        /*-------------------------------------------------------------------*
         *                   Search for best quantizer                        *
         *-------------------------------------------------------------------*/

        err_min = abs_s (sub (*gain, mult (gcode0, qua_gain_code[0])));
        index = 0;              move16 (); 

        for (i = 1; i < NB_QUA_CODE; i++)
        {
            err = abs_s (sub (*gain, mult (gcode0, qua_gain_code[i])));

            test (); 
            if (sub (err, err_min) < 0)
            {
                err_min = err;                  move16 (); 
                index = i;                      move16 (); 
            }
        }

        *gain = mult (gcode0, qua_gain_code[index]);
                                                move16 (); 

        /*------------------------------------------------------------------*
         *  update table of past quantized energies                         *
         *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                         *
         *  past_qua_en(Q12) = 20 * Log10(qua_gain_code) / constant         *
         *                   = Log2(qua_gain_code)                          *
         *                                           constant = 20*Log10(2) *
         *------------------------------------------------------------------*/

        for (i = 3; i > 0; i--)
        {
            past_qua_en[i] = past_qua_en[i - 1];move16 (); 
        }
        Log2 (L_deposit_l (qua_gain_code[index]), &exp, &frac);

        past_qua_en[0] = shr (frac, 5);         move16 (); 
        past_qua_en[0] = add (past_qua_en[0], shl (sub (exp, 11), 10));
                                                move16 (); 

        update_gain_code_history_tx (*gain, gain_code_old_tx);
    }
    else
    {
        logic16 (); test (); test (); 
        if ((txdtx_ctrl & TX_PREV_HANGOVER_ACTIVE) != 0 && (i_subfr == 0))
        {
            gcode0_CN = update_gcode0_CN (gain_code_old_tx);
            gcode0_CN = shl (gcode0_CN, 4);
        }
        *gain = CN_excitation_gain;             move16 (); 

        logic16 (); test (); test (); 
        if ((txdtx_ctrl & TX_SID_UPDATE) != 0)
        {
            aver_gain = aver_gain_code_history (CN_excitation_gain,
                                                gain_code_old_tx);

            /*---------------------------------------------------------------*
             *                   Search for best quantizer                    *
             *---------------------------------------------------------------*/

            err_min = abs_s (sub (aver_gain, 
                                  mult (gcode0_CN, qua_gain_code[0])));
            index = 0;                          move16 (); 

            for (i = 1; i < NB_QUA_CODE; i++)
            {
                err = abs_s (sub (aver_gain, 
                                  mult (gcode0_CN, qua_gain_code[i])));

                test (); 
                if (sub (err, err_min) < 0)
                {
                    err_min = err;              move16 (); 
                    index = i;                  move16 (); 
                }
            }
        }
        update_gain_code_history_tx (*gain, gain_code_old_tx);

        /*-------------------------------------------------------------------*
         *  reset table of past quantized energies                            *
         *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                            *
         *-------------------------------------------------------------------*/

        for (i = 0; i < 4; i++)
        {
            past_qua_en[i] = -2381;             move16 (); 
        }
    }

    return index;
}
