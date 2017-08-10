/*!
 * \file main.c
 * \brief gsm vocoder execution
 */

// User includes
#include "coder.h"
#include "decoder.h"
#include "ed_iface.h"
#include <unistd.h>
#include <stdlib.h>

/*
 * User application
 */

void * coder_main(void *a) {
	Word16 spch_inp[45600];
	#include "rom_ns_content285.h"
	coder(spch_inp);
}

void * decoder_main(void *a) {
	Word16 spch_dec[247];
	Word16 spch_out[160];
	int fd;
	if ((fd = open("/dev/net/tun", 0)) < 0) {
		printf("Error, opening /dev/net/tun\n");
		exit(1);
	}

	for (int i = 0; i < 285; i++) {
		ed_iface(fd, spch_dec);
		decoder(spch_dec, spch_out);
	}

	close(fd);
}

#if 0

#include <stdlib.h>
#include "coder.h"
#include "decoder.h"
#include "ed_iface.h"

int main(int argc, char *argv[]) {
	static Word16 spch_inp[1600];
	static Word16 spch_cod[2500];
	static Word16 spch_dec[2500];
	static Word16 spch_out[1600];

// 	Word16 *spch_inp = (Word16 *)malloc(1600*sizeof(Word16));
// 	Word16 *spch_cod = (Word16 *)malloc(2500*sizeof(Word16));
// 	Word16 *spch_dec = (Word16 *)malloc(2500*sizeof(Word16));
// 	Word16 *spch_out = (Word16 *)malloc(1600*sizeof(Word16));

	//Read the input samples:
	#include "input_data.dat"

	coder(spch_inp, spch_cod);
	ed_iface(spch_cod, spch_dec);
	decoder(spch_dec, spch_out);

	return 0;
}

#endif
