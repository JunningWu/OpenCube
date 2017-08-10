//==========================================================================
//	uc_master_base.h
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

#ifndef UC_MASTER_BASE_H
#define UC_MASTER_BASE_H

#include "hal/uc_hal_base_if.h"
#include "hal/uc_tlm_bus_sockets.h"

class UC_TLM_bus_class;

/**
 * \brief Functions shared by all bus masters
 */
class UC_master_base : public sc_module, public virtual UC_cpu_if
{

	int m_bus_priority;

public:

	UC_master_base(sc_module_name name);

	virtual void set_bus_priority(int prio);
	virtual int get_bus_priority();
	virtual void bind(UC_TLM_bus_class * bus);

	UC_tlm_initiator_socket<> m_port; ///< Master's port

};
#endif
