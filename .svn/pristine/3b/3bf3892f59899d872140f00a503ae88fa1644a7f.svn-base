/* file: vocoder_env_HW_plusROMs.c */
/* Environment of vocoder */
/* Stuctural RTL description (HW) */
/* Function: vocoder_env_HW plus ROMs */

#include "systemc.h"
#include "new_speech_gen.h"
#include "serial_dec_gen.h"
#include "serial_cod_check.h"
#include "synth_check.h"
#include "rom_ns.h"
#include "rom_serial_dec.h"
#include "rom_serial_cod.h"
#include "rom_synth.h"

#include "rom_sizes.h"

SC_MODULE(vocoder_env_HW_ROMs) {
public:
  //common
  sc_in<bool>	reset;
  sc_in_clk		clk;

  //new_speech_gen
  sc_out<sc_uint<ROM_NS_DATA_SIZE> > new_speech;
  sc_out<bool>	new_speech_req;
  sc_in<bool>	new_speech_ack;

  //serial_dec_gen
  sc_out<sc_uint<ROM_SD_DATA_SIZE> > serial_dec;
  sc_out<bool>	serial_dec_req;
  sc_in<bool>	serial_dec_ack;

  //serial_cod_check
  sc_in<sc_uint<ROM_SC_DATA_SIZE> > serial_cod;
  sc_out<bool>	serial_cod_req;
  sc_in<bool>	serial_cod_ack;
  sc_out<bool> serial_cod_match;
  sc_out<bool> serial_cod_mismatch;
  
  //synth_check
  sc_in<sc_uint<ROM_SY_DATA_SIZE> > synth;
  sc_out<bool>	synth_req;
  sc_in<bool>	synth_ack;
  sc_out<bool> synth_match;
  sc_out<bool> synth_mismatch;
  
  //rom_new_speech
  sc_uint<ROM_NS_ADDR_SIZE> rom_ns_addr;
  sc_uint<ROM_NS_DATA_SIZE> rom_ns_data;
      
  //rom_serial_decoder
  sc_uint<ROM_SD_ADDR_SIZE> rom_sd_addr;
  sc_uint<ROM_SD_DATA_SIZE> rom_sd_data;
      
  //rom_serial_coder
  sc_uint<ROM_SC_ADDR_SIZE> rom_sc_addr;
  sc_uint<ROM_SC_DATA_SIZE> rom_sc_data;
      
  //rom_synth
  sc_uint<ROM_SY_ADDR_SIZE> rom_sy_addr;
  sc_uint<ROM_SY_DATA_SIZE> rom_sy_data;
      
  new_speech_gen *new_speech_gen_inst;
  rom_ns *rom_ns_inst;
   	
  serial_dec_gen *serial_dec_gen_inst;
  rom_serial_dec *rom_serial_dec_inst;
   	
  serial_cod_check *serial_cod_check_inst;
  rom_serial_cod *rom_serial_cod_inst;
   	
  synth_check *synth_check_inst;
  rom_synth *rom_synth_inst;
   	
  SC_CTOR(vocoder_env_HW_ROMs) {
  
	new_speech_gen_inst= new new_speech_gen("new_speech_gen_inst");
	new_speech_gen_inst->clk(clk);
	new_speech_gen_inst->reset(reset);
	new_speech_gen_inst->rom_ns_addr(rom_ns_addr);
	new_speech_gen_inst->rom_ns_data(rom_ns_data);
	new_speech_gen_inst->new_speech(new_speech);
	new_speech_gen_inst->new_speech_req(new_speech_req);
	new_speech_gen_inst->new_speech_ack(new_speech_ack);
	
	rom_ns_inst= new rom_ns("rom_ns_inst");
	rom_ns_inst->clk(clk);
	rom_ns_inst->addr(rom_ns_addr);
	rom_ns_inst->dout(rom_ns_data);

	serial_dec_gen_inst= new serial_dec_gen("serial_dec_gen_inst");
	serial_dec_gen_inst->clk(clk);
	serial_dec_gen_inst->reset(reset);
	serial_dec_gen_inst->rom_sd_addr(rom_sd_addr);
	serial_dec_gen_inst->rom_sd_data(rom_sd_data);
	serial_dec_gen_inst->serial_dec(serial_dec);
	serial_dec_gen_inst->serial_dec_req(serial_dec_req);
	serial_dec_gen_inst->serial_dec_ack(serial_dec_ack);
	
	rom_serial_dec_inst= new rom_serial_dec("rom_serial_dec_inst");
	rom_serial_dec_inst->clk(clk);
	rom_serial_dec_inst->addr(rom_sd_addr);
	rom_serial_dec_inst->dout(rom_sd_data);

	serial_cod_check_inst= new serial_cod_check("serial_cod_check_inst");
	serial_cod_check_inst->clk(clk);
	serial_cod_check_inst->reset(reset);
	serial_cod_check_inst->rom_sc_addr(rom_sc_addr);
	serial_cod_check_inst->rom_sc_data(rom_sc_data);
	serial_cod_check_inst->serial_cod(serial_cod);
	serial_cod_check_inst->serial_cod_req(serial_cod_req);
	serial_cod_check_inst->serial_cod_ack(serial_cod_ack);
	serial_cod_check_inst->serial_cod_match(serial_cod_match);
	serial_cod_check_inst->serial_cod_mismatch(serial_cod_mismatch);
	
	rom_serial_cod_inst= new rom_serial_cod("rom_serial_cod_inst");
	rom_serial_cod_inst->clk(clk);
	rom_serial_cod_inst->addr(rom_sc_addr);
	rom_serial_cod_inst->dout(rom_sc_data);

	synth_check_inst= new synth_check("synth_check_inst");
	synth_check_inst->clk(clk);
	synth_check_inst->reset(reset);
	synth_check_inst->rom_sy_addr(rom_sy_addr);
	synth_check_inst->rom_sy_data(rom_sy_data);
	synth_check_inst->synth(synth);
	synth_check_inst->synth_req(synth_req);
	synth_check_inst->synth_ack(synth_ack);
	synth_check_inst->synth_match(synth_match);
	synth_check_inst->synth_mismatch(synth_mismatch);
	
	rom_synth_inst= new rom_synth("rom_synth_inst");
	rom_synth_inst->clk(clk);
	rom_synth_inst->addr(rom_sy_addr);
	rom_synth_inst->dout(rom_sy_data);

 }
};
