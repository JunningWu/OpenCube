//# include "../HW/HW_test/rom_sizes.h" 
//#define ROM_NS_ADDR_SIZE 11 
//#define ROM_NS_DATA_SIZE 13 
 
#define _1_FRAMES_TEST 1 
#define _2_FRAMES_TEST 2 
#define _10_FRAMES_TEST 3 
#define _285_FRAMES_TEST 4 
 
//#define BI_TEST _1_FRAMES_TEST 
//#define BI_TEST _2_FRAMES_TEST 
//#define BI_TEST _10_FRAMES_TEST 
#define BI_TEST _285_FRAMES_TEST 
 
#if BI_TEST == _1_FRAMES_TEST 
// 1 FRAME 
#define ROM_NS_DEPTH 160 
#define ROM_SC_DEPTH 246 
 
#elif BI_TEST == _2_FRAMES_TEST 
// 2 FRAMES 
#define ROM_NS_DEPTH 320 
#define ROM_SC_DEPTH 492 
 
#elif BI_TEST == _10_FRAMES_TEST 
// 10 FRAMES 
// This is a configuration (10 frames)length that can be included in OR plaform 
#define ROM_NS_DEPTH 1600 
#define ROM_SC_DEPTH 2460 
 
#elif BI_TEST == _285_FRAMES_TEST 
// 285 FRAMES (TEST0) Speech and homing test (in continous mode) 
//#define ROM_NS_DEPTH (160*285) 
//#define ROM_SC_DEPTH (246*285) 
#define ROM_NS_DEPTH 45600 
#define ROM_SC_DEPTH 70110 
 
#endif 
 
#define DTX_MODE_ENABLED  1 
#define DTX_MODE_DISABLED 0 
 
// Declared here for the limited memory of the system for the built-in test 
//int rom_ns_array[ROM_NS_DEPTH]; 
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
//Word16 rom_serial_cod_array[ROM_SC_DEPTH];  
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
