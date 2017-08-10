CXX = g++
INCLUDES= -I../expat
CXXFLAGS = -g -O2 -Wall -fexceptions
CXXCOMPILE = $(CXX) $(INCLUDES) $(CXXFLAGS)
LIBRARY = libmiexpat.a

.SUFFIXES: .cpp .o
.cpp.o:
	$(CXXCOMPILE) -o $@ -c $<

default: $(LIBRARY)

clean:
	rm -f *.o

LIB_EXPATMM_OBJS = ExpatMM-version.o ExpatXMLParser.o
$(LIBRARY): $(LIB_EXPATMM_OBJS)

ExpatMM-version.o: ExpatMM-version.cpp expatmm.h
ExpatXMLParser.o: ExpatXMLParser.cpp ExpatXMLParser.h expatmm.h