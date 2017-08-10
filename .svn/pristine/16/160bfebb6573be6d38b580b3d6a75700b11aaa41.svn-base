#ifndef _CODER_ENV_CC
#define _CODER_ENV_CC
 

// The input part of the testbench. This process dumps to a FIFO input 
// of the coder the first 10 frames of the TEST0 (speech) tests. 
// The number of frames can be easily changed and enlarged (but this 
// number is intended for the final platform used in the test, thus 
// can be changed for the ROM/RAM platforms restrictions or HW memory 
// for a HW version of these testbench) 
// 

void *PCMStream_gen(void *args){

PCM_fifos();

	#ifdef SW_MSG	 
		printf("PCMStream_gen: START\n"); 
	#endif 

	int i; 
	Word16 j; 
	Word16 frame; 
	#ifndef _CODER_2PORTS	 
		Word16 dtx_mode; 
	#endif 
	
	 
	BEGIN_PROF(); 
	 
	//	sc_uint<ROM_NS_DATA_SIZE> rom_ns_array[ROM_NS_DEPTH]; 	 
	//	int rom_ns_array[ROM_NS_DEPTH]; 
 
	 
	#ifndef _CODER_2PORTS	 
    		dtx_mode = 0;	// DTX disabled by default	 
 
		// SEND DTX STATE TO THE VAD COMPUTATION MODULE 
		CH_MONITOR(dtx_mode_in->write((bool)dtx_mode);) 
	#endif 
 
	frame = 0; 
	j=0; 
 
	#ifndef _CODER_2PORTS	 
		// SEND DTX STATE TO THE VAD COMPUTATION MODULE 
		//	CH_MONITOR(dtx_mode_out->write(DTX_MODE_ENABLED);) 
		CH_MONITOR(dtx_mode_in->write(DTX_MODE_DISABLED);) 
	#endif 
 
	for(i=0;i<ROM_NS_DEPTH;i++) { 
		CH_MONITOR(sample_in->write((Word16)rom_ns_array[i]);) 
		if(j<159) { 
			j++; 
		} 
		else { 
			j=0; 
			frame++; 
	#ifdef SW_MSG 
		#if (LEVEL==SPECIFICATION) 
			cout << "Frame " << frame << "sent." << endl; 
		#else			 
			printf("Frame %d sent.\n", frame); 
		#endif 
	#endif			 
	} 
	
	#ifdef TIMED_TESTBENCH 
	
	
		wait(125,SC_US); // 8KHz 
		
		//wait(1,SC_SEC); // Working with unitary times can give a quick idea ot 
						// how latencies are distributed in the output. Then 
						// maybe these latencies can be shorten (maybe not in 
						// those cases imposed by dependencies) 
		 
		// Fr the case of throughput and before applying the profiling library 
		// which includes system delays, the distribution of deltas (therefore 
		// of the computation of the system, each time a new environment event 
		// or data is supplied  can be studied with de delta monitor for activity 
		// analysis 
	#endif		 
	} 
	
	return NULL;
	
}//PCMStream_gen



void *RadioSubSystem(void *args){


Radio_fifos();

	#ifdef SW_MSG	 
		printf("RadioSubSystem: START\n"); 
	#endif 
	 
	#ifdef SC_DEBUG		 
		#if (LEVEL==COSIMULATION)||(LEVEL==SW_GENERATION) 
			SHOW_RESULTS(); 
		#endif 
	#endif	 
	 bool match; 
	 
	bool rec_bit; 
	 
	Word16 i; 
	int index; 
	Word16 frame; 
	 
	#ifdef	TIMED_TESTBENCH 
		FILE *f_spec_time_report; // File time_report forced by spec time report 
		if ((f_spec_time_report = (FILE *)fopen ("spec_time_report", "wb")) == NULL)	{ 
	        	fprintf (stderr,"ENV: RSS: Error opening time spec time report file!!\n"); 
	        	exit (0); 
	        } 
	#endif 
	 
	//	Word16 rom_serial_cod_array[ROM_SC_DEPTH]; 
 
	BEGIN_PROF();	 
	 
	index = 0; 
	frame = 0; 
 
	match = true; 
	 
	while(index<ROM_SC_DEPTH) { 
		//while(true) {		 
		//		for(i=0; i<(SERIAL_SIZE-1); i++) { 
		for(i=0; i<(SERIAL_SIZE+1); i++) { 
			CH_MONITOR(rec_bit = prm_out->read();) 
			if(rom_serial_cod_array[index] != rec_bit) { 
				printf("wrong value out_bit %d\n",index);
				match = false; 
				break; 
			} 
			index++; 
			#ifdef TIMED_TESTBENCH 
				//			fprintf(f_spec_time_report,"out_bit %d\t= %l at time\n",index,sc_time_stamp().to_string()); 
				
			#endif			 
		} 
		if(match) { 
			frame++; 
			#ifdef TIMED_TESTBENCH 
				cout << "RadioSubSystem: Received Frame " << frame << " at time " << sc_time_stamp() << ", check OK." << endl;			 
			#else 
 
				#ifdef SW_MSG 
					#if (LEVEL==SPECIFICATION) 
						cout << "RadioSubSystem: Received Frame " << frame << ", check OK." << endl; 
					#else			 
						printf("RadioSubSystem: Received Frame %d, check OK.\n", frame); 
					#endif			 
				#endif 
 
			#endif			 
		} else { 
			break; 
		} 
		// it consumes RSS bits 
		//		CH_MONITOR(serial_coded_prm->read();) // vad =  
		//		CH_MONITOR(serial_coded_prm->read();)  // sp = 
	} 
	 
	//	for(i=0; i<ROM_SC_DEPTH; i++) 
	#ifdef SW_MSG 
		#if (LEVEL==SPECIFICATION) 
			if(match==true) { 
				cout << "Coder test OK." << endl; 
			} else { 
				cout << "RadioSubSystem: Coder test Failed." << endl; 
			} 
			cout << "after " << index << " bits coded." << endl;	 
		#else	 
		if(match==true) { 
			printf("Coder test OK.\n"); 
		} else { 
			printf("Coder test Failed.\n"); 
		} 
		printf("after %d bits coded.\n",index); 
		#endif	 
	#endif 
	 
	#ifdef SC_DEBUG	 
		#if (LEVEL==COSIMULATION)||(LEVEL==SW_GENERATION) 
			SHOW_RESULTS(); 
		#endif	 
	#endif
	

	int pid = getpid();
	
	for(int i_pid=pid+1;i_pid<40;i_pid++)kill(i_pid,SIGABRT);
	for(int i_pid=pid-1;i_pid>0;i_pid--)kill(i_pid,SIGABRT);
	uc_user_sc_stop();

	return NULL; 

}//RadioSubSystem

#endif 
