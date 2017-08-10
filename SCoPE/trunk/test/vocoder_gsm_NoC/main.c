/*!
 * \file main.c
 * \brief gsm vocoder execution
 */

//System includes
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// User includes
#include "coder.h"
#include "decoder.h"
#include "ed_iface.h"

/*
 * User application
 */

void * coder_main(void *a) {
	Word16 spch_inp[45600];
//	#include "rom_ns_content285.h"
	coder(spch_inp);
	printf("El coder ha acabao!!\n");
}

void * decoder_main(void *a) {
	Word16 spch_dec[247];
	Word16 spch_out[160];
	int i;
	int fd;
	if ((fd = open("/dev/net/tun", 0)) < 0) {
		printf("Error, opening /dev/net/tun\n");
		exit(1);
	}

	for (i = 0; i < 285; i++) {
		ed_iface(fd, spch_dec);
		decoder(spch_dec, spch_out);
	}

	close(fd);

	printf("Ha acabao todo!!!\n");

}

