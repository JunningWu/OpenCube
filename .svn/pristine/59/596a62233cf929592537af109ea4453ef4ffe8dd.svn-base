#include "uc_IO_hw.h"

extern "C"{
#include "../c_base/common/cnst.h"
};
// The input part of the testbench. This process dumps to a FIFO input 
// of the coder the first 10 frames of the TEST0 (speech) tests. 
// The number of frames can be easily changed and enlarged (but this 
// number is intended for the final platform used in the test, thus 
// can be changed for the ROM/RAM platforms restrictions or HW memory 
// for a HW version of these testbench) 
// 

	#if BI_TEST == _1_FRAMES_TEST 
		// 1 FRAME 
		#include "../HW/HW_test/rom_ns_content1.h" 
	 
	#elif BI_TEST == _2_FRAMES_TEST 
		// 2 FRAMES 
		#include "../HW/HW_test/rom_ns_content2.h" 
	 
	#elif BI_TEST == _10_FRAMES_TEST 
		// 10 FRAMES 
		#include "../HW/HW_test/rom_ns_content10.h" 
 
	#elif BI_TEST == _285_FRAMES_TEST 
		// 285 FRAMES (TEST0) Speech and homing test (in continous mode) 
		#include "../HW/HW_test/rom_ns_content285.h" 
	#endif 	

	#if BI_TEST == _1_FRAMES_TEST 
		// 1 FRAME 
		#include "../HW/HW_test/rom_serial_cod_content1.h" 
	 
	#elif BI_TEST == _2_FRAMES_TEST 
		// 2 FRAMES 
		#include "../HW/HW_test/rom_serial_cod_content2.h" 
		 
	#elif BI_TEST == _10_FRAMES_TEST 
		// 10 FRAMES 
		#include "../HW/HW_test/rom_serial_cod_content10.h" 
		 
	#elif BI_TEST == _285_FRAMES_TEST 
		// 285 FRAMES (TEST0) Speech and homing test (in continous mode) 
		#include "../HW/HW_test/rom_serial_cod_content285.h" 
 
	#endif 


uc_IO_hw::uc_IO_hw(sc_module_name module_name, unsigned int begin, unsigned int end, int ret): UC_hw_if(module_name, begin, end)
        , response_time(ret, SC_NS){

	#ifdef SW_MSG	 
		printf("PCMStream_gen: START\n"); 
	#endif 
printf("IO_HW: START\n"); 	 
//	BEGIN_PROF(); 
	 
	//	sc_uint<ROM_NS_DATA_SIZE> rom_ns_array[ROM_NS_DEPTH]; 	 
	//	int rom_ns_array[ROM_NS_DEPTH]; 
 

	frame1 = 0; frame2 = 0; 
	j1=0; i1=0; i2=0;
	dtx_mode = DTX_MODE_DISABLED;	// DTX disabled by default	 
  	sample_index = 0;


	#ifdef SW_MSG	 
		printf("RadioSubSystem: START\n"); 
	#endif 
	 
	#ifdef SC_DEBUG		 
		#if (LEVEL==COSIMULATION)||(LEVEL==SW_GENERATION) 
			SHOW_RESULTS(); 
		#endif 
	#endif	 
	 	 
	//	Word16 rom_serial_cod_array[ROM_SC_DEPTH]; 
 
//	BEGIN_PROF();	 
	 
 
	match = true; 

}	 

int uc_IO_hw::read(ADDRESS addr, DATA data, unsigned int size){ 

//printf("sent addr %x, index %d\n", addr, i1);
  if(addr&4){
	*(int *)data=dtx_mode;
//	cout << "muestra dtx_mode "<<dtx_mode<<" ";	
   }	
   else {
//	cout << "muestra "<<i1<<" "<<rom_ns_array[i1]<<" "<<endl;

      *(int *)data=(int)rom_ns_array[i1++];

      if(j1<159) {
         j1++;
      } else {
         j1=0;
         frame1++;
         printf("Frame %d sent.\n", frame1);
      }

   }
	return 4;
	
}


int uc_IO_hw::write(ADDRESS addr, DATA data, unsigned int size){

//printf("received data %d, index %d\n",*(int*)addr, sample_index);

   if (sample_index<ROM_SC_DEPTH) {

      if(i2<(SERIAL_SIZE+1)) {

	 if(rom_serial_cod_array[sample_index] != *(int*)data) {
            match=false;
      	    printf("\nVocoder test Failed\nafter %d frames coded&decoded, in sample %d.\n",frame2, i2);
	 	exit(0);
            return 4;
         }
	sample_index++;
	i2++;
		
      }
      if(i2<SERIAL_SIZE+1) return 4;
      i2=0;
      if (match) {
         frame2++;
	 printf("Vocoder output: Received Frame %d, check OK.\n", frame2);

      }
   }
	 
 
   if (sample_index<ROM_SC_DEPTH) return 4;
   if(match) {
      printf("Vocoder test OK after %d frames coded&decoded.\n", frame2);
      return_value=0;

      sc_stop();
   } else {
     	    printf("Vocoder test Failed\nafter %d frames coded / decoded, in sample %d.\n",frame2, i2);
   }
   return 4;

/*	int pid = getpid();
	
	for(int i_pid=pid+1;i_pid<40;i_pid++)kill(i_pid,SIGABRT);
	for(int i_pid=pid-1;i_pid>0;i_pid--)kill(i_pid,SIGABRT);
	
	return NULL; */

}

void *load_hw(const char *name,int begin,int end,int ret, void* ){
	return new uc_IO_hw(name, begin, end, ret);
}
