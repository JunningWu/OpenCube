
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
				 
#include "../../../time_class.h"

extern int test_return;

void * test1(void *args) {
	sem_t sem;
	sem_init(&sem, 0, 0);
	
	struct timespec old_time;
	clock_gettime(CLOCK_REALTIME, &old_time);
	
	struct timespec wait_time;
	wait_time.tv_sec = 0;
	wait_time.tv_nsec = 500000000;
	if (sem_timedwait(&sem, &wait_time) != -1) {
		return NULL;
	}
	
	struct timespec current_time;
	clock_gettime(CLOCK_REALTIME, &current_time);
	
	if (current_time - old_time > wait_time) {
		return NULL;
	}
	
	test_return = 0;
	
	return NULL;
}
