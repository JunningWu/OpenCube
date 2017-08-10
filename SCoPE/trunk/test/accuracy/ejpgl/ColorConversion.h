/**
 * \file ColorConversion.h
 */
#ifndef COLOR_CONVERSION_H
#define COLOR_CONVERSION_H

#include "ejpgl.h"

void RGB2YCrCb(signed char pixelmatrix[MACRO_BLOCK_SIZE][MACRO_BLOCK_SIZE*3], signed char YMatrix[MATRIX_SIZE][MATRIX_SIZE],
			   signed char CrMatrix[MATRIX_SIZE][MATRIX_SIZE], signed char CbMatrix[MATRIX_SIZE][MATRIX_SIZE], unsigned int sample);

#endif /* COLOR_CONVERSION_H */
