#ifndef _PRE_PROCESS_CC
#define _PRE_PROCESS_CC

#include "linux/io.h"

extern "C" void *pre_filtering(void *args);
extern "C" void *homing_frame_test(void *args);

void *pre_filtering(void *args){
pre_filt_fifos();

	/*sc_fifo_SS<Word16>	*IN_SAMPLE;
	sc_fifo_SS<Word16>	*PREPROC_INBAND_RESET;
	sc_fifo_SS<Word16>      *sample_in;
	sc_fifo_SS<Word16>	*preproc_sample_1;
	sc_fifo_SS<Word16>	*preproc_sample_2;
	sc_fifo_SS<Word16>      *preproc_sample_3;
	
	struct pre_filtering_data *funcion_imput_data=(struct pre_filtering_data *)args;
	
	IN_SAMPLE=funcion_imput_data->IN_SAMPLE;
	PREPROC_INBAND_RESET=funcion_imput_data->PREPROC_INBAND_RESET;
	sample_in=funcion_imput_data->sample_in;
	preproc_sample_1=funcion_imput_data->preproc_sample_1;
	preproc_sample_2=funcion_imput_data->preproc_sample_2;
	preproc_sample_3=funcion_imput_data->preproc_sample_3;*/
	
	#define EHF_MASK 0x0008 /* Encoder Homing Frame pattern */ 
	Word16 reset;
	// filter coefficients (fc = 80 Hz, coeff. b[] is divided by 2) 
	const Word16 b[3] = {1899, -3798, 1899}; 
	const Word16 a[3] = {4096, 7807, -3733}; 
	Word16	i; // ecessary to control the inband reset condition (given at frame synchrony) 
	Word16 x2; 
	Word32 L_tmp; 
	Word16 L_tmp_round; 
	// Values to be preserved between calls 
	// y[] values are kept in double precision 
	Word16 y2_hi, y2_lo, y1_hi, y1_lo, x0, x1; 
		
	BEGIN_PROF();	 

//	while(true) 
	int frame=0; 
	while(frame < NUM_FRAMES)
	{ 
		#ifdef DBG_MSG			 
			printf("VAD_COMPUTATION:------------------HOMING RESET---------------------- \n"); 
		#endif		 
		// INBAND-RESET LOOP
		// Init preprocess
		y2_hi = 0; 
		y2_lo = 0; 
		y1_hi = 0; 
		y1_lo = 0; 
		x0 = 0; 
		x1 = 0;
		while(frame < NUM_FRAMES) { 
//		for(int frame=0;frame < 2*BI_TEST; frame++) { 
			for(i=0;i<L_FRAME; i++) { 
				x2 = x1;                     
				x1 = x0;                     
				//x0 = signal[i];              
#ifdef HW_ACCESS
				int* sample_in = (int*)0xBF000000;
#endif
				HW_ACCESS_READ(x0,sample_in);

		 		CH_MONITOR(IN_SAMPLE->write(x0);) 
				//cout << "PREPROC: sample sent to homing process." << endl; 
				 
				x0 = x0 & 0xfff8;  // Delete the 3 LSBs (13-bit input)
				 
				/*  y[i] = b[0]*x[i]/2 + b[1]*x[i-1]/2 + b140[2]*x[i-2]/2  */ 
				/*                     + a[1]*y[i-1] + a[2] * y[i-2];      */ 
		 
				L_tmp = Mpy_32_16 (y1_hi, y1_lo, a[1]); 
				L_tmp = L_add (L_tmp, Mpy_32_16 (y2_hi, y2_lo, a[2])); 
				L_tmp = L_mac (L_tmp, x0, b[0]); 
				L_tmp = L_mac (L_tmp, x1, b[1]); 
				L_tmp = L_mac (L_tmp, x2, b[2]); 
				L_tmp = L_shl (L_tmp, 3); 
				L_tmp_round = round (L_tmp);   
				//cout << "PREPROC: sending preprocessed sample." << endl;						 
				//		sample_out->write(round (L_tmp)); 
				CH_MONITOR(preproc_sample_1->write(L_tmp_round);) 
				CH_MONITOR(preproc_sample_2->write(L_tmp_round);) 
				CH_MONITOR(preproc_sample_3->write(L_tmp_round);)
				//cout << "PREPROC: preprocessed sample sent." << endl; 
				y2_hi = y1_hi;               
				y2_lo = y1_lo;               
				L_Extract (L_tmp, &y1_hi, &y1_lo); 
			} 
			 
			// the read is separated in order to avoid the break affects 
			// counters update when CH_MONITOR macro is used at SW_GENERATION 
			// level for profiling!! 
			CH_MONITOR(reset = PREPROC_INBAND_RESET->read();) 			 
			frame++;
			if(reset) break;
		} 
	}


}//pre_filtering




void *homing_frame_test(void *args){

homing_fifos();

	#define EHF_MASK 0x0008 /* Encoder Homing Frame pattern */ 
	Word16	i; 
	Word16	sample; 
	bool homing_frame; 
 
	BEGIN_PROF(); 
	while(true) { 
		homing_frame = false; 
		// cout << "PREPROC: HOMING: Before the loop." << endl;		 
		for (i = 0; i < L_FRAME; i++) { 
		// cout << "PREPROC: HOMING: Reading sample " << i << endl; 
		// CAREFULL!! A statement like this: 
		// (a) 
		// homing_frame = homing_frame || (IN_SAMPLE->read() ^ EHF_MASK); 
		// does not function as this 
		// (b) 
		CH_MONITOR(sample = IN_SAMPLE->read();) 
		homing_frame = homing_frame || (sample ^ EHF_MASK);			 
		//... because last option forces the execution of synchronization 
		// meanwhile the first one overrides the read access if homing_frame 
		// is already true (sencond branch is not evaluated) 
		// Nevertheless this statement 
		// (c) 
		// is valid 
		// homing_frame = (IN_SAMPLE->read() ^ EHF_MASK) || homing_frame; 
		// since read access is placed in the first branch (always evaluated). 
		// The (b) option has been left in order to protect the code against 
		// contingent compiler optizations that could play with commutativity of 
		// || operation. 
	} 
	#ifdef DBG_MSG		 
		printf("PREPROC: HOMING: Sending reset flag.\n"); 
		if(!homing_frame) printf("DECIDE TRAMA DE HOMING!!!!\n"); 
		else printf("DECIDE TRAMA DE VOZ!!!!\n"); 
	#endif	 
	CH_MONITOR(PREPROC_INBAND_RESET->write(!homing_frame);) // returns true if it was a homing frame 
	CH_MONITOR(inband_reset_1->write(!homing_frame);) // returns true if it was a homing frame 
	CH_MONITOR(inband_reset_2->write(!homing_frame);) // returns true if it was a homing frame 
	CH_MONITOR(inband_reset_3->write(!homing_frame);) // returns true if it was a homing frame 
	CH_MONITOR(inband_reset_4->write(!homing_frame);) // returns true if it was a homing frame 
	CH_MONITOR(inband_reset_5->write(!homing_frame);) // returns true if it was a homing frame				 
	//		reset_flag_out_N->write(!homing_frame); // returns true if it was a homing frame	 
	#ifdef DBG_MSG		 
		printf("PREPROC: HOMING: reset flag sent.\n");		 
	#endif 
    	} 
}//homing_frame_test
#endif 
