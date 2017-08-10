/**
 * \file io.c
 */

#include "ejpgl.h"
#include "stream.h"

extern stream_t *din;
extern stream_t *dout;

#define __I386
#define __LITTLEENDIAN

void writejpegfooter(stream_t *data);
void readbmpfile(stream_t *data, signed char pixelmatrix[MACRO_BLOCK_SIZE][MACRO_BLOCK_SIZE*3],
				 unsigned int mrow, unsigned int mcol, INFOHEADER * header);


void get_MB(stream_t *data, int mb_row, int mb_col, signed char pixelmatrix[MACRO_BLOCK_SIZE][MACRO_BLOCK_SIZE*3],
			INFOHEADER *bmpheader) {
	readbmpfile(data, pixelmatrix, mb_row, mb_col, bmpheader);
}


void put_char(unsigned char c) {
	swrite(&c, 1, 1, dout);
}


int getbmpheader(INFOHEADER *header, stream_t *str) {
	if (sseek(str, 14, SEEK_SET) != 0) {
		//Error in fseek
		return 0;
	}
	sread(header, sizeof(INFOHEADER), 1, str);
	if (sseek(str, 1024, SEEK_CUR) != 0) {
		//Error in fseek
		return 0;
	}
	return 1;
}


void writejpegfooter(stream_t *data) {
	unsigned char footer[2];
	footer[0] = 0xff;
	footer[1] = 0xd9;
	swrite(footer, sizeof(footer), 1, data);
	return;

}

static unsigned char buffer[MACRO_BLOCK_SIZE*3];  // move array on main memory

void readbmpfile(stream_t *data, signed char pixelmatrix[MACRO_BLOCK_SIZE][MACRO_BLOCK_SIZE*3],
				 unsigned int mrow, unsigned int mcol, INFOHEADER * header) {
	unsigned int row, col;
	for (row = 0;row < MACRO_BLOCK_SIZE; row++) {
		//Find first point of row in the matrix to be read.
		sseek(data, -(3*header->width*(row + 1 + mrow*MACRO_BLOCK_SIZE) - (MACRO_BLOCK_SIZE*3)*mcol), SEEK_END);
		//Read row from matrix
		sread(buffer, 1, MACRO_BLOCK_SIZE*3, data);
		for (col = 0; col < MACRO_BLOCK_SIZE*3; col++) {
			pixelmatrix[row][col] = buffer[col] - 128;
		}
	}
	return;
}


