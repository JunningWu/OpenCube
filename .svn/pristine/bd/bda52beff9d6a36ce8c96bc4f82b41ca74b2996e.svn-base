//
// fifos.h
//
// fifos for coder_posix
//
//	TEISA Dpt.
//  University of Cantabria
//  Author: J.A.Adamez Martin
//  Date:	Enero 2005
//


#ifndef _FIFOS_H
#define _FIFOS_H

#include "sc_fifo_SS_3.h"	
//#include "sc_fifo_SS_2.h"	

#ifndef HW_ACCESS
__thread sc_fifo_SS<Word16> *sample_in;
__thread sc_fifo_SS<bool> *dtx_mode_in;
__thread sc_fifo_SS<bool> *prm_out;
#endif
//Para pre_filtering y homing_frame_test

__thread sc_fifo_SS<Word16> *IN_SAMPLE;
__thread sc_fifo_SS<Word16> *PREPROC_INBAND_RESET;
__thread sc_fifo_SS<Word16> *preproc_sample_1;	
__thread sc_fifo_SS<Word16> *preproc_sample_2;	
__thread sc_fifo_SS<Word16> *preproc_sample_3;		
__thread sc_fifo_SS<bool> *inband_reset_1;
__thread sc_fifo_SS<bool> *inband_reset_2;
__thread sc_fifo_SS<bool> *inband_reset_3;
__thread sc_fifo_SS<bool> *inband_reset_4;
__thread sc_fifo_SS<bool> *inband_reset_5;	

//Para frame_lsp_fun

__thread sc_fifo_SS<bool> *dtx_mode_1;
__thread sc_fifo_SS<bool> *dtx_mode_2;
__thread sc_fifo_SS<bool> *dtx_mode_3;
__thread sc_fifo_SS<Word16> *txdtx_ctrl_vad_1;
__thread sc_fifo_SS<Word16> *A_13;
__thread sc_fifo_SS<Word16> *A_24;
__thread sc_fifo_SS<Word16> *lsp;
__thread sc_fifo_SS<Word16> *Aq_lpc;
__thread sc_fifo_SS<Word16> *r;
__thread sc_fifo_SS<Word16> *scal_acf_;
__thread sc_fifo_SS<Word16> *rc_;
__thread sc_fifo_SS<Word16> *lpc_prm;

//Para frame_int_tol

__thread sc_fifo_SS<Word16> *txdtx_ctrl_vad_2;
__thread sc_fifo_SS<Word16> *A_;
__thread sc_fifo_SS<Word16> *lags_;
__thread sc_fifo_SS<Word16> *tol;

//Para frame_int_tol

__thread sc_fifo_SS<Word16> *txdtx_ctrl_vad_3 ;
__thread sc_fifo_SS<Word16> *txdtx_ctrl_vad_4;
__thread sc_fifo_SS<Word16> *txdtx_ctrl_vad_5;
__thread sc_fifo_SS<bool> *l_pn_seed_tx_reset; 

//Para subframe_coder_fun

__thread sc_fifo_SS<Word16> *subfrm_prm ;

//Para cn_encoder_fun

__thread sc_fifo_SS<Word16> *prm_cn;

//Para serializer_fun

__thread sc_fifo_SS<bool> *bit_stream_; 


void pre_filt_fifos(){
#ifndef HW_ACCESS
	sample_in = new sc_fifo_SS<Word16>("sample_in",L_FRAME);
#endif
	IN_SAMPLE = new sc_fifo_SS<Word16>("IN_SAMPLE",L_FRAME);
	preproc_sample_1 = new sc_fifo_SS<Word16>("preproc_sample_1",L_FRAME);	
	preproc_sample_2 = new sc_fifo_SS<Word16>("preproc_sample_2",L_FRAME);	
	preproc_sample_3 = new sc_fifo_SS<Word16>("preproc_sample_3",L_FRAME);		
	PREPROC_INBAND_RESET = new sc_fifo_SS<Word16>("PREPROC_INBAND_RESET",1);
}
void homing_fifos(){
	IN_SAMPLE = new sc_fifo_SS<Word16>("IN_SAMPLE",L_FRAME);
	PREPROC_INBAND_RESET = new sc_fifo_SS<Word16>("PREPROC_INBAND_RESET",1);
	inband_reset_1 = new sc_fifo_SS<bool>("inband_reset_1",1);
	inband_reset_2 = new sc_fifo_SS<bool>("inband_reset_2",1);
	inband_reset_3 = new sc_fifo_SS<bool>("inband_reset_3",1);
	inband_reset_4 = new sc_fifo_SS<bool>("inband_reset_4",1);
	inband_reset_5 = new sc_fifo_SS<bool>("inband_reset_5",1);	
}

void frame_lsp_fifos(){	
	//Para frame_lsp_fun
#ifndef HW_ACCESS
	dtx_mode_in= new sc_fifo_SS<bool>("dtx_mode_in",1);
#endif
	dtx_mode_1= new sc_fifo_SS<bool>("dtx_mode_1",1);
	dtx_mode_2= new sc_fifo_SS<bool>("dtx_mode_2",1);
	dtx_mode_3= new sc_fifo_SS<bool>("dtx_mode_3",1);
	lsp=new sc_fifo_SS<Word16>("lsp",L_FRAME);
	preproc_sample_1 = new sc_fifo_SS<Word16>("preproc_sample_1",L_FRAME);	
	A_24=new sc_fifo_SS<Word16>("A_24",L_FRAME);
	scal_acf_=new sc_fifo_SS<Word16>("scal_acf_",1);
	r=new sc_fifo_SS<Word16>("r",MP1);
	rc_=new sc_fifo_SS<Word16>("rc_",4);
	txdtx_ctrl_vad_1=new sc_fifo_SS<Word16>("txdtx_ctrl_vad_1",1);
	lpc_prm=new sc_fifo_SS<Word16>("prm",5);
	Aq_lpc=new sc_fifo_SS<Word16>("Aq_lpc",L_FRAME);
	A_13=new sc_fifo_SS<Word16>("A_13",L_FRAME);
	inband_reset_1 = new sc_fifo_SS<bool>("inband_reset_1",1);
}

void frame_int_fifos(){	
	dtx_mode_1= new sc_fifo_SS<bool>("dtx_mode_1",1);
	lsp=new sc_fifo_SS<Word16>("lsp",L_FRAME);
	A_24=new sc_fifo_SS<Word16>("A_24",L_FRAME);
	A_=new sc_fifo_SS<Word16>("A_",4*MP1);
	txdtx_ctrl_vad_2=new sc_fifo_SS<Word16>("txdtx_ctrl_vad_2",1);
	A_13=new sc_fifo_SS<Word16>("A_13",L_FRAME);
	preproc_sample_2 = new sc_fifo_SS<Word16>("preproc_sample_2",L_FRAME);	
	lags_=new sc_fifo_SS<Word16>("lags",2);
	tol=new sc_fifo_SS<Word16>("tol",2);
	inband_reset_2 = new sc_fifo_SS<bool>("inband_reset_2",1);

}

void vad_fifos(){
	dtx_mode_3= new sc_fifo_SS<bool>("dtx_mode_3",1);
	r=new sc_fifo_SS<Word16>("r",MP1);
	scal_acf_=new sc_fifo_SS<Word16>("scal_acf_",1);
	rc_=new sc_fifo_SS<Word16>("rc_",4);
	l_pn_seed_tx_reset= new sc_fifo_SS<bool>("l_pn_seed_tx_reset",1); 

	txdtx_ctrl_vad_1 = new sc_fifo_SS<Word16>("txdtx_ctrl_vad_1",1);
	txdtx_ctrl_vad_2 = new sc_fifo_SS<Word16>("txdtx_ctrl_vad_2",1);
	txdtx_ctrl_vad_3 = new sc_fifo_SS<Word16>("txdtx_ctrl_vad_3",1);
	txdtx_ctrl_vad_4 = new sc_fifo_SS<Word16>("txdtx_ctrl_vad_4",1);
	txdtx_ctrl_vad_5 = new sc_fifo_SS<Word16>("txdtx_ctrl_vad_5",1);

	lags_=new sc_fifo_SS<Word16>("lags",2);
	inband_reset_4 = new sc_fifo_SS<bool>("inband_reset_4",1);

}

void subframe_fifos(){
	dtx_mode_2= new sc_fifo_SS<bool>("dtx_mode_2",1);
	A_=new sc_fifo_SS<Word16>("A_",4*MP1);
	Aq_lpc=new sc_fifo_SS<Word16>("Aq_lpc",L_FRAME);
	tol=new sc_fifo_SS<Word16>("tol",2);
	preproc_sample_3 = new sc_fifo_SS<Word16>("preproc_sample_3",L_FRAME);		
	txdtx_ctrl_vad_3 = new sc_fifo_SS<Word16>("txdtx_ctrl_vad_3",1);
	subfrm_prm =new sc_fifo_SS<Word16>("subfrm_prm",13*4);
	l_pn_seed_tx_reset= new sc_fifo_SS<bool>("l_pn_seed_tx_reset",1); 
	inband_reset_3 = new sc_fifo_SS<bool>("inband_reset_3",1);
}


void cn_encoder_fifos(){
	txdtx_ctrl_vad_4 = new sc_fifo_SS<Word16>("txdtx_ctrl_vad_4",1);
	lpc_prm=new sc_fifo_SS<Word16>("prm",5);
	subfrm_prm =new sc_fifo_SS<Word16>("subfrm_prm",13*4);
	prm_cn=new sc_fifo_SS<Word16>("prm_cn",PRM_SIZE);
	inband_reset_5 = new sc_fifo_SS<bool>("inband_reset_5",1);
}	

void serializer_fifos(){
	prm_cn=new sc_fifo_SS<Word16>("prm_cn",PRM_SIZE);
	bit_stream_=new sc_fifo_SS<bool>("bit_stream_",SERIAL_SIZE-1); 
}
	

void sid_fifos(){
	txdtx_ctrl_vad_5 = new sc_fifo_SS<Word16>("txdtx_ctrl_vad_5",1);
	bit_stream_=new sc_fifo_SS<bool>("bit_stream_",SERIAL_SIZE-1); 
#ifndef HW_ACCESS
	prm_out=new sc_fifo_SS<bool>("prm_out",244+2);
#endif
}

#ifndef HW_ACCESS
void PCM_fifos(){
	dtx_mode_in= new sc_fifo_SS<bool>("dtx_mode_in",1);
	sample_in = new sc_fifo_SS<Word16>("sample_in",L_FRAME);
}
	
void Radio_fifos(){
	prm_out=new sc_fifo_SS<bool>("prm_out",244+2);
	
}
#endif
#endif
