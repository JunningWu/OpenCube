//==========================================================================
//	uc_semaphore_class.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: lun mar 31 2008
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

#ifndef UC_SEMAPHORE_CLASS_H
#define UC_SEMAPHORE_CLASS_H

#include <vector>
using namespace std;
#include "rtos/api/posix/uc_sleep_channel.h"

class UC_process_class;

class uc_sem_class : public UC_sleep_channel {
public:
	uc_sem_class() {
		type_of_channel = 3;
		channel_priority = 0;
	}

	int pshared;
	char *name;
	int sem_id;

	vector<UC_thread_class *> open_sem;
	UC_process_class *parent_process;

	int unblock(UC_thread_class *thread);
};

#endif
