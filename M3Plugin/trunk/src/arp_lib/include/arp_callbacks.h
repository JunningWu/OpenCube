#ifndef _ARP_CALLBACKS_H_
#define _ARP_CALLBACKS_H_

#ifdef __cplusplus
extern "C" {
#endif

void mps_arp_enterCB_playback(const arp_sid sid, void *data, void *context);

void mps_arp_leaveCB_playback(const arp_sid sid, void *data, void* context);

void mps_arp_lock_callback(const void* data, void* context);

void mps_arp_unlock_callback(const void* data, void* context);

#ifdef __cplusplus
}
#endif

#endif


