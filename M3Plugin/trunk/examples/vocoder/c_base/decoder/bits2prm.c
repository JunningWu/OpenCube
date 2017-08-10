/*************************************************************************
 *
 *  FUNCTION:  Bits2prm_12k2
 *
 *  PURPOSE: Retrieves the vector of encoder parameters from the received
 *           serial bits in a frame.
 *
 *  DESCRIPTION: The encoder parameters are:
 *
 *     BFI      bad frame indicator      1 bit
 *
 *     LPC:
 *              1st codebook             7 bit
 *              2nd codebook             8 bit
 *              3rd codebook             8+1 bit
 *              4th codebook             8 bit
 *              5th codebook             6 bit
 *
 *     1st and 3rd subframes:
 *           pitch period                9 bit
 *           pitch gain                  4 bit
 *           codebook index              35 bit
 *           codebook gain               5 bit
 *
 *     2nd and 4th subframes:
 *           pitch period                6 bit
 *           pitch gain                  4 bit
 *           codebook index              35 bit
 *           codebook gain               5 bit
 *
 *************************************************************************/

#include "typedef.h"
#include "basic_op.h"

#define BIT_0     0
#define BIT_1     1

//*************************************************************************
// *
// *  FUNCTION:  Bin2int                   
// *
// *  PURPOSE: Read "no_of_bits" bits from the array bitstream[] and convert
// *           to integer.
// *
// *************************************************************************/

Word16 Bin2int (        /* Reconstructed parameter                      */
    Word16 no_of_bits,  /* input : number of bits associated with value */
    Word16 *bitstream   /* output: address where bits are written       */
)
{
    Word16 value, i, bit;

    value = 0;
    for (i = 0; i < no_of_bits; i++)
    {
        value = shl (value, 1);
        bit = *bitstream++;
        if (sub (bit, BIT_1) == 0)
            value = add (value, 1);
    }
    return (value);
}
