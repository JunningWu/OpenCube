/* Behavioral description (HW) for Behavioral Compiler */
/* Function: New speech frames generation */
/* Frames in outside ROMs */

#include "systemc.h"
#include "new_speech_gen.h"

void new_speech_gen::new_speech_gen_func() {

	sc_uint<ROM_NS_ADDR_SIZE> addr_count;

//reset part
	rom_ns_addr->write(0);
	new_speech->write(0);
	new_speech_req->write(false);
	addr_count=0;

  	wait();

  	main_loop:while(true)
  	{
		rom_ns_addr->write(addr_count);
		new_speech_req->write(true);
    	//wait();

		wait_until(new_speech_ack.delayed() == true);

    	new_speech_req->write(false);
		new_speech->write(rom_ns_data->read());
		if (addr_count==ROM_NS_DEPTH-1)
			addr_count=0;
		else
			addr_count=addr_count+1;

		wait();
  	}
  
}
