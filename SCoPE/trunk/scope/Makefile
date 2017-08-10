#==========================================================================
#	Makefile
#	Author: Juan Castillo, David Quijano
#	Date: lun dic 3 2007
#	Description:
#==========================================================================
#  The following code is derived, directly or indirectly, from SCoPE,
#  released June 30, 2008. Copyright (C) 2006 Design of Systems on Silicon (DS2)
#  The Initial Developer of the Original Code is the University of Cantabria
#  for Design of Systems on Silicon (DS2)
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License (GPL) or GNU Lesser General Public License(LGPL) as published by
#  the Free Software Foundation; either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License (GPL) or GNU Lesser General Public License(LGPL) for more details.
#
#  You should have received a copy of the GNU General Public License (GPL) or GNU Lesser General Public License(LGPL)
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#  For more information about SCOPE you can visit
#  http://www.teisa.unican.es/scope or write an e-mail to
#  scope@teisa.unican.es or a letter to SCoPE, GIM - TEISA, University
#  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
#==========================================================================

# Make into the next subdirectories:
SRC_DIR =	hal \
			rtos/api/posix \
			rtos/api/ucos \
			rtos/drivers \
			rtos/kernel \
			rtos/low_level \
			rtos/qt_interface \
			rtos/utils

OBJ_DIR = $(addsuffix /*.o, $(SRC_DIR))
OBJ = $(foreach i, $(OBJ_DIR), $(wildcard $(i)))

.PHONY: $(SRC_DIR)

scope: $(SRC_DIR)
#	$(MAKE) -C sicosys/SC_Simul
	ar cru libscope.a $(OBJ) ../tinyxml/*.o
	ranlib libscope.a
	rm ../lib/libscope.a -rf
	cd ../lib/ && ln -s ../scope/libscope.a && cd -
	

$(SRC_DIR):
	$(MAKE) objects -C $@

clean:
	rm -f `find . -name *.o`
#	make -C rtlinux clean

distclean:
	rm -f `find . -name *.o` `find . -name depend` libscope.a 
#	rm -f sicosys/SC_Simul/libatcs.a
#	make -C rtlinux distclean

depend:
	for i in $(SRC_DIR);			\
	do								\
		$(MAKE) dependent -C $$i;	\
	done
