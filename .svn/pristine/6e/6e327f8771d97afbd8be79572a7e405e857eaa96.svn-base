//==========================================================================
//	uc_arbiter_class.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mié ago 27 2008
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

#include "hal/uc_arbiter_class.h"

UC_arbiter_class::UC_arbiter_class()
{
	m_free = true;
}

UC_arbiter_class::~UC_arbiter_class()
{
}

/**
 * \brief Gets the control of the bus
 */
void UC_arbiter_class::request(sc_time time, int master)
{
	if (m_free == true) {
		m_free = false;
	}
	else {
		sc_event *ev = new sc_event;
		m_queue.push_back(ev);
		wait(*ev);
		delete ev;
	}

	wait(time);
}

/**
 * \brief Releases the control of the bus
 */
void UC_arbiter_class::release(sc_time time, int master)
{
	wait(time);

	if (m_queue.size() != 0) {
		sc_event *ev = m_queue.front();
		m_queue.erase(m_queue.begin());
		ev->notify(SC_ZERO_TIME);
	}
	else {
		m_free = true;
	}
}
