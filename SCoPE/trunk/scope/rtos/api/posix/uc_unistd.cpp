//==========================================================================
//	uc_unistd.cpp
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

#include <errno.h>
#include <fcntl.h>
#include "rtos/api/posix/uc_unistd.h"
#include "rtos/drivers/uc_linux_miscdevice.h"
#include "rtos/kernel/uc_rtos_class.h"
#include "rtos/drivers/uc_file.h"
#include "rtos/api/posix/uc_posix.h"
#include "rtos/drivers/uc_fops.h"
#include "rtos/drivers/uc_io.h"

extern __thread UC_process_class *qt_parent_process;
extern __thread UC_rtos_class *qt_parent_rtos;

uc_pid_t UC_posix_class::uc_getpid(){
	return qt_parent_process->m_pid;
}

uc_gid_t UC_posix_class::uc_getgid(){
	return qt_parent_process->m_pgrp;
}

uc_pid_t UC_posix_class::uc_getppid(){
	return qt_parent_process->m_ppid;
}

uc_pid_t UC_posix_class::uc_getpgid(uc_pid_t pid){
	if (pid < 1) {
		errno = EINVAL;
		return -1;
	}
	vector<UC_process_class *>::iterator it_process;
	for (it_process = qt_parent_rtos->m_process_list.begin(); it_process != qt_parent_rtos->m_process_list.end(); it_process++) {
		if ((*it_process)->m_pid == pid) {
			return (*it_process)->m_pgrp;
		}
	}
	errno = ESRCH;
	return -1;
}

int UC_posix_class::uc_setpgid (pid_t pid,pid_t pgid) {
	UC_process_class *process;
	if (pid < 0) {
		errno = EINVAL;
		return -1;
	}

	if (pid == 0) {
		if (qt_parent_process->m_is_group_leader) {
			return 0;
		}
		qt_parent_process->m_pgrp = pgid;
		qt_parent_process->m_egid = pgid;
		vector<UC_process_class *>::iterator it_process;
		for (it_process = qt_parent_process->m_group_leader->m_group_childs.begin();
				   it_process != qt_parent_process->m_group_leader->m_group_childs.end(); it_process++)
		{
			if (*it_process == qt_parent_process) {
				qt_parent_process->m_group_leader->m_group_childs.erase(it_process);
				break;
			}
		}

		for (it_process = qt_parent_rtos->m_group_leaders.begin(); it_process != qt_parent_rtos->m_group_leaders.end(); it_process++) {
			if ((*it_process)->m_pgrp == pgid) {
				break;
			}
		}

		if (it_process != qt_parent_rtos->m_group_leaders.end()) {
			qt_parent_process->m_is_group_leader = 0;
			qt_parent_process->m_group_leader = *it_process;
			(*it_process)->m_group_childs.push_back(qt_parent_process);
		}
		else {
			qt_parent_process->m_is_group_leader = 1;
			qt_parent_process->m_group_leader = qt_parent_process;
			qt_parent_rtos->m_group_leaders.push_back(qt_parent_process);
		}

		if (pgid >= qt_parent_rtos->m_last_pgrp) {
			qt_parent_rtos->m_last_pgrp = pgid + 1;
		}
		return 0;
	}
	else {
		vector<UC_process_class *>::iterator it_process;
		for (it_process = qt_parent_rtos->m_process_list.begin();
				   it_process != qt_parent_rtos->m_process_list.end(); it_process++)
		{
			if ((*it_process)->m_pid == pid) {
				if ((*it_process)->m_is_group_leader) {
					return 0;
				}
				(*it_process)->m_pgrp = pgid;
				(*it_process)->m_egid = pgid;
				vector<UC_process_class *>::iterator it_process2;
				for (it_process2 = (*it_process)->m_group_leader->m_group_childs.begin();
								 it_process2 != (*it_process)->m_group_leader->m_group_childs.end(); it_process2++)
				{
					if (*it_process2 == *it_process) {
						(*it_process)->m_group_leader->m_group_childs.erase(it_process2);
						break;
					}
				}

				for (it_process2 = qt_parent_rtos->m_group_leaders.begin(); it_process2 != qt_parent_rtos->m_group_leaders.end(); it_process2++) {
					if ((*it_process2)->m_pgrp == pgid) {
						break;
					}
				}

				if (it_process2 != qt_parent_rtos->m_group_leaders.end()) {
					(*it_process)->m_is_group_leader = 0;
					(*it_process)->m_group_leader = *it_process2;
					(*it_process2)->m_group_childs.push_back(*it_process);
				}
				else {
					(*it_process)->m_is_group_leader = 1;
					(*it_process)->m_group_leader = (*it_process);
					qt_parent_rtos->m_group_leaders.push_back(*it_process);
				}

				if (pgid >= qt_parent_rtos->m_last_pgrp) {
					qt_parent_rtos->m_last_pgrp = pgid + 1;
				}
				return 0;
			}
		}
	}
	errno = ESRCH;
	return -1;
}

uc_pid_t UC_posix_class::uc_getpgrp() {
	return qt_parent_process->m_pgrp;
}

int UC_posix_class::uc_setpgrp() {
	if (qt_parent_process->m_is_group_leader) {
		return 0;
	}
	vector<UC_process_class *>::iterator it_process;
	for (it_process = qt_parent_process->m_group_leader->m_group_childs.begin();
			it_process != qt_parent_process->m_group_leader->m_group_childs.end(); it_process++)
	{
		if (*it_process == qt_parent_process) {
			qt_parent_process->m_group_leader->m_group_childs.erase(it_process);
			break;
		}
	}

	qt_parent_process->m_pgrp = qt_parent_rtos->m_last_pgrp;
	qt_parent_process->m_egid = qt_parent_rtos->m_last_pgrp++;
	qt_parent_process->m_is_group_leader = 1;
	qt_parent_process->m_group_leader = qt_parent_process;
	qt_parent_rtos->m_group_leaders.push_back(qt_parent_process);
	return 0;
}

int UC_posix_class::uc_setuid(uc_uid_t uid){
	qt_parent_process->m_uid = uid;
	qt_parent_process->m_euid = uid;
	return 0;
}

uc_uid_t UC_posix_class::uc_getuid(){
	return qt_parent_process->m_uid;
}

uc_uid_t UC_posix_class::uc_geteuid (){
	return qt_parent_process->m_euid;
}

int UC_posix_class::uc_seteuid (uc_uid_t ee){
	qt_parent_process->m_euid = ee;
	return 0;
}

uc_gid_t UC_posix_class::uc_getegid (){
	return qt_parent_process->m_egid;
}

int UC_posix_class::uc_setegid (uc_gid_t ee){
	qt_parent_process->m_egid = ee;
	return 0;
}

long UC_posix_class::uc_gethostid(){
	return qt_parent_rtos->get_current_cpu()->m_id;
}

int UC_posix_class::uc_write(int fd, const void *buf_i, size_t len) {
	int size;
	int ret;

	uc_pthread_testcancel();

	vector<struct uc_device_t *>::iterator it_device;
	for (it_device = qt_parent_rtos->m_dev_list.begin(); it_device != qt_parent_rtos->m_dev_list.end(); it_device++) {
		if ((*it_device)->fd == fd) {
			int major = (*it_device)->major;
			if (major == MISC_DEVICE_MAJOR) {	// miscdevice
				vector<struct uc_driver_t *>::iterator it_driver;
				for (it_driver = qt_parent_rtos->m_miscdriver_list.begin();
								 it_driver != qt_parent_rtos->m_miscdriver_list.end(); it_driver++)
				{
					if ((*it_driver)->minor == (*it_device)->minor) {
						ret = (*it_driver)->fops->uc_write(&(*it_device)->dev_file, (const char*)buf_i, len, &(*it_device)->dev_file.f_pos);
						break;
					}
				}
			} else {
				ret = qt_parent_rtos->m_driver_list[major].fops->uc_write(&(*it_device)->dev_file, (const char*)buf_i, len, &(*it_device)->dev_file.f_pos);
			}
			return ret;
		}
	}

	return uc_write_base(fd, (void *)buf_i,len);
}

int UC_posix_class::uc_write(int fd,const void *buf_i, size_t len, off_t offset) {
	int size;
	int ret;

	uc_pthread_testcancel();

	vector<struct uc_device_t *>::iterator it_device;
	for (it_device = qt_parent_rtos->m_dev_list.begin(); it_device != qt_parent_rtos->m_dev_list.end(); it_device++) {
		if ((*it_device)->fd == fd) {
			loff_t loffset = (loff_t)offset;
			int major = (*it_device)->major;
			if (major == MISC_DEVICE_MAJOR) {	// miscdevice
				vector<struct uc_driver_t *>::iterator it_driver;
				for (it_driver = qt_parent_rtos->m_miscdriver_list.begin();
								 it_driver != qt_parent_rtos->m_miscdriver_list.end(); it_driver++)
				{
					if ((*it_driver)->minor == (*it_device)->minor) {
						ret = (*it_driver)->fops->uc_write(&(*it_device)->dev_file, (const char*)buf_i, len, &loffset);
						break;
					}
				}
			} else {
				ret = qt_parent_rtos->m_driver_list[major].fops->uc_write(&(*it_device)->dev_file, (const char*)buf_i, len, &loffset);
			}
			return ret;
		}
	}

	return uc_write_base(fd, (void *)buf_i, len);
}

int UC_posix_class::uc_pwrite(int fd,const void *buf_i, size_t len, off_t offset) {
	int size;

	uc_pthread_testcancel();
	return uc_pwrite_base(fd, (void *)buf_i, len, offset);
}

int UC_posix_class::uc_write_base(int fd, void *buf_i, size_t len) {
	int flag;
	uc_file *filedes = qt_parent_rtos->get_file(fd);

	if (filedes == NULL) {
		printf("Error: file descriptor does not exists");
	}

	if (filedes->type == 2) {
		return filedes->write_func(fd, buf_i, len);
	}
	else {
		return uc_write_base_std(filedes, buf_i, len);
	}
}

int UC_posix_class::uc_write_base_std(uc_file *filedes, void *buf_i, size_t len) {
		int ret_v = 0, sig_id, owner, flag;

		int signal = 0;
		int state = fcntl(filedes->fd, F_GETFL);
		if (O_ASYNC & state) {
			signal = 1;
			sig_id = fcntl(filedes->fd, F_GETSIG);
			owner = fcntl(filedes->fd, F_GETOWN);
			if (sig_id == 0) {
				sig_id = SIGIO;
			}
			fcntl(filedes->fd, F_SETFL, state & (!O_ASYNC));
		}

		if (!(O_NONBLOCK & state)) {
			fcntl(filedes->fd, F_SETFL, state | O_NONBLOCK);
			ret_v = write(filedes->fd, buf_i, len);
			while (ret_v == 0 || (ret_v == -1 && errno == EAGAIN)) {
				filedes->add_wait_write(qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread);
				uc_pthread_testcancel();
				ret_v = write(filedes->fd, buf_i, len);
			}
		}
		else {
			ret_v = write(filedes->fd, buf_i, len);
		}

		filedes->awake_wait_read();

		uc_kill(owner, sig_id);
		fcntl(filedes->fd, F_SETFL, state);
		fcntl(filedes->fd, F_SETSIG, sig_id);
		fcntl(filedes->fd, F_SETOWN, owner);

		if (ret_v == -1) {
			qt_parent_rtos->get_real_errno();
		}

		return ret_v;
}

int UC_posix_class::uc_pwrite_base(int fd, void* buf_i, size_t len, off_t offset) {
		int ret_v=0, sig_id, owner, flag;

		int signal = 0;
		int state = fcntl(fd, F_GETFL);
		uc_file *filedes = qt_parent_rtos->get_file(fd);
		if (filedes == NULL) {
			printf("Error: file descriptor does not exists");
		}

		if (O_ASYNC & state) {
			signal = 1;
			sig_id = fcntl(fd, F_GETSIG);
			owner = fcntl(fd, F_GETOWN);
			if (sig_id == 0) {
				sig_id = SIGIO;
			}
			fcntl(fd, F_SETFL, state & (!O_ASYNC));
		}

		if (!(O_NONBLOCK & state)) {
			fcntl(fd, F_SETFL, state | O_NONBLOCK);
			ret_v = pwrite(fd, buf_i, len, offset);
			while (ret_v == 0 || (ret_v == -1 && errno == EAGAIN)) {
				filedes->add_wait_write(qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread);
				uc_pthread_testcancel();
				ret_v = pwrite(fd, buf_i, len, offset);
			}
		}
		else {
			ret_v = pwrite(fd, buf_i, len, offset);
		}

		filedes->awake_wait_read();

		uc_kill(owner, sig_id);
		fcntl(fd, F_SETFL, state);
		fcntl(fd, F_SETSIG, sig_id);
		fcntl(fd, F_SETOWN, owner);

		if (ret_v == -1) {
			qt_parent_rtos->get_real_errno();
		}

		return ret_v;
}

int UC_posix_class::uc_read(int fd, void *buf, size_t len) {
	void *buf2 = new char[len + 1];
	int ret, aux;
	uc_pthread_testcancel();

	vector<struct uc_device_t *>::iterator it_device;
	for (it_device = qt_parent_rtos->m_dev_list.begin(); it_device != qt_parent_rtos->m_dev_list.end(); it_device++) {
		if ((*it_device)->fd == fd) {
			int major = (*it_device)->major;
			if (major == MISC_DEVICE_MAJOR) {	// miscdevice
				vector<struct uc_driver_t *>::iterator it_driver;
				for (it_driver = qt_parent_rtos->m_miscdriver_list.begin();
								 it_driver != qt_parent_rtos->m_miscdriver_list.end(); it_driver++)
				{
					if ((*it_driver)->minor == (*it_device)->minor) {
						ret = (*it_driver)->fops->uc_read(&(*it_device)->dev_file, (char*)buf, len, &(*it_device)->dev_file.f_pos);
						break;
					}
				}
			} else {
				ret = qt_parent_rtos->m_driver_list[major].fops->uc_read(&(*it_device)->dev_file, (char*)buf, len, &(*it_device)->dev_file.f_pos);
			}
			delete[] (char*)buf2;
			return ret;
		}
	}

	ret = uc_read_base(fd,buf2,len);
	delete[] (char*)buf2;
	return ret;
}

int UC_posix_class::uc_pread(int fd, void *buf, size_t len, off_t offset) {
	void *buf2 = new char[len+1];
	uc_pthread_testcancel();
	int res = uc_pread_base(fd, buf2, len, offset);
	delete [] (char*)buf2;
	return res;
}

int UC_posix_class::uc_read_base(int fd, void *buf_i, size_t len) {
	uc_file *filedes = qt_parent_rtos->get_file(fd);
	if (filedes == NULL) {
		printf("Error: file descriptor does not exists");
	}

	if (filedes->type == 2) {
		return filedes->read_func(fd, buf_i, len);
	}
	else {
		return uc_read_base_std(fd, buf_i, len);
	}
}

int UC_posix_class::uc_read_base_std(int fd,void* buf_i,size_t len){
	int ret_v = 0, sig_id, owner, flag;

	int signal = 0;
	int state = fcntl(fd, F_GETFL);
	uc_file *filedes = qt_parent_rtos->get_file(fd);
	if (filedes == NULL) {
		printf("Error: file descriptor does not exists");
	}

	if (O_ASYNC & state) {
		signal = 1;
		sig_id = fcntl(fd, F_GETSIG);
		owner = fcntl(fd, F_GETOWN);
		if (sig_id == 0) {
			sig_id = SIGIO;
		}
		fcntl(fd, F_SETFL, state & (!O_ASYNC));
	}

	if (!(O_NONBLOCK & state)) {
		fcntl(fd, F_SETFL, state | O_NONBLOCK);
		ret_v = read(fd, buf_i, len);
		while (ret_v == 0 || (ret_v == -1 && errno == EAGAIN)) {
			filedes->add_wait_read(qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread);
			uc_pthread_testcancel();
			ret_v = read(fd, buf_i, len);
		}
	}
	else {
		ret_v = read(fd, buf_i, len);
	}

	filedes->awake_wait_write();

	uc_kill(owner, sig_id);
	fcntl(fd, F_SETFL, state);
	fcntl(fd, F_SETSIG, sig_id);
	fcntl(fd, F_SETOWN, owner);

	if (ret_v == -1) {
		qt_parent_rtos->get_real_errno();
	}

	return ret_v;
}

int UC_posix_class::uc_pread_base(int fd, void *buf_i, size_t len, off_t offset) {
	int ret_v = 0, sig_id, owner, flag;

	int signal = 0;
	int state = fcntl(fd, F_GETFL);
	uc_file *filedes = qt_parent_rtos->get_file(fd);
	if (filedes == NULL) {
		printf("Error: file descriptor does not exists");
	}

	if (O_ASYNC & state) {
		signal = 1;
		sig_id = fcntl(fd, F_GETSIG);
		owner = fcntl(fd, F_GETOWN);
		if (sig_id == 0) {
			sig_id = SIGIO;
		}
		fcntl(fd, F_SETFL, state & (!O_ASYNC));
	}

	if (!(O_NONBLOCK & state)) {
		fcntl(fd, F_SETFL, state | O_NONBLOCK);
		ret_v = pread(fd, buf_i, len, offset);
		while (ret_v == 0 || (ret_v == -1 && errno == EAGAIN)) {
			filedes->add_wait_read(qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread);
			uc_pthread_testcancel();
			ret_v = pread(fd, buf_i, len, offset);
		}
	}
	else {
		ret_v = pread(fd, buf_i, len, offset);
	}

	filedes->awake_wait_write();

	uc_kill(owner, sig_id);
	fcntl(fd, F_SETFL, state);
	fcntl(fd, F_SETSIG, sig_id);
	fcntl(fd, F_SETOWN, owner);

	if(ret_v==-1) {
		qt_parent_rtos->get_real_errno();
	}

	return ret_v;
}

int UC_posix_class::uc_nice(int inc){
	if (inc < -19 && inc > 20) {
		return -1;
	}
	qt_parent_rtos->get_current_cpu()->m_current_task->set_nice(inc);
	return 0;
}

int UC_posix_class::uc_usleep(unsigned long useconds){
	struct timespec rqtp;

	if (useconds >= 1000000) {
		errno = EINVAL;
		return -1;
	}

	uc_pthread_testcancel();

	rqtp.tv_sec = (int)(useconds/1e6);
	rqtp.tv_nsec = (int)((useconds - rqtp.tv_sec * 1e6) * 1000);
	qt_parent_rtos->m_tic_timer->clock_nanosleep(CLOCK_REALTIME, 0, &rqtp, NULL);
	uc_pthread_testcancel();
	return 0;
}

unsigned int UC_posix_class::uc_sleep(unsigned int seconds){
	struct timespec rqtp, rem;
	uc_pthread_testcancel();

	rqtp.tv_sec = seconds;
	rqtp.tv_nsec = 0;
	int ret = qt_parent_rtos->m_tic_timer->clock_nanosleep(CLOCK_REALTIME, 0, &rqtp, &rem);
	uc_pthread_testcancel();
	if (ret == EINTR) {
		return rem.tv_sec;
	}
	return 0;
}

unsigned int UC_posix_class::uc_alarm(unsigned int seconds){
	struct uc_sigevent s_ev;
	timer_t timer;
	struct itimerspec i_spec;

	s_ev.sigev_notify = SIGEV_SIGNAL;
	s_ev.sigev_signo = SIGALRM;
	i_spec.it_value.tv_sec = seconds;
	i_spec.it_value.tv_nsec = 0;
	i_spec.it_interval.tv_sec = 0;
	i_spec.it_interval.tv_nsec = 0;

	qt_parent_rtos->m_tic_timer->create_timer(CLOCK_REALTIME, &s_ev, &timer);
	qt_parent_rtos->m_tic_timer->set_time(timer, 0, &i_spec, NULL);
	return 0;
}

int UC_posix_class::uc_pause() {
	uc_pthread_testcancel();
	qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread->block();
	uc_pthread_testcancel();
	return 0;
}

void UC_posix_class::uc__exit(int status){
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

	for (it_task = qt_parent_process->m_task_list.begin(); it_task != qt_parent_process->m_task_list.end(); it_task++) {
		for (it_thread = (*it_task)->m_thread_list.begin(); it_thread != (*it_task)->m_thread_list.end(); it_thread++) {
			uc_pthread_cancel((*it_thread)->m_tid);
			if (*it_thread != thread) {
				uc_pthread_join((*it_thread)->m_tid, NULL);
			}
		}
	}

	qt_parent_process->m_exit_code = status;
	// FIXME
	/*
	ret=uc_ch_exec_mod_SU_ZB();
	current_thread->end_value=end_value;

	if(ret){current_thread->pr_table->exit_code=(int)(int*)end_value;}

	if (!strncmp(current_thread->func_name, "interrupt_", 10)) {
		current_process->pr_table->thread_list.delete_element(current_thread);
		//delete current_thread;
	}

	if(dynamic) throw end_exception;
	else UC_PERFIDIX_Kernel.SystemC_connection.block_thread(current_thread);
	printf("Incoherencia: Death process has been awaked\n");
	UC_BUG();
	*/
}

int UC_posix_class::uc_open(const char *camino, int flags){
	int fd_num;
	vector<struct uc_device_t *>::iterator it_device;
	for (it_device = qt_parent_rtos->m_dev_list.begin(); it_device != qt_parent_rtos->m_dev_list.end(); it_device++) {
		if ((*it_device)->path!=NULL && strcmp((*it_device)->path, camino)==0) {
			//FLAGS?
			int major = (*it_device)->major;
			if (major == MISC_DEVICE_MAJOR) {	// miscdevice
				vector<struct uc_driver_t *>::iterator it_driver;
				for (it_driver = qt_parent_rtos->m_miscdriver_list.begin();
								 it_driver != qt_parent_rtos->m_miscdriver_list.end(); it_driver++)
				{
					if ((*it_driver)->minor == (*it_device)->minor) {
						fd_num = (*it_driver)->fops->uc_open(&(*it_device)->i_node, &(*it_device)->dev_file);
						if (fd_num > 0) {
							(*it_device)->fd = fd_num;
						}
						else {
							(*it_device)->fd = (*it_device)->dev_file.fd;
						}

						return (*it_device)->fd;
					}
				}
			} else {
				fd_num = qt_parent_rtos->m_driver_list[major].fops->uc_open(&(*it_device)->i_node, &(*it_device)->dev_file);
				if (fd_num > 0) {
					(*it_device)->fd = fd_num;
				}
				else {
					(*it_device)->fd = (*it_device)->dev_file.fd;
				}
				return (*it_device)->fd;
			}
		}
	}

	uc_file *filedes = qt_parent_rtos->get_file(camino);
	if (filedes != NULL) {
		filedes->f_count++;
		return filedes->fd;
	}

	filedes = new uc_file;
	int ret = open(camino, flags);
	filedes->fd = ret;
	filedes->f_flags = 0;
	filedes->f_count = 1;
	filedes->name = new char[strlen(camino) + 1];
	strcpy(filedes->name, camino);
	qt_parent_rtos->add_file(filedes);
	return ret;
}

int UC_posix_class::uc_open(const char *camino, int flags, mode_t modo){
	int fd_num;
	vector<struct uc_device_t *>::iterator it_device;
	for (it_device = qt_parent_rtos->m_dev_list.begin(); it_device != qt_parent_rtos->m_dev_list.end(); it_device++) {
		if ((*it_device)->path!=NULL && strcmp((*it_device)->path, camino)==0) {
			//FLAGS?
			int major = (*it_device)->major;
			if (major == MISC_DEVICE_MAJOR) {	// miscdevice
				vector<struct uc_driver_t *>::iterator it_driver;
				for (it_driver = qt_parent_rtos->m_miscdriver_list.begin();
								 it_driver != qt_parent_rtos->m_miscdriver_list.end(); it_driver++)
				{
					if ((*it_driver)->minor == (*it_device)->minor) {
						fd_num = (*it_driver)->fops->uc_open(&(*it_device)->i_node, &(*it_device)->dev_file);
						if (fd_num > 0) {
							(*it_device)->fd = fd_num;
						}
						else {
							(*it_device)->fd = (*it_device)->dev_file.fd;
						}

						return (*it_device)->fd;
					}
				}
			} else {
				fd_num = qt_parent_rtos->m_driver_list[major].fops->uc_open(&(*it_device)->i_node, &(*it_device)->dev_file);
				if (fd_num > 0) {
					(*it_device)->fd = fd_num;
				}
				else {
					(*it_device)->fd = (*it_device)->dev_file.fd;
				}
				return (*it_device)->fd;
			}
		}
	}

	uc_file *filedes = qt_parent_rtos->get_file(camino);
	if (filedes != NULL) {
			filedes->f_count++;
			return filedes->fd;
	}

	filedes = new uc_file;
	int ret = open(camino, flags, modo);
	filedes->fd = ret;
	filedes->f_flags = 0;
	filedes->f_count = 1;
	filedes->name = new char[strlen(camino) + 1];
	strcpy(filedes->name, camino);
	qt_parent_rtos->add_file(filedes);
	return ret;
}

int UC_posix_class::uc_creat(const char *camino, mode_t modo){
	int fd_num;
	vector<struct uc_device_t *>::iterator it_device;
	for (it_device = qt_parent_rtos->m_dev_list.begin(); it_device != qt_parent_rtos->m_dev_list.end(); it_device++) {
		if ((*it_device)->path!=NULL && strcmp((*it_device)->path, camino)==0) {
			//FLAGS?
			int major = (*it_device)->major;
			if (major == MISC_DEVICE_MAJOR) {	// miscdevice
				vector<struct uc_driver_t *>::iterator it_driver;
				for (it_driver = qt_parent_rtos->m_miscdriver_list.begin();
								 it_driver != qt_parent_rtos->m_miscdriver_list.end(); it_driver++)
				{
					if ((*it_driver)->minor == (*it_device)->minor) {
						fd_num = (*it_driver)->fops->uc_open(&(*it_device)->i_node, &(*it_device)->dev_file);
						if (fd_num > 0) {
							(*it_device)->fd = fd_num;
						}
						else {
							(*it_device)->fd = (*it_device)->dev_file.fd;
						}

						return (*it_device)->fd;
					}
				}
			} else {
				fd_num = qt_parent_rtos->m_driver_list[major].fops->uc_open(&(*it_device)->i_node, &(*it_device)->dev_file);
				if (fd_num > 0) {
					(*it_device)->fd = fd_num;
				}
				else {
					(*it_device)->fd = (*it_device)->dev_file.fd;
				}
				return (*it_device)->fd;
			}
		}
	}

	uc_file *filedes = qt_parent_rtos->get_file(camino);
	if (filedes != NULL) {
			filedes->f_count++;
			return filedes->fd;
	}

	filedes = new uc_file;
	int ret = creat(camino, modo);
	filedes->fd = ret;
	filedes->f_flags = 0;
	filedes->f_count = 1;
	filedes->name = new char[strlen(camino) + 1];
	strcpy(filedes->name, camino);
	qt_parent_rtos->add_file(filedes);
	return ret;
}

int UC_posix_class::uc_pipe(int fd[2]) {
	uc_file *filedes = new uc_file;
	uc_file *filedes2 = new uc_file;

	int ret = pipe(fd);
	filedes->fd = fd[0];
	filedes->type = 1;
	filedes->couple = filedes2;
	filedes->is_closed = 0;
	filedes->f_flags = 0;
	filedes->f_count = 1;
	qt_parent_rtos->add_file(filedes);
	filedes2->fd = fd[1];
	filedes2->type = 1;
	filedes2->couple = filedes;
	filedes2->is_closed = 0;
	filedes2->f_count = 1;
	filedes2->f_flags = 0;
	qt_parent_rtos->add_file(filedes2);
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	thread->m_semaphore_list_fork.push_back(fd[0]);
	thread->m_semaphore_list_fork.push_back(fd[1]);
	return ret;
}

int UC_posix_class::uc_close(int fd){
	vector<struct uc_device_t *>::iterator it_device;
	for (it_device = qt_parent_rtos->m_dev_list.begin(); it_device != qt_parent_rtos->m_dev_list.end(); it_device++) {
		if ((*it_device)->fd == fd) {
			(*it_device)->fd = -1;
			break;
		}
	}

	uc_file *filedes = qt_parent_rtos->get_file(fd);
	if (filedes == NULL) {
		return -1;
	}

	filedes->f_count--;
	if (filedes->f_count == 0) {
		if (filedes->type == 2) {
			filedes->close_func(fd);
		}
		else {
			close(fd);
			qt_parent_rtos->delete_file(filedes);
		}
	}
	return 0;
}

int UC_posix_class::uc_shm_open(const char *name, int oflag, mode_t mode) {
	return shm_open(name, oflag, mode);
}

int UC_posix_class::uc_ftruncate(int fildes, off_t length) {
	return ftruncate(fildes, length);
}

int UC_posix_class::uc_shm_unlink(const char *name) {
	return shm_unlink(name);
}

void *UC_posix_class::uc_mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off) {
	return mmap(addr, len, prot, flags, fildes, off);
}

int UC_posix_class::uc_munmap(void *addr, size_t len) {
	return munmap(addr, len);
}
