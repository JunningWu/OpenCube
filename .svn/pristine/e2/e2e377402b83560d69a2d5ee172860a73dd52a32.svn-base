//==========================================================================
//	uc_microcos_c.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mié ene 30 2008
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

#include "rtos/kernel/uc_rtos_class.h"
#include "rtos/api/ucos/uc_microcos.h"
extern unsigned long long uc_segment_time;

// Auxiliary functions
/**
 * \brief Change the thread state from USER to SUPER_USER
 */
#define USER_TO_SUPER_USER() \
			UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread; \
			bool change_state = false; \
			if (thread->m_state == USER) { \
				change_state = true; \
				/*if(qt_parent_rtos->get_current_cpu()->m_icache != NULL)		\
					{qt_parent_rtos->get_current_cpu()->m_icache->flush();}	\
				if(qt_parent_rtos->get_current_cpu()->m_dcache != NULL)		\
					{qt_parent_rtos->get_current_cpu()->m_dcache->flush();}*/	\
				thread->set_next_state(USER, SUPER_USER); 		\
				stp_yield(); \
			}

/**
 * \brief Change the thread state from SUPER_USER to USER
 */
#define SUPER_USER_TO_WAITING() \
			if (change_state == true) { \
				thread->set_next_state(SUPER_USER, WAITING); \
				stp_yield(); \
			}

/**
 * \brief The MicroCOS object path
 */
#define MicroCOS qt_parent_rtos->MicroCOS

extern __thread UC_rtos_class *qt_parent_rtos;
extern __thread UC_process_class *qt_parent_process;

/**************************/
/*** MicroCOS functions ***/
/**************************/

// These functions must start with the USER_TO_SUPER_USER() macro
// and finish with the SUPER_USER_TO_WAITING() macro.
// The implementation of the MicroCOS functions are in the
// UC_microcos_class class. Thus, to execute them they must be
// called through the correct path, that is loaded in the
// MicroCOS macro. (To call them: MicroCOS->MicroCOS_function).
// This is the user interface.
// This way of work splits the C++ implementation from the C
// interface and the state switches from the real implementation
// to avoid double state switches when MicroCOS to MicroCOS calls are done.

void   OSInit(void) {
	USER_TO_SUPER_USER();
	MicroCOS->OSInit();
	SUPER_USER_TO_WAITING();
}

void   OSIntEnter(void) {
	USER_TO_SUPER_USER();
	MicroCOS->OSIntEnter();
	SUPER_USER_TO_WAITING();
}

void   OSIntExit(void) {
	USER_TO_SUPER_USER();
	MicroCOS->OSIntExit();
	SUPER_USER_TO_WAITING();
}

void   OSSchedLock(void) {
	USER_TO_SUPER_USER();
	MicroCOS->OSSchedLock();
	SUPER_USER_TO_WAITING();
}

void   OSSchedUnlock(void) {
	USER_TO_SUPER_USER();
	MicroCOS->OSSchedUnlock();
	SUPER_USER_TO_WAITING();
}

void   OSStart(void) {
	USER_TO_SUPER_USER();
	MicroCOS->OSStart();
	SUPER_USER_TO_WAITING();
}

void   OSStatInit(void) {
	USER_TO_SUPER_USER();
	MicroCOS->OSStatInit();
	SUPER_USER_TO_WAITING();
}

INT16U OSVersion(void) {
	USER_TO_SUPER_USER();
	INT16U ret = MicroCOS->OSVersion();
	SUPER_USER_TO_WAITING();
	return ret;
}

// INT8U  OSEventNameGet(OS_EVENT *pevent, INT8U *pname, INT8U *perr) {
// 	USER_TO_SUPER_USER();
// 	INT8U ret = MicroCOS->OSEventNameGet(pevent, pname, perr);
// 	SUPER_USER_TO_WAITING();
// 	return ret;
// }

// void   OSEventNameSet(OS_EVENT *pevent, INT8U *pname, INT8U *perr) {
// 	USER_TO_SUPER_USER();
// 	MicroCOS->OSEventNameSet(pevent, pname, perr);
// 	SUPER_USER_TO_WAITING();
// }


// Event flags management
OS_FLAGS OSFlagAccept(OS_FLAG_GRP *pgrp, OS_FLAGS flags, INT8U wait_type, INT8U *perr) {
	USER_TO_SUPER_USER();
	OS_FLAGS ret = MicroCOS->OSFlagAccept(pgrp, flags, wait_type, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

OS_FLAG_GRP *OSFlagCreate(OS_FLAGS flags, INT8U *perr) {
	USER_TO_SUPER_USER();
	OS_FLAG_GRP *ret = MicroCOS->OSFlagCreate(flags, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

OS_FLAG_GRP *OSFlagDel(OS_FLAG_GRP *pgrp, INT8U opt, INT8U *perr) {
	USER_TO_SUPER_USER();
	OS_FLAG_GRP * ret = MicroCOS->OSFlagDel(pgrp, opt, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

INT8U OSFlagNameGet(OS_FLAG_GRP *pgrp, INT8U *pname, INT8U *perr) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSFlagNameGet(pgrp, pname, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

void  OSFlagNameSet(OS_FLAG_GRP *pgrp, INT8U *pname, INT8U *perr) {
	USER_TO_SUPER_USER();
	MicroCOS->OSFlagNameSet(pgrp, pname, perr);
	SUPER_USER_TO_WAITING();
}

OS_FLAGS OSFlagPend(OS_FLAG_GRP *pgrp, OS_FLAGS flags, INT8U wait_type, INT16U timeout, INT8U *perr) {
	USER_TO_SUPER_USER();
	OS_FLAGS ret = MicroCOS->OSFlagPend(pgrp, flags, wait_type, timeout, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

OS_FLAGS OSFlagPendGetFlagsRdy(void) {
	USER_TO_SUPER_USER();
	OS_FLAGS ret = MicroCOS->OSFlagPendGetFlagsRdy();
	SUPER_USER_TO_WAITING();
	return ret;
}

OS_FLAGS OSFlagPost(OS_FLAG_GRP *pgrp, OS_FLAGS flags, INT8U opt, INT8U *perr) {
	USER_TO_SUPER_USER();
	OS_FLAGS ret = MicroCOS->OSFlagPost(pgrp, flags, opt, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

OS_FLAGS OSFlagQuery(OS_FLAG_GRP *pgrp, INT8U *perr) {
	USER_TO_SUPER_USER();
	OS_FLAGS ret = MicroCOS->OSFlagQuery(pgrp, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}


// Message mailbox management
void *OSMboxAccept(OS_EVENT *pevent) {
	USER_TO_SUPER_USER();
	MicroCOS->OSMboxAccept(pevent);
	SUPER_USER_TO_WAITING();
}

OS_EVENT *OSMboxCreate(void *pmsg) {
	USER_TO_SUPER_USER();
	OS_EVENT *ret = MicroCOS->OSMboxCreate(pmsg);
	SUPER_USER_TO_WAITING();
	return ret;
}

OS_EVENT *OSMboxDel(OS_EVENT *pevent, INT8U opt, INT8U *perr) {
	USER_TO_SUPER_USER();
	OS_EVENT *ret = MicroCOS->OSMboxDel(pevent, opt, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

void *OSMboxPend(OS_EVENT *pevent, INT16U timeout, INT8U *perr) {
	USER_TO_SUPER_USER();
	void *ret = MicroCOS->OSMboxPend(pevent, timeout, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

INT8U OSMboxPendAbort(OS_EVENT *pevent, INT8U opt, INT8U *perr) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSMboxPendAbort(pevent, opt, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

INT8U OSMboxPost(OS_EVENT *pevent, void *pmsg) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSMboxPost(pevent, pmsg);
	SUPER_USER_TO_WAITING();
	return ret;
}

INT8U OSMboxPostOpt(OS_EVENT *pevent, void *pmsg, INT8U opt) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSMboxPostOpt(pevent, pmsg, opt);
	SUPER_USER_TO_WAITING();
	return ret;
}

INT8U OSMboxQuery(OS_EVENT *pevent, OS_MBOX_DATA *p_mbox_data) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSMboxQuery(pevent, p_mbox_data);
	SUPER_USER_TO_WAITING();
	return ret;
}


// Memory management
OS_MEM *OSMemCreate(void *addr, INT32U nblks, INT32U blksize, INT8U *perr) {
	USER_TO_SUPER_USER();
	OS_MEM *ret = MicroCOS->OSMemCreate(addr, nblks, blksize, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

void   *OSMemGet(OS_MEM *pmem, INT8U *perr) {
	USER_TO_SUPER_USER();
	void *ret = MicroCOS->OSMemGet(pmem, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

INT8U   OSMemNameGet(OS_MEM *pmem, INT8U *pname, INT8U *perr) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSMemNameGet(pmem, pname, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

void    OSMemNameSet(OS_MEM *pmem, INT8U *pname, INT8U *perr) {
	USER_TO_SUPER_USER();
	MicroCOS->OSMemNameSet(pmem, pname, perr);
	SUPER_USER_TO_WAITING();
}

INT8U   OSMemPut(OS_MEM *pmem, void *pblk) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSMemPut(pmem, pblk);
	SUPER_USER_TO_WAITING();
	return ret;
}

INT8U   OSMemQuery(OS_MEM *pmem, OS_MEM_DATA *p_mem_data) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSMemQuery(pmem, p_mem_data);
	SUPER_USER_TO_WAITING();
	return ret;
}


// Mutual exclusion semaphore management
BOOLEAN   OSMutexAccept(OS_EVENT *pevent, INT8U *perr) {
	USER_TO_SUPER_USER();
	BOOLEAN ret = MicroCOS->OSMutexAccept(pevent, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

OS_EVENT *OSMutexCreate(INT8U prio, INT8U *perr) {
	USER_TO_SUPER_USER();
	OS_EVENT *ret = MicroCOS->OSMutexCreate(prio, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

OS_EVENT *OSMutexDel(OS_EVENT *pevent, INT8U opt, INT8U *perr) {
	USER_TO_SUPER_USER();
	OS_EVENT *ret = MicroCOS->OSMutexDel(pevent, opt, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

void  OSMutexPend(OS_EVENT *pevent, INT16U timeout, INT8U *perr) {
	USER_TO_SUPER_USER();
	MicroCOS->OSMutexPend(pevent, timeout, perr);
	SUPER_USER_TO_WAITING();
}

INT8U OSMutexPost(OS_EVENT *pevent) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSMutexPost(pevent);
	SUPER_USER_TO_WAITING();
	return ret;
}

INT8U OSMutexQuery(OS_EVENT *pevent, OS_MUTEX_DATA *p_mutex_data) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSMutexQuery(pevent, p_mutex_data);
	SUPER_USER_TO_WAITING();
	return ret;
}


// Message queue management
void *OSQAccept(OS_EVENT *pevent, INT8U *perr) {
	USER_TO_SUPER_USER();
	void *ret = MicroCOS->OSQAccept(pevent, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

OS_EVENT *OSQCreate(void **start, INT16U size) {
	USER_TO_SUPER_USER();
	OS_EVENT *ret = MicroCOS->OSQCreate(start, size);
	SUPER_USER_TO_WAITING();
	return ret;
}

OS_EVENT *OSQDel(OS_EVENT *pevent, INT8U opt, INT8U *perr) {
	USER_TO_SUPER_USER();
	OS_EVENT *ret = MicroCOS->OSQDel(pevent, opt, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

INT8U OSQFlush(OS_EVENT *pevent) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSQFlush(pevent);
	SUPER_USER_TO_WAITING();
	return ret;
}

void *OSQPend(OS_EVENT *pevent, INT16U timeout, INT8U *perr) {
	USER_TO_SUPER_USER();
	void *ret = MicroCOS->OSQPend(pevent, timeout, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

// INT8U OSQPendAbort(OS_EVENT *pevent, INT8U opt, INT8U *perr) {
// 	USER_TO_SUPER_USER();
// 	INT8U ret = MicroCOS->OSQPendAbort(pevent, opt, perr);
// 	SUPER_USER_TO_WAITING();
// 	return ret;
// }

INT8U OSQPost(OS_EVENT *pevent, void *pmsg) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSQPost(pevent, pmsg);
	SUPER_USER_TO_WAITING();
	return ret;
}

INT8U OSQPostFront(OS_EVENT *pevent, void *pmsg) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSQPostFront(pevent, pmsg);
	SUPER_USER_TO_WAITING();
	return ret;
}

// INT8U OSQPostOpt(OS_EVENT *pevent, void *pmsg, INT8U opt) {
// 	USER_TO_SUPER_USER();
// 	INT8U ret = MicroCOS->OSQPostOpt(pevent, pmsg, opt);
// 	SUPER_USER_TO_WAITING();
// 	return ret;
// }

INT8U OSQQuery(OS_EVENT *pevent, OS_Q_DATA *p_q_data) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSQQuery(pevent, p_q_data);
	SUPER_USER_TO_WAITING();
	return ret;
}


// Semaphore management
INT16U OSSemAccept(OS_EVENT *pevent) {
	USER_TO_SUPER_USER();
	INT16U ret = MicroCOS->OSSemAccept(pevent);
	SUPER_USER_TO_WAITING();
	return ret;
}

OS_EVENT *OSSemCreate(INT16U cnt) {
	USER_TO_SUPER_USER();
	OS_EVENT *ret = MicroCOS->OSSemCreate(cnt);
	SUPER_USER_TO_WAITING();
	return ret;
}

OS_EVENT *OSSemDel(OS_EVENT *pevent, INT8U opt, INT8U *perr) {
	USER_TO_SUPER_USER();
	OS_EVENT *ret = MicroCOS->OSSemDel(pevent, opt, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

void  OSSemPend(OS_EVENT *pevent, INT16U timeout, INT8U *perr) {
	USER_TO_SUPER_USER();
	MicroCOS->OSSemPend(pevent, timeout, perr);
	SUPER_USER_TO_WAITING();
}

INT8U OSSemPendAbort(OS_EVENT *pevent, INT8U opt, INT8U *perr) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSSemPendAbort(pevent, opt, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

INT8U OSSemPost(OS_EVENT *pevent) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSSemPost(pevent);
	SUPER_USER_TO_WAITING();
	return ret;
}

INT8U OSSemQuery(OS_EVENT *pevent, OS_SEM_DATA *p_sem_data) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSSemQuery(pevent, p_sem_data);
	SUPER_USER_TO_WAITING();
	return ret;
}

void  OSSemSet(OS_EVENT *pevent, INT16U cnt, INT8U *perr) {
	USER_TO_SUPER_USER();
	MicroCOS->OSSemSet(pevent, cnt, perr);
	SUPER_USER_TO_WAITING();
}


// Task management
INT8U OSTaskChangePrio(INT8U oldprio, INT8U newprio) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSTaskChangePrio(oldprio, newprio);
	SUPER_USER_TO_WAITING();
	return ret;
}

INT8U OSTaskCreate(void (*task)(void *p_arg), void *p_arg, OS_STK *ptos, INT8U prio) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSTaskCreate(task, p_arg, ptos, prio);
	SUPER_USER_TO_WAITING();
	return ret;
}

INT8U OSTaskCreateExt(void (*task)(void *p_arg), void *p_arg, OS_STK *ptos, INT8U prio,
						INT16U id, OS_STK *pbos, INT32U stk_size, void *pext, INT16U opt) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSTaskCreateExt(task, p_arg, ptos, prio, id, pbos, stk_size, pext, opt);
	SUPER_USER_TO_WAITING();
	return ret;
}

INT8U OSTaskDel(INT8U prio) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSTaskDel(prio);
	SUPER_USER_TO_WAITING();
	return ret;
}

INT8U OSTaskDelReq(INT8U prio) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSTaskDelReq(prio);
	SUPER_USER_TO_WAITING();
	return ret;
}

INT8U OSTaskNameGet(INT8U prio, INT8U *pname, INT8U *perr) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSTaskNameGet(prio, pname, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

void  OSTaskNameSet(INT8U prio, INT8U *pname, INT8U *perr) {
	USER_TO_SUPER_USER();
	MicroCOS->OSTaskNameSet(prio, pname, perr);
	SUPER_USER_TO_WAITING();
}

INT8U OSTaskResume(INT8U prio) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSTaskResume(prio);
	SUPER_USER_TO_WAITING();
	return ret;
}

INT8U OSTaskSuspend(INT8U prio) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSTaskSuspend(prio);
	SUPER_USER_TO_WAITING();
	return ret;
}

// INT8U OSTaskStkChk(INT8U prio, OS_STK_DATA *p_stk_data) {
// 	USER_TO_SUPER_USER();
// 	INT8U ret = MicroCOS->OSTaskStkChk(prio, p_stk_data);
// 	SUPER_USER_TO_WAITING();
// 	return ret;
// }

// INT8U OSTaskQuery(INT8U prio, OS_TCB *p_task_data) {
// 	USER_TO_SUPER_USER();
// 	INT8U ret = MicroCOS->OSTaskQuery(prio, p_task_data);
// 	SUPER_USER_TO_WAITING();
// 	return ret;
// }


// Time management
void OSTimeDly(INT16U ticks) {
	USER_TO_SUPER_USER();
	MicroCOS->OSTimeDly(ticks);
	SUPER_USER_TO_WAITING();
}

INT8U OSTimeDlyHMSM(INT8U hours, INT8U minutes, INT8U seconds, INT16U milli) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSTimeDlyHMSM(hours, minutes, seconds, milli);
	SUPER_USER_TO_WAITING();
	return ret;
}

INT8U OSTimeDlyResume(INT8U prio) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSTimeDlyResume(prio);
	SUPER_USER_TO_WAITING();
	return ret;
}

INT32U OSTimeGet(void) {
	USER_TO_SUPER_USER();
	INT32U ret = MicroCOS->OSTimeGet();
	SUPER_USER_TO_WAITING();
	return ret;
}

void OSTimeSet(INT32U ticks) {
	USER_TO_SUPER_USER();
	MicroCOS->OSTimeSet(ticks);
	SUPER_USER_TO_WAITING();
}

// void OSTimeTick(void) {
// 	USER_TO_SUPER_USER();
// 	MicroCOS->OSTimeTick();
// 	SUPER_USER_TO_WAITING();
// }


// Timer management
OS_TMR *OSTmrCreate(INT32U dly, INT32U period, INT8U opt, OS_TMR_CALLBACK callback,
					void *callback_arg, INT8U *pname, INT8U *perr) {
	USER_TO_SUPER_USER();
	OS_TMR *ret = MicroCOS->OSTmrCreate(dly, period, opt, callback, callback_arg, pname, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

BOOLEAN OSTmrDel(OS_TMR *ptmr, INT8U *perr) {
	USER_TO_SUPER_USER();
	BOOLEAN ret = MicroCOS->OSTmrDel(ptmr, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

INT8U OSTmrNameGet(OS_TMR *ptmr, INT8U *pdest, INT8U *perr) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSTmrNameGet(ptmr, pdest, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

INT32U OSTmrRemainGet(OS_TMR *ptmr, INT8U *perr) {
	USER_TO_SUPER_USER();
	INT32U ret = MicroCOS->OSTmrRemainGet(ptmr, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

INT8U OSTmrStateGet(OS_TMR *ptmr, INT8U *perr) {
	USER_TO_SUPER_USER();
	INT8U ret = MicroCOS->OSTmrStateGet(ptmr, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

BOOLEAN OSTmrStart(OS_TMR *ptmr, INT8U *perr) {
	USER_TO_SUPER_USER();
	BOOLEAN ret = MicroCOS->OSTmrStart(ptmr, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

BOOLEAN OSTmrStop(OS_TMR *ptmr, INT8U opt, void *callback_arg, INT8U *perr) {
	USER_TO_SUPER_USER();
	BOOLEAN ret = MicroCOS->OSTmrStop(ptmr, opt, callback_arg, perr);
	SUPER_USER_TO_WAITING();
	return ret;
}

// INT8U OSTmrSignal(void) {
// 	USER_TO_SUPER_USER();
// 	INT8U ret = MicroCOS->OSTmrSignal();
// 	SUPER_USER_TO_WAITING();
// 	return ret;
// }
