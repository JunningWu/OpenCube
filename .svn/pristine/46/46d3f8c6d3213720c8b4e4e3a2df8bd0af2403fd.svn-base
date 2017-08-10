//
// adaptative_codebook_search.c
//
// SystemC Adaptation for ANSI-C Vocoder
//
//	TEISA Dpt.
//  University of Cantabria
//  Author: F.Herrera
//  Date:	August 2003
//

void Adaptive_Codebook_Search(
//				Word16 *Ap1,  // input 
//				Word16 *Ap2,   // input
				Word16 txdtx_ctrl_var, // input
				Word16 *A,
				Word16 *Aq,
								Word16 *T_op,		// input
								Word16 ana[],		// output  : Analysis parameters)
								Word16 &TO_out,		// output  : pitch parameter in closed loop
								Word16 &gain_pit_out, // output
								Word16 *CN_excitation_gain // output
)

{

	Word16 i;
	Word16 pit_flag;
	Word16 T0_frac;
static	Word16 T0_min;
static	Word16 T0_max;
//	Word16 T0_min;
//	Word16 T0_max;
	
	Word16 T0;		
	
    Word16 Ap1[MP1];                // (**) A(z) with spectral expansion         
    Word16 Ap2[MP1];                // (**) A(z) with spectral expansion         
		
        if ((txdtx_ctrl_var & TX_SP_FLAG) != 0)
        {
            // ---------------------------------------------------------------
            //  Find the weighted LPC coefficients for the weighting filter.  
            // ---------------------------------------------------------------
            Weight_Ai (A, F_gamma1, Ap1);

            Weight_Ai (A, F_gamma2, Ap2);

            // ---------------------------------------------------------------
            //  Compute impulse response, h1[], of weighted synthesis filter  
            // ---------------------------------------------------------------

            for (i = 0; i <= M; i++)
            {
                ai_zero[i] = Ap1[i];                             
            }

            Syn_filt (Aq, ai_zero, h1, L_SUBFR, zero, 0);
            
			Syn_filt (Ap2, h1, h1, L_SUBFR, zero, 0);

        }
		
        // ---------------------------------------------------------------
        //          Find the target vector for pitch search:             
        // ---------------------------------------------------------------

        //Residu (Aq, &speech[i_subfr], res2, L_SUBFR);   // LPC residual
		Residu (Aq, &x[i_subfr+M], res2, L_SUBFR);   // LPC residual
		
        if ((txdtx_ctrl_var & TX_SP_FLAG) == 0)
        {
            // Compute comfort noise excitation gain based on LP residual energy

            *CN_excitation_gain = compute_CN_excitation_gain (res2);
             
        }
        else
        {
            Copy (res2, &exc[i_subfr], L_SUBFR);

            Syn_filt (Aq, &exc[i_subfr], error, L_SUBFR, mem_err, 0);

            Residu (Ap1, error, xn, L_SUBFR);
            
			Syn_filt (Ap2, xn, xn, L_SUBFR, mem_w0, 0); // target signal xn[]

            //--------------------------------------------------------------
            //                 Closed-loop fractional pitch search          
            //--------------------------------------------------------------

            // flag for first and 3th subframe
            pit_flag = i_subfr;                                  

            // set t0_min and t0_max for 1st subf
			if ( i_subfr == 0)
			{
				// Range for closed loop pitch search in 1st subframe
		
				T0_min = sub (T_op[0], 3);
				
				if (sub (T0_min, PIT_MIN) < 0)
				{
					T0_min = PIT_MIN;                                    
				}
				T0_max = add (T0_min, 6);
				 
				if (sub (T0_max, PIT_MAX) > 0)
				{
					T0_max = PIT_MAX;                                    
					T0_min = sub (T0_max, 6);
				}
								
			}
			
            // set t0_min and t0_max for 3th subf
            if (sub (i_subfr, L_FRAME_BY2) == 0)
            {
                T0_min = sub (T_op[1], 3);

                if (sub (T0_min, PIT_MIN) < 0)
                {
                    T0_min = PIT_MIN;                            
                }
                T0_max = add (T0_min, 6);
				
                if (sub (T0_max, PIT_MAX) > 0)
                {
                    T0_max = PIT_MAX;                            
                    T0_min = sub (T0_max, 6);
                }
                pit_flag = 0;                                    
            }

            T0 = Pitch_fr6 (&exc[i_subfr], xn, h1, L_SUBFR, T0_min, T0_max,
                            pit_flag, &T0_frac);                 

			TO_out = T0;
			
            *ana = Enc_lag6 (T0, &T0_frac, &T0_min, &T0_max, PIT_MIN,
                             PIT_MAX, pit_flag);
        }
        ana++;
        // Incrementation of ana is done here to work also
        // when no speech activity is present

		
        if ((txdtx_ctrl_var & TX_SP_FLAG) != 0)
        {

            // ---------------------------------------------------------------
            //  - find unity gain pitch excitation (adaptive codebook entry)  
            //    with fractional interpolation.                              
            //  - find filtered pitch exc. y1[]=exc[] convolved with h1[]     
            //  - compute pitch gain and limit between 0 and 1.2              
            //  - update target vector for codebook search                    
            //  - find LTP residual.                                          
            // ---------------------------------------------------------------

            Pred_lt_6 (&exc[i_subfr], T0, T0_frac, L_SUBFR);

            Convolve (&exc[i_subfr], h1, y_1, L_SUBFR);

            gain_pit_out = G_pitch (xn, y_1, L_SUBFR);       

            *ana = q_gain_pitch (&gain_pit_out);                     

        }
        else
        {
            gain_pit_out = 0;                                        
        }

        ana++;                  // Incrementation of ana is done here to work
                                //  also when no speech activity is present
}
