/*****************************************************************************



 *****************************************************************************/


#ifndef _UC_IO_HW_H_
#define _UC_IO_HW_H_

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

#include "sc_scope.h"


class uc_IO_hw : public UC_hw_if
{

  //transmitter
  int j1,i1;
  int dtx_mode;
  int frame1,m_size;
  char *m_info;
  sc_time response_time;
  bool match;

// Declared here for the limited memory of the system for the built-in test 
//  int rom_ns_array[ROM_NS_DEPTH]; 
//  short rom_serial_cod_array[ROM_SC_DEPTH];

  // receiver
  int	i2;
  unsigned int sample_index;
  int frame2;
  int return_value;

public :

  uc_IO_hw(sc_module_name module_name, unsigned int begin, unsigned int end, int ret);

        int read(ADDRESS addr, DATA data, unsigned int size);
        int write(ADDRESS addr, DATA data, unsigned int size);

};

extern "C" void *load_hw(const char*,int,int,int, void*);
#endif /* _SLAVE_H_ */


