/**************************************************************************
 *
 *   File Name:  d_homing.c
 *
 *   Purpose:
 *      This file contains the following functions:
 *
 *      decoder_homing_frame_test()  checks if a frame of input speech
 *                                   parameters matches the Decoder Homing
 *                                   Frame pattern.
 *
 *      decoder_reset()              called by reset_dec() to reset all of
 *                                   the state variables for the decoder
 *
 *      reset_dec()                  calls functions to reset the state
 *                                   variables for the decoder, and for
 *                                   the receive DTX and Comfort Noise.
 *
 **************************************************************************/

#include "../common/typedef.h"
#include "../common/cnst.h"
#include "../common/q_plsf_5.tab"

#include "dtx_dec.h"
#include "decoder.h"
#include "d_homing.h"

#define PRM_NO    57


/***************************************************************************
 *
 *   FUNCTION NAME:  decoder_homing_frame_test
 *
 *   PURPOSE:
 *      Checks if a frame of input speech parameters matches the Decoder
 *      Homing Frame pattern, which is:
 *
 *      parameter    decimal value    hexidecimal value
 *      ---------    -------------    -----------------
 *      LPC 1        4                0x0004
 *      LPC 2        47               0x002F
 *      LPC 3        180              0x00B4
 *      LPC 4        144              0x0090
 *      LPC 5        62               0x003E
 *      LTP-LAG 1    342              0x0156
 *      LTP-GAIN 1   11               0x000B
 *      PULSE1_1     0                0x0000
 *      PULSE1_2     1                0x0001
 *      PULSE1_3     15               0x000F
 *      PULSE1_4     1                0x0001
 *      PULSE1_5     13               0x000D
 *      PULSE1_6     0                0x0000
 *      PULSE1_7     3                0x0003
 *      PULSE1_8     0                0x0000
 *      PULSE1_9     3                0x0003
 *      PULSE1_10    0                0x0000
 *      FCB-GAIN 1   3                0x0003
 *      LTP-LAG 2    54               0x0036
 *      LTP-GAIN 2   1                0x0001
 *      PULSE2_1     8                0x0008
 *      PULSE2_2     8                0x0008
 *      PULSE2_3     5                0x0005
 *      PULSE2_4     8                0x0008
 *      PULSE2_5     1                0x0001
 *      PULSE2_6     0                0x0000
 *      PULSE2_7     0                0x0000
 *      PULSE2_8     1                0x0001
 *      PULSE2_9     1                0x0001
 *      PULSE2_10    0                0x0000
 *      FCB-GAIN 2   0                0x0000
 *      LTP-LAG 3    342              0x0156
 *      LTP-GAIN 3   0                0x0000
 *      PULSE3_1     0                0x0000
 *      PULSE3_2     0                0x0000
 *      PULSE3_3     0                0x0000
 *      PULSE3_4     0                0x0000
 *      PULSE3_5     0                0x0000
 *      PULSE3_6     0                0x0000
 *      PULSE3_7     0                0x0000
 *      PULSE3_8     0                0x0000
 *      PULSE3_9     0                0x0000
 *      PULSE3_10    0                0x0000
 *      FCB-GAIN 3   0                0x0000
 *      LTP-LAG 4    54               0x0036
 *      LTP-GAIN 4   11               0x000B
 *      PULSE4_1     0                0x0000
 *      PULSE4_2     0                0x0000
 *      PULSE4_3     0                0x0000
 *      PULSE4_4     0                0x0000
 *      PULSE4_5     0                0x0000
 *      PULSE4_6     0                0x0000
 *      PULSE4_7     0                0x0000
 *      PULSE4_8     0                0x0000
 *      PULSE4_9     0                0x0000
 *      PULSE4_10    0                0x0000
 *      FCB-GAIN 4   0                0x0000
 *
 *   INPUT:
 *      parm[]  one frame of speech parameters in parallel format
 *
 *      nbr_of_params
 *              the number of consecutive parameters in parm[] to match
 *
 *   OUTPUT:
 *      None
 *
 *   RETURN:
 *      0       input frame does not match the Decoder Homing Frame pattern.
 *      1       input frame matches the Decoder Homing Frame pattern.
 *
 **************************************************************************/

Word16 decoder_homing_frame_test (Word16 parm[], Word16 nbr_of_params)
{
#ifdef PERF
	uc_prof_control->add_function_time();
#endif

    static const Word16 dhf_mask[PRM_NO] =
    {
        0x0004,                 /* LPC 1 */
        0x002f,                 /* LPC 2 */
        0x00b4,                 /* LPC 3 */
        0x0090,                 /* LPC 4 */
        0x003e,                 /* LPC 5 */

        0x0156,                 /* LTP-LAG 1 */
        0x000b,                 /* LTP-GAIN 1 */
        0x0000,                 /* PULSE 1_1 */
        0x0001,                 /* PULSE 1_2 */
        0x000f,                 /* PULSE 1_3 */
        0x0001,                 /* PULSE 1_4 */
        0x000d,                 /* PULSE 1_5 */
        0x0000,                 /* PULSE 1_6 */
        0x0003,                 /* PULSE 1_7 */
        0x0000,                 /* PULSE 1_8 */
        0x0003,                 /* PULSE 1_9 */
        0x0000,                 /* PULSE 1_10 */
        0x0003,                 /* FCB-GAIN 1 */

        0x0036,                 /* LTP-LAG 2 */
        0x0001,                 /* LTP-GAIN 2 */
        0x0008,                 /* PULSE 2_1 */
        0x0008,                 /* PULSE 2_2 */
        0x0005,                 /* PULSE 2_3 */
        0x0008,                 /* PULSE 2_4 */
        0x0001,                 /* PULSE 2_5 */
        0x0000,                 /* PULSE 2_6 */
        0x0000,                 /* PULSE 2_7 */
        0x0001,                 /* PULSE 2_8 */
        0x0001,                 /* PULSE 2_9 */
        0x0000,                 /* PULSE 2_10 */
        0x0000,                 /* FCB-GAIN 2 */

        0x0156,                 /* LTP-LAG 3 */
        0x0000,                 /* LTP-GAIN 3 */
        0x0000,                 /* PULSE 3_1 */
        0x0000,                 /* PULSE 3_2 */
        0x0000,                 /* PULSE 3_3 */
        0x0000,                 /* PULSE 3_4 */
        0x0000,                 /* PULSE 3_5 */
        0x0000,                 /* PULSE 3_6 */
        0x0000,                 /* PULSE 3_7 */
        0x0000,                 /* PULSE 3_8 */
        0x0000,                 /* PULSE 3_9 */
        0x0000,                 /* PULSE 3_10 */
        0x0000,                 /* FCB-GAIN 3 */

        0x0036,                 /* LTP-LAG 4 */
        0x000b,                 /* LTP-GAIN 4 */
        0x0000,                 /* PULSE 4_1 */
        0x0000,                 /* PULSE 4_2 */
        0x0000,                 /* PULSE 4_3 */
        0x0000,                 /* PULSE 4_4 */
        0x0000,                 /* PULSE 4_5 */
        0x0000,                 /* PULSE 4_6 */
        0x0000,                 /* PULSE 4_7 */
        0x0000,                 /* PULSE 4_8 */
        0x0000,                 /* PULSE 4_9 */
        0x0000,                 /* PULSE 4_10 */
        0x0000                  /* FCB-GAIN 4 */ };

    Word16 i, j;

    for (i = 0; i < nbr_of_params; i++)
    {
        j = parm[i] ^ dhf_mask[i];

        if (j)
            break;
    }

    return !j;
}
