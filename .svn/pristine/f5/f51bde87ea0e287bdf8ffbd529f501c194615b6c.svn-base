//==========================================================================
//	uc_generate_execution_trace.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mar 11 2009
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

#ifndef _UC_GENERATE_EXECUTION_TIME_H_
#define _UC_GENERATE_EXECUTION_TIME_H_

#include "rtos/utils/uc_backtrace.h"
#include "rtos/kernel/uc_thread_class.h"
#include <stdio.h>
#include <fcntl.h>


class generate_execution_trace{
	FILE *file;
public:
	generate_execution_trace();
	~generate_execution_trace();

	void print_start(UC_thread_class *thread, int cpu);
	void print_end(UC_thread_class *thread, int cpu);
	void print_start_bus(UC_thread_class *thread, int cpu);
	void print_end_bus(UC_thread_class *thread, int cpu);
};

extern generate_execution_trace execution_trace;

#endif
