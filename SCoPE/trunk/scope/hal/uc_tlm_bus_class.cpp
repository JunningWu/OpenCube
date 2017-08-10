//==========================================================================
//	uc_tlm_bus.cpp
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

#include "hal/uc_tlm_bus_class.h"
#include "hal/uc_arbiter_class.h"
#include "hal/uc_hw_if.h"

/**
 * \brief Constructor
 *
 * \param name The module name
 * \param bandwidth The bus bandwidth
 * \param mem_addr The memory address of the main memory
 */
UC_TLM_bus_class::UC_TLM_bus_class(sc_module_name name, unsigned long long bandwidth,  unsigned int mem_addr)
	: sc_module(name)
	, UC_address_manager_class(&m_tlm_initiator_sockets)
{
	m_bandwidth = bandwidth;
	m_mem_addr = mem_addr;

	m_arbiter = new UC_arbiter_class();

	for (int i = 0; i < NUM_IRQS; i++) {
		m_irq_port_table[i] = 0;
	}

	m_sent_bytes = 0;

	m_debug = false;

	m_current_master = -1;
}

UC_TLM_bus_class::~UC_TLM_bus_class()
{
}

/**
 * \brief Creates a new initiator port
 *
 * \return The pointer to the new port
 */
UC_tlm_initiator_socket<> * UC_TLM_bus_class::new_initiator_port()
{
	// As we are out of the constructor, we can not add more components
	// to the module. SystemC returns an error.
	// To solve this, we have to insert the module into the simcontext hierarchy.
	sc_get_curr_simcontext()->hierarchy_push(this);

	// Now, we can add new components
	UC_tlm_initiator_socket<> *new_port = new UC_tlm_initiator_socket<>();
	new_port->bind(*this);
	m_tlm_initiator_sockets.push_back(new_port);

	// We undo the simcontext change
	sc_get_curr_simcontext()->hierarchy_pop();
	return new_port;
}

/**
 * \brief Creates a new target port
 *
 * \return The pointer to the new port
 */
UC_tlm_target_socket<> * UC_TLM_bus_class::new_target_port()
{
	// As we are out of the constructor, we can not add more components
	// to the module. SystemC returns an error.
	// To solve this, we have to insert the module into the simcontext hierarchy.
	sc_get_curr_simcontext()->hierarchy_push(this);

	// Now, we can add new components
	UC_tlm_target_socket<> *new_port = new UC_tlm_target_socket<>();
	new_port->bind(*this);
	m_tlm_target_sockets.push_back(new_port);

	// We undo the simcontext change
	sc_get_curr_simcontext()->hierarchy_pop();
	return new_port;
}

/**
 * \brief Connects the bus with a HW
 *
 * \param hw HW to be connected
 */
void UC_TLM_bus_class::bind(UC_hw_if_b *hw)
{
	new_initiator_port()->bind(*hw->get_target_port());
}

/**
 * \brief Connects the bus with other one
 *
 * \param hw HW to be connected
 */
void UC_TLM_bus_class::bind(UC_TLM_bus_class *bus)
{
	new_initiator_port()->bind(*bus->new_target_port());
}

/**
 * \brief Sets the memory address for send_to_memory accesses
 *
 * \param addr The memory address
 */
void UC_TLM_bus_class::set_memory_address(unsigned int addr)
{
	m_mem_addr = addr;
}

/**
 * \brief Sets a new bus arbiter
 * The bus has an arbiter by default. With this function the default arbiter
 * is deleted and replaced by the new one
 *
 * \param arbiter The new bus arbiter
 */
void UC_TLM_bus_class::set_arbiter(UC_arbiter_class * arbiter)
{
	delete m_arbiter;
	m_arbiter = arbiter;
}

/**
 * \brief Sends the interruption signal
 *
 * \param irq The irq number to send
 */
void UC_TLM_bus_class::send_irq(int irq)
{
	if (irq < 1 || irq > NUM_IRQS) {
		(*m_tlm_target_sockets[0])->send_irq(irq);
		return;
	}

	int port_number = m_irq_port_table[irq];
	if (port_number == -1) {
		for (int i = 0; i < m_tlm_target_sockets.size(); i++) {
			(*m_tlm_target_sockets[i])->send_irq(irq);
		}
	}
	else {
		(*m_tlm_target_sockets[port_number])->send_irq(irq);
	}
}

/**
 * \brief Configures the interruption controller
 *
 * Links an irq line with the port to send the irq through
 * \param irq The interruption number
 * \param port The port number
 */
void UC_TLM_bus_class::configure_irq_port(int irq, int port)
{
	// irq 0 is used by the Tic Timer
	if (irq < 1 || irq > NUM_IRQS) {
		cout << "Error: configure_irq_port failed, irq " << irq << " out of bounds" << endl;
		return;
	}

	m_irq_port_table[irq] = port;
}

/**
 * \brief Reads data from the desired address
 *
 * \param addr Address to read from
 * \param data Buffer to write the desired data
 * \param size Number of bytes to read
 * \return Number of read bytes
 */
int UC_TLM_bus_class::read(ADDRESS addr, DATA data, unsigned int size)
{
	// Creates the TLM packet
	tlm_generic_payload trans;
	trans.set_command(TLM_READ_COMMAND);
	trans.set_address(addr);
	trans.set_data_ptr((unsigned char*)data);
	trans.set_data_length(size);
	trans.set_response_status(TLM_INCOMPLETE_RESPONSE);

	sc_time delay = SC_ZERO_TIME;

	// Sends the packet
	b_transport(trans, delay);

	// Updates the bytes sent
	// FIXME: the address is not a data, so it is not added to the amount of information sent
	// m_sent_bytes += sizeof(addr);
	if (trans.is_response_ok() == true) {
		m_sent_bytes += size;
		return size;
	}
	else {
		return 0;
	}
}

/**
 * \brief Writes data to the desired address
 *
 * \param addr Address to write to
 * \param data Buffer that contains the data to copy
 * \param size Number of bytes to write
 * \return Number of written bytes
 */
int UC_TLM_bus_class::write(ADDRESS addr, DATA data, unsigned int size)
{
	// Creates the TLM packet
	tlm_generic_payload trans;
	trans.set_command(TLM_WRITE_COMMAND);
	trans.set_address(addr);
	trans.set_data_ptr((unsigned char*)data);
	trans.set_data_length(size);
	trans.set_response_status(TLM_INCOMPLETE_RESPONSE);

	sc_time delay = SC_ZERO_TIME;

	// Sends the packet
	b_transport(trans, delay);

	// FIXME: the address is not a data, so it is not added to the amount of information sent
	// m_sent_bytes += sizeof(addr);
	if (trans.is_response_ok() == true) {
		m_sent_bytes += size;
		return size;
	}
	else {
		return 0;
	}
}

/**
 * \brief Sends data to the main memory
 * It sends data to the default address if it is not changed in the constructor.\n
 * The main use of this function is to generate traffic with the memory
 * for simulating cache transactions.
 *
 * \param size Number of bytes to send
 * \return Number of sent bytes
 */
int UC_TLM_bus_class::send_to_memory(unsigned int size)
{
	assert(m_mem_addr != -1);

	int ret = 0;

	void *foo = malloc(size);
	ret = read(m_mem_addr, foo, size);

	free(foo);
	return ret;
}

/**
 * \brief Blocking transport function
 * This is the TLM2 function to perform data transferences.\n
 * It asks the arbiter for using the bus and then sends the transport
 * to the next HW module. When this returns, the transport tells the
 * arbiter that is ready to release the bus.
 *
 * \param trans The generic TLM2 data packet
 * \param delay A delay time to initiate the transference. No use for this bus
 */
void UC_TLM_bus_class::b_transport(tlm_generic_payload & trans, sc_time & delay)
{
	// NOTE get m_current_master before other transaction starts (or SystemC preemption)
	int current_master = m_current_master;
	double time = trans.get_data_length();
	// To nanoseconds
	time *= 1e9;
	time /= m_bandwidth;

	UC_tlm_initiator_socket<> *port = get_port(trans.get_address());
	if (port == NULL) {
		return;
	}

	if (trans.is_read() == true) {
		// get the bus access
		//m_arbiter->request(SC_ZERO_TIME, current_master);

		// past the packet to the next module
		(*port)->b_transport(trans, delay);

		if (trans.is_response_ok() == true) {
			// release the bus
			//m_arbiter->release(sc_time(time, SC_NS), current_master);
		}
		else {
			//m_arbiter->release(SC_ZERO_TIME, current_master);
		}

		// add transfer statistics
		add_read_access (trans.get_data_length());
	}
	else {
		// write request

		// get the bus access
		//m_arbiter->request(sc_time(time, SC_NS), current_master);

		// past the packet to the next module
		(*port)->b_transport(trans, delay);

		// release the bus
		//m_arbiter->release(SC_ZERO_TIME, current_master);

		// add transfer statistics
		add_write_access (trans.get_data_length());
	}
}

/**
 * \brief Set the current master
 * The target socket updates the current master variable each time a read, write,
 * send_to_memory or b_transport request arrives. This is done to know the master
 * that did the request and be able to sched the request appropriately.
 *
 * \param socket The pointer to the socket that is using the bus
 */
void UC_TLM_bus_class::current_master(UC_tlm_target_socket<> *socket) {
	for (int i = 0; i < m_tlm_target_sockets.size(); i++) {
		if (m_tlm_target_sockets[i] == socket) {
			m_current_master = i;
			break;
		}
	}
}

/**
 * \brief Gets statistics of the bus for the gui
 *
 * \return A packet with the statistics
 */
bus_statistics UC_TLM_bus_class::get_statistics()
{
	bus_statistics stats;
	stats.bytes = m_sent_bytes;
	return stats;
}

/**
 * \brief Sets the debug mode ON/OFF
 *
 * \param cond The debug mode
 */
void UC_TLM_bus_class::set_debug(bool cond)
{
	// NOTE actually useless
	m_debug = cond;
}
