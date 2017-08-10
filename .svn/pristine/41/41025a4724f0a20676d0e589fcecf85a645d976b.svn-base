//==========================================================================
//	uc_ll_ctrl_io.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: jue feb 14 2008
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

#include "systemc.h"
#include "rtos/low_level/uc_ll_ctrl_io.h"
#include "rtos/kernel/uc_rtos_class.h"
#include "hal/uc_bus_data.h"
#include "rtos/drivers/uc_hal_sw.h"

extern __thread UC_rtos_class *qt_parent_rtos;
extern __thread UC_process_class *qt_parent_process;

struct uc_periph_control * inctrl(unsigned int port){
	DATA rec;
	// FIXME can it be deleted?
// 	ADDRESS addr = qt_parent_rtos->get_current_cpu()->m_port->get_addr_from_port(port);
// 	read_control(qt_parent_process->m_cpu, rec, sizeof(struct uc_periph_control), addr, 0);

	return (struct uc_periph_control*)rec;
}

void outctrl(struct uc_periph_control *value, unsigned int port){
	// FIXME
// 	ADDRESS addr = qt_parent_rtos->get_current_cpu()->m_port->get_addr_from_port(port);
// 	write_control(qt_parent_process->m_cpu, (DATA)value, sizeof(struct uc_periph_control), addr, 0);
}

struct uc_periph_control * uc_ioreadCtrl(void *addr){
	DATA rec;
	// FIXME
// 	read(qt_parent_process->m_cpu, rec, sizeof(struct uc_periph_control), (unsigned int)addr, 0);
	return (struct uc_periph_control*)rec;
}

void uc_iowriteCtrl(struct uc_periph_control *value, void *addr){
	// FIXME
// 	write_control(qt_parent_process->m_cpu, (DATA)value, sizeof(struct uc_periph_control), (unsigned int)addr, 0);
}
