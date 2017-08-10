/***************************************************************************
 *
 *  FILE NAME:    dec_12k2.c
 *
 *  FUNCTIONS DEFINED IN THIS FILE:
 *                   Init_Decoder_12k2   and  Decoder_12k2
 *
 *
 *  Init_Decoder_12k2():
 *      Initialization of variables for the decoder section.
 *
 *  Decoder_12k2():
 *      Speech decoder routine operating on a frame basis.
 *
 ***************************************************************************/

#include "typedef.h"
#include "basic_op.h"
#include "sig_proc.h"
#include "count.h"
#include "codec.h"
#include "cnst.h"

#include "dtx.h"

extern Word16 dtx_mode;

/*---------------------------------------------------------------*
 *   Decoder constant parameters (defined in "cnst.h")           *
 *---------------------------------------------------------------*
 *   L_FRAME     : Frame size.                                   *
 *   L_FRAME_BY2 : Half the frame size.                          *
 *   L_SUBFR     : Sub-frame size.                               *
 *   M           : LPC order.                                    *
 *   MP1         : LPC order+1                                   *
 *   PIT_MIN     : Minimum pitch lag.                            *
 *   PIT_MAX     : Maximum pitch lag.                            *
 *   L_INTERPOL  : Length of filter for interpolation            *
 *   PRM_SIZE    : size of vector containing analysis parameters *
 *---------------------------------------------------------------*/

/*--------------------------------------------------------*
 *         Static memory allocation.                      *
 *--------------------------------------------------------*/

 /* Excitation vector */

static Word16 old_exc[L_FRAME + PIT_MAX + L_INTERPOL];
static Word16 *exc;

 /* Lsp (Line spectral pairs) */

static Word16 lsp_old[M];

 /* Filter's memory */

static Word16 mem_syn[M];

 /* Memories for bad frame handling */

static Word16 prev_bf;
static Word16 state;

/***************************************************************************
 *
 *   FUNCTION:  Init_Decoder_12k2
 *
 *   PURPOSE: Initialization of variables for the decoder section.
 *
 ***************************************************************************/

void Init_Decoder_12k2 (void)
{
    /* Initialize static pointer */

    exc = old_exc + PIT_MAX + L_INTERPOL;

    /* Static vectors to zero */

    Set_zero (old_exc, PIT_MAX + L_INTERPOL);
    Set_zero (mem_syn, M);

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

    /* Initialize memories of bad frame handling */

    prev_bf = 0;
    state = 0;

    return;
}

/***************************************************************************
 *
 *   FUNCTION:  Decoder_12k2
 *
 *   PURPOSE:   Speech decoder routine.
 *
 ***************************************************************************/

void Decoder_12k2 (
    Word16 parm[], /* input : vector of synthesis parameters
                      parm[0] = bad frame indicator (bfi)       */
    Word16 synth[],/* output: synthesis speech                  */
    Word16 A_t[],  /* output: decoded LP filter in 4 subframes  */
    Word16 TAF,
    Word16 SID_flag
)
{

    /* LPC coefficients */
	Word16 *Az;                 /* Pointer on A_t */

	/* LSPs */
	Word16 lsp_new[M];
	Word16 lsp_mid[M];

	/* Algebraic codevector */
	Word16 code[L_SUBFR];

	/* excitation */
	Word16 excp[L_SUBFR];

	/* Scalars */
	Word16 i, i_subfr;
	Word16 T0, T0_frac, index;
	Word16 gain_pit, gain_code, bfi, pit_sharp;
	Word16 temp;
	Word32 L_temp;

	extern Word16 rxdtx_ctrl, rx_dtx_state;
	extern Word32 L_pn_seed_rx;

	/* Test bad frame indicator (bfi) */
	bfi = *parm++;
	move16 (); 

    /* Set state machine */
	test();
	test(); 
	if (bfi != 0) {
		state = add (state, 1);
	}
	else if (sub (state, 6) == 0) {
		state = 5;
	}
	else {
		state = 0;
	}

	test (); 
	if (sub (state, 6) > 0) {
		state = 6;
	}
	rx_dtx (&rxdtx_ctrl, TAF, bfi, SID_flag);

    /* If this frame is the first speech frame after CNI period,     */
    /* set the BFH state machine to an appropriate state depending   */
    /* on whether there was DTX muting before start of speech or not */
    /* If there was DTX muting, the first speech frame is muted.     */
    /* If there was no DTX muting, the first speech frame is not     */
    /* muted. The BFH state machine starts from state 5, however, to */
    /* keep the audible noise resulting from a SID frame which is    */
    /* erroneously interpreted as a good speech frame as small as    */
    /* possible (the decoder output in this case is quickly muted)   */
	test();
	logic16 ();
	if ((rxdtx_ctrl & RX_FIRST_SP_FLAG) != 0) {
        test();
		logic16();
		if ((rxdtx_ctrl & RX_PREV_DTX_MUTING) != 0) {
			state = 5;
			move16 ();
			prev_bf = 1;
			move16 ();
		}
		else {
			state = 5;
			move16 ();
			prev_bf = 0;
			move16 ();
		}
	}

#if (WMOPS)
	fwc ();                     /* function worst case */

    /* Note! The following test is performed only for determining
       whether or not DTX mode is active, in order to switch off
       worst worst case complexity printout when DTX mode is active
    */
	if ((rxdtx_ctrl & RX_SP_FLAG) == 0) {
		dtx_mode = 1;
	}
#endif

	D_plsf_5 (parm, lsp_mid, lsp_new, bfi, rxdtx_ctrl, rx_dtx_state);

#if (WMOPS)
	fwc ();                     /* function worst case */
#endif
	/* Advance synthesis parameters pointer */
	parm += 5;
	move16(); 

	test();
	logic16(); 
	if ((rxdtx_ctrl & RX_SP_FLAG) != 0) {
		/* Interpolation of LPC for the 4 subframes */
		Int_lpc (lsp_old, lsp_mid, lsp_new, A_t);
	}
	else {
		/* Comfort noise: use the same parameters in each subframe */
		Lsp_Az (lsp_new, A_t);

		for (i = 0; i < MP1; i++) {
			A_t[i + MP1] = A_t[i];
			move16 (); 
			A_t[i + 2 * MP1] = A_t[i];
			move16 (); 
			A_t[i + 3 * MP1] = A_t[i];
			move16 (); 
		}
	}

	/* update the LSPs for the next frame */
	for (i = 0; i < M; i++) {
		lsp_old[i] = lsp_new[i];
		move16 (); 
	}
#if (WMOPS)
	fwc ();                     /* function worst case */
#endif

    /*---------------------------------------------------------------------*
     *       Loop for every subframe in the analysis frame                 *
     *---------------------------------------------------------------------*
     * The subframe size is L_SUBFR and the loop is repeated               *
     * L_FRAME/L_SUBFR times                                               *
     *     - decode the pitch delay                                        *
     *     - decode algebraic code                                         *
     *     - decode pitch and codebook gains                               *
     *     - find the excitation and compute synthesis speech              *
     *---------------------------------------------------------------------*/

	/* pointer to interpolated LPC parameters */
	Az = A_t;
	move16 (); 

	for (i_subfr = 0; i_subfr < L_FRAME; i_subfr += L_SUBFR) {
		index = *parm++;
		move16 (); /* pitch index */

		test();
		logic16 (); 
		if ((rxdtx_ctrl & RX_SP_FLAG) != 0) {
			T0 = Dec_lag6 (index, PIT_MIN, PIT_MAX, i_subfr, L_FRAME_BY2, &T0_frac, bfi);
#if (WMOPS)
			fwc ();             /* function worst case */
#endif

            /*-------------------------------------------------*
             * - Find the adaptive codebook vector.            *
             *-------------------------------------------------*/

			Pred_lt_6 (&exc[i_subfr], T0, T0_frac, L_SUBFR);
#if (WMOPS)
			fwc ();             /* function worst case */
#endif
		}
		else {
            T0 = L_SUBFR;
			move16 (); 
		}

        /*-------------------------------------------------------*
         * - Decode pitch gain.                                  *
         *-------------------------------------------------------*/

		index = *parm++;
		move16 (); 

		gain_pit = d_gain_pitch (index, bfi, state, prev_bf, rxdtx_ctrl);
		move16 (); 
#if (WMOPS)
		fwc ();                 /* function worst case */
#endif

        /*-------------------------------------------------------*
         * - Decode innovative codebook.                         *
         *-------------------------------------------------------*/

		test();
		logic16 (); 
		if ((rxdtx_ctrl & RX_SP_FLAG) != 0) {
			dec_10i40_35bits (parm, code);
		}
		else {
			/* Use pseudo noise for excitation when SP_flag == 0 */
			build_CN_code (code, &L_pn_seed_rx);
		}

		parm += 10;
		move16 (); 
#if (WMOPS)
		fwc ();                 /* function worst case */
#endif

        /*-------------------------------------------------------*
         * - Add the pitch contribution to code[].               *
         *-------------------------------------------------------*/

        /* pit_sharp = gain_pit;                   */
        /* if (pit_sharp > 1.0) pit_sharp = 1.0;   */

		pit_sharp = shl (gain_pit, 3);

        /* This loop is not entered when SP_FLAG is 0 */
		for (i = T0; i < L_SUBFR; i++) {
			temp = mult (code[i - T0], pit_sharp);
			code[i] = add (code[i], temp);
			move16 (); 
		}
#if (WMOPS)
		fwc ();                 /* function worst case */
#endif
        /* post processing of excitation elements */

		test (); /* This test is not passed when SP_FLAG is 0 */
		if (sub (pit_sharp, 16384) > 0) {
			for (i = 0; i < L_SUBFR; i++) {
				temp = mult (exc[i + i_subfr], pit_sharp);
				L_temp = L_mult (temp, gain_pit);
				L_temp = L_shl (L_temp, 1);
				excp[i] = round_z (L_temp);
				move16 (); 
			}
		}
        /*-------------------------------------------------*
         * - Decode codebook gain.                         *
         *-------------------------------------------------*/

		index = *parm++;
		move16 (); /* index of energy VQ */

		d_gain_code (index, code, L_SUBFR, &gain_code, bfi, state, prev_bf, rxdtx_ctrl, i_subfr, rx_dtx_state);
#if (WMOPS)
		fwc ();                 /* function worst case */
#endif

        /*-------------------------------------------------------*
         * - Find the total excitation.                          *
         * - Find synthesis speech corresponding to exc[].       *
         *-------------------------------------------------------*/
		for (i = 0; i < L_SUBFR; i++) {
            /* exc[i] = gain_pit*exc[i] + gain_code*code[i]; */
			L_temp = L_mult (exc[i + i_subfr], gain_pit);
			L_temp = L_mac (L_temp, code[i], gain_code);
			L_temp = L_shl (L_temp, 3);
			exc[i + i_subfr] = round_z (L_temp);
			move16 (); 
		}
#if (WMOPS)
		fwc ();                 /* function worst case */
#endif

		test (); 
		if (sub (pit_sharp, 16384) > 0) {
			for (i = 0; i < L_SUBFR; i++) {
				excp[i] = add (excp[i], exc[i + i_subfr]);
				move16 (); 
			}
			agc2 (&exc[i_subfr], excp, L_SUBFR);
			Syn_filt (Az, excp, &synth[i_subfr], L_SUBFR, mem_syn, 1);
		}
		else {
			Syn_filt (Az, &exc[i_subfr], &synth[i_subfr], L_SUBFR, mem_syn, 1);
		}

#if (WMOPS)
		fwc ();                 /* function worst case */
#endif
        /* interpolated LPC parameters for next subframe */
		Az += MP1;
		move16 (); 
	}

    /*--------------------------------------------------*
     * Update signal for next frame.                    *
     * -> shift to the left by L_FRAME  exc[]           *
     *--------------------------------------------------*/

	Copy (&old_exc[L_FRAME], &old_exc[0], PIT_MAX + L_INTERPOL);
#if (WMOPS)
	fwc ();                     /* function worst case */
#endif
	prev_bf = bfi;
	move16 (); 

	return;
}
