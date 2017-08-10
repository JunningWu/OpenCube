//==========================================================================
//	uc_hw_io.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mié feb 6 2008
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

#include "hal/uc_hw_io.h"

UC_hw_io::UC_hw_io(sc_module_name module_name, unsigned int begin, unsigned int end, int ret)
	: UC_hw_if(module_name, begin, end)
	, response_time(ret, SC_NS)
{
	m_size = -1;
}

int UC_hw_io::write(ADDRESS addr, DATA data, unsigned int size) {
	int i;
	if (size < 0) {
		return size;
	}

	if (m_size == 0) {
		m_size = -1;
		return 0;
	}

	if (m_size < 0) {
		m_size = size;
		wait(IO_ev);
	}

	i = m_size;
	m_size = -1;
	bus_ev.notify();
	wait(response_time);
	return i;
}

int UC_hw_io::read(ADDRESS addr, DATA data, unsigned int size) {
	int i;

	if (size < 0) {
		return size;
	}

	if (m_size == 0) {
		m_size = -1;
		return 0;
	}

	if (m_size < 0) {
		m_size = size;
		wait(IO_ev);
	}

	i = m_size;
	strcpy((char*)data, m_info);
	m_size = -1;
	bus_ev.notify();
	wait(response_time);
	return i;
}

void UC_hw_io::information_received(DATA data, int size, ADDRESS addr) {
	IO_ev.notify();
	if (size <= 0) {
		m_size = 0;
		return;
	}

	m_size = size;

	if (m_info != NULL) {
		free(m_info);
	}

	m_info = (char*)malloc(size);
	memcpy(m_info, data, size);
}

void UC_hw_io::information_sent(int size) {
	IO_ev.notify();
	if (size <= 0) {
		 m_size = 0;
		 return;
	}
	m_size = size;
}
