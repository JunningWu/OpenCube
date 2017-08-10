//==========================================================================
//	uc_hw_dma.h
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

#ifndef _UC_HW_DMA_H
#define _UC_HW_DMA_H

#include "hal/uc_tlm_bus_sockets.h"
#include "hal/uc_memory_map.h"
#include "hal/uc_bus_data.h"
#include "hal/uc_master_slave_base.h"

class UC_TLM_bus_class;

/**
 * \brief Module DMA
 *
 * This class represents a DMA module
 */
class UC_hw_dma : public UC_master_slave_base
{
	typedef int dma_addr_t;

public :
	SC_HAS_PROCESS(UC_hw_dma);

	UC_hw_dma(sc_module_name module_name, unsigned int begin, unsigned int end);

	int read(ADDRESS addr, DATA data, unsigned int size);
	int write(ADDRESS addr, DATA data, unsigned int size);

private:
	void dma_transactions_func();

	int m_n_channels; ///< Number of channels

	int *m_dma_residue; ///< Array of sizes of remaining data in bytes

	bool *m_dma_enabled; ///< Array of enabled channels

	char *m_dma_mode; ///< Array of DMA modes

	unsigned int *m_dma_addr_mem; ///< Array of addresses to memory

	unsigned int *m_dma_addr_dev; ///< Array of addresses to device

	unsigned int *m_dma_count; ///< Array of sizes of transfered data in bytes

	/**
	 * \brief Array of the DMA masks
	 *
	 * Lines to be used to transfer data
	 * 8  bits = 0xff
	 * 16 bits = 0xffff
	 * 32 bits = 0xffffff
	 */
	unsigned int *m_dma_mask;

	int m_channels_enabled; ///< Number of enabled channels

	sc_event m_dma_event; ///< Event to awake dma_transactions_func thread if it is waiting for transferences

	unsigned int m_start_addr; ///< DMA base addr
};

#endif
