/**
 * \file main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "stream.h"

int ejpgl_main(stream_t *data_in, stream_t *data_out);

int main(int argc, char* argv[]) {
	int ret;
	FILE *infile, *outfile;
	stream_t data_in, data_out;
	struct stat file_status;
	
	//Open input file:
	if (stat(argv[1], &file_status) != 0) {
		return 1;
	}
// 	if (!S_ISREG(file_status)) {
// 		printf("BMP image %s is not a regular file\n", argv[1]);
// 		return 1;
// 	}
	infile = fopen(argv[1], "rb");
	if (infile == NULL) {
		printf("Cannot open intput file %s\n", argv[1]);
		return 1;
	}
		
	//Open output file:
	outfile = fopen(argv[2], "wb");
	if (outfile == NULL) {
		printf("Cannot open output file %s\n", argv[2]);
		return 1;
	}
	
	//Initialize streams:
	data_in.size = file_status.st_size;
	data_in.data = (byte_t *)malloc(file_status.st_size);
	data_in.pos = data_in.data;
	fread(data_in.data, sizeof(byte_t), file_status.st_size, infile);
	data_out.data = (byte_t *)malloc(file_status.st_size);
	data_out.pos = data_out.data;
	data_out.size = 0;
		
	ret = ejpgl_main(&data_in, &data_out);
	
	fwrite(data_out.data, sizeof(byte_t), data_out.size, outfile);
	
	free(data_in.data);
	free(data_out.data);
	return ret;
}


