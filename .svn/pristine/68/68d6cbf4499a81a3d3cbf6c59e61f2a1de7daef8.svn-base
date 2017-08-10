/*************************************************************************
 *   FUNCTION:  Q_plsf_5()
 *
 *   PURPOSE:  Quantization of 2 sets of LSF parameters using 1st order MA
 *             prediction and split by 5 matrix quantization (split-MQ)
 *
 *   DESCRIPTION:
 *
 *        p[i] = pred_factor*past_r2q[i];   i=0,...,m-1
 *        r1[i]= lsf1[i] - p[i];      i=0,...,m-1
 *        r2[i]= lsf2[i] - p[i];      i=0,...,m-1
 *   where:
 *        lsf1[i]           1st mean-removed LSF vector.
 *        lsf2[i]           2nd mean-removed LSF vector.
 *        r1[i]             1st residual prediction vector.
 *        r2[i]             2nd residual prediction vector.
 *        past_r2q[i]       Past quantized residual (2nd vector).
 *
 *   The residual vectors r1[i] and r2[i] are jointly quantized using
 *   split-MQ with 5 codebooks. Each 4th dimension submatrix contains 2
 *   elements from each residual vector. The 5 submatrices are as follows:
 *     {r1[0], r1[1], r2[0], r2[1]};  {r1[2], r1[3], r2[2], r2[3]};
 *     {r1[4], r1[5], r2[4], r2[5]};  {r1[6], r1[7], r2[6], r2[7]};
 *                    {r1[8], r1[9], r2[8], r2[9]};
 *
 *************************************************************************/
#include "../common/cnst.h"
#include "../common/typedef.h"
#include "../common/basic_op.h"
#include "../common/sig_proc.h"

#include "dtx_cod.h"

/* Locals functions */

void Lsf_wt (
    Word16 *lsf,       /* input : LSF vector                    */
     Word16 *wf2       /* output: square of weighting factors   */
);

Word16 Vq_subvec (     /* output: return quantization index     */
    Word16 *lsf_r1,    /* input : 1st LSF residual vector       */
    Word16 *lsf_r2,    /* input : and LSF residual vector       */
    const Word16 *dico,/* input : quantization codebook         */
    Word16 *wf1,       /* input : 1st LSF weighting factors     */
    Word16 *wf2,       /* input : 2nd LSF weighting factors     */
    Word16 dico_size   /* input : size of quantization codebook */
);
Word16 Vq_subvec_s (   /* output: return quantization index     */
    Word16 *lsf_r1,    /* input : 1st LSF residual vector       */
    Word16 *lsf_r2,    /* input : and LSF residual vector       */
    const Word16 *dico,/* input : quantization codebook         */
    Word16 *wf1,       /* input : 1st LSF weighting factors     */
    Word16 *wf2,       /* input : 2nd LSF weighting factors     */
    Word16 dico_size   /* input : size of quantization codebook */
);
/* M  ->order of linear prediction filter                      */
/* LSF_GAP  -> Minimum distance between LSF after quantization */
/*             50 Hz = 205                                     */
/* PRED_FAC -> Predcition factor                               */

#define M         10
#define LSF_GAP   205
#define PRED_FAC  21299

#include "../common/q_plsf_5.tab"         /* Codebooks of LSF prediction residual */

//extern Word16 lsf_old_tx[DTX_HANGOVER][M];

void Q_plsf_5 (
    Word16 *lsp1,      /* input : 1st LSP vector                     */
    Word16 *lsp2,      /* input : 2nd LSP vector                     */
    Word16 *lsp1_q,    /* output: quantized 1st LSP vector           */
    Word16 *lsp2_q,    /* output: quantized 2nd LSP vector           */
    Word16 *indice,    /* output: quantization indices of 5 matrices */
    Word16 txdtx_ctrl, /* input : tx dtx control word                */
	Word16 past_r2_q[], /* input/output: Past quantized prediction error */
	Word16 lsf_old_tx[][M] // input/output: Comfort noise LSF averaging buffer
)
{

    Word16 i;
    Word16 lsf1[M], lsf2[M], wf1[M], wf2[M], lsf_p[M], lsf_r1[M], lsf_r2[M];
    Word16 lsf1_q[M], lsf2_q[M];
    Word16 lsf_aver[M];
    static Word16 lsf_p_CN[M];

    /* convert LSFs to normalize frequency domain 0..16384  */

    Lsp_lsf (lsp1, lsf1, M);
    Lsp_lsf (lsp2, lsf2, M);

    /* Update LSF CN quantizer "memory" */

        
    if ((txdtx_ctrl & TX_SP_FLAG) == 0
        && (txdtx_ctrl & TX_PREV_HANGOVER_ACTIVE) != 0)
    {
        update_lsf_p_CN (lsf_old_tx, lsf_p_CN);
    }
      
    if ((txdtx_ctrl & TX_SID_UPDATE) != 0)
    {
        /* New SID frame is to be sent:
        Compute average of the current LSFs and the LSFs in the history */

        aver_lsf_history (lsf_old_tx, lsf1, lsf2, lsf_aver);
    }
    /* Update LSF history with unquantized LSFs when no speech activity
    is present */

      
    if ((txdtx_ctrl & TX_SP_FLAG) == 0)
    {
        update_lsf_history (lsf1, lsf2, lsf_old_tx);
    }
      
    if ((txdtx_ctrl & TX_SID_UPDATE) != 0)
    {
        /* Compute LSF weighting factors for lsf2, using averaged LSFs */
        /* Set LSF weighting factors for lsf1 to zero */
        /* Replace lsf1 and lsf2 by the averaged LSFs */

        Lsf_wt (lsf_aver, wf2);
        for (i = 0; i < M; i++)
        {
            wf1[i] = 0;                                  
            lsf1[i] = lsf_aver[i];                       
            lsf2[i] = lsf_aver[i];                       
        }
    }
    else
    {
        /* Compute LSF weighting factors */

        Lsf_wt (lsf1, wf1);
        Lsf_wt (lsf2, wf2);
    }

    /* Compute predicted LSF and prediction error */

      
    if ((txdtx_ctrl & TX_SP_FLAG) != 0)
    {
        for (i = 0; i < M; i++)
        {
            lsf_p[i] = add (mean_lsf[i], mult (past_r2_q[i], PRED_FAC));
                                                         
            lsf_r1[i] = sub (lsf1[i], lsf_p[i]);         
            lsf_r2[i] = sub (lsf2[i], lsf_p[i]);         
        }
    }
    else
    {
        for (i = 0; i < M; i++)
        {
            lsf_r1[i] = sub (lsf1[i], lsf_p_CN[i]);      
            lsf_r2[i] = sub (lsf2[i], lsf_p_CN[i]);      
        }
    }

    /*---- Split-VQ of prediction error ----*/

    indice[0] = Vq_subvec (&lsf_r1[0], &lsf_r2[0], dico1_lsf,
                           &wf1[0], &wf2[0], DICO1_SIZE);
                                                         

    indice[1] = Vq_subvec (&lsf_r1[2], &lsf_r2[2], dico2_lsf,
                           &wf1[2], &wf2[2], DICO2_SIZE);
                                                         

    indice[2] = Vq_subvec_s (&lsf_r1[4], &lsf_r2[4], dico3_lsf,
                             &wf1[4], &wf2[4], DICO3_SIZE);
                                                         

    indice[3] = Vq_subvec (&lsf_r1[6], &lsf_r2[6], dico4_lsf,
                           &wf1[6], &wf2[6], DICO4_SIZE);
                                                         

    indice[4] = Vq_subvec (&lsf_r1[8], &lsf_r2[8], dico5_lsf,
                           &wf1[8], &wf2[8], DICO5_SIZE);
                                                         

    /* Compute quantized LSFs and update the past quantized residual */
    /* In case of no speech activity, skip computing the quantized LSFs,
       and set past_r2_q to zero (initial value) */

      
    if ((txdtx_ctrl & TX_SP_FLAG) != 0)
    {
        for (i = 0; i < M; i++)
        {
            lsf1_q[i] = add (lsf_r1[i], lsf_p[i]);       
            lsf2_q[i] = add (lsf_r2[i], lsf_p[i]);       
            past_r2_q[i] = lsf_r2[i];                    
        }

        /* verification that LSFs has minimum distance of LSF_GAP */

        Reorder_lsf (lsf1_q, LSF_GAP, M);
        Reorder_lsf (lsf2_q, LSF_GAP, M);

        /* Update LSF history with quantized LSFs
        when hangover period is active */

          
        if ((txdtx_ctrl & TX_HANGOVER_ACTIVE) != 0)
        {
            update_lsf_history (lsf1_q, lsf2_q, lsf_old_tx);
        }
        /*  convert LSFs to the cosine domain */

        Lsf_lsp (lsf1_q, lsp1_q, M);
        Lsf_lsp (lsf2_q, lsp2_q, M);
    }
    else
    {
        for (i = 0; i < M; i++)
        {
            past_r2_q[i] = 0;                            
        }
    }

    return;
}

/* Quantization of a 4 dimensional subvector */

Word16 Vq_subvec (      /* output: return quantization index     */
    Word16 *lsf_r1,     /* input : 1st LSF residual vector       */
    Word16 *lsf_r2,     /* input : and LSF residual vector       */
    const Word16 *dico, /* input : quantization codebook         */
    Word16 *wf1,        /* input : 1st LSF weighting factors     */
    Word16 *wf2,        /* input : 2nd LSF weighting factors     */
    Word16 dico_size    /* input : size of quantization codebook */
)
{

    Word16 i, index, temp;
    const Word16 *p_dico;
    Word32 dist_min, dist;

    dist_min = MAX_32;                                   
    p_dico = dico;                                       

    for (i = 0; i < dico_size; i++)
    {
        temp = sub (lsf_r1[0], *p_dico++);
        temp = mult (wf1[0], temp);
        dist = L_mult (temp, temp);

        temp = sub (lsf_r1[1], *p_dico++);
        temp = mult (wf1[1], temp);
        dist = L_mac (dist, temp, temp);

        temp = sub (lsf_r2[0], *p_dico++);
        temp = mult (wf2[0], temp);
        dist = L_mac (dist, temp, temp);

        temp = sub (lsf_r2[1], *p_dico++);
        temp = mult (wf2[1], temp);
        dist = L_mac (dist, temp, temp);

         
        if (L_sub (dist, dist_min) < (Word32) 0)
        {
            dist_min = dist;                             
            index = i;                                   
        }
    }

    /* Reading the selected vector */

    p_dico = &dico[shl (index, 2)];                      
    lsf_r1[0] = *p_dico++;                               
    lsf_r1[1] = *p_dico++;                               
    lsf_r2[0] = *p_dico++;                               
    lsf_r2[1] = *p_dico++;                               

    return index;

}

/* Quantization of a 4 dimensional subvector with a signed codebook */

Word16 Vq_subvec_s (    /* output: return quantization index     */
    Word16 *lsf_r1,     /* input : 1st LSF residual vector       */
    Word16 *lsf_r2,     /* input : and LSF residual vector       */
    const Word16 *dico, /* input : quantization codebook         */
    Word16 *wf1,        /* input : 1st LSF weighting factors     */
    Word16 *wf2,        /* input : 2nd LSF weighting factors     */
    Word16 dico_size)   /* input : size of quantization codebook */
{

    Word16 i, index, sign, temp;
    const Word16 *p_dico;
    Word32 dist_min, dist;

    dist_min = MAX_32;                                   
    p_dico = dico;                                       

    for (i = 0; i < dico_size; i++)
    {
        /* test positive */

        temp = sub (lsf_r1[0], *p_dico++);
        temp = mult (wf1[0], temp);
        dist = L_mult (temp, temp);

        temp = sub (lsf_r1[1], *p_dico++);
        temp = mult (wf1[1], temp);
        dist = L_mac (dist, temp, temp);

        temp = sub (lsf_r2[0], *p_dico++);
        temp = mult (wf2[0], temp);
        dist = L_mac (dist, temp, temp);

        temp = sub (lsf_r2[1], *p_dico++);
        temp = mult (wf2[1], temp);
        dist = L_mac (dist, temp, temp);

         
        if (L_sub (dist, dist_min) < (Word32) 0)
        {
            dist_min = dist;                             
            index = i;                                   
            sign = 0;                                    
        }
        /* test negative */

        p_dico -= 4;                                     
        temp = add (lsf_r1[0], *p_dico++);
        temp = mult (wf1[0], temp);
        dist = L_mult (temp, temp);

        temp = add (lsf_r1[1], *p_dico++);
        temp = mult (wf1[1], temp);
        dist = L_mac (dist, temp, temp);

        temp = add (lsf_r2[0], *p_dico++);
        temp = mult (wf2[0], temp);
        dist = L_mac (dist, temp, temp);

        temp = add (lsf_r2[1], *p_dico++);
        temp = mult (wf2[1], temp);
        dist = L_mac (dist, temp, temp);

         
        if (L_sub (dist, dist_min) < (Word32) 0)
        {
            dist_min = dist;                             
            index = i;                                   
            sign = 1;                                    
        }
    }

    /* Reading the selected vector */

    p_dico = &dico[shl (index, 2)];                      
     
    if (sign == 0)
    {
        lsf_r1[0] = *p_dico++;                           
        lsf_r1[1] = *p_dico++;                           
        lsf_r2[0] = *p_dico++;                           
        lsf_r2[1] = *p_dico++;                           
    }
    else
    {
        lsf_r1[0] = negate (*p_dico++);                  
        lsf_r1[1] = negate (*p_dico++);                  
        lsf_r2[0] = negate (*p_dico++);                  
        lsf_r2[1] = negate (*p_dico++);                  
    }

    index = shl (index, 1);
    index = add (index, sign);

    return index;

}

/****************************************************
 * FUNCTION  Lsf_wt                                                         *
 *                                                                          *
 ****************************************************
 * Compute LSF weighting factors                                            *
 *                                                                          *
 *  d[i] = lsf[i+1] - lsf[i-1]                                              *
 *                                                                          *
 *  The weighting factors are approximated by two line segment.             *
 *                                                                          *
 *  First segment passes by the following 2 points:                         *
 *                                                                          *
 *     d[i] = 0Hz     wf[i] = 3.347                                         *
 *     d[i] = 450Hz   wf[i] = 1.8                                           *
 *                                                                          *
 *  Second segment passes by the following 2 points:                        *
 *                                                                          *
 *     d[i] = 450Hz   wf[i] = 1.8                                           *
 *     d[i] = 1500Hz  wf[i] = 1.0                                           *
 *                                                                          *
 *  if( d[i] < 450Hz )                                                      *
 *    wf[i] = 3.347 - ( (3.347-1.8) / (450-0)) *  d[i]                      *
 *  else                                                                    *
 *    wf[i] = 1.8 - ( (1.8-1.0) / (1500-450)) *  (d[i] - 450)               *
 *                                                                          *
 *                                                                          *
 *  if( d[i] < 1843)                                                        *
 *    wf[i] = 3427 - (28160*d[i])>>15                                       *
 *  else                                                                    *
 *    wf[i] = 1843 - (6242*(d[i]-1843))>>15                                 *
 *                                                                          *
 *--------------------------------------------------------------------------*/

void Lsf_wt (
    Word16 *lsf,         /* input : LSF vector                  */
    Word16 *wf)          /* output: square of weighting factors */
{

    Word16 temp;
    Word16 i;
    /* wf[0] = lsf[1] - 0  */
    wf[0] = lsf[1];                                      
    for (i = 1; i < 9; i++)
    {
        wf[i] = sub (lsf[i + 1], lsf[i - 1]);            
    }
    /* wf[9] = 0.5 - lsf[8] */    
    wf[9] = sub (16384, lsf[8]);      

    for (i = 0; i < 10; i++)
    {
        temp = sub (wf[i], 1843);
         
        if (temp < 0)
        {
            wf[i] = sub (3427, mult (wf[i], 28160));     
        }
        else
        {
            wf[i] = sub (1843, mult (temp, 6242));       
        }

        wf[i] = shl (wf[i], 3);  
    }
    return;
}
