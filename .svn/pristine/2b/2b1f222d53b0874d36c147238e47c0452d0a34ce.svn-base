/***************************************************************************
 *
 *     File Name: ed_iface.c
 *
 *     Purpose: Speech encoder/decoder interface device
 *
 *     This program transforms the output file format of the GSM Enhanced
 *     Full Rate Encoder module consisting of:
 *         * 244 speech parameter bits (see GSM TS 06.60)
 *         *   1 voice activity flag VAD (see GSM TS 06.82)
 *         *   1 speech flag SP (see GSM TS 06.81)
 *
 *     to the input file format of the GSM Enhanced Full Rate Decoder module
 *     requiring:
 *         *   1 channel condition flag BFI (see GSM TS 06.61, 05.05)
 *         * 244 speech parameter bits (see GSM TS 06.60)
 *         *   1 SID flag (2 bits) (see GSM TS 06.81, 05.05)
 *         *   1 time alignment flag TAF (see GSM TS 06.81)
 *
 *     Between SID updates the speech parameters are replaced by random
 *     values simulating an interrupted transmission on the air interface
 *
 *     Below is a listing of all the functions appearing in the file,
 *     with a short description of their purpose.
 *
 *     Convert single frame from encoder output format to decoder
 *     input format:
 *       enc_dec_interface()
 *     Receive single encoder output parameter frame:
 *       encoder_interface()
 *     Send single decoder input parameter frame:
 *       decoder_interface()
 *     Open file for binary read or write:
 *       open_bin_file()
 *     Set the speech parameters to random values:
 *       random_parameters()
 *
 **************************************************************************/

/***************************************************************************
 *
 *      Include-Files
 *
 **************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "typedef.h"
#include "cnst.h"
#include "dtx.h"


#define OPEN_WB  "wb"
#define OPEN_RB  "rb"

/***************************************************************************
 *
 *      Local function prototypes
 *
 **************************************************************************/
static Word16 enc_dec_interface (int fd, Word16 *spch_dec);
static Word16 encoder_interface (int fd, Word16 *spch_cod, Word16 serial_in_para[]);
static Word16 decoder_interface (Word16 serial_out_file[], Word16 *spch_dec);
static void random_parameters (Word16 serial_params[]);
Word16 module(Word16 num, Word16 div);

/***************************************************************************
 *
 *      Local functions
 *
 **************************************************************************/

Word16 module(Word16 num, Word16 mod) {
	Word16 ret = num;
	if (num <= mod) {
		ret = 0;
	}
	else {
		while (ret >= mod) {
			ret -= mod;
		}
	}
	return ret;
}


static Word16 encoder_interface (int fd, Word16 *spch_cod, Word16 serial_in_para[]) {
// 	static Word16 num_frames = 0;
	Word16 i;
// 	if (num_frames == FRAMES) {
// 		return 1;
// 	}

	Word16 serial_in_para_temp[246+12];
	if (read(fd, serial_in_para_temp, 246*sizeof(Word16)+12) <= 0) {
		printf("Error, reading /dev/net/tun\n");
		exit(1);
	}
	memcpy(serial_in_para, &(serial_in_para_temp[6]), 246*sizeof(Word16));

// 	for (i=0 ; i<246 ; i++) {
// 		serial_in_para[i] = spch_cod[247*num_frames+i];
// 	}
// 	num_frames++;
	return 0;
}


static Word16 decoder_interface (Word16 serial_out_file[], Word16 *spch_dec) {
// 	static Word16 num_frames = 0;
	Word16 i;
// 	if (num_frames == FRAMES) {
// 		return 1;
// 	}
	for (i=0 ; i<247 ; i++) {
// 		spch_dec[247*num_frames+i] = serial_out_file[i];
		spch_dec[i] = serial_out_file[i];
	}
// 	num_frames++;
	return 0;
}


static void random_parameters (Word16 serial_params[]) {
	static Word32 L_PN_seed = 0x321CEDE2L;
	Word16 i;
	/* Set the 244 speech parameter bits to random bit values */
	/* Function pseudonoise() is defined in dtx.c             */
	/*--------------------------------------------------------*/
	for (i = 0; i < 244; i++) {
		serial_params[i] = pseudonoise (&L_PN_seed, 1);
	}
	return;
}


static Word16 enc_dec_interface (int fd, Word16 *spch_dec) {

	#define SPEECH      1
	#define CNIFIRSTSID 2
	#define CNICONT     3
	#define VALIDSID    11
	#define GOODSPEECH  33

	Word16 spch_cod[246];
	static Word16 decoding_mode = {SPEECH};
	static Word16 TAF_count = {1};
	Word16 serial_in_para[246], i, frame_type;
	Word16 serial_out_para[247];

	if (encoder_interface (fd, spch_cod, serial_in_para) != 0) {
		return (1);
	}

	/* Copy input parameters to output parameters */
	/* ------------------------------------------ */
	for (i = 0; i < 244; i++) {
		serial_out_para[i+1] = serial_in_para[i];
	}

	/* Set channel status (BFI) flag to zero */
	/* --------------------------------------*/
	serial_out_para[0] = 0;     /* BFI flag */

	/* Evaluate SID flag                                  */
	/* Function sid_frame_detection() is defined in dtx.c */
	/* -------------------------------------------------- */
	serial_out_para[245] = sid_frame_detection(&serial_out_para[1]);

	/* Evaluate TAF flag */
	/* ----------------- */
	if (TAF_count == 0) {
		serial_out_para[246] = 1;
	}
	else {
		serial_out_para[246] = 0;
	}

	TAF_count = module((TAF_count + 1), 24);

	/* Frame classification:                                                */
	/* Since the transmission is error free, the received frames are either */
	/* valid speech or valid SID frames                                     */
	/* -------------------------------------------------------------------- */
	if (serial_out_para[245] == 2) {
		frame_type = VALIDSID;
	}
	else if (serial_out_para[245] == 0) {
		frame_type = GOODSPEECH;
	}
	else {
		return (1);
	}

	/* Update of decoder state */
	/* ----------------------- */
	if (decoding_mode == SPEECH) {
		/* State of previous frame */
		if (frame_type == VALIDSID) {
			decoding_mode = CNIFIRSTSID;
		}
		else if (frame_type == GOODSPEECH) {
			decoding_mode = SPEECH;
		}
	}
	else {
		/* comfort noise insertion mode */
		if (frame_type == VALIDSID) {
			decoding_mode = CNICONT;
		}
		else if (frame_type == GOODSPEECH) {
			decoding_mode = SPEECH;
		}
	}

	/* Replace parameters by random data if in CNICONT-mode and TAF=0 */
	/* -------------------------------------------------------------- */
	if ((decoding_mode == CNICONT) && (serial_out_para[246] == 0)) {
		random_parameters (&serial_out_para[1]);
		/* Set flags such that an "unusable frame" is produced */
		serial_out_para[0] = 1;       /* BFI flag */
		serial_out_para[245] = 0;     /* SID flag */
	}

	if (decoder_interface (serial_out_para, spch_dec) != 0) {
		return (1);
	}

	return (0);
}


/****************************************************************************
 *
 *          Main program of the encoder/decoder interface device
 *
 ***************************************************************************/
int ed_iface(int fd, Word16 *spch_dec) {

// 	while (enc_dec_interface(fd, spch_dec) == 0) { }
	enc_dec_interface(fd, spch_dec);

	return (0);
}
