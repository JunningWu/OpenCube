//==========================================================================
//	uc_hw_apic.cpp
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

#include "hal/uc_hw_apic.h"
#include "rtos/utils/uc_debug.h"

UC_hw_APIC::UC_hw_APIC(sc_module_name module_name, unsigned int begin, unsigned int end)
	: UC_hw_if(module_name, begin, end)
{
	m_status = 0;
	m_raw_status = 0;
	m_enable = 0xffffffff;

	// Starts with the IRQ 2 for all slaves
	m_irq_slave = 0x00000222;
	m_irq_master = 0;
}

/**
 * \brief HW read function overload
 *
 * \param addr Accessed address
 * \param data Buffer where return data
 * \param size Size of data needed
 * \return Number of bytes really read
 */
int UC_hw_APIC::read(ADDRESS addr, DATA data, unsigned int size)
{
	int local_addr = (addr - get_start_address());
	int local_data = 0;

	switch (local_addr) {
		// Main APIC module
		case 0x10: // Status
			local_data = m_status;
			break;
		case 0x14: // Raw_status
			local_data = m_raw_status;
			break;
		case 0x18: // Enable
			local_data = m_enable;
			break;

		// Slave APIC modules
		case 0x30: // Status
		case 0x50:
		case 0x70:
			local_data = m_apic_slaves[(local_addr>>5)-1].get_status();
			break;
		case 0x34: // Raw_status
		case 0x54:
		case 0x74:
			local_data = m_apic_slaves[(local_addr>>5)-1].get_raw_status();
			break;
		case 0x38: // Enable
		case 0x58:
		case 0x78:
			local_data = m_apic_slaves[(local_addr>>5)-1].get_enable();
			break;
		default:
// 			UC_DEBUG(EXIT, "UC_hw_APIC read error, %x out of bounds", addr);
			break;
	}

	*(int*)data = local_data;
	return 4;
}

/**
 * \brief HW write function overload
 *
 * \param addr Accessed address
 * \param data Buffer where data is stored
 * \param size Size of data to write
 * \return Number of bytes really written
 */
int UC_hw_APIC::write(ADDRESS addr, DATA data, unsigned int size)
{
	int local_addr = (addr - get_start_address());
	int local_data = *(int*)data;
	bool slave_status;
	int apic_block;

	switch (local_addr) {
		// IRQ configuration
		case 0x00: // APIC slave - IRQ number
			m_irq_slave = local_data;
			break;
		case 0x04: // IRQ number - master port
			m_irq_master = local_data;
			break;
		// Main APIC module
		case 0x1C: // Clear
			m_raw_status = m_raw_status & ~local_data;
			m_status = m_status & ~local_data;
			// NOTE who has to clear the irq? Main APIC or slave APIC automatically?
			break;
		case 0x20: // Enable_set
			m_enable = m_enable | local_data;
			break;
		case 0x24: // Enable_clear
			m_enable = m_enable & ~local_data;
			break;

		// Slave APIC modules
		case 0x3C: // Clear
		case 0x5C:
		case 0x7C:
			apic_block = (local_addr>>5) - 1;
			slave_status = m_apic_slaves[apic_block].set_clear(local_data);
			if (slave_status == false) {
				int mask = 1 << apic_block;
				m_raw_status = m_raw_status & ~mask;
				m_status = m_status & ~mask;
			}
			break;
		case 0x40: // Enable_set
		case 0x60:
		case 0x80:
			m_apic_slaves[(local_addr>>5)-2].set_enable_set(local_data);
			break;
		case 0x44: // Enable_clear
		case 0x64:
		case 0x84:
			m_apic_slaves[(local_addr>>5)-2].set_enable_clear(local_data);
			break;
		default:
// 			UC_DEBUG(EXIT, "UC_hw_APIC write error, %x out of bounds", addr);
			break;
	}
	return 4;
}

/**
 * \brief Connects the irq ports to the APIC
 *
 * \param irq_ports A pointer to the vector where irq ports are enqueued
 */
void UC_hw_APIC::set_irq_ports(vector< UC_tlm_target_socket < > * > * irq_ports)
{
	m_irq_ports = irq_ports;
}

/**
 * \brief Raises an IRQ and send it if needed
 *
 * \param irq The IRQ sended
 */
void UC_hw_APIC::raise_irq(int irq)
{
	int apic_block = irq / 32; // actual apic slave block
	bool slave_status = m_apic_slaves[apic_block].raise_irq(irq % 32);

	// Irq not enabled, nothing to do
	if (slave_status == false) {
		return;
	}

	int mask = 1 << apic_block;
	m_raw_status = m_raw_status | mask;
	if ((m_enable & mask) && ((m_status & mask) == 0)) {
		m_status = m_status | mask;

		int irq_to_send = (m_irq_slave & (0xF << apic_block)) >> (4*apic_block);
		int master_to_send = (m_irq_master & (0xF << apic_block)) >> (4*apic_block);
		(*(*m_irq_ports)[master_to_send])->send_irq(irq_to_send);
	}
}
