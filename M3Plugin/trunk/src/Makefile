SRCS_CPP = $(shell ls -d *.cpp)
SRCS_CPP += $(shell ls -d io_access/*.cpp)

OBJS_CPP = $(SRCS_CPP:.cpp=.o)



SRCS_C = $(shell ls -d *.c)
SRCS_CPP += $(shell ls -d io_access/*.c)

OBJS_C = $(SRCS_C:.c=.o)

LOCAL_INC_DIRS =        $(SCOPE_HOME)/scope                     \
                        $(SCOPE_HOME)/scope/hal                 \
                        $(SCOPE_HOME)/scope/rtos/api/posix      \
                        $(SCOPE_HOME)/scope/rtos/api/ucos       \
                        $(SCOPE_HOME)/scope/rtos/drivers        \
                        $(SCOPE_HOME)/scope/rtos/kernel         \
                        $(SCOPE_HOME)/scope/rtos/low_level      \
                        $(SCOPE_HOME)/scope/rtos/qt_interface   \
                        $(SCOPE_HOME)/scope/rtos/utils

INC_DIRS =      	../expatmm				\
                        $(SCOPE_HOME)/scope/sicosys/SC_Simul 	\
                        $(SYSTEMC)/include                   	\
                        $(SYSTEMC)/src/sysc/qt          	\
                        $(SCOPE_HOME)/TLM2/include/tlm       	\
                        $(LOCAL_INC_DIRS)

INC_DIR = $(addprefix -I,$(INC_DIRS))

CFLAGS = -c -g -O3 -rdynamic
EXTRA_FLAGS = 

LDFLAGS = -L. -L$(SCOPE_HOME)/scope -L$(SYSTEMC)/lib-linux -L$(SCOPE_HOME)/scope/sicosys/SC_Simul -L$(SCOPE_HOME)/tinyxml  -L$(SCOPE_XML_PLUGIN)/build -Wsymbolic #--export-dynamic -Bsymbolic
LIBS = -Wl,--whole-archive -lscope -Wl,--no-whole-archive -ltinyxml  -lsystemc -lexpatmm -lpthread -lrt -ldl 

#LIB_NAME =  libloadxml.so
LIB_NAME =  libloadxml.a
EXE = scope_tool.x

all: $(LIB_NAME) $(EXE) arp

$(EXE): $(OBJS_CPP) $(OBJS_C)
	g++ -g $(INC_DIR) -O3 -rdynamic -o $@ $(OBJS_CPP)  $(OBJS_C) $(LDFLAGS) $(LIBS)

$(LIB_NAME): $(OBJS_CPP) $(OBJS_C)
	ar -cru $(LIB_NAME) $(OBJS_CPP) $(OBJS_C)
#	g++ -fPIC -shared  -g $(INC_DIR) -O3 -rdynamic -o $@ $(OBJS_CPP)  $(OBJS_C) -fvisibility=protected  $(EXTRA_FLAGS)

.cpp.o:
	g++ -g $(CFLAGS) $(INC_DIR) $(EXTRA_FLAGS) -c $< -o $@

.c.o:
	g++ -g $(CFLAGS) $(INC_DIR) $(EXTRA_FLAGS) -c $< -o $@

arp:
	make -C arp_lib

clean:
	rm -f *.o *.x *.a *.so
	make -C arp_lib clean
