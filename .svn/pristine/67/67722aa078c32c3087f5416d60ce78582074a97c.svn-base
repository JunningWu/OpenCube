.PHONY: examples

ARCHIVER = ar
LIBRARY = libexpatmm.a
EXPAT = ./expat
EXPATMM = ./expatmm

ADDED_FLAGS =   


all: build libmiexpat scope_tool libloadxml components

distclean: clean

clean:
	make -C expat clean
	make -C expatmm clean
	make -C src clean
	make -C examples clean
	make -C scope_components clean
	make -C src/arp_lib clean
	rm -rf build

build:
	mkdir build

arp_lib: build
	make -C src/arp_lib
	cp src/arp_lib/lib/libarp_scope.so build/libarp_scope.so

LIB_EXPAT_OBJS = $(EXPAT)/xmlparse.o $(EXPAT)/xmltok.o $(EXPAT)/xmlrole.o
LIB_EXPATMM_OBJS = $(EXPATMM)/ExpatMM-version.o $(EXPATMM)/ExpatXMLParser.o
libmiexpat:
	make -C expat
	make -C expatmm
	$(ARCHIVER) rcs $(LIBRARY) $(LIB_EXPAT_OBJS) $(LIB_EXPATMM_OBJS)
	mv $(LIBRARY) build
	cp $(EXPAT)/expat_external.h $(EXPAT)/expat.h src
	cp $(EXPATMM)/*.h src

libloadxml:
	make -C src  EXTRA_FLAGS="$(ADDED_FLAGS) " 
	mv -f src/libloadxml.* build/

scope_tool:
	make -C src scope_tool.x
	mv -f src/scope_tool.x build/

examples:
	make -C examples

components:
	make -C scope_components

run: all
	make -C examples run

no_platform:
	make -C src no_platform
	mv -f src/libloadxml.* build/
