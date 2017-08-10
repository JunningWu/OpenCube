/*************************************************************************
 *
 *  FUNCTION:  G_pitch
 *
 *  PURPOSE:  Compute the pitch (adaptive codebook) gain. Result in Q12
 *
 *  DESCRIPTION:
 *      The adaptive codebook gain is given by
 *
 *              g = <x[], y[]> / <y[], y[]>
 *
 *      where x[] is the target vector, y[] is the filtered adaptive
 *      codevector, and <> denotes dot product.
 *      The gain is limited to the range [0,1.2]
 *
 *************************************************************************/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "count.h"
#include "sig_proc.h"

Word16 G_pitch (        /* (o)   : Gain of pitch lag saturated to 1.2      */
    Word16 xn[],        /* (i)   : Pitch target.                           */
    Word16 y1[],        /* (i)   : Filtered adaptive codebook.             */
    Word16 L_subfr      /*       : Length of subframe.                     */
)
{
    Word16 i;
    Word16 xy, yy, exp_xy, exp_yy, gain;
    Word32 s;

    Word16 scaled_y1[80];       /* Usually dynamic allocation of (L_subfr) */

    /* divide by 2 "y1[]" to avoid overflow */

    for (i = 0; i < L_subfr; i++)
    {
        scaled_y1[i] = shr (y1[i], 2); move16 (); 
    }

    /* Compute scalar product <y1[],y1[]> */

    s = 0L;                            move32 (); /* Avoid case of all zeros */
    for (i = 0; i < L_subfr; i++)
    {
        s = L_mac (s, y1[i], y1[i]);
    }
    test (); 
    if (L_sub (s, MAX_32) != 0L)       /* Test for overflow */
    {
        s = L_add (s, 1L);             /* Avoid case of all zeros */
        exp_yy = norm_l (s);
        yy = round_z (L_shl (s, exp_yy));
    }
    else
    {
        s = 1L;                        move32 (); /* Avoid case of all zeros */
        for (i = 0; i < L_subfr; i++)
        {
            s = L_mac (s, scaled_y1[i], scaled_y1[i]);
        }
        exp_yy = norm_l (s);
        yy = round_z (L_shl (s, exp_yy));
        exp_yy = sub (exp_yy, 4);
    }

    /* Compute scalar product <xn[],y1[]> */

    Overflow = 0;                      move16 (); 
    s = 1L;                            move32 (); /* Avoid case of all zeros */
    for (i = 0; i < L_subfr; i++)
    {
        Carry = 0;                     move16 ();
        s = L_macNs (s, xn[i], y1[i]);

        test ();
        if (Overflow != 0)
        {
	    break;
        }
    }
    test (); 
    if (Overflow == 0)
    {
        exp_xy = norm_l (s);
        xy = round_z (L_shl (s, exp_xy));
    }
    else
    {
        s = 1L;                        move32 (); /* Avoid case of all zeros */
        for (i = 0; i < L_subfr; i++)
        {
            s = L_mac (s, xn[i], scaled_y1[i]);
        }
        exp_xy = norm_l (s);
        xy = round_z (L_shl (s, exp_xy));
        exp_xy = sub (exp_xy, 2);
    }

    /* If (xy < 4) gain = 0 */

    i = sub (xy, 4);

    test (); 
    if (i < 0)
        return ((Word16) 0);

    /* compute gain = xy/yy */

    xy = shr (xy, 1);                  /* Be sure xy < yy */
    gain = div_s (xy, yy);

    i = add (exp_xy, 3 - 1);           /* Denormalization of division */
    i = sub (i, exp_yy);

    gain = shr (gain, i);

    /* if(gain >1.2) gain = 1.2 */

    test (); 
    if (sub (gain, 4915) > 0)
    {
        gain = 4915;                   move16 (); 
    }
    return (gain);
}
