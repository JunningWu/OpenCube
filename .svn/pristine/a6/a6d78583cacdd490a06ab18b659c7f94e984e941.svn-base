//
// 
//    File Name: dtx.h
// 
//    Purpose:   Contains the COMMON prototypes for functions of DTX.
//               Also contains COMMON definitions of constants used in DTX functions.
// 
//		TEISA Dpt.
//  University of Cantabria
//  Date: Oct.2003
//  Author: F.Herrera
//  Adaptation of Coder code to SystemC
//
//  Common part of the dtx code
//

#ifndef _DTX_H
#define _DTX_H


#define PN_INITIAL_SEED 0x70816958L   /* Pseudo noise generator seed value  */

#define CN_INT_PERIOD 24              /* Comfort noise interpolation period
                                         (nbr of frames between successive
                                         SID updates in the decoder) */

#define DTX_HANGOVER 7                /* Period when SP=1 although VAD=0.
                                         Used for comfort noise averaging */

/* Frame classification constants */

#define VALID_SID_FRAME          1
#define INVALID_SID_FRAME        2
#define GOOD_SPEECH_FRAME        3
#define UNUSABLE_FRAME           4

/* Inverse values of DTX hangover period and DTX hangover period + 1 */

#define INV_DTX_HANGOVER (0x7fff / DTX_HANGOVER)
#define INV_DTX_HANGOVER_P1 (0x7fff / (DTX_HANGOVER+1))

#define NB_PULSE 10 /* Number of pulses in fixed codebook excitation */

/* SID frame classification thresholds */

#define VALID_SID_THRESH 2
#define INVALID_SID_THRESH 16

/* Constant DTX_ELAPSED_THRESHOLD is used as threshold for allowing
   SID frame updating without hangover period in case when elapsed
   time measured from previous SID update is below 24 */

//#define DTX_ELAPSED_THRESHOLD (24 + DTX_HANGOVER - 1)

/* Index map for encoding and detecting SID codeword */

static const Word16 SID_codeword_bit_idx[95] =
{
     45,  46,  48,  49,  50,  51,  52,  53,  54,  55,
     56,  57,  58,  59,  60,  61,  62,  63,  64,  65,
     66,  67,  68,  94,  95,  96,  98,  99, 100, 101,
    102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
    112, 113, 114, 115, 116, 117, 118, 148, 149, 150,
    151, 152, 153, 154, 155, 156, 157, 158, 159, 160,
    161, 162, 163, 164, 165, 166, 167, 168, 169, 170,
    171, 196, 197, 198, 199, 200, 201, 202, 203, 204,
    205, 206, 207, 208, 209, 212, 213, 214, 215, 216,
    217, 218, 219, 220, 221
};

// frame_lsp
void update_lsf_history (
    Word16 lsf1[M],
    Word16 lsf2[M],
    Word16 lsf_old[DTX_HANGOVER][M]
);

void update_lsf_p_CN (
    Word16 lsf_old[DTX_HANGOVER][M],
    Word16 lsf_p_CN[M]
);

// subframe_coder: innovative_coder: q_gain_code
Word16 update_gcode0_CN (
    Word16 gain_code_old_tx[4 * DTX_HANGOVER]
);

// subframe_coder: innovative_coder
void build_CN_code (
    Word16 cod[],
    Word32 *seed
);

// usada por la anterior
Word16 pseudonoise (
    Word32 *shift_reg,
    Word16 no_bits
);

#endif
