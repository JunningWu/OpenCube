# 
# DIRECTORIES = $(shell ls -F | grep "/")
# #DIRECTORIES = $(shell ls -l | grep ^d | awk '{print $9}')
# 
# 
# all:$(DIRECTORIES)
# 	$(foreach dir,$(DIRECTORIES),make -C $(dir);)
# 	ar -cru libcomponents.a */*.o
# 	mv libcomponents.a ../build/
# 
# clean:
# 	$(foreach dir,$(DIRECTORIES),make -C $(dir) clean;)
# 


all: components

clean:
	rm -f */*.o  *.a

SRCS_CPP = $(shell test -e */*.cpp && ls -d */*.cpp)
OBJS_CPP = $(SRCS_CPP:.cpp=.o)

SRCS_CC = $(shell test -e */*.cc && ls -d */*.cc)
OBJS_CC = $(SRCS_CC:.cc=.o)

SRCS_C = $(shell test -e */*.c && ls -d */*.c)
OBJS_C = $(SRCS_C:.c=.o)

LOCAL_INC_DIRS =        $(SCOPE_HOME)/scope                                     \
                        $(SCOPE_HOME)/scope/hal                                 \
                        $(SCOPE_HOME)/scope/rtos/api/posix              \
                        $(SCOPE_HOME)/scope/rtos/api/ucos               \
                        $(SCOPE_HOME)/scope/rtos/drivers                \
                        $(SCOPE_HOME)/scope/rtos/kernel                 \
                        $(SCOPE_HOME)/scope/rtos/low_level              \
                        $(SCOPE_HOME)/scope/rtos/qt_interface   \
                        $(SCOPE_HOME)/scope/rtos/utils

INC_DIRS =      	$(SCOPE_XML_PLUGIN)/expatmm\
			$(SCOPE_XML_PLUGIN)/src\
                        $(SCOPE_HOME)/scope/sicosys/SC_Simul \
                        $(SYSTEMC)/include                      \
                        $(SYSTEMC)/src/sysc/qt          \
                        $(SCOPE_HOME)/TLM2/include/tlm          \
                        $(LOCAL_INC_DIRS)

INC_DIR = $(addprefix -I,$(INC_DIRS))

CFLAGS = -c -g -O0 $(INC_DIR)

LFLAGS = -L. -L$(SCOPE_HOME)/scope -L$(SYSTEMC)/lib-linux -L$(SCOPE_HOME)/scope/sicosys/SC_Simul -L$(SCOPE_HOME)/tinyxml -Wsymbolic #--export-dynamic -Bsymbolic
LIBS = -lscope -ltinyxml -latcs -lsystemc -lpthread -lrt -ldl -lmiexpat

all: 
components: $(OBJS_CPP) $(OBJS_CC) $(OBJS_C)
#	ar -cru libcomponents.a $(OBJS_CPP) $(OBJS_CC) $(OBJS_C)
	g++ -fPIC -shared -g -O3 -rdynamic $(OBJS_CPP)  $(OBJS_C) $(OBJS_CC) -fvisibility=protected -o libcomponents.so
	mv libcomponents.* ../build/
#	g++ -o run.x $(OBJS_CPP) $(OBJS_C) $(LFLAGS) $(LIBS)

.cpp.o:
	g++ -g $(CFLAGS) $(INC_DIR) -c $< -o $@

.cc.o:
	g++ -g $(CFLAGS) $(INC_DIR) -c $< -o $@

.c.o:
	g++ -g $(CFLAGS) $(INC_DIR) -c $< -o $@

