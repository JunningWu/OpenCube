//==========================================================================
//	uc_io.cpp
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

#include <errno.h>
#include "rtos/drivers/uc_io.h"
#include "rtos/drivers/uc_resource.h"
#include "rtos/kernel/uc_os_defines.h"
#include "rtos/api/posix/uc_signal.h"
#include "rtos/kernel/uc_rtos_class.h"
#include "rtos/drivers/uc_fops.h"
#include "rtos/low_level/uc_ll_bus_io.h"


extern __thread UC_rtos_class *qt_parent_rtos;
extern __thread UC_process_class *qt_parent_process;

//timer functions

/**
 * \brief Initializes a timer.
 *
 * init_timer() must be done to a timer prior calling *any* of the
 * other timer functions.
 * \param timer The timer to be initialized
 */
void init_timer(struct timer_list * timer)
{
	timer->magic = TIMER_MAGIC;
	uc_pthread_spin_init(&timer->lock, 0);
}

void timer_list_to_posix_timer_func(uc_sigval sig) {
	struct timer_list_to_posix_timer *list = (struct timer_list_to_posix_timer *)( sig.sival_ptr );
	(list->function)(list->data);
}

/**
 * \brief Causes a function to be executed when a given amount of time has passed
 *
 * \param timer The timer to be added
 */
void add_timer(struct timer_list *timer) {
	unsigned long flags;
	if (timer->magic != TIMER_MAGIC) {
		init_timer(timer);
	}
	uc_spin_lock_irqsave(&timer->lock, flags);

	struct uc_sigevent event;
	timer_t timer_id;

	struct timer_list_to_posix_timer *func_and_data = new struct timer_list_to_posix_timer;
	func_and_data->function = timer->function;
	func_and_data->data = timer->data;

	event.sigev_notify = SIGEV_THREAD;
	event.sigev_notify_function = timer_list_to_posix_timer_func;
	event.sigev_value.sival_ptr = (void *)(func_and_data);
	event.sigev_notify_attributes = NULL;

	uc_timer_create(CLOCK_REALTIME, &event, &timer_id);

	itimerspec timerdata;
	timespec t_zero = {0,0};
	timerdata.it_interval = t_zero;
	timespec t_expire = {timer->expires/100, 10000000*(timer->expires%100)};
	timerdata.it_value = t_expire;
	timer_settime(timer_id, 0, &timerdata, NULL);

	uc_timer_list_t *tim = new uc_timer_list_t;
	tim->timer = timer;
	tim->timer_id = timer_id;
	qt_parent_rtos->m_timer_list_control.push_back(tim);
	uc_spin_unlock_irqrestore(&timer->lock, flags);
}

void del_timer(struct timer_list *timer) {
	vector<uc_timer_list_t *>::iterator it_timer;
	for (it_timer = qt_parent_rtos->m_timer_list_control.begin();
			it_timer != qt_parent_rtos->m_timer_list_control.end(); it_timer++)
	{
		if ((*it_timer)->timer == timer) {
			uc_timer_delete((*it_timer)->timer_id);
			return;
		}
	}
}

// queues
void set_current_state(int state) {
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	if ( (state & (TASK_INTERRUPTIBLE | TASK_UNINTERRUPTIBLE)) || (state == TASK_RUNNING) ) {
		thread->wait_queue_state = state;
	} else {
		printf("set_current_state, unknown state");
	}
}

void schedule() {
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	thread->block();
}

long schedule_timeout(long timeout) {
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	struct timespec ts;
	ts.tv_sec = timeout / 1000000;
	ts.tv_nsec= (timeout % 1000000) * 1000;
	thread->block(&ts);

	//FIXME Return value should be the timeout remaining time , not 0
	return 0;
}

int default_wake_function(wait_queue_t *curr, unsigned mode, int sync, void *key) {
	/*
	* Sync wakeups (i.e. those types of wakeups where the waker
	* has indicated that it will leave the CPU in short order)
	* don't trigger a preemption, if the woken up task will run on
	* this cpu. (in this case the 'I will reschedule' promise of
	* the waker guarantees that the freshly woken up task is going
	* to be considered on this CPU.)
	*/
	// FIXME sync is not considered
	if (curr->task->wait_queue_state & mode) {
		curr->task->unblock();
		return 1;
	}

	return 0;
}

int autoremove_wake_function(wait_queue_t *wait, unsigned mode, int sync, void *key) {
        int ret = default_wake_function(wait, mode, sync, key);
        if (ret) {
			list_del_init(&wait->task_list);
		}
        return ret;
}

void init_waitqueue_head(wait_queue_head_t *q) {
	q->task_list.prev = &(q->task_list);
	q->task_list.next = &(q->task_list);
}

void init_waitqueue_entry(wait_queue_t *q, struct task_struct *p) {
	// FIXME it is supposed that the thread 'p' is the actual thread (commonly is true, but might be not)
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	q->flags = 0;
	q->task = thread;
	q->func = default_wake_function;
}

void init_waitqueue_func_entry(wait_queue_t *q, wait_queue_func_t func) {
	q->flags = 0;
	q->task = NULL;
	q->func = func;
}

int waitqueue_active(wait_queue_head_t *q) {
	return !list_empty(&q->task_list);
}

void add_wait_queue(wait_queue_head_t *q, wait_queue_t *wait) {
	unsigned long flags;
	wait->flags &= ~WQ_FLAG_EXCLUSIVE;
// FIXME must work with spin_locks
//	spin_lock_irqsave(&q->lock, flags);
	list_add(&wait->task_list, &q->task_list);
//	spin_unlock_irqrestore(&q->lock, flags);
}

void add_wait_queue_exclusive(wait_queue_head_t *q, wait_queue_t *wait) {
	unsigned long flags;
	wait->flags |= WQ_FLAG_EXCLUSIVE;
	uc_spin_lock_irqsave(&q->lock, flags);
	list_add_tail(&wait->task_list, &q->task_list);
	uc_spin_unlock_irqrestore(&q->lock, flags);
}

void remove_wait_queue(wait_queue_head_t *q, wait_queue_t *wait) {
	unsigned long flags;
//	spin_lock_irqsave(&q->lock, flags);
	list_del(&wait->task_list);
//	spin_unlock_irqrestore(&q->lock, flags);
}

void prepare_to_wait(wait_queue_head_t *q, wait_queue_t *wait, int state) {
	unsigned long flags;
	wait->flags &= ~WQ_FLAG_EXCLUSIVE;
//	spin_lock_irqsave(&q->lock, flags);
	if (list_empty(&wait->task_list)) {
			list_add(&wait->task_list, &q->task_list);
	}
	if (is_sync_wait(wait)) {
			set_current_state(state);
	}
//	spin_unlock_irqrestore(&q->lock, flags);
}

void finish_wait(wait_queue_head_t *q, wait_queue_t *wait) {
	unsigned long flags;

	set_current_state(TASK_RUNNING);
	if (!list_empty_careful(&wait->task_list)) {
		uc_spin_lock_irqsave(&q->lock, flags);
		list_del_init(&wait->task_list);
		uc_spin_unlock_irqrestore(&q->lock, flags);
	}
}

void __wake_up(wait_queue_head_t *q, unsigned int mode, int nr_exclusive, void *key) {
	struct list_head *tmp, *next;

	list_for_each_safe(tmp, next, &q->task_list) {
		wait_queue_t *curr;
		unsigned flags;
		curr = list_entry(tmp, wait_queue_t, task_list);
		flags = curr->flags;
		if (curr->func(curr, mode, 0, key) && (flags & WQ_FLAG_EXCLUSIVE) && !--nr_exclusive) {
			break;
		}
	}
}

void __wake_up_sync(wait_queue_head_t *q, unsigned int mode, int nr_exclusive, void *key) {
	int sync = 1;
	if (!q) {
		return;
	}
	if (!nr_exclusive) {
		sync = 0;
	}
	struct list_head *tmp, *next;

	list_for_each_safe(tmp, next, &q->task_list) {
		wait_queue_t *curr;
		unsigned flags;
		curr = list_entry(tmp, wait_queue_t, task_list);
		flags = curr->flags;
		if (curr->func(curr, mode, sync, key) && (flags & WQ_FLAG_EXCLUSIVE) && !--nr_exclusive) {
			break;
		}
	}
}

/**
 * \brief Allocates a chunk of memory no larger than 33554432
 */
void * uc_kmalloc(unsigned int size, int flags) {
#define CACHE(x) if (size <= x) return calloc(1,x);
#define PAGE_SIZE 0 // there are not pages
#define CONFIG_LARGE_ALLOCS
#if (PAGE_SIZE == 4096)
	CACHE(32)
#endif
	CACHE(64)
	CACHE(128)
	CACHE(256)
	CACHE(512)
	CACHE(1024)
	CACHE(2048)
	CACHE(4096)
	CACHE(8192)
	CACHE(16384)
	CACHE(32768)
	CACHE(65536)
	CACHE(131072)
	CACHE(262144)
	CACHE(524288)
	CACHE(1048576)
#ifdef CONFIG_LARGE_ALLOCS
	CACHE(2097152)
	CACHE(4194304)
	CACHE(8388608)
	CACHE(16777216)
	CACHE(33554432)
#endif
#undef PAGE_SIZE
#undef CONFIG_LARGE_ALLOCS
#undef CACHE
	return NULL;
}

/**
 * \brief Frees memory previously allocated with kmalloc()
 */
void uc_kfree(void *objp) {
	free((void *)objp);
}

/**
 * \brief Copies chunks of memory from kernel space to user space
 */
void memcpy_tofs(void * to, const void * from, unsigned long n) {
	memcpy(to, from, n);
}

/**
 * \brief Copies chunks of memory from user space to kernel space
 */
void memcpy_fromfs(void * to, const void * from, unsigned long n) {
	memcpy(to, from, n);
}

//register_*dev()  	Registers a device with the kernel.
int register_chrdev(unsigned int major, const char *name, struct file_operations *fops) {
	if (major >= MAX_DEV) {
		return -EINVAL;
	}

	// If major == 0, dynamic allocation
	// It looks for a free major number
	if (major == 0) {
		for (int i = 0; i < MAX_DEV; i++) {
			if (qt_parent_rtos->m_driver_list[i].name == NULL) {
				major = i;
				break;
			}
		}
	}
	else {
		if (qt_parent_rtos->m_driver_list[major].name != (char*)NULL) {
			return -EBUSY;
		}
	}

	qt_parent_rtos->m_driver_list[major].name = (char*)name;
	qt_parent_rtos->m_driver_list[major].fops = fops;
	qt_parent_rtos->m_driver_list[major].major = major;

	return major;
}

int register_blkdev(unsigned int major, const char *name, struct file_operations *fops) {
	if (major >= MAX_DEV) {
		return -EINVAL;
	}

	// If major == 0, dynamic allocation
	// It looks for a free major number
	if (major == 0) {
		for (int i = 0; i < MAX_DEV; i++) {
			if (qt_parent_rtos->m_driver_list[i].name == NULL) {
				major = i;
				break;
			}
		}
	}
	else {
		if (qt_parent_rtos->m_driver_list[major].name != (char*)NULL) {
			return -EBUSY;
		}
	}

	qt_parent_rtos->m_driver_list[major].name = (char*)name;
	qt_parent_rtos->m_driver_list[major].fops = fops;
	qt_parent_rtos->m_driver_list[major].major = major;

	return major;
}

//unregister_*dev()
int unregister_chrdev(unsigned int major, const char *name) {
	if ( 	(major >= MAX_DEV) ||
			(qt_parent_rtos->m_driver_list[major].fops == NULL) ||
			(qt_parent_rtos->m_driver_list[major].name != name) )
	{
		return -EINVAL;
	}

	qt_parent_rtos->m_driver_list[major].name = NULL;
	qt_parent_rtos->m_driver_list[major].fops = NULL;

	return 0;
}

int unregister_blkdev(unsigned int major, const char *name) {
	if (	(major >= MAX_DEV) ||
			(qt_parent_rtos->m_driver_list[major].fops == NULL) ||
			(qt_parent_rtos->m_driver_list[major].name != name) )
	{
		return -EINVAL;
	}

	qt_parent_rtos->m_driver_list[major].name = NULL;
	qt_parent_rtos->m_driver_list[major].fops = NULL;

	return 0;
}

//sys_get*()       	System calls used to get information regarding the process, user, or group.
long sys_getpid(void) {
	return qt_parent_rtos->POSIX->uc_getpid();
}

long sys_getuid(void) {
	return qt_parent_rtos->POSIX->uc_getuid();
}

long sys_getgid(void) {
	return qt_parent_rtos->POSIX->uc_getgid();
}

long sys_geteuid(void) {
	return qt_parent_rtos->POSIX->uc_geteuid();
}

long sys_getegid(void) {
	return qt_parent_rtos->POSIX->uc_getegid();
}

long sys_getppid(void) {
	return qt_parent_rtos->POSIX->uc_getppid();
}

long sys_getpgrp(void) {
	return qt_parent_rtos->POSIX->uc_getpgrp();
}

/**
 * \brief Checks if the I/O port region is available for allocation
 *
 * \param from initial port to check
 * \param extent number of ports to check
 * \return 0 if the range is available otherwise -EBUSY
 */
int check_region(unsigned long from, unsigned long extent) {
	resource *elem;

	// change from port number to address number
	UC_cpu_class *cpu = qt_parent_rtos->get_current_cpu();
	// FIXME
// 	from = cpu->m_port->get_addr_from_port(from);

	for (int i = 0; i < qt_parent_rtos->port_resource_list.size(); i++) {
		elem = qt_parent_rtos->port_resource_list[i];
		if ( (elem->start <= from) && (elem->end >= from) ) {
			return -EBUSY;
		}
		if ( (elem->start <= from+extent-1) && (elem->end >= from+extent-1) ) {
			return -EBUSY;
		}
		if ( (elem->start > from) && (elem->end < from+extent-1) ) {
			return -EBUSY;
		}
	}

	return 0;
}

/**
 * \brief Requests the I/O port region
 *
 * \param from initial port to request
 * \param extent number of ports to request
 * \param name driver's name that request the region
 * \return a pointer to the resource structure if success NULL if failure
 */
struct resource *request_region(unsigned long from, unsigned long extent, const char *name) {
	if (check_region(from, extent) == 0) {
		// change from port number to address number
		UC_cpu_class *cpu = qt_parent_rtos->get_current_cpu();
		//FIXME
// 		from = cpu->m_port->get_addr_from_port(from);

		resource *elem = new resource;
		elem->name = name;
		elem->start = from;
		elem->end = from + extent - 1;
		elem->flags = 0;
		elem->parent = NULL;
		elem->sibling = NULL;
		elem->child = NULL;
		qt_parent_rtos->port_resource_list.push_back(elem);

		for (int i = 0; i < MAX_DEV; i++) {
			if (qt_parent_rtos->m_driver_list[i].name == NULL) {
				continue;
			}

			if (qt_parent_rtos->m_driver_list[i].name != NULL && !strcmp(qt_parent_rtos->m_driver_list[i].name, name)) {
				qt_parent_rtos->m_driver_list[i].start_addr = from;
				qt_parent_rtos->m_driver_list[i].end_addr = from+extent-1;
				return elem;
			}
		}

		return elem;
	}
	else {
		return NULL;
	}
}

/**
 * \brief Releases the I/O port region
 *
 * \param from initial port to release
 * \param extent number of ports to release
 */
void release_region(unsigned long from, unsigned long extent) {
	resource *elem;

	// change from port number to address number
	UC_cpu_class *cpu = qt_parent_rtos->get_current_cpu();
	// FIXME
// 	from = cpu->m_port->get_addr_from_port(from);

	for (int i = 0; i < qt_parent_rtos->port_resource_list.size(); i++) {
		elem = qt_parent_rtos->port_resource_list[i];
		if ( (elem->start == from) && (elem->end == from + extent - 1) ) {
			qt_parent_rtos->port_resource_list.erase(qt_parent_rtos->port_resource_list.begin() + i);
			delete elem;
			return;
		}
	}
}

/**
 * \brief Checks if the I/O memory region is available for allocation
 *
 * \param start initial address to check
 * \param extent number of addresses to check
 * \return 0 if the range is available otherwise -EBUSY
 */
int check_mem_region (unsigned long start, unsigned long extent) {
	resource *elem;

	for (int i = 0; i < qt_parent_rtos->mem_resource_list.size(); i++) {
		elem = qt_parent_rtos->mem_resource_list[i];
		if ( (elem->start <= start) && (elem->end >= start) ) {
			return -EBUSY;
		}
		if ( (elem->start <= start+extent-1) && (elem->end >= start+extent-1) ) {
			return -EBUSY;
		}
		if ( (elem->start > start) && (elem->end < start+extent-1) ) {
			return -EBUSY;
		}
	}

	return 0;
}

/**
 * \brief Requests the I/O memory region
 *
 * \param start initial address to request
 * \param extent number of addresses to request
 * \param name driver's name that request the region
 * \return a pointer to the resource structure if success NULL if failure
 */
struct resource *request_mem_region (unsigned long start, unsigned long extent, const char *name) {
	if (check_mem_region(start, extent) == 0) {
		resource *elem = new resource;
		elem->name = name;
		elem->start = start;
		elem->end = start+extent-1;
		elem->flags = 0;
		elem->parent = NULL;
		elem->sibling = NULL;
		elem->child = NULL;
		qt_parent_rtos->mem_resource_list.push_back(elem);
		return elem;
	}
	else {
		return NULL;
	}
}

/**
 * \brief Releases the I/O memory region
 *
 * \param from initial address to release
 * \param extent number of addresses to release
 */
void release_mem_region (unsigned long start, unsigned long extent) {
	resource *elem;

	for (int i = 0; i < qt_parent_rtos->mem_resource_list.size(); i++) {
		elem = qt_parent_rtos->mem_resource_list[i];
		if ( (elem->start == start) && (elem->end == start+extent-1) ) {
			qt_parent_rtos->mem_resource_list.erase(qt_parent_rtos->mem_resource_list.begin() + i);
			delete elem;
			return;
		}
	}
}

int uc_mknod(char *name, int flags, dev_t dev) {
	uc_device_t *device;
	file_operations *fops = NULL;

	int major, minor;
	major = dev >> 8;
	minor = dev & 0xFF;
	//dev_t dev = (major<<8) | minor;
	timespec current_time;
	clock_gettime(CLOCK_REALTIME, &current_time);
	struct inode i_node = {dev, INODE_MODE_777, 1, 0, 0, 0, 0, current_time, current_time, current_time};

	if (major == 10) {	// miscdevice
		struct uc_driver_t *driver;
		for (int i = 0; i < qt_parent_rtos->m_miscdriver_list.size(); i++) {
			driver = qt_parent_rtos->m_miscdriver_list[i];
			if (minor == driver->minor) {
				fops = qt_parent_rtos->m_miscdriver_list[i]->fops;
				break;
			}
		}
	}
	else {
		fops = qt_parent_rtos->m_driver_list[major].fops;
	}

	device = new uc_device_t;
	device->path = NULL;
	if(name!=NULL) {
		device->path = (char*)malloc(strlen(name)+1);
		strcpy(device->path,name);
	}
	device->major = major;
	device->minor = minor;
	device->i_node = i_node;


	device->dev_file.f_op = fops;
	device->dev_file.name = device->path;
	device->dev_file.f_count = 0;
	device->dev_file.f_flags = O_RDWR;
	device->dev_file.f_mode = O_RDWR;
	device->dev_file.f_pos = 0;
//	device->dev_file.f_uid = 0;
//	device->dev_file.f_gid = 0;
//	device->dev_file._maxcount = 1;
	device->dev_file.private_data = NULL;

	qt_parent_rtos->assign_file_descriptor(&device->dev_file);
	device->fd = device->dev_file.fd;

	qt_parent_rtos->m_dev_list.push_back(device);
	return 0;
}

/**
 * \brief Request an irq signal
 *
 * Requests an IRQ from the kernel, and, if successful,
 * installs an IRQ interrupt handler.
 * \param irq number of the interruption
 * \param handler pointer to the handler to install
 * \param irqflags run a fast interruption if true and a slow one if false
 * \param devname name of the device
 * \param dev_id device identification
 * \return 0 if successful
 *        -1 if device not found
 *        -EINVAL if out of bounds or no handler
 *        -EBUSY if a handler is already installed for irq
 */
int request_irq(unsigned int irq, irqreturn_t (*handler)(int, void *, struct pt_regs *), unsigned long irqflags, const char * devname, void *dev_id) {

	if (irq > 15 || handler == NULL) {
		return -EINVAL;
	}

	if (qt_parent_rtos->m_idt[irq].m_handler != NULL) {
		return -EBUSY;
	}

	pt_regs *pt_regs_default = new pt_regs;
	qt_parent_rtos->m_idt[irq].m_handler = handler;
	qt_parent_rtos->m_idt[irq].m_args = dev_id;
	qt_parent_rtos->m_idt[irq].m_regs = pt_regs_default;
	qt_parent_rtos->m_idt[irq].m_irq = irq;
	qt_parent_rtos->m_idt[irq].m_cpu = qt_parent_rtos->get_current_cpu();
	qt_parent_rtos->m_idt[irq].m_flag = (irqflags > 0);
	for (int i = 0; i < MAX_DEV; i++) {
		if (qt_parent_rtos->m_driver_list[i].name != NULL && !strcmp(qt_parent_rtos->m_driver_list[i].name, devname)) {
			qt_parent_rtos->m_driver_list[i].irq_channel = irq;
			return 0;
		}
	}
	return -1; //invalid device
}

/**
 * \brief Frees an IRQ previously acquired with request_irq()
 */
void free_irq(unsigned int irq, void *dev_id) {
	qt_parent_rtos->m_idt[irq].m_handler = NULL;
	for (int i = 0; i < MAX_DEV; i++) {
		if (qt_parent_rtos->m_driver_list[i].irq_channel == irq) {
			qt_parent_rtos->m_driver_list[i].irq_channel = -1;
			return;
		}
	}
}

/**
 * \brief Requests a DMA channel
 *
 * \param channel The number of the channel up to MAX_DMA_CHANNELS
 * \param name The name of the device to link the DMA channel
 * \return 0 if success, -1 if fails
 */
int request_dma(unsigned int channel, const char *name) {
	int driver = -1;

	for (int i = 0; i < MAX_DEV; i++) {
		if (qt_parent_rtos->m_driver_list[i].dma_channel == channel) {
			// fails, channel already assigned
			return -1;
		}

		if (qt_parent_rtos->m_driver_list[i].name == name) {
			if (qt_parent_rtos->m_driver_list[i].dma_channel == -1) {
				driver = i;
			}
		}
	}

	if (driver == -1) {
		// fails, nothing found
		return -1;
	} else {
		// success
		qt_parent_rtos->m_driver_list[driver].dma_channel = channel;
		return 0;
	}
}

/**
 * \brief Frees a DMA channel
 *
 * \param channel The number of the channel to free
 */
void free_dma(unsigned int channel) {
	for (int i = 0; i < MAX_DEV; i++) {
		if (qt_parent_rtos->m_driver_list[i].dma_channel == channel) {
			qt_parent_rtos->m_driver_list[i].dma_channel = -1;
			return;
		}
	}
}


/**
 * \brief Saves the current IRQ state into flags and disable current processor's interruptions
 */
unsigned long _uc_local_irq_save() {
	cli();
	return 0;	//no flags returned
}

void uc_local_irq_restore(unsigned long flags) {
	sti();
}

void uc_preempt_disable() {
//
// FIXME (las interrupciones ya deben estar capadas)
//
}

void uc_preempt_enable() {
}

void _uc_raw_spin_lock_flags(uc_pthread_spinlock_t *lock, unsigned long flags) {
	// flags are not used commonly
	uc_pthread_spin_lock(lock);
}

void _uc_raw_spin_unlock(uc_pthread_spinlock_t *lock) {
	uc_pthread_spin_unlock(lock);
}

void __uc_release(uc_pthread_spinlock_t *lock) {
}

unsigned long _uc_spin_lock_irqsave(uc_pthread_spinlock_t *lock) {
	unsigned long flags = 0;

	uc_local_irq_save(flags);
	uc_preempt_disable();
	_uc_raw_spin_lock_flags(lock, flags);
	return flags;
}

void uc_spin_unlock_irqrestore(uc_pthread_spinlock_t *lock, unsigned long flags) {
	_uc_raw_spin_unlock(lock);
	uc_local_irq_restore(flags);
	uc_preempt_enable();
	__uc_release(lock);
}

/**
 * \brief Prevents interrupts from being acknowledged
 */
void cli() {
	qt_parent_rtos->get_current_cpu()->m_irq_enabled = false;
}

/**
 * \brief Allows interrupts to be acknowledged
 */
void sti() {
	qt_parent_rtos->get_current_cpu()->m_irq_enabled = true;
}
