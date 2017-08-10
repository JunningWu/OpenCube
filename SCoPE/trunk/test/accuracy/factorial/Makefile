#Makefile for Factorial with SCoPE and Skyeye

.PHONY: all scope clean distclean

EXE = factorial.x

all: scope
	$(MAKE) -C ./scope
	clear
	./scope/$(EXE)

clean:
	$(MAKE) -C ./scope clean

distclean:
	$(MAKE) -C ./scope distclean

