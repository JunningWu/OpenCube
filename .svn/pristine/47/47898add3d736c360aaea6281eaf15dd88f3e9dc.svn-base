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

commands:
	@echo
	@echo "  all:         build all"
	@echo "  scope:       build scope"
	@echo "  utils:       build utils"
	@echo "  tinyxml:     build tinyxml"
	@echo "  test:        build test"
	@echo "  clean:       clean project"
	@echo "  distclean:   clean project for distribution"
	@echo


.PHONY: scope
.PHONY: utils
.PHONY: tinyxml
.PHONY: test
.PHONY: plugins

all: tinyxml utils scope plugins

scope: tinyxml
	$(MAKE) scope -C ./scope

utils:
	$(MAKE) -C ./utils

plugins:
	$(MAKE) -C ./plugins

tinyxml:
	$(MAKE) -C ./tinyxml

test: scope utils
	$(MAKE) test -C ./test

.PHONY: clean
.PHONY: distclean
clean:
	$(MAKE) clean -C ./tinyxml
	$(MAKE) clean -C ./utils
	$(MAKE) clean -C ./scope
	$(MAKE) clean -C ./test
	$(MAKE) clean -C ./plugins
	rm bin/* -rf
	rm lib/* -rf
	

distclean:
	$(MAKE) distclean -C ./tinyxml
	$(MAKE) distclean -C ./utils
	$(MAKE) distclean -C ./scope
	$(MAKE) distclean -C ./test
	$(MAKE) distclean -C ./plugins
	rm bin/* -rf
	rm lib/* -rf

depend:
	$(MAKE) depend -C ./scope
