//
// coder_posix.cpp
//
// Manual translation for coder_posix
//
//	TEISA Dpt.
//  University of Cantabria
//  Author: J.A.Adamez Martin
//  Revision y particionado en varios ficheros otra vez: F. Herrera
//  Date:	Enero 2005
//

//#define _TEMP_VAR_FOR_SW_GENERATION
#define SW_MSG
//#define DBG_MSG

#define NUM_FRAMES 285
#define HW_ACCESS 2

//#define PRINT_SCHEDULER
#define CH_MONITOR(channel_access) channel_access
#define BEGIN_PROF()
#if HW_ACCESS==1
	#define HW_ACCESS_WRITE(a,b) *a = b;
	#define HW_ACCESS_READ(a,b) a = *b;
#else
#if HW_ACCESS==2
	#define HW_ACCESS_WRITE(a,b) uc_iowrite32(b, a);
	#define HW_ACCESS_READ(a,b) a = uc_ioread32(b);
#else
//	#define HW_ACCESS_WRITE(a,b) CH_MONITOR(a->write(b);)
//	#define HW_ACCESS_READ(a,b) CH_MONITOR(a = b->read();) 
int index_rom_serial_cod_array=0;
int index_rom_serial_cod_array_base=0;
int index_rom_serial_cod_array_frame=0;
int index_rom_ns_array=0;
	#define HW_ACCESS_WRITE(a,b) if(b != rom_serial_cod_array[index_rom_serial_cod_array++])\
					{printf("Wrong coded value %d\n",index_rom_serial_cod_array);}\
				     else {\
					if(index_rom_serial_cod_array==ROM_SC_DEPTH){\
						printf("simulation ok\n");uc_user_sc_stop();}\
					index_rom_serial_cod_array_base++;\
					if((index_rom_serial_cod_array_base==(SERIAL_SIZE+1))){\
						printf("frame ok %d\n",index_rom_serial_cod_array_frame);\
						index_rom_serial_cod_array_base=0;index_rom_serial_cod_array_frame++;}\
				     }
	#define HW_ACCESS_READ(a,b) CH_MONITOR(a = rom_ns_array[index_rom_ns_array++];) 
#endif
#endif

#include <iostream>
using namespace std;
#include <signal.h>
#include <linux/ioport.h>


pthread_t p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11;

// C base code

extern "C"{
#include "../c_base/common/typedef.h"
#include "../c_base/common/cnst.h"
#include "../c_base/common/basic_op.h"
#include "../c_base/common/sig_proc.h"
#include "../c_base/common/oper_32b.h"
#include "../c_base/common/dtx.h"
#include "../c_base/common/codec.h"
#include "../c_base/common/gains_tb.h"
#include "../c_base/coder/window2.tab"
#include "../c_base/coder/coder.h"
#include "../c_base/coder/dtx_cod.h"
#include "../c_base/coder/vad.h"
};

// Posix Libraries
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#include "init.h"
#include "fifos.h"

// coder thread functions
#include "pre_process.cc"
#include "frame_lsp.cc"
#include "frame_int_tol.cc"
#include "vad_computation.cc"
#include "adaptive_codebook_search_m.c"
#include "innovative_codebook_search_m.c"
#include "subframe_coder.cc"
#include "cn_encoder.cc" 
#include "serializer.cc"
#include "sid_codeword_encoder.cc"

// environment thread functions
#ifndef HW_ACCESS
//#include "coder_env.cc"
#endif

//#define THREAD_STACK_SIZE 8192
#define THREAD_STACK_SIZE 7000
#define NUMBER_OF_THREADS 11

