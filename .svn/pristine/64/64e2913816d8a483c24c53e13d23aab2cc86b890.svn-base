/* file: rom_ns.c */
/* Function: ROM containing new_speech samples */

#include "systemc.h"
#include "rom_sizes.h"

SC_MODULE(rom_ns) {
public:
  sc_in_clk	clk;
  sc_in<sc_uint<ROM_NS_ADDR_SIZE> >	addr;
  sc_out<sc_uint<ROM_NS_DATA_SIZE> > dout;
  
  sc_uint<ROM_NS_DATA_SIZE> rom_ns_array[ROM_NS_DEPTH]; 	
      
  void rom_ns_func();
  
  SC_CTOR(rom_ns) {
/* synopsys synthesis_off */
	SC_METHOD(rom_ns_func);
	sensitive_pos << clk;
/* synopsys synthesis_on */
  }
};
