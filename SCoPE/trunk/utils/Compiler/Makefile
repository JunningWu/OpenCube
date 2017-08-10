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
#  it under the terms of the GNU General Public License (GPL) or GNU Lesser General Public License(LGPL) as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License (GPL) or GNU Lesser General Public License(LGPL) for more details.
#
#  You should have received a copy of the GNU General Public License (GPL) or GNU Lesser General Public License(LGPL)
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#  For more information about SCoPE you can visit
#  http://www.teisa.unican.es/scope or write an e-mail to
#  scope@teisa.unican.es or a letter to SCoPE, GIM - TEISA, University
#  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
#==========================================================================

# Compiler:
CXX = g++
CFLAGS = -g -c -O0

# Linker:
LD = $(CXX)

# Other utilities:
AR = ar
RANLIB = ranlib
TINYXML_ROOT = $(SCOPE_HOME)/tinyxml

# Sources:
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:%.cpp=%.o)

# Compiler options:
INC_DIR = -I. -I./build -I$(TINYXML_ROOT) 

#-I$(GETOPT_HOME)

# Linker options:
LFLAGS =
LIB_DIR = -L$(TINYXML_ROOT) 
#-L$(GETOPT_HOME)
LIB = -ltinyxml 
#-lgetopt

TARGET = scope-g++

.PHONY: all $(TARGET) clean distclean

# Make rules
all: $(TARGET)

# Executable:
$(TARGET) : $(OBJS)
	$(LD) $(LFLAGS) $(LIB_DIR) $(OBJS) -o $(TARGET) $(LIB)

# Object files:
%.o: %.cpp
	$(CXX) $(CFLAGS) $(INC_DIR) $< -o $@

clean:
	$(RM) -rf $(OBJS)

distclean:
	$(RM) -rf $(OBJS) $(TARGET)

