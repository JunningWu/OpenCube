/***************************************************************************
 *
 *   File Name: vad.h
 *
 *   Purpose:   Contains the prototypes for all functions of voice activity
 *              detection. Also contains the type definition for the pseudo
 *              floating point data type.
 *
 **************************************************************************/

/* Struct for storing pseudo floating point exponent and mantissa */
struct _fp
{
    Word16 e;          /* exponent */
    Word16 m;          /* mantissa */
};

typedef struct _fp Pfloat;

void vad_reset (void);

Word16 vad_computation (
    Word16 r_h[],
    Word16 r_l[],
    Word16 scal_acf,
    Word16 rc[],
    Word16 ptch
);

void energy_computation (
    Word16 r_h[],
    Word16 scal_acf,
    Word16 rvad[],
    Word16 scal_rvad,
    Pfloat * acf0,
    Pfloat * pvad
);

void acf_averaging (
    Word16 r_h[],
    Word16 r_l[],
    Word16 scal_acf,
    Word32 L_av0[],
    Word32 L_av1[]
);

void predictor_values (
    Word32 L_av1[],
    Word16 rav1[],
    Word16 *scal_rav1
);

void schur_recursion (
    Word32 L_av1[],
    Word16 vpar[]
);

void step_up (
    Word16 np,
    Word16 vpar[],
    Word16 aav1[]
);

void compute_rav1 (
    Word16 aav1[],
    Word16 rav1[],
    Word16 *scal_rav1
);

Word16 spectral_comparison (
    Word16 rav1[],
    Word16 scal_rav1,
    Word32 L_av0[]
);

void threshold_adaptation (
    Word16 stat,
    Word16 ptch,
    Word16 tone,
    Word16 rav1[],
    Word16 scal_rav1,
    Pfloat pvad,
    Pfloat acf0,
    Word16 rvad[],
    Word16 *scal_rvad,
    Pfloat * thvad
);

void tone_detection (
    Word16 rc[],
    Word16 *tone
);

Word16 vad_decision (
    Pfloat pvad,
    Pfloat thvad
);

Word16 vad_hangover (
    Word16 vvad
);

void periodicity_update (
    Word16 lags[],
    Word16 *ptch
);
