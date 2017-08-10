//==========================================================================
//	uc_apic_slave.h
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

#ifndef UC_APIC_SLAVE_H
#define UC_APIC_SLAVE_H

class UC_APIC_slave
{
public:
	UC_APIC_slave();

	int get_status();
	int get_raw_status();
	int get_enable();
	bool set_clear(int clear);
	void set_enable_set(int enable_set);
	void set_enable_clear(int enable_clear);
	bool raise_irq(int irq);

private:
	int m_status;		///< IRQ status register
	int m_raw_status;	///< IRQ status register before masking
	int m_enable;		///< IRQ mask
// 	int m_clear;
// 	int m_enable_set;
// 	int m_enable_clear;
};

#endif
