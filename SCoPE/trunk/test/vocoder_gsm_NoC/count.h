/* Global counter variable for calculation of complexity weight */

typedef struct
{
    Word32 add;        /* Complexity Weight of 1 */
    Word32 sub;
    Word32 abs_s;
    Word32 shl;
    Word32 shr;
    Word32 extract_h;
    Word32 extract_l;
    Word32 mult;
    Word32 L_mult;
    Word32 negate;
    Word32 round;
    Word32 L_mac;
    Word32 L_msu;
    Word32 L_macNs;
    Word32 L_msuNs;
    Word32 L_add;      /* Complexity Weight of 2 */
    Word32 L_sub;
    Word32 L_add_c;
    Word32 L_sub_c;
    Word32 L_negate;
    Word32 L_shl;
    Word32 L_shr;
    Word32 mult_r;
    Word32 shr_r;
    Word32 shift_r;
    Word32 mac_r;
    Word32 msu_r;
    Word32 L_deposit_h;
    Word32 L_deposit_l;
    Word32 L_shr_r;    /* Complexity Weight of 3 */
    Word32 L_shift_r;
    Word32 L_abs;
    Word32 L_sat;      /* Complexity Weight of 4 */
    Word32 norm_s;     /* Complexity Weight of 15 */
    Word32 div_s;      /* Complexity Weight of 18 */
    Word32 norm_l;     /* Complexity Weight of 30 */
    Word32 DataMove16; /* Complexity Weight of 1 */
    Word32 DataMove32; /* Complexity Weight of 2 */
    Word32 Logic16;    /* Complexity Weight of 1 */
    Word32 Logic32;    /* Complexity Weight of 2 */
    Word32 Test;       /* Complexity Weight of 2 */
}
BASIC_OP;

Word32 TotalWeightedOperation (void);
Word32 DeltaWeightedOperation (void);

void Init_WMOPS_counter (void);
void Reset_WMOPS_counter (void);
void WMOPS_output (Word16 dtx_mode);
Word32 fwc (void);

void move16 (void);
void move32 (void);
void logic16 (void);
void logic32 (void);
void test (void);
