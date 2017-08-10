//
// 
//    File Name: dtx_dec.c
// 
//    Purpose:   Contains the DECODER implementations for functions of DTX.
//               Also contains DECODER implementations of constants used in DTX functions.
// 
//		TEISA Dpt.
//  University of Cantabria
//  Date: Dic.2003
//  Author: F.Herrera
//  Adaptation of Coder code to SystemC
//
//  Decoder part of the dtx code
//

#include "../common/typedef.h"
#include "../common/cnst.h"
#include "../common/basic_op.h"
#include "../common/sig_proc.h"

#include "dtx_dec.h"

//*************************************************************************
// 
//    FUNCTION NAME: rx_dtx
// 
//    PURPOSE: DTX handler of the speech decoder. Determines when to update
//             the reference comfort noise parameters (LSF and gain) at the
//             end of the speech burst. Also classifies the incoming frames
//             according to SID flag and BFI flag
//             and determines when the transmission is active during comfort
//             noise insertion. This function also initializes the pseudo
//             noise generator shift register.
// 
//             Operation of the RX DTX handler is based on measuring the
//             lengths of speech bursts and the lengths of the pauses between
//             speech bursts to determine when there exists a hangover period
//             at the end of a speech burst. The idea is to keep in sync with
//            the TX DTX handler to be able to update the reference comfort
//             noise parameters at the same time instances.
// 
//    INPUTS:      *rxdtx_ctrl             Old decoder DTX control word
//                 *rx_dtx_state           Old state of comfort noise interpolation
//                 *prev_SID_frames_lost   Old Counter for lost SID frames
//                 TAF           Time alignment flag
//                 bfi           Bad frame indicator flag
//                 SID_flag      Silence descriptor flag
// 
//    OUTPUTS:     *rxdtx_ctrl             Updated decoder DTX control word
//                 rx_dtx_state            Updated state of comfort noise interpolation
//                                         period (global variable)
//                 *prev_SID_frames_lost   Updated Counter for lost SID frames
//                 L_pn_seed_rx            Initialized pseudo noise generator shift
//                                         register (global variable)
// 
//    RETURN VALUE: none
// 
// ************************************************************************

void rx_dtx (
    Word16 *rxdtx_ctrl,
    Word16 *rx_dtx_state,
    Word16 *prev_SID_frames_lost,
    Word32 *L_pn_seed_rx_p,
    Word16 *rxdtx_N_elapsed,
    Word16 TAF,
    Word16 bfi,
    Word16 SID_flag
)
{
#ifdef PERF
	uc_prof_control->add_function_time();
#endif

    Word16 frame_type;
    
    static Word16 rxdtx_aver_period;           // Length of hangover period (VAD=0, SP=1)    

    /* Frame classification according to bfi-flag and ternary-valued
       SID flag. The frames between SID updates (not actually trans-
       mitted) are also classified here; they will be discarded later
       and provided with "NO TRANSMISSION"-flag */

    if ((sub (SID_flag, 2) == 0) && (bfi == 0))
    {
        frame_type = VALID_SID_FRAME;
    }
    else if ((SID_flag == 0) && (bfi == 0))
    {
        frame_type = GOOD_SPEECH_FRAME;
    }
    else if ((SID_flag == 0) && (bfi != 0))
    {
        frame_type = UNUSABLE_FRAME;
    }
    else
    {
        frame_type = INVALID_SID_FRAME;
    }

    /* Update of decoder state */
    /* Previous frame was classified as a speech frame */
    if ((*rxdtx_ctrl & RX_SP_FLAG) != 0)
    {
        if (sub (frame_type, VALID_SID_FRAME) == 0)
        {
            *rxdtx_ctrl = RX_FIRST_SID_UPDATE;
        }
        else if (sub (frame_type, INVALID_SID_FRAME) == 0)
        {
            *rxdtx_ctrl = RX_FIRST_SID_UPDATE
                        | RX_INVALID_SID_FRAME;
        }
        else if (sub (frame_type, UNUSABLE_FRAME) == 0)
        {
            *rxdtx_ctrl = RX_SP_FLAG;
        }
        else if (sub (frame_type, GOOD_SPEECH_FRAME) == 0)
        {
            *rxdtx_ctrl = RX_SP_FLAG;
        }
    }
    else
    {
        if (sub (frame_type, VALID_SID_FRAME) == 0)
        {
            *rxdtx_ctrl = RX_CONT_SID_UPDATE;
        }
        else if (sub (frame_type, INVALID_SID_FRAME) == 0)
        {
            *rxdtx_ctrl = RX_CONT_SID_UPDATE
                        | RX_INVALID_SID_FRAME;
        }
        else if (sub (frame_type, UNUSABLE_FRAME) == 0)
        {
            *rxdtx_ctrl = RX_CNI_BFI;
        }
        else if (sub (frame_type, GOOD_SPEECH_FRAME) == 0)
        {
            /* If the previous frame (during CNI period) was muted,
               raise the RX_PREV_DTX_MUTING flag */
            if ((*rxdtx_ctrl & RX_DTX_MUTING) != 0)
            {
                *rxdtx_ctrl = RX_SP_FLAG | RX_FIRST_SP_FLAG
                            | RX_PREV_DTX_MUTING;
            }
            else
            {
                *rxdtx_ctrl = RX_SP_FLAG | RX_FIRST_SP_FLAG;
            }
        }
    }

    if ((*rxdtx_ctrl & RX_SP_FLAG) != 0)
    {
        *prev_SID_frames_lost = 0;
        *rx_dtx_state = CN_INT_PERIOD - 1;
    }
    else
    {
        /* First SID frame */
        if ((*rxdtx_ctrl & RX_FIRST_SID_UPDATE) != 0)
        {
            *prev_SID_frames_lost = 0;
            *rx_dtx_state = CN_INT_PERIOD - 1;
        }

        /* SID frame detected, but not the first SID */
        if ((*rxdtx_ctrl & RX_CONT_SID_UPDATE) != 0)
        {
            *prev_SID_frames_lost = 0;
			
            if (sub (frame_type, VALID_SID_FRAME) == 0)
            {
                *rx_dtx_state = 0;
            }
            else if (sub (frame_type, INVALID_SID_FRAME) == 0)
            {
                if (sub(*rx_dtx_state, (CN_INT_PERIOD - 1)) < 0)
                {
                    *rx_dtx_state = add(*rx_dtx_state, 1);
                }
            }
        }

        /* Bad frame received in CNI mode */
        if ((*rxdtx_ctrl & RX_CNI_BFI) != 0)
        {
            if (sub (*rx_dtx_state, (CN_INT_PERIOD - 1)) < 0)
            {
                *rx_dtx_state = add (*rx_dtx_state, 1);
            }

            /* If an unusable frame is received during CNI period
               when TAF == 1, the frame is classified as a lost
               SID frame */
            if (sub (TAF, 1) == 0)
            {
                *rxdtx_ctrl = *rxdtx_ctrl | RX_LOST_SID_FRAME;	
                *prev_SID_frames_lost = add (*prev_SID_frames_lost, 1);
            }
            else /* No transmission occurred */
            {
                *rxdtx_ctrl = *rxdtx_ctrl | RX_NO_TRANSMISSION;
            }

            if (sub (*prev_SID_frames_lost, 1) > 0)
            {
                *rxdtx_ctrl = *rxdtx_ctrl | RX_DTX_MUTING;
            }
        }
    }

    /* N_elapsed (frames since last SID update) is incremented. If SID
       is updated N_elapsed is cleared later in this function */

    *rxdtx_N_elapsed = add (*rxdtx_N_elapsed, 1);

    if ((*rxdtx_ctrl & RX_SP_FLAG) != 0)
    {
        rxdtx_aver_period = DTX_HANGOVER;
    }
    else
    {
        if (sub (*rxdtx_N_elapsed, DTX_ELAPSED_THRESHOLD) > 0)
        {
            *rxdtx_ctrl |= RX_UPD_SID_QUANT_MEM;
            *rxdtx_N_elapsed = 0;
            rxdtx_aver_period = 0;
            *L_pn_seed_rx_p = PN_INITIAL_SEED;
        }
        else if (rxdtx_aver_period == 0)
        {
            *rxdtx_N_elapsed = 0;
        }
        else
        {
            rxdtx_aver_period = sub (rxdtx_aver_period, 1);
        }
    }

    return;
}

// ***********************************************************************
// 
//    FUNCTION NAME: update_gain_code_history_rx
// 
//    PURPOSE: Update the fixed codebook gain parameter history of the
//             decoder. The fixed codebook gain parameters kept in the buffer
//             are used later for computing the reference fixed codebook
//             gain parameter value.
// 
//    INPUTS:      new_gain_code   New fixed codebook gain value
// 
//                 gain_code_old_tx[0..4*DTX_HANGOVER-1]
//                                 Old fixed codebook gain history of decoder
// 
//    OUTPUTS:     gain_code_old_tx[0..4*DTX_HANGOVER-1]
//                                 Updated fixed codebk gain history of decoder
// 
//    RETURN VALUE: none
// 
// ************************************************************************

void update_gain_code_history_rx (
    Word16 new_gain_code,
    Word16 gain_code_old_rx[4 * DTX_HANGOVER],
    Word16 *buf_p_rx
)
{
#ifdef PERF
	uc_prof_control->add_function_time();
#endif

    // Circular buffer
    gain_code_old_rx[*buf_p_rx] = new_gain_code;

    if (sub (*buf_p_rx, (4 * DTX_HANGOVER - 1)) == 0)
    {
        *buf_p_rx = 0;
    }
    else
    {
        *buf_p_rx = add (*buf_p_rx, 1);
    }

    return;
}

// ************************************************************************
// 
//    FUNCTION NAME: interpolate_CN_param
// 
//    PURPOSE: Interpolate a comfort noise parameter value over the comfort
//             noise update period.
// 
//    INPUTS:      old_param     The older parameter of the interpolation
//                               (the endpoint the interpolation is started
//                               from)
//                 new_param     The newer parameter of the interpolation
//                               (the endpoint the interpolation is ended to)
//                 rx_dtx_state  State of the comfort noise insertion period
// 
//    OUTPUTS:     none
// 
//    RETURN VALUE: Interpolated CN parameter value
// 
// ************************************************************************

Word16 interpolate_CN_param (
    Word16 old_param,
    Word16 new_param,
    Word16 rx_dtx_state
)
{
#ifdef PERF
	uc_prof_control->add_function_time();
#endif

    static const Word16 interp_factor[CN_INT_PERIOD] =
    {
        0x0555, 0x0aaa, 0x1000, 0x1555, 0x1aaa, 0x2000,
        0x2555, 0x2aaa, 0x3000, 0x3555, 0x3aaa, 0x4000,
        0x4555, 0x4aaa, 0x5000, 0x5555, 0x5aaa, 0x6000,
        0x6555, 0x6aaa, 0x7000, 0x7555, 0x7aaa, 0x7fff};
    Word16 temp;
    Word32 L_temp;

    L_temp = L_mult (interp_factor[rx_dtx_state], new_param);
    temp = sub (0x7fff, interp_factor[rx_dtx_state]);
    temp = add (temp, 1);
    L_temp = L_mac (L_temp, temp, old_param);
    temp = round (L_temp);

    return temp;
}

// ************************************************************************
// 
//   FUNCTION NAME:  interpolate_CN_lsf
// 
//    PURPOSE: Interpolate comfort noise LSF parameter vector over the comfort
//             noise update period.
// 
//    INPUTS:      lsf_old_CN[0..9]
//                               The older LSF parameter vector of the
//                               interpolation (the endpoint the interpolation
//                               is started from)
//                 lsf_new_CN[0..9]
//                               The newer LSF parameter vector of the
//                               interpolation (the endpoint the interpolation
//                               is ended to)
//                 rx_dtx_state  State of the comfort noise insertion period
// 
//    OUTPUTS:     lsf_interp_CN[0..9]
//                               Interpolated LSF parameter vector
// 
//    RETURN VALUE: none
// 
// ************************************************************************

void interpolate_CN_lsf (
    Word16 lsf_old_CN[M],
    Word16 lsf_new_CN[M],
    Word16 lsf_interp_CN[M],
    Word16 rx_dtx_state
)
{
#ifdef PERF
	uc_prof_control->add_function_time();
#endif

    Word16 i;

    for (i = 0; i < M; i++)
    {
        lsf_interp_CN[i] = interpolate_CN_param (lsf_old_CN[i],
                                                 lsf_new_CN[i],
                                                 rx_dtx_state);
    }

    return;
}
