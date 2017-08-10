//==========================================================================
//	uc_microcos_defines.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: lun ene 28 2008
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


#ifndef _UC_MICROCOS_DEFINES_H
#define _UC_MICROCOS_DEFINES_H

#include "rtos/api/posix/uc_signal.h"

/*
*********************************************************************************************************
*                                             MISCELLANEOUS
*********************************************************************************************************
*/

#ifdef   OS_GLOBALS
#define  OS_EXT
#else
#define  OS_EXT  extern
#endif

#ifndef  OS_FALSE
#define  OS_FALSE                     0u
#endif

#ifndef  OS_TRUE
#define  OS_TRUE                      1u
#endif

#define  OS_ASCII_NUL          (INT8U)0

#define  OS_PRIO_SELF              0xFFu                /* Indicate SELF priority                      */

// #if OS_TASK_STAT_EN > 0
// #define  OS_N_SYS_TASKS               2u                /* Number of system tasks                      */
// #else
// #define  OS_N_SYS_TASKS               1u
// #endif

#define  OS_TASK_STAT_PRIO  (OS_LOWEST_PRIO - 1)        /* Statistic task priority                     */
#define  OS_TASK_IDLE_PRIO  (OS_LOWEST_PRIO)            /* IDLE      task priority                     */

#if OS_LOWEST_PRIO <= 63
#define  OS_EVENT_TBL_SIZE ((OS_LOWEST_PRIO) / 8 + 1)   /* Size of event table                         */
#define  OS_RDY_TBL_SIZE   ((OS_LOWEST_PRIO) / 8 + 1)   /* Size of ready table                         */
#else
#define  OS_EVENT_TBL_SIZE ((OS_LOWEST_PRIO) / 16 + 1)  /* Size of event table                         */
#define  OS_RDY_TBL_SIZE   ((OS_LOWEST_PRIO) / 16 + 1)  /* Size of ready table                         */
#endif

#define  OS_TASK_IDLE_ID          65535u                /* ID numbers for Idle, Stat and Timer tasks   */
#define  OS_TASK_STAT_ID          65534u
#define  OS_TASK_TMR_ID           65533u

#define  OS_EVENT_EN       (((OS_Q_EN > 0) && (OS_MAX_QS > 0)) || (OS_MBOX_EN > 0) || (OS_SEM_EN > 0) || (OS_MUTEX_EN > 0))

#define  OS_TCB_RESERVED         ((OS_TCB *)1)

/*$PAGE*/
/*
*********************************************************************************************************
*                              TASK STATUS (Bit definition for OSTCBStat)
*********************************************************************************************************
*/
#define  OS_STAT_RDY               0x00u    /* Ready to run                                            */
#define  OS_STAT_SEM               0x01u    /* Pending on semaphore                                    */
#define  OS_STAT_MBOX              0x02u    /* Pending on mailbox                                      */
#define  OS_STAT_Q                 0x04u    /* Pending on queue                                        */
#define  OS_STAT_SUSPEND           0x08u    /* Task is suspended                                       */
#define  OS_STAT_MUTEX             0x10u    /* Pending on mutual exclusion semaphore                   */
#define  OS_STAT_FLAG              0x20u    /* Pending on event flag group                             */

#define  OS_STAT_PEND_ANY         (OS_STAT_SEM | OS_STAT_MBOX | OS_STAT_Q | OS_STAT_MUTEX | OS_STAT_FLAG)

/*
*********************************************************************************************************
*                           TASK PEND STATUS (Status codes for OSTCBStatPend)
*********************************************************************************************************
*/
#define  OS_STAT_PEND_OK              0u    /* Pending status OK, not pending, or pending complete     */
#define  OS_STAT_PEND_TO              1u    /* Pending timed out                                       */
#define  OS_STAT_PEND_ABORT           2u    /* Pending aborted                                         */

/*
*********************************************************************************************************
*                                        OS_EVENT types
*********************************************************************************************************
*/
#define  OS_EVENT_TYPE_UNUSED         0u
#define  OS_EVENT_TYPE_MBOX           1u
#define  OS_EVENT_TYPE_Q              2u
#define  OS_EVENT_TYPE_SEM            3u
#define  OS_EVENT_TYPE_MUTEX          4u
#define  OS_EVENT_TYPE_FLAG           5u

#define  OS_TMR_TYPE                100u    /* Used to identify Timers ...                             */
                                            /* ... (Must be different value than OS_EVENT_TYPE_xxx)    */

/*
*********************************************************************************************************
*                                         EVENT FLAGS
*********************************************************************************************************
*/
#define  OS_FLAG_WAIT_CLR_ALL         0u    /* Wait for ALL    the bits specified to be CLR (i.e. 0)   */
#define  OS_FLAG_WAIT_CLR_AND         0u

#define  OS_FLAG_WAIT_CLR_ANY         1u    /* Wait for ANY of the bits specified to be CLR (i.e. 0)   */
#define  OS_FLAG_WAIT_CLR_OR          1u

#define  OS_FLAG_WAIT_SET_ALL         2u    /* Wait for ALL    the bits specified to be SET (i.e. 1)   */
#define  OS_FLAG_WAIT_SET_AND         2u

#define  OS_FLAG_WAIT_SET_ANY         3u    /* Wait for ANY of the bits specified to be SET (i.e. 1)   */
#define  OS_FLAG_WAIT_SET_OR          3u


#define  OS_FLAG_CONSUME           0x80u    /* Consume the flags if condition(s) satisfied             */


#define  OS_FLAG_CLR                  0u
#define  OS_FLAG_SET                  1u

/*
*********************************************************************************************************
*                                   Values for OSTickStepState
*
* Note(s): This feature is used by uC/OS-View.
*********************************************************************************************************
*/

#if OS_TICK_STEP_EN > 0
#define  OS_TICK_STEP_DIS             0u    /* Stepping is disabled, tick runs as mormal               */
#define  OS_TICK_STEP_WAIT            1u    /* Waiting for uC/OS-View to set OSTickStepState to _ONCE  */
#define  OS_TICK_STEP_ONCE            2u    /* Process tick once and wait for next cmd from uC/OS-View */
#endif

/*
*********************************************************************************************************
*       Possible values for 'opt' argument of OSSemDel(), OSMboxDel(), OSQDel() and OSMutexDel()
*********************************************************************************************************
*/
#define  OS_DEL_NO_PEND               0u
#define  OS_DEL_ALWAYS                1u

/*
*********************************************************************************************************
*                                        OS???Pend() OPTIONS
*
* These #defines are used to establish the options for OS???PendAbort().
*********************************************************************************************************
*/
#define  OS_PEND_OPT_NONE             0u    /* NO option selected                                      */
#define  OS_PEND_OPT_BROADCAST        1u    /* Broadcast action to ALL tasks waiting                   */

/*
*********************************************************************************************************
*                                     OS???PostOpt() OPTIONS
*
* These #defines are used to establish the options for OSMboxPostOpt() and OSQPostOpt().
*********************************************************************************************************
*/
#define  OS_POST_OPT_NONE          0x00u    /* NO option selected                                      */
#define  OS_POST_OPT_BROADCAST     0x01u    /* Broadcast message to ALL tasks waiting                  */
#define  OS_POST_OPT_FRONT         0x02u    /* Post to highest priority task waiting                   */
#define  OS_POST_OPT_NO_SCHED      0x04u    /* Do not call the scheduler if this option is selected    */

/*
*********************************************************************************************************
*                                 TASK OPTIONS (see OSTaskCreateExt())
*********************************************************************************************************
*/
#define  OS_TASK_OPT_NONE        0x0000u    /* NO option selected                                      */
#define  OS_TASK_OPT_STK_CHK     0x0001u    /* Enable stack checking for the task                      */
#define  OS_TASK_OPT_STK_CLR     0x0002u    /* Clear the stack when the task is create                 */
#define  OS_TASK_OPT_SAVE_FP     0x0004u    /* Save the contents of any floating-point registers       */

/*
*********************************************************************************************************
*                            TIMER OPTIONS (see OSTmrStart() and OSTmrStop())
*********************************************************************************************************
*/
#define  OS_TMR_OPT_NONE              0u    /* No option selected                                      */

#define  OS_TMR_OPT_ONE_SHOT          1u    /* Timer will not automatically restart when it expires    */
#define  OS_TMR_OPT_PERIODIC          2u    /* Timer will     automatically restart when it expires    */

#define  OS_TMR_OPT_CALLBACK          3u    /* OSTmrStop() option to call 'callback' w/ timer arg.     */
#define  OS_TMR_OPT_CALLBACK_ARG      4u    /* OSTmrStop() option to call 'callback' w/ new   arg.     */

/*
*********************************************************************************************************
*                                            TIMER STATES
*********************************************************************************************************
*/
#define  OS_TMR_STATE_UNUSED          0u
#define  OS_TMR_STATE_STOPPED         1u
#define  OS_TMR_STATE_COMPLETED       2u
#define  OS_TMR_STATE_RUNNING         3u

/*
*********************************************************************************************************
*                                             ERROR CODES
*********************************************************************************************************
*/
#define OS_ERR_NONE                   0u

#define OS_ERR_EVENT_TYPE             1u
#define OS_ERR_PEND_ISR               2u
#define OS_ERR_POST_NULL_PTR          3u
#define OS_ERR_PEVENT_NULL            4u
#define OS_ERR_POST_ISR               5u
#define OS_ERR_QUERY_ISR              6u
#define OS_ERR_INVALID_OPT            7u
#define OS_ERR_PDATA_NULL             9u

#define OS_ERR_TIMEOUT               10u
#define OS_ERR_EVENT_NAME_TOO_LONG   11u
#define OS_ERR_PNAME_NULL            12u
#define OS_ERR_PEND_LOCKED           13u
#define OS_ERR_PEND_ABORT            14u
#define OS_ERR_DEL_ISR               15u
#define OS_ERR_CREATE_ISR            16u
#define OS_ERR_NAME_GET_ISR          17u
#define OS_ERR_NAME_SET_ISR          18u

#define OS_ERR_MBOX_FULL             20u

#define OS_ERR_Q_FULL                30u
#define OS_ERR_Q_EMPTY               31u

#define OS_ERR_PRIO_EXIST            40u
#define OS_ERR_PRIO                  41u
#define OS_ERR_PRIO_INVALID          42u

#define OS_ERR_SEM_OVF               50u

#define OS_ERR_TASK_CREATE_ISR       60u
#define OS_ERR_TASK_DEL              61u
#define OS_ERR_TASK_DEL_IDLE         62u
#define OS_ERR_TASK_DEL_REQ          63u
#define OS_ERR_TASK_DEL_ISR          64u
#define OS_ERR_TASK_NAME_TOO_LONG    65u
#define OS_ERR_TASK_NO_MORE_TCB      66u
#define OS_ERR_TASK_NOT_EXIST        67u
#define OS_ERR_TASK_NOT_SUSPENDED    68u
#define OS_ERR_TASK_OPT              69u
#define OS_ERR_TASK_RESUME_PRIO      70u
#define OS_ERR_TASK_SUSPEND_IDLE     71u
#define OS_ERR_TASK_SUSPEND_PRIO     72u
#define OS_ERR_TASK_WAITING          73u

#define OS_ERR_TIME_NOT_DLY          80u
#define OS_ERR_TIME_INVALID_MINUTES  81u
#define OS_ERR_TIME_INVALID_SECONDS  82u
#define OS_ERR_TIME_INVALID_MS       83u
#define OS_ERR_TIME_ZERO_DLY         84u
#define OS_ERR_TIME_DLY_ISR          85u

#define OS_ERR_MEM_INVALID_PART      90u
#define OS_ERR_MEM_INVALID_BLKS      91u
#define OS_ERR_MEM_INVALID_SIZE      92u
#define OS_ERR_MEM_NO_FREE_BLKS      93u
#define OS_ERR_MEM_FULL              94u
#define OS_ERR_MEM_INVALID_PBLK      95u
#define OS_ERR_MEM_INVALID_PMEM      96u
#define OS_ERR_MEM_INVALID_PDATA     97u
#define OS_ERR_MEM_INVALID_ADDR      98u
#define OS_ERR_MEM_NAME_TOO_LONG     99u

#define OS_ERR_NOT_MUTEX_OWNER      100u

#define OS_ERR_FLAG_INVALID_PGRP    110u
#define OS_ERR_FLAG_WAIT_TYPE       111u
#define OS_ERR_FLAG_NOT_RDY         112u
#define OS_ERR_FLAG_INVALID_OPT     113u
#define OS_ERR_FLAG_GRP_DEPLETED    114u
#define OS_ERR_FLAG_NAME_TOO_LONG   115u

#define OS_ERR_PIP_LOWER            120u

#define OS_ERR_TMR_INVALID_DLY      130u
#define OS_ERR_TMR_INVALID_PERIOD   131u
#define OS_ERR_TMR_INVALID_OPT      132u
#define OS_ERR_TMR_INVALID_NAME     133u
#define OS_ERR_TMR_NON_AVAIL        134u
#define OS_ERR_TMR_INACTIVE         135u
#define OS_ERR_TMR_INVALID_DEST     136u
#define OS_ERR_TMR_INVALID_TYPE     137u
#define OS_ERR_TMR_INVALID          138u
#define OS_ERR_TMR_ISR              139u
#define OS_ERR_TMR_NAME_TOO_LONG    140u
#define OS_ERR_TMR_INVALID_STATE    141u
#define OS_ERR_TMR_STOPPED          142u
#define OS_ERR_TMR_NO_CALLBACK      143u

/*
*********************************************************************************************************
*                                    OLD ERROR CODE NAMES (< V2.84)
*********************************************************************************************************
*/
#define OS_NO_ERR                    OS_ERR_NONE
#define OS_TIMEOUT                   OS_ERR_TIMEOUT
#define OS_TASK_NOT_EXIST            OS_ERR_TASK_NOT_EXIST
#define OS_MBOX_FULL                 OS_ERR_MBOX_FULL
#define OS_Q_FULL                    OS_ERR_Q_FULL
#define OS_Q_EMPTY                   OS_ERR_Q_EMPTY
#define OS_PRIO_EXIST                OS_ERR_PRIO_EXIST
#define OS_PRIO_ERR                  OS_ERR_PRIO
#define OS_PRIO_INVALID              OS_ERR_PRIO_INVALID
#define OS_SEM_OVF                   OS_ERR_SEM_OVF
#define OS_TASK_DEL_ERR              OS_ERR_TASK_DEL
#define OS_TASK_DEL_IDLE             OS_ERR_TASK_DEL_IDLE
#define OS_TASK_DEL_REQ              OS_ERR_TASK_DEL_REQ
#define OS_TASK_DEL_ISR              OS_ERR_TASK_DEL_ISR
#define OS_NO_MORE_TCB               OS_ERR_TASK_NO_MORE_TCB
#define OS_TIME_NOT_DLY              OS_ERR_TIME_NOT_DLY
#define OS_TIME_INVALID_MINUTES      OS_ERR_TIME_INVALID_MINUTES
#define OS_TIME_INVALID_SECONDS      OS_ERR_TIME_INVALID_SECONDS
#define OS_TIME_INVALID_MS           OS_ERR_TIME_INVALID_MS
#define OS_TIME_ZERO_DLY             OS_ERR_TIME_ZERO_DLY
#define OS_TASK_SUSPEND_PRIO         OS_ERR_TASK_SUSPEND_PRIO
#define OS_TASK_SUSPEND_IDLE         OS_ERR_TASK_SUSPEND_IDLE
#define OS_TASK_RESUME_PRIO          OS_ERR_TASK_RESUME_PRIO
#define OS_TASK_NOT_SUSPENDED        OS_ERR_TASK_NOT_SUSPENDED
#define OS_MEM_INVALID_PART          OS_ERR_MEM_INVALID_PART
#define OS_MEM_INVALID_BLKS          OS_ERR_MEM_INVALID_BLKS
#define OS_MEM_INVALID_SIZE          OS_ERR_MEM_INVALID_SIZE
#define OS_MEM_NO_FREE_BLKS          OS_ERR_MEM_NO_FREE_BLKS
#define OS_MEM_FULL                  OS_ERR_MEM_FULL
#define OS_MEM_INVALID_PBLK          OS_ERR_MEM_INVALID_PBLK
#define OS_MEM_INVALID_PMEM          OS_ERR_MEM_INVALID_PMEM
#define OS_MEM_INVALID_PDATA         OS_ERR_MEM_INVALID_PDATA
#define OS_MEM_INVALID_ADDR          OS_ERR_MEM_INVALID_ADDR
#define OS_MEM_NAME_TOO_LONG         OS_ERR_MEM_NAME_TOO_LONG
#define OS_TASK_OPT_ERR              OS_ERR_TASK_OPT
#define OS_FLAG_INVALID_PGRP         OS_ERR_FLAG_INVALID_PGRP
#define OS_FLAG_ERR_WAIT_TYPE        OS_ERR_FLAG_WAIT_TYPE
#define OS_FLAG_ERR_NOT_RDY          OS_ERR_FLAG_NOT_RDY
#define OS_FLAG_INVALID_OPT          OS_ERR_FLAG_INVALID_OPT
#define OS_FLAG_GRP_DEPLETED         OS_ERR_FLAG_GRP_DEPLETED


/*
 * DATA TYPES (Compiler and architecture specific)
 */
typedef unsigned char  		BOOLEAN;
typedef unsigned char	  	INT8U;      /* Unsigned  8 bit quantity          */
typedef signed   char  		INT8S;      /* Signed    8 bit quantity          */
typedef unsigned short int   	INT16U;     /* Unsigned 16 bit quantity          */
typedef signed   short int   	INT16S;     /* Signed   16 bit quantity          */
typedef unsigned int  		INT32U;     /* Unsigned 32 bit quantity          */
typedef signed   int  		INT32S;     /* Signed   32 bit quantity          */
typedef float          		FP32;       /* Single precision floating point   */
typedef INT32U  		OS_STK;     /* Each stack entry is 32 bit wide   */
typedef uc_sigset_t		OS_CPU_SR;  // The 'status register' corresponds to signals



                                       /* ---------------------- MISCELLANEOUS ----------------------- */
#define OS_APP_HOOKS_EN           1    /* Application-defined hooks are called from the uC/OS-II hooks */
#define OS_ARG_CHK_EN             0    /* Enable (1) or Disable (0) argument checking                  */
#define OS_CPU_HOOKS_EN           1    /* uC/OS-II hooks are found in the processor port files         */

#define OS_DEBUG_EN               1    /* Enable(1) debug variables                                    */

#define OS_EVENT_NAME_SIZE       16    /* Determine the size of the name of a Sem, Mutex, Mbox or Q    */

#define OS_LOWEST_PRIO           63    /* Defines the lowest priority that can be assigned ...         */
                                       /* ... MUST NEVER be higher than 254!                           */

#define OS_MAX_EVENTS            10    /* Max. number of event control blocks in your application      */
#define OS_MAX_FLAGS              5    /* Max. number of Event Flag Groups    in your application      */
#define OS_MAX_MEM_PART           5    /* Max. number of memory partitions                             */
#define OS_MAX_QS                 4    /* Max. number of queue control blocks in your application      */
#define OS_MAX_TASKS             20    /* Max. number of tasks in your application, MUST be >= 2       */

#define OS_SCHED_LOCK_EN          1    /*     Include code for OSSchedLock() and OSSchedUnlock()       */

#define OS_TICK_STEP_EN           1    /* Enable tick stepping feature for uC/OS-View                  */
#define OS_TICKS_PER_SEC        100    /* Set the number of ticks in one second                        */


                                       /* --------------------- TASK STACK SIZE ---------------------- */
#define OS_TASK_TMR_STK_SIZE    128    /* Timer      task stack size (# of OS_STK wide entries)        */
#define OS_TASK_STAT_STK_SIZE   128    /* Statistics task stack size (# of OS_STK wide entries)        */
#define OS_TASK_IDLE_STK_SIZE   128    /* Idle       task stack size (# of OS_STK wide entries)        */


                                       /* --------------------- TASK MANAGEMENT ---------------------- */
#define OS_TASK_CHANGE_PRIO_EN    1    /*     Include code for OSTaskChangePrio()                      */
#define OS_TASK_CREATE_EN         1    /*     Include code for OSTaskCreate()                          */
#define OS_TASK_CREATE_EXT_EN     1    /*     Include code for OSTaskCreateExt()                       */
#define OS_TASK_DEL_EN            1    /*     Include code for OSTaskDel()                             */
#define OS_TASK_NAME_SIZE        16    /*     Determine the size of a task name                        */
#define OS_TASK_PROFILE_EN        1    /*     Include variables in OS_TCB for profiling                */
#define OS_TASK_QUERY_EN          1    /*     Include code for OSTaskQuery()                           */
#define OS_TASK_STAT_EN           1    /*     Enable (1) or Disable(0) the statistics task             */
#define OS_TASK_STAT_STK_CHK_EN   1    /*     Check task stacks from statistic task                    */
#define OS_TASK_SUSPEND_EN        1    /*     Include code for OSTaskSuspend() and OSTaskResume()      */
#define OS_TASK_SW_HOOK_EN        1    /*     Include code for OSTaskSwHook()                          */


                                       /* ----------------------- EVENT FLAGS ------------------------ */
#define OS_FLAG_EN                1    /* Enable (1) or Disable (0) code generation for EVENT FLAGS    */
#define OS_FLAG_ACCEPT_EN         1    /*     Include code for OSFlagAccept()                          */
#define OS_FLAG_DEL_EN            1    /*     Include code for OSFlagDel()                             */
#define OS_FLAG_NAME_SIZE        16    /*     Determine the size of the name of an event flag group    */
#define OS_FLAGS_NBITS           16    /*     Size in #bits of OS_FLAGS data type (8, 16 or 32)        */
#define OS_FLAG_QUERY_EN          1    /*     Include code for OSFlagQuery()                           */
#define OS_FLAG_WAIT_CLR_EN       1    /*     Include code for Wait on Clear EVENT FLAGS               */


                                       /* -------------------- MESSAGE MAILBOXES --------------------- */
#define OS_MBOX_EN                1    /* Enable (1) or Disable (0) code generation for MAILBOXES      */
#define OS_MBOX_ACCEPT_EN         1    /*     Include code for OSMboxAccept()                          */
#define OS_MBOX_DEL_EN            1    /*     Include code for OSMboxDel()                             */
#define OS_MBOX_PEND_ABORT_EN     1    /*     Include code for OSMboxPendAbort()                       */
#define OS_MBOX_POST_EN           1    /*     Include code for OSMboxPost()                            */
#define OS_MBOX_POST_OPT_EN       1    /*     Include code for OSMboxPostOpt()                         */
#define OS_MBOX_QUERY_EN          1    /*     Include code for OSMboxQuery()                           */


                                       /* --------------------- MEMORY MANAGEMENT -------------------- */
#define OS_MEM_EN                 1    /* Enable (1) or Disable (0) code generation for MEMORY MANAGER */
#define OS_MEM_NAME_SIZE         16    /*     Determine the size of a memory partition name            */
#define OS_MEM_QUERY_EN           1    /*     Include code for OSMemQuery()                            */


                                       /* ---------------- MUTUAL EXCLUSION SEMAPHORES --------------- */
#define OS_MUTEX_EN               1    /* Enable (1) or Disable (0) code generation for MUTEX          */
#define OS_MUTEX_ACCEPT_EN        1    /*     Include code for OSMutexAccept()                         */
#define OS_MUTEX_DEL_EN           1    /*     Include code for OSMutexDel()                            */
#define OS_MUTEX_QUERY_EN         1    /*     Include code for OSMutexQuery()                          */


                                       /* ---------------------- MESSAGE QUEUES ---------------------- */
#define OS_Q_EN                   1    /* Enable (1) or Disable (0) code generation for QUEUES         */
#define OS_Q_ACCEPT_EN            1    /*     Include code for OSQAccept()                             */
#define OS_Q_DEL_EN               1    /*     Include code for OSQDel()                                */
#define OS_Q_FLUSH_EN             1    /*     Include code for OSQFlush()                              */
#define OS_Q_PEND_ABORT_EN        1    /*     Include code for OSQPendAbort()                          */
#define OS_Q_POST_EN              1    /*     Include code for OSQPost()                               */
#define OS_Q_POST_FRONT_EN        1    /*     Include code for OSQPostFront()                          */
#define OS_Q_POST_OPT_EN          1    /*     Include code for OSQPostOpt()                            */
#define OS_Q_QUERY_EN             1    /*     Include code for OSQQuery()                              */


                                       /* ------------------------ SEMAPHORES ------------------------ */
#define OS_SEM_EN                 1    /* Enable (1) or Disable (0) code generation for SEMAPHORES     */
#define OS_SEM_ACCEPT_EN          1    /*    Include code for OSSemAccept()                            */
#define OS_SEM_DEL_EN             1    /*    Include code for OSSemDel()                               */
#define OS_SEM_PEND_ABORT_EN      1    /*    Include code for OSSemPendAbort()                         */
#define OS_SEM_QUERY_EN           1    /*    Include code for OSSemQuery()                             */
#define OS_SEM_SET_EN             1    /*    Include code for OSSemSet()                               */


                                       /* --------------------- TIME MANAGEMENT ---------------------- */
#define OS_TIME_DLY_HMSM_EN       1    /*     Include code for OSTimeDlyHMSM()                         */
#define OS_TIME_DLY_RESUME_EN     1    /*     Include code for OSTimeDlyResume()                       */
#define OS_TIME_GET_SET_EN        1    /*     Include code for OSTimeGet() and OSTimeSet()             */
#define OS_TIME_TICK_HOOK_EN      1    /*     Include code for OSTimeTickHook()                        */


                                       /* --------------------- TIMER MANAGEMENT --------------------- */
#define OS_TMR_EN                 1    /* Enable (1) or Disable (0) code generation for TIMERS         */
#define OS_TMR_CFG_MAX           16    /*     Maximum number of timers                                 */
#define OS_TMR_CFG_NAME_SIZE     16    /*     Determine the size of a timer name                       */
#define OS_TMR_CFG_WHEEL_SIZE     8    /*     Size of timer wheel (#Spokes)                            */
#define OS_TMR_CFG_TICKS_PER_SEC 10    /*     Rate at which timer management task runs (Hz)            */



#endif /* END OF _MICROCOS_DEFINES_H */
