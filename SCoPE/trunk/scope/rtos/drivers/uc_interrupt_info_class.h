//==========================================================================
//	uc_interrupt_info_class.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: lun dic 10 2007
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

#ifndef UC_INTERRUPT_INFO_CLASS_H
#define UC_INTERRUPT_INFO_CLASS_H

#include "rtos/kernel/uc_cpu_class.h"
#include "rtos/api/posix/sys_uc_types.h"
#include "rtos/drivers/linux/uc_irqreturn.h"

class UC_cpu_class;

class UC_interrupt_info_class{
public:
    UC_interrupt_info_class();

    ~UC_interrupt_info_class();

	irqreturn_t (*m_handler)(int, void *, struct pt_regs *); ///< The interruption handler
	void *m_args; ///< The interruption handler's arguments
	struct pt_regs *m_regs; ///< The cpu registers
	int m_irq; ///< The interruption number
	UC_cpu_class *m_cpu; ///< The cpu that executes the interruption
	/**
	 * \brief Flags for the irq
	 *
	 * false = slow interrupt
	 * true = fast interrupt
	 */
	bool m_flag;
};

#endif
