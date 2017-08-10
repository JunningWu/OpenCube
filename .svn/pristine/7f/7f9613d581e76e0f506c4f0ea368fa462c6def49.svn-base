/**
 * \file io.h
 */
#ifndef IO_H
#define IO_H

#include "ejpgl.h"
#include "stream.h"

void writejpegfooter(stream_t *data);
void readbmpfile(stream_t *data, signed char pixelmatrix[MACRO_BLOCK_SIZE][MACRO_BLOCK_SIZE*3],
				 unsigned int mrow, unsigned int mcol, INFOHEADER * header);
void get_MB(stream_t *data, int mb_row, int mb_col, signed char pixelmatrix[MACRO_BLOCK_SIZE][MACRO_BLOCK_SIZE*3],
			INFOHEADER *bmpheader);
void put_char(unsigned char c);
int getbmpheader(INFOHEADER *header, stream_t *str);
void writejpegfooter(stream_t *data);
void readbmpfile(stream_t *data, signed char pixelmatrix[MACRO_BLOCK_SIZE][MACRO_BLOCK_SIZE*3],
				 unsigned int mrow, unsigned int mcol, INFOHEADER * header);


#endif /* IO_H */
