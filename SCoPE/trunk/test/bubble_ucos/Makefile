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

SCOPE_FLAGS = --scope-cpu=arm926t --scope-method=op-cost --scope-api=ucos/posix --scope-verbose
#SCOPE_FLAGS = --scope-cpu=arm926t --scope-method=asm-sentences --scope-crosscompiler=arm-elf-gcc-3.4.3 --scope-api=ucos/posix --scope-verbose --scope-preserve-files
SW_CXX = scope-g++ $(SCOPE_FLAGS)

INC_DIR = 	-I$(SCOPE_HOME)/scope \
		-I$(SYSTEMC)/include \
		-I$(SCOPE_HOME)/TLM2/include/tlm

LIB_DIR = -L. -L$(SCOPE_HOME)/scope -L$(SYSTEMC)/lib-linux -L$(SCOPE_HOME)/tinyxml
LIB = -lscope -ltinyxml -lsystemc -lpthread -lrt

TARGET=ucos-linux
CC=g++

# PORT=../uCOS-II/Ports/linux-hal
# SOURCE=../uCOS-II/Source

CFLAGS = -I. -O0 -g
#-fcall-used-reg / eax

all: bubble
# ucos book1

# ucos:
# 	$(CC) -c $(CFLAGS) $(PORT)/os_cpu_c.c
# 	$(CC) -c $(CFLAGS) $(SOURCE)/ucos_ii.c

bubble: sc_main.o
	$(SW_CXX) -c $(CFLAGS) $(INC_DIR) bubble.cpp
	$(CXX) $(LIB_DIR) sc_main.o bubble.o -o $(TARGET)-bubble $(LIB)

sc_main.o : sc_main.cpp
	$(CXX) -c $(CFLAGS) $(INC_DIR) $< -o $@

.PHONY: clean
.PHONY: distclean
clean:
	rm -f *.o

distclean:
	rm -f *.o $(TARGET)-bubble *.ii
