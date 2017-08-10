/*************************************************************************
 *
 *  FUNCTION:   G_code
 *
 *  PURPOSE:  Compute the innovative codebook gain.
 *
 *  DESCRIPTION:
 *      The innovative codebook gain is given by
 *
 *              g = <x[], y[]> / <y[], y[]>
 *
 *      where x[] is the target vector, y[] is the filtered innovative
 *      codevector, and <> denotes dot product.
 *
 *************************************************************************/

#include "typedef.h"
#include "basic_op.h"
#include "count.h"
#include "cnst.h"

Word16 G_code (         /* out   : Gain of innovation code         */
    Word16 xn2[],       /* in    : target vector                   */
    Word16 y2[]         /* in    : filtered innovation vector      */
)
{
    Word16 i;
    Word16 xy, yy, exp_xy, exp_yy, gain;
    Word16 scal_y2[L_SUBFR];
    Word32 s;

    /* Scale down Y[] by 2 to avoid overflow */

    for (i = 0; i < L_SUBFR; i++)
    {
        scal_y2[i] = shr (y2[i], 1);  move16 (); 
    }

    /* Compute scalar product <X[],Y[]> */

    s = 1L;                           move32 (); /* Avoid case of all zeros */
    for (i = 0; i < L_SUBFR; i++)
    {
        s = L_mac (s, xn2[i], scal_y2[i]);
    }
    exp_xy = norm_l (s);
    xy = extract_h (L_shl (s, exp_xy));

    /* If (xy < 0) gain = 0  */

    test (); 
    if (xy <= 0)
        return ((Word16) 0);

    /* Compute scalar product <Y[],Y[]> */

    s = 0L;                           move32 (); 
    for (i = 0; i < L_SUBFR; i++)
    {
        s = L_mac (s, scal_y2[i], scal_y2[i]);
    }
    exp_yy = norm_l (s);
    yy = extract_h (L_shl (s, exp_yy));

    /* compute gain = xy/yy */

    xy = shr (xy, 1);                 /* Be sure xy < yy */
    gain = div_s (xy, yy);

    /* Denormalization of division */
    i = add (exp_xy, 5);              /* 15-1+9-18 = 5 */
    i = sub (i, exp_yy);

    gain = shr (gain, i);

    return (gain);
}
