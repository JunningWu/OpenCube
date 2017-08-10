//==========================================================================
//	uc_hw_if.h
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

#ifndef UC_HW_IF_H_
#define UC_HW_IF_H_

#include "systemc.h"
#include "../TLM2/include/tlm/tlm.h"
#include "hal/uc_bus_data.h"
#include "hal/uc_tlm_bus_sockets.h"
#include "hal/uc_hal_base_if.h"
#include "hal/uc_address_manager_class.h"
#include "hal/uc_tlm_statistics.h"


class UC_hw_if_b
	: public UC_tlm_statistics
//	, public UC_hw_if_base //it is inside address_manager_class
{
public:
	UC_hw_if_b(sc_module_name module_name){;}

	// Receive request
	virtual void b_transport(tlm_generic_payload &trans, sc_time &delay)=0;

	// Send Interruption
	virtual void send_irq()=0;
	virtual void send_irq(int id)=0;

	virtual UC_tlm_target_socket<> * get_target_port()=0;

	// Specific peripheral functions (DATA = void *, ADRESS = unsigned int)
	virtual int read(ADDRESS addr, DATA data, unsigned int size, void *extension)=0;
	virtual int write(ADDRESS addr, DATA data, unsigned int size, void *extension)=0;
	virtual int read(ADDRESS addr, DATA data, unsigned int size)=0;
	virtual int write(ADDRESS addr, DATA data, unsigned int size)=0;

	// Burst size control
	virtual void set_burst_size( int burst_size )=0;
	virtual int get_burst_size()=0;

};



class UC_hw_if:
	public sc_module, 
	public UC_hw_if_b,
	public UC_address_manager_class

{
public:
	UC_hw_if(sc_module_name module_name, unsigned int begin, unsigned int end, int irq_num = -1);

	UC_hw_if(sc_module_name module_name, int irq_num = -1);

	// Receive request
	virtual void b_transport(tlm_generic_payload &trans, sc_time &delay);

	// Send Interruption
	virtual void send_irq();
	virtual void send_irq(int id);

	virtual UC_tlm_target_socket<> * get_target_port();

	// Specific peripheral functions (DATA = void *, ADRESS = unsigned int)
	virtual int read(ADDRESS addr, DATA data, unsigned int size, void *extension);
	virtual int write(ADDRESS addr, DATA data, unsigned int size, void *extension);
	virtual int read(ADDRESS addr, DATA data, unsigned int size);
	virtual int write(ADDRESS addr, DATA data, unsigned int size);

	// Burst size control
	virtual void set_burst_size( int burst_size );
	virtual int get_burst_size();

protected:
	int m_irq_num;
	UC_tlm_target_socket<> m_target_socket;
	int m_burst_size;				///< The burst size accepted by the peripheral
	
};

#endif
