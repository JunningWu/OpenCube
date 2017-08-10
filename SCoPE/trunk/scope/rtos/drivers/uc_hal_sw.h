//==========================================================================
//	uc_hal_sw.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: jue nov 22 2007
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


#ifndef UC_HAL_SW_H
#define UC_HAL_SW_H

#include "rtos/drivers/uc_hal_sw_defines.h"
class UC_cpu_class;

int HAL_stack_creation(int function_id, ...);
// a: ADDRES, b:DATA, c: size
#define read(a, b, c)				HAL_stack_creation(UC_IO_READ_ID, a, b, c)
#define write(a, b, c)				HAL_stack_creation(UC_IO_WRITE_ID, a, b, c)
#define icache_read(a,b,c)			HAL_stack_creation(UC_ICACHE_READ_ID, a, b, c)
#define dcache_read(a,b,c)			HAL_stack_creation(UC_DCACHE_READ_ID, a, b, c)
#define dcache_write(a,b,c)			HAL_stack_creation(UC_DCACHE_WRITE_ID, a, b, c)

#define send_to_memory(a)			HAL_stack_creation(UC_ICACHE_MEM_ID, a)
#define dcache_from_memory(a)			HAL_stack_creation(UC_DCACHE_MEM_ID, a)
#define dcache_to_memory(a)			HAL_stack_creation(UC_DCACHE_WMEM_ID, a)

#define configure_irq(a, b)			HAL_stack_creation(UC_IO_IRQ_ID, a, b)

#endif
