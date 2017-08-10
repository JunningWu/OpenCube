//==========================================================================
//	uc_posix.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mié oct 31 2007
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

#ifndef UC_POSIX_CLASS_H
#define UC_POSIX_CLASS_H

#include "rtos/kernel/uc_rtos_class.h"
#include "rtos/kernel/uc_process_class.h"
#include "rtos/api/posix/uc_pthread_rwlock.h"
#include "rtos/api/posix/uc_pthread_spin_lock.h"
#include "rtos/api/posix/uc_mqueue.h"
#include "rtos/drivers/uc_file.h"

class UC_posix_class{
public:
	UC_posix_class(UC_rtos_class *rtos);

	~UC_posix_class();

	UC_task_class * uc_pthread_create(UC_process_class *process, void* (*function)(void *), void *args, const char *name);

	UC_thread_class * uc_create_thread(UC_task_class *task, const uc_pthread_attr_t *__attr,
						void *(*__start_routine) (void *), void *__arg, const char *func_name);

	UC_thread_class *uc_get_thread(uc_pthread_t tid);
	uc_pthread_rwlock_class *get_uc_pthread_rwlock_t(int id);
	struct lockdata *uc_get_rwlock_lockling_element(uc_pthread_rwlock_class *lock, UC_thread_class *thread);
	void eat_time(unsigned long long time);
	void thread_constraints_check_start();
	void thread_constraints_check_end(long long min, long long max);


	/*** POSIX functions ***/

	int uc_pthread_create(uc_pthread_t *__thread_arg, const uc_pthread_attr_t *__attr,
						void *(*__start_routine) (void *), void *__arg, const char *func_name);

	int uc_pthread_attr_init(uc_pthread_attr_t *attr);
	int uc_pthread_attr_destroy(uc_pthread_attr_t *attr);
	int uc_pthread_attr_setdetachstate(uc_pthread_attr_t *attr, int detachstate);
	int uc_pthread_attr_getdetachstate(const uc_pthread_attr_t *attr, int *detachstate);
	int uc_pthread_attr_setschedpolicy(uc_pthread_attr_t *attr, int policy);
	int uc_pthread_attr_getschedpolicy(const uc_pthread_attr_t *attr, int *policy);
	int uc_pthread_attr_setschedparam(uc_pthread_attr_t *attr,const struct uc_sched_param *param);
	int uc_pthread_attr_getschedparam(const uc_pthread_attr_t *attr,struct uc_sched_param *param);
	int uc_pthread_attr_setinheritsched(uc_pthread_attr_t *attr, int inherit);
	int uc_pthread_attr_getinheritsched(const uc_pthread_attr_t *attr, int *inherit);
	int uc_pthread_attr_setscope(uc_pthread_attr_t *attr, int scope);
	int uc_pthread_attr_getscope(const uc_pthread_attr_t *attr, int *scope);
	int uc_pthread_attr_setstack(uc_pthread_attr_t *attr, void *stack, size_t stacksize);
	int uc_pthread_attr_getstack(uc_pthread_attr_t *attr, void **stack, size_t *stacksize);
	int uc_pthread_attr_setstackaddr(uc_pthread_attr_t *attr, void *stack);
	int uc_pthread_attr_getstackaddr(uc_pthread_attr_t *attr, void **stack);
	int uc_pthread_attr_setstacksize(uc_pthread_attr_t *attr, size_t stacksize);
	int uc_pthread_attr_getstacksize(uc_pthread_attr_t *attr, size_t *stacksize);
	int uc_pthread_attr_getguardsize(const uc_pthread_attr_t *attr, size_t *guardsize);
	int uc_pthread_attr_setguardsize(uc_pthread_attr_t *attr, size_t guardsize);
	int uc_pthread_cancel(uc_pthread_t thread);
	void uc_pthread_cleanup_push(void (*routine)(void*), void *arg);
	void uc_pthread_cleanup_pop (int execute);
	int uc_pthread_cond_init(uc_pthread_cond_t *cond, uc_pthread_condattr_t *cond_attr);
	int uc_pthread_cond_destroy(uc_pthread_cond_t *cond);
	int uc_pthread_cond_signal(uc_pthread_cond_t *cond);
	int uc_pthread_cond_broadcast(uc_pthread_cond_t *cond);
	int uc_pthread_cond_timedwait(uc_pthread_cond_t *cond, uc_pthread_mutex_t *mutex,const struct timespec *abstime);
	int uc_pthread_cond_wait(uc_pthread_cond_t *cond, uc_pthread_mutex_t *mutex);
	int uc_pthread_condattr_init(uc_pthread_condattr_t *attr);
	int uc_pthread_condattr_destroy(uc_pthread_condattr_t *attr);
	int uc_pthread_condattr_getpshared(const uc_pthread_condattr_t *attr, int *pshared);
	int uc_pthread_condattr_setpshared(uc_pthread_condattr_t *attr, int pshared);
	int uc_pthread_condattr_getclock(const uc_pthread_condattr_t *attr,clockid_t *clock_id);
	int uc_pthread_condattr_setclock(uc_pthread_condattr_t *attr,clockid_t clock_id);
	int uc_pthread_detach (uc_pthread_t __th);
	int uc_pthread_equal (uc_pthread_t __thread1, uc_pthread_t __thread2);
	void uc_pthread_exit (void *__retval);
	int uc_pthread_join (uc_pthread_t __th, void **__thread_return);
	int uc_pthread_setschedparam(uc_pthread_t target_thread, int policy, const struct uc_sched_param *param);
	int uc_pthread_getschedparam(uc_pthread_t target_thread, int *policy, struct uc_sched_param *param);
	int uc_pthread_setschedprio(uc_pthread_t target_thread, int priority);
	int uc_pthread_getschedprio(uc_pthread_t target_thread, int *priority);
	int uc_pthread_key_create(uc_pthread_key_t *key, void (*destructor)(void*));
	int uc_pthread_key_delete(uc_pthread_key_t key);
	int uc_pthread_setspecific(uc_pthread_key_t key, void *value);
	void *uc_pthread_getspecific(uc_pthread_key_t key);
	int uc_pthread_getcpuclockid(uc_pthread_t thread_id, clockid_t *clock_id);
	int uc_pthread_mutex_init(uc_pthread_mutex_t *mutex,const uc_pthread_mutexattr_t *mutexattr);
	int uc_pthread_mutex_lock(uc_pthread_mutex_t *mutex);
	int uc_pthread_mutex_unlock(uc_pthread_mutex_t *mutex);
	int uc_pthread_mutex_trylock(uc_pthread_mutex_t *mutex);
	int uc_pthread_mutex_destroy(uc_pthread_mutex_t *mutex);
	int uc_pthread_mutex_getprioceiling(const uc_pthread_mutex_t *, int *);
	int uc_pthread_mutex_setprioceiling(const uc_pthread_mutex_t *, int, int *);
	int uc_pthread_mutex_timedlock(uc_pthread_mutex_t *mutex,const struct timespec* timelock);
	int uc_pthread_mutexattr_init(uc_pthread_mutexattr_t *attr);
	int uc_pthread_mutexattr_destroy(uc_pthread_mutexattr_t *attr);
	int uc_pthread_mutexattr_getprioceiling(const uc_pthread_mutexattr_t *, int*);
	int uc_pthread_mutexattr_getprotocol(const uc_pthread_mutexattr_t *, int*);
	int uc_pthread_mutexattr_getpshared(const uc_pthread_mutexattr_t *attr, int*);
	int uc_pthread_mutexattr_gettype(const uc_pthread_mutexattr_t *attr, int*);
	int uc_pthread_mutexattr_setprioceiling( uc_pthread_mutexattr_t *attr, int value);
	int uc_pthread_mutexattr_setprotocol( uc_pthread_mutexattr_t *attr, int value);
	int uc_pthread_mutexattr_setpshared( uc_pthread_mutexattr_t *attr, int value);
	int uc_pthread_mutexattr_settype( uc_pthread_mutexattr_t *attr, int value);
	int uc_pthread_once(uc_pthread_once_t* once_control, void (*init_routine)(void));
	uc_pthread_t uc_pthread_self();
	int uc_pthread_setcancelstate(int state, int *oldstate);
	int uc_pthread_setcanceltype(int type, int *oldtype);
	void uc_pthread_testcancel();

	int uc_pthread_rwlockattr_init(uc_pthread_rwlockattr_t *attr);
	int uc_pthread_rwlockattr_destroy(uc_pthread_rwlockattr_t *attr);
	int uc_pthread_rwlockattr_getpshared(const uc_pthread_rwlockattr_t *attr, int *num);
	int uc_pthread_rwlockattr_setpshared(uc_pthread_rwlockattr_t *attr, int num);
	int uc_pthread_rwlock_init(uc_pthread_rwlock_t *id_lock, const uc_pthread_rwlockattr_t *attr);
	int uc_pthread_rwlock_destroy(uc_pthread_rwlock_t *id_lock);
	int uc_pthread_rwlock_rdlock(uc_pthread_rwlock_t *id_lock);
	int uc_pthread_rwlock_tryrdlock(uc_pthread_rwlock_t *id_lock);
	int uc_pthread_rwlock_timedrdlock(uc_pthread_rwlock_t *id_lock, const struct timespec *abs_time);
	int uc_pthread_rwlock_wrlock(uc_pthread_rwlock_t *id_lock);
	int uc_pthread_rwlock_trywrlock(uc_pthread_rwlock_t *id_lock);
	int uc_pthread_rwlock_timedwrlock(uc_pthread_rwlock_t *id_lock, const struct timespec *abs_time);
	int uc_pthread_rwlock_unlock(uc_pthread_rwlock_t *id_lock);

	int uc_pthread_spin_lock(uc_pthread_spinlock_t *lock);
	int uc_pthread_spin_trylock(uc_pthread_spinlock_t *lock);
	int uc_pthread_spin_destroy(uc_pthread_spinlock_t *lock);
	int uc_pthread_spin_init(uc_pthread_spinlock_t *lock, int pshared);
	int uc_pthread_spin_unlock(uc_pthread_spinlock_t *lock);

	uc_mqd_t uc_mq_open(const char *, int);
	uc_mqd_t uc_mq_open(const char *, int, int, uc_mq_attr *);
	int uc_mq_close(uc_mqd_t);
	int uc_mq_notify(uc_mqd_t, const struct uc_sigevent *);
	int uc_mq_getattr(uc_mqd_t, struct uc_mq_attr *);
	int uc_mq_setattr(uc_mqd_t, const struct uc_mq_attr *, struct uc_mq_attr *);
	ssize_t uc_mq_receive(uc_mqd_t, char *, size_t, void *);
	ssize_t uc_mq_timedreceive(uc_mqd_t, char *, size_t,void *, const struct timespec *);
	int uc_mq_send(uc_mqd_t, const char *, size_t, int );
	int uc_mq_timedsend(uc_mqd_t, const char *, size_t, int , const struct timespec *);
	int uc_mq_timedsend_front(uc_mqd_t mqd_id, const char *buffer, size_t size, int msg_prio, const struct timespec *abs_time);
	int uc_mq_unlink(const char *);

	int uc_sched_setparam(int pid, const struct uc_sched_param *param);
	int uc_sched_getparam(int pid, struct uc_sched_param *param);
	int uc_sched_getscheduler(int pid);
	int uc_sched_setscheduler(int pid, int policy, const struct uc_sched_param *param);
	int uc_sched_yield();
	int uc_sched_get_priority_max(int policy);
	int uc_sched_get_priority_min(int policy);
	int uc_sched_rr_get_interval(int pid, struct timespec *tp);
	int uc_sched_setaffinity(int pid, unsigned int len, unsigned long *mask);
	int uc_sched_getaffinity(int pid, unsigned int len, unsigned long *mask);
	struct uc_sched_param * uc_schedparam_init();

	int uc_sem_close(uc_sem_t *);
	int uc_sem_destroy(uc_sem_t *);
	int uc_sem_getvalue(uc_sem_t *, int *);
	int uc_sem_init(uc_sem_t *, int, int);
	uc_sem_t *uc_sem_open(const char *, int);
	uc_sem_t *uc_sem_open(const char *,int , int, int );
	int uc_sem_post(uc_sem_t *);
	int uc_sem_timedwait(uc_sem_t *, const struct timespec *);
	int uc_sem_trywait(uc_sem_t *);
	int uc_sem_unlink(const char *);
	int uc_sem_wait(uc_sem_t *);

	int uc_shm_open(const char *name, int oflag, mode_t mode);
	int uc_ftruncate(int fildes, off_t length);
	int uc_shm_unlink(const char *name);
	void *uc_mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off);
	int uc_munmap(void *addr, size_t len);

	int uc_kill(uc_pid_t, int);
	int uc_killpg(uc_pid_t, int);
	int uc_pthread_kill(uc_pthread_t, int);
	int uc_pthread_sigmask(int, const uc_sigset_t *, uc_sigset_t *);
	int uc_raise(int);
	int uc_sigaction(int, const struct uc_sigaction *, struct uc_sigaction *);
	int uc_sigaddset(uc_sigset_t *, int);
	int uc_sigdelset(uc_sigset_t *, int);
	int uc_sigemptyset(uc_sigset_t *);
	int uc_sigfillset(uc_sigset_t *);
	int uc_sigismember(const uc_sigset_t *, int);
	uc_sighandler_t uc_signal(int sig, uc_sighandler_t handler);
	int uc_sigpending(uc_sigset_t *);
	int uc_sigprocmask(int, const uc_sigset_t *, uc_sigset_t *);
	int uc_sigqueue(uc_pid_t, int, const union uc_sigval);
	int uc_sigsuspend(const uc_sigset_t *);
	int uc_sigtimedwait(const uc_sigset_t *, uc_siginfo_t *, const struct timespec *);
	int uc_sigwait(const uc_sigset_t *, int *);
	int uc_sigwaitinfo(const uc_sigset_t *, uc_siginfo_t *);
	void (*uc_bsd_signal(int sig, void (*handler)(int)))(int);
	void (*uc_sigset(int sig, void (*disp)(int)))(int);
	int uc_sighold(int);
	int uc_sigignore(int);
	int uc_sigpause(int);
	int uc_sigrelse(int);
	int uc_siginterrupt(int, int);
	int uc_has_permission(UC_process_class *current_process, UC_process_class *process);

	clock_t uc_clock();
	time_t uc_time(time_t *);
	double uc_difftime(time_t time1, time_t time0);
	int uc_nanosleep(const struct timespec *nseconds, struct timespec *rem);
	int uc_clock_getcpuclockid(int pid, clockid_t *clockid);
	int uc_clock_getres(clockid_t clock_id, struct timespec *rqtp);
	int uc_clock_gettime(clockid_t clock_id, struct timespec *rqtp);
	int uc_clock_settime(clockid_t clock_id, const struct timespec *rqtp);
	int uc_clock_nanosleep(clockid_t clock_id, int flags, const struct timespec *rqtp, struct timespec *rmtp);
	int uc_timer_create(clockid_t clockid, struct uc_sigevent *s_ev, timer_t *timer);
	int uc_timer_delete(timer_t timer);
	int uc_timer_gettime(timer_t time, struct itimerspec * spec);
	int uc_timer_getoverrun(timer_t timer);
	int uc_timer_settime(timer_t timer, int flags, const struct itimerspec *i_spec, struct itimerspec *o_spec);

	int uc_mkstemp(char *tmp_char);
	void uc_abort();
	int uc_system(const char* buf);

	uc_pid_t uc_getpid();
	uc_gid_t uc_getgid();
	uc_pid_t uc_getppid();
	uc_pid_t uc_getpgid(uc_pid_t pid);
	int uc_setpgid(uc_pid_t pid, uc_pid_t pgid);
	uc_pid_t uc_getpgrp();
	int uc_setpgrp();
	int uc_setuid(uc_uid_t uid);
	int uc_getuid();
	uc_gid_t uc_getegid();
	int uc_setegid(uc_gid_t ee);
	uc_uid_t uc_geteuid();
	int uc_seteuid(uc_uid_t ee);
	long uc_gethostid();
	int uc_open(const char *camino, int flags);
	int uc_open(const char *camino, int flags, mode_t modo);
	int uc_creat(const char *camino, mode_t modo);
	int uc_pipe(int filedes[2]);
	int uc_close(int fd);
	int uc_write(int fd, const void *buf, size_t len);
	int uc_write(int fd, const void *buf, size_t len, off_t offset);
	int uc_write_base(int fd, void *buf_i, size_t len);
	int uc_read(int fd, void *buf, size_t len);
	int uc_pread(int fd, void *buf, size_t count, off_t offset);
	int uc_read_base(int fd, void *buf_i, size_t len);
	int uc_read_base_std(int fd,void* buf_i,size_t len);
	int uc_pread_base(int fd, void *buf_i, size_t len, off_t offset);
	int uc_pwrite(int fd, const void *buf, size_t count, off_t offset);
	int uc_pwrite_base(int fd, void* buf_i, size_t len, off_t offset);
	int uc_write_base_std(uc_file *filedes, void *buf_i, size_t len);
	int uc_nice(int inc);
	int uc_usleep(unsigned long int useconds);
	unsigned int uc_sleep(unsigned int seconds);
	unsigned int uc_alarm(unsigned int seconds);
	int uc_pause(void);
	void uc__exit(int status);
	void uc_exit(int status);

	int uc_gettimeofday(struct uc_timeval *tv, void *none);

	uc_pid_t uc_wait(int *ret_val);
	uc_pid_t uc_waitpid (uc_pid_t __pid, int *__stat_loc, int __options);

	int uc_select(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

	int uc_smp_processor_id();

private:
	UC_rtos_class *m_parent_rtos;
};

#endif
