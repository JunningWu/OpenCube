//==========================================================================
//	uc_apic_slave.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mar dic 16 2008
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

#include <iostream>
#include "hal/uc_apic_slave.h"
#include "rtos/utils/uc_debug.h"
using namespace std;

UC_APIC_slave::UC_APIC_slave()
{
	m_status = 0;
	m_raw_status = 0;
	m_enable = 0xffffffff;
}

int UC_APIC_slave::get_status()
{
	return m_status;
}

int UC_APIC_slave::get_raw_status()
{
	return m_raw_status;
}

int UC_APIC_slave::get_enable()
{
	return m_enable;
}

bool UC_APIC_slave::set_clear(int clear)
{
	m_status = m_status & ~clear;
	m_raw_status = m_raw_status & ~clear;

	return (m_status != 0);
}

void UC_APIC_slave::set_enable_set(int enable_set)
{
	m_enable = m_enable | enable_set;
}

void UC_APIC_slave::set_enable_clear(int enable_clear)
{
	m_enable = m_enable & ~enable_clear;
}

bool UC_APIC_slave::raise_irq(int irq)
{
	if (irq < 0 || irq > 31) {
		//UC_DEBUG(ERROR, "UC_APIC_slave::raise_irq error, irq %d out of bounds", irq);
		return (m_status != 0);
	}

	int mask = 1 << irq;
	m_raw_status = m_raw_status | mask;
	if (m_enable & mask) {
		m_status = m_status | mask;
	}

	return (m_status != 0);
}
