
#include "../../../time_class.h"

#include  <ucos_ii.h>
#include  <time.h>

#define OS_TASK_DEF_STK_SIZE (2000)  /* Default stack size in words */
#define  TASK_STK_SIZE       1000*OS_TASK_DEF_STK_SIZE      /* Size of each task's stacks (# of WORDs)            */

OS_STK        TaskStartStk[TASK_STK_SIZE];

void  Task(void *data);                       /* Function prototypes of tasks                  */
void  TaskStart(void *data);                  /* Function prototypes of Startup task           */

extern int test_return;

void * test2(void *args) {
	struct timespec new_clocktime;
	new_clocktime.tv_sec = 3;
	new_clocktime.tv_nsec = 0;
	clock_settime(CLOCK_REALTIME, &new_clocktime);
	INT8U perr;
	OS_EVENT* mutex = OSMutexCreate(1, &perr);
	OSMutexPend(mutex, 0, &perr);

	struct timespec old_time;
	clock_gettime(CLOCK_REALTIME, &old_time);

	struct timespec wait_time;
	wait_time.tv_sec = 0;
	wait_time.tv_nsec = 500000000;

	// Testing timeout
	OSMutexPend(mutex, 50, &perr);

	struct timespec current_time;
	clock_gettime(CLOCK_REALTIME, &current_time);

	if (current_time - old_time > wait_time) {
		return NULL;
	}
	if (current_time == old_time) {
		return NULL;
	}

	test_return = 0;

	return NULL;
}

void * test1 (void *)
{
    OSInit();                                              /* Initialize uC/OS-II                      */
    OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 0);
    OSStart();                                             /* Start multitasking                       */
    return 0;
}

void  TaskStart (void *pdata)
{
    OSStatInit();                                          /* Initialize uC/OS-II's statistics         */
    test2(NULL);
}
