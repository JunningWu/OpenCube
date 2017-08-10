//==========================================================================
//	sc_scope.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mar abr 29 2008
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

#ifndef _SC_SCOPE_H_
#define _SC_SCOPE_H_

class UC_rtos_class;
class UC_process_class;
extern __thread UC_rtos_class *qt_parent_rtos;
extern __thread UC_process_class *qt_parent_process;

#include <systemc.h>

// SW
#include "rtos/kernel/uc_rtos_class.h"
#include "rtos/kernel/uc_process_class.h"
#include "rtos/api/posix/uc_posix_c.h"

// HAL
#include "hal/uc_tlm_bus_class.h"

// HW
#include "hal/uc_hw_if.h"
#include "hal/uc_hw_memory.h"
#include "hal/uc_memory_map.h"
#include "hal/uc_hw_dma.h"
#include "hal/uc_hw_apic.h"

// NoC
#include "hal/uc_noc_interface.h"
#include "hal/uc_hw_noc.h"

// GUI
#include "rtos/kernel/uc_gui_connector.h"
#include "rtos/utils/uc_gui_interface.h"

// debug
#include "rtos/utils/uc_debug.h"
#include "rtos/utils/uc_simulator_configuration.h"
// drivers
#include "rtos/drivers/uc_linux_module.h"
#include "rtos/drivers/uc_net_tun.h"
#define insmod(rtos, driver) \
	uc_module_init_class kernel_module_init_##driver(); \
	rtos->kernel_module_init(kernel_module_init_##driver())

#endif
