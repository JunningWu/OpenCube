#include "include/arp_functions.h"
#include "include/arp_callbacks.h"

#include "scope_arp.h"

extern unsigned long long uc_segment_time;

static unsigned long long prev_value;

void mps_arp_enterCB_record(const arp_sid sid, void *data, void *context)
{
	//prev_value = uc_segment_time;

}

void mps_arp_leaveCB_record(const arp_sid sid, void *data, void *context)
{
	*(int*)data = uc_segment_time-prev_value;
	prev_value = uc_segment_time;
}

void start_record(){
	arp_add_callbacks(arp_uint, mps_arp_enterCB_record, mps_arp_leaveCB_record, 0, 0, 0);
  	arp_start(4, 148, arp_playback, 0, 0, mps_arp_lock_callback, mps_arp_unlock_callback, 0);
}
