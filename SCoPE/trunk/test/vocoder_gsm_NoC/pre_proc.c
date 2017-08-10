/*************************************************************************
 *
 *  FUNCTION:  Pre_Process()
 *
 *  PURPOSE: Preprocessing of input speech.
 *
 *  DESCRIPTION:
 *     - 2nd order high pass filtering with cut off frequency at 80 Hz.
 *     - Divide input by two.
 *
 *************************************************************************/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "count.h"

/*------------------------------------------------------------------------*
 *                                                                        *
 * Algorithm:                                                             *
 *                                                                        *
 *  y[i] = b[0]*x[i]/2 + b[1]*x[i-1]/2 + b[2]*x[i-2]/2                    *
 *                     + a[1]*y[i-1]   + a[2]*y[i-2];                     *
 *                                                                        *
 *                                                                        *
 *  Input is divided by two in the filtering process.                     *
 *------------------------------------------------------------------------*/

/* filter coefficients (fc = 80 Hz, coeff. b[] is divided by 2) */

static const Word16 b[3] = {1899, -3798, 1899};
static const Word16 a[3] = {4096, 7807, -3733};

/* Static values to be preserved between calls */
/* y[] values are kept in double precision     */

static Word16 y2_hi, y2_lo, y1_hi, y1_lo, x0, x1;

/* Initialization of static values */

void Init_Pre_Process (void)
{
    y2_hi = 0;
    y2_lo = 0;
    y1_hi = 0;
    y1_lo = 0;
    x0 = 0;
    x1 = 0;
}

void Pre_Process (
    Word16 signal[], /* input/output signal */
    Word16 lg)       /* lenght of signal    */
{
    Word16 i, x2;
    Word32 L_tmp;

    for (i = 0; i < lg; i++)
    {
        x2 = x1;                   move16 (); 
        x1 = x0;                   move16 (); 
        x0 = signal[i];            move16 (); 

        /*  y[i] = b[0]*x[i]/2 + b[1]*x[i-1]/2 + b140[2]*x[i-2]/2  */
        /*                     + a[1]*y[i-1] + a[2] * y[i-2];      */

        L_tmp = Mpy_32_16 (y1_hi, y1_lo, a[1]);
        L_tmp = L_add (L_tmp, Mpy_32_16 (y2_hi, y2_lo, a[2]));
        L_tmp = L_mac (L_tmp, x0, b[0]);
        L_tmp = L_mac (L_tmp, x1, b[1]);
        L_tmp = L_mac (L_tmp, x2, b[2]);
        L_tmp = L_shl (L_tmp, 3);
        signal[i] = round_z (L_tmp); move16 (); 

        y2_hi = y1_hi;             move16 (); 
        y2_lo = y1_lo;             move16 (); 
        L_Extract (L_tmp, &y1_hi, &y1_lo);
    }
    return;
}
