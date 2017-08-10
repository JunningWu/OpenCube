
#include "../common/codec.h"
#include "../common/dtx.h"

void Init_Decoder_12k2 (void);

void Decoder_12k2 (
    Word16 parm[],     /* input : vector of synthesis parameters
                                  parm[0] = bad frame indicator (bfi) */
    Word16 synth[],    /* output: synthesis speech                    */
    Word16 A_t[],      /* output: decoded LP filter in 4 subframes    */
    Word16 TAF,
    Word16 SID_flag
);

Word16 Bin2int (        /* Reconstructed parameter                      */
    Word16 no_of_bits,  /* input : number of bits associated with value */
    Word16 *bitstream   /* output: address where bits are written       */
);

void Init_Post_Filter (void);

void Post_Filter (
   Word16 *syn,       // in/out: synthesis speech (postfiltered is output)
   Word16 *Az_4,      // input : interpolated LPC parameters in all subfr
   Word16 i_subfr1,
   Word16 *mem_pre,
   Word16 *past_gain,
   Word16 *syn_post
);

void dec_10i40_35bits (
    Word16 index[],    /* (i)   : index of 10 pulses (sign+position)        */
    Word16 cod[]       /* (o)   : algebraic (fixed) codebook excitation     */
);

Word16 Dec_lag6 (      /* output: return integer pitch lag                  */
    Word16 index,      /* input : received pitch index                      */
    Word16 pit_min,    /* input : minimum pitch lag                         */
    Word16 pit_max,    /* input : maximum pitch lag                         */
    Word16 i_subfr,    /* input : subframe flag                             */
    Word16 L_frame_by2,/* input : speech frame size divided by 2            */
    Word16 *T0_frac,   /* output: fractional part of pitch lag              */
    Word16 bfi,         /* input : bad frame indicator                       */
    Word16 &T0_old
);

Word16 d_gain_pitch ( /* out      : quantized pitch gain           */
    Word16 index,     /* in       : index of quantization          */
    Word16 bfi,       /* in       : bad frame indicator (good = 0) */
    Word16 state,     /* in       : state of the state machine             */
    Word16 prev_bf,   // Previous bf  
    Word16 rxdtx_ctrl,
    Word16 pbuf[5],
    Word16 *past_gain_pit,
    Word16 *prev_gp    
);

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
);

void D_plsf_5 (
    Word16 *indice,       /* input : quantization indices of 5 submatrices */
    Word16 *lsp1_q,       /* output: quantized 1st LSP vector              */
    Word16 *lsp2_q,       /* output: quantized 2nd LSP vector              */
    Word16 lsf_old_rx[][M],  // Comfort noise LSF averaging buffer
    Word16 *past_r2_q,       // Past quantized prediction error
    Word16 past_lsf_q[M],    // Past dequantized lsfs
    Word16 lsf_p_CN[M],      //Reference LSF parameter vector (comfort noise)
    Word16 lsf_old_CN[M],    //  LSF memories for comfort noise interpolation
    Word16 lsf_new_CN[M],
    Word16 bfi,           /* input : bad frame indicator (set to 1 if a bad
                                     frame is received)                    */
    Word16 rxdtx_ctrl,    /* input : RX DTX control word                   */
    Word16 rx_dtx_state   /* input : state of the comfort noise insertion
                                     period                                */
);

