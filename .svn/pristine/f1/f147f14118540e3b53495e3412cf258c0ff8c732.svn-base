//==========================================================================
//	uc_address_manager_class.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: lun jul 21 2008
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

#ifndef UC_ADDRESS_MANAGER_CLASS_H
#define UC_ADDRESS_MANAGER_CLASS_H

#include <vector>
#include "../TLM2/include/tlm/tlm.h"
#include "hal/uc_hal_base_if.h"

using namespace std;
using namespace tlm;

/**
 * \brief This class is used to manage the HW addresses
 *
 * Every bus or peripheral has to inherit this class as a standard and automatic
 * way to store and manage the addresses. Each peripheral has a range of addresses
 * which is stored in this class. Then, it can be used to obtain a complete memory map
 * of the peripherals connected to a specific object by calling the generate_memory_map
 * function. Commonly, this feature is used by a bus to create the memory map to know
 * how to route the received packets.
 */
class UC_address_manager_class : public virtual UC_TLM_bus_if {
public:
	enum addr_type {UNKNOWN, REGISTER, PARAMETER, CONTROL};

	/**
	 * \brief Structure to store an address range
	 */
	struct uc_range_t {
		unsigned int begin;			///< First address
		unsigned int end;			///< Last address
		UC_tlm_initiator_socket<> *port;	///< The port associated to this address range
		addr_type type;				///< The type of the range
		unsigned int position;			///< If 'type' == REGISTER, this is the register position in the parameter class
	};

	UC_address_manager_class(vector<UC_tlm_initiator_socket<> *> *ports);
	virtual unsigned int get_num_ranges();
	virtual int get_range(unsigned int range, unsigned int &begin, unsigned int &end);
	virtual int set_range(unsigned int begin, unsigned int end, UC_tlm_initiator_socket<> *port = NULL);
	virtual int set_range(unsigned int begin, unsigned int end, addr_type type, unsigned int initial_position = 0);

	virtual int generate_memory_map();
	virtual UC_tlm_initiator_socket<> *get_port(unsigned int address);
	virtual addr_type get_type(unsigned int address, unsigned int &position);
	virtual unsigned int get_start_address();
	virtual unsigned int get_final_address();

private:
	vector<uc_range_t> m_ranges;			///< The list of ranges
	vector<UC_tlm_initiator_socket<> *> *m_ports;	///< The list of ports to the connected peripherals
	unsigned int m_start_address;	///< The lowest address
	unsigned int m_final_address;	///< The highest address
};

#endif
