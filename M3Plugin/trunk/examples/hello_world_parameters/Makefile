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
EXE = run.x

# Objects
OBJ =	hello.o

.PHONY: $(EXE)
all: $(EXE)

# Compile sc_main.cpp with standar g++
#sc_main.o : sc_main.cpp
#	$(CXX) $(CFLAGS) $^ -o $@

$(EXE): $(OBJ) #sc_main.o
	$(CXX) $(LFLAGS) -o $@ $^ $(LIBS)

# Implicit Rules
.c.o:
	$(SW_CXX) $(CFLAGS) $<

.PHONY: run
run:$(EXE)
	export SCOPE_PROJECT=config.scope
	./$(EXE) -xml platform.xml -xsc parameter.xml -xmd metrics.xml -xof output.xml

.PHONY: clean
clean:
	rm -rf $(OBJ) *.ii prsd_* $(EXE) output.xml

.PHONY: distclean
distclean: clean
	rm -rf $(EXE)
