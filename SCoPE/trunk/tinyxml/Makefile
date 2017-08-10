#****************************************************************************
#
# Makefile for TinyXml test.
# Lee Thomason
# www.grinninglizard.com
#
# This is a GNU make (gmake) makefile
#****************************************************************************

# DEBUG can be set to YES to include debugging info, or NO otherwise
DEBUG          := YES

# PROFILE can be set to YES to include profiling info, or NO otherwise
PROFILE        := NO

# TINYXML_USE_STL can be used to turn on STL support. NO, then STL
# will not be used. YES will include the STL files.
#TINYXML_USE_STL := YES

#****************************************************************************

CC     := gcc
# CXX    := g++
# LD     := g++
AR     := ar
RANLIB := ranlib

DEBUG_CFLAGS     := -Wall -Wno-format -g -DDEBUG -O0
RELEASE_CFLAGS   := -Wall -Wno-unknown-pragmas -Wno-format -O2

LIBS		 :=

DEBUG_CXXFLAGS   := ${DEBUG_CFLAGS}
RELEASE_CXXFLAGS := ${RELEASE_CFLAGS}

DEBUG_LDFLAGS    := -g
RELEASE_LDFLAGS  :=

ifeq (YES, ${DEBUG})
   CFLAGS       := ${DEBUG_CFLAGS}
   CXXFLAGS     := ${DEBUG_CXXFLAGS}
   LDFLAGS      := ${DEBUG_LDFLAGS}
else
   CFLAGS       := ${RELEASE_CFLAGS}
   CXXFLAGS     := ${RELEASE_CXXFLAGS}
   LDFLAGS      := ${RELEASE_LDFLAGS}
endif

ifeq (YES, ${PROFILE})
   CFLAGS   := ${CFLAGS} -pg -O2
   CXXFLAGS := ${CXXFLAGS} -pg -O2
   LDFLAGS  := ${LDFLAGS} -pg
endif

#****************************************************************************
# Preprocessor directives
#****************************************************************************

ifeq (YES, ${TINYXML_USE_STL})
  DEFS := -DTIXML_USE_STL
else
  DEFS :=
endif

#****************************************************************************
# Include paths
#****************************************************************************

#INCS := -I/usr/include/g++-2 -I/usr/local/include
INCS :=


#****************************************************************************
# Makefile code common to all platforms
#****************************************************************************

CFLAGS   := ${CFLAGS}   ${DEFS}
CXXFLAGS := ${CXXFLAGS} ${DEFS}

#****************************************************************************
# Targets of the build
#****************************************************************************

#Select library or executable for target
#OUTPUT := xmltest
OUTPUT := libtinyxml.a

all: ${OUTPUT}


#****************************************************************************
# Source files
#****************************************************************************

#SRCS := tinyxml.cpp tinyxmlparser.cpp xmltest.cpp tinyxmlerror.cpp tinystr.cpp
SRCS := tinyxml.cpp tinyxmlparser.cpp tinyxmlerror.cpp tinystr.cpp

# Add on the sources for libraries
SRCS := ${SRCS}

OBJS := $(addsuffix .o,$(basename ${SRCS}))

#****************************************************************************
# Output
#****************************************************************************

${OUTPUT}: ${OBJS}
	${AR} -cru $@ ${OBJS} ${LIBS} ${EXTRA_LIBS}
	${RANLIB} $@
	rm ../lib/${OUTPUT} -rf
	cd ../lib && ln -s ../tinyxml/${OUTPUT} && cd -

xmltest: ${OBJS}
	${LD} -o $@ ${LDFLAGS} ${OBJS} ${LIBS} ${EXTRA_LIBS}

#****************************************************************************
# common rules
#****************************************************************************

# Rules for compiling source files to object files
%.o : %.cpp
	${CXX} -c ${CXXFLAGS} ${INCS} $< -o $@

%.o : %.c
	${CC} -c ${CFLAGS} ${INCS} $< -o $@

dist:
	bash makedistlinux

.PHONY: clean
.PHONY: distclean
clean:
	-rm -f core *.o

distclean:
	-rm -f core *.o libtinyxml.a xmltest

depend:
	#makedepend ${INCS} ${SRCS}

tinyxml.o: tinyxml.h tinystr.h
tinyxmlparser.o: tinyxml.h tinystr.h
xmltest.o: tinyxml.h tinystr.h
tinyxmlerror.o: tinyxml.h tinystr.h

