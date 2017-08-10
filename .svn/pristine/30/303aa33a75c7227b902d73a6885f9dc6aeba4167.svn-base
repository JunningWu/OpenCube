#==========================================================================
#	Makefile
#	Author: Juan Castillo, David Quijano
#	Date: lun dic 3 2007
#	Description:
#==========================================================================
#  Copyright (C) 2008 Design of Systems on Silicon(DS2)
#  Main developer: University of Cantabria
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#  For more information about SCoPE you can visit
#  http://www.teisa.unican.es/scope or write an e-mail to
#  scope@teisa.unican.es or a letter to SCoPE, GIM - TEISA, University
#  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
#==========================================================================

#
#	makefile for GSM Enhanced Full Rate Speech Codec
#	Version 5.1.0
#	June 26, 1996
#

# For GCC compiler
CXX = g++

SCOPE_CXX = $(SCOPE_HOME)/bin/scope-g++
SCOPE_FLAGS = --scope-cpu=arm926t --scope-method=op-cost #asm-sentences
SW_CXX = $(SCOPE_CXX) $(SCOPE_FLAGS)
# SW_CXX = $(SCOPE_HOME)/compiler/sw_g++


SCOPE_INC_DIR =	-I$(SCOPE_HOME)/scope \
		-I$(SYSTEMC)/include \
		-I$(SCOPE_HOME)/TLM2/include/tlm

LOCAL_INC_DIRS =        $(SCOPE_HOME)/scope                     \
                        $(SCOPE_HOME)/scope/hal                 \
                        $(SCOPE_HOME)/scope/rtos/api/posix      \
                        $(SCOPE_HOME)/scope/rtos/api/ucos       \
                        $(SCOPE_HOME)/scope/rtos/drivers        \
                        $(SCOPE_HOME)/scope/rtos/kernel         \
                        $(SCOPE_HOME)/scope/rtos/low_level      \
                        $(SCOPE_HOME)/scope/rtos/qt_interface   \
                        $(SCOPE_HOME)/scope/rtos/utils

INC_DIR = $(addprefix -I,$(LOCAL_INC_DIRS)) $(SCOPE_INC_DIR)

CFLAGS = -c -g -O0 $(INC_DIR)

LFLAGS = -L../../build -L. -L$(SCOPE_HOME)/scope -L$(SYSTEMC)/lib-linux -L$(SCOPE_HOME)/scope/sicosys/SC_Simul -L$(SCOPE_HOME)/tinyxml -export-dynamic
LIBS = -Wl,--whole-archive -lscope -Wl,--no-whole-archive -ltinyxml -lsystemc  -lloadxml -lexpatmm -ldl -lpthread -lrt

# Executables:
VOCODER = vocoder.x

# Objects
OBJ =	coder.o cod_12k2.o az_lsp.o autocorr.o convolve.o c1035pf.o basicop2.o \
		pitch_ol.o pitch_f6.o enc_lag6.o g_pitch.o residu.o count.o weight_a.o \
		q_gains.o g_code.o pred_lt6.o copy.o levinson.o lag_wind.o prm2bits.o \
		syn_filt.o int_lpc.o q_plsf_5.o set_zero.o log2.o pow2.o oper_32b.o \
		lsp_lsf.o reorder.o lsp_az.o inv_sqrt.o inter_6.o pre_proc.o \
		vad.o dtx.o e_homing.o \
		decoder.o dec_12k2.o pstfilt2.o dec_lag6.o agc.o preemph.o \
		bits2prm.o d_plsf_5.o d_gains.o d1035pf.o d_homing.o \
		ed_iface.o

SCOPE_PROJECT=config.scope


.PHONY: $(VOCODER)
all: $(VOCODER)

# Compile sc_main.cpp with standar g++
#sc_main.o : sc_main.cpp
#	$(CXX) $(CFLAGS) $^ -o $@

$(VOCODER): main.o $(OBJ) #sc_main.o
	$(CXX) $(LFLAGS) -o $@ $^ $(LIBS)

# Implicit Rules
.c.o:
	$(SW_CXX) $(CFLAGS) $<

.PHONY: run
run:$(VOCODER)
	./$(VOCODER) -xml platform.xml -xmd dse.xml -xsc parameter.xml -xof output.xml

.PHONY: clean
clean:
	rm -rf $(OBJ) *.ii sc_main.o main.o prsd_* $(VOCODER) output.xml

.PHONY: distclean
distclean: clean
	rm -rf $(VOCODER)

# Individual File Dependencies
main.o:		coder.h decoder.h ed_iface.h
agc.o:      typedef.h basic_op.h count.h sig_proc.h cnst.h
autocorr.o: typedef.h basic_op.h oper_32b.h count.h cnst.h
az_lsp.o:   typedef.h basic_op.h oper_32b.h count.h cnst.h grid.tab
basicop2.o: typedef.h basic_op.h count.h
bits2prm.o: typedef.h basic_op.h count.h
c1035pf.o:  typedef.h basic_op.h sig_proc.h count.h
cod_12k2.o: typedef.h basic_op.h sig_proc.h count.h codec.h cnst.h \
	    window2.tab vad.h dtx.h
coder.o:    typedef.h basic_op.h sig_proc.h count.h codec.h cnst.h \
            n_stack.h e_homing.h dtx.h
convolve.o: typedef.h basic_op.h count.h
copy.o:     typedef.h basic_op.h count.h
count.o:    typedef.h count.h
d1035pf.o:  typedef.h basic_op.h count.h
d_gains.o:  typedef.h basic_op.h oper_32b.h count.h sig_proc.h gains_tb.h \
	    cnst.h dtx.h
d_homing.o: typedef.h cnst.h dtx.h codec.h d_homing.h q_plsf_5.tab
d_plsf_5.o: typedef.h basic_op.h count.h sig_proc.h q_plsf_5.tab cnst.h dtx.h
dec_12k2.o: typedef.h basic_op.h sig_proc.h count.h codec.h cnst.h dtx.h
dec_lag6.o: typedef.h basic_op.h count.h
decoder.o:  typedef.h n_stack.h basic_op.h sig_proc.h count.h codec.h \
            cnst.h d_homing.h
dtx.o:      typedef.h basic_op.h cnst.h sig_proc.h count.h dtx.h
e_homing.o: typedef.h cnst.h vad.h dtx.h codec.h sig_proc.h e_homing.h
ed_iface.o: typedef.h cnst.h dtx.h
enc_lag6.o: typedef.h basic_op.h count.h
g_code.o:   typedef.h basic_op.h count.h cnst.h
g_pitch.o:  typedef.h basic_op.h oper_32b.h count.h sig_proc.h
int_lpc.o:  typedef.h basic_op.h count.h sig_proc.h
inter_6.o:  typedef.h basic_op.h count.h
inv_sqrt.o: typedef.h basic_op.h count.h inv_sqrt.tab
lag_wind.o: typedef.h basic_op.h oper_32b.h count.h lag_wind.tab
levinson.o: typedef.h basic_op.h oper_32b.h count.h
log2.o:     typedef.h basic_op.h count.h log2.tab
lsp_az.o:   typedef.h basic_op.h oper_32b.h count.h sig_proc.h
lsp_lsf.o:  typedef.h basic_op.h count.h lsp_lsf.tab
n_proc.o:
oper_32b.o: typedef.h basic_op.h oper_32b.h count.h
pitch_f6.o: typedef.h basic_op.h oper_32b.h count.h sig_proc.h codec.h
pitch_ol.o: typedef.h basic_op.h oper_32b.h count.h sig_proc.h
pow2.o:     typedef.h basic_op.h count.h pow2.tab
pre_proc.o: typedef.h basic_op.h oper_32b.h count.h
pred_lt6.o: typedef.h basic_op.h count.h
preemph.o:  typedef.h basic_op.h count.h
prm2bits.o: typedef.h basic_op.h count.h
pstfilt2.o: typedef.h basic_op.h sig_proc.h count.h codec.h cnst.h
q_gains.o:  typedef.h basic_op.h oper_32b.h count.h sig_proc.h gains_tb.h \
	    cnst.h dtx.h
q_plsf_5.o: typedef.h basic_op.h count.h sig_proc.h cnst.h dtx.h q_plsf_5.tab
reorder.o:  typedef.h basic_op.h count.h
residu.o:   typedef.h basic_op.h count.h
set_zero.o: typedef.h basic_op.h count.h
syn_filt.o: typedef.h basic_op.h count.h
vad.o:      typedef.h cnst.h basic_op.h oper_32b.h count.h vad.h
weight_a.o: typedef.h basic_op.h count.h
