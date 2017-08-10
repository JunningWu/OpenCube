#==========================================================================
#	Makefile
#	Author: Juan Castillo, David Quijano
#	Date: lun dic 3 2007
#	Description:
#==========================================================================
#  The following code is derived, directly or indirectly, from SCoPE,
#  released June 30, 2008. Copyright (C) 2006 Design of Systems on Silicon (DS2)
#  The Initial Developer of the Original Code is the University of Cantabria
#  for Design of Systems on Silicon (DS2)
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 3 of the License, or
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
#  For more information about SCOPE you can visit
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

 SCOPE_CXX = scope-g++
 SCOPE_FLAGS = --scope-cpu=arm926t --scope-method=op-cost #asm-sentences
 SW_CXX = $(SCOPE_CXX) $(SCOPE_FLAGS)
# SW_CXX = $(SCOPE_HOME)/bin/sw_g++

INC_DIR =	-I$(SCOPE_HOME)/scope \
		-I$(SYSTEMC)/include \
		-I$(SCOPE_HOME)/TLM2/include/tlm

CFLAGS = -c -g -O0 $(INC_DIR) #-DHW_ACCESS=2

SPECIFICATION_LIBS =  -lcoder_c_base -lcommon_c_base

LFLAGS = -L$(SCOPE_XML_PLUGIN)/build -L. -L$(SCOPE_HOME)/scope -L$(SYSTEMC)/lib-linux -L$(SCOPE_HOME)/scope/sicosys/SC_Simul -L$(SCOPE_HOME)/tinyxml -L$(C_BASE_LIB_DIR) #-export-dynamic 
LIBS = -Wl,--whole-archive -lscope -Wl,--no-whole-archive -ltinyxml -lsystemc  -lloadxml -lexpatmm -ldl -lpthread -lrt $(SPECIFICATION_LIBS)

# Executables:
VOCODER = vocoder.x

# Objects
OBJ =	coder_posix.o uc_IO_hw.o
LIB = libvocoder.so

PROJECT_DIR = ..
C_BASE_LIB_DIR = $(PROJECT_DIR)/lib

.PHONY: $(VOCODER)  libcommon_c_base libcoder_c_base

all: libcommon_c_base libcoder_c_base $(LIB) $(VOCODER)

run: libcommon_c_base libcoder_c_base $(LIB) $(VOCODER)  
	export LD_LIBRARY_PATH=.:$(LD_LIBRARY_PATH)
	@echo "To execute the test run './vocoder.x -xml platform.xml -xof output.xml'"


$(LIB): $(OBJ) #sc_main.o
	g++ -fPIC -shared   -g -O3 $(INC_DIR) -o $@  $(OBJ) -fvisibility=protected  $(LFLAGS) $(SPECIFICATION_LIBS) # -rdynamic


libcommon_c_base: 
	echo "compiling libcommon_c_base"
	$(MAKE) -C $(PROJECT_DIR)/c_base/common

libcoder_c_base: 
	echo "compiling libcoder_c_base"
	$(MAKE) -C $(PROJECT_DIR)/c_base/coder

# Compile sc_main.cpp with standar g++
coder_posix.o : coder_posix.cc
	$(SW_CXX) $(CFLAGS) $^ -o $@

uc_IO_hw.o : uc_IO_hw.cc
	$(CXX) $(CFLAGS) $^ -o $@

$(VOCODER): $(OBJ) libcommon_c_base libcoder_c_base 
	$(CXX) $(LFLAGS) -o $(VOCODER) -lvocoder $(LIBS)

# Implicit Rules
.c.o:
	$(SW_CXX) $(CFLAGS) $<

.PHONY: clean
clean:
	rm -rf $(OBJ) sc_main.o main.o $(LIB) output.xml

.PHONY: distclean
distclean: clean
	rm -rf $(VOCODER) prsd_*.ii output.xml
	rm -rf $(C_BASE_LIB_DIR)/*

