//
// innovative_codebook_search.c
//
// SystemC Adaptation for ANSI-C Vocoder
//
//	TEISA Dpt.
//  University of Cantabria
//  Author: F.Herrera
//  Date:	August 2003
//

void Innovative_Codebook_Search(
								Word16 txdtx_ctrl_var, // input
								Word16 *Aq,
								Word16 T0,		// input : pitch parameter in closed loop for this subframe
								Word16 gain_pit, // input
								Word16 *CN_excitation_gain, // input								
								Word16 ana[],    // output  : Analysis parameters)
								Word16 synth[],   // output  : Local synthesis
								Word16 past_qua_en[], // input/output past quantized energies.								
								Word32 *L_pn_seed_tx_par, // input/output seed for Confort noise code generation
								Word16 gain_code_old_tx[]	// input/output									
)

{
	
//static    Word16 synth[L_FRAME];        // Buffer for synthesis speech
	
	Word16 i;
	Word16 j;
	Word16 k;
    Word16 temp;
    Word32 L_temp;
	Word16 gain_code;
	Word16 pit_sharp;	
	
	Word16 xn2[L_SUBFR];           // Target vector for codebook search     
	
    Word16 code[L_SUBFR];          // Fixed codebook excitation
	
    Word16 y2[L_SUBFR];            // Filtered fixed codebook excitation	
	
    if ((txdtx_ctrl_var & TX_SP_FLAG) != 0) {
		
		// xn2[i]   = xn[i] - y1[i] * gain_pit  
        // res2[i] -= exc[i+i_subfr] * gain_pit 

        for (i = 0; i < L_SUBFR; i++) {
			L_temp = L_mult (y_1[i], gain_pit);
            L_temp = L_shl (L_temp, 3);
            xn2[i] = sub (xn[i], extract_h (L_temp));        

            L_temp = L_mult (exc[i + i_subfr], gain_pit);
            L_temp = L_shl (L_temp, 3);
            res2[i] = sub (res2[i], extract_h (L_temp));     
		}

        // -------------------------------------------------------------
        //  - include pitch contribution into impulse resp. h1[]        
        // -------------------------------------------------------------

        // pit_sharp = gain_pit;                   
        // if (pit_sharp > 1.0) pit_sharp = 1.0;   

        pit_sharp = shl (gain_pit, 3);

        for (i = T0; i < L_SUBFR; i++) {
			temp = mult (h1[i - T0], pit_sharp);
            h1[i] = add (h1[i], temp);
		}

        // --------------------------------------------------------------
        //  - Innovative codebook search (find index and gain)           
        // --------------------------------------------------------------
        code_10i40_35bits (xn2, res2, h1, code, y2, ana);

	} else {
		build_CN_code (code, L_pn_seed_tx_par);
//		build_CN_code (code, &L_pn_seed_tx);		
    }
	
    ana += 10;                                               

    if ((txdtx_ctrl_var & TX_SP_FLAG) != 0) {

		//-------------------------------------------------------
        //  - Add the pitch contribution to code[].              
        //-------------------------------------------------------

        for (i = T0; i < L_SUBFR; i++) {
			temp = mult (code[i - T0], pit_sharp);
            code[i] = add (code[i], temp);                   
		}

        // ------------------------------------------------------
        //  - Quantization of fixed codebook gain.               
        // ------------------------------------------------------

        gain_code = G_code (xn2, y2);                        

	}
    *ana++ = q_gain_code (code, L_SUBFR, &gain_code, past_qua_en, txdtx_ctrl_var, i_subfr,*CN_excitation_gain,gain_code_old_tx);
         
    //------------------------------------------------------
    // - Find the total excitation                          
    // - find synthesis speech corresponding to exc[]       
    // - update filter memories for finding the target      
    //   vector in the next subframe                        
    //   (update mem_err[] and mem_w0[])                    
    //------------------------------------------------------

    for (i = 0; i < L_SUBFR; i++) {
		// exc[i] = gain_pit*exc[i] + gain_code*code[i];

        L_temp = L_mult (exc[i + i_subfr], gain_pit);
        L_temp = L_mac (L_temp, code[i], gain_code);
        L_temp = L_shl (L_temp, 3);
        exc[i + i_subfr] = round (L_temp);                   
    }

    Syn_filt (Aq, &exc[i_subfr], &synth[i_subfr], L_SUBFR, mem_syn, 1);

    if ((txdtx_ctrl_var & TX_SP_FLAG) != 0) {
	    j = 0; // here for profiling library limitation
        for (i = L_SUBFR - M; i < L_SUBFR; i++) {
//			mem_err[j] = sub (speech[i_subfr + i], synth[i_subfr + i]);
			mem_err[j] = sub (x[i_subfr + i + M], synth[i_subfr + i]);
			
            temp = extract_h (L_shl (L_mult (y_1[i], gain_pit), 3));
            k = extract_h (L_shl (L_mult (y2[i], gain_code), 5));
            mem_w0[j] = sub (xn[i], add (temp, k));
			j++;
        }
	} else {
		for (j = 0; j < M; j++) {
			mem_err[j] = 0;                                  
            mem_w0[j] = 0;                                   
		}
	}
		
}
