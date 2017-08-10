#
#  Author : Fernando Herrera
#  TEISA DPT.
#  University of Cantabria
#  December 2003
#
#
# Makefile to compile the C base objects of the coder
#


PROJECT_DIR = ../..
C_BASE_LIB_DIR = $(PROJECT_DIR)/lib
CODER_C_BASE_OBJS_DIR = $(C_BASE_LIB_DIR)/coder_objs
AR_COMMAND = ar

CODER_C_BASE_LIST = az_lsp \
					autocorr \
					convolve \
					c1035pf \
					pitch_ol \
					pitch_f6 \
					enc_lag6 \
					g_pitch \
					q_gains \
					g_code \
					levinson \
					lag_wind \
					int_lpc \
					q_plsf_5 \
					inter_6 \
					vad \
					dtx_cod

CODER_C_BASE_SRCS = $(foreach ELEM, $(CODER_C_BASE_LIST), $(ELEM).c)
CODER_C_BASE_OBJS = $(foreach ELEM, $(CODER_C_BASE_LIST), $(CODER_C_BASE_OBJS_DIR)/$(ELEM).o)	

 SCOPE_CXX = scope-g++
 SCOPE_FLAGS = --scope-cpu=arm926t --scope-method=op-cost #asm-sentences
 SW_CXX = $(SCOPE_CXX) $(SCOPE_FLAGS)
# SW_CXX = $(SCOPE_HOME)/bin/sw_g++

INC_DIR =	-I$(SCOPE_HOME)/scope \
		-I$(SYSTEMC)/include \
		-I$(SCOPE_HOME)/TLM2/include/tlm

CFLAGS = -c -g -O0 $(INC_DIR)

all: coder_c_base


# Implicit Rules
$(CODER_C_BASE_OBJS_DIR)/%.o:%.c
	$(SW_CXX) $(CFLAGS) -c $< -o $@

coder_c_base:create_work_dirs $(CODER_C_BASE_OBJS)
	rm -f $(C_BASE_LIB_DIR)/libcoder_c_base.a
	$(AR_COMMAND) r $(C_BASE_LIB_DIR)/libcoder_c_base.a $(CODER_C_BASE_OBJS_DIR)/*


create_work_dirs:
	mkdir -p $(C_BASE_LIB_DIR)
	mkdir -p $(CODER_C_BASE_OBJS_DIR)


clean:
	rm -rf $(C_BASE_LIB_DIR)/coder_objs
	rm -f $(C_BASE_LIB_DIR)/libcoder_c_base.a

