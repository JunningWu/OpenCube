
#include <pthread.h>
#include <mqueue.h>
#include <time.h>
#include <errno.h>
				 
#include "../../../time_class.h"

extern int test_return;

void * test1(void *args) {
	mqd_t mq;
	
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 1;
	attr.mq_msgsize = sizeof(char);
	if ((mq = mq_open("/mqueue", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, & attr)) == (mqd_t)-1) {
		return NULL;
	}
	
	// RECEIVE
	struct timespec old_time;
	clock_gettime(CLOCK_REALTIME, &old_time);
	
	struct timespec wait_time;
	wait_time.tv_sec = 0;
	wait_time.tv_nsec = 500000000;
	
	// Testing timeout
	char foo;
	if (mq_timedreceive(mq, &foo, 1, NULL, &wait_time) != -1) {
		return NULL;
	}
	
	struct timespec current_time;
	clock_gettime(CLOCK_REALTIME, &current_time);
	
	if (current_time - old_time > wait_time) {
		return NULL;
	}
	
	// SEND
	clock_gettime(CLOCK_REALTIME, &old_time);
	// Filling the queue
	mq_send(mq, &foo, 1, 0);
	// Testing timeout
	if (mq_timedsend(mq, &foo, 1, 0, &wait_time) != -1) {
		return NULL;
	}
	
	clock_gettime(CLOCK_REALTIME, &current_time);
	
	if (current_time - old_time > wait_time) {
		return NULL;
	}
		
	test_return = 0;
	
	return NULL;
}
