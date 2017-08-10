
#include "../common/codec.h"

void code_10i40_35bits (
    Word16 x[],        /* (i)   : target vector                             */
    Word16 cn[],       /* (i)   : residual after long term prediction       */
    Word16 h[],        /* (i)   : impulse response of weighted synthesis
                                  filter                                    */
    Word16 cod[],      /* (o)   : algebraic (fixed) codebook excitation     */
    Word16 y[],        /* (o)   : filtered fixed codebook excitation        */
    Word16 indx[]      /* (o)   : index of 10 pulses (sign + position)      */
);

Word16 Enc_lag6 (      /* output: Return index of encoding                  */
    Word16 T0,         /* input : Pitch delay                               */
    Word16 *T0_frac,   /* in/out: Fractional pitch delay                    */
    Word16 *T0_min,    /* in/out: Minimum search delay                      */
    Word16 *T0_max,    /* in/out: Maximum search delay                      */
    Word16 pit_min,    /* input : Minimum pitch delay                       */
    Word16 pit_max,    /* input : Maximum pitch delay                       */
    Word16 pit_flag    /* input : Flag for 1st or 3rd subframe              */
);

Word16 q_gain_pitch (   /* Return index of quantization */
    Word16 *gain        /* (i)  :  Pitch gain to quantize  */
);

Word16 q_gain_code (   /* Return quantization index                         */
    Word16 code[],     /* (i)      : fixed codebook excitation              */
    Word16 lcode,      /* (i)      : codevector size                        */
    Word16 *gain,      /* (i/o)    : quantized fixed codebook gain          */
	Word16 past_qua_en[], // past quantized energies.	
    Word16 txdtx_ctrl,
    Word16 i_subfr,
	Word16 CN_excitation_gain,
	Word16 gain_code_old_tx[]  // Iput/output	
);

Word16 G_pitch (       /* (o)     : Gain of pitch lag saturated to 1.2      */
    Word16 xn[],       /* (i)     : Pitch target.                           */
    Word16 y_1[],       /* (i)     : Filtered adaptive codebook.             */
    Word16 L_subfr     /*         : Length of subframe.                     */
);

Word16 G_code (        /* out      : Gain of innovation code.               */
    Word16 xn[],       /* in       : target vector                          */
    Word16 y2[]        /* in       : filtered inovation vector              */
);

Word16 Interpol_6 (    /* (o)  : interpolated value                         */
    Word16 *x,         /* (i)  : input vector                               */
    Word16 frac        /* (i)  : fraction                                   */
);
void Int_lpc (
    Word16 lsp_old[],  /* input: LSP vector at the 4th subfr. of past frame */
    Word16 lsp_mid[],  /* input: LSP vector at the 2nd subfr. of
                          present frame                                     */
    Word16 lsp_new[],  /* input: LSP vector at the 4th subfr. of
                          present frame                                     */
    Word16 Az[]        /* output: interpolated LP parameters in all subfr.  */
);
void Int_lpc2 (
    Word16 lsp_old[],  /* input: LSP vector at the 4th subfr. of past frame */
    Word16 lsp_mid[],  /* input: LSP vector at the 2nd subframe of
                          present frame                                     */
    Word16 lsp_new[],  /* input: LSP vector at the 4th subframe of
                          present frame                                     */
    Word16 Az[]        /* output:interpolated LP parameters
                          in subframes 1 and 3                              */
);

Word16 Pitch_fr6 (     /* (o)     : pitch period.                           */
    Word16 exc[],      /* (i)     : excitation buffer                       */
    Word16 xn[],       /* (i)     : target vector                           */
    Word16 h[],        /* (i)     : impulse response of synthesis and
                                    weighting filters                       */
    Word16 L_subfr,    /* (i)     : Length of subframe                      */
    Word16 t0_min,     /* (i)     : minimum value in the searched range.    */
    Word16 t0_max,     /* (i)     : maximum value in the searched range.    */
    Word16 i_subfr,    /* (i)     : indicator for first subframe.           */
    Word16 *pit_frac   /* (o)     : chosen fraction.                        */
);

Word16 Pitch_ol (      /* output: open loop pitch lag                       */
    Word16 signal[],   /* input: signal used to compute the open loop pitch */
                       /* signal[-pit_max] to signal[-1] should be known    */
    Word16 pit_min,    /* input : minimum pitch lag                         */
    Word16 pit_max,    /* input : maximum pitch lag                         */
    Word16 L_frame     /* input : length of frame to compute pitch          */
);

void Pred_lt_6 (
    Word16 exc[],      /* in/out: excitation buffer                         */
    Word16 T0,         /* input : integer pitch lag                         */
    Word16 frac,       /* input : fraction of lag                           */
    Word16 L_subfr     /* input : subframe size                             */
);

void Q_plsf_5 (
    Word16 *lsp1,      /* input : 1st LSP vector                            */
    Word16 *lsp2,      /* input : 2nd LSP vector                            */
    Word16 *lsp1_q,    /* output: quantized 1st LSP vector                  */
    Word16 *lsp2_q,    /* output: quantized 2nd LSP vector                  */
    Word16 *indice,    /* output: quantization indices of 5 matrices        */
    Word16 txdtx_ctrl, /* input : tx dtx control word                       */
	Word16 past_r2_q[], /* input/output: Past quantized prediction error */
	Word16 lsf_old_tx[][M] // input/output: Comfort noise LSF averaging buffer
);
