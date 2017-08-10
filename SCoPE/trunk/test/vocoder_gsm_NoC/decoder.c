/***************************************************************************
 *
 *  FILE NAME:  decoder.c
 *
 *         Usage : decoder  bitstream_file  synth_file
 *
 *         Format for bitstream_file:
 *           One word (2-byte) for bad frame indication (BFI) flag bit
 *               0x0000 -> good frame;  0x0001 -> bad frame
 *           244  words (2-byte) containing 244 bits.
 *               Bit 0 = 0x0000 and Bit 1 = 0x0001
 *           One word (2-byte) for ternary Silence Descriptor (SID) flag
 *               0x0000 -> inactive (no detected speech activity);
 *               0x0001 -> active
 *           One word (2-byte) for Time Alignment Flag (TAF) bit
 *               0x0000 -> inactive (no transmission of speech frames);
 *               0x0001 -> active
 *
 *         Format for synth_file:
 *           Synthesis is written to a binary file of 16 bits data.
 *
 ***************************************************************************/

#include "typedef.h"
#include "n_stack.h"
#include "basic_op.h"
#include "sig_proc.h"
#include "count.h"
#include "codec.h"
#include "cnst.h"
#include "d_homing.h"

/* These constants define the number of consecutive parameters
   that function decoder_homing_frame_test() checks */

#define WHOLE_FRAME 57
#define TO_FIRST_SUBFRAME 18

/*-----------------------------------------------------------------*
 *             Global variables                                    *
 *-----------------------------------------------------------------*/
Word16 synth_buf[M + L_FRAME];

/*-----------------------------------------------------------------*
 *            Main decoder routine                                 *
 *-----------------------------------------------------------------*/

//int main (int argc, char *argv[]) {
int decoder(Word16 *spch_dec, Word16 *spch_out) {
	Word16 *synth;					/* Synthesis                  */
	Word16 parm[PRM_SIZE + 1];		/* Synthesis parameters       */
	Word16 serial[SERIAL_SIZE+2];	/* Serial stream              */
	Word16 Az_dec[AZ_SIZE];			/* Decoded Az for post-filter in 4 subframes, length= 44 */
	Word16 i, frame, temp;
	Word16 total_frames, aux, out_offset;
	Word16 TAF, SID_flag;
	Word16 reset_flag;
	static Word16 reset_flag_old = 1;

	/*-----------------------------------------------------------------*
	 *           Initialization of decoder                             *
	 *-----------------------------------------------------------------*/
	synth = synth_buf + M;

	reset_dec(); /* Bring the decoder and receive DTX to the initial state */

	/*-----------------------------------------------------------------*
	 *            Loop for each "L_FRAME" speech data                  *
	 *-----------------------------------------------------------------*/
	frame = 0;
	out_offset = 0;

	unsigned int mq_prio;

	for (total_frames=0 ; total_frames<1/*FRAMES*/ ; total_frames++) {
		//Read one encoded frame:
 		for (aux=0 ; aux<SERIAL_SIZE+2 ; aux++) {
 			serial[aux] = spch_dec[L_FRAME*frame+aux];
 		}
		++frame;

		SID_flag = serial[SERIAL_SIZE];		/* Receive SID flag */
		TAF = serial[SERIAL_SIZE+1];		/* Receive TAF flag */

		Bits2prm_12k2(serial, parm);		/* serial to parameters   */

		if (parm[0] == 0) {
			/* BFI == 0, perform DHF check */
            if (reset_flag_old == 1) {
				/* Check for second and further successive DHF (to first subfr.) */
				reset_flag = decoder_homing_frame_test(&parm[1], TO_FIRST_SUBFRAME);
			}
			else {
				reset_flag = 0;
			}
		}
		else {
			/* BFI==1, bypass DHF check (frame is taken as not being a DHF) */
			reset_flag = 0;
		}

		if ((reset_flag != 0) && (reset_flag_old != 0)) {
			/* Force the output to be the encoder homing frame pattern */
			for (i = 0; i < L_FRAME; i++) {
				synth[i] = EHF_MASK;
			}
		}
		else {
			Decoder_12k2(parm, synth, Az_dec, TAF, SID_flag);	/* Synthesis */
			Post_Filter(synth, Az_dec);							/* Post-filter */

			for (i = 0; i < L_FRAME; i++)  {
				/* Upscale the 15 bit linear PCM to 16 bits, then truncate to 13 bits */
				temp = shl (synth[i], 1);
				synth[i] = temp & 0xfff8;
				logic16();
				move16();
			}

		}

		/* Write the bit stream to output */
		for (i = 0 ; i < L_FRAME ; i++) {
			spch_out[out_offset++] = synth[i];
		}

        /* BFI == 0, perform check for first DHF (whole frame) */
		if ((parm[0] == 0) && (reset_flag_old == 0)) {
			reset_flag = decoder_homing_frame_test (&parm[1], WHOLE_FRAME);
		}

		if (reset_flag != 0) {
			/* Bring the decoder and receive DTX to the home state */
			reset_dec();
		}
		reset_flag_old = reset_flag;
	} /* End while */

	return 0;
}
