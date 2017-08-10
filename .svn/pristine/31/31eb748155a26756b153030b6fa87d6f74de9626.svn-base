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

# SCoPE options:
SCOPE_CXX = scope-g++
SCOPE_FLAGS = --scope-method=op-cost #--scope-preserve-files --scope-verbose --scope-crosscompiler=arm-linux-gcc --scope-method=asm-sentences --scope-language=c
#SCOPE_FLAGS = --scope-crosscompiler=arm-elf-gcc --scope-method=asm-sentences --scope-preserve-files --scope-verbose #--scope-nodcache #--scope-optimizations #--scope-noicache --scope-notime
#SCOPE_FLAGS = --scope-method=op-cost --scope-preserve-files --scope-verbose --scope-crosscompiler=arm-elf-gcc-3.4.3 --scope-
#SCOPE_FLAGS = --scope-crosscompiler=arm-elf-gcc-3.4.3 --scope-method=asm-sentences --scope-preserve-files --scope-verbosemethod=asm-sentences --scope-language=c
#SCOPE_FLAGS = --scope-method=asm-sentences --scope-verbose --scope-crosscompiler=arm-elf-gcc-3.4.3 --scope-verbose --scope-method=asm-sentences --scope-language=c --scope-preserve-files

SCOPE_INC_DIR =	-I$(SCOPE_HOME)/scope \
				-I$(SYSTEMC)/include \
				-I$(SCOPE_HOME)/TLM2/include/tlm

# Compiler options:
CXX = g++
DEBUG = -g
OPT = -O0
# OPT = -O2
CFLAGS = $(DEBUG) $(OPT) -c
INC_DIR = 
LIB_DIR = -L$(SYSTEMC)/lib-linux -L$(SCOPE_HOME)/scope -L$(SCOPE_HOME)/tinyxml
LIB = -lscope -ltinyxml -lsystemc -lpthread -lrt

OBJS =  factorial.o
OUT = factorial.x

.PHONY: $(OUT)
# Link:
$(OUT): $(OBJS) sc_main.o
	$(CXX) $(LIB_DIR) $^ -o $@ $(LIB)

# Parse and compile software application files:
%.o: %.cpp
	$(SCOPE_CXX) $(SCOPE_FLAGS) $(CFLAGS) $(INC_DIR) $^ -o $@

# Compile sc_main.cpp with standar g++
sc_main.o : sc_main.cpp
	$(CXX) $(CFLAGS) $(SCOPE_INC_DIR) $^ -o $@

# Clean:
.PHONY: clean distclean
clean:
	rm -rf $(OBJS) sc_main.o

distclean: clean
	rm -rf $(OUT)

# File dependencies:
factorial.o: factorial.cpp

