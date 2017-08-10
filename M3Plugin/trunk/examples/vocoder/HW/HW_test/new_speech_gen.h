/* Behavioral description (HW) for Behavioral Compiler */
/* Function: New speech frames generation */
/* Frames in outside ROMs */

#include "rom_sizes.h"

SC_MODULE(new_speech_gen) {
public:
  sc_in<bool>	reset;
  sc_in_clk		clk;
  
  // ROM new_speech
  sc_out<sc_uint<ROM_NS_ADDR_SIZE> > rom_ns_addr;
  sc_in<sc_uint<ROM_NS_DATA_SIZE> > rom_ns_data;
  
  // FIFO new_speech
  sc_out<sc_uint<ROM_NS_DATA_SIZE> > new_speech;
  sc_out<bool>	new_speech_req;
  sc_in<bool>	new_speech_ack;
    
  void new_speech_gen_func();
  	
  SC_CTOR(new_speech_gen) {
	SC_CTHREAD(new_speech_gen_func,clk.pos());
	watching(reset.delayed() == true);
  }
};
