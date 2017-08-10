#
#  TEISA DPT.
#  University of Cantabria
#  2009
#
# Makefile to compile the common C base objects of the vocoder
#

PROJECT_DIR = ../..
C_BASE_LIB_DIR = $(PROJECT_DIR)/lib
AR_COMMAND = ar

COMMON_C_BASE_OBJS_DIR = $(C_BASE_LIB_DIR)/common_objs

COMMON_C_BASE_LIST = basicop2 \
					oper_32b \
					dtx \
					inv_sqrt \
					log2 \
					lsp_lsf \
					pow2 \
					copy \
					lsp_az \
					pred_lt6 \
					reorder \
					residu \
					set_zero \
					syn_filt \
					weight_a					


COMMON_C_BASE_SRCS = $(foreach ELEM, $(COMMON_C_BASE_LIST), $(ELEM).c)
COMMON_C_BASE_OBJS = $(foreach ELEM, $(COMMON_C_BASE_LIST), $(COMMON_C_BASE_OBJS_DIR)/$(ELEM).o)

 SCOPE_CXX = scope-g++
 SCOPE_FLAGS = --scope-cpu=arm926t --scope-method=op-cost #asm-sentences
 SW_CXX = $(SCOPE_CXX) $(SCOPE_FLAGS)
# SW_CXX = $(SCOPE_HOME)/bin/sw_g++

INC_DIR =	-I$(SCOPE_HOME)/scope \
		-I$(SYSTEMC)/include \
		-I$(SCOPE_HOME)/TLM2/include/tlm

CFLAGS = -c -g -O0 $(INC_DIR)


all: common_c_base


# Implicit Rules
$(COMMON_C_BASE_OBJS_DIR)/%.o:%.c
	$(SW_CXX) $(CFLAGS) -c $< -o $@

common_c_base:create_work_dirs $(COMMON_C_BASE_OBJS)
	rm -f $(C_BASE_LIB_DIR)/libcommon_c_base.a
	$(AR_COMMAND) r $(C_BASE_LIB_DIR)/libcommon_c_base.a $(COMMON_C_BASE_OBJS_DIR)/*



create_work_dirs:
	mkdir -p $(C_BASE_LIB_DIR)
	mkdir -p $(COMMON_C_BASE_OBJS_DIR)

clean:
	rm -rf $(C_BASE_LIB_DIR)/common_objs
	rm -f $(C_BASE_LIB_DIR)/libcommon_c_base.a
