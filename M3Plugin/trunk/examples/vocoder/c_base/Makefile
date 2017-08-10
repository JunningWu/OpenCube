#
#  Author : Fernando Herrera
#  TEISA DPT.
#  University of Cantabria
#  December 2003
#
# Makefile for constructing all the libraries necessary to
# generate coder, decoder and vocoder libraries
#

include ../Makefile.base

vocoder_c_base:coder_c_base decoder_c_base

coder_c_base: common_c_base
	$(MAKE) -C ./coder 

decoder_c_base: common_c_base
	$(MAKE) -C ./decoder

common_c_base:
	$(MAKE) -C ./common

clean:
	$(MAKE) -C ./common clean
	$(MAKE) -C ./coder clean
	$(MAKE) -C ./decoder clean
	rmdir $(C_BASE_LIB_DIR)

ultraclean:
	rm -rf ../lib
	rm -rf ../COS_lib
	rm -rf ../OR_lib
