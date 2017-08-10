//==========================================================================
//	uc_backtrace.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mié abr 2 2008
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

#ifndef UC_BACKTRACE_H
#define UC_BACKTRACE_H

#include "rtos/utils/uc_simulator_configuration.h"
#include "rtos/kernel/uc_enums.h"
class UC_thread_class;
class UC_task_class;
class UC_process_class;

const char *uc_print_state(UC_process_states state);

void uc_save_backtrace(UC_thread_class *current_thread);

char *uc_get_demangled_backtrace_string(const char * str);

void uc_print_backtrace(UC_thread_class *thread);
void uc_print_backtrace(UC_process_class *process);
void uc_print_backtrace(UC_task_class *task);
void uc_print_backtrace();

int uc_backtrace_required();
int uc_activate_backtrace(int val=1);

#endif
