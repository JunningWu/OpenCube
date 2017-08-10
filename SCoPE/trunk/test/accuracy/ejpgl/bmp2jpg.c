/**
 * \file bmp2jpg.c
 */

#include "ejpgl.h"
#include "io.h"
#include "dct.h"
#include "ColorConversion.h"
#include "zzq.h"
#include "huffman.h"
#include "stream.h"

static INFOHEADER _bmpheader;
static JPEGHEADER _jpegheader;
stream_t *din;
stream_t *dout;

signed char pixelmatrix[MACRO_BLOCK_SIZE][MACRO_BLOCK_SIZE*3];
signed char YMatrix[MATRIX_SIZE][MATRIX_SIZE];
signed char CrMatrix[MATRIX_SIZE][MATRIX_SIZE];
signed char CbMatrix[MATRIX_SIZE][MATRIX_SIZE];

int writejpegheader(INFOHEADER *header, JPEGHEADER *jpegheader);

/////////////////////////////////////////////////////////////////////////////

int ejpgl_main(stream_t *data_in, stream_t *data_out){
	return ejpgl_main_1(data_in,data_out);
}

int ejpgl_main_1(stream_t *data_in, stream_t *data_out) {
	int compression, sample;
	unsigned int col, cols, row, rows;
	int jpegheadersize;
	INFOHEADER *bmpheader;
	JPEGHEADER *jpgheader;
	//Initialize headers:
	din = data_in;
	dout = data_out;
	bmpheader = &_bmpheader;
	jpgheader = &_jpegheader;
	if (getbmpheader(bmpheader, data_in) == 0) {
		//Error in getbmpheader 
		return 1;
	}
	jpegheadersize = writejpegheader(bmpheader, &_jpegheader);
	if (jpegheadersize == 0) {
		//Error in writejpegheader
		return 1;
	}
	swrite(jpgheader, jpegheadersize, 1, data_out);
	//Initialize coder:
	rows = bmpheader->height >> 4;
	cols = bmpheader->width >> 4;
	dct_init_start();
	zzq_encode_init_start(compression);
	vlc_init_start();
	//Start cogind:
	for (row = 0; row < rows; row++) {
		for (col = 0; col < cols; col++) {
			get_MB(data_in, row, col, pixelmatrix, bmpheader);
			for (sample = 0;sample < 5;sample++) {
				if (sample < 4) {
					RGB2YCrCb(pixelmatrix, YMatrix, CrMatrix, CbMatrix, sample);
					dct(YMatrix, 0);
				}
				else {
					dct(CrMatrix, 1);
					dct(CbMatrix, 2);
				}
			}
		}
	}
	dct_stop_done();
	zzq_encode_stop_done();
	vlc_stop_done();
	writejpegfooter(data_out);
	return 0;
}



//////////////////////////////////////////////

int ejpgl_main_2(stream_t *data_in, stream_t *data_out) {
	int compression, sample;
	unsigned int col, cols, row, rows;
	int jpegheadersize;
	INFOHEADER *bmpheader;
	JPEGHEADER *jpgheader;
	//Initialize headers:
	din = data_in;
	dout = data_out;
	bmpheader = &_bmpheader;
	jpgheader = &_jpegheader;
	if (getbmpheader(bmpheader, data_in) == 0) {
		//Error in getbmpheader 
		return 1;
	}
	jpegheadersize = writejpegheader(bmpheader, &_jpegheader);
	if (jpegheadersize == 0) {
		//Error in writejpegheader
		return 1;
	}
	swrite(jpgheader, jpegheadersize, 1, data_out);
	//Initialize coder:
	rows = bmpheader->height >> 4;
	cols = bmpheader->width >> 4;
	dct_init_start();
	zzq_encode_init_start(compression);
	vlc_init_start();
	//Start cogind:
	for (row = 0; row < rows; row++) {
		for (col = 0; col < cols; col++) {
			get_MB(data_in, row, col, pixelmatrix, bmpheader);
			for (sample = 0;sample < 5;sample++) {
				if (sample < 4) {
					RGB2YCrCb(pixelmatrix, YMatrix, CrMatrix, CbMatrix, sample);
					dct(YMatrix, 0);
				}
				else {
					dct(CrMatrix, 1);
					dct(CbMatrix, 2);
				}
			}
		}
	}
	dct_stop_done();
	zzq_encode_stop_done();
	vlc_stop_done();
	writejpegfooter(data_out);
	return 0;
}

///////////////////////////////////////////////////////////////


int ejpgl_main_3(stream_t *data_in, stream_t *data_out) {
	int compression, sample;
	unsigned int col, cols, row, rows;
	int jpegheadersize;
	INFOHEADER *bmpheader;
	JPEGHEADER *jpgheader;
	//Initialize headers:
	din = data_in;
	dout = data_out;
	bmpheader = &_bmpheader;
	jpgheader = &_jpegheader;
	if (getbmpheader(bmpheader, data_in) == 0) {
		//Error in getbmpheader 
		return 1;
	}
	jpegheadersize = writejpegheader(bmpheader, &_jpegheader);
	if (jpegheadersize == 0) {
		//Error in writejpegheader
		return 1;
	}
	swrite(jpgheader, jpegheadersize, 1, data_out);
	//Initialize coder:
	rows = bmpheader->height >> 4;
	cols = bmpheader->width >> 4;
	dct_init_start();
	zzq_encode_init_start(compression);
	vlc_init_start();
	//Start coding:
	for (row = 0; row < rows; row++) {
		for (col = 0; col < cols; col++) {
			get_MB(data_in, row, col, pixelmatrix, bmpheader);
			for (sample = 0;sample < 5;sample++) {
				if (sample < 4) {
					RGB2YCrCb(pixelmatrix, YMatrix, CrMatrix, CbMatrix, sample);
					dct(YMatrix, 0);
				}
				else {
					dct(CrMatrix, 1);
					dct(CbMatrix, 2);
				}
			}
		}
	}
	dct_stop_done();
	zzq_encode_stop_done();
	vlc_stop_done();
	writejpegfooter(data_out);
	return 0;
}

////////////////////////////////////////////////////////////////

int ejpgl_main_4(stream_t *data_in, stream_t *data_out) {
	int compression, sample;
	unsigned int col, cols, row, rows;
	int jpegheadersize;
	INFOHEADER *bmpheader;
	JPEGHEADER *jpgheader;
	//Initialize headers:
	din = data_in;
	dout = data_out;
	bmpheader = &_bmpheader;
	jpgheader = &_jpegheader;
	if (getbmpheader(bmpheader, data_in) == 0) {
		//Error in getbmpheader 
		return 1;
	}
	jpegheadersize = writejpegheader(bmpheader, &_jpegheader);
	if (jpegheadersize == 0) {
		//Error in writejpegheader
		return 1;
	}
	swrite(jpgheader, jpegheadersize, 1, data_out);
	//Initialize coder:
	rows = bmpheader->height >> 4;
	cols = bmpheader->width >> 4;
	dct_init_start();
	zzq_encode_init_start(compression);
	vlc_init_start();
	//Start cogind:
	for (row = 0; row < rows; row++) {
		for (col = 0; col < cols; col++) {
			get_MB(data_in, row, col, pixelmatrix, bmpheader);
			for (sample = 0;sample < 5;sample++) {
				if (sample < 4) {
					RGB2YCrCb(pixelmatrix, YMatrix, CrMatrix, CbMatrix, sample);
					dct(YMatrix, 0);
				}
				else {
					dct(CrMatrix, 1);
					dct(CbMatrix, 2);
				}
			}
		}
	}
	dct_stop_done();
	zzq_encode_stop_done();
	vlc_stop_done();
	writejpegfooter(data_out);
	return 0;
}

////////////////////////////////////////////////////////////////////////
int ejpgl_main_5(stream_t *data_in, stream_t *data_out) {
	int compression, sample;
	unsigned int col, cols, row, rows;
	int jpegheadersize;
	INFOHEADER *bmpheader;
	JPEGHEADER *jpgheader;
	//Initialize headers:
	din = data_in;
	dout = data_out;
	bmpheader = &_bmpheader;
	jpgheader = &_jpegheader;
	if (getbmpheader(bmpheader, data_in) == 0) {
		//Error in getbmpheader 
		return 1;
	}
	jpegheadersize = writejpegheader(bmpheader, &_jpegheader);
	if (jpegheadersize == 0) {
		//Error in writejpegheader
		return 1;
	}
	swrite(jpgheader, jpegheadersize, 1, data_out);
	//Initialize coder:
	rows = bmpheader->height >> 4;
	cols = bmpheader->width >> 4;
	dct_init_start();
	zzq_encode_init_start(compression);
	vlc_init_start();
	//Start cogind:
	for (row = 0; row < rows; row++) {
		for (col = 0; col < cols; col++) {
			get_MB(data_in, row, col, pixelmatrix, bmpheader);
			for (sample = 0;sample < 5;sample++) {
				if (sample < 4) {
					RGB2YCrCb(pixelmatrix, YMatrix, CrMatrix, CbMatrix, sample);
					dct(YMatrix, 0);
				}
				else {
					dct(CrMatrix, 1);
					dct(CbMatrix, 2);
				}
			}
		}
	}
	dct_stop_done();
	zzq_encode_stop_done();
	vlc_stop_done();
	writejpegfooter(data_out);
	return 0;
}

