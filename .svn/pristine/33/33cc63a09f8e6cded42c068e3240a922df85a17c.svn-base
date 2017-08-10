//==========================================================================
//	uc_simple_noc.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: jue feb 28 2008
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

#ifndef _UC_SIMPLE_NOC_H_
#define _UC_SIMPLE_NOC_H_

#include "hal/scuc_noc_if.h"
#include "hal/scuc_noc_sim_if.h"

#include <vector>

class PRZSimulation;

struct package_sent{
	int x, y, z;
	long long end_time; //delivering time
	void * packet;
};

class UC_Simple_NoC: public SCUC_NoC_Sim_IF{
	SCUC_NoC_IF *m_UC_NoC_If;
	int m_size_x, m_size_y, m_size_z;
	int m_transfer_delay;
	int m_bandwidth;

	std::vector<package_sent*> m_messages;

public:
	UC_Simple_NoC(SCUC_NoC_IF *UC_NoC_If) {
		init_simulation(UC_NoC_If);
		m_bandwidth = (int)50e6;
		m_transfer_delay = 10;
	}

	void paramerize_NoC(int size_x, int size_y, int size_z, int bandwidth, int delay);

	void init_simulation(SCUC_NoC_IF *UC_NoC_If);
	int sim_main_function(int argc, char* argv[], int flit_length = 32);

	bool trysend_package(int x,int y,int z,int x_d,int y_d,int z_d,
							double time,int size,void *packet);
	bool bufferFull(int x,int y,int z);
	int m_messagesInNetwork();
};

SCUC_NoC_Sim_IF *UCSCCreateSimpleNoCSimulator(SCUC_NoC_IF *UC_NoC_If);

#endif
