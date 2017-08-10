/**
 * \file dct.h
 */
#ifndef DCT_H
#define DCT_H

extern signed int weights[512];
extern signed short dctresult[MATRIX_SIZE][MATRIX_SIZE];
extern signed int weights[512];

int dct_init_start();
int dct_stop_done();
void dct(signed char pixels[8][8], int color);


#endif /* DCT_H */
