//==========================================================================
//	uc_microcos.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mar ene 29 2008
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

#include <fcntl.h>
#include <errno.h>

#include "rtos/api/ucos/uc_microcos.h"
#include "rtos/kernel/uc_process_class.h"
#include "rtos/kernel/uc_rtos_class.h"
#include "rtos/kernel/uc_os_defines.h"
#include "rtos/api/posix/uc_mqueue.h"

extern __thread UC_process_class *qt_parent_process;
extern __thread UC_rtos_class *qt_parent_rtos;


UC_microcos_class::UC_microcos_class(UC_rtos_class *rtos)
{
	m_parent_rtos = rtos;
}

// Miscellaneous
void   UC_microcos_class::OSInit(void) {
	// Nothing to do
}

void   UC_microcos_class::OSIntEnter(void) {
	qt_parent_rtos->OSIntNesting++;
}

void   UC_microcos_class::OSIntExit(void) {
	qt_parent_rtos->OSIntNesting--;
}

void   UC_microcos_class::OSSchedLock(void) {
	qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread->m_attribs.schedparam.sched_priority += 100;
}

void   UC_microcos_class::OSSchedUnlock(void) {
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	if (thread->m_attribs.schedparam.sched_priority > 100) {
		thread->m_attribs.schedparam.sched_priority -= 100;
	}
}

void   UC_microcos_class::OSStart(void) {
	qt_parent_rtos->MicroCOS_started = true;
	qt_parent_rtos->POSIX->uc_pthread_mutex_unlock(&qt_parent_rtos->MicroCOS_started_mutex);
}

void   UC_microcos_class::OSStatInit(void) {
	// Nothing to do
}

INT16U UC_microcos_class::OSVersion(void) {
	return 285;
}

// No documentation
/*
INT8U  OSEventNameGet(OS_EVENT *pevent, INT8U *pname, INT8U *perr) {
}

void   OSEventNameSet(OS_EVENT *pevent, INT8U *pname, INT8U *perr) {
}
*/

// Gets the thread id from a given priority
uc_pthread_t UC_microcos_class::get_tid_by_prio(int prio) {
	vector<UC_process_class *>::iterator it_process;
	vector<UC_task_class *>::iterator it_task;
	vector<UC_thread_class *>::iterator it_thread;

	for (it_process = qt_parent_rtos->m_process_list.begin(); it_process != qt_parent_rtos->m_process_list.end(); it_process++) {
		for (it_task = (*it_process)->m_task_list.begin(); it_task != (*it_process)->m_task_list.end(); it_task++) {
			for (it_thread = (*it_task)->m_thread_list.begin(); it_thread != (*it_task)->m_thread_list.end(); it_thread++) {
				if ((*it_thread)->m_attribs.schedparam.sched_priority == OS_LOWEST_PRIO - prio) {
					return (*it_thread)->m_tid;
				}
			}
		}
	}

	return -1;
}

// Event flags management

/*!
 * \brief This function blocks the actual thread until the flags condition is satisfied
 * \param pgrp The flags group
 * \param flags A pointer to the flags to satisfy. This will return the flag that has satisfied the condition
 * \param wait_type The wait type
 * \param timeout The timeout measured in clock ticks
 * \return Returns if the task has been awoken by the flags condition (false) or by the timeout (true)
 */
bool UC_microcos_class::uc_FlagBlock(OS_FLAG_GRP *pgrp, OS_FLAGS *flags, INT8U wait_type, INT16U timeout) {
	// current thread
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	// new thread entry to block
	struct MicroCOS_flags blocking_flags;
	blocking_flags.thread = thread;
	blocking_flags.wait_type = wait_type;
	blocking_flags.flags = *flags;

	// adding the entry
	((vector<MicroCOS_flags *> *)pgrp->OSFlagWaitList)->push_back(&blocking_flags);

	if (timeout == 0) {
		// blocking
		thread->block();
	}
	else {
		// timeout: number of tick timer
		struct timespec ts;
		qt_parent_rtos->POSIX->uc_clock_gettime(CLOCK_REALTIME, &ts);
		ts = ts + UC_tic_timer_class::tics_2_timespec(timeout);

		// blocking
		thread->block(&ts);

		// the thread has been awoken
		if (thread->m_timeout_expired) {
			*flags = (OS_FLAGS)0;
			return true;
		}
	}

	*flags = blocking_flags.flags;
	return false;
}

/*!
 * \brief This function unblocks the threads in the flag queue if its condition is satisfied
 * \param pgrp The flags group
 */
void UC_microcos_class::uc_FlagUnblock(OS_FLAG_GRP *pgrp) {
	MicroCOS_flags *block_elem;
	OS_FLAGS flags = pgrp->OSFlagFlags;
	OS_FLAGS flags_ready;

	vector<MicroCOS_flags *> *blocked_thread_list = ((vector<MicroCOS_flags *> *)pgrp->OSFlagWaitList);
	for (int i = 0; i < blocked_thread_list->size(); i++) {
		block_elem = (*blocked_thread_list)[i];
		switch (block_elem->wait_type) {
			case OS_FLAG_WAIT_SET_ALL:
				flags_ready = block_elem->flags & flags;
				if (flags_ready == block_elem->flags) {
					block_elem->thread->MicroCOS_flags_ready = flags_ready;
					block_elem->thread->unblock();
					blocked_thread_list->erase(blocked_thread_list->begin() + i);
					i--;
				}
				break;

			case OS_FLAG_WAIT_SET_ANY:
				flags_ready = block_elem->flags & flags;
				if (flags_ready != (OS_FLAGS)0) {
					block_elem->thread->MicroCOS_flags_ready = flags_ready;
					block_elem->thread->unblock();
					blocked_thread_list->erase(blocked_thread_list->begin() + i);
					i--;
				}
				break;
			case OS_FLAG_WAIT_CLR_ALL:
				flags_ready = block_elem->flags & ~flags;
				if (flags_ready == block_elem->flags) {
					block_elem->thread->MicroCOS_flags_ready = flags_ready;
					block_elem->thread->unblock();
					blocked_thread_list->erase(blocked_thread_list->begin() + i);
					i--;
				}
				break;
			case OS_FLAG_WAIT_CLR_ANY:
				flags_ready = block_elem->flags & ~flags;
				if (flags_ready != (OS_FLAGS)0) {
					block_elem->thread->MicroCOS_flags_ready = flags_ready;
					block_elem->thread->unblock();
					blocked_thread_list->erase(blocked_thread_list->begin() + i);
					i--;
				}
				break;
			default:
				break;
		}
	}
}

OS_FLAGS UC_microcos_class::OSFlagAccept(OS_FLAG_GRP *pgrp, OS_FLAGS flags, INT8U wait_type, INT8U *perr) {
	// No event flag group
	if (pgrp == NULL) {
		*perr = OS_FLAG_INVALID_PGRP;
		return (OS_FLAGS)0;
	}

	// It is not an event flag group
	if (pgrp->OSFlagType != OS_EVENT_TYPE_FLAG) {
		*perr = OS_ERR_EVENT_TYPE;
		return (OS_FLAGS)0;
	}

	OS_FLAGS flags_ready;

	// It checks if the OS_FLAG_CONSUME is set and eliminate it
	bool consume = false;
	if (wait_type & OS_FLAG_CONSUME) {
		wait_type &= ~OS_FLAG_CONSUME;
		consume = true;
	} else {
		consume = false;
	}

	switch (wait_type) {
		case OS_FLAG_WAIT_SET_ALL:
			flags_ready = pgrp->OSFlagFlags & flags;
			if (flags_ready == flags) {
				if (consume == true) {
					pgrp->OSFlagFlags &= ~flags_ready;
				}
				*perr = OS_NO_ERR;
				return pgrp->OSFlagFlags;
			} else {
				*perr = OS_FLAG_ERR_NOT_RDY;
				return ((OS_FLAGS)0);
			}
			break;

		case OS_FLAG_WAIT_SET_ANY:
			flags_ready = pgrp->OSFlagFlags & flags;
			if (flags_ready != (OS_FLAGS)0) {
				if (consume == true) {
					pgrp->OSFlagFlags &= ~flags_ready;
				}
				*perr = OS_NO_ERR;
				return pgrp->OSFlagFlags;
			} else {
				*perr = OS_FLAG_ERR_NOT_RDY;
				return ((OS_FLAGS)0);
			}
			break;
		case OS_FLAG_WAIT_CLR_ALL:
			flags_ready = ~pgrp->OSFlagFlags & flags;
			if (flags_ready == flags) {
				if (consume == true) {
					pgrp->OSFlagFlags |= flags_ready;
				}
				*perr = OS_NO_ERR;
				return pgrp->OSFlagFlags;
			} else {
				*perr = OS_FLAG_ERR_NOT_RDY;
				return ((OS_FLAGS)0);
			}
			break;
		case OS_FLAG_WAIT_CLR_ANY:
			flags_ready = ~pgrp->OSFlagFlags & flags;
			if (flags_ready != (OS_FLAGS)0) {
				if (consume == true) {
					pgrp->OSFlagFlags |= ~flags_ready;
				}
				*perr = OS_NO_ERR;
				return pgrp->OSFlagFlags;
			} else {
				*perr = OS_FLAG_ERR_NOT_RDY;
				return ((OS_FLAGS)0);
			}
			break;
		default:
			*perr = OS_FLAG_ERR_WAIT_TYPE;
			return ((OS_FLAGS)0);
	}
}

// TODO: OS_ERR_CREATE_ISR
OS_FLAG_GRP *UC_microcos_class::OSFlagCreate(OS_FLAGS flags, INT8U *perr) {
	OS_FLAG_GRP *flag_grp = new OS_FLAG_GRP;
	flag_grp->OSFlagType = OS_EVENT_TYPE_FLAG;
	flag_grp->OSFlagWaitList = new vector<MicroCOS_flags *>;
	flag_grp->OSFlagFlags = flags;

	*perr = OS_NO_ERR;
	return flag_grp;
}

OS_FLAG_GRP *UC_microcos_class::OSFlagDel(OS_FLAG_GRP *pgrp, INT8U opt, INT8U *perr) {
	// No event flag group
	if (pgrp == NULL) {
		*perr = OS_FLAG_INVALID_PGRP;
		return pgrp;
	}

	// It is not an event flag group
	if (pgrp->OSFlagType != OS_EVENT_TYPE_FLAG) {
		*perr = OS_ERR_EVENT_TYPE;
		return pgrp;
	}

	// When to delete the queue
	if (opt == OS_DEL_NO_PEND) {
		// Tasks waiting
		if (((vector<MicroCOS_flags *> *)pgrp->OSFlagWaitList)->size() > 0) {
			*perr = OS_ERR_TASK_WAITING;
			return pgrp;
		} else {
			if (pgrp->OSFlagWaitList != NULL) {
				delete (vector<MicroCOS_flags *> *)pgrp->OSFlagWaitList;
			}
			delete pgrp;

			*perr = OS_NO_ERR;
			return NULL;
		}
	} else if (opt == OS_DEL_ALWAYS) {
		// TODO: Mover todos los threads que esperan en la cola a Ready
		if (pgrp->OSFlagWaitList != NULL) {
			delete (vector<MicroCOS_flags *> *)pgrp->OSFlagWaitList;
		}
		delete pgrp;

		*perr = OS_NO_ERR;
		return NULL;
	} else {
		*perr = OS_ERR_INVALID_OPT;
		return pgrp;
	}
}

INT8U UC_microcos_class::OSFlagNameGet(OS_FLAG_GRP *pgrp, INT8U *pname, INT8U *perr) {
	// No event flag group
	if (pgrp == NULL) {
		*perr = OS_FLAG_INVALID_PGRP;
		return 0;
	}

	// It is not an event flag group
	if (pgrp->OSFlagType != OS_EVENT_TYPE_FLAG) {
		*perr = OS_ERR_EVENT_TYPE;
		return 0;
	}

	// No name
	if (pname == NULL) {
		*perr = OS_ERR_PNAME_NULL;
		return 0;
	}

	strncpy((char *)pname, (char *)pgrp->OSFlagName, OS_FLAG_NAME_SIZE);

	return strlen((char *)pname);
}

void  UC_microcos_class::OSFlagNameSet(OS_FLAG_GRP *pgrp, INT8U *pname, INT8U *perr) {
	// No event flag group
	if (pgrp == NULL) {
		*perr = OS_FLAG_INVALID_PGRP;
		return;
	}

	// It is not an event flag group
	if (pgrp->OSFlagType != OS_EVENT_TYPE_FLAG) {
		*perr = OS_ERR_EVENT_TYPE;
		return;
	}

	// No name
	if (pname == NULL) {
		*perr = OS_ERR_PNAME_NULL;
		return;
	}

	strncpy((char *)pgrp->OSFlagName, (char *)pname, OS_FLAG_NAME_SIZE);
	*perr = OS_NO_ERR;
}

OS_FLAGS UC_microcos_class::OSFlagPend(OS_FLAG_GRP *pgrp, OS_FLAGS flags, INT8U wait_type, INT16U timeout, INT8U *perr) {
	// No event flag group
	if (pgrp == NULL) {
		*perr = OS_FLAG_INVALID_PGRP;
		return (OS_FLAGS)0;
	}

	// It is not an event flag group
	if (pgrp->OSFlagType != OS_EVENT_TYPE_FLAG) {
		*perr = OS_ERR_EVENT_TYPE;
		return (OS_FLAGS)0;
	}

	OS_FLAGS flags_ready;
	int block_timeout = false;

	// It checks if the OS_FLAG_CONSUME is set and eliminate it
	bool consume = false;
	if (wait_type & OS_FLAG_CONSUME) {
		wait_type &= ~OS_FLAG_CONSUME;
		consume = true;
	} else {
		consume = false;
	}

	switch (wait_type) {
		case OS_FLAG_WAIT_SET_ALL:
			flags_ready = pgrp->OSFlagFlags & flags;
			if (flags_ready == flags) {
				if (consume == true) {
					pgrp->OSFlagFlags &= ~flags_ready;
				}
				*perr = OS_NO_ERR;
				return flags_ready;
			} else {
				block_timeout = uc_FlagBlock(pgrp, &flags, wait_type, timeout);
			}
			break;

		case OS_FLAG_WAIT_SET_ANY:
			flags_ready = pgrp->OSFlagFlags & flags;
			if (flags_ready != (OS_FLAGS)0) {
				if (consume == true) {
					pgrp->OSFlagFlags &= ~flags_ready;
				}
				*perr = OS_NO_ERR;
				return flags_ready;
			} else {
				block_timeout = uc_FlagBlock(pgrp, &flags, wait_type, timeout);
			}
			break;
		case OS_FLAG_WAIT_CLR_ALL:
			flags_ready = ~pgrp->OSFlagFlags & flags;
			if (flags_ready == flags) {
				if (consume == true) {
					pgrp->OSFlagFlags |= flags_ready;
				}
				*perr = OS_NO_ERR;
				return flags_ready;
			} else {
				block_timeout = uc_FlagBlock(pgrp, &flags, wait_type, timeout);
			}
			break;
		case OS_FLAG_WAIT_CLR_ANY:
			flags_ready = ~pgrp->OSFlagFlags & flags;
			if (flags_ready != (OS_FLAGS)0) {
				if (consume == true) {
					pgrp->OSFlagFlags |= ~flags_ready;
				}
				*perr = OS_NO_ERR;
				return flags_ready;
			} else {
				block_timeout = uc_FlagBlock(pgrp, &flags, wait_type, timeout);
			}
			break;
		default:
			*perr = OS_FLAG_ERR_WAIT_TYPE;
			return ((OS_FLAGS)0);
	}

	if (block_timeout) {
		*perr = OS_TIMEOUT;
		return ((OS_FLAGS)0);
	} else {
		if (consume == true) {
			switch (wait_type) {
				case OS_FLAG_WAIT_SET_ALL:
				case OS_FLAG_WAIT_SET_ANY:
					pgrp->OSFlagFlags &= ~flags;
					break;
				case OS_FLAG_WAIT_CLR_ALL:
				case OS_FLAG_WAIT_CLR_ANY:
					pgrp->OSFlagFlags |= flags;
					break;
			}
		}
		return qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread->MicroCOS_flags_ready;
	}
}

OS_FLAGS UC_microcos_class::OSFlagPendGetFlagsRdy(void) {
	return qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread->MicroCOS_flags_ready;
}

OS_FLAGS UC_microcos_class::OSFlagPost(OS_FLAG_GRP *pgrp, OS_FLAGS flags, INT8U opt, INT8U *perr) {
	// No event flag group
	if (pgrp == NULL) {
		*perr = OS_FLAG_INVALID_PGRP;
		return (OS_FLAGS)0;
	}

	// It is not an event flag group
	if (pgrp->OSFlagType != OS_EVENT_TYPE_FLAG) {
		*perr = OS_ERR_EVENT_TYPE;
		return (OS_FLAGS)0;
	}

	switch (opt) {
		case OS_FLAG_SET:
			pgrp->OSFlagFlags |= flags;
			break;
		case OS_FLAG_CLR:
			pgrp->OSFlagFlags &= ~flags;
			break;
		default:
			*perr = OS_FLAG_INVALID_PGRP;
			return (OS_FLAGS)0;
	}

	uc_FlagUnblock(pgrp);

	*perr = OS_NO_ERR;
	return pgrp->OSFlagFlags;
}

OS_FLAGS UC_microcos_class::OSFlagQuery(OS_FLAG_GRP *pgrp, INT8U *perr) {
	// No event flag group
	if (pgrp == NULL) {
		*perr = OS_FLAG_INVALID_PGRP;
		return (OS_FLAGS)0;
	}

	// It is not an event flag group
	if (pgrp->OSFlagType != OS_EVENT_TYPE_FLAG) {
		*perr = OS_ERR_EVENT_TYPE;
		return (OS_FLAGS)0;
	}

	*perr = OS_NO_ERR;
	return pgrp->OSFlagFlags;
}


// Message mailbox management
void *UC_microcos_class::OSMboxAccept(OS_EVENT *pevent) {
	// Get the number of messages in the queue
	uc_mqd_t *mqueue = (uc_mqd_t *)(pevent->OSEventPtr);
	uc_mqd_class *mq = qt_parent_rtos->find_mqd(*mqueue);
	int num_elements = mq->attr.mq_curmsgs;

	if (num_elements == 0) {
		return NULL;
	}

	// Get and return the message
	// pointer-sized message
	char *message = new char[4];
	uc_mq_receive(*mqueue, message, 4, NULL);

	return (void *)message;
}

OS_EVENT *UC_microcos_class::OSMboxCreate(void *pmsg) {
	struct uc_mq_attr *attr = new uc_mq_attr();
	uc_mqd_t *mqueue = new uc_mqd_t();

	// Creating the attributes and the name
	//attr->mq_flags = O_NONBLOCK;
	attr->mq_maxmsg = 1;
	attr->mq_msgsize = 4;
	char *name = new char[13];
	strcpy(name, "/microc_mbox");

	// Opening the queue
	*mqueue = uc_mq_open(name, O_CREAT | O_RDWR, S_IRUSR|S_IWUSR, attr);

	// No queue
	if (*mqueue == -1) {
		delete [] name;
		delete(mqueue);
		return NULL;
	}

	// The structure to return
	OS_EVENT *event = new os_event();
	event->OSEventType = OS_EVENT_TYPE_MBOX;
	event->OSEventPtr = (void*)mqueue;
	strncpy((char*)event->OSEventName, name, OS_EVENT_NAME_SIZE);

	//    INT16U   OSEventGrp;                     /* Group corresponding to tasks waiting for event to occur */
	//    INT16U   OSEventTbl[OS_EVENT_TBL_SIZE];  /* List of tasks waiting for event to occur                */

	if (pmsg != NULL) {
		OSMboxPost(event, pmsg);
	}

	delete [] name;

	return event;
}

OS_EVENT *UC_microcos_class::OSMboxDel(OS_EVENT *pevent, INT8U opt, INT8U *perr) {
	// No queue
	if (pevent == NULL) {
		*perr = OS_ERR_PEVENT_NULL;
		return pevent;
	}

	// It is not a queue
	if (pevent->OSEventType != OS_EVENT_TYPE_MBOX) {
		*perr = OS_ERR_EVENT_TYPE;
		return pevent;
	}

	uc_mqd_t *mqueue = (uc_mqd_t *)(pevent->OSEventPtr);
	uc_mqd_class *mq = qt_parent_rtos->find_mqd(*mqueue);

	// When to delete the queue
	if (opt == OS_DEL_NO_PEND) {
		// Tasks waiting
		if (mq->wr_blocked_threads.size() > 0) {
			*perr = OS_ERR_TASK_WAITING;
			return pevent;
		} else {
			uc_mq_close(*mqueue);

			*perr = OS_NO_ERR;
			return NULL;
		}
	} else if (opt == OS_DEL_ALWAYS) {
		// TODO: Mover todos los threads que esperan en la cola a Ready
		uc_mq_close(*mqueue);

		*perr = OS_NO_ERR;
		return NULL;
	} else {
		*perr = OS_ERR_INVALID_OPT;
		return pevent;
	}
}

void *UC_microcos_class::OSMboxPend(OS_EVENT *pevent, INT16U timeout, INT8U *perr) {
	// No queue
	if (pevent == NULL) {
		*perr = OS_ERR_PEVENT_NULL;
		return pevent;
	}

	// It is not a queue
	if (pevent->OSEventType != OS_EVENT_TYPE_MBOX) {
		*perr = OS_ERR_EVENT_TYPE;
		return pevent;
	}

	char *message = new char[4]; // pointer-sized
	uc_mqd_t *mqueue = (uc_mqd_t *)(pevent->OSEventPtr);
	int size;

	if (timeout == 0) {
		size = qt_parent_rtos->POSIX->uc_mq_receive(*mqueue, message, 4, NULL);
	}
	else {
		// timeout: number of tick timer
		struct timespec ts;
		qt_parent_rtos->POSIX->uc_clock_gettime(CLOCK_REALTIME, &ts);
		ts = ts + UC_tic_timer_class::tics_2_timespec(timeout);

		// get the message
		size = qt_parent_rtos->POSIX->uc_mq_timedreceive(*mqueue, message, 4, NULL, &ts);

		if (size == -1) {
			delete message;
			*perr = OS_TIMEOUT;
			return NULL;
		}
	}

	*perr = OS_NO_ERR;
	return message;
}

INT8U UC_microcos_class::OSMboxPendAbort(OS_EVENT *pevent, INT8U opt, INT8U *perr) {
}

INT8U UC_microcos_class::OSMboxPost(OS_EVENT *pevent, void *pmsg) {
	// No queue
	if (pevent == NULL) {
		return OS_ERR_PEVENT_NULL;
	}

	// It is not a queue
	if (pevent->OSEventType != OS_EVENT_TYPE_MBOX) {
		return OS_ERR_EVENT_TYPE;
	}

	// No queue
	if (pmsg == NULL) {
		return OS_ERR_POST_NULL_PTR;
	}

	uc_mqd_t *mqueue = (uc_mqd_t *)(pevent->OSEventPtr);
	uc_mqd_class *mq = qt_parent_rtos->find_mqd(*mqueue);

	// Queue full
	if (mq->attr.mq_maxmsg == mq->attr.mq_curmsgs) {
		return OS_MBOX_FULL;
	}

	// Send the pointer-sized message
	qt_parent_rtos->POSIX->uc_mq_send(*mqueue, (char *)&pmsg, 4, 0);

	return OS_NO_ERR;
}

INT8U UC_microcos_class::OSMboxPostOpt(OS_EVENT *pevent, void *pmsg, INT8U opt) {
}

INT8U UC_microcos_class::OSMboxQuery(OS_EVENT *pevent, OS_MBOX_DATA *p_mbox_data) {
	// No queue
	if (pevent == NULL) {
		return OS_ERR_PEVENT_NULL;
	}

	// It is not a queue
	if (pevent->OSEventType != OS_EVENT_TYPE_MBOX) {
		return OS_ERR_EVENT_TYPE;
	}

	uc_mqd_t *mqueue = (uc_mqd_t *)(pevent->OSEventPtr);
	uc_mqd_class *mq = qt_parent_rtos->find_mqd(*mqueue);

	struct mq_mess_element *mess;
	mess = mq->mq_mess_list[0];
	p_mbox_data->OSMsg = (void *)mess;
	// OSEventTbl and OSEventGrp not used

	return OS_NO_ERR;
}


// Memory management
OS_MEM *UC_microcos_class::OSMemCreate(void *addr, INT32U nblks, INT32U blksize, INT8U *perr) {
	if (addr == NULL) {
		*perr = OS_MEM_INVALID_ADDR;
		return NULL;
	}

	if (nblks < 2) {
		*perr = OS_MEM_INVALID_BLKS;
		return NULL;
	}

	if (blksize < sizeof(void *)) {
		*perr = OS_MEM_INVALID_SIZE;
		return NULL;
	}

	OS_MEM *mem = new OS_MEM();
	mem->OSMemAddr = addr;

	mem->OSMemFreeList = malloc(nblks * sizeof(void *));
	char *char_addr = (char *)addr;
	for (int addr_index = 0; addr_index < nblks; addr_index++) {
		((void **)mem->OSMemFreeList)[addr_index] = &char_addr[blksize * addr_index];
	}

	mem->OSMemBlkSize = blksize;
	mem->OSMemNBlks = nblks;
	mem->OSMemNFree = nblks;

	*perr = OS_NO_ERR;
	return mem;
}

void   *UC_microcos_class::OSMemGet(OS_MEM *pmem, INT8U *perr) {
	if (pmem == NULL) {
		*perr = OS_MEM_INVALID_PMEM;
		return NULL;
	}

	if (pmem->OSMemNFree == 0) {
		*perr = OS_MEM_NO_FREE_BLKS;
		return NULL;
	}

	pmem->OSMemNFree--;
	*perr = OS_NO_ERR;
	return ((void **)pmem->OSMemFreeList)[pmem->OSMemNFree];
}

INT8U   UC_microcos_class::OSMemNameGet(OS_MEM *pmem, INT8U *pname, INT8U *perr) {
	if (pmem == NULL) {
		return OS_MEM_INVALID_PMEM;
	}

	if (pname == NULL) {
		return OS_ERR_PNAME_NULL;
	}

	strcpy((char*)pname, (char*)pmem->OSMemName);

	return OS_NO_ERR;
}

void    UC_microcos_class::OSMemNameSet(OS_MEM *pmem, INT8U *pname, INT8U *perr) {
	if (pmem == NULL) {
		*perr = OS_MEM_INVALID_PMEM;
		return;
	}

	if (pname == NULL) {
		*perr = OS_ERR_PNAME_NULL;
		return;
	}

	strcpy((char*)pmem->OSMemName, (char*)pname);

	*perr = OS_NO_ERR;
}

INT8U   UC_microcos_class::OSMemPut(OS_MEM *pmem, void *pblk) {
	if (pmem == NULL) {
		return OS_MEM_INVALID_PMEM;
	}

	if (pblk == NULL) {
		return OS_MEM_INVALID_PBLK;
	}

	if (pmem->OSMemNFree == pmem->OSMemNBlks) {
		return OS_MEM_FULL;
	}

	((void **)pmem->OSMemFreeList)[pmem->OSMemNFree] = pblk;
	pmem->OSMemNFree++;
	return OS_NO_ERR;
}

INT8U   UC_microcos_class::OSMemQuery(OS_MEM *pmem, OS_MEM_DATA *p_mem_data) {
	if (pmem == NULL) {
		return OS_MEM_INVALID_PMEM;
	}

	if (p_mem_data == NULL) {
		return OS_MEM_INVALID_PDATA;
	}

	p_mem_data->OSAddr = pmem->OSMemAddr;
	p_mem_data->OSFreeList = pmem->OSMemFreeList;
	p_mem_data->OSBlkSize = pmem->OSMemBlkSize;
	p_mem_data->OSNBlks = pmem->OSMemNBlks;
	p_mem_data->OSNFree = pmem->OSMemNFree;
	p_mem_data->OSNUsed = pmem->OSMemNBlks - pmem->OSMemNFree;

	return OS_NO_ERR;
}


// Mutual exclusion semaphore management
BOOLEAN   UC_microcos_class::OSMutexAccept(OS_EVENT *pevent, INT8U *perr) {
	// No mutex
	if (pevent == NULL) {
		*perr = OS_ERR_PEVENT_NULL;
		return 0;
	}

	// It is not a mutex
	if (pevent->OSEventType != OS_EVENT_TYPE_MUTEX) {
		*perr = OS_ERR_EVENT_TYPE;
		return 0;
	}

	uc_pthread_mutex_t *mutex = (uc_pthread_mutex_t *)(pevent->OSEventPtr);

	int done = qt_parent_rtos->POSIX->uc_pthread_mutex_trylock(mutex);

	*perr = OS_NO_ERR;

	if (done == 0) {
		// the mutex is available
		return 1;
	} else {
		// the mutex is owned by other task
		return 0;
	}
}

OS_EVENT *UC_microcos_class::OSMutexCreate(INT8U prio, INT8U *perr) {
	if (prio > OS_LOWEST_PRIO) {
		*perr = OS_PRIO_INVALID;
		return NULL;
	}

	// Checks if the priority exist
	vector<UC_process_class *>::iterator it_process;
	vector<UC_task_class *>::iterator it_task;
	vector<UC_thread_class *>::iterator it_thread;

	for (it_process = qt_parent_rtos->m_process_list.begin(); it_process != qt_parent_rtos->m_process_list.end(); it_process++) {
		for (it_task = (*it_process)->m_task_list.begin(); it_task != (*it_process)->m_task_list.end(); it_task++) {
			for (it_thread = (*it_task)->m_thread_list.begin(); it_thread != (*it_task)->m_thread_list.end(); it_thread++) {
				if ((*it_thread)->m_attribs.schedparam.sched_priority == OS_LOWEST_PRIO - prio) {
					*perr = OS_PRIO_EXIST;
					return NULL;
				}
			}
		}
	}

	uc_pthread_mutex_t *mutex = new uc_pthread_mutex_t;

	// mutex attributes
	uc_pthread_mutexattr_t *mutexattr = new uc_pthread_mutexattr_t;
	mutexattr->protocol = PTHREAD_PRIO_INHERIT;
	mutexattr->pshared = PTHREAD_PROCESS_SHARED;
	mutexattr->prioceiling = OS_LOWEST_PRIO - prio;
	mutexattr->type = PTHREAD_MUTEX_DEFAULT;

	// mutex init
	qt_parent_rtos->POSIX->uc_pthread_mutex_init(mutex, mutexattr);

	// The structure to return
	struct os_event *event = new os_event();
	event->OSEventType = OS_EVENT_TYPE_MUTEX;
	event->OSEventPtr = (void*)mutex;

	*perr = OS_NO_ERR;
	return event;
}

OS_EVENT *UC_microcos_class::OSMutexDel(OS_EVENT *pevent, INT8U opt, INT8U *perr) {
	// No mutex
	if (pevent == NULL) {
		*perr = OS_ERR_PEVENT_NULL;
		return pevent;
	}

	// It is not a mutex
	if (pevent->OSEventType != OS_EVENT_TYPE_MUTEX) {
		*perr = OS_ERR_EVENT_TYPE;
		return pevent;
	}

	uc_pthread_mutex_t *mutex = (uc_pthread_mutex_t *)(pevent->OSEventPtr);
	uc_pthread_mutex_class *mutex_class = qt_parent_rtos->find_mutex(mutex->id);

	// When to delete the mutex
	if (opt == OS_DEL_NO_PEND) {
		// Tasks waiting
		if (mutex_class->empty() == 0) {
			*perr = OS_ERR_TASK_WAITING;
			return pevent;
		} else {
			qt_parent_rtos->POSIX->uc_pthread_mutex_destroy(mutex);

			*perr = OS_NO_ERR;
			return NULL;
		}
	} else if (opt == OS_DEL_ALWAYS) {
		// TODO: Mover todos los threads que esperan en la cola a Ready
		qt_parent_rtos->POSIX->uc_pthread_mutex_destroy(mutex);

		*perr = OS_NO_ERR;
		return NULL;
	} else {
		*perr = OS_ERR_INVALID_OPT;
		return pevent;
	}
}

void  UC_microcos_class::OSMutexPend(OS_EVENT *pevent, INT16U timeout, INT8U *perr) {
	// No mutex
	if (pevent == NULL) {
		*perr = OS_ERR_PEVENT_NULL;
		return;
	}

	// It is not a mutex
	if (pevent->OSEventType != OS_EVENT_TYPE_MUTEX) {
		*perr = OS_ERR_EVENT_TYPE;
		return;
	}

	uc_pthread_mutex_t *mutex = (uc_pthread_mutex_t *)(pevent->OSEventPtr);

	int done;
	if (timeout == 0) {
		done = qt_parent_rtos->POSIX->uc_pthread_mutex_lock(mutex);
	}
	else {
		// timeout: number of tick timer
		struct timespec ts;
		qt_parent_rtos->POSIX->uc_clock_gettime(CLOCK_REALTIME, &ts);
		ts = ts + UC_tic_timer_class::tics_2_timespec(timeout);

		done = qt_parent_rtos->POSIX->uc_pthread_mutex_timedlock(mutex, &ts);

		if (done == ETIMEDOUT) {
			*perr = OS_TIMEOUT;
			return;
		}
	}

	*perr = OS_NO_ERR;
}

INT8U UC_microcos_class::OSMutexPost(OS_EVENT *pevent) {
	// No mutex
	if (pevent == NULL) {
		return OS_ERR_PEVENT_NULL;
	}

	// It is not a mutex
	if (pevent->OSEventType != OS_EVENT_TYPE_MUTEX) {
		return OS_ERR_EVENT_TYPE;
	}

	uc_pthread_mutex_t *mutex = (uc_pthread_mutex_t *)(pevent->OSEventPtr);
	uc_pthread_mutex_class *mutex_class = qt_parent_rtos->find_mutex(mutex->id);

	int done = qt_parent_rtos->POSIX->uc_pthread_mutex_unlock(mutex);

	if (done == EPERM) {
		return OS_ERR_NOT_MUTEX_OWNER;
	}

	return OS_NO_ERR;
}

INT8U UC_microcos_class::OSMutexQuery(OS_EVENT *pevent, OS_MUTEX_DATA *p_mutex_data) {
	// No mutex
	if (pevent == NULL) {
		return OS_ERR_PEVENT_NULL;
	}

	// It is not a mutex
	if (pevent->OSEventType != OS_EVENT_TYPE_MUTEX) {
		return OS_ERR_EVENT_TYPE;
	}

	uc_pthread_mutex_t *mutex = (uc_pthread_mutex_t *)(pevent->OSEventPtr);
	uc_pthread_mutex_class *mutex_class = qt_parent_rtos->find_mutex(mutex->id);

	p_mutex_data->OSMutexPIP = mutex_class->channel_priority;
	p_mutex_data->OSOwnerPrio = mutex_class->get_owner()->m_priority;
	p_mutex_data->OSValue = mutex_class->empty();

	return OS_NO_ERR;
}


// Message queue management
void *UC_microcos_class::OSQAccept(OS_EVENT *pevent, INT8U *perr) {
	// No queue
	if (pevent == NULL) {
		*perr = OS_ERR_PEVENT_NULL;
		return NULL;
	}

	// It is not a queue
	if (pevent->OSEventType != OS_EVENT_TYPE_Q) {
		*perr = OS_ERR_EVENT_TYPE;
		return NULL;
	}

	// Get the number of messages in the queue
	uc_mqd_t *mqueue = (uc_mqd_t *)(pevent->OSEventPtr);
	uc_mqd_class *mq = qt_parent_rtos->find_mqd(*mqueue);
	int num_elements = mq->attr.mq_curmsgs;

	if (num_elements == 0) {
		*perr = OS_NO_ERR;
		return NULL;
	}

	// Get and return the message
	// pointer-sized message
	char *message = new char[4];
	qt_parent_rtos->POSIX->uc_mq_receive(*mqueue, message, 4, NULL);

	*perr = OS_NO_ERR;
	return (void *)message;
}

OS_EVENT *UC_microcos_class::OSQCreate(void **start, INT16U size) {
	struct uc_mq_attr *attr = new uc_mq_attr();
	uc_mqd_t *mqueue = new uc_mqd_t();

	// Creating the attributes and the name
	//attr->mq_flags = O_NONBLOCK;
	attr->mq_maxmsg = size;
	attr->mq_msgsize = 4;
	char *name = new char[15];
	strcpy(name, "/microc_mqueue");

	// Opening the queue
	*mqueue = qt_parent_rtos->POSIX->uc_mq_open(name, O_CREAT | O_RDWR, S_IRUSR|S_IWUSR, attr);

	// No queue
	if (*mqueue == -1) {
		delete [] name;
		delete(mqueue);
		return NULL;
	}

	// The structure to return
	struct os_event *event = new os_event();
	event->OSEventType = OS_EVENT_TYPE_Q;
	event->OSEventPtr = (void*)mqueue;
	strcpy((char*)event->OSEventName, (char*)name);

//    INT16U   OSEventGrp;                     /* Group corresponding to tasks waiting for event to occur */
//    INT16U   OSEventTbl[OS_EVENT_TBL_SIZE];  /* List of tasks waiting for event to occur                */

	delete [] name;

	return event;
}

// TODO: OS_ERR_DEL_ISR
OS_EVENT *UC_microcos_class::OSQDel(OS_EVENT *pevent, INT8U opt, INT8U *perr) {
	// No queue
	if (pevent == NULL) {
		*perr = OS_ERR_PEVENT_NULL;
		return pevent;
	}

	// It is not a queue
	if (pevent->OSEventType != OS_EVENT_TYPE_Q) {
		*perr = OS_ERR_EVENT_TYPE;
		return pevent;
	}

	uc_mqd_t *mqueue = (uc_mqd_t *)(pevent->OSEventPtr);
	uc_mqd_class *mq = qt_parent_rtos->find_mqd(*mqueue);

	// When to delete the queue
	if (opt == OS_DEL_NO_PEND) {
		// Tasks waiting
		if (mq->wr_blocked_threads.size() > 0) {
			*perr = OS_ERR_TASK_WAITING;
			return pevent;
		} else {
			qt_parent_rtos->POSIX->uc_mq_close(*mqueue);

			*perr = OS_NO_ERR;
			return NULL;
		}
	} else if (opt == OS_DEL_ALWAYS) {
		// TODO: Mover todos los threads que esperan en la cola a Ready
		qt_parent_rtos->POSIX->uc_mq_close(*mqueue);

		*perr = OS_NO_ERR;
		return NULL;
	} else {
		*perr = OS_ERR_INVALID_OPT;
		return pevent;
	}
}

INT8U UC_microcos_class::OSQFlush(OS_EVENT *pevent) {
	// No queue
	if (pevent == NULL) {
		return OS_ERR_PEVENT_NULL;
	}

	// It is not a queue
	if (pevent->OSEventType != OS_EVENT_TYPE_Q) {
		return OS_ERR_EVENT_TYPE;
	}

	// Cleaning the messages from the list
	uc_mqd_t *mqueue = (uc_mqd_t *)(pevent->OSEventPtr);
	uc_mqd_class *mq = qt_parent_rtos->find_mqd(*mqueue);
	mq->mq_mess_list.clear();

	return OS_NO_ERR;
}

// TODO: OS_ERR_PEND_ISR
void *UC_microcos_class::OSQPend(OS_EVENT *pevent, INT16U timeout, INT8U *perr) {
	// No queue
	if (pevent == NULL) {
		*perr = OS_ERR_PEVENT_NULL;
		return pevent;
	}

	// It is not a queue
	if (pevent->OSEventType != OS_EVENT_TYPE_Q) {
		*perr = OS_ERR_EVENT_TYPE;
		return pevent;
	}

	uc_mqd_t *mqueue = (uc_mqd_t *)(pevent->OSEventPtr);
	char *message = new char[4]; // pointer-sized

	int size;
	if (timeout == 0) {
		size = qt_parent_rtos->POSIX->uc_mq_receive(*mqueue, message, 4, NULL);
	}
	else {
		// timeout: number of tick timer
		struct timespec ts;
		qt_parent_rtos->POSIX->uc_clock_gettime(CLOCK_REALTIME, &ts);
		ts = ts + UC_tic_timer_class::tics_2_timespec(timeout);

		// get the message
		size = qt_parent_rtos->POSIX->uc_mq_timedreceive(*mqueue, message, 4, NULL, &ts);

		if (size == -1) {
			delete message;
			*perr = OS_TIMEOUT;
			return NULL;
		}
	}

	*perr = OS_NO_ERR;
	return message;
}

// No documentation
//INT8U OSQPendAbort(OS_EVENT *pevent, INT8U opt, INT8U *perr) {
//}

INT8U UC_microcos_class::OSQPost(OS_EVENT *pevent, void *pmsg) {
	// No queue
	if (pevent == NULL) {
		return OS_ERR_PEVENT_NULL;
	}

	// It is not a queue
	if (pevent->OSEventType != OS_EVENT_TYPE_Q) {
		return OS_ERR_EVENT_TYPE;
	}

	// No queue
	if (pmsg == NULL) {
		return OS_ERR_POST_NULL_PTR;
	}

	uc_mqd_t *mqueue = (uc_mqd_t *)(pevent->OSEventPtr);
	uc_mqd_class *mq = qt_parent_rtos->find_mqd(*mqueue);

	// Queue full
	if (mq->attr.mq_maxmsg == mq->attr.mq_curmsgs) {
		return OS_Q_FULL;
	}

	// Send the pointer-sized message
	qt_parent_rtos->POSIX->uc_mq_send(*mqueue, (char *)&pmsg, 4, 0);

	return OS_NO_ERR;
}

INT8U UC_microcos_class::OSQPostFront(OS_EVENT *pevent, void *pmsg) {
	// No queue
	if (pevent == NULL) {
		return OS_ERR_PEVENT_NULL;
	}

	// It is not a queue
	if (pevent->OSEventType != OS_EVENT_TYPE_Q) {
		return OS_ERR_EVENT_TYPE;
	}

	// No queue
	if (pmsg == NULL) {
		return OS_ERR_POST_NULL_PTR;
	}

	uc_mqd_t *mqueue = (uc_mqd_t *)(pevent->OSEventPtr);
	uc_mqd_class *mq = qt_parent_rtos->find_mqd(*mqueue);

	// Queue full
	if (mq->attr.mq_maxmsg == mq->attr.mq_curmsgs) {
		return OS_Q_FULL;
	}

	// Send the pointer-sized message
	qt_parent_rtos->POSIX->uc_mq_timedsend_front(*mqueue, (char *)pmsg, 4, 0, NULL);

	return OS_NO_ERR;
}

// Broadcast????
//INT8U OSQPostOpt(OS_EVENT *pevent, void *pmsg, INT8U opt) {
//}

INT8U UC_microcos_class::OSQQuery(OS_EVENT *pevent, OS_Q_DATA *p_q_data) {
	// No queue
	if (pevent == NULL) {
		return OS_ERR_PEVENT_NULL;
	}

	// It is not a queue
	if (pevent->OSEventType != OS_EVENT_TYPE_Q) {
		return OS_ERR_EVENT_TYPE;
	}

	uc_mqd_t *mqueue = (uc_mqd_t *)(pevent->OSEventPtr);
	uc_mqd_class *mq = qt_parent_rtos->find_mqd(*mqueue);

	struct mq_mess_element *mess;
	mess = mq->mq_mess_list[0];
	p_q_data->OSMsg = (void *)mess;
	p_q_data->OSNMsgs = mq->attr.mq_curmsgs;
	p_q_data->OSQSize = mq->attr.mq_maxmsg;
	// OSEventTbl and OSEventGrp not used

	return OS_NO_ERR;
}

// Semaphore management
INT16U UC_microcos_class::OSSemAccept(OS_EVENT *pevent) {
	// get the semaphore
	uc_sem_t *sem = (uc_sem_t *)(pevent->OSEventPtr);
	uc_sem_class *semaphore = qt_parent_rtos->find_sem(*sem);
	int trywait = qt_parent_rtos->POSIX->uc_sem_trywait(sem);

	if (trywait != 0) {
		return 0;
	}

	pevent->OSEventCnt = semaphore->counter;
	return (semaphore->counter + 1);
}

OS_EVENT *UC_microcos_class::OSSemCreate(INT16U cnt) {
	uc_sem_t *sem = new uc_sem_t;
	qt_parent_rtos->POSIX->uc_sem_init(sem, 1, cnt);

	OS_EVENT *event = new OS_EVENT;

	event->OSEventType = OS_EVENT_TYPE_SEM;
	// This pointer is not used by MicroCOS semaphores,
	// so here it is used to store the pointer to the POSIX semaphore
	event->OSEventPtr = (void *)sem;
	event->OSEventCnt = cnt;

	return event;
}

OS_EVENT *UC_microcos_class::OSSemDel(OS_EVENT *pevent, INT8U opt, INT8U *perr) {
	// No semaphore
	if (pevent == NULL) {
		*perr = OS_ERR_PEVENT_NULL;
		return pevent;
	}

	// It is not a semaphore
	if (pevent->OSEventType != OS_EVENT_TYPE_SEM) {
		*perr = OS_ERR_EVENT_TYPE;
		return pevent;
	}

	uc_sem_t *sem = (uc_sem_t *)(pevent->OSEventPtr);
	uc_sem_class *semaphore = qt_parent_rtos->find_sem(*sem);

	// When to delete the semaphore
	if (opt == OS_DEL_NO_PEND) {
		// Tasks waiting
		if (semaphore->open_sem.size() > 0) {
			*perr = OS_ERR_TASK_WAITING;
			return pevent;
		} else {
			qt_parent_rtos->POSIX->uc_sem_destroy(sem);

			*perr = OS_NO_ERR;
			return NULL;
		}
	} else if (opt == OS_DEL_ALWAYS) {
		// TODO: Mover todos los threads que esperan en la cola a Ready
		qt_parent_rtos->POSIX->uc_mq_close(*sem);

		*perr = OS_NO_ERR;
		return NULL;
	} else {
		*perr = OS_ERR_INVALID_OPT;
		return pevent;
	}
}

void  UC_microcos_class::OSSemPend(OS_EVENT *pevent, INT16U timeout, INT8U *perr) {
	// No semaphore
	if (pevent == NULL) {
		*perr = OS_ERR_PEVENT_NULL;
		return;
	}

	// It is not a semaphore
	if (pevent->OSEventType != OS_EVENT_TYPE_SEM) {
		*perr = OS_ERR_EVENT_TYPE;
		return;
	}

	// get the semaphore
	uc_sem_t *sem = (uc_sem_t *)(pevent->OSEventPtr);
	uc_sem_class *semaphore = qt_parent_rtos->find_sem(*sem);

	if (timeout == 0) {
		qt_parent_rtos->POSIX->uc_sem_wait(sem);
	}
	else {
		// timeout: number of tick timer
		struct timespec ts;
		qt_parent_rtos->POSIX->uc_clock_gettime(CLOCK_REALTIME, &ts);
		ts = ts + UC_tic_timer_class::tics_2_timespec(timeout);

		int timedwait = qt_parent_rtos->POSIX->uc_sem_timedwait(sem, &ts);

		if (timedwait != 0) {
			*perr = OS_TIMEOUT;
			return;
		}
	}

	pevent->OSEventCnt = semaphore->counter;
	*perr = OS_NO_ERR;
}

INT8U UC_microcos_class::OSSemPendAbort(OS_EVENT *pevent, INT8U opt, INT8U *perr) {
}

INT8U UC_microcos_class::OSSemPost(OS_EVENT *pevent) {
	// No semaphore
	if (pevent == NULL) {
		return OS_ERR_PEVENT_NULL;
	}

	// It is not a semaphore
	if (pevent->OSEventType != OS_EVENT_TYPE_SEM) {
		return OS_ERR_EVENT_TYPE;
	}

	// get the semaphore
	uc_sem_t *sem = (uc_sem_t *)(pevent->OSEventPtr);
	uc_sem_class *semaphore = qt_parent_rtos->find_sem(*sem);
	qt_parent_rtos->POSIX->uc_sem_post(sem);

	pevent->OSEventCnt = semaphore->counter;

	return OS_NO_ERR;
}

INT8U UC_microcos_class::OSSemQuery(OS_EVENT *pevent, OS_SEM_DATA *p_sem_data) {
	// No semaphore
	if (pevent == NULL) {
		return OS_ERR_PEVENT_NULL;
	}

	// It is not a semaphore
	if (pevent->OSEventType != OS_EVENT_TYPE_SEM) {
		return OS_ERR_EVENT_TYPE;
	}

	p_sem_data->OSCnt = pevent->OSEventCnt;

	return OS_NO_ERR;
}

void  UC_microcos_class::OSSemSet(OS_EVENT *pevent, INT16U cnt, INT8U *perr) {
	// No semaphore
	if (pevent == NULL) {
		*perr = OS_ERR_PEVENT_NULL;
		return;
	}

	// It is not a semaphore
	if (pevent->OSEventType != OS_EVENT_TYPE_SEM) {
		*perr = OS_ERR_EVENT_TYPE;
		return;
	}

	uc_sem_t *sem = (uc_sem_t *)(pevent->OSEventPtr);
	uc_sem_class *semaphore = qt_parent_rtos->find_sem(*sem);
	if (semaphore->open_sem.size() != 0) {
		*perr = OS_ERR_TASK_WAITING;
	}

	semaphore->counter = cnt;
	pevent->OSEventCnt = cnt;

	*perr = OS_NO_ERR;
}


// Task management
INT8U UC_microcos_class::OSTaskChangePrio(INT8U oldprio, INT8U newprio) {
	if (oldprio >= OS_LOWEST_PRIO || newprio >= OS_LOWEST_PRIO) {
		return OS_PRIO_INVALID;
	}

	UC_thread_class *change_thread;	// thread to change

	vector<UC_process_class *>::iterator it_process;
	vector<UC_task_class *>::iterator it_task;
	vector<UC_thread_class *>::iterator it_thread;

	for (it_process = qt_parent_rtos->m_process_list.begin(); it_process != qt_parent_rtos->m_process_list.end(); it_process++) {
		for (it_task = (*it_process)->m_task_list.begin(); it_task != (*it_process)->m_task_list.end(); it_task++) {
			for (it_thread = (*it_task)->m_thread_list.begin(); it_thread != (*it_task)->m_thread_list.end(); it_thread++) {
				if ((*it_thread)->m_attribs.schedparam.sched_priority == OS_LOWEST_PRIO - newprio) {
					return OS_PRIO_EXIST;
				}

				if ((*it_thread)->m_attribs.schedparam.sched_priority == OS_LOWEST_PRIO - oldprio) {
					change_thread = *it_thread;
				}
			}
		}
	}

	if (change_thread == NULL) {
		return OS_PRIO_ERR;
	}

	change_thread->m_attribs.schedparam.sched_priority = OS_LOWEST_PRIO - newprio;

	return OS_NO_ERR;
}

// Converts the MicroCOS format to the POSIX one
void * MicroCOS_start_routine(void *void_task_and_args) {
	struct MicroCOS_task_and_args* task_and_args = (struct MicroCOS_task_and_args *) void_task_and_args;

	if (qt_parent_rtos->MicroCOS_started == false) {
		qt_parent_rtos->POSIX->uc_pthread_mutex_lock(&qt_parent_rtos->MicroCOS_started_mutex);
		qt_parent_rtos->POSIX->uc_pthread_mutex_unlock(&qt_parent_rtos->MicroCOS_started_mutex);
	}

	task_and_args->task(task_and_args->args);
	return NULL;
}

INT8U UC_microcos_class::OSTaskCreate(void (*task)(void *p_arg), void *p_arg, OS_STK *ptos, INT8U prio) {
	// Checks the invalid priority
	if (prio > OS_LOWEST_PRIO) {
		return OS_PRIO_INVALID;
	}

	// Checks if the priority exist
	vector<UC_process_class *>::iterator it_process;
	vector<UC_task_class *>::iterator it_task;
	vector<UC_thread_class *>::iterator it_thread;

	for (it_process = qt_parent_rtos->m_process_list.begin(); it_process != qt_parent_rtos->m_process_list.end(); it_process++) {
		for (it_task = (*it_process)->m_task_list.begin(); it_task != (*it_process)->m_task_list.end(); it_task++) {
			for (it_thread = (*it_task)->m_thread_list.begin(); it_thread != (*it_task)->m_thread_list.end(); it_thread++) {
				if ((*it_thread)->m_attribs.schedparam.sched_priority == OS_LOWEST_PRIO - prio) {
					return OS_PRIO_EXIST;
				}
			}
		}
	}

	uc_pthread_t *threadarg = new uc_pthread_t;

	uc_pthread_attr_t *attr = new uc_pthread_attr_t;
	attr->schedparam.sched_priority = OS_LOWEST_PRIO - prio;

	struct MicroCOS_task_and_args *task_and_args = new MicroCOS_task_and_args;
	task_and_args->task = task;
	task_and_args->args = p_arg;

	char *name = new char[13];
	strcpy(name, "task_prio_");
	char *prio_str = new char[3];
	sprintf(prio_str, "%d", prio);
	strcat(name, prio_str);

	qt_parent_rtos->POSIX->uc_pthread_create(threadarg, attr, MicroCOS_start_routine, (void*)task_and_args, name);

	delete [] name;

	return OS_NO_ERR;
}

INT8U UC_microcos_class::OSTaskCreateExt(void (*task)(void *p_arg), void *p_arg, OS_STK *ptos, INT8U prio,
			INT16U id, OS_STK *pbos, INT32U stk_size, void *pext, INT16U opt) {
	// Checks the invalid priority
	if (prio > OS_LOWEST_PRIO) {
		return OS_PRIO_INVALID;
	}

	// Checks if the priority exist
	vector<UC_process_class *>::iterator it_process;
	vector<UC_task_class *>::iterator it_task;
	vector<UC_thread_class *>::iterator it_thread;

	for (it_process = qt_parent_rtos->m_process_list.begin(); it_process != qt_parent_rtos->m_process_list.end(); it_process++) {
		for (it_task = (*it_process)->m_task_list.begin(); it_task != (*it_process)->m_task_list.end(); it_task++) {
			for (it_thread = (*it_task)->m_thread_list.begin(); it_thread != (*it_task)->m_thread_list.end(); it_thread++) {
				if ((*it_thread)->m_attribs.schedparam.sched_priority == OS_LOWEST_PRIO - prio) {
					return OS_PRIO_EXIST;
				}
			}
		}
	}

	uc_pthread_t *threadarg;

	uc_pthread_attr_t *attr = new uc_pthread_attr_t;
	attr->schedparam.sched_priority = OS_LOWEST_PRIO - prio;
	attr->pthread_id = id;

	struct MicroCOS_task_and_args *task_and_args = new MicroCOS_task_and_args;
	task_and_args->task = task;
	task_and_args->args = p_arg;

	char *name = new char[13];
	strcpy(name, "task_prio_");
	char *prio_str = new char[3];
	sprintf(prio_str, "%d", prio);
	strcat(name, prio_str);

	qt_parent_rtos->POSIX->uc_pthread_create(threadarg, attr, MicroCOS_start_routine, (void*)task_and_args, name);

	delete [] name;

	return OS_NO_ERR;
}

INT8U UC_microcos_class::OSTaskDel(INT8U prio) {
	if (prio >= OS_LOWEST_PRIO) {
		return OS_PRIO_INVALID;
	}

	int tid;
	if (prio == OS_PRIO_SELF) {
		tid = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread->m_tid;
	} else {
		tid = get_tid_by_prio(prio);
		if (tid == -1) {
			return OS_TASK_DEL_ERR;
		}
	}

	qt_parent_rtos->POSIX->uc_pthread_kill(tid, SIGKILL);

	return OS_NO_ERR;
}

INT8U UC_microcos_class::OSTaskDelReq(INT8U prio) {
	if (prio >= OS_LOWEST_PRIO && prio != OS_PRIO_SELF) {
		return OS_PRIO_INVALID;
	}

	int tid;
	if (prio == OS_PRIO_SELF) {
		tid = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread->m_tid;
	} else {
		tid = get_tid_by_prio(prio);
		if (tid == -1) {
			return OS_TASK_NOT_EXIST;
		}
	}

	qt_parent_rtos->POSIX->uc_pthread_cancel(tid);

	return OS_NO_ERR;
}

INT8U UC_microcos_class::OSTaskNameGet(INT8U prio, INT8U *pname, INT8U *perr) {
	if (prio >= OS_LOWEST_PRIO || prio != OS_PRIO_SELF) {
		*perr = OS_PRIO_INVALID;
		return 0;
	}

	if (pname == NULL) {
		*perr = OS_ERR_PNAME_NULL;
		return 0;
	}

	if (qt_parent_rtos->OSIntNesting > 0) {
		*perr = OS_ERR_NAME_GET_ISR;
		return 0;
	}

	// Checks if the priority exist
	UC_thread_class *change_thread;	// thread for change the name

	vector<UC_process_class *>::iterator it_process;
	vector<UC_task_class *>::iterator it_task;
	vector<UC_thread_class *>::iterator it_thread;

	if (prio == OS_PRIO_SELF) {
		change_thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	} else {
		bool found = false;
		for (it_process = qt_parent_rtos->m_process_list.begin(); it_process != qt_parent_rtos->m_process_list.end(); it_process++) {
			for (it_task = (*it_process)->m_task_list.begin(); it_task != (*it_process)->m_task_list.end(); it_task++) {
				for (it_thread = (*it_task)->m_thread_list.begin(); it_thread != (*it_task)->m_thread_list.end(); it_thread++) {
					if ((*it_thread)->m_attribs.schedparam.sched_priority == OS_LOWEST_PRIO - prio) {
						change_thread = *it_thread;
						found = true;
						break;
					}
				}
				if (found == true) {
					break;
				}
			}
			if (found == true) {
				break;
			}
		}
	}

	if (change_thread == NULL) {
		*perr = OS_ERR_TASK_NOT_EXIST;
	}

	// The priority exist and the name is copied
	strcpy((char*)pname, change_thread->m_func_name);

	*perr = OS_NO_ERR;

	return strlen(change_thread->m_func_name);
}

void  UC_microcos_class::OSTaskNameSet(INT8U prio, INT8U *pname, INT8U *perr) {
	if (prio >= OS_LOWEST_PRIO || prio != OS_PRIO_SELF) {
		*perr = OS_PRIO_INVALID;
		return;
	}

	if (pname == NULL) {
		*perr = OS_ERR_PNAME_NULL;
		return;
	}

	if (qt_parent_rtos->OSIntNesting > 0) {
		*perr = OS_ERR_NAME_GET_ISR;
		return;
	}

	// Checks if the priority exist
	UC_thread_class *change_thread;	// thread for change the name

	vector<UC_process_class *>::iterator it_process;
	vector<UC_task_class *>::iterator it_task;
	vector<UC_thread_class *>::iterator it_thread;

	if (prio == OS_PRIO_SELF) {
		change_thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	} else {
		bool found = false;
		for (it_process = qt_parent_rtos->m_process_list.begin(); it_process != qt_parent_rtos->m_process_list.end(); it_process++) {
			for (it_task = (*it_process)->m_task_list.begin(); it_task != (*it_process)->m_task_list.end(); it_task++) {
				for (it_thread = (*it_task)->m_thread_list.begin(); it_thread != (*it_task)->m_thread_list.end(); it_thread++) {
					if ((*it_thread)->m_attribs.schedparam.sched_priority == OS_LOWEST_PRIO - prio) {
						change_thread = *it_thread;
						found = true;
						break;
					}
				}
				if (found == true) {
					break;
				}
			}
			if (found == true) {
				break;
			}
		}
	}

	if (change_thread == NULL) {
		*perr = OS_ERR_TASK_NOT_EXIST;
	}

	// The priority exist and the name is copied
	delete change_thread->m_func_name;
	change_thread->m_func_name = new char[strlen((char*)pname) + 1];
	strcpy(change_thread->m_func_name, (char*)pname);

	*perr = OS_NO_ERR;
}

INT8U UC_microcos_class::OSTaskResume(INT8U prio) {
	if (prio >= OS_LOWEST_PRIO) {
		return OS_PRIO_INVALID;
	}

	int tid = get_tid_by_prio(prio);
	if (tid == -1) {
		return OS_TASK_RESUME_PRIO;
	}

	qt_parent_rtos->POSIX->uc_pthread_kill(tid, SIGCONT);

	return OS_NO_ERR;
}

INT8U UC_microcos_class::OSTaskSuspend(INT8U prio) {
	if (prio >= OS_LOWEST_PRIO || prio != OS_PRIO_SELF) {
		return OS_PRIO_INVALID;
	}

	int tid = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread->m_tid;

	qt_parent_rtos->POSIX->uc_pthread_kill(tid, SIGSTOP);

	return OS_NO_ERR;
}

// Not using the stack structures
/*
INT8U OSTaskStkChk(INT8U prio, OS_STK_DATA *p_stk_data) {
}
*/

// Not using the OS_TCB structure
/*
INT8U OSTaskQuery(INT8U prio, OS_TCB *p_task_data) {
}
*/


// Time management
void UC_microcos_class::OSTimeDly(INT16U ticks) {
	unsigned long usecs = (unsigned long)(1e6/TICK_TIMER_HZ * ticks);
	if (usecs / 1000000 > 0) {
		qt_parent_rtos->POSIX->uc_sleep(usecs / 1000000);
	} else {
		qt_parent_rtos->POSIX->uc_usleep(usecs);
	}
}

INT8U UC_microcos_class::OSTimeDlyHMSM(INT8U hours, INT8U minutes, INT8U seconds, INT16U milli) {
	if (minutes > 59) {
		return OS_TIME_INVALID_MINUTES;
	}

	if (seconds > 59) {
		return OS_TIME_INVALID_SECONDS;
	}

	if (milli > 999) {
		return OS_TIME_INVALID_MS;
	}

	if (hours + minutes + seconds + milli == 0) {
		return OS_TIME_ZERO_DLY;
	}

	unsigned long secs = hours*3600 + minutes*60 + seconds;
	if (secs > 0) {
		qt_parent_rtos->POSIX->uc_sleep(secs);
	} else {
		qt_parent_rtos->POSIX->uc_usleep(milli * 1000);
	}
	return OS_NO_ERR;
}

INT8U UC_microcos_class::OSTimeDlyResume(INT8U prio) {
	if (prio >= OS_LOWEST_PRIO) {
		return OS_PRIO_INVALID;
	}

	int tid = get_tid_by_prio(prio);
	if (tid == -1) {
		return OS_TASK_NOT_EXIST;
	}

	qt_parent_rtos->POSIX->uc_pthread_kill(tid, SIGCONT);

	return OS_NO_ERR;
}

INT32U UC_microcos_class::OSTimeGet(void) {
	return qt_parent_rtos->m_tic_timer->get_MicroCOS_num_tics();
}

void UC_microcos_class::OSTimeSet(INT32U ticks) {
	qt_parent_rtos->m_tic_timer->set_MicroCOS_num_tics(ticks);
}

// MicroC/OS II internal function
void OSTimeTick(void) {
	}

// Timer management
// No documentation

/*!
 * \brief Function to run the MicroCOS timer callback with its arguments
 * \param sig The argument acepted by the POSIX timer function
 */
void OSTmr_to_posix_timer_func(uc_sigval sig) {
	struct OSTmr_to_posix_timer *timer_func = (struct OSTmr_to_posix_timer *)( sig.sival_ptr );
	(timer_func->function)(timer_func->ptmr, timer_func->parg);
}

OS_TMR *UC_microcos_class::OSTmrCreate(INT32U dly, INT32U period, INT8U opt, OS_TMR_CALLBACK callback,
			void *callback_arg, INT8U *pname, INT8U *perr) {

	switch (opt) {
		case OS_TMR_OPT_PERIODIC:
			if (period == 0) {
				*perr = OS_ERR_TMR_INVALID_PERIOD;
				return ((OS_TMR *)0);
			}
			break;

		case OS_TMR_OPT_ONE_SHOT:
			if (dly == 0) {
				*perr = OS_ERR_TMR_INVALID_DLY;
				return ((OS_TMR *)0);
			}
			break;

		default:
			*perr = OS_ERR_TMR_INVALID_OPT;
			return ((OS_TMR *)0);
	}

	if (strlen((char *)pname) > OS_TMR_CFG_NAME_SIZE) {
		*perr = OS_ERR_TMR_NAME_TOO_LONG;
		return ((OS_TMR *)0);
	}

	OS_TMR *timer = new OS_TMR();

	// Prepare the wrapper to the POSIX function
	struct uc_sigevent event;
	timer_t timer_id;
	struct OSTmr_to_posix_timer *func_and_data = new struct OSTmr_to_posix_timer;
	func_and_data->function = callback;
	func_and_data->ptmr = timer;
	func_and_data->parg = callback_arg;
	event.sigev_notify = SIGEV_THREAD;
	event.sigev_notify_function = OSTmr_to_posix_timer_func;
	event.sigev_value.sival_ptr = (void *)(func_and_data);
	event.sigev_notify_attributes = NULL;

	// Create the POSIX timer
	qt_parent_rtos->POSIX->uc_timer_create(CLOCK_REALTIME, &event, &timer_id);

	// Init the timer
	itimerspec timerdata;
	double time;
	switch (opt) {
		case OS_TMR_OPT_PERIODIC:
			// Period time
			time = (double)period / TICK_TIMER_HZ;
			struct timespec ts_period;
			ts_period.tv_sec = (int)time;
			ts_period.tv_nsec = (int)( (time-(double)ts_period.tv_sec) * 1000000000 );
			timerdata.it_interval = ts_period;
			timerdata.it_value.tv_sec = 0;
			timerdata.it_value.tv_nsec = 0;
			break;

		case OS_TMR_OPT_ONE_SHOT:
			// Delay time
			time = (double)dly / TICK_TIMER_HZ;
			struct timespec ts_dly;
			ts_dly.tv_sec = (int)time;
			ts_dly.tv_nsec = (int)( (time-(double)ts_dly.tv_sec) * 1000000000 );
			timerdata.it_value = ts_dly;
			timerdata.it_interval.tv_sec = 0;
			timerdata.it_interval.tv_nsec = 0;
			break;
		default:
			break;
	}

	qt_parent_rtos->POSIX->uc_timer_settime(timer_id, 0, &timerdata, NULL);

	// The MicroCOS structure
	timer->OSTmrType = OS_TMR_TYPE;
	timer->OSTmrCallback = callback;
	timer->OSTmrCallbackArg = callback_arg;
	timer->OSTmrDly = dly;
	timer->OSTmrPeriod = period;
	timer->OSTmrOpt = opt;
	timer->timer_id = timer_id;
	timer->OSTmrState = OS_TMR_STATE_STOPPED;
	strncpy((char *)timer->OSTmrName, (char *)pname, OS_TMR_CFG_NAME_SIZE);

	*perr = OS_NO_ERR;
	return timer;
}

BOOLEAN UC_microcos_class::OSTmrDel(OS_TMR *ptmr, INT8U *perr) {
	if (ptmr == (OS_TMR *)0) {
		*perr = OS_ERR_TMR_INVALID;
		return (OS_FALSE);
	}

	if (ptmr->OSTmrType != OS_TMR_TYPE) {
		*perr = OS_ERR_TMR_INVALID_TYPE;
		return (OS_FALSE);
	}

	switch (ptmr->OSTmrState) {
		case OS_TMR_STATE_RUNNING:
		case OS_TMR_STATE_STOPPED:
		case OS_TMR_STATE_COMPLETED:
			qt_parent_rtos->POSIX->uc_timer_delete(ptmr->timer_id);
			delete ptmr;
			*perr = OS_ERR_NONE;
			return (OS_TRUE);
		case OS_TMR_STATE_UNUSED:
			*perr = OS_ERR_TMR_INACTIVE;
			return (OS_FALSE);
		default:
			*perr = OS_ERR_TMR_INVALID_STATE;
			return (OS_FALSE);
	}
}

INT8U UC_microcos_class::OSTmrNameGet(OS_TMR *ptmr, INT8U *pdest, INT8U *perr) {
	if (pdest == (INT8U *)0) {
		*perr = OS_ERR_TMR_INVALID_DEST;
		return 0;
	}

	if (ptmr == (OS_TMR *)0) {
		*perr = OS_ERR_TMR_INVALID;
		return 0;
	}

	if (ptmr->OSTmrType != OS_TMR_TYPE) {
		*perr = OS_ERR_TMR_INVALID_TYPE;
		return 0;
	}

	switch (ptmr->OSTmrState) {
		case OS_TMR_STATE_RUNNING:
		case OS_TMR_STATE_STOPPED:
		case OS_TMR_STATE_COMPLETED:
			strcpy((char *)pdest, (char *)ptmr->OSTmrName);
			*perr = OS_ERR_NONE;
			return strlen((char *)ptmr->OSTmrName);
		case OS_TMR_STATE_UNUSED:
			*perr = OS_ERR_TMR_INACTIVE;
			return 0;
		default:
			*perr = OS_ERR_TMR_INVALID_STATE;
			return 0;
	}
}

INT32U UC_microcos_class::OSTmrRemainGet(OS_TMR *ptmr, INT8U *perr) {
	if (ptmr == (OS_TMR *)0) {
		*perr = OS_ERR_TMR_INVALID;
		return 0;
	}

	if (ptmr->OSTmrType != OS_TMR_TYPE) {
		*perr = OS_ERR_TMR_INVALID_TYPE;
		return 0;
	}

	struct itimerspec time_spec;
	switch (ptmr->OSTmrState) {
		case OS_TMR_STATE_RUNNING:
		case OS_TMR_STATE_STOPPED:
			qt_parent_rtos->POSIX->uc_timer_gettime(ptmr->timer_id, &time_spec);
			*perr  = OS_ERR_NONE;
			return (INT32U)((time_spec.it_value.tv_sec + time_spec.it_value.tv_nsec / 1000000000) * TICK_TIMER_HZ);
		case OS_TMR_STATE_COMPLETED:
			*perr = OS_ERR_NONE;
			return 0;
		case OS_TMR_STATE_UNUSED:
			*perr = OS_ERR_TMR_INACTIVE;
			return 0;
		default:
			*perr = OS_ERR_TMR_INVALID_STATE;
			return 0;
	}
}

INT8U UC_microcos_class::OSTmrStateGet(OS_TMR *ptmr, INT8U *perr) {
	if (ptmr == (OS_TMR *)0) {
		*perr = OS_ERR_TMR_INVALID;
		return 0;
	}

	if (ptmr->OSTmrType != OS_TMR_TYPE) {
		*perr = OS_ERR_TMR_INVALID_TYPE;
		return 0;
	}

	INT8U state = ptmr->OSTmrState;
	switch (state) {
		case OS_TMR_STATE_UNUSED:
		case OS_TMR_STATE_STOPPED:
		case OS_TMR_STATE_COMPLETED:
		case OS_TMR_STATE_RUNNING:
			*perr = OS_ERR_NONE;
			break;
		default:
			*perr = OS_ERR_TMR_INVALID_STATE;
			break;
	}
	return state;
}

BOOLEAN UC_microcos_class::OSTmrStart(OS_TMR *ptmr, INT8U *perr) {
	if (ptmr == (OS_TMR *)0) {
		*perr = OS_ERR_TMR_INVALID;
		return (OS_FALSE);
	}

	if (ptmr->OSTmrType != OS_TMR_TYPE) {
		*perr = OS_ERR_TMR_INVALID_TYPE;
		return (OS_FALSE);
	}

	switch (ptmr->OSTmrState) {
		case OS_TMR_STATE_RUNNING:
		case OS_TMR_STATE_STOPPED:
		case OS_TMR_STATE_COMPLETED:
			itimerspec timerdata;
			double time;
			switch (ptmr->OSTmrOpt) {
				case OS_TMR_OPT_PERIODIC:
					// Period time
					time = (double)ptmr->OSTmrPeriod / TICK_TIMER_HZ;
					struct timespec ts_period;
					ts_period.tv_sec = (int)time;
					ts_period.tv_nsec = (int)( (time-(double)ts_period.tv_sec) * 1000000000 );
					timerdata.it_interval = ts_period;
					timerdata.it_value.tv_sec = 0;
					timerdata.it_value.tv_nsec = 0;
					break;

				case OS_TMR_OPT_ONE_SHOT:
					// Delay time
					time = (double)ptmr->OSTmrDly / TICK_TIMER_HZ;
					struct timespec ts_dly;
					ts_dly.tv_sec = (int)time;
					ts_dly.tv_nsec = (int)( (time-(double)ts_dly.tv_sec) * 1000000000 );
					timerdata.it_value = ts_dly;
					timerdata.it_interval.tv_sec = 0;
					timerdata.it_interval.tv_nsec = 0;
					break;
				default:
					break;
			}
			qt_parent_rtos->POSIX->uc_timer_settime(ptmr->timer_id, 0, &timerdata, NULL);
			*perr = OS_ERR_NONE;
			return (OS_TRUE);

		case OS_TMR_STATE_UNUSED:
			*perr = OS_ERR_TMR_INACTIVE;
			return (OS_FALSE);
		default:
			*perr = OS_ERR_TMR_INVALID_STATE;
			return (OS_FALSE);
	}
}

BOOLEAN UC_microcos_class::OSTmrStop(OS_TMR *ptmr, INT8U opt, void *callback_arg, INT8U *perr) {
	if (ptmr == (OS_TMR *)0) {
		*perr = OS_ERR_TMR_INVALID;
		return (OS_FALSE);
	}

	if (ptmr->OSTmrType != OS_TMR_TYPE) {
		*perr = OS_ERR_TMR_INVALID_TYPE;
		return (OS_FALSE);
	}

	OS_TMR_CALLBACK  pfnct;
	switch (ptmr->OSTmrState) {
		case OS_TMR_STATE_RUNNING:
			*perr = OS_ERR_NONE;
			switch (opt) {
				case OS_TMR_OPT_CALLBACK:
					pfnct = ptmr->OSTmrCallback;
					if (pfnct != (OS_TMR_CALLBACK)0) {
						(*pfnct)((void *)ptmr, ptmr->OSTmrCallbackArg);
					} else {
						*perr = OS_ERR_TMR_NO_CALLBACK;
					}
					break;
				case OS_TMR_OPT_CALLBACK_ARG:
					pfnct = ptmr->OSTmrCallback;
					if (pfnct != (OS_TMR_CALLBACK)0) {
						(*pfnct)((void *)ptmr, callback_arg);
					} else {
						*perr = OS_ERR_TMR_NO_CALLBACK;
					}
					break;
				case OS_TMR_OPT_NONE:
					break;
				default:
					*perr = OS_ERR_TMR_INVALID_OPT;
					break;
			}
			return (OS_TRUE);

		case OS_TMR_STATE_COMPLETED:
		case OS_TMR_STATE_STOPPED:
			*perr = OS_ERR_TMR_STOPPED;
			return (OS_TRUE);
		case OS_TMR_STATE_UNUSED:
			*perr = OS_ERR_TMR_INACTIVE;
			return (OS_FALSE);
		default:
			*perr = OS_ERR_TMR_INVALID_STATE;
			return (OS_FALSE);
	}
}

/*
INT8U OSTmrSignal(void) {
}
*/
