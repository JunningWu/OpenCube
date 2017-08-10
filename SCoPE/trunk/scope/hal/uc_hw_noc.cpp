//==========================================================================
//	uc_hw_noc.cpp
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

// typedef uc_NoC_bus_if<unsigned int,void *> uc_NoC_bus_if_type;

#include "hal/uc_noc_interface.h"
#include "hal/uc_hw_noc.h"

unsigned char UC_hw_NoC::m_MAC_static[6] = {0, 0, 0, 0, 0, 0};

UC_hw_NoC::UC_hw_NoC(sc_module_name module_name, unsigned int begin, unsigned int end, int irq_num, int ret, UC_NoC_Interface *simulator)
	: UC_hw_if(module_name, begin, end, irq_num)
{
	m_node = 0;
	for (int i = 0; i < 6; i++) {
		m_MAC[i] = m_MAC_static[i];
		m_node += (m_MAC[i] << (8 * (5 - i)));
	}

	for (int i = 5; i >= 0; i++) {
		if (m_MAC_static[i] == 255) {
			m_MAC_static[i] = 0;
		}
		else {
			m_MAC_static[i]++;
			break;
		}
	}
	m_NoC_simulator = NULL;
	if(simulator != NULL){
		set_NoC_simulator(simulator);
	}
}

void UC_hw_NoC::set_NoC_simulator(UC_NoC_Interface *simulator){
	if(m_NoC_simulator!= NULL){
		printf("Error: NoC simulator has already been binded to the network interface\n");
		return;
	}
	m_NoC_simulator = simulator;
	m_NoC_simulator->add_bus_interface(this);
}


int UC_hw_NoC::write(ADDRESS addr, DATA data, unsigned int size) {
	// Simulation of the real data to transfer
	if (addr & 0x2F == 0x2F) {
		return size;
	}

	bool ret;
	struct uc_packet_info* m_info;
	if (size > (int)sizeof(struct uc_packet_info)) {
		 size = sizeof(struct uc_packet_info);
	}

	m_info = (struct uc_packet_info*)malloc(sizeof(struct uc_packet_info));
	memcpy(m_info, data, size);

	void* m_info_data = malloc(m_info->size);
	memcpy(m_info_data, m_info->packet, m_info->size);
	m_info->packet = m_info_data;

	ret = m_NoC_simulator->trysend_package(m_info, m_node);

	if (ret == false) {
		free(m_info);
		return 0;
	}

	return size;
}

int UC_hw_NoC::read(ADDRESS addr, DATA data, unsigned int size) {
	// gets the MAC
	if (addr & 0x3F == 0x3F) {
		memcpy(data, m_MAC, 6);
		return 6;
	}

	// Simulation of the real data to transfer
	if (addr & 0x2F == 0x2F) {
		return size;
	}

	bool ret;
	struct uc_packet_info* m_info = new struct uc_packet_info;
	ret = m_NoC_simulator->trywait_package(&m_info, m_node);
	if (ret == false) {
		return 0;
	}

	memcpy(data, m_info, size);
	delete(m_info);
	return size;
}

int UC_hw_NoC::get_node() {
	return m_node;
}

int UC_hw_NoC::get_num_nodes() {
	int num_nodes = 0;
	for (int i = 0; i < 6; i++) {
		num_nodes += (m_MAC_static[i] << (8 * (6 - i)));
	}
	return num_nodes;
}
