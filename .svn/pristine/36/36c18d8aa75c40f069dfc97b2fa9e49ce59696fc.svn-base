//==========================================================================
//	uc_mqueue.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mar ene 15 2008
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

#include <errno.h>
#include <fcntl.h>
#include "rtos/api/posix/uc_posix.h"
#include "rtos/api/posix/uc_mqueue_class.h"
#include "rtos/api/posix/uc_mqueue.h"

extern __thread UC_rtos_class *qt_parent_rtos;
extern __thread UC_process_class *qt_parent_process;

int uc_mqd_class::unblock(UC_thread_class *thread){
	counter++;
	vector<UC_thread_class *>::iterator it_thread;
	for (it_thread = blocked_threads.begin(); it_thread != blocked_threads.end(); it_thread++) {
		if (*it_thread == thread) {
			blocked_threads.erase(it_thread);
			break;
		}
	}
	return 0;
}

int UC_posix_class::uc_mq_close(uc_mqd_t mqd_id){
	uc_mqd_class *mqd;
	uc_mqd_params *p_aux;

	if ((mqd = qt_parent_rtos->find_mqd(mqd_id)) == NULL) {
		errno = EBADF;
		return -1;
	}

	if ((p_aux = qt_parent_rtos->find_mqd_params(uc_getpid(), mqd)) == NULL) {
		errno = EBADF;
		return -1;
	}

	if (mqd->process_notify == qt_parent_process) {
		mqd->process_notify = NULL;
	}

	vector<uc_mqd_class *>::iterator it_mqd;
	for (it_mqd = qt_parent_rtos->Mqueue_List.begin(); it_mqd != qt_parent_rtos->Mqueue_List.end(); it_mqd++) {
		if ((*it_mqd)->mqd_id == mqd_id) {
			qt_parent_rtos->Mqueue_List.erase(it_mqd);
			break;
		}
	}

	vector<uc_mqd_params *>::iterator it_mqd_p;
	for (it_mqd_p = mqd->mqd_opened.begin(); it_mqd_p != mqd->mqd_opened.end(); it_mqd_p++) {
		if (*it_mqd_p == p_aux) {
			mqd->mqd_opened.erase(it_mqd_p);
			break;
		}
	}

	if (mqd->to_destroy && mqd->mqd_opened.size() == 0) {
		for (it_mqd = qt_parent_rtos->Mqueue_List.begin(); it_mqd != qt_parent_rtos->Mqueue_List.end(); it_mqd++) {
			if (*it_mqd == mqd) {
				qt_parent_rtos->Mqueue_List.erase(it_mqd);
				break;
			}
		}
		delete mqd;
	}
	return 0;
}

uc_mqd_t UC_posix_class::uc_mq_open(const char *name, int flags){
	uc_mqd_params *p_aux;
	uc_mqd_class *mqd;
	uc_mqd_t mqd_id;
	int exists = 0;

	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	if (name == NULL || name[0] != '/') {
		errno = EINVAL;
		return -1;
	}

	if (strlen(name) > PATH_MAX || strlen(name) > NAME_MAX) {
		errno = ENAMETOOLONG;
		return -1;
	}

	if (flags & O_RDWR) {
		flags |= O_RDONLY | O_WRONLY;
	}
/*
	if (thread != NULL && thread->in_fork) {
		thread->semaphore_list_fork.extract_first_element(mqd_id);
		thread->semaphore_list_fork.push_back(mqd_id);
		if ((mqd = uc_find_mqd(mqd_id)) == NULL) {
			return 0;
		}
		if ((p_aux = uc_find_mqd_params(thread->pr_table->pid, mqd)) == NULL) {
			p_aux = new uc_mqd_params;
			p_aux->pid = qt_parent_process->m_pid;
			p_aux->read = 1;
			p_aux->write = 1;
			p_aux->unblock = flags & O_NONBLOCK;
			mqd->mqd_opened.push_back(p_aux);
		}
		return mqd_id;
	}
*/
	vector<uc_mqd_class *>::iterator it_mqd;
	for (it_mqd = qt_parent_rtos->Mqueue_List.begin(); it_mqd != qt_parent_rtos->Mqueue_List.end(); it_mqd++) {
		if (strcmp((*it_mqd)->name, name) == 0) {
			mqd = *it_mqd;
			exists = 1;
			break;
		}
	}

	if (exists) {
		if (flags & O_EXCL) {
			errno = EEXIST;
			return -1;
		}
		else {
			if ((p_aux = qt_parent_rtos->find_mqd_params(qt_parent_process->m_pid, mqd)) == NULL) {
				p_aux = new uc_mqd_params;
			}
			else {
				vector<uc_mqd_params *>::iterator it_mqd_p;
				for (it_mqd_p = mqd->mqd_opened.begin(); it_mqd_p != mqd->mqd_opened.end(); it_mqd_p++) {
					if (*it_mqd_p == p_aux) {
						mqd->mqd_opened.erase(it_mqd_p);
						break;
					}
				}
			}
			p_aux->pid = uc_getpid();
			p_aux->unblock = flags & O_NONBLOCK;
			p_aux->read = !(flags & O_WRONLY) || flags & O_RDWR;
			p_aux->write = flags & O_WRONLY || flags & O_RDWR;
			mqd->mqd_opened.push_back(p_aux);

			thread->m_semaphore_list_fork.push_back(mqd->mqd_id);
			return mqd->mqd_id;
		}
	}

	if (flags & O_CREAT) {
		mqd = new uc_mqd_class;
		mqd->attr.mq_curmsgs = 0;
		mqd->attr.mq_msgsize = 1024;
		mqd->attr.mq_maxmsg = 10;
		mqd->attr.mq_flags = flags;

		p_aux = new uc_mqd_params;
		p_aux->pid = uc_getpid();
		p_aux->unblock = flags & O_NONBLOCK;
		p_aux->read = !(flags & O_WRONLY) || flags & O_RDWR;
		p_aux->write = flags & O_WRONLY || flags & O_RDWR;
		mqd->mqd_opened.push_back(p_aux);

		mqd->mqd_id = qt_parent_rtos->m_mqd_id_max++;
		mqd->parent = thread;
		mqd->name = new char[strlen(name)+1];
		strcpy(mqd->name, name);
		qt_parent_rtos->Mqueue_List.push_back(mqd);
		thread->m_semaphore_list_fork.push_back(mqd->mqd_id);
		return mqd->mqd_id;
	}

	errno = ENOENT;
	return -1;
}

uc_mqd_t UC_posix_class::uc_mq_open(const char *name, int flags, int mode, uc_mq_attr *attr){
	uc_mqd_params *p_aux;
	uc_mqd_class *mqd;
	uc_mqd_t mqd_id;
	int exists = 0;

	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	if (name == NULL || name[0] != '/') {
		errno = EINVAL;
		return -1;
	}

	if (strlen(name) > PATH_MAX || strlen(name) > NAME_MAX) {
		errno = ENAMETOOLONG;
		return -1;
	}
/*
	if (thread != NULL && thread->in_fork) {
		thread->semaphore_list_fork.extract_first_element(mqd_id);
		thread->semaphore_list_fork.push_back(mqd_id);
		if ((mqd = uc_find_mqd(mqd_id)) == NULL) {
			return 0;
		}
		if ((p_aux = uc_find_mqd_params(qt_parent_process->pid, mqd)) == NULL) {
			p_aux = new uc_mqd_params;
			p_aux->pid = qt_parent_process->m_pid;
			p_aux->read = 1;
			p_aux->write = 1;
			mqd->mqd_opened.push_back(p_aux);
		}
		return mqd_id;
	}
*/
	vector<uc_mqd_class *>::iterator it_mqd;
	for (it_mqd = qt_parent_rtos->Mqueue_List.begin(); it_mqd != qt_parent_rtos->Mqueue_List.end(); it_mqd++) {
		if (strcmp((*it_mqd)->name, name) == 0) {
			mqd = *it_mqd;
			exists = 1;
			break;
		}
	}

	if (exists) {
		if (flags & O_EXCL) {
			errno = EEXIST;
			return -1;
		}
		else {
			if ((p_aux = qt_parent_rtos->find_mqd_params(qt_parent_process->m_pid, mqd)) == NULL) {
				p_aux = new uc_mqd_params;
			}
			else {
				vector<uc_mqd_params *>::iterator it_mqd_p;
				for (it_mqd_p = mqd->mqd_opened.begin(); it_mqd_p != mqd->mqd_opened.end(); it_mqd_p++) {
					if (*it_mqd_p == p_aux) {
						mqd->mqd_opened.erase(it_mqd_p);
						break;
					}
				}
			}
			p_aux->pid = uc_getpid();
			p_aux->unblock = flags & O_NONBLOCK;
			p_aux->read = !(flags & O_WRONLY) || flags & O_RDWR;
			p_aux->write = flags & O_WRONLY || flags & O_RDWR;
			mqd->mqd_opened.push_back(p_aux);

			thread->m_semaphore_list_fork.push_back(mqd->mqd_id);
			return mqd->mqd_id;
		}
	}
	if (flags & O_CREAT) {
		mqd = new uc_mqd_class;
		if (attr != NULL) {
			if (attr->mq_maxmsg <= 0 || attr->mq_msgsize <= 0) {
				errno = EINVAL;
				return -1;
			}
			mqd->attr = *attr;
			mqd->attr.mq_curmsgs = 0;
		}
		else{
			mqd->attr.mq_curmsgs = 0;
			mqd->attr.mq_msgsize = 1024;
			mqd->attr.mq_maxmsg = 10;
			mqd->attr.mq_flags = flags;
		}

		p_aux = new uc_mqd_params;
		p_aux->pid = uc_getpid();
		p_aux->unblock = flags & O_NONBLOCK;
		p_aux->read = !(flags & O_WRONLY) || flags & O_RDWR;
		p_aux->write = flags & O_WRONLY || flags & O_RDWR;
		mqd->mqd_opened.push_back(p_aux);

		mqd->mqd_id = qt_parent_rtos->m_mqd_id_max++;
		mqd->parent = thread;
		mqd->name = new char[strlen(name)+1];
		strcpy(mqd->name, name);
		qt_parent_rtos->Mqueue_List.push_back(mqd);
		thread->m_semaphore_list_fork.push_back(mqd->mqd_id);
		return mqd->mqd_id;
	}

	errno = ENOENT;
	return -1;
}

int UC_posix_class::uc_mq_notify(uc_mqd_t mqd_id, const struct uc_sigevent *ev) {
	uc_mqd_class *mqd;

	if ((mqd = qt_parent_rtos->find_mqd(mqd_id)) == NULL) {
		errno = EBADF;
		return -1;
	}

	if (ev == NULL) {
		mqd->process_notify = NULL;
	}
	else {
		if (mqd->process_notify != NULL) {
			errno = EBUSY;
			return -1;
		}
		mqd->process_notify = qt_parent_process;
		mqd->notify_event = *ev;
	}
	return 0;
}

int UC_posix_class::uc_mq_getattr(uc_mqd_t mqd_id, struct uc_mq_attr *attr){
	uc_mqd_class *mqd;
	uc_mqd_params *p_aux;
	if ((mqd = qt_parent_rtos->find_mqd(mqd_id)) == NULL) {
		errno = EBADF;
		return -1;
	}

	if ((p_aux = qt_parent_rtos->find_mqd_params(uc_getpid(), mqd)) == NULL) {
		errno = EBADF;
		return -1;
	}

	if (attr == NULL) {
		errno = EINVAL;
		return -1;
	}

	*attr = mqd->attr;
	attr->mq_flags = 0;

	if (p_aux->read) {
		attr->mq_flags |= O_RDONLY;
	}

	if (p_aux->write) {
		attr->mq_flags |= O_WRONLY;
	}

	if (p_aux->unblock) {
		attr->mq_flags |= O_NONBLOCK;
	}

	return 0;
}

int UC_posix_class::uc_mq_setattr(uc_mqd_t mqd_id, const struct uc_mq_attr *attr, struct uc_mq_attr *old_attr){
	uc_mqd_class *mqd;
	uc_mqd_params *p_aux;

	if ((mqd = qt_parent_rtos->find_mqd(mqd_id)) == NULL) {
		errno = EBADF;
		return -1;
	}

	if ((p_aux = qt_parent_rtos->find_mqd_params(uc_getpid(), mqd)) == NULL) {
		errno = EBADF;
		return -1;
	}

	if (attr == NULL) {
		errno = EINVAL;
		return -1;
	}

	if (old_attr != NULL){
		*old_attr = mqd->attr;
		old_attr->mq_flags = 0;

		if (p_aux->read) {
			old_attr->mq_flags |= O_RDONLY;
		}

		if (p_aux->write) {
			old_attr->mq_flags |= O_WRONLY;
		}

		if (p_aux->unblock) {
			old_attr->mq_flags |= O_NONBLOCK;
		}
	}
	p_aux->unblock = attr->mq_flags & O_NONBLOCK;
	return 0;
}

int UC_posix_class::uc_mq_send(uc_mqd_t mqd_id, const char *buffer, size_t size, int msg_prio){
	return uc_mq_timedsend(mqd_id, buffer, size, msg_prio, NULL);
}

int UC_posix_class::uc_mq_timedsend(uc_mqd_t mqd_id, const char *buffer, size_t size, int msg_prio, const struct timespec *abs_time){
	int notify = 1;
	uc_mqd_params *p_aux;
	uc_mqd_class *mqd;
	struct mq_mess_element *mess;

	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	if ((mqd = qt_parent_rtos->find_mqd(mqd_id)) == NULL) {
		errno = EBADF;
		return -1;
	}
	if ((p_aux = qt_parent_rtos->find_mqd_params(uc_getpid(), mqd)) == NULL) {
		errno = EBADF;
		return -1;
	}
	if (p_aux->write == 0) {
		errno = EBADF;
		return -1;
	}
	if (size > (unsigned)mqd->attr.mq_msgsize) {
		errno = EMSGSIZE;
		return -1;
	}
	if (msg_prio >= MQ_PRIO_MAX || msg_prio < 0) {
		errno = EINVAL;
		return -1;
	}
	if (abs_time != NULL && (abs_time->tv_nsec < 0 || abs_time->tv_nsec >= 1000000000)) {
		errno = EINVAL;
		return -1;
	}

	mess = new mq_mess_element;
	mess->priority = msg_prio;
	mess->length = size;
	mess->buffer = (char *)calloc(size + 1, 1);
	memcpy(mess->buffer, buffer, size);

	mess->buffer[size] = '\0';

	while (p_aux->unblock == 0 && mqd->attr.mq_maxmsg <= mqd->mq_mess_list.size()) {
		thread->block(&mqd->wr_blocked_threads, abs_time);

		if (thread->m_timeout_expired && abs_time != NULL) {
			errno = ETIMEDOUT;
			return -1;
		}
/*
		if (thread->m_signal_executed) {
			errno = EINTR;
			return -1;
		}
*/
	}

	if (mqd->attr.mq_maxmsg <= mqd->mq_mess_list.size()) {
		errno = EAGAIN;
		return -1;
	}

	vector<struct mq_mess_element *>::iterator it_mess;
	for (it_mess = mqd->mq_mess_list.begin(); it_mess != mqd->mq_mess_list.end(); it_mess++) {
		if ((*it_mess)->priority < msg_prio) {
			break;
		}
	}

	mqd->mq_mess_list.insert(it_mess, mess);
	mqd->attr.mq_curmsgs++;
	if (mqd->max_entries < mqd->mq_mess_list.size()) {
		mqd->max_entries = mqd->mq_mess_list.size();
	}

	notify = 0;
	while (mqd->blocked_threads.size() != 0) {
		mqd->blocked_threads[0]->set_microcos_unblocker(UC_thread_class::MQUEUE, mqd_id);
		mqd->blocked_threads[0]->unblock();
// 		mqd->blocked_threads.erase(mqd->blocked_threads.begin());
	}

/*
	if (notify && mqd->process_notify != NULL) {
		mqd->process_notify->signals_control.signal_process(&mqd->notify_event);
		mqd->process_notify=NULL;
	}
*/
	return 0;
}

// Function created to support MicroC/OS II integration
// Insert the message at the front of the queue
int UC_posix_class::uc_mq_timedsend_front(uc_mqd_t mqd_id, const char *buffer, size_t size, int msg_prio, const struct timespec *abs_time){
	int notify = 1;
	uc_mqd_params *p_aux;
	uc_mqd_class *mqd;
	struct mq_mess_element *mess;

	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	if ((mqd = qt_parent_rtos->find_mqd(mqd_id)) == NULL) {
		errno = EBADF;
		return -1;
	}

	if ((p_aux = qt_parent_rtos->find_mqd_params(uc_getpid(), mqd)) == NULL) {
		errno = EBADF;
		return -1;
	}

	if (p_aux->write == 0) {
		errno = EBADF;
		return -1;
	}

	if (size > (unsigned)mqd->attr.mq_msgsize) {
		errno = EMSGSIZE;
		return -1;
	}

	if (msg_prio >= MQ_PRIO_MAX || msg_prio < 0) {
		errno = EINVAL;
		return -1;
	}

	if (abs_time != NULL && (abs_time->tv_nsec < 0 || abs_time->tv_nsec >= 1000000000)) {
		errno = EINVAL;
		return -1;
	}

	mess = new mq_mess_element;
	mess->priority = msg_prio;
	mess->length = size;
	mess->buffer = (char *)calloc(size + 1, 1);//new char[40];
	memcpy(mess->buffer, buffer, size);

	mess->buffer[size] = '\0';

	while (p_aux->unblock == 0 && mqd->attr.mq_maxmsg <= mqd->mq_mess_list.size()) {
		mqd->wr_blocked_threads.push_back(thread);
		thread->block(abs_time);
		if (thread->m_timeout_expired && abs_time != NULL) {
			errno = ETIMEDOUT;
			return -1;
		}

		if(thread->m_signal_executed){
			errno = EINTR;
			return -1;
		}
	}

	if (mqd->attr.mq_maxmsg <= mqd->mq_mess_list.size()){
		errno = EAGAIN;
		return -1;
	}

	mqd->mq_mess_list.insert(mqd->mq_mess_list.begin(), mess);

	mqd->attr.mq_curmsgs++;
	if (mqd->max_entries < mqd->mq_mess_list.size()) {
		mqd->max_entries = mqd->mq_mess_list.size();
	}

	notify = 0;
	while (mqd->blocked_threads.size() != 0) {
		mqd->blocked_threads[0]->set_microcos_unblocker(UC_thread_class::MQUEUE, mqd_id);
		mqd->blocked_threads[0]->unblock();
// 		mqd->blocked_threads.erase(mqd->blocked_threads.begin());
	}

/*
	if (notify && mqd->process_notify != NULL) {
	mqd->process_notify->signals_control.signal_process(&mqd->notify_event);
	mqd->process_notify=NULL;
}
*/

	return 0;
}

ssize_t UC_posix_class::uc_mq_receive(uc_mqd_t mqd_id, char * buffer, size_t size, void *msg_prio){
	return uc_mq_timedreceive(mqd_id, buffer, size, msg_prio, NULL);
}

int UC_posix_class::uc_mq_timedreceive(uc_mqd_t mqd_id, char *buffer, size_t size, void *msg_prio, const struct timespec *abs_time){
	uc_mqd_params *p_aux;
	uc_mqd_class *mqd;
	struct mq_mess_element *mess;

	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	if ((mqd = qt_parent_rtos->find_mqd(mqd_id)) == NULL) {
		errno = EBADF;
		return -1;
	}
	if ((p_aux = qt_parent_rtos->find_mqd_params(uc_getpid(), mqd)) == NULL) {
		errno = EBADF;
		return -1;
	}
	if (p_aux->read == 0) {
		errno = EBADF;
		return -1;
	}
	if (size < (unsigned)mqd->attr.mq_msgsize) {
		errno = EMSGSIZE;
		return -1;
	}

	while (p_aux->unblock == 0 && mqd->mq_mess_list.size() <= 0) {
		if (abs_time != NULL && (abs_time->tv_nsec < 0 || abs_time->tv_nsec >= 1000000000)) {
			errno = EINVAL;
			return -1;
		}

		thread->block(&mqd->blocked_threads, abs_time);

		if (thread->m_timeout_expired && abs_time != NULL) {
			errno = ETIMEDOUT;
			return -1;
		}
		/*
		if(thread->signal_executed) {
			errno=EINTR;
			return -1;
		}
		*/
	}

	if (mqd->mq_mess_list.size() <= 0) {
		errno = EAGAIN;
		return -1;
	}

	mess = mqd->mq_mess_list[0];
	mqd->mq_mess_list.erase(mqd->mq_mess_list.begin());

	if (msg_prio != NULL) {
			*(int*)msg_prio = mess->priority;
	}

	if (size > (unsigned)mess->length) {
		size = mess->length;
	}

	memcpy(buffer, mess->buffer, size);

	mqd->attr.mq_curmsgs--;

	while (mqd->wr_blocked_threads.size() != 0) {
		mqd->wr_blocked_threads[0]->set_microcos_unblocker(UC_thread_class::MQUEUE, mqd_id);
		mqd->wr_blocked_threads[0]->unblock();
	}

	delete mess;
	return size;
}

int UC_posix_class::uc_mq_unlink(const char *name) {
	uc_mqd_class *mqd;
	uc_mqd_params *p_aux;
	int exists = 0;

	vector<uc_mqd_class *>::iterator it_mqd;
	for (it_mqd = qt_parent_rtos->Mqueue_List.begin(); it_mqd != qt_parent_rtos->Mqueue_List.end(); it_mqd++) {
		if (strcmp((*it_mqd)->name, name) == 0) {
			exists = 1;
			break;
		}
	}

	if (exists) {
		if ((p_aux = qt_parent_rtos->find_mqd_params(uc_getpid(), mqd)) != NULL) {
			vector<uc_mqd_params *>::iterator it_mqd_p;
			for (it_mqd_p = mqd->mqd_opened.begin(); it_mqd_p != mqd->mqd_opened.end(); it_mqd_p++) {
				if (*it_mqd_p == p_aux) {
					mqd->mqd_opened.erase(it_mqd_p);
					break;
				}
			}
		}

		if (mqd->mqd_opened.size() > 0) {
			mqd->to_destroy = 1;
			return 0;
		}

		for (it_mqd = qt_parent_rtos->Mqueue_List.begin(); it_mqd != qt_parent_rtos->Mqueue_List.end(); it_mqd++) {
			if (*it_mqd == mqd) {
				qt_parent_rtos->Mqueue_List.erase(it_mqd);
				break;
			}
		}

		return 0;
	}

	errno = ENOENT;
	return -1;
}
