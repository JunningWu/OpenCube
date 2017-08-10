//==========================================================================
//	uc_ll_bus_io.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mar feb 5 2008
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
#include "rtos/low_level/uc_ll_bus_io.h"
#include "rtos/kernel/uc_rtos_class.h"
#include "rtos/kernel/uc_process_class.h"
#include "hal/uc_bus_data.h"
#include "hal/uc_memory_map.h"
#include "rtos/drivers/uc_hal_sw.h"

extern __thread UC_rtos_class *qt_parent_rtos;
extern __thread UC_process_class *qt_parent_process;

/**
 * \brief Reads 8 bits from a device
 *
 * \param addr Address to read from
 * \return The 8 read bits
 */
char uc_ioread8(void *addr) {
	char *val = (char*)malloc(1);
	if (uc_ioreadBurst((void**)&val, 1, addr) < 1) {
		qt_parent_rtos->get_current_cpu()->send_irq(SW_BUS_EXC);
	}
	char ret = *val;
	free(val);
	return ret;
}

/**
 * \brief Writes 8 bits to a device
 *
 * \param value the 8 bits to write
 * \param addr address to write to
 */
void uc_iowrite8(unsigned char value, void *addr){
	if (uc_iowriteBurst((void*)&value, 1, addr) < 1) {
		qt_parent_rtos->get_current_cpu()->send_irq(SW_BUS_EXC);
	}
}

/**
 * \brief Reads 32 bits from a device
 *
 * \param addr address to read from
 * \return the 32 read bits
 */
long uc_ioread32(void *addr) {
	int *val = (int*)malloc(4);
	if (((int)addr & 0x3) != 0) {
		qt_parent_rtos->get_current_cpu()->send_irq(ALIGN_EXC);
		return 0;
	}

	if (uc_ioreadBurst((void**)&val, 4, addr) < 4) {
		qt_parent_rtos->get_current_cpu()->send_irq(SW_BUS_EXC);
	}

	long ret = *val;
	free(val);
	return ret;
}

/**
 * \brief Writes 32 bits to a device
 *
 * \param value the 32 bits to write
 * \param addr address to write to
 */
void uc_iowrite32(unsigned long value, void *addr){
	if (((int)addr & 0x3) != 0) {
		qt_parent_rtos->get_current_cpu()->send_irq(ALIGN_EXC);
		return;
	}

	if (uc_iowriteBurst((void*)&value, 4, addr) < 4) {
		qt_parent_rtos->get_current_cpu()->send_irq(SW_BUS_EXC);
	}
}

/**
 * \brief Reads 'size' bytes from a device
 *
 * '*value' needs to be a buffer and not only a pointer
 * \param value a double pointer to write the read data
 * \param size the number of bytes to read
 * \param addr address to read from
 * \return the number of read bytes
 */
int uc_ioreadBurst(void **value,int size, void *addr) {
	int ret,sent = 0;

	ret = read((ADDRESS)addr, (DATA&)*value, size);
	sent += ret;

	while (sent < size && ret > 0) {
		ret = read((ADDRESS)addr, (DATA&)*value, size - sent);
		sent += ret;
	}

	if (sent < 0) {
		qt_parent_rtos->get_current_cpu()->send_irq(SW_BUS_EXC);
	}

	if (ret > 0) {
		return sent;
	}
	else {
		return sent - ret;
	}
}

/**
 * \brief Writes 'size' bytes to a device
 * \param value a pointer to read from
 * \param size the number of bytes to write
 * \param addr address to write to
 * \return the number of written bytes
 */
int uc_iowriteBurst(void *value, int size, void *addr) {
	int ret, sent = 0;

	ret = write((ADDRESS)addr, (DATA)value, size);
	sent += ret;

	while (sent < size && ret > 0) {
		value = &((char*)value)[ret];
		ret = write((ADDRESS)addr, (DATA)value, size - sent);
		sent += ret;
	}

	if (sent < 0) {
		qt_parent_rtos->get_current_cpu()->send_irq(SW_BUS_EXC);
	}

	if (ret > 0) {
		return sent;
	}
	else {
		return sent - ret;
	}
}
