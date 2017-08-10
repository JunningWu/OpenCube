//==========================================================================
//	uc_master_base.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: vie jul 3 2009
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

#include "hal/uc_master_base.h"
#include "hal/uc_tlm_bus_class.h"

UC_master_base::UC_master_base(sc_module_name name):sc_module(name),m_port("initiator_socket"){
	m_port.bind(*this);
}


/**
 * \brief Function to set the bus access priority of the master
 *
 * @param prio The priority
 */
void UC_master_base::set_bus_priority(int prio) {
	m_bus_priority = prio;
}

/**
 * \brief Function to get the bus access priority of the master
 *
 * @return prio The priority
 */
int UC_master_base::get_bus_priority() {
	return m_bus_priority;
}

/**
 * \brief Binds the bus master with the TLM bus
 *
 * @param bus The bus to connect the master
 */
void UC_master_base::bind(UC_TLM_bus_class * bus){
	UC_tlm_target_socket<> *socket = bus->new_target_port();
	m_port.bind(*socket);
	bus->set_master_priority(socket, m_bus_priority);
}
