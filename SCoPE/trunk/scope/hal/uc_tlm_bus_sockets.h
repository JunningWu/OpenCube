//==========================================================================
//	uc_tlm_bus_sockets.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mar sep 2 2008
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

#ifndef UC_TLM_BUS_SOCKETS_H
#define UC_TLM_BUS_SOCKETS_H

#include "../TLM2/include/tlm/tlm.h"
using namespace tlm;

#include "hal/uc_hal_base_if.h"

/**
 * \brief Initiator port for the bus
 */
template 	<unsigned int BUSWIDTH
			, typename TYPES
			, int N
			, sc_core::sc_port_policy POL
			>
class UC_tlm_initiator_socket
	: public tlm_base_initiator_socket	<BUSWIDTH
										, UC_TLM_bus_if
										, UC_cpu_if
										, N
										, POL
										>
{
public:
	UC_tlm_initiator_socket()
		: tlm_base_initiator_socket	<BUSWIDTH
										, UC_TLM_bus_if
										, UC_cpu_if
										, N
										, POL
										>()
	{
	}

	explicit UC_tlm_initiator_socket(const char* name)
		: tlm_base_initiator_socket	<BUSWIDTH
										, UC_TLM_bus_if
										, UC_cpu_if
										, N
										, POL
										>(name)
	{
	}

	/**
	 * \brief bind overload
	 * Save the target port and call to the parent function.
	 * This is done to be able to send the number of the initiator to the bus.
	 * With this information, the bus can sched the requests appropriately.
	 */
	void bind(UC_tlm_target_socket<>& s) {
		m_s = &s;
		tlm_base_initiator_socket<BUSWIDTH, UC_TLM_bus_if, UC_cpu_if, N, POL>::bind(s);
	}

	void operator() (UC_tlm_target_socket<>& s) {
		bind(s);
	}

	// Parent bind functions has to be overloaded, otherwise these functions are lost
	typedef tlm_base_target_socket_b<BUSWIDTH, UC_TLM_bus_if, UC_cpu_if> base_target_socket_type;
	typedef tlm_base_initiator_socket_b<BUSWIDTH, UC_TLM_bus_if, UC_cpu_if> base_type;
	typedef UC_cpu_if bw_interface_type;

	void bind(base_target_socket_type& s){
		tlm_base_initiator_socket<BUSWIDTH, UC_TLM_bus_if, UC_cpu_if, N, POL>::bind(s);
	}
	void operator() (base_target_socket_type& s){
		tlm_base_initiator_socket<BUSWIDTH, UC_TLM_bus_if, UC_cpu_if, N, POL>::operator ()(s);
	}
	void bind(base_type& s){
		tlm_base_initiator_socket<BUSWIDTH, UC_TLM_bus_if, UC_cpu_if, N, POL>::bind(s);
	}
	void operator() (base_type& s){
		tlm_base_initiator_socket<BUSWIDTH, UC_TLM_bus_if, UC_cpu_if, N, POL>::operator ()(s);
	}
	void bind(bw_interface_type& s){
		tlm_base_initiator_socket<BUSWIDTH, UC_TLM_bus_if, UC_cpu_if, N, POL>::bind(s);
	}
	void operator() (bw_interface_type& s){
		tlm_base_initiator_socket<BUSWIDTH, UC_TLM_bus_if, UC_cpu_if, N, POL>::operator ()(s);
	}

	/**
	 * \brief -> overload
	 * This function return the fw interface as its parent. Moreover, it set the current master in the bus.
	 * That is, the bus can know the origin of the current request by the m_current_master variable.
	 */
	UC_TLM_bus_if * operator ->(){
		UC_TLM_bus_if * interface = tlm_base_initiator_socket<BUSWIDTH, UC_TLM_bus_if, UC_cpu_if, N, POL>::operator ->();
		interface->current_master(m_s);
		return interface;
	}

private:
	UC_tlm_target_socket<> *m_s;
};

/**
 * \brief Target port for the bus
 */
template	<unsigned int BUSWIDTH
			, typename TYPES
			, int N
			, sc_core::sc_port_policy POL
			>
class UC_tlm_target_socket
	: public tlm_base_target_socket	<BUSWIDTH
									, UC_TLM_bus_if
									, UC_cpu_if
									, N
									, POL
									>
{
public:
	UC_tlm_target_socket()
		: tlm_base_target_socket	<BUSWIDTH
									, UC_TLM_bus_if
									, UC_cpu_if
									, N
									, POL
									>()
	{
	}

	explicit UC_tlm_target_socket(const char* name)
		: tlm_base_target_socket	<BUSWIDTH
									, UC_TLM_bus_if
									, UC_cpu_if
									, N
									, POL
									>(name)
	{
	}
};

#endif
