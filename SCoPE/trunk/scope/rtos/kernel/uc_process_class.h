//==========================================================================
//	uc_process_class.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mar oct 30 2007
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

#ifndef UC_PROCESS_CLASS_H
#define UC_PROCESS_CLASS_H

#include <iostream>
#include <vector>
using namespace std;

#include <pthread.h>
#include "rtos/api/posix/uc_pthread.h"
#include "rtos/api/posix/uc_pthread_class.h"
#include "rtos/kernel/uc_task_class.h"
#include "rtos/qt_interface/stp.h"
#include "rtos/api/posix/uc_pthread_rwlock.h"
#include "rtos/api/posix/uc_semaphore.h"
#include "rtos/kernel/uc_signals_control.h"

class UC_rtos_class;
class UC_task_class;
class uc_pthread_rwlock_class;

class UC_process_class{
public:
    UC_process_class(UC_rtos_class *rtos_parent, const char *name, float optimization = -1);

    ~UC_process_class();

	void create_thread(stp_t *stp);

	UC_task_class * create_new_task();
	void destroy_process();

	UC_task_class * get_current_task(int cpu);

	pthread_mutex_t m_process_mutex; ///< Host mutex to block its host POSIX thread

	vector<UC_task_class *> m_task_list; ///< The list of this process' task

	stp_t *m_stp; ///< Pointer to the qt structure of the next thread to execute
	bool m_stp_destroy; ///< Boolean to destroy the host, POSIX thread destroying the qt structure

	UC_rtos_class *m_parent_rtos; ///< The parent UC_rtos_class

	vector<uc_pthread_key_struct*> m_key_list;
	vector<uc_pthread_cond_t *> m_cond_list;
	vector<uc_pthread_rwlock_class *> m_rwlock_list;
	vector<uc_pthread_mutex_t *> m_mutex_list;

	int m_rwlock_max;

	vector<int *> Semaphores_List;

	UC_signals_control signals_control;

	int m_pid, m_ppid;
	uc_uid_t m_uid, m_euid, m_suid, m_fsuid;
	uc_gid_t m_egid, m_sgid, m_fsgid;
// 	int ngroups;
// 	uc_gid_t groups[NGROUPS];

	uc_gid_t m_pgrp;
	int m_is_group_leader;
	UC_process_class *m_group_leader;
	vector<UC_process_class *> m_group_childs;
// 	uc_pid_t session;
// 	uc_pid_t tgid;

	int m_error_code;
	int m_exit_code;
	int m_exit_signal;

	char *m_process_name;
	float m_optimization_factor;
private:
};

#endif
