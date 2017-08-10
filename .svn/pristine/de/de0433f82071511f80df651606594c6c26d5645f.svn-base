#Makefile for parser tests:

SUB_DIR = $(filter-out ./.%, $(filter ./%, $(sort $(shell find . -maxdepth 1 -type d))))

.PHONY: all clean distclean

all:
	for i in $(SUB_DIR);\
	do\
		$(MAKE) -C $$i;\
	done

clean:
	for i in $(SUB_DIR);\
	do\
		$(MAKE) clean -C $$i;\
	done

distclean: clean
	for i in $(SUB_DIR);\
	do\
		$(MAKE) distclean -C $$i;\
	done
