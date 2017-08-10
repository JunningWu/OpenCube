/*************************************************************************
 *
 *  FUNCTION:   D_plsf_5()
 *
 *  PURPOSE: Decodes the 2 sets of LSP parameters in a frame using the
 *           received quantization indices.
 *
 *  DESCRIPTION:
 *           The two sets of LSFs are quantized using split by 5 matrix
 *           quantization (split-MQ) with 1st order MA prediction.
 *
 *   See "q_plsf_5.c" for more details about the quantization procedure
 *
 *************************************************************************/

#include "typedef.h"
#include "basic_op.h"
#include "count.h"
#include "sig_proc.h"

#include "q_plsf_5.tab"         /* Codebooks of LSF prediction residual */

#include "cnst.h"
#include "dtx.h"

/* M  ->order of linear prediction filter                      */
/* LSF_GAP  -> Minimum distance between LSF after quantization */
/*             50 Hz = 205                                     */
/* PRED_FAC -> Prediction factor = 0.65                        */
/* ALPHA    ->  0.9                                            */
/* ONE_ALPHA-> (1.0-ALPHA)                                     */

#define M         10
#define LSF_GAP   205
#define PRED_FAC  21299
#define ALPHA     31128
#define ONE_ALPHA 1639

/* Past quantized prediction error */

Word16 past_r2_d[M];

/* Past dequantized lsfs */

Word16 past_lsf_q[M];

/* Reference LSF parameter vector (comfort noise) */

Word16 lsf_p_CN[M];

/*  LSF memories for comfort noise interpolation */

Word16 lsf_old_CN[M], lsf_new_CN[M];

 /* LSF parameter buffer */

extern Word16 lsf_old_rx[DTX_HANGOVER][M];

void D_plsf_5 (
    Word16 *indice,       /* input : quantization indices of 5 submatrices */
    Word16 *lsp1_q,       /* output: quantized 1st LSP vector              */
    Word16 *lsp2_q,       /* output: quantized 2nd LSP vector              */
    Word16 bfi,           /* input : bad frame indicator (set to 1 if a bad
                                     frame is received)                    */
    Word16 rxdtx_ctrl,    /* input : RX DTX control word                   */
    Word16 rx_dtx_state   /* input : state of the comfort noise insertion
                                     period                                */
)
{
    Word16 i;
    const Word16 *p_dico;
    Word16 temp, sign;
    Word16 lsf1_r[M], lsf2_r[M];
    Word16 lsf1_q[M], lsf2_q[M];

    /* Update comfort noise LSF quantizer memory */
    test (); logic16 (); 
    if ((rxdtx_ctrl & RX_UPD_SID_QUANT_MEM) != 0)
    {
        update_lsf_p_CN (lsf_old_rx, lsf_p_CN);
    }
    /* Handle cases of comfort noise LSF decoding in which past
    valid SID frames are repeated */

    test (); logic16 (); 
    test (); logic16 (); 
    test (); logic16 (); 
    if (((rxdtx_ctrl & RX_NO_TRANSMISSION) != 0)
        || ((rxdtx_ctrl & RX_INVALID_SID_FRAME) != 0)
        || ((rxdtx_ctrl & RX_LOST_SID_FRAME) != 0))
    {

        test (); logic16 (); 
        if ((rxdtx_ctrl & RX_NO_TRANSMISSION) != 0)
        {
            /* DTX active: no transmission. Interpolate LSF values in memory */
            interpolate_CN_lsf (lsf_old_CN, lsf_new_CN, lsf2_q, rx_dtx_state);
        }
        else
        {                       /* Invalid or lost SID frame: use LSFs
                                   from last good SID frame */
            for (i = 0; i < M; i++)
            {
                lsf_old_CN[i] = lsf_new_CN[i];  move16 (); 
                lsf2_q[i] = lsf_new_CN[i];      move16 (); 
                past_r2_d[i] = 0;               move16 (); 
            }
        }

        for (i = 0; i < M; i++)
        {
            past_lsf_q[i] = lsf2_q[i];          move16 (); 
        }

        /*  convert LSFs to the cosine domain */
        Lsf_lsp (lsf2_q, lsp2_q, M);

        return;
    }

    test (); 
    if (bfi != 0)                               /* if bad frame */
    {
        /* use the past LSFs slightly shifted towards their mean */

        for (i = 0; i < M; i++)
        {
            /* lsfi_q[i] = ALPHA*past_lsf_q[i] + ONE_ALPHA*mean_lsf[i]; */

            lsf1_q[i] = add (mult (past_lsf_q[i], ALPHA),
                             mult (mean_lsf[i], ONE_ALPHA));
                                                move16 (); 

            lsf2_q[i] = lsf1_q[i];              move16 (); 
        }

        /* estimate past quantized residual to be used in next frame */

        for (i = 0; i < M; i++)
        {
            /* temp  = mean_lsf[i] +  past_r2_d[i] * PRED_FAC; */

            temp = add (mean_lsf[i], mult (past_r2_d[i], PRED_FAC));

            past_r2_d[i] = sub (lsf2_q[i], temp);
                                                move16 (); 
        }
    }
    else
        /* if good LSFs received */
    {
        /* decode prediction residuals from 5 received indices */

        p_dico = &dico1_lsf[shl (indice[0], 2)];
        lsf1_r[0] = *p_dico++;                  move16 (); 
        lsf1_r[1] = *p_dico++;                  move16 (); 
        lsf2_r[0] = *p_dico++;                  move16 (); 
        lsf2_r[1] = *p_dico++;                  move16 (); 

        p_dico = &dico2_lsf[shl (indice[1], 2)];
        lsf1_r[2] = *p_dico++;                  move16 (); 
        lsf1_r[3] = *p_dico++;                  move16 (); 
        lsf2_r[2] = *p_dico++;                  move16 (); 
        lsf2_r[3] = *p_dico++;                  move16 (); 

        sign = indice[2] & 1;                   logic16 (); 
        i = shr (indice[2], 1);
        p_dico = &dico3_lsf[shl (i, 2)];        move16 (); 

        test (); 
        if (sign == 0)
        {
            lsf1_r[4] = *p_dico++;              move16 (); 
            lsf1_r[5] = *p_dico++;              move16 (); 
            lsf2_r[4] = *p_dico++;              move16 (); 
            lsf2_r[5] = *p_dico++;              move16 (); 
        }
        else
        {
            lsf1_r[4] = negate (*p_dico++);     move16 (); 
            lsf1_r[5] = negate (*p_dico++);     move16 (); 
            lsf2_r[4] = negate (*p_dico++);     move16 (); 
            lsf2_r[5] = negate (*p_dico++);     move16 (); 
        }

        p_dico = &dico4_lsf[shl (indice[3], 2)];move16 (); 
        lsf1_r[6] = *p_dico++;                  move16 (); 
        lsf1_r[7] = *p_dico++;                  move16 (); 
        lsf2_r[6] = *p_dico++;                  move16 (); 
        lsf2_r[7] = *p_dico++;                  move16 (); 

        p_dico = &dico5_lsf[shl (indice[4], 2)];move16 (); 
        lsf1_r[8] = *p_dico++;                  move16 (); 
        lsf1_r[9] = *p_dico++;                  move16 (); 
        lsf2_r[8] = *p_dico++;                  move16 (); 
        lsf2_r[9] = *p_dico++;                  move16 (); 

        /* Compute quantized LSFs and update the past quantized residual */
        /* Use lsf_p_CN as predicted LSF vector in case of no speech
           activity */

        test (); logic16 (); 
        if ((rxdtx_ctrl & RX_SP_FLAG) != 0)
        {
            for (i = 0; i < M; i++)
            {
                temp = add (mean_lsf[i], mult (past_r2_d[i], PRED_FAC));
                lsf1_q[i] = add (lsf1_r[i], temp);
                                                move16 (); 
                lsf2_q[i] = add (lsf2_r[i], temp);
                                                move16 (); 
                past_r2_d[i] = lsf2_r[i];       move16 (); 
            }
        }
        else
        {                       /* Valid SID frame */
            for (i = 0; i < M; i++)
            {
                lsf2_q[i] = add (lsf2_r[i], lsf_p_CN[i]);
                                                move16 (); 

                /* Use the dequantized values of lsf2 also for lsf1 */
                lsf1_q[i] = lsf2_q[i];          move16 (); 

                past_r2_d[i] = 0;               move16 (); 
            }
        }
    }

    /* verification that LSFs have minimum distance of LSF_GAP Hz */

    Reorder_lsf (lsf1_q, LSF_GAP, M);
    Reorder_lsf (lsf2_q, LSF_GAP, M);

    test (); logic16 (); 
    if ((rxdtx_ctrl & RX_FIRST_SID_UPDATE) != 0)
    {
        for (i = 0; i < M; i++)
        {
            lsf_new_CN[i] = lsf2_q[i];          move16 (); 
        }
    }
    test (); logic16 (); 
    if ((rxdtx_ctrl & RX_CONT_SID_UPDATE) != 0)
    {
        for (i = 0; i < M; i++)
        {
            lsf_old_CN[i] = lsf_new_CN[i];      move16 (); 
            lsf_new_CN[i] = lsf2_q[i];          move16 (); 
        }
    }
    test (); logic16 (); 
    if ((rxdtx_ctrl & RX_SP_FLAG) != 0)
    {
        /* Update lsf history with quantized LSFs
           when speech activity is present. If the current frame is
           a bad one, update with most recent good comfort noise LSFs */

        if (bfi==0)
        {
            update_lsf_history (lsf1_q, lsf2_q, lsf_old_rx);
        }
        else
        {
            update_lsf_history (lsf_new_CN, lsf_new_CN, lsf_old_rx);
        }

        for (i = 0; i < M; i++)
        {
            lsf_old_CN[i] = lsf2_q[i];          move16 (); 
        }
    }
    else
    {
        interpolate_CN_lsf (lsf_old_CN, lsf_new_CN, lsf2_q, rx_dtx_state);
    }

    for (i = 0; i < M; i++)
    {
        past_lsf_q[i] = lsf2_q[i];              move16 (); 
    }

    /*  convert LSFs to the cosine domain */

    Lsf_lsp (lsf1_q, lsp1_q, M);
    Lsf_lsp (lsf2_q, lsp2_q, M);

    return;
}
