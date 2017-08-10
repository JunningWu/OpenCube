//==========================================================================
//	uc_tlm_bus.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: lun ago 25 2008
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

#ifndef UC_TLM_BUS_AMBA_CLASS_H
#define UC_TLM_BUS_AMBA_CLASS_H

#include <vector>
#include "systemc.h"
#include "tlm.h"
using namespace tlm;
using namespace std;

#include "uc_tlm_bus_class.h"

class UC_hw_if_base;

#define NUM_IRQS 16

class UC_TLM_bus_amba_class
	: public UC_TLM_bus_class
{
public:
	UC_TLM_bus_amba_class(sc_module_name name, unsigned long long bandwidth,  unsigned int mem_addr = RAM_START, int bus_width = 32);

	~UC_TLM_bus_amba_class();

	
	// R/W
	int read(ADDRESS addr, DATA data, unsigned int size, void *extension);
	int write(ADDRESS addr, DATA data, unsigned int size, void *extension);
	 int send_to_memory(unsigned int size, void *extension);
	void b_transport(tlm_generic_payload &trans, sc_time &delay);///It has a particular implementation in derived classes
	
	void init_statistics();
	double calculate(float P_i, int hburst, int bus_priority, double time_ns, float period);


protected:

	int m_bus_width;
	double time_wait;
	
	double *n_access;
	float *RSizei;
	float *Pi;
	float *PRj;
	float *PFi;

};

#endif
