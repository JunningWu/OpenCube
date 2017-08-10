//==========================================================================
//	uc_address_manager_class.cpp
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

#include "hal/uc_address_manager_class.h"
#include "hal/uc_tlm_bus_sockets.h"

/**
 * \brief Constructor
 *
 * \param port ports connected to HW devices
 */
UC_address_manager_class::UC_address_manager_class(vector<UC_tlm_initiator_socket<> *> *ports)
{
	m_ports = ports;
	m_start_address = -1;
	m_final_address = 0;
}

/**
 * \brief Gets the number of addresses ranges
 *
 * \return the number of ranges
 */
unsigned int UC_address_manager_class::get_num_ranges()
{
	return m_ranges.size();
}

/**
 * \brief Gets a specified range number
 *
 * The first range is the number 0
 * \param range the number of the range to return
 * \param begin return parameter with the initial address
 * \param end return parameter with the final address
 * \return 0 if success, -1 if faliure
 */
int UC_address_manager_class::get_range(unsigned int range, unsigned int &begin, unsigned int &end)
{
	if (m_ranges.size() == 0) {
		generate_memory_map();
	}

	if (range >= m_ranges.size()) {
		return -1;
	}

	begin = m_ranges[range].begin;
	end = m_ranges[range].end;
	return 0;
}

/**
 * \brief Creates a new address range
 *
 * \param begin the initial address
 * \param end the final address
 * \param port the port connected to this address range.
 * This parameter is only for buses and modules that re-sends data, not need for most cases
 * \return 0 if success, -1 if faliure
 */
int UC_address_manager_class::set_range(unsigned int begin, unsigned int end, UC_tlm_initiator_socket<> *port)
{
	// NOTE Se puede revisar si está en la lista o si se solapa con otros rangos
	uc_range_t range;
	range.begin = begin;
	range.end = end;
	range.port = port;
	range.type = UNKNOWN;
	range.position = 0;
	m_ranges.push_back(range);

	if (begin < m_start_address) {
		m_start_address = begin;
	}

	if (end > m_final_address) {
		m_final_address = end;
	}

	return 0;
}

/**
 * \brief Creates a new address range
 *
 * Some modules need to distinguish among different address types: REGISTER, PARAMETER, CONTROL.
 * It is not for common modules. If you do not know what is that, your function is the other 'set_range'
 * \param begin the initial address
 * \param end the final address
 * \param type the type of the range
 * \param initial_position the initial position of the resgister in the parameter class.
 * If you define a range, the individual position will be calculated
 * \return 0 if success, -1 if faliure
 */
int UC_address_manager_class::set_range(unsigned int begin, unsigned int end, addr_type type, unsigned int initial_position)
{
	uc_range_t range;
	range.begin = begin;
	range.end = end;
	range.port = NULL;
	range.type = type;
	range.position = initial_position;
	m_ranges.push_back(range);

	if (begin < m_start_address) {
		m_start_address = begin;
	}

	if (end > m_final_address) {
		m_final_address = end;
	}

	return 0;
}


/**
 * \brief Generates a memory map of the connected objects
 *
 * \return 0 if success, -1 if faliure
 */
int UC_address_manager_class::generate_memory_map()
{
	unsigned int begin, end;
	int range;
	for (int i = 0; i < m_ports->size(); i++) {
		range = 0;
		while ((*(*m_ports)[i])->get_range(range, begin, end) == 0) {
			set_range(begin, end, (*m_ports)[i]);
			range++;
		}
	}
	return 0;
}

/**
 * \brief Gets the out port corresponding to an address
 *
 * This is used to find the route to send a message when it must be redirected
 * to other hardware
 * \param address the address to send the message
 * \return the pointer to the correspondent port
 */
UC_tlm_initiator_socket<> *UC_address_manager_class::get_port(unsigned int address)
{
	for (int i = 0; i < m_ranges.size(); i++) {
		if (m_ranges[i].begin <= address && m_ranges[i].end >= address) {
			return m_ranges[i].port;
		}
	}
}

/**
 * \brief Gets the type corresponding to an address
 *
 * This is used for special hardware that has a type associated to each address: REGISTER, PARAMETER, CONTROL
 * \param address the address to send the message
 * \param position returns the position in the parameter class of a register element
 * \return the address type
 */
UC_address_manager_class::addr_type UC_address_manager_class::get_type(unsigned int address, unsigned int &position)
{
	for (int i = 0; i < m_ranges.size(); i++) {
		if (m_ranges[i].begin <= address && m_ranges[i].end >= address) {
			position = m_ranges[i].position + address - m_ranges[i].begin;
			return m_ranges[i].type;
		}
	}
}


/**
 * \brief Gets the start address
 *
 * \return the lowest address stored
 */
unsigned int UC_address_manager_class::get_start_address() {
	return m_start_address;
}

/**
 * \brief Gets the final address
 *
 * \return the highest address stored
 */
unsigned int UC_address_manager_class::get_final_address() {
	return m_final_address;
}
