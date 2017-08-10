#Makefile for Hanoi Tower with SCoPE and Skyeye

.PHONY: all scope clean distclean

EXE = queens.x

all: scope
	$(MAKE) -C ./scope
	clear
	./scope/$(EXE)

clean:
	$(MAKE) -C ./scope clean

distclean:
	$(MAKE) -C ./scope distclean

