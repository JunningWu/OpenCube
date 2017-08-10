//==========================================================================
//	uc_HAL_base_if.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: vie nov 23 2007
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

#ifndef UC_HAL_BASE_IF_H
#define UC_HAL_BASE_IF_H

#include "../TLM2/include/tlm/tlm.h"
using namespace tlm;
#include "systemc.h"
#include "hal/uc_bus_data.h"

// #include "uc_tlm_bus_sockets.h"

template <unsigned int BUSWIDTH = 32
			, typename TYPES = tlm_base_protocol_types
			, int N = 1
			, sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND
			>
class UC_tlm_initiator_socket;

template <unsigned int BUSWIDTH = 32
			, typename TYPES = tlm_base_protocol_types
			, int N = 1
			, sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND
			>
class UC_tlm_target_socket;

/**
 * \brief Additional information added to bus transfers
 */
class uc_transfer_extension_t 
	: public tlm_extension<uc_transfer_extension_t>
{
public:
	unsigned long long ts; 	// SCoPE estimated time when the transfer should start
	unsigned int width;	// Word width for this transfer
	int hburst;		// Burst size
	double ttx;		// Transmission time expected
//	double ttx;

	uc_transfer_extension_t():ts(0),width(32),ttx(0){}

	tlm::tlm_extension_base* clone() const {
		uc_transfer_extension_t* new_extension = new uc_transfer_extension_t();
		*new_extension = *this;
		return new_extension;
	}
	void copy_from(const tlm::tlm_extension_base& data){
		*this = *((uc_transfer_extension_t*)&data);
	}
} ;


/**
 * \brief Functions that can be called by HAL to access a master component
 */
class UC_cpu_if : public virtual sc_interface
{
public:
	virtual void send_irq(int irq) {};
	virtual int get_bus_priority(){};
};

/**
 * \brief HAL's functions
 *
 * Internal HAL's functions and configuration functions
 */
class UC_HAL_base_hw_if
	: public virtual sc_interface
	//, public virtual UC_cpu_if
{
public:
	// Conexion
	virtual UC_tlm_initiator_socket<> * new_initiator_port() {return NULL;};
	virtual UC_tlm_target_socket<>    * new_target_port() {return NULL;};

	//IRQ
	virtual void configure_irq_port(int irq, int port) {};
};

/**
 * \brief Gets range of addresses from HWs
 */
class UC_address_if : public virtual sc_interface {
public:
	virtual int get_range(unsigned int range, unsigned int &begin, unsigned int &end) = 0;
};


class UC_hw_if_base
	: public virtual UC_HAL_base_hw_if
	, public virtual tlm_blocking_transport_if<>
{
public:

	// Receive request
	virtual void b_transport(tlm_generic_payload &trans, sc_time &delay)=0;

	// Send Interruption
//	virtual void send_irq()=0;
	virtual void send_irq(int id)=0;
	// Burst size control
	virtual void set_burst_size( int burst_size )=0;
	virtual int get_burst_size()=0;

	virtual UC_tlm_target_socket<> * get_target_port()=0;

	// Specific peripheral functions (DATA = void *, ADRESS = unsigned int)
	virtual int read(ADDRESS addr, DATA data, unsigned int size, void *extension)=0;
	virtual int write(ADDRESS addr, DATA data, unsigned int size, void *extension)=0;
	virtual int read(ADDRESS addr, DATA data, unsigned int size)=0;
	virtual int write(ADDRESS addr, DATA data, unsigned int size)=0;



};

/**
 * \brief Functions that can be called by the software to be executed by the HAL
 */
class UC_HAL_base_sw_if
	: public virtual UC_hw_if_base
{
public:
	virtual int read(ADDRESS addr, DATA data, unsigned int size, void *extension) {
		return this->read(addr,data,size);
	};
	virtual int write(ADDRESS addr, DATA data, unsigned int size, void *extension) {
		return this->write(addr,data,size);
	};
	virtual int send_to_memory(unsigned int size, void *extension) {
		return this->send_to_memory(size);
	};

	virtual int read(ADDRESS addr, DATA data, unsigned int size) {
		printf("Warning: UC_hw_if_base::read function not overloaded for this component\n"); 
		return 0;
	};
	virtual int write(ADDRESS addr, DATA data, unsigned int size) {
		printf("Warning: UC_hw_if_base::write function not overloaded for this component\n"); 
		return 0;
	};
	virtual int send_to_memory(unsigned int size) {
		printf("Warning: UC_hw_if_base::send_to_memory function not overloaded for this component\n"); 
		return 0;
	};
};

/**
 * \brief Forward bus interface
 */
class UC_TLM_bus_if
	: public virtual UC_HAL_base_sw_if
	, /*public virtual UC_HAL_base_hw_if
	, */public virtual UC_address_if
{
public:
	virtual void current_master(UC_tlm_target_socket<> *socket) {};
	virtual int get_burst_size(){printf("Error: get_burst_size not implemented\n");};

	virtual void set_burst_size(int data){printf("Error: set_burst_size not implemented\n");};

	virtual UC_tlm_target_socket<> * get_target_port(){printf("Error: get_target_port not implemented\n");};
	virtual ~UC_TLM_bus_if(){};

};

#endif
