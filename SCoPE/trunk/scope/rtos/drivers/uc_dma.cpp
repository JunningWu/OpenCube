//==========================================================================
//	uc_dma.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mar feb 12 2008
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
#include <fcntl.h>
#include "rtos/drivers/uc_dma.h"
#include "rtos/low_level/uc_ll_bus_io.h"
#include "hal/uc_memory_map.h"
#include "rtos/kernel/uc_rtos_class.h"
#include "rtos/api/posix/uc_unistd.h"
#include "rtos/kernel/uc_os_defines.h"
#include "rtos/drivers/uc_io.h"

using namespace std;

extern __thread UC_rtos_class *qt_parent_rtos;

/**
 * \brief Acquires the DMA spinlock
 *
 * This function also blocks interrupts on the local processor;
 * thus the return value is the usual "flags'' value, which must
 * be used when reenabling interrupts.
 */
long claim_dma_lock() {
	//return processor_com.uc_claim_dma_lock();
	if (uc_sem_wait(&dma_lock) == -1) {
		uc_sem_init(&dma_lock, 1, 1);
		uc_sem_wait(&dma_lock);
	}
	//uc_pthread_spin_lock(dma_spinlock);
	return 0;
}

/**
 * \brief Returns the DMA spinlock and restores the previous interrupt status
 */
void release_dma_lock(long flags) {
	//processor_com.uc_release_dma_lock(flags);
	uc_sem_post(&dma_lock);
	//uc_pthread_spin_unlock(dma_spinlock);
}

/**
 * \brief Indicates whether the channel must read from the device (DMA_MODE_READ) or write to it (DMA_MODE_WRITE)
 *
 * A third mode exists, DMA_MODE_CASCADE, which is used to release control of the bus.
 * Cascading is the way the first controller is connected to the top of the second, but it can also be
 * used by true ISA bus-master devices. We won't discuss bus mastering here.
 */
void set_dma_mode(unsigned int channel, char mode) {
	//processor_com.uc_set_dma_mode(channel, mode);
	uc_iowrite8(mode, (void*)(DMA_START + (channel - 1) * 0x20 + DMA_MODE_PORT));
}

/**
 * \brief Assigns the address of the DMA buffer.
 *
 * The function stores the addr in the controller.
 * The addr argument must be a bus address
 */
void set_dma_addr(unsigned int channel, unsigned int addr) {
	//processor_com.uc_set_dma_addr(channel, addr);
	uc_iowrite32(addr, (void*)(DMA_START + (channel - 1) * 0x20 + DMA_ADDR_MEM_PORT));
	for (int i = 0; i < MAX_DEV; i++) {
		if (qt_parent_rtos->m_driver_list[i].dma_channel == channel) {
			uc_iowrite32(qt_parent_rtos->m_driver_list[i].start_addr, (void*)(DMA_START + (channel - 1) * 0x20 + DMA_ADDR_DEV_PORT));
			return;
		}
	}
	cout << "Error: the device does not have start address assigned" << endl;
}

/**
 * \brief Assigns the number of bytes to transfer
 *
 * The count argument represents bytes for 16-bit channels as well;
 * in this case, the number must be even.
 */
void set_dma_count(unsigned int channel, unsigned int count) {
	//processor_com.uc_set_dma_count(channel, count);
	uc_iowrite32(count, (void*)(DMA_START + (channel - 1) * 0x20 + DMA_COUNT_PORT));
}

/**
 * \brief A DMA channel can be disabled within the controller
 *
 * The channel should be disabled before the controller is configured,
 * to prevent improper operation (the controller is programmed via eight-bit
 * data transfers, and thus none of the previous functions is executed atomically).
 */
void disable_dma(unsigned int channel) {
	//processor_com.uc_disable_dma(channel);
	uc_iowrite8(0, (void*)(DMA_START + (channel - 1) * 0x20 + DMA_ENABLED_PORT));
}

/**
 * \brief This function tells the controller that the DMA channel contains valid data.
 */
void enable_dma(unsigned int channel) {
	//processor_com.uc_enable_dma(channel);
	uc_iowrite8(1, (void*)(DMA_START + (channel - 1) * 0x20 + DMA_ENABLED_PORT));
}

/**
 * \brief The driver sometimes needs to know if a DMA transfer has been completed
 *
 * This function returns the number of bytes that are still to be transferred.
 * The return value is 0 after a successful transfer and is unpredictable (but not 0)
 * while the controller is working. The unpredictability reflects the fact that the
 * residue is a 16-bit value, which is obtained by two 8-bit input operations.
 */
int get_dma_residue(unsigned int channel) {
	//return processor_com.uc_get_dma_residue(channel);
	return uc_ioread32((void*)(DMA_START + (channel - 1) * 0x20 + DMA_RESIDUE_PORT));
}

/**
 * \brief This function clears the DMA flip-flop.
 *
 * The flip-flop is used to control access to 16-bit registers.
 * The registers are accessed by two consecutive 8-bit operations, and the flip-flop is used to select
 * the least significant byte (when it is clear) or the most significant byte (when it is set).
 * The flip-flop automatically toggles when 8 bits have been transferred; the programmer must clear the
 * flip-flop (to set it to a known state) before accessing the DMA registers.
 */
void clear_dma_ff(unsigned int channel) {
	//processor_com.uc_clear_dma_ff(channel);
	uc_iowrite8(1, (void*)(DMA_START + (channel - 1) * 0x20 + DMA_CLEAR_PORT));
}

int set_dma_mask(unsigned int channel, unsigned int mask) {
	//processor_com.uc_set_dma_mask(channel, mask);
	uc_iowrite32(mask, (void*)(DMA_START + (channel - 1) * 0x20 + DMA_MASK_PORT));
	return 0;
}

void *dma_alloc_coherent(struct device *dev, size_t size, dma_addr_t *bus_addr, int flag) {
	char name[10] = "/dma";
	int fd = uc_shm_open(name, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
	uc_ftruncate(fd, size);
	bus_addr = (dma_addr_t *)uc_mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	struct uc_dev_dma_t *elem = new uc_dev_dma_t;
	elem->dev = dev;
	elem->addr = (void*)bus_addr;
	elem->fd = fd;
	elem->name = name;
	uc_dev_dma_list.push_back(elem);
	return (void*)bus_addr;
}

void dma_free_coherent(struct device *dev, size_t size, void *cpuaddr, dma_addr_t bus_addr) {
	void *addr;
	addr = (void*)bus_addr;
	uc_dev_dma_t *elem;
	for (int i = 0; i < uc_dev_dma_list.size(); i++) {
		elem = uc_dev_dma_list[i];
		if (elem->dev == dev && elem->addr == addr) {
			uc_munmap(addr, size);
			close(elem->fd);
			uc_shm_unlink(elem->name);
			uc_dev_dma_list.erase(uc_dev_dma_list.begin() + i);
			return;
		}
	}
}
