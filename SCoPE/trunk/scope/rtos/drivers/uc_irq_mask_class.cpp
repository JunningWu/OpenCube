//==========================================================================
//	uc_irq_mask_class.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: vie dic 14 2007
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

#include "rtos/drivers/uc_irq_mask_class.h"

UC_irq_mask_class::UC_irq_mask_class()
{
	m_current_mask = 0;
	m_masks[0] = 0;
}


UC_irq_mask_class::~UC_irq_mask_class()
{
}

/**
 * \brief Sets the next mask
 *
 * \param next_mask The next mask to store
 */
void UC_irq_mask_class::next_mask(unsigned short next_mask) {
	m_current_mask++;
	m_masks[m_current_mask] = next_mask;
}

/**
 * \brief Gets the current mask
 *
 * \return The current mask
 */
unsigned short UC_irq_mask_class::current_mask() {
	return m_masks[m_current_mask];
}

/**
 * \brief Cleans the current mask and recovers the previous one
 */
void UC_irq_mask_class::previous_mask() {
	m_current_mask--;
}
