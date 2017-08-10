/***************************************************************************
 *
 *  FILE NAME:    cod_12k2.c
 *
 *  FUNCTIONS DEFINED IN THIS FILE:
 *                   Coder_12k2  and  Init_Coder_12k2
 *
 *
 *  Init_Coder_12k2(void):
 *      Initialization of variables for the coder section.
 *
 *  Coder_12k2(Word16 ana[], Word16 synth[]):
 *      Speech encoder routine operating on a frame basis.
 *

***************************************************************************/

#include "typedef.h"
#include "basic_op.h"
#include "sig_proc.h"
#include "count.h"
#include "codec.h"
#include "cnst.h"

#include "window2.tab"

#include "vad.h"
#include "dtx.h"

/*-----------------------------------------------------------*
 *    Coder constant parameters (defined in "cnst.h")        *
 *-----------------------------------------------------------*
 *   L_WINDOW    : LPC analysis window size                  *
 *   L_FRAME     : Frame size                                *
 *   L_FRAME_BY2 : Half the frame size                       *
 *   L_SUBFR     : Sub-frame size                            *
 *   M           : LPC order                                 *
 *   MP1         : LPC order+1                               *
 *   L_TOTAL     : Total size of speech buffer               *
 *   PIT_MIN     : Minimum pitch lag                         *
 *   PIT_MAX     : Maximum pitch lag                         *
 *   L_INTERPOL  : Length of filter for interpolation        *
 *-----------------------------------------------------------*/

/*--------------------------------------------------------*
 *         Static memory allocation.                      *
 *--------------------------------------------------------*/

 /* Speech vector */

static Word16 old_speech[L_TOTAL];
static Word16 *speech, *p_window, *p_window_mid;
Word16 *new_speech;             /* Global variable */

 /* Weight speech vector */

static Word16 old_wsp[L_FRAME + PIT_MAX];
static Word16 *wsp;

 /* Excitation vector */

static Word16 old_exc[L_FRAME + PIT_MAX + L_INTERPOL];
static Word16 *exc;

 /* Zero vector */

static Word16 ai_zero[L_SUBFR + MP1];
static Word16 *zero;

 /* Impulse response vector */

static Word16 *h1;
static Word16 hvec[L_SUBFR * 2];

 /* Spectral expansion factors */

static const Word16 F_gamma1[M] =
{
    29491, 26542, 23888, 21499, 19349,
    17414, 15672, 14105, 12694, 11425
};
static const Word16 F_gamma2[M] =
{
    19661, 11797, 7078, 4247, 2548,
    1529, 917, 550, 330, 198
};

 /* Lsp (Line spectral pairs) */

static Word16 lsp_old[M];
static Word16 lsp_old_q[M];

 /* Filter's memory */

static Word16 mem_syn[M], mem_w0[M], mem_w[M];
static Word16 mem_err[M + L_SUBFR], *error;

/***************************************************************************
 *  FUNCTION:   Init_Coder_12k2
 *
 *  PURPOSE:   Initialization of variables for the coder section.
 *
 *  DESCRIPTION:
 *       - initilize pointers to speech buffer
 *       - initialize static  pointers
 *       - set static vectors to zero
 *
 ***************************************************************************/

void Init_Coder_12k2 (void)
{

/*--------------------------------------------------------------------------*
 *          Initialize pointers to speech vector.                           *
 *--------------------------------------------------------------------------*/

    new_speech = old_speech + L_TOTAL - L_FRAME;/* New speech     */
    speech = new_speech;                        /* Present frame  */
    p_window = old_speech + L_TOTAL - L_WINDOW; /* For LPC window */
    p_window_mid = p_window;                    /* For LPC window */

    /* Initialize static pointers */

    wsp = old_wsp + PIT_MAX;
    exc = old_exc + PIT_MAX + L_INTERPOL;
    zero = ai_zero + MP1;
    error = mem_err + M;
    h1 = &hvec[L_SUBFR];

    /* Static vectors to zero */

    Set_zero (old_speech, L_TOTAL);
    Set_zero (old_exc, PIT_MAX + L_INTERPOL);
    Set_zero (old_wsp, PIT_MAX);
    Set_zero (mem_syn, M);
    Set_zero (mem_w, M);
    Set_zero (mem_w0, M);
    Set_zero (mem_err, M);
    Set_zero (zero, L_SUBFR);
    Set_zero (hvec, L_SUBFR);   /* set to zero "h1[-L_SUBFR..-1]" */

    /* Initialize lsp_old [] */

    lsp_old[0] = 30000;
    lsp_old[1] = 26000;
    lsp_old[2] = 21000;
    lsp_old[3] = 15000;
    lsp_old[4] = 8000;
    lsp_old[5] = 0;
    lsp_old[6] = -8000;
    lsp_old[7] = -15000;
    lsp_old[8] = -21000;
    lsp_old[9] = -26000;

    /* Initialize lsp_old_q[] */

    Copy (lsp_old, lsp_old_q, M);

    return;
}

/***************************************************************************
 *   FUNCTION:   Coder_12k2
 *
 *   PURPOSE:  Principle encoder routine.
 *
 *   DESCRIPTION: This function is called every 20 ms speech frame,
 *       operating on the newly read 160 speech samples. It performs the
 *       principle encoding functions to produce the set of encoded parameters
 *       which include the LSP, adaptive codebook, and fixed codebook
 *       quantization indices (addresses and gains).
 *
 *   INPUTS:
 *       No input arguments are passed to this function. However, before
 *       calling this function, 160 new speech data samples should be copied to
 *       the vector new_speech[]. This is a global pointer which is declared in
 *       this file (it points to the end of speech buffer minus 160).
 *
 *   OUTPUTS:
 *
 *       ana[]:     vector of analysis parameters.
 *       synth[]:   Local synthesis speech (for debugging purposes)
 *
 ***************************************************************************/

void Coder_12k2 (
    Word16 ana[],    /* output  : Analysis parameters */
    Word16 synth[]   /* output  : Local synthesis     */
)
{
    /* LPC coefficients */

    Word16 r_l[MP1], r_h[MP1];      /* Autocorrelations lo and hi           */
    Word16 A_t[(MP1) * 4];          /* A(z) unquantized for the 4 subframes */
    Word16 Aq_t[(MP1) * 4];         /* A(z)   quantized for the 4 subframes */
    Word16 Ap1[MP1];                /* A(z) with spectral expansion         */
    Word16 Ap2[MP1];                /* A(z) with spectral expansion         */
    Word16 *A, *Aq;                 /* Pointer on A_t and Aq_t              */
    Word16 lsp_new[M], lsp_new_q[M];/* LSPs at 4th subframe                 */
    Word16 lsp_mid[M], lsp_mid_q[M];/* LSPs at 2nd subframe                 */

    /* Other vectors */

    Word16 xn[L_SUBFR];            /* Target vector for pitch search        */
    Word16 xn2[L_SUBFR];           /* Target vector for codebook search     */
    Word16 res2[L_SUBFR];          /* Long term prediction residual         */
    Word16 code[L_SUBFR];          /* Fixed codebook excitation             */
    Word16 y1[L_SUBFR];            /* Filtered adaptive excitation          */
    Word16 y2[L_SUBFR];            /* Filtered fixed codebook excitation    */

    /* Scalars */

    Word16 i, j, k, i_subfr;
    Word16 T_op, T0, T0_min, T0_max, T0_frac;
    Word16 gain_pit, gain_code, pit_flag, pit_sharp;
    Word16 temp;
    Word32 L_temp;

    Word16 scal_acf, VAD_flag, lags[2], rc[4];

    extern Word16 ptch;
    extern Word16 txdtx_ctrl, CN_excitation_gain;
    extern Word32 L_pn_seed_tx;
    extern Word16 dtx_mode;

    /*----------------------------------------------------------------------*
     *  - Perform LPC analysis: (twice per frame)                           *
     *       * autocorrelation + lag windowing                              *
     *       * Levinson-Durbin algorithm to find a[]                        *
     *       * convert a[] to lsp[]                                         *
     *       * quantize and code the LSPs                                   *
     *       * find the interpolated LSPs and convert to a[] for all        *
     *         subframes (both quantized and unquantized)                   *
     *----------------------------------------------------------------------*/

    /* LP analysis centered at 2nd subframe */


    scal_acf = Autocorr (p_window_mid, M, r_h, r_l, window_160_80);
                                /* Autocorrelations */

#if (WMOPS)
    fwc ();                     /* function worst case */
#endif

    Lag_window (M, r_h, r_l);   /* Lag windowing    */

#if (WMOPS)
    fwc ();                     /* function worst case */
#endif

    Levinson (r_h, r_l, &A_t[MP1], rc); /* Levinson-Durbin  */

#if (WMOPS)
    fwc ();                     /* function worst case */
#endif

    Az_lsp (&A_t[MP1], lsp_mid, lsp_old); /* From A(z) to lsp */

#if (WMOPS)
    fwc ();                     /* function worst case */
#endif

    /* LP analysis centered at 4th subframe */

    /* Autocorrelations */
    scal_acf = Autocorr (p_window, M, r_h, r_l, window_232_8);

#if (WMOPS)
    fwc ();                     /* function worst case */
#endif

    Lag_window (M, r_h, r_l);   /* Lag windowing    */

#if (WMOPS)
    fwc ();                     /* function worst case */
#endif

    Levinson (r_h, r_l, &A_t[MP1 * 3], rc); /* Levinson-Durbin  */

#if (WMOPS)
    fwc ();                     /* function worst case */
#endif

    Az_lsp (&A_t[MP1 * 3], lsp_new, lsp_mid); /* From A(z) to lsp */

#if (WMOPS)
    fwc ();                     /* function worst case */
#endif

    if (dtx_mode == 1)
    {
        /* DTX enabled, make voice activity decision */
        VAD_flag = vad_computation (r_h, r_l, scal_acf, rc, ptch);
                                                                move16 (); 

        tx_dtx (VAD_flag, &txdtx_ctrl); /* TX DTX handler */
    }
    else
    {
        /* DTX disabled, active speech in every frame */
        VAD_flag = 1;
        txdtx_ctrl = TX_VAD_FLAG | TX_SP_FLAG;
    }

    /* LSP quantization (lsp_mid[] and lsp_new[] jointly quantized) */

    Q_plsf_5 (lsp_mid, lsp_new, lsp_mid_q, lsp_new_q, ana, txdtx_ctrl);

#if (WMOPS)
    fwc ();                     /* function worst case */
#endif
    ana += 5;                                                   move16 (); 

    /*--------------------------------------------------------------------*
     * Find interpolated LPC parameters in all subframes (both quantized  *
     * and unquantized).                                                  *
     * The interpolated parameters are in array A_t[] of size (M+1)*4     *
     * and the quantized interpolated parameters are in array Aq_t[]      *
     *--------------------------------------------------------------------*/

    Int_lpc2 (lsp_old, lsp_mid, lsp_new, A_t);

#if (WMOPS)
    fwc ();                     /* function worst case */
#endif

    test (); logic16 (); 
    if ((txdtx_ctrl & TX_SP_FLAG) != 0)
    {
        Int_lpc (lsp_old_q, lsp_mid_q, lsp_new_q, Aq_t);

        /* update the LSPs for the next frame */
        for (i = 0; i < M; i++)
        {
            lsp_old[i] = lsp_new[i];                            move16 (); 
            lsp_old_q[i] = lsp_new_q[i];                        move16 (); 
        }
    }
    else
    {
        /* Use unquantized LPC parameters in case of no speech activity */
        for (i = 0; i < MP1; i++)
        {
            Aq_t[i] = A_t[i];                                   move16 (); 
            Aq_t[i + MP1] = A_t[i + MP1];                       move16 (); 
            Aq_t[i + MP1 * 2] = A_t[i + MP1 * 2];               move16 (); 
            Aq_t[i + MP1 * 3] = A_t[i + MP1 * 3];               move16 (); 
        }

        /* update the LSPs for the next frame */
        for (i = 0; i < M; i++)
        {
            lsp_old[i] = lsp_new[i];                            move16 (); 
            lsp_old_q[i] = lsp_new[i];                          move16 (); 
        }
    }

#if (WMOPS)
    fwc ();                     /* function worst case */
#endif

    /*----------------------------------------------------------------------*
     * - Find the weighted input speech wsp[] for the whole speech frame    *
     * - Find the open-loop pitch delay for first 2 subframes               *
     * - Set the range for searching closed-loop pitch in 1st subframe      *
     * - Find the open-loop pitch delay for last 2 subframes                *
     *----------------------------------------------------------------------*/

    A = A_t;                                                    move16 (); 
    for (i = 0; i < L_FRAME; i += L_SUBFR)
    {
        Weight_Ai (A, F_gamma1, Ap1);

#if (WMOPS)
        fwc ();                 /* function worst case */
#endif

        Weight_Ai (A, F_gamma2, Ap2);

#if (WMOPS)
        fwc ();                 /* function worst case */
#endif

        Residu (Ap1, &speech[i], &wsp[i], L_SUBFR);

#if (WMOPS)
        fwc ();                 /* function worst case */
#endif

        Syn_filt (Ap2, &wsp[i], &wsp[i], L_SUBFR, mem_w, 1);

#if (WMOPS)
        fwc ();                 /* function worst case */
#endif

        A += MP1;                                               move16 (); 
    }

    /* Find open loop pitch lag for first two subframes */

    T_op = Pitch_ol (wsp, PIT_MIN, PIT_MAX, L_FRAME_BY2);       move16 (); 

#if (WMOPS)
    fwc ();                     /* function worst case */
#endif

    lags[0] = T_op;                                             move16 (); 

    test (); logic16 (); 
    if ((txdtx_ctrl & TX_SP_FLAG) != 0)
    {
        /* Range for closed loop pitch search in 1st subframe */

        T0_min = sub (T_op, 3);
        test (); 
        if (sub (T0_min, PIT_MIN) < 0)
        {
            T0_min = PIT_MIN;                                   move16 (); 
        }
        T0_max = add (T0_min, 6);
        test (); 
        if (sub (T0_max, PIT_MAX) > 0)
        {
            T0_max = PIT_MAX;                                   move16 (); 
            T0_min = sub (T0_max, 6);
        }
#if (WMOPS)
        fwc ();                 /* function worst case */
#endif
    }
    /* Find open loop pitch lag for last two subframes */

    T_op = Pitch_ol (&wsp[L_FRAME_BY2], PIT_MIN, PIT_MAX, L_FRAME_BY2);
                                                                move16 (); 

#if (WMOPS)
    fwc ();                     /* function worst case */
#endif

    if (dtx_mode == 1)
    {
        lags[1] = T_op;                                         move16 (); 
        periodicity_update (lags, &ptch);
    }
    /*----------------------------------------------------------------------*
     *          Loop for every subframe in the analysis frame               *
     *----------------------------------------------------------------------*
     *  To find the pitch and innovation parameters. The subframe size is   *
     *  L_SUBFR and the loop is repeated L_FRAME/L_SUBFR times.             *
     *     - find the weighted LPC coefficients                             *
     *     - find the LPC residual signal res[]                             *
     *     - compute the target signal for pitch search                     *
     *     - compute impulse response of weighted synthesis filter (h1[])   *
     *     - find the closed-loop pitch parameters                          *
     *     - encode the pitch delay                                         *
     *     - update the impulse response h1[] by including pitch            *
     *     - find target vector for codebook search                         *
     *     - codebook search                                                *
     *     - encode codebook address                                        *
     *     - VQ of pitch and codebook gains                                 *
     *     - find synthesis speech                                          *
     *     - update states of weighting filter                              *
     *----------------------------------------------------------------------*/

    /* pointer to interpolated LPC parameters          */
    A = A_t;                                                    move16 ();
    /* pointer to interpolated quantized LPC parameters */    
    Aq = Aq_t;                                                  move16 (); 

    for (i_subfr = 0; i_subfr < L_FRAME; i_subfr += L_SUBFR)
    {

        test (); logic16 (); 
        if ((txdtx_ctrl & TX_SP_FLAG) != 0)
        {

            /*---------------------------------------------------------------*
             * Find the weighted LPC coefficients for the weighting filter.  *
             *---------------------------------------------------------------*/

            Weight_Ai (A, F_gamma1, Ap1);

#if (WMOPS)
            fwc ();             /* function worst case */
#endif

            Weight_Ai (A, F_gamma2, Ap2);

#if (WMOPS)
            fwc ();             /* function worst case */
#endif

            /*---------------------------------------------------------------*
             * Compute impulse response, h1[], of weighted synthesis filter  *
             *---------------------------------------------------------------*/

            for (i = 0; i <= M; i++)
            {
                ai_zero[i] = Ap1[i];                            move16 (); 
            }

            Syn_filt (Aq, ai_zero, h1, L_SUBFR, zero, 0);

#if (WMOPS)
            fwc ();             /* function worst case */
#endif

            Syn_filt (Ap2, h1, h1, L_SUBFR, zero, 0);

#if (WMOPS)
            fwc ();             /* function worst case */
#endif

        }
        /*---------------------------------------------------------------*
         *          Find the target vector for pitch search:             *
         *---------------------------------------------------------------*/

        Residu (Aq, &speech[i_subfr], res2, L_SUBFR);   /* LPC residual */

#if (WMOPS)
        fwc ();                 /* function worst case */
#endif

        test (); logic16 (); 
        if ((txdtx_ctrl & TX_SP_FLAG) == 0)
        {
            /* Compute comfort noise excitation gain based on
            LP residual energy */

            CN_excitation_gain = compute_CN_excitation_gain (res2);
            move16 (); 
        }
        else
        {
            Copy (res2, &exc[i_subfr], L_SUBFR);

#if (WMOPS)
            fwc ();             /* function worst case */
#endif

            Syn_filt (Aq, &exc[i_subfr], error, L_SUBFR, mem_err, 0);

#if (WMOPS)
            fwc ();             /* function worst case */
#endif

            Residu (Ap1, error, xn, L_SUBFR);

#if (WMOPS)
            fwc ();             /* function worst case */
#endif

            Syn_filt (Ap2, xn, xn, L_SUBFR, mem_w0, 0); /* target signal xn[]*/

#if (WMOPS)
            fwc ();             /* function worst case */
#endif

            /*--------------------------------------------------------------*
             *                 Closed-loop fractional pitch search          *
             *--------------------------------------------------------------*/

            /* flag for first and 3th subframe */            
            pit_flag = i_subfr;                                 move16 (); 
            test (); 
            /* set t0_min and t0_max for 3th subf.*/
            if (sub (i_subfr, L_FRAME_BY2) == 0)
            {
                T0_min = sub (T_op, 3);

                test (); 
                if (sub (T0_min, PIT_MIN) < 0)
                {
                    T0_min = PIT_MIN;                           move16 (); 
                }
                T0_max = add (T0_min, 6);
                test (); 
                if (sub (T0_max, PIT_MAX) > 0)
                {
                    T0_max = PIT_MAX;                           move16 (); 
                    T0_min = sub (T0_max, 6);
                }
                pit_flag = 0;                                   move16 (); 
            }
#if (WMOPS)
            fwc ();             /* function worst case */
#endif

            T0 = Pitch_fr6 (&exc[i_subfr], xn, h1, L_SUBFR, T0_min, T0_max,
                            pit_flag, &T0_frac);                move16 (); 

#if (WMOPS)
            fwc ();             /* function worst case */
#endif

            *ana = Enc_lag6 (T0, &T0_frac, &T0_min, &T0_max, PIT_MIN,
                             PIT_MAX, pit_flag);
            move16 (); 

#if (WMOPS)
            fwc ();             /* function worst case */
#endif
        }
        ana++;
        /* Incrementation of ana is done here to work also
        when no speech activity is present */

        test (); logic16 (); 

        if ((txdtx_ctrl & TX_SP_FLAG) != 0)
        {

            /*---------------------------------------------------------------*
             * - find unity gain pitch excitation (adaptive codebook entry)  *
             *   with fractional interpolation.                              *
             * - find filtered pitch exc. y1[]=exc[] convolved with h1[]     *
             * - compute pitch gain and limit between 0 and 1.2              *
             * - update target vector for codebook search                    *
             * - find LTP residual.                                          *
             *---------------------------------------------------------------*/

            Pred_lt_6 (&exc[i_subfr], T0, T0_frac, L_SUBFR);

#if (WMOPS)
            fwc ();             /* function worst case */
#endif

            Convolve (&exc[i_subfr], h1, y1, L_SUBFR);

#if (WMOPS)
            fwc ();             /* function worst case */
#endif

            gain_pit = G_pitch (xn, y1, L_SUBFR);      move16 (); 

#if (WMOPS)
            fwc ();             /* function worst case */
#endif

            *ana = q_gain_pitch (&gain_pit);                    move16 (); 

#if (WMOPS)
            fwc ();             /* function worst case */
#endif

        }
        else
        {
            gain_pit = 0;                                       move16 (); 
        }

        ana++;                  /* Incrementation of ana is done here to work
                                   also when no speech activity is present */

        test (); logic16 (); 

        if ((txdtx_ctrl & TX_SP_FLAG) != 0)
        {
            /* xn2[i]   = xn[i] - y1[i] * gain_pit  */
            /* res2[i] -= exc[i+i_subfr] * gain_pit */

            for (i = 0; i < L_SUBFR; i++)
            {
                L_temp = L_mult (y1[i], gain_pit);
                L_temp = L_shl (L_temp, 3);
                xn2[i] = sub (xn[i], extract_h (L_temp));       move16 (); 

                L_temp = L_mult (exc[i + i_subfr], gain_pit);
                L_temp = L_shl (L_temp, 3);
                res2[i] = sub (res2[i], extract_h (L_temp));    move16 (); 
            }

#if (WMOPS)
            fwc ();             /* function worst case */
#endif

            /*-------------------------------------------------------------*
             * - include pitch contribution into impulse resp. h1[]        *
             *-------------------------------------------------------------*/

            /* pit_sharp = gain_pit;                   */
            /* if (pit_sharp > 1.0) pit_sharp = 1.0;   */

            pit_sharp = shl (gain_pit, 3);

            for (i = T0; i < L_SUBFR; i++)
            {
                temp = mult (h1[i - T0], pit_sharp);
                h1[i] = add (h1[i], temp);                      move16 (); 
            }

#if (WMOPS)
            fwc ();             /* function worst case */
#endif

            /*--------------------------------------------------------------*
             * - Innovative codebook search (find index and gain)           *
             *--------------------------------------------------------------*/

            code_10i40_35bits (xn2, res2, h1, code, y2, ana);

#if (WMOPS)
            fwc ();             /* function worst case */
#endif

        }
        else
        {
            build_CN_code (code, &L_pn_seed_tx);
        }
        ana += 10;                                              move16 (); 

        test (); logic16 (); 
        if ((txdtx_ctrl & TX_SP_FLAG) != 0)
        {

            /*-------------------------------------------------------*
             * - Add the pitch contribution to code[].               *
             *-------------------------------------------------------*/

            for (i = T0; i < L_SUBFR; i++)
            {
                temp = mult (code[i - T0], pit_sharp);
                code[i] = add (code[i], temp);                  move16 (); 
            }

#if (WMOPS)
            fwc ();             /* function worst case */
#endif

            /*------------------------------------------------------*
             * - Quantization of fixed codebook gain.               *
             *------------------------------------------------------*/

            gain_code = G_code (xn2, y2);                       move16 (); 

#if (WMOPS)
            fwc ();             /* function worst case */
#endif

        }
        *ana++ = q_gain_code (code, L_SUBFR, &gain_code, txdtx_ctrl, i_subfr);
        move16 (); 

#if (WMOPS)
        fwc ();                 /* function worst case */
#endif

        /*------------------------------------------------------*
         * - Find the total excitation                          *
         * - find synthesis speech corresponding to exc[]       *
         * - update filter memories for finding the target      *
         *   vector in the next subframe                        *
         *   (update mem_err[] and mem_w0[])                    *
         *------------------------------------------------------*/

        for (i = 0; i < L_SUBFR; i++)
        {
            /* exc[i] = gain_pit*exc[i] + gain_code*code[i]; */

            L_temp = L_mult (exc[i + i_subfr], gain_pit);
            L_temp = L_mac (L_temp, code[i], gain_code);
            L_temp = L_shl (L_temp, 3);
            exc[i + i_subfr] = round_z (L_temp);                  move16 (); 
        }

#if (WMOPS)
        fwc ();                 /* function worst case */
#endif

        Syn_filt (Aq, &exc[i_subfr], &synth[i_subfr], L_SUBFR, mem_syn, 1);

#if (WMOPS)
        fwc ();                 /* function worst case */
#endif

        test (); logic16 (); 
        if ((txdtx_ctrl & TX_SP_FLAG) != 0)
        {

            for (i = L_SUBFR - M, j = 0; i < L_SUBFR; i++, j++)
            {
                mem_err[j] = sub (speech[i_subfr + i], synth[i_subfr + i]);
                                                                move16 (); 
                temp = extract_h (L_shl (L_mult (y1[i], gain_pit), 3));
                k = extract_h (L_shl (L_mult (y2[i], gain_code), 5));
                mem_w0[j] = sub (xn[i], add (temp, k));         move16 (); 
            }
        }
        else
        {
            for (j = 0; j < M; j++)
            {
                mem_err[j] = 0;                                 move16 (); 
                mem_w0[j] = 0;                                  move16 (); 
            }
        }

#if (WMOPS)
        fwc ();                 /* function worst case */
#endif
        /* interpolated LPC parameters for next subframe */
        A += MP1;                                               move16 (); 
        Aq += MP1;                                              move16 (); 
    }

    /*--------------------------------------------------*
     * Update signal for next frame.                    *
     * -> shift to the left by L_FRAME:                 *
     *     speech[], wsp[] and  exc[]                   *
     *--------------------------------------------------*/

    Copy (&old_speech[L_FRAME], &old_speech[0], L_TOTAL - L_FRAME);

#if (WMOPS)
    fwc ();                     /* function worst case */
#endif

    Copy (&old_wsp[L_FRAME], &old_wsp[0], PIT_MAX);

#if (WMOPS)
    fwc ();                     /* function worst case */
#endif

    Copy (&old_exc[L_FRAME], &old_exc[0], PIT_MAX + L_INTERPOL);

#if (WMOPS)
    fwc ();                     /* function worst case */
#endif

    return;
}
