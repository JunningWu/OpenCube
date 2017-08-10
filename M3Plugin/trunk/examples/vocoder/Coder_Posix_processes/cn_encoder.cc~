#ifndef CN_ENCODER_CC
#define CN_ENCODER_CC

extern "C" void *CN_encoder_fun(void *args);

void *CN_encoder_fun(void *args){

cn_encoder_fifos();

	bool reset;
	Word16 old_CN_mem_tx[6]; // The most recent CN parameters are stored
	Word16 this_frame_txdtx_ctrl;
	Word16 param_number;
	Word16 param;
	Word16 params[39];

	BEGIN_PROF();	
	while(true) {
		#ifdef DBG_MSG
		printf("CN_ENCODER:------------------ HOMING RESET---------------------- \n");
		#endif
		// INBAND-RESET LOOP
		// Should be done also in an inband reset 
		for(param_number=0;param_number<6; param_number++) old_CN_mem_tx[param_number]=0;
		while(true) {
			CH_MONITOR(this_frame_txdtx_ctrl = txdtx_ctrl_vad_4->read();)
			if ((this_frame_txdtx_ctrl & TX_SP_FLAG) == 0) {
				if( ((this_frame_txdtx_ctrl & TX_SID_UPDATE) != 0) && ((this_frame_txdtx_ctrl & TX_USE_OLD_SID) != 0) ) {
							// I do not still know if this case is possible
							// This code does what the ANSI-C code would do for this case.
							// In this case, a data dependency appears between 56 param and the 17 parameter
							// therefore an inner params memory from param 17 to param 55 is necessary.
					for (param_number=0;param_number<17;param_number++) {
						if(param_number<5) {
							CH_MONITOR(param = lpc_prm->read();)
							old_CN_mem_tx[param_number] = param;
						}
						else {
							CH_MONITOR(subfrm_prm->read();)
							param = 0;
						}
						//if(param_number<5
						//	param = old_CN_mem_tx[param_number];
						//else
						//	param = 0;
						//if (param>4) param = 0;
						CH_MONITOR(prm_cn->write(param);)
					} 
					// stores last 40 parameters
					for (param_number=17;param_number<57;param_number++) {
						CH_MONITOR(params[param_number-17] = subfrm_prm->read();)
					}
					old_CN_mem_tx[5] = params[39]; // param 57
					for (param_number=17;param_number<57;param_number++) {
						if(param_number == 17) {
							param = old_CN_mem_tx[5];
						}
						else if(param_number == 30) {
							param = old_CN_mem_tx[5];
						}
						else if(param_number == 43) {
							param = old_CN_mem_tx[5];
						}
						else if(param_number == 56) {
							param = old_CN_mem_tx[5];
						}
						else {
							param = 0;
						}
						CH_MONITOR(prm_cn->write(params[param_number-17]);)
					}
						
				} else if ((this_frame_txdtx_ctrl & TX_SID_UPDATE) != 0) {				
					// bypass, but using data for CN memory update 
					for (param_number=0;param_number<57;param_number++) {
						if(param_number<5) {
							CH_MONITOR(param = lpc_prm->read();)
						} else {
							if((param_number==17)||(param_number==30)||(param_number==43)||(param_number==56)) {
								CH_MONITOR(param=subfrm_prm->read();)
							} else {
								CH_MONITOR(subfrm_prm->read();)
								param = 0;
							}
						}
						CH_MONITOR(prm_cn->write(param);)
						if(param_number<5) old_CN_mem_tx[param_number] = param;
						if(param_number==56) old_CN_mem_tx[5] = param;	
					}
				} else if ((this_frame_txdtx_ctrl & TX_USE_OLD_SID) != 0) {
					// bypass substituting 1-4, 5, 17, 30, 43, 56 values
					for (param_number=0;param_number<57;param_number++) {					
						if(param_number < 5) {
							CH_MONITOR(lpc_prm->read();)
							param = old_CN_mem_tx[param_number];
						} else {
							CH_MONITOR(subfrm_prm->read();)
							if(param_number == 17) {
								param = old_CN_mem_tx[5];
							} else if(param_number == 30) {
								param = old_CN_mem_tx[5];
							} else if(param_number	== 43) {
								param = old_CN_mem_tx[5];
							} else if(param_number == 56) {
								param = old_CN_mem_tx[5];
							} else {
								param = 0;
							}
						} 
						CH_MONITOR(prm_cn->write(param);) 
					}									 
				} 
			} else { 
				// bypass the frame parameters 
				for(param_number=0;param_number<57;param_number++) { 
					if(param_number<5) { 
						CH_MONITOR(param = lpc_prm->read();)
						CH_MONITOR(prm_cn->write(param);) 
					} 
					else { 
						CH_MONITOR(param = subfrm_prm->read();) 
						CH_MONITOR(prm_cn->write(param);) 
					} 
				} 
 
/* 
				for(param_number=0;param_number<57;param_number++) { 
					if(param_number<5) {
						CH_MONITOR(prm_out->write(lpc_prm_in->read());) 
					} 
					else { 
						CH_MONITOR(prm_out->write(subframe_prm_in->read());) 
					} 
				} 
*/
			} 
			// CHECK OF IN-BAND RESET (lectura del canal siempre_hecha) 
			// the read is separated in order to avoid the break affects 
			// counters update when CH_MONITOR macro is used at SW_GENERATION 
			// level for profiling!! 
			CH_MONITOR(reset = inband_reset_5->read();) 
			if(reset) break; 
				 
			// En los SC_THREAD la estructura break permite salir del bucle 
			// principar y saltar al bucle de reset. Este contiene las sentencias 
			// de inicializaci�n (en nuestro caso las de reset en banda) Si hay alguna 
			// sentencia incluida en el reset inicial, pero no en el reset en banda, 
			// se antepone al inicio del reset en banda. 
			// La estructura mediante bucles de reset y main y break tiene una 
			// coincidencia grande con respecto al estilo de comportamiento en VHDL y 
			// una traslaci�n pr�cticamente directa al SystemC de comportamiento 
			// (eliminado el lazo de reset, usando el watching en los SC_CTHREAD...) 
			 
			//		La estructura if(in_band_reset->read()) goto inband_reset_label; no se usar� 
			//	para evitar cualquier referencia a lenguage no estructurado.		 
						
		}	// END MAIN LOOP 
	}	// END INBAND-RESET LOOP 

}// CN_encode_fun
#endif 
