//==========================================================================
//	uc_hw_noc.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mié feb 6 2008
//	Description:
//==========================================================================
//  The following code is derived, directly or indirectly, from SCoPE,
//  released June 30, 2008. Copyright (C) 2006 Design of Systems on Silicon (DS2)
//  The Initial Developer of the Original Code is the University of Cantabria
//  for Design of Systems on Silicon (DS2)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License (GPL) or GNU Lesser General Public License(LGPL) as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License (GPL) or GNU Lesser General Public License(LGPL) for more details.
//
//  You should have received a copy of the GNU General Public License (GPL) or GNU Lesser General Public License(LGPL)
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//  For more information about SCOPE you can visit
//  http://www.teisa.unican.es/scope or write an e-mail to
//  scope@teisa.unican.es or a letter to SCoPE, GIM - TEISA, University
//  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
//==========================================================================

#ifndef _UC_HW_NOC_H
#define _UC_HW_NOC_H

#include "hal/uc_hw_if.h"

class UC_NoC_Interface;

class UC_hw_NoC : public UC_hw_if
{
	unsigned char m_MAC[6];
	static unsigned char m_MAC_static[6];
	long long m_node;
public:
	UC_NoC_Interface *m_NoC_simulator;

	UC_hw_NoC(sc_module_name module_name, unsigned int begin, unsigned int end, int irq_num, int ret, UC_NoC_Interface *simulator);

	int read(ADDRESS addr, DATA data, unsigned int size);
	int write(ADDRESS addr, DATA data, unsigned int size);

	int get_node();
	int get_num_nodes();
	
	void set_NoC_simulator(UC_NoC_Interface *simulator);

};

#endif
