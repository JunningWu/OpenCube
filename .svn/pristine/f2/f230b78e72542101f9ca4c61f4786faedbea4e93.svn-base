/*--------------------------------------------------------------*
* Function prototypes for general SIGnal PROCessing functions. *
*--------------------------------------------------------------*/

/* Mathematic functions  */

Word32 Inv_sqrt (      /* (o) : output value   (range: 0<=val<1)            */
    Word32 L_x         /* (i) : input value    (range: 0<=val<=7fffffff)    */
);
void Log2 (
    Word32 L_x,        /* (i) : input value                                 */
    Word16 *exponent,  /* (o) : Integer part of Log2.   (range: 0<=val<=30) */
    Word16 *fraction   /* (o) : Fractional part of Log2. (range: 0<=val<1)*/
);
Word32 Pow2 (          /* (o) : result       (range: 0<=val<=0x7fffffff)    */
    Word16 exponent,   /* (i) : Integer part.      (range: 0<=val<=30)      */
    Word16 fraction    /* (i) : Fractional part.  (range: 0.0<=val<1.0)     */
);

/* General signal processing */

void Init_Pre_Process (void);
void Pre_Process (
    Word16 signal[],   /* Input/output signal                               */
    Word16 lg          /* Lenght of signal                                  */
);

Word16 Autocorr (
    Word16 x[],        /* (i)    : Input signal                             */
    Word16 m,          /* (i)    : LPC order                                */
    Word16 r_h[],      /* (o)    : Autocorrelations  (msb)                  */
    Word16 r_l[],      /* (o)    : Autocorrelations  (lsb)                  */
    const Word16 wind[]/* (i)    : window for LPC analysis.                 */
);
void Lag_window (
    Word16 m,          /* (i)    : LPC order                                */
    Word16 r_h[],      /* (i/o)  : Autocorrelations  (msb)                  */
    Word16 r_l[]       /* (i/o)  : Autocorrelations  (lsb)                  */
);
void Levinson (
    Word16 Rh[],       /* (i)    : Rh[m+1] Vector of autocorrelations (msb) */
    Word16 Rl[],       /* (i)    : Rl[m+1] Vector of autocorrelations (lsb) */
    Word16 A[],        /* (o)    : A[m]    LPC coefficients  (m = 10)       */
    Word16 rc[]        /* (o)    : rc[4]   First 4 reflection coefficients  */
);
void Az_lsp (
    Word16 a[],        /* (i)    : predictor coefficients                   */
    Word16 lsp[],      /* (o)    : line spectral pairs                      */
    Word16 old_lsp[]   /* (i)    : old lsp[] (in case not found 10 roots)   */
);
void Lsp_Az (
    Word16 lsp[],      /* (i)    : line spectral frequencies                */
    Word16 a[]         /* (o)    : predictor coefficients (order = 10)      */
);
void Lsf_lsp (
    Word16 lsf[],      /* (i)    : lsf[m] normalized (range: 0.0<=val<=0.5) */
    Word16 lsp[],      /* (o)    : lsp[m] (range: -1<=val<1)                */
    Word16 m           /* (i)    : LPC order                                */
);
void Lsp_lsf (
    Word16 lsp[],      /* (i)    : lsp[m] (range: -1<=val<1)                */
    Word16 lsf[],      /* (o)    : lsf[m] normalized (range: 0.0<=val<=0.5) */
    Word16 m           /* (i)    : LPC order                                */
);
void Reorder_lsf (
    Word16 *lsf,       /* (i/o)  : vector of LSFs   (range: 0<=val<=0.5)    */
    Word16 min_dist,   /* (i)    : minimum required distance                */
    Word16 n           /* (i)    : LPC order                                */
);
void Weight_Fac (
    Word16 gamma,      /* (i)    : Spectral expansion.                      */
    Word16 fac[]       /* (i/o)  : Computed factors.                        */
);
void Weight_Ai (
    Word16 a[],        /* (i)  : a[m+1]  LPC coefficients   (m=10)          */
    const Word16 fac[],/* (i)  : Spectral expansion factors.                */
    Word16 a_exp[]     /* (o)  : Spectral expanded LPC coefficients         */
);
void Residu (
    Word16 a[],        /* (i)  : prediction coefficients                    */
    Word16 x[],        /* (i)  : speech signal                              */
    Word16 y[],        /* (o)  : residual signal                            */
    Word16 lg          /* (i)  : size of filtering                          */
);
void Syn_filt (
    Word16 a[],        /* (i)  : a[m+1] prediction coefficients   (m=10)    */
    Word16 x[],        /* (i)  : input signal                               */
    Word16 y[],        /* (o)  : output signal                              */
    Word16 lg,         /* (i)  : size of filtering                          */
    Word16 mem[],      /* (i/o): memory associated with this filtering.     */
    Word16 update      /* (i)  : 0=no update, 1=update of memory.           */
);
void Convolve (
    Word16 x[],        /* (i)  : input vector                               */
    Word16 h[],        /* (i)  : impulse response                           */
    Word16 y[],        /* (o)  : output vector                              */
    Word16 L           /* (i)  : vector size                                */
);
void agc (
    Word16 *sig_in,    /* (i)  : postfilter input signal                    */
    Word16 *sig_out,   /* (i/o): postfilter output signal                   */
    Word16 agc_fac,    /* (i)  : AGC factor                                 */
    Word16 l_trm       /* (i)  : subframe size                              */
);
void agc2 (
    Word16 *sig_in,    /* (i)  : postfilter input signal                    */
    Word16 *sig_out,   /* (i/o): postfilter output signal                   */
    Word16 l_trm       /* (i)  : subframe size                              */
);
void preemphasis (
    Word16 *signal,    /* (i/o): input signal overwritten by the output     */
    Word16 g,          /* (i)  : preemphasis coefficient                    */
    Word16 L           /* (i)  : size of filtering                          */
);

/* General */

void Copy (
    Word16 x[],        /* (i)  : input vector                               */
    Word16 y[],        /* (o)  : output vector                              */
    Word16 L           /* (i)  : vector length                              */
);
void Set_zero (
    Word16 x[],        /* (o)  : vector to clear                            */
    Word16 L           /* (i)  : length of vector                           */
);
