//==========================================================================
//	uc_io.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: lun feb 11 2008
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

#ifndef _UC_IO_H
#define _UC_IO_H

#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

#include "../api/posix/uc_pthread_spin_lock.h"
#include "uc_fops.h"
#include "../api/posix/sys_uc_types.h"
#include "../drivers/list.h"

struct __wait_queue {
	unsigned int flags;
#define WQ_FLAG_EXCLUSIVE       0x01
	struct UC_thread_class * task;
	wait_queue_func_t func;
	struct list_head task_list;
};

struct wait_queue_head_t {
	uc_pthread_spinlock_t lock;
	struct list_head task_list;
};

#define MISC_DEVICE_MAJOR 	10
#define TUN_MINOR            200


#define TIMER_MAGIC 0x4b87ad6e
#define TVN_BITS 6
#define TVR_BITS 8
#define TVN_SIZE (1 << TVN_BITS)
#define TVR_SIZE (1 << TVR_BITS)

typedef struct tvec_s {
	struct list_head vec[TVN_SIZE];
} tvec_t;

typedef struct tvec_root_s {
	struct list_head vec[TVR_SIZE];
} tvec_root_t;

struct timer_list;

struct tvec_t_base_s {
	pthread_spinlock_t lock;
	unsigned long timer_jiffies;
	struct timer_list *running_timer;
	tvec_root_t tv1;
	tvec_t tv2;
	tvec_t tv3;
	tvec_t tv4;
	tvec_t tv5;
};

typedef struct tvec_t_base_s tvec_base_t;

struct timer_list {
	struct list_head entry;
	unsigned long expires;

	unsigned long magic;
	uc_pthread_spinlock_t lock;

	void (*function)(long);
	long data;

	struct tvec_t_base_s *base;

	timer_list() {
		uc_pthread_spin_init(&lock,0);
		magic = TIMER_MAGIC;
	}
};

struct timer_list_to_posix_timer {
	void (*function)(long);
	long data;
};

struct uc_timer_list_t {
	timer_list *timer;
	timer_t timer_id;
};

#define TASK_RUNNING			0
#define TASK_INTERRUPTIBLE		1
#define TASK_UNINTERRUPTIBLE	2
#define TASK_STOPPED			4
#define TASK_TRACED				8
#define EXIT_ZOMBIE				16
#define TASK_DEAD				32

///	enum to define the type of driver
enum uc_driver_type { CHAR, BLOCK, NET };

/**
 * \brief Struct for drivers
 *
 * This structure store the information to use a driver
 */
struct uc_driver_t {
	// FIXME each driver can handle several devices. Change irq, dma and addresses to vectors
	char *name;						///< driver's name
	struct file_operations *fops;	///< file operation functions (open, read, write...)
	int major;						///< driver's major number
	int minor;						///< device's minor number (for misc drivers)
	int irq_channel;				///< driver's irq line
	int dma_channel;				///< driver's dma channel
	int start_addr;					///< initial memory address
	int end_addr;					///< final memory address
	enum uc_driver_type type;		///< type of driver: CHAR, BLOCK or NET

	//! Constructor
	uc_driver_t() {
		name = NULL;
		fops = NULL;
		major = -1;
		minor = -1;
		irq_channel = -1;
		dma_channel = -1;
		start_addr = -1;
		end_addr = -1;
		type = CHAR;
	}

	~uc_driver_t() {
		if (name != NULL) {
			free(name);
		}
	}
};

#define DECLARE_WAITQUEUE(q, t) wait_queue_t q = {0, t, default_wake_function, {NULL, NULL}}

#define DECLARE_WAIT_QUEUE_HEAD(q) \
	wait_queue_head_t q = new wait_queue_head_t; \
	q->lock = SPIN_LOCK_UNLOCKED; \
	q->task_list = {&q.task_list, &q.task_list}

#define __WAITQUEUE_INITIALIZER(name, tsk) {	\
	tsk, default_wake_function, { NULL, NULL } }

struct pt_regs {
	long ebx;
	long ecx;
	long edx;
	long esi;
	long edi;
	long ebp;
	long eax;
	int  xds;
	int  xes;
	long orig_eax;
	long eip;
	int  xcs;
	long eflags;
	long esp;
	int  xss;
};

void init_timer(struct timer_list *timer);
void add_timer(struct timer_list *timer);
void del_timer(struct timer_list *timer);

void set_current_state(int state);
void schedule();
long schedule_timeout(long);
int default_wake_function(wait_queue_t *curr, unsigned int mode, int sync, void *key);
int autoremove_wake_function(wait_queue_t *wait, unsigned int mode, int sync, void *key);
void init_waitqueue_head(wait_queue_head_t *q);
void init_waitqueue_entry(wait_queue_t *q, struct task_struct *p);
void init_waitqueue_func_entry(wait_queue_t *q, wait_queue_func_t func);
int waitqueue_active(wait_queue_head_t *q);
void add_wait_queue(wait_queue_head_t *q, wait_queue_t *wait);
void add_wait_queue_exclusive(wait_queue_head_t *q, wait_queue_t *wait);
void remove_wait_queue(wait_queue_head_t *q, wait_queue_t *wait);
void prepare_to_wait(wait_queue_head_t *q, wait_queue_t *wait, int state);
void finish_wait(wait_queue_head_t *q, wait_queue_t *wait);
void __wake_up(wait_queue_head_t *q, unsigned int mode, int nr_exclusive, void *key);
void __wake_up_sync(wait_queue_head_t *q, unsigned int mode, int nr_exclusive, void *key);

void * uc_kmalloc(unsigned int size, int flags);
void uc_kfree(void *objp);

void memcpy_tofs(void * to, const void * from, unsigned long n);
void memcpy_fromfs(void * to, const void * from, unsigned long n);

int register_chrdev(unsigned int major, const char *name, struct file_operations *fops);
int register_blkdev(unsigned int major, const char *name, struct file_operations *fops);
int unregister_chrdev(unsigned int major, const char *name);
int unregister_blkdev(unsigned int major, const char *name);

long sys_getpid(void);
long sys_getuid(void);
long sys_getgid(void);
long sys_geteuid(void);
long sys_getegid(void);
long sys_getppid(void);
long sys_getpgrp(void);

int check_region(unsigned long from, unsigned long extent);
struct resource *request_region(unsigned long from, unsigned long extent, const char *name);
void release_region(unsigned long from, unsigned long extent);
int check_mem_region (unsigned long start, unsigned long extent);
struct resource *request_mem_region (unsigned long start, unsigned long extent, const char *name);
void release_mem_region (unsigned long start, unsigned long extent);

int uc_mknod(char *name, int flags, dev_t dev);

// int request_irq(unsigned int irq, irqreturn_t (*)(int, void *, struct pt_regs *), unsigned long irqflags, const char * devname, void *dev_id);
// void free_irq(unsigned int irq, void *dev_id);

int request_dma(unsigned int channel, const char *name);
void free_dma(unsigned int channel);

unsigned long _uc_spin_lock_irqsave(uc_pthread_spinlock_t *lock);
void uc_spin_unlock_irqrestore(uc_pthread_spinlock_t *lock, unsigned long flags);

void cli();
void sti();

#define uc_spin_lock_irqsave(lock, flags) flags = _uc_spin_lock_irqsave(lock)

#define is_sync_wait(wait)      (!(wait) || ((wait)->task))

#define signal_pending(thread) thread->m_parent_task->m_parent_process->m_signals_control->sigpending(t, NULL)

#define DEFINE_WAIT(name)													\
	wait_queue_t name = {													\
		.task		= qt_parent_process->m_current_task->m_current_thread;,	\
		.func		= autoremove_wake_function,								\
		.task_list	= {	.next = &(name).task_list,							\
						.prev = &(name).task_list,							\
},													\
}

#define __wait_event(wq, condition)								\
{																\
	DEFINE_WAIT(__wait);										\
	for (;;) {													\
		prepare_to_wait(&wq, &__wait, TASK_UNINTERRUPTIBLE);	\
		if (condition)											\
			break;												\
		schedule();												\
}															\
	finish_wait(&wq, &__wait);									\
}

#define wait_event(wq, condition)		\
{										\
	if (!condition)						\
		__wait_event(wq, condition);	\
}

#define __wait_event_interruptible(wq, condition, ret)								\
{																					\
	wait_queue_t __wait;															\
	UC_thread_class *current = qt_parent_process->m_current_task->m_current_thread;	\
	DEFINE_WAIT(__wait);															\
	for (;;) {																		\
		prepare_to_wait(&wq, &__wait, TASK_INTERRUPTIBLE);							\
		if (condition)																\
			break;																	\
		if (!signal_pending(current)) {												\
			schedule();																\
			continue;																\
}																			\
		ret = -ERESTARTSYS;															\
		break;																		\
}																				\
	finish_wait(&wq, &__wait);														\
}

#define wait_event_interruptible(wq, condition)				\
({															\
	int __ret = 0;											\
	if (!(condition))										\
		__wait_event_interruptible(wq, condition, __ret);	\
	__ret;													\
})

//wake_up*()       	Wakes up a process that has been put to sleep using a *sleep_on() function.
#define wake_up(x)								__wake_up((x),TASK_UNINTERRUPTIBLE | TASK_INTERRUPTIBLE, 1, NULL)
#define wake_up_nr(x, nr)						__wake_up((x),TASK_UNINTERRUPTIBLE | TASK_INTERRUPTIBLE, nr, NULL)
#define wake_up_all(x)							__wake_up((x),TASK_UNINTERRUPTIBLE | TASK_INTERRUPTIBLE, 0, NULL)
#define wake_up_sync(x)							__wake_up_sync((x),TASK_UNINTERRUPTIBLE | TASK_INTERRUPTIBLE, 1, NULL)
#define wake_up_sync_nr(x, nr)					__wake_up_sync((x),TASK_UNINTERRUPTIBLE | TASK_INTERRUPTIBLE, nr, NULL)
#define wake_up_interruptible(x)				__wake_up((x),TASK_INTERRUPTIBLE, 1, NULL)
#define wake_up_interruptible_nr(x, nr)			__wake_up((x),TASK_INTERRUPTIBLE, nr, NULL)
#define wake_up_interruptible_all(x)			__wake_up((x),TASK_INTERRUPTIBLE, 0, NULL)
#define wake_up_interruptible_sync(x)			__wake_up_sync((x),TASK_INTERRUPTIBLE, 1, NULL)
#define wake_up_interruptible_sync_nr(x, nr)	__wake_up_sync((x),TASK_INTERRUPTIBLE,  nr, NULL)

/**
 * \brief Reports the major device number for a device
 */
#define MAJOR(a) (((unsigned)(a))>>8)

/**
 * \brief Reports the minor device number for a device
 */
#define MINOR(a) ((a)&0xff)

/**
 * \brief Gets a simple variable from user space
 */
#define get_user(x,ptr) ({memcpy(&x, ptr, sizeof (*(ptr))); 0;})

/**
 * \brief Writes a simple value into user space
 */
#define put_user(x,ptr) ({memcpy(ptr, &x, sizeof (*(ptr))); 0;})

/**
 * \brief A version of printf() for the kernel
 */
#define printk printf

#define uc_local_irq_save(flags) flags = _uc_local_irq_save()

#endif
