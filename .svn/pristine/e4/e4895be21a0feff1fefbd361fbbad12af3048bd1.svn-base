/***************************************************************************
 *
 *  FILE NAME:  CODER.C
 *
 *  Main program of the EFR coder at 12.2 kbit/s.
 *
 *    Usage : coder speech_file  bitstream_file
 *
 *    Format for speech_file:
 *      Speech is read from a binary file of 16 bits data.
 *
 *    Format for bitstream_file:
 *      244  words (2-byte) containing 244 bits.
 *          Bit 0 = 0x0000 and Bit 1 = 0x0001
 *      One word (2-byte) for voice activity decision (VAD) flag bit
 *          0x0000 -> inactive (no detected speech activity);
 *          0x0001 -> active
 *      One word (2-byte) for speech (SP) flag bit
 *          0x0000 -> inactive (no transmission of speech frames);
 *          0x0001 -> active
 *
 ***************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "typedef.h"
#include "basic_op.h"
#include "sig_proc.h"
#include "count.h"
#include "codec.h"
#include "cnst.h"
#include "n_stack.h"
#include "e_homing.h"
#include "dtx.h"


Word16 dtx_mode;
extern Word16 txdtx_ctrl;

int coder(Word16 *spch_inp) {
	Word16 spch_cod[70110];
	extern Word16 *new_speech;		// Pointer to new speech data
	Word16 prm[PRM_SIZE];			// Analysis parameters
	Word16 serial[SERIAL_SIZE-1];	// Output bitstream buffer
	Word16 syn[L_FRAME];			// Buffer for synthesis speech
	Word16 frame;
	Word16 vad, sp;
	Word16 reset_flag;
	Word16 i;
	Word16 total_frames;
	Word16 aux;
	Word32 out_offset;				//The next position to write in the output buffer

	dtx_mode = 0;					//DTX disabled by default
	//-----------------------------------------------------------------------*
	// Initialisation of the coder.                                          *
	//-----------------------------------------------------------------------*
	reset_enc(); //Bring the encoder, VAD and DTX to the initial state

	//Loop for each "L_FRAME" speech data:
	frame = 0;
	out_offset = 0;

	int fd;
	Word16 data_to_send[246+12];
	memcpy(data_to_send, "\0\0\0\0\0\1\0\0\0\0\0\0", 12);
	if ((fd = open("/dev/net/tun", 0)) < 0) {
		printf("Error, opening /dev/net/tun\n");
		exit(1);
	}

	for (total_frames=0 ; total_frames<FRAMES ; total_frames++) {
		//Copy the next frame (L_FRAME samples)
		for (aux=0 ; aux<L_FRAME ; aux++) {
			new_speech[aux] = spch_inp[L_FRAME*frame+aux];
		}
		++frame;

		//Check whether this frame is an encoder homing frame:
		reset_flag = encoder_homing_frame_test(new_speech);

		for (i = 0; i < L_FRAME; i++) {
			//Delete the 3 LSBs (13-bit input):
			new_speech[i] = new_speech[i] & 0xfff8;
			logic16();
			move16 ();
		}

		//filter + downscaling:
		Pre_Process(new_speech, L_FRAME);

		//Find speech parameters
		Coder_12k2 (prm, syn);
		test();
		logic16();
		if ((txdtx_ctrl & TX_SP_FLAG) == 0) {
            //Write comfort noise parameters into the parameter frame.
            //Use old parameters in case SID frame is not to be updated
			CN_encoding(prm, txdtx_ctrl);
		}
		//Parameters to serial bits:
		Prm2bits_12k2(prm, &serial[0]);

		test ();
		logic16 ();
		if ((txdtx_ctrl & TX_SP_FLAG) == 0) {
            //Insert SID codeword into the serial parameter frame
			sid_codeword_encoding (&serial[0]);
		}

        //Write the bit stream:
		for (i = 0 ; i < SERIAL_SIZE-1 ; i++) {
			spch_cod[out_offset++] = serial[i];
		}

		//Write the VAD- and SP-flags to file after the speech parameter bit stream
		vad = 0;
		sp = 0;

		if ((txdtx_ctrl & TX_VAD_FLAG) != 0) {
			vad = 1;
		}
		if ((txdtx_ctrl & TX_SP_FLAG) != 0) {
			sp = 1;
		}
		spch_cod[out_offset++] = vad;
		spch_cod[out_offset++] = sp;

		if (reset_flag != 0) {
			//Bring the encoder, VAD and DTX to the home state
			reset_enc();
		}

		memcpy(&(data_to_send[6]), &(spch_cod[total_frames*246]), 246*sizeof(Word16));
		if (write(fd, data_to_send, 246*sizeof(Word16)+12) <= 0) {
			printf("Error, writing /dev/net/tun\n");
			exit(1);
		}
	} //End main while()

	close(fd);

	return (0);
}
