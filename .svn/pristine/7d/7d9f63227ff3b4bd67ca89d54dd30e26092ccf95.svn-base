/* Behavioral description (HW) for Behavioral Compiler */
/* Function: Serial of coder checking */
/* Expected values in outside ROMs */

#include "rom_sizes.h"

SC_MODULE(serial_cod_check) {
public:
  sc_in<bool>	reset;
  sc_in_clk		clk;
  
  //ROM serial_coder
  sc_out<sc_uint<ROM_SC_ADDR_SIZE> > rom_sc_addr;
  sc_in<sc_uint<ROM_SC_DATA_SIZE> > rom_sc_data;
  
  // FIFO serial_coder
  sc_in<sc_uint<ROM_SC_DATA_SIZE> > serial_cod;
  sc_out<bool>	serial_cod_req;
  sc_in<bool>	serial_cod_ack;
  
  // status ports
  sc_out<bool> serial_cod_match;
  sc_out<bool> serial_cod_mismatch;
    
  void serial_cod_check_func();
  	
  SC_CTOR(serial_cod_check) {
	SC_CTHREAD(serial_cod_check_func,clk.pos());
	watching(reset.delayed() == true);
  }
};
