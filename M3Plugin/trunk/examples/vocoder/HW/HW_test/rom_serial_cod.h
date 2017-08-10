/* Function: ROM containing serial output of coder */

#include "systemc.h"
#include "rom_sizes.h"

SC_MODULE(rom_serial_cod) {
public:
  sc_in_clk	clk;
  sc_in<sc_uint<ROM_SC_ADDR_SIZE> >	addr;
  sc_out<sc_uint<ROM_SC_DATA_SIZE> > dout;

  sc_uint<ROM_SC_DATA_SIZE> rom_serial_cod_array[ROM_SC_DEPTH]; 	
      
  void rom_serial_cod_func();
  	
  SC_CTOR(rom_serial_cod) {
/* ynopsys synthesis_off */
	SC_METHOD(rom_serial_cod_func);
	sensitive_pos << clk;
/* ynopsys synthesis_on */
  }
};
