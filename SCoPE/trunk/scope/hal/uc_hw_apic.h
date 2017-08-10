//==========================================================================
//	uc_hw_apic.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mié dic 10 2008
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

#ifndef UC_HW_APIC_H
#define UC_HW_APIC_H

#include "hal/uc_hw_if.h"
#include "hal/uc_apic_slave.h"

class UC_hw_APIC : public UC_hw_if
{
public:
	UC_hw_APIC(sc_module_name module_name, unsigned int begin, unsigned int end);

	int read(ADDRESS addr, DATA data, unsigned int size);
	int write(ADDRESS addr, DATA data, unsigned int size);

	void set_irq_ports(vector<UC_tlm_target_socket<> *> *irq_ports);

	void raise_irq(int irq);

private:
	bool bit_test(int str, int bit);
	void bit_set(int str, int bit);
	void bit_clear(int str, int bit);

	vector<UC_tlm_target_socket<> *> *m_irq_ports;	///< Bus' master ports to send the IRQs
	UC_APIC_slave m_apic_slaves[3];	///< The APIC slaves that control the IRQs
	int m_irq_master;	///< The master to send the IRQ (4 bits per apic_slave)
	int m_irq_slave;	///< The IRQ number for each apic_slave (4 bits per apic_slave)
	int m_status;		///< IRQ status register
	int m_raw_status;	///< IRQ status register before masking
	int m_enable;		///< IRQ mask
};

#endif
