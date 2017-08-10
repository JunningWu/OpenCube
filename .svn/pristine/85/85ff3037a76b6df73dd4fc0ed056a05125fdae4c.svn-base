#
#  Author : Fernando Herrera
#  TEISA DPT.
#  University of Cantabria
#  December 2003
#
#
# Makefile to compile the C base objects of the decoder
#

PROJECT_DIR = ../..
C_BASE_LIB_DIR = $(PROJECT_DIR)/lib
AR_COMMAND = ar

DECODER_C_BASE_OBJS_DIR = $(C_BASE_LIB_DIR)/decoder_objs

DECODER_C_BASE_LIST =	bits2prm\
			dec_lag6 \
			agc \
			preemph \
       			d_gains \
			d1035pf \
			d_plsf_5 \
			dtx_dec\
			pstfilt2\
			d_homing

#DECODER_C_BASE_LIST =	dec_lag6 \
#			agc \
#			preemph \
#       		d_gains \
#			d1035pf \
#			d_plsf_5 \
#			dtx_dec
#dec_12k2\
#pstfilt2\
#int_lpc\
#d_homing
						
				
DECODER_C_BASE_SRCS = $(foreach ELEM, $(DECODER_C_BASE_LIST), $(ELEM).c)
DECODER_C_BASE_OBJS = $(foreach ELEM, $(DECODER_C_BASE_LIST), $(ELEM).o)

decoder_c_base:create_work_dirs $(DECODER_C_BASE_OBJS)
	rm -f $(C_BASE_LIB_DIR)/libdecoder_c_base.a
	$(AR_COMMAND) r $(C_BASE_LIB_DIR)/libdecoder_c_base.a $(DECODER_C_BASE_OBJS_DIR)/*

# SCOPE_CXX = scope-g++
# SCOPE_FLAGS = --scope-cpu=arm926t --scope-method=op-cost #asm-sentences
# SW_CXX = $(SCOPE_CXX) $(SCOPE_FLAGS)
SW_CXX = $(SCOPE_HOME)/bin/sw_g++

INC_DIR =	-I$(SCOPE_HOME)/scope \
		-I$(SYSTEMC)/include \
		-I$(SCOPE_HOME)/TLM2/include/tlm

CFLAGS = -c -g -O0 $(INC_DIR)


all: decoder_c_base


# Implicit Rules
$(DECODER_C_BASE_OBJS_DIR)/%.o:%.c
	$(SW_CXX) $(CFLAGS) -c $< -o $@

decoder_c_base:create_work_dirs $(DECODER_C_BASE_OBJS)
	rm -f $(C_BASE_LIB_DIR)/libdecoder_c_base.a
	$(AR_COMMAND) r $(C_BASE_LIB_DIR)/libdecoder_c_base.a $(DECODER_C_BASE_OBJS_DIR)/*



create_work_dirs:
	mkdir -p $(C_BASE_LIB_DIR)
	mkdir -p $(DECODER_C_BASE_OBJS_DIR)

clean:
	rm -rf $(C_BASE_LIB_DIR)/decoder_objs
	rm -f $(C_BASE_LIB_DIR)/libdecoder_c_base.a
