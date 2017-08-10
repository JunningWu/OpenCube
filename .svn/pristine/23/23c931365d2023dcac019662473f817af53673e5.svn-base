CC = gcc
INCLUDES = -I.
CFLAGS = -g -O2 -Wall -Wmissing-prototypes -Wstrict-prototypes -fexceptions

CXX = g++
CPPFLAGS =  -DHAVE_EXPAT_CONFIG_H
CXXFLAGS = -g -O2 -Wall -fexceptions

COMPILE = $(CC) $(INCLUDES) $(CFLAGS) $(DEFS) $(CPPFLAGS)
CXXCOMPILE = $(CXX) $(INCLUDES) $(CXXFLAGS) $(DEFS) $(CPPFLAGS)

ARCHIVER = ar
LIBRARY = libmiexpat.a

default: $(LIBRARY)

clean:
	rm -f *.o $(LIBRARY)

LIB_OBJS = xmlparse.o xmltok.o xmlrole.o
$(LIBRARY): $(LIB_OBJS)
	$(ARCHIVER) rcs $(LIBRARY) $(LIB_OBJS)

xmlparse.o: xmlparse.c expat.h xmlrole.h xmltok.h \
	expat_config.h expat_external.h internal.h

xmlrole.o: xmlrole.c ascii.h xmlrole.h \
	expat_config.h expat_external.h internal.h

xmltok.o: xmltok.c xmltok_impl.c xmltok_ns.c \
	ascii.h asciitab.h iasciitab.h latin1tab.h \
	nametab.h utf8tab.h xmltok.h xmltok_impl.h \
	expat_config.h expat_external.h internal.h

.SUFFIXES: .c .cpp .o

.cpp.o:
	$(CXXCOMPILE) -o $@ -c $<
.c.o:
	$(COMPILE) -o $@ -c $<
