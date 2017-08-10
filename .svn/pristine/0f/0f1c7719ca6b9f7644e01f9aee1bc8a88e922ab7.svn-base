
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

extern int test_return;

sem_t sem;
int test_var = 0;

void *sem1(void*) {
	test_var++;
	if (test_var != 1) {
		return NULL;
	}
	
	sem_wait(&sem);
	
	test_var++;
	if (test_var != 2) {
		return NULL;
	}
	
	sleep(1);
	
	test_var++;
	if (test_var != 103) {
		return NULL;
	}
	
	sem_post(&sem);
	
	sleep(2);
	
	test_var++;
	if (test_var != 404) {
		return NULL;
	}
	
	test_return = 0;

	return NULL;
}

void *sem2(void*) {
	usleep(10000);
	
	test_var += 100;
	if (test_var != 102) {
		return NULL;
	}
	
	sem_wait(&sem);
	
	test_var += 100;
	if (test_var != 203) {
		return NULL;
	}
	
	sleep(1);
	
	test_var += 100;
	if (test_var != 303) {
		return NULL;
	}
	
	sem_post(&sem);
	
	test_var += 100;
	if (test_var != 403) {
		return NULL;
	}
	
	return NULL;
}

void * test1(void *args) {
	sem_init(&sem, 0, 1);
	
	pthread_t th1 ,th2;
	pthread_create(&th1, NULL, sem1, NULL);
	pthread_create(&th2, NULL, sem2, NULL);
	
	return NULL;
}
