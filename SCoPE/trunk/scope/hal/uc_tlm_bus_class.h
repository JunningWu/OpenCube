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

#ifndef UC_TLM_BUS_CLASS_H
#define UC_TLM_BUS_CLASS_H

#include <vector>
#include "systemc.h"
#include "../TLM2/include/tlm/tlm.h"
using namespace tlm;
using namespace std;

#include "hal/uc_memory_map.h"
#include "hal/uc_address_manager_class.h"
#include "hal/uc_bus_data.h"
#include "rtos/kernel/uc_gui_statistics.h"
#include "hal/uc_tlm_bus_sockets.h"
#include "hal/uc_tlm_statistics.h"
#include "hal/uc_hw_if.h"
class UC_hw_if_base;

#define NUM_IRQS 16

class UC_arbiter_class;

class UC_TLM_bus_class
	: public sc_module
	, public UC_address_manager_class //	, public UC_TLM_bus_if, inside address manager
	, public UC_tlm_statistics
	, public virtual UC_cpu_if

{
public:
	UC_TLM_bus_class(sc_module_name name, unsigned long long bandwidth,  unsigned int mem_addr = RAM_START);

	~UC_TLM_bus_class();

	// Conexion
	virtual UC_tlm_initiator_socket<> * new_initiator_port();
	virtual UC_tlm_target_socket<>    * new_target_port();
	virtual void bind(UC_hw_if_b *hw);
	virtual void bind(UC_TLM_bus_class *bus);
	virtual void set_memory_address(unsigned int addr);
	virtual void set_arbiter(UC_arbiter_class *arbiter);
	virtual void set_master_priority(UC_tlm_target_socket<> *socket, int priority){}

	// IRQ
	virtual void send_irq(int irq);
	virtual void configure_irq_port(int irq, int port);

	// R/W
	virtual int read(ADDRESS addr, DATA data, unsigned int size);
	virtual int write(ADDRESS addr, DATA data, unsigned int size);
	virtual int send_to_memory(unsigned int size);
	virtual void b_transport(tlm_generic_payload &trans, sc_time &delay);
	virtual void current_master(UC_tlm_target_socket<> *socket);

	// Debug
	virtual bus_statistics get_statistics();  ///< generate data for the gui 
	virtual void set_debug(bool cond);

	virtual UC_tlm_target_socket<> * get_target_port(){return m_tlm_target_sockets[0];};
	
protected:
	UC_arbiter_class *m_arbiter;	///< The bus arbiter

	// Ports
	vector<UC_tlm_initiator_socket<> *> m_tlm_initiator_sockets;	///< Initiator sockets list
	vector<UC_tlm_target_socket<>    *> m_tlm_target_sockets;		///< Target sockets list

	unsigned long long m_bandwidth;		///< Bus bandwidth
	unsigned int m_mem_addr;		///< Base memory address to use send_to_memory

	/**
	 * \brief Links the irq numbers with the correspondence port
	 *
	 * If value >=  0 then the irq is sent through this port
	 * If value == -1 then the irq is sent through all ports
	 */
	int m_irq_port_table[NUM_IRQS];

	long long m_sent_bytes;	///< Statistical variable that counts the number of bytes sent through the bus (only read/write, no transport)

	bool m_debug;	///< Debug mode

	/**
	 * \brief This is the master of the last bus request
	 * \warning This variable is very volatile. It is only useful just after the request.
	 * If SystemC preempts the thread, this variable is not reliable.
	 */
	int m_current_master;
};

#endif
