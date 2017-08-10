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

TINYXML_ROOT = $(SCOPE_HOME)/tinyxml

# Files for Lex and Yacc
LEX_FILE = CxxLexer.l
YACC_FILE = CxxParser.y
LEX_SRC = $(LEX_FILE:%.l=%.cpp)
YACC_SRC = $(YACC_FILE:%.y=%.cpp)
YACC_HDR = $(YACC_FILE:%.y=%.h)
YACC_OUT = $(YACC_FILE:%.y=%.output)

# Compilation units
SOURCES =	$(LEX_SRC)				\
			$(YACC_SRC)				\
			CxxSearchContext.cpp	\
			CxxToken.cpp			\
			CxxAnnotation.cpp		\
			CxxCosts.cpp
OBJS = $(SOURCES:%.cpp=%.o)

# Compiler flags
CXX = g++
CFLAGS = -g -c -O0
INC_DIR = -I$(TINYXML_ROOT)
TARGET = opcost


# Linker flags
LINK = $(CXX)
LFLAGS =
LIB_DIR = -L$(TINYXML_ROOT)
LIB = -ltinyxml -lrt

# Lex flags
LEX = flex
LEX_FLAGS = -l

# Bison flags
YACC = bison
YACC_FLAGS = -d -t -v -y

# Make rules
all: $(TARGET)

# Lex rule:
$(LEX_SRC): $(LEX_FILE) $(YACC_HDR)
	$(LEX) $(LEX_FLAGS) $(LEX_FILE)
	ln -s -f lex.yy.c $@

# Yacc rule:
$(YACC_SRC) $(YACC_HDR): $(YACC_FILE)
	$(YACC) $(YACC_FLAGS) $(YACC_FILE)
	ln -s -f y.tab.c $(YACC_SRC)
	ln -s -f y.tab.h $(YACC_HDR)
	ln -s -f y.output $(YACC_OUT)

# Object files:
.cpp.o:
	$(CXX) $(CFLAGS) $(INC_DIR) $<
	
# Executable:
$(TARGET) : $(OBJS)
	$(LINK) $(LFLAGS) $(LIB_DIR) $(OBJS) -o $(TARGET) $(LIB)


.PHONY: clean
clean:
	$(RM) -rf $(OBJS)

.PHONY: distclean
distclean:
	$(RM) -rf $(OBJS) $(TARGET) $(LEX_SRC) $(YACC_SRC) $(YACC_HDR) $(YACC_OUT) y.tab.h y.tab.c y.output lex.yy.c

# Header dependencies:
$(LEX_SRC): $(YACC_HDR) CxxLexing.hxx CxxLexing.cxx
$(YACC_SRC): CxxParsing.hxx CxxParsing.cxx
CxxAnnotation.cpp: CxxAnnotation.h operators.h CxxCosts.h CxxToken.h CxxSearchContext.h
CxxCosts.cpp: CxxCosts.h operators.h
CxxSearchContext.cpp: CxxSearchContext.h
CxxToken.cpp: CxxToken.h
