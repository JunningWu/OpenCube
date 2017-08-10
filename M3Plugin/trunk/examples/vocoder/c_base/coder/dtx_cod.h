//
// 
//    File Name: dtx_cod.h
// 
//    Purpose:   Contains the CODER prototypes for all the functions of DTX.
//               Also contains CODER definitions of constants used in DTX functions.
// 
//		TEISA Dpt.
//  University of Cantabria
//  Date: Oct.2003
//  Author: F.Herrera
//  Adaptation of Coder code to SystemC
//
//  dtx for coder
//

#include "../common/dtx.h"

/* Encoder DTX control flags */

#define TX_SP_FLAG               0x0001
#define TX_VAD_FLAG              0x0002
#define TX_HANGOVER_ACTIVE       0x0004
#define TX_PREV_HANGOVER_ACTIVE  0x0008
#define TX_SID_UPDATE            0x0010
#define TX_USE_OLD_SID           0x0020

void aver_lsf_history (
    Word16 lsf_old[DTX_HANGOVER][M],
    Word16 lsf1[M],
    Word16 lsf2[M],
    Word16 lsf_aver[M]
);

// subframe_coder: adaptive_coder
Word16 compute_CN_excitation_gain (
    Word16 res2[L_SUBFR]
);

// subframe_coder: innovative_coder: q_gain_code
Word16 aver_gain_code_history (
    Word16 CN_excitation_gain,
    Word16 gain_code_old[4 * DTX_HANGOVER]
);
