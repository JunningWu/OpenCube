//==========================================================================
//	uc_hw_if.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mié jul 23 2008
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

#include "systemc.h"
#include "hal/uc_hw_if.h"



/**
 * \brief Constructor
 *
 * \param module_name the module name
 * \param begin the initial HW address
 * \param end the final HW address
 * \param irq_num the irq number
 */
UC_hw_if::UC_hw_if(sc_module_name module_name, unsigned int begin, unsigned int end, int irq_num)
	: UC_hw_if_b(module_name), UC_address_manager_class(NULL), m_target_socket("target_socket")
{
	m_irq_num = irq_num;

	m_target_socket.bind(*this);

	m_burst_size = 1;

	set_range(begin, end);
}

/**
 * \brief Constructor
 *
 * It is required to set the address ranges with the function 'set_range'
 * \param module_name the module name
 * \param irq_num the irq number
 */
UC_hw_if::UC_hw_if(sc_module_name module_name, int irq_num)
	: UC_hw_if_b(module_name), UC_address_manager_class(NULL), m_target_socket("target_socket")
{
	m_irq_num = irq_num;

	m_target_socket.bind(*this);
}

/**
 * \brief Sends irq to the initiator through the bus
 */
void UC_hw_if::send_irq() {
	if (m_irq_num != -1) {
		m_target_socket->send_irq(m_irq_num);
	}
}

/**
 * \brief Sends irq to the initiator through the bus
 *
 * \param id irq number
 */
void UC_hw_if::send_irq(int id) {
	if (id != -1) {
		m_target_socket->send_irq(id);
	}
}

/**
 * \brief Receives a transaction from the bus and transform the interface to read/write
 *
 * \param trans The transaction packet
 * \param delay Transaction delay (actually ignored)
 */
void UC_hw_if::b_transport(tlm_generic_payload &trans, sc_time &delay) {
	unsigned int addr = trans.get_address();
	DATA data = trans.get_data_ptr();
	unsigned int size = trans.get_data_length();
	int bytes;

	if (trans.is_read() == true) {
		bytes = this->read(addr, data, size);
		add_read_access (bytes);
	}
	else if (trans.is_write() == true) {
		bytes = this->write(addr, data, size);
		add_write_access (bytes);
	}
	else {
		cout << "HW error: TLM command unrecognized" << endl;
		bytes = -3;
	}

	switch (bytes) {
		case 0:
			trans.set_response_status(TLM_INCOMPLETE_RESPONSE);
			break;
		case -1:
			trans.set_response_status(TLM_GENERIC_ERROR_RESPONSE);
			break;
		case -2:
			trans.set_response_status(TLM_ADDRESS_ERROR_RESPONSE);
			break;
		case -3:
			trans.set_response_status(TLM_COMMAND_ERROR_RESPONSE);
			break;
		default:
			trans.set_response_status(TLM_OK_RESPONSE);
			break;
	}
}


/**
 * \brief Write function to be reimplemented by the HW
 *
 * \param addr The transaction address
 * \param data The transaction data
 * \param size The transaction size
 * \param extension Unused. For compatibility reasons
 * \return The number of bytes written
 */
int UC_hw_if::write(ADDRESS addr, DATA data, unsigned int size, void* extension) {
	printf("Warning: UC_hw_if_b_base::write function not overloaded for this component\n"); 
	return -1;
}

/**
 * \brief Read function to be reimplemented by the HW
 *
 * \param addr The transaction address
 * \param data The transaction data
 * \param size The transaction size
 * \param extension Unused. For compatibility reasons
 * \return The number of bytes read
 */
int UC_hw_if::read(ADDRESS addr, DATA data, unsigned int size, void* extension) {
	printf("Warning: UC_hw_if_b_base::read function not overloaded for this component\n"); 
	return -1;
}


/**
 * \brief Write function to be reimplemented by the HW
 * Should not be used. Included for compatibility reasons with previous versions
 *
 * \param addr The transaction address
 * \param data The transaction data
 * \param size The transaction size
 * \return The number of bytes written
 */
int UC_hw_if::write(ADDRESS addr, DATA data, unsigned int size) {
	return this->write(addr, data, size, NULL);
}

/**
 * \brief Read function to be reimplemented by the HW
 * Should not be used. Included for compatibility reasons with previous versions
 *
 * \param addr The transaction address
 * \param data The transaction data
 * \param size The transaction size
 * \return The number of bytes read
 */
int UC_hw_if::read(ADDRESS addr, DATA data, unsigned int size) {
	return this->read(addr, data, size, NULL);
}

/**
 * \brief Gets the target socket
 *
 * \return The target socket
 */
UC_tlm_target_socket<> * UC_hw_if::get_target_port() {
	return &m_target_socket;
}

/**
 * \brief Gets the burst size corresponding to an address
 *
 * This is used for special hardware that has a burst size associated
 * \return the burst size
 */
int UC_hw_if::get_burst_size(){
	return m_burst_size;
}

/**
 * \brief Assign the burst size associated to the peripheral
 *
 * \param burst_size the maximum burst size accepted by the module.
 */
void UC_hw_if::set_burst_size(int burst_size)
{
	m_burst_size = burst_size;
}

