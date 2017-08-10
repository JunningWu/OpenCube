/* Behavioral description (HW) for Behavioral Compiler */
/* Function: Serial of coder checking */
/* Expected values in outside ROMs */

#include "systemc.h"
#include "serial_cod_check.h"

void serial_cod_check::serial_cod_check_func() {
	sc_uint<ROM_SC_ADDR_SIZE> addr_count;

//reset part
	rom_sc_addr->write(0);
	serial_cod_req->write(false);
	serial_cod_match->write(false);
	serial_cod_mismatch->write(false);
	addr_count=0;
	
  	wait();

  	main_loop:while(true)
  	{
		serial_cod_match->write(false);
		serial_cod_mismatch->write(false);
		rom_sc_addr->write(addr_count);
		serial_cod_req->write(true);
    	//wait();

		wait_until(serial_cod_ack.delayed() == true);

		serial_cod_req->write(false);
		if (rom_sc_data->read()==serial_cod->read())
			serial_cod_match->write(true);
		else
			serial_cod_mismatch->write(true);			
		if (addr_count==ROM_SC_DEPTH-1)
			addr_count=0;
		else
			addr_count=addr_count+1;

		wait();
		
  	}



}
