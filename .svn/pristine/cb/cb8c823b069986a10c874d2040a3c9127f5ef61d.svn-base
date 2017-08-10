//==========================================================================
// bubble.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: vie feb 22 2008
//	Description:
//==========================================================================
//  The following code is derived, directly or indirectly, from PERFidiX,
//  released June 30, 2006. Copyright (C) 2006 Design of Systems on Silicon(DS2)
//  The Initial Developer of the Original Code is the University of Cantabria
//  for Design of Systems on Silicon (DS2)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//  For more information about SCope you can visit
//  http://www.teisa.unican.es/scope or write an e-mail to
//  scope@teisa.unican.es or a letter to SCope, GIM - TEISA, University
//  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
//==========================================================================


/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                           (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
*                                               EXAMPLE #1
*********************************************************************************************************
*/

#include <iostream>
using namespace std;
#include "stdlib.h"

#include  <ucos_ii.h>


/** the ugly output stuff */
// #include "term_display.h"

/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

#define OS_TASK_DEF_STK_SIZE (2000)  /* Default stack size in words */
#define  TASK_STK_SIZE          10000*OS_TASK_DEF_STK_SIZE      /* Size of each task's stacks (# of WORDs)            */
#define  N_TASKS                        3       /* Number of identical tasks                          */

/*
*********************************************************************************************************
*                                               VARIABLES
*********************************************************************************************************
*/

OS_STK        TaskStk[N_TASKS][TASK_STK_SIZE];        /* Tasks stacks                                  */
OS_STK        TaskStartStk[TASK_STK_SIZE];
char          TaskData[N_TASKS];                      /* Parameters to pass to each task               */
//OS_EVENT     *RandomSem;

/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

        void  Task(void *data);                       /* Function prototypes of tasks                  */
        void  TaskStart(void *data);                  /* Function prototypes of Startup task           */
static  void  TaskStartCreateTasks(void);

/*
*********************************************************************************************************
*                                                MAIN
*********************************************************************************************************
*/

OS_EVENT *RandomSem;
int bubble_main (void)
{
//     PC_DispClrScr();      /* Clear the screen                         */

    OSInit();                                              /* Initialize uC/OS-II                      */

//     PC_DOSSaveReturn();                                    /* Save environment to return to DOS        */
    //PC_VectSet(uCOS, OSCtxSw);                             /* Install uC/OS-II's context switch vector */

    RandomSem   = OSSemCreate(1);                          /* Random number semaphore                  */

    OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 0);
    OSStart();                                             /* Start multitasking                       */
    return 0;
}


/*
*********************************************************************************************************
*                                              STARTUP TASK
*********************************************************************************************************
*/
void  TaskStart (void *pdata)
{
// #if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
//     OS_CPU_SR  cpu_sr;
// #endif

//     pdata = pdata;                                         /* Prevent compiler warning                 */

//     OS_ENTER_CRITICAL();
//     PC_VectSet(0x08, (void*)OSTickISR);                           /* Install uC/OS-II's clock tick ISR        */
//     PC_SetTickRate(OS_TICKS_PER_SEC);                      /* Reprogram tick rate                      */
//     OS_EXIT_CRITICAL();

    OSStatInit();                                          /* Initialize uC/OS-II's statistics         */

    TaskStartCreateTasks();                                /* Create all the application tasks         */

    for (;;) {
        OSTimeDlyHMSM(0, 10, 0, 0);                         /* Wait one second                          */
    }
}

/*
*********************************************************************************************************
*                                             CREATE TASKS
*********************************************************************************************************
*/

static  void  TaskStartCreateTasks (void)
{
    INT8U  i;

    for (i = 0; i < N_TASKS; i++) {                        /* Create N_TASKS identical tasks           */
        TaskData[i] = '0' + i;                             /* Each task will display its own letter    */
        OSTaskCreate(Task, (void *)&TaskData[i], &TaskStk[i][TASK_STK_SIZE - 1], i + 1);
    }
}

/*
*********************************************************************************************************
*                                                  TASKS
*********************************************************************************************************
*/

int final = 0;
#define N_ELEM 5000

void  Task (void *pdata)
{
	int *a = new int[N_ELEM];
	int i,j,aux;

	for (i=0 ; i<N_ELEM ; i++) {
		a[i] = N_ELEM-i;
	}
	aux = 0;
	for (i=1 ; i<N_ELEM ; i++) {
		for (j=0 ; j<N_ELEM-i ; j++) {
			if (a[j] > a[j+1]) {
				aux = a[j];
				a[j] = a[j+1];
				a[j+1] = aux;
			}
		}
	}
	cout << "Thread finished" << endl;
	INT8U err;
	OSSemPend(RandomSem, 0, &err);           /* Acquire semaphore to perform random numbers        */
	final++;
	if (final == 3) {
 		exit(0);
	}
	OSSemPost(RandomSem);                    /* Release semaphore                                  */
	while (true) {
		OSTimeDlyHMSM(0, 10, 0, 0);
	}
}

void OSTaskSwHook() {}
void OSTaskStatHook() {}
