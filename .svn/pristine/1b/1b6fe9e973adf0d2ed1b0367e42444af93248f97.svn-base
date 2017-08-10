
#include <pthread.h>
#include <time.h>
#include <errno.h>

#include "../../../time_class.h"

extern int test_return;

void * test1(void *args) {
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, NULL);

	struct timespec old_time;
	clock_gettime(CLOCK_REALTIME, &old_time);

	struct timespec wait_time;
	wait_time.tv_sec = 0;
	wait_time.tv_nsec = 500000000;
	// First lock
	pthread_mutex_lock(&mutex);
	// Testing timeout
	if (pthread_mutex_timedlock(&mutex, &wait_time) != ETIMEDOUT) {
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
