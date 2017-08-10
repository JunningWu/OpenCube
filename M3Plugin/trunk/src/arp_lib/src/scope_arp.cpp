#include "include/arp_functions.h"
#include "include/arp_callbacks.h"
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "scope_arp.h"

extern unsigned long long uc_segment_time;
extern unsigned long long uc_segment_instr;
unsigned long uc_get_base_clock_period();
extern "C" int uc_pthread_self();
int uc_pthread_self();
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static int prev_sid;
static int prev_value;
static long long total_time=0;
static char string[50];
static long long limit_time = 100000000;

/*class c1{
public:
	~c1(){
		printf("TOTAL CYCLES FROM ARP: %lld\n",total_time);
	}
};
c1 mi_c1;*/

void mps_arp_enterCB_playback(const arp_sid sid, void *data, void *context)
{

/*	
	if(*(int*)data==0) {
		std::cout << "entering sid: "<< sid <<" with time 0" << std::endl;
		assert(0);
	}
	uc_segment_time += *(int*)data;
	uc_segment_instr += uc_segment_time / uc_get_base_clock_period();
	prev_sid = sid;
	prev_value = *(int*)data;
	
	if(prev_value<0){
		printf("Error: received ARP value is negative\n");
		exit(-1);
	}


	total_time+=prev_value;
	if(total_time>limit_time){
		sprintf(string,"echo \"time %lld ns \" > time.dat",total_time);
		system(string);
		limit_time+=100000000;
	}

*/

//printf("entering sid: %d, thread:%d, time loaded: %d, segment time: %d \n",sid,uc_pthread_self(),*(int*)data,uc_segment_time);
//std::cout << "sid: "<< sid <<", thread: "<< uc_pthread_self() <<", time loaded: "<< *(int*)data<<std::endl;

}

void mps_arp_leaveCB_playback(const arp_sid sid, void *data, void *context)
{
	if(*(int*)data==0) {
		std::cout << "entering sid: "<< sid <<" with time 0" << std::endl;
		assert(0);
	}
	uc_segment_instr += *(int*)data ;
	uc_segment_time += *(int*)data * uc_get_base_clock_period();
	prev_sid = sid;
	prev_value = *(int*)data;
	
	if(prev_value<0){
		printf("Error: received ARP value is negative\n");
		exit(-1);
	}


	total_time+=prev_value;
/*	if(total_time>limit_time){
		sprintf(string,"echo \"time %lld ns \" > time.dat",total_time);
		system(string);
		limit_time+=100000000;
	}*/

//	if(prev_sid != sid) std::cerr << "Error: leaving wrong segment" << std::endl;
//	if(prev_value != *(int*)data) std::cerr << "Error: leaving  segment with wrong timing info" << std::endl;
/*std::cout << "exiting  sid: "<< sid <<", thread: "<< uc_pthread_self() <<", time loaded: "<< *(int*)data<<", segment time: "<< uc_segment_time <<std::endl;*/
}

void mps_arp_lock_callback(const void* data, void* context)
{
  pthread_mutex_lock(&mutex);
}

void mps_arp_unlock_callback(const void* data, void* context)
{
  pthread_mutex_unlock(&mutex);
}

void *createContest (){
  pthread_mutex_init(&mutex,NULL);
  return &mutex;	
}

void start_playback(){
	arp_add_callbacks(arp_uint, mps_arp_enterCB_playback, mps_arp_leaveCB_playback, 0, 0, 0);
  	arp_start(4, 148, arp_playback, 0, 0, mps_arp_lock_callback, mps_arp_unlock_callback, 0);
}