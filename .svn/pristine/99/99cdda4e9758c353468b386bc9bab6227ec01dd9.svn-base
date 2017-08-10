/**
 * \file zzq.h
 */
#ifndef ZZQ_H
#define ZZQ_H

#include "ejpgl.h"

extern unsigned char quantization_table[MATRIX_SIZE][MATRIX_SIZE];
extern signed char bitstream[NUMBER_OF_PIXELS] ;

int zzq_encode_init_start(int compression);
int zzq_encode_stop_done();
void zzq_encode(signed short pixelmatrix[MATRIX_SIZE][MATRIX_SIZE], int color);

#endif /* ZZQ_H */
