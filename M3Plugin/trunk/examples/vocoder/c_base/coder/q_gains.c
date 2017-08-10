/*--------------------------------------------------------------------------*
 * Function q_gain_pitch(), q_gain_code()                                  *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                                  *
 * Scalar quantization of the pitch gain and the innovative codebook gain.  *
 *                                                                          *
 * MA prediction is performed on the innovation energy                      *
 * (in dB/(20*log10(2))) with mean removed.                                 *
 *-------------------------------------------------------------------------*/

#include "../common/typedef.h"
#include "../common/cnst.h"
#include "../common/basic_op.h"
#include "../common/oper_32b.h"
#include "../common/sig_proc.h"
#include "../common/gains_tb.h"

#include "dtx_cod.h"

Word16 q_gain_pitch (   /* Return index of quantization */
    Word16 *gain        /* (i)  :  Pitch gain to quantize  */
) {

    Word16 i, index, gain_q14, err, err_min;

    gain_q14 = shl (*gain, 2);

    err_min = abs_s (sub (gain_q14, qua_gain_pitch[0]));
    index = 0;                                   

    for (i = 1; i < NB_QUA_PITCH; i++)
    {
        err = abs_s (sub (gain_q14, qua_gain_pitch[i]));

         
        if (sub (err, err_min) < 0)
        {
            err_min = err;                       
            index = i;                           
        }
    }

    *gain = shr (qua_gain_pitch[index], 2);      

    return index;
}
