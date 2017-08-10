//==========================================================================
//	uc_microcos.h
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


#ifndef _UC_MICROCOS_H
#define _UC_MICROCOS_H

#include <time.h>
#include "rtos/api/ucos/uc_microcos_defines.h"
#include "rtos/api/ucos/ucos_ii_common.h"
// #include "sys_uc_types.h"

class UC_thread_class;
class UC_rtos_class;

/*** MicroCOS structures ***/
#if 0
// Event control block
typedef struct os_event {
	INT8U    OSEventType;                    /* Type of event control block (see OS_EVENT_TYPE_xxxx)    */
	void    *OSEventPtr;                     /* Pointer to message or queue structure                   */
	INT16U   OSEventCnt;                     /* Semaphore Count (not used if other EVENT type)          */
	INT16U   OSEventGrp;                     /* Group corresponding to tasks waiting for event to occur */
	INT16U   OSEventTbl[OS_EVENT_TBL_SIZE];  /* List of tasks waiting for event to occur                */
	INT8U    OSEventName[OS_EVENT_NAME_SIZE];
} OS_EVENT;

// Event flags control block
typedef  INT32U   OS_FLAGS;


typedef struct os_flag_grp {                /* Event Flag Group                                        */
	INT8U         OSFlagType;               /* Should be set to OS_EVENT_TYPE_FLAG                     */
	void         *OSFlagWaitList;           /* Pointer to first NODE of task waiting on event flag     */
	OS_FLAGS      OSFlagFlags;              /* 8, 16 or 32 bit flags                                   */
	INT8U         OSFlagName[OS_FLAG_NAME_SIZE];
} OS_FLAG_GRP;


typedef struct os_flag_node {               /* Event Flag Wait List Node                               */
	void         *OSFlagNodeNext;           /* Pointer to next     NODE in wait list                   */
	void         *OSFlagNodePrev;           /* Pointer to previous NODE in wait list                   */
	void         *OSFlagNodeTCB;            /* Pointer to TCB of waiting task                          */
	void         *OSFlagNodeFlagGrp;        /* Pointer to Event Flag Group                             */
	OS_FLAGS      OSFlagNodeFlags;          /* Event flag to wait on                                   */
	INT8U         OSFlagNodeWaitType;       /* Type of wait:                                           */
	/*      OS_FLAG_WAIT_AND                                   */
	/*      OS_FLAG_WAIT_ALL                                   */
	/*      OS_FLAG_WAIT_OR                                    */
	/*      OS_FLAG_WAIT_ANY                                   */
} OS_FLAG_NODE;


// Message mailbox data
typedef struct os_mbox_data {
	void   *OSMsg;                         /* Pointer to message in mailbox                            */
	INT16U  OSEventTbl[OS_EVENT_TBL_SIZE]; /* List of tasks waiting for event to occur                 */
	INT16U  OSEventGrp;                    /* Group corresponding to tasks waiting for event to occur  */
} OS_MBOX_DATA;


// Memory partition data structures
typedef struct os_mem {                   /* MEMORY CONTROL BLOCK                                      */
	void   *OSMemAddr;                    /* Pointer to beginning of memory partition                  */
	void   *OSMemFreeList;                /* Pointer to list of free memory blocks                     */
	INT32U  OSMemBlkSize;                 /* Size (in bytes) of each block of memory                   */
	INT32U  OSMemNBlks;                   /* Total number of blocks in this partition                  */
	INT32U  OSMemNFree;                   /* Number of memory blocks remaining in this partition       */
	INT8U   OSMemName[OS_MEM_NAME_SIZE];  /* Memory partition name                                     */
} OS_MEM;


typedef struct os_mem_data {
	void   *OSAddr;                    /* Pointer to the beginning address of the memory partition     */
	void   *OSFreeList;                /* Pointer to the beginning of the free list of memory blocks   */
	INT32U  OSBlkSize;                 /* Size (in bytes) of each memory block                         */
	INT32U  OSNBlks;                   /* Total number of blocks in the partition                      */
	INT32U  OSNFree;                   /* Number of memory blocks free                                 */
	INT32U  OSNUsed;                   /* Number of memory blocks used                                 */
} OS_MEM_DATA;


// Mutual exclusion semaphore data
typedef struct os_mutex_data {
	INT16U  OSEventTbl[OS_EVENT_TBL_SIZE];  /* List of tasks waiting for event to occur                */
	INT16U  OSEventGrp;                     /* Group corresponding to tasks waiting for event to occur */
	BOOLEAN OSValue;                        /* Mutex value (OS_FALSE = used, OS_TRUE = available)      */
	INT8U   OSOwnerPrio;                    /* Mutex owner's task priority or 0xFF if no owner         */
	INT8U   OSMutexPIP;                     /* Priority Inheritance Priority or 0xFF if no owner       */
} OS_MUTEX_DATA;


// Message queue data
typedef struct os_q {                   /* QUEUE CONTROL BLOCK                                         */
	struct os_q   *OSQPtr;              /* Link to next queue control block in list of free blocks     */
	void         **OSQStart;            /* Pointer to start of queue data                              */
	void         **OSQEnd;              /* Pointer to end   of queue data                              */
	void         **OSQIn;               /* Pointer to where next message will be inserted  in   the Q  */
	void         **OSQOut;              /* Pointer to where next message will be extracted from the Q  */
	INT16U         OSQSize;             /* Size of queue (maximum number of entries)                   */
	INT16U         OSQEntries;          /* Current number of entries in the queue                      */
} OS_Q;


typedef struct os_q_data {
	void          *OSMsg;               /* Pointer to next message to be extracted from queue          */
	INT16U         OSNMsgs;             /* Number of messages in message queue                         */
	INT16U         OSQSize;             /* Size of message queue                                       */
	INT16U         OSEventTbl[OS_EVENT_TBL_SIZE];  /* List of tasks waiting for event to occur         */
	INT16U         OSEventGrp;          /* Group corresponding to tasks waiting for event to occur     */
} OS_Q_DATA;


// Semaphore data
typedef struct os_sem_data {
	INT16U  OSCnt;                          /* Semaphore count                                         */
	INT16U  OSEventTbl[OS_EVENT_TBL_SIZE];  /* List of tasks waiting for event to occur                */
	INT16U  OSEventGrp;                     /* Group corresponding to tasks waiting for event to occur */
} OS_SEM_DATA;


// Task stack data
typedef struct os_stk_data {
	INT32U  OSFree;                    /* Number of free bytes on the stack                            */
	INT32U  OSUsed;                    /* Number of bytes used on the stack                            */
} OS_STK_DATA;


// Task control block
typedef struct os_tcb {
	OS_STK          *OSTCBStkPtr;      /* Pointer to current top of stack                              */

	void            *OSTCBExtPtr;      /* Pointer to user definable data for TCB extension             */
	OS_STK          *OSTCBStkBottom;   /* Pointer to bottom of stack                                   */
	INT32U           OSTCBStkSize;     /* Size of task stack (in number of stack elements)             */
	INT16U           OSTCBOpt;         /* Task options as passed by OSTaskCreateExt()                  */
	INT16U           OSTCBId;          /* Task ID (0..65535)                                           */

	struct os_tcb   *OSTCBNext;        /* Pointer to next     TCB in the TCB list                      */
	struct os_tcb   *OSTCBPrev;        /* Pointer to previous TCB in the TCB list                      */

	OS_EVENT        *OSTCBEventPtr;    /* Pointer to event control block                               */

	void            *OSTCBMsg;         /* Message received from OSMboxPost() or OSQPost()              */

	OS_FLAG_NODE    *OSTCBFlagNode;    /* Pointer to event flag node                                   */
	OS_FLAGS         OSTCBFlagsRdy;    /* Event flags that made task ready to run                      */

	INT16U           OSTCBDly;         /* Nbr ticks to delay task or, timeout waiting for event        */
	INT8U            OSTCBStat;        /* Task      status                                             */
	INT8U            OSTCBStatPend;    /* Task PEND status                                             */
	INT8U            OSTCBPrio;        /* Task priority (0 == highest)                                 */

	INT8U            OSTCBX;           /* Bit position in group  corresponding to task priority        */
	INT8U            OSTCBY;           /* Index into ready table corresponding to task priority        */
	INT16U           OSTCBBitX;        /* Bit mask to access bit position in ready table               */
	INT16U           OSTCBBitY;        /* Bit mask to access bit position in ready group               */
	INT8U            OSTCBDelReq;      /* Indicates whether a task needs to delete itself              */

	INT32U           OSTCBCtxSwCtr;    /* Number of time the task was switched in                      */
	INT32U           OSTCBCyclesTot;   /* Total number of clock cycles the task has been running       */
	INT32U           OSTCBCyclesStart; /* Snapshot of cycle counter at start of task resumption        */
	OS_STK          *OSTCBStkBase;     /* Pointer to the beginning of the task stack                   */
	INT32U           OSTCBStkUsed;     /* Number of bytes used from the stack                          */
	INT8U            OSTCBTaskName[OS_TASK_NAME_SIZE];
} OS_TCB;


// Timer data types
typedef  void (*OS_TMR_CALLBACK)(void *ptmr, void *parg);


typedef  struct  os_tmr {
	INT8U            OSTmrType;                       /* Should be set to OS_TMR_TYPE                                  */
	OS_TMR_CALLBACK  OSTmrCallback;                   /* Function to call when timer expires                           */
	void            *OSTmrCallbackArg;                /* Argument to pass to function when timer expires               */
	void            *OSTmrNext;                       /* Double link list pointers                                     */
	void            *OSTmrPrev;
	INT32U           OSTmrMatch;                      /* Timer expires when OSTmrTime == OSTmrMatch                    */
	INT32U           OSTmrDly;                        /* Delay time before periodic update starts                      */
	INT32U           OSTmrPeriod;                     /* Period to repeat timer                                        */
	INT8U            OSTmrName[OS_TMR_CFG_NAME_SIZE]; /* Name to give the timer                                        */
	INT8U            OSTmrOpt;                        /* Options (see OS_TMR_OPT_xxx)                                  */
	INT8U            OSTmrState;                      /* Indicates the state of the timer:                             */
	/*     OS_TMR_STATE_UNUSED                                       */
	/*     OS_TMR_STATE_RUNNING                                      */
	/*     OS_TMR_STATE_STOPPED                                      */
	timer_t          timer_id;                        /* Timer identification for the POSIX timer                      */
} OS_TMR;


typedef  struct  os_tmr_wheel {
	OS_TMR          *OSTmrFirst;                      /* Pointer to first timer in linked list                         */
	INT16U           OSTmrEntries;
} OS_TMR_WHEEL;

#endif

/// Struct to support the conversion from MicroCOS start_routine format to the POSIX one
struct MicroCOS_task_and_args {
	void (*task)(void *args);	// The task to run
	void *args;			// The arguments to pass to the task
};

/// Struct for flags support
struct MicroCOS_flags {
	UC_thread_class *thread;
	INT8U wait_type;
	OS_FLAGS flags;
};

/// Struct to run the MicroCOS timer callback with its arguments
struct OSTmr_to_posix_timer {
	OS_TMR_CALLBACK function;
	void *ptmr;
	void *parg;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class UC_microcos_class {
public:

	UC_microcos_class(UC_rtos_class *rtos);

	/*** Function prototypes ***/

	// Miscellaneous
	#define OS_ENTER_CRITICAL() cli()
	#define OS_EXIT_CRITICAL() sti()
	void   OSInit(void);
	void   OSIntEnter(void);
	void   OSIntExit(void);
	void   OSSchedLock(void);
	void   OSSchedUnlock(void);
	void   OSStart(void);
	void   OSStatInit(void);
	INT16U OSVersion(void);
	uc_pthread_t get_tid_by_prio(int prio);
	INT8U  OSEventNameGet(OS_EVENT *pevent, INT8U *pname, INT8U *perr);
	void   OSEventNameSet(OS_EVENT *pevent, INT8U *pname, INT8U *perr);

	// Event flags management
	bool uc_FlagBlock(OS_FLAG_GRP *pgrp, OS_FLAGS *flags, INT8U wait_type, INT16U timeout);
	void uc_FlagUnblock(OS_FLAG_GRP *pgrp);
	OS_FLAGS OSFlagAccept(OS_FLAG_GRP *pgrp, OS_FLAGS flags, INT8U wait_type, INT8U *perr);
	OS_FLAG_GRP *OSFlagCreate(OS_FLAGS flags, INT8U *perr);
	OS_FLAG_GRP *OSFlagDel(OS_FLAG_GRP *pgrp, INT8U opt, INT8U *perr);
	INT8U OSFlagNameGet(OS_FLAG_GRP *pgrp, INT8U *pname, INT8U *perr);
	void  OSFlagNameSet(OS_FLAG_GRP *pgrp, INT8U *pname, INT8U *perr);
	OS_FLAGS OSFlagPend(OS_FLAG_GRP *pgrp, OS_FLAGS flags, INT8U wait_type, INT16U timeout, INT8U *perr);
	OS_FLAGS OSFlagPendGetFlagsRdy(void);
	OS_FLAGS OSFlagPost(OS_FLAG_GRP *pgrp, OS_FLAGS flags, INT8U opt, INT8U *perr);
	OS_FLAGS OSFlagQuery(OS_FLAG_GRP *pgrp, INT8U *perr);

	// Message mailbox management
	void *OSMboxAccept(OS_EVENT *pevent);
	OS_EVENT *OSMboxCreate(void *pmsg);
	OS_EVENT *OSMboxDel(OS_EVENT *pevent, INT8U opt, INT8U *perr);
	void *OSMboxPend(OS_EVENT *pevent, INT16U timeout, INT8U *perr);
	INT8U OSMboxPendAbort(OS_EVENT *pevent, INT8U opt, INT8U *perr);
	INT8U OSMboxPost(OS_EVENT *pevent, void *pmsg);
	INT8U OSMboxPostOpt(OS_EVENT *pevent, void *pmsg, INT8U opt);
	INT8U OSMboxQuery(OS_EVENT *pevent, OS_MBOX_DATA *p_mbox_data);

	// Memory management
	OS_MEM *OSMemCreate(void *addr, INT32U nblks, INT32U blksize, INT8U *perr);
	void   *OSMemGet(OS_MEM *pmem, INT8U *perr);
	INT8U   OSMemNameGet(OS_MEM *pmem, INT8U *pname, INT8U *perr);
	void    OSMemNameSet(OS_MEM *pmem, INT8U *pname, INT8U *perr);
	INT8U   OSMemPut(OS_MEM *pmem, void *pblk);
	INT8U   OSMemQuery(OS_MEM *pmem, OS_MEM_DATA *p_mem_data);

	// Mutual exclusion semaphore management
	BOOLEAN   OSMutexAccept(OS_EVENT *pevent, INT8U *perr);
	OS_EVENT *OSMutexCreate(INT8U prio, INT8U *perr);
	OS_EVENT *OSMutexDel(OS_EVENT *pevent, INT8U opt, INT8U *perr);
	void  OSMutexPend(OS_EVENT *pevent, INT16U timeout, INT8U *perr);
	INT8U OSMutexPost(OS_EVENT *pevent);
	INT8U OSMutexQuery(OS_EVENT *pevent, OS_MUTEX_DATA *p_mutex_data);

	// Message queue management
	void *OSQAccept(OS_EVENT *pevent, INT8U *perr);
	OS_EVENT *OSQCreate(void **start, INT16U size);
	OS_EVENT *OSQDel(OS_EVENT *pevent, INT8U opt, INT8U *perr);
	INT8U OSQFlush(OS_EVENT *pevent);
	void *OSQPend(OS_EVENT *pevent, INT16U timeout, INT8U *perr);
	INT8U OSQPendAbort(OS_EVENT *pevent, INT8U opt, INT8U *perr);
	INT8U OSQPost(OS_EVENT *pevent, void *pmsg);
	INT8U OSQPostFront(OS_EVENT *pevent, void *pmsg);
	INT8U OSQPostOpt(OS_EVENT *pevent, void *pmsg, INT8U opt);
	INT8U OSQQuery(OS_EVENT *pevent, OS_Q_DATA *p_q_data);

	// Semaphore management
	INT16U OSSemAccept(OS_EVENT *pevent);
	OS_EVENT *OSSemCreate(INT16U cnt);
	OS_EVENT *OSSemDel(OS_EVENT *pevent, INT8U opt, INT8U *perr);
	void  OSSemPend(OS_EVENT *pevent, INT16U timeout, INT8U *perr);
	INT8U OSSemPendAbort(OS_EVENT *pevent, INT8U opt, INT8U *perr);
	INT8U OSSemPost(OS_EVENT *pevent);
	INT8U OSSemQuery(OS_EVENT *pevent, OS_SEM_DATA *p_sem_data);
	void  OSSemSet(OS_EVENT *pevent, INT16U cnt, INT8U *perr);

	// Task management
	INT8U OSTaskChangePrio(INT8U oldprio, INT8U newprio);
	INT8U OSTaskCreate(void (*task)(void *p_arg), void *p_arg, OS_STK *ptos, INT8U prio);
	INT8U OSTaskCreateExt(void (*task)(void *p_arg), void *p_arg, OS_STK *ptos, INT8U prio,
						INT16U id, OS_STK *pbos, INT32U stk_size, void *pext, INT16U opt);
	INT8U OSTaskDel(INT8U prio);
	INT8U OSTaskDelReq(INT8U prio);
	INT8U OSTaskNameGet(INT8U prio, INT8U *pname, INT8U *perr);
	void  OSTaskNameSet(INT8U prio, INT8U *pname, INT8U *perr);
	INT8U OSTaskResume(INT8U prio);
	INT8U OSTaskSuspend(INT8U prio);
	INT8U OSTaskStkChk(INT8U prio, OS_STK_DATA *p_stk_data);
	INT8U OSTaskQuery(INT8U prio, OS_TCB *p_task_data);

	// Time management
	void OSTimeDly(INT16U ticks);
	INT8U OSTimeDlyHMSM(INT8U hours, INT8U minutes, INT8U seconds, INT16U milli);
	INT8U OSTimeDlyResume(INT8U prio);
	INT32U OSTimeGet(void);
	void OSTimeSet(INT32U ticks);
	void OSTimeTick(void);

	// Timer management
	OS_TMR *OSTmrCreate(INT32U dly, INT32U period, INT8U opt, OS_TMR_CALLBACK callback,
						void *callback_arg, INT8U *pname, INT8U *perr);
	BOOLEAN OSTmrDel(OS_TMR *ptmr, INT8U *perr);
	INT8U OSTmrNameGet(OS_TMR *ptmr, INT8U *pdest, INT8U *perr);
	INT32U OSTmrRemainGet(OS_TMR *ptmr, INT8U *perr);
	INT8U OSTmrStateGet(OS_TMR *ptmr, INT8U *perr);
	BOOLEAN OSTmrStart(OS_TMR *ptmr, INT8U *perr);
	BOOLEAN OSTmrStop(OS_TMR *ptmr, INT8U opt, void *callback_arg, INT8U *perr);
	INT8U OSTmrSignal(void);

private:
	UC_rtos_class *m_parent_rtos;
};

#endif /* END OF _UC_MICROCOS_H */
