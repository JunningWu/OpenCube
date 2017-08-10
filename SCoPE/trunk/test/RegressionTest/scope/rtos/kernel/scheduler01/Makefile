#==========================================================================
#	Makefile
#	Author: Juan Castillo, David Quijano
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

SCOPE_FLAGS = --scope-cpu=arm926t --scope-method=op-cost --scope-hw
SW_CXX = scope-g++ $(SCOPE_FLAGS)

INC_DIR = 	-I$(SCOPE_HOME)/scope \
		-I$(SYSTEMC)/include \
		-I$(SCOPE_HOME)/TLM2/include/tlm

# ifeq (Makefile.paths, $(wildcard $(SCOPE_HOME)/Makefile.paths))
# include $(SCOPE_HOME)/Makefile.paths
# else
# COMPILER_HOME = $(SCOPE_HOME)
# endif
# 
# SW_CXX = $(COMPILER_HOME)/compiler/sw_g++
# LOCAL_INC_DIRS =	$(SCOPE_HOME)/scope 					\
# 					$(SCOPE_HOME)/scope/hal 				\
# 					$(SCOPE_HOME)/scope/rtos/api/posix		\
# 					$(SCOPE_HOME)/scope/rtos/api/ucos 		\
# 					$(SCOPE_HOME)/scope/rtos/drivers 		\
# 					$(SCOPE_HOME)/scope/rtos/kernel 		\
# 					$(SCOPE_HOME)/scope/rtos/low_level 		\
# 					$(SCOPE_HOME)/scope/rtos/qt_interface	\
# 					$(SCOPE_HOME)/scope/rtos/utils
# 
# INC_DIRS =	\
# 			$(SCOPE_HOME)/scope/sicosys/SC_Simul \
# 			$(SYSTEMC)/include 			\
# 			$(SCOPE_HOME)/TLM2/include/tlm		\
# 			$(LOCAL_INC_DIRS)
# 
# INC_DIR = $(addprefix -I,$(INC_DIRS))

CFLAGS = -c -g -O0 -DWMOPS=1 $(INC_DIR)

LFLAGS = -L. -L$(SCOPE_HOME)/scope -L$(SYSTEMC)/lib-linux
LIBS = -lscope -lsystemc -lpthread -lrt

OBJS = test.o

OUT = run.x

# Linker rule:
simple_test: sc_main.o $(OBJS)
	$(CXX) $^ -o $(OUT) $(LFLAGS) $(LIBS)

sc_main.o : sc_main.cpp
	$(CXX) -g -c $(INC_DIR) $<

.cpp.o:
	$(SW_CXX) -g -c $(INC_DIR) $^

clean:
	rm -f *.o *.ii prsd_*

distclean: clean
	rm -f $(OUT)
