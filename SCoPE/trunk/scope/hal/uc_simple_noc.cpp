//==========================================================================
//	uc_simple_noc.cpp
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

#include "hal/uc_simple_noc.h"
#include <stdlib.h>

void UC_Simple_NoC::paramerize_NoC(int size_x, int size_y, int size_z, int bandwidth, int delay){
	m_size_x = size_x;
	m_size_y = size_y;
	m_size_z = size_z;
	m_bandwidth = (int)1e8;
	m_transfer_delay = 10;
}

void UC_Simple_NoC::init_simulation(SCUC_NoC_IF *UC_NoC_If){
	m_UC_NoC_If = UC_NoC_If;
}

int UC_Simple_NoC::sim_main_function(int argc, char* argv[], int flit_length){
	long long clock,timeout;

	while( true )
	{
		if (m_messages.size() > 0 ) {
			timeout = m_messages[0]->end_time;
		}
		else {
			timeout = -1;
		}

		if (!m_UC_NoC_If->sim_waitForMessage(clock, timeout)) {
			return false;
		}

		while ( m_messages.size() > 0 ) {
			if (clock >= m_messages[0]->end_time) {
				m_UC_NoC_If->sim_messageReceived(m_messages[0]->x, m_messages[0]->y,
					m_messages[0]->z, m_messages[0]->packet);
				m_messages.erase(m_messages.begin());
			}
			else {
				break;
			}
		}

	}
	return true;
}

#define ABS(x) ( x>0 ? x : (x)*-1)

bool UC_Simple_NoC::trysend_package(int x,int y,int z,int x_d,int y_d,int z_d,
									double time,int size,void *packet){
	struct package_sent *str_pck;
	int i;

	str_pck = (struct package_sent*) malloc(sizeof(struct package_sent));
	str_pck->x = x_d;
	str_pck->y = y_d;
	str_pck->z = z_d;
	str_pck->packet = packet;
	str_pck->end_time = ABS(x_d - x) + ABS(y_d - y) + ABS(z_d - z);

	str_pck->end_time*=(long long)(((float)size)/((float)m_bandwidth/(int)1e9));
	str_pck->end_time+=(long long)time;

	for (i = 0; i < m_messages.size(); i++) {
		if (m_messages[i]->end_time > str_pck->end_time) {
			break;
		}
	}
	m_messages.insert(m_messages.begin() + i, str_pck);
	return true;
}

bool UC_Simple_NoC::bufferFull(int x,int y,int z){
	return false;
}

int UC_Simple_NoC::m_messagesInNetwork(){
	m_messages.size();
}

SCUC_NoC_Sim_IF *UCSCCreateSimpleNoCSimulator(SCUC_NoC_IF* noc_if){
	return new UC_Simple_NoC(noc_if);
}
