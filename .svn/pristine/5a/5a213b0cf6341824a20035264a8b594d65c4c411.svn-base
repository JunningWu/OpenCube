//
// 
//    File Name: dtx_dec.h
// 
//    Purpose:   Contains the DECODER prototypes for functions of DTX.
//               Also contains DECODER definitions of constants used in DTX functions.
// 
//		TEISA Dpt.
//  University of Cantabria
//  Date: Dic.2003
//  Author: F.Herrera
//  Adaptation of Coder code to SystemC
//
//  Decoder part of the dtx code
//

#include "../common/dtx.h"

// Decoder DTX control flags

#define RX_SP_FLAG               0x0001
#define RX_UPD_SID_QUANT_MEM     0x0002
#define RX_FIRST_SID_UPDATE      0x0004
#define RX_CONT_SID_UPDATE       0x0008
#define RX_LOST_SID_FRAME        0x0010
#define RX_INVALID_SID_FRAME     0x0020
#define RX_NO_TRANSMISSION       0x0040
#define RX_DTX_MUTING            0x0080
#define RX_PREV_DTX_MUTING       0x0100
#define RX_CNI_BFI               0x0200
#define RX_FIRST_SP_FLAG         0x0400

// Constant DTX_ELAPSED_THRESHOLD is used as threshold for allowing
//   SID frame updating without hangover period in case when elapsed
//   time measured from previous SID update is below 24

#define DTX_ELAPSED_THRESHOLD (24 + DTX_HANGOVER - 1)

void rx_dtx (
    Word16 *rxdtx_ctrl,
    Word16 *rx_dtx_state,
    Word16 *prev_SID_frames_lost,
    Word32 *L_pn_seed_rx_p,
    Word16 *rxdtx_N_elapsed,
    Word16 TAF,
    Word16 bfi,
    Word16 SID_flag
);

void CN_encoding (
    Word16 params[],
    Word16 txdtx_ctrl
);

void sid_codeword_encoding (
    Word16 ser2[]
);

Word16 sid_frame_detection (
    Word16 ser2[]
);

void aver_lsf_history (
    Word16 lsf_old[DTX_HANGOVER][M],
    Word16 lsf1[M],
    Word16 lsf2[M],
    Word16 lsf_aver[M]
);

void update_gain_code_history_tx (
    Word16 new_gain_code,
    Word16 gain_code_old_tx[4 * DTX_HANGOVER]
);

void update_gain_code_history_rx (
    Word16 new_gain_code,
    Word16 gain_code_old_rx[4 * DTX_HANGOVER],
    Word16 *buf_p_rx    
);

Word16 compute_CN_excitation_gain (
    Word16 res2[L_SUBFR]
);

Word16 aver_gain_code_history (
    Word16 CN_excitation_gain,
    Word16 gain_code_old[4 * DTX_HANGOVER]
);

Word16 interpolate_CN_param (
    Word16 old_param,
    Word16 new_param,
    Word16 rx_dtx_state
);

void interpolate_CN_lsf (
    Word16 lsf_old_CN[M],
    Word16 lsf_new_CN[M],
    Word16 lsf_interp_CN[M],
    Word16 rx_dtx_state
);
