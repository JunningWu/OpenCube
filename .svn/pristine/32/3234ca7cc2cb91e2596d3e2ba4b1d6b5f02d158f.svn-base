//==========================================================================
//	uc_stdlib.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mié ene 23 2008
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

#include "rtos/api/posix/uc_stdlib.h"
#include "rtos/api/posix/uc_posix.h"
#include "rtos/drivers/uc_file.h"

extern __thread UC_process_class *qt_parent_process;
extern __thread UC_rtos_class *qt_parent_rtos;

int UC_posix_class::uc_mkstemp(char *tmp_char) {
	uc_file *filedes = new uc_file;
	int ret = mkstemp(tmp_char);
	filedes->fd = ret;
	qt_parent_rtos->add_file(filedes);
	return ret;
}

void UC_posix_class::uc_abort(){
	uc_raise(SIGABRT);
	uc_exit(1);
}

int UC_posix_class::uc_system(const char* buf){
	printf("Warning: Shell not available for system( %s )\n",buf);
	return -1;
}

void UC_posix_class::uc_exit(int status){
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	vector<UC_thread_class *>::iterator it_thread;
	vector<UC_task_class *>::iterator it_task;
	for (it_task = qt_parent_process->m_task_list.begin(); it_task != qt_parent_process->m_task_list.end(); it_task++) {
		for (it_thread = (*it_task)->m_thread_list.begin(); it_thread != (*it_task)->m_thread_list.end(); it_thread++) {
			uc_pthread_cancel((*it_thread)->m_tid);
			if ((*it_thread)->m_state == BLOCKED) {
				(*it_thread)->unblock();
			}
		}
	}

	UC_thread_class *thread_tmp;
	for (it_task = qt_parent_process->m_task_list.begin(); it_task != qt_parent_process->m_task_list.end(); it_task++) {
		for (int i = 0; i < (*it_task)->m_thread_list.size(); /*nothing*/) {
			thread_tmp = (*it_task)->m_thread_list[i];
			uc_pthread_cancel(thread_tmp->m_tid);
			if (thread_tmp != thread) {
				if(uc_pthread_join(thread_tmp->m_tid, NULL) != 0) i++;
			}
			else {
				i++;
			}
		}
	}

	qt_parent_process->m_exit_code = status;
	uc_pthread_exit((void *)status);
}
