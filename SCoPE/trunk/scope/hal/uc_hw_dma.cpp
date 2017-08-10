//==========================================================================
//	uc_hw_dma.cpp
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

#include "hal/uc_hw_dma.h"
#include "hal/uc_tlm_bus_class.h"

/**
 * \brief Custom constructor
 *
 * \param module_name module's name
 * \param irq_num HW interruption
 * \param ret HW delay time
 */
UC_hw_dma::UC_hw_dma(sc_module_name module_name, unsigned int begin, unsigned int end)
	: UC_master_slave_base(module_name, begin, end)
{
	m_n_channels = DMA_CHANNELS;
	m_channels_enabled = 0;
	m_dma_residue = new int[DMA_CHANNELS];
	m_dma_enabled = new bool[DMA_CHANNELS];
	m_dma_mode = new char[DMA_CHANNELS];
	m_dma_addr_mem = new unsigned int[DMA_CHANNELS];
	m_dma_addr_dev = new unsigned int[DMA_CHANNELS];
	m_dma_count = new unsigned int[DMA_CHANNELS];
	m_dma_mask = new unsigned int[DMA_CHANNELS];
	m_start_addr = UC_master_slave_base::get_start_address();

//	m_port.bind(*this);

	SC_THREAD(dma_transactions_func);
}

/**
 * \brief Write function
 *
 * Inherited from uc_generic_bus_if
 * \param data pointer to data to be written
 * \param size bytes of data to be written
 * \param addr HW's address to write to
 * \param extension pointer to an optional struct with additional information
 * \return size of the written data
 */
int UC_hw_dma::write(ADDRESS addr, DATA data, unsigned int size) {
	int reg = (addr - m_start_addr) & 0x1f;
	int ch = (addr - m_start_addr) >> 5;

	if (reg == DMA_ENABLED_PORT) {
		m_dma_enabled[ch] = *((char*)data);
		if (m_dma_enabled[ch] == true) {
			m_channels_enabled++;
			m_dma_event.notify(SC_ZERO_TIME);
		}
	} else if (reg == DMA_MODE_PORT) {
		m_dma_mode[ch] = *((char*)data);
	} else if (reg == DMA_ADDR_MEM_PORT) {
		m_dma_addr_mem[ch] = *((int*)data);
	} else if (reg == DMA_ADDR_DEV_PORT) {
		m_dma_addr_dev[ch] = *((int*)data);
	} else if (reg == DMA_COUNT_PORT) {
		m_dma_count[ch] = *((int*)data);
		m_dma_residue[ch] = *((int*)data);
	} else if (reg == DMA_MASK_PORT) {
		m_dma_mask[ch] = *((int*)data);
	} else if (reg == DMA_CLEAR_PORT) {
		m_dma_enabled[ch] = 0;
		m_dma_mode[ch] = 0;
		m_dma_addr_mem[ch] = 0;
		m_dma_addr_dev[ch] = 0;
		m_dma_count[ch] = 0;
		m_dma_residue[ch] = 0;
	} else cout << "Error in dma, trying to write in a read-only port " << endl;

	return 4; // sizeof(int)
}

/**
 * \brief Read function
 *
 * Inherited from uc_generic_bus_if
 * \param data double pointer to memory for writing in
 * \param size bytes of data to be read
 * \param addr HW's address to read from
 * \return size of the read data
 */
int UC_hw_dma::read(ADDRESS addr, DATA data, unsigned int size) {
	int reg = (addr - m_start_addr) & 0x1f;
	int ch = (addr - m_start_addr) >> 5;
	if (reg == DMA_RESIDUE_PORT) {
		**((int**)data) = m_dma_residue[ch];
	} else cout << "Error in dma, trying to read from a write-only port " << endl;

	return 4;
}

/**
 * \brief Thread that performs the DMA transactions between memory and devices
 */
void UC_hw_dma::dma_transactions_func() {
	void *ptr;
	void *info;
	uc_transfer_extension_t *extension = new uc_transfer_extension_t;

	while (true) {
		if (m_channels_enabled == 0)
			wait(m_dma_event);
		for (int channel = 0; channel < m_n_channels; channel++) {
			if (m_dma_enabled[channel]) {

				ptr = *((void**)&(m_dma_addr_mem[channel]));
				info = malloc(m_dma_count[channel]);

				if (m_dma_mode[channel] == DMA_MODE_READ) { // from device to memory
					extension->ts = sc_time_stamp().value()/1000;
					m_port->read(m_dma_addr_dev[channel], info, m_dma_count[channel], extension);

					extension->ts = sc_time_stamp().value()/1000;
					m_port->write(RAM_START, info, m_dma_count[channel], extension);
					memcpy(ptr, info, m_dma_count[channel]);
					m_dma_residue[channel] = 0;
				} else if(m_dma_mode[channel] == DMA_MODE_WRITE) { // from memory to device
					extension->ts = sc_time_stamp().value()/1000;
					m_port->read(m_start_addr, info, m_dma_count[channel], extension);

					extension->ts = sc_time_stamp().value()/1000;
					m_port->write(m_dma_addr_dev[channel], ptr, m_dma_count[channel], extension);
					m_dma_residue[channel] = 0;
				} else {
					cout << "DMA error: incorrect DMA mode" << endl;
				}

				free(info);

				m_channels_enabled--;
				break; // priority for lower channels
			}
		}
	}
	delete extension;
}

