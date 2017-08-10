//==========================================================================
//	uc_dma.h
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


#ifndef _UC_DMA_H
#define _UC_DMA_H

#include "rtos/api/posix/uc_semaphore.h"
#include "rtos/api/posix/uc_semaphore_class.h"
vector<struct uc_dev_dma_t *> uc_dev_dma_list;

#ifdef __cplusplus
extern "C" {
#endif

uc_sem_t dma_lock;
struct uc_dev_dma_t {
	struct device *dev;
	void *addr;
	int fd;
	char *name;

	bool operator == (uc_dev_dma_t a) {
		if ( (a.dev == dev) && (a.addr == addr) && (a.fd == fd) && (a.name == name) ) {
			return true;
		}
		else {
			return false;
		}
	}

	bool operator != (uc_dev_dma_t a) {
		if ( (a.dev == dev) && (a.addr == addr) && (a.fd == fd) && (a.name == name) ) {
			return false;
		}
		else {
			return true;
		}
	}
};


typedef int dma_addr_t;

long claim_dma_lock();
void release_dma_lock(long flags);
void set_dma_mode(unsigned int channel, char mode);
void set_dma_addr(unsigned int channel, unsigned int addr);
void set_dma_count(unsigned int channel, unsigned int count);
void disable_dma(unsigned int channel);
void enable_dma(unsigned int channel);
int get_dma_residue(unsigned int channel);
void clear_dma_ff(unsigned int channel);
int set_dma_mask(unsigned int channel, unsigned int mask);
void *dma_alloc_coherent(struct device *dev, size_t size, dma_addr_t *bus_addr, int flag);
void dma_free_coherent(struct device *dev, size_t size, void *cpuaddr, dma_addr_t bus_addr);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
