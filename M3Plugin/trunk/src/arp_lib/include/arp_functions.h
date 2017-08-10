#ifndef _ARP_FUNCTIONS_H_
#define _ARP_FUNCTIONS_H_


#ifdef __cplusplus
extern "C" {
#endif

/*
 * Type definitions.
 */
typedef unsigned short arp_tid;  /* Thread ids */
typedef unsigned short arp_sid;  /* Section ids */
typedef int            arp_lidx; /* Loop index */

typedef enum _arp_data_type
             { arp_char,  arp_uchar, 
               arp_short, arp_ushort, 
               arp_int,   arp_uint,
               arp_long,  arp_ulong, 
               arp_llong, arp_ullong, 
               arp_float, arp_double, 
               arp_ldouble } arp_data_type;

typedef enum _arp_mode
             { arp_record_average,
               arp_record_all, 
               arp_record_histogram,
               arp_playback } arp_mode;

// typedef enum _arp_file_format
//              { arp_ascii,
//                arp_binary } arp_file_format;

typedef enum _arp_error_t
             { arp_warning, arp_error_fatal, arp_error_non_fatal,
               arp_error_syntax, arp_error_io } arp_error_t;

typedef void (*arp_enter_callback) (const arp_sid, void* data, void* context);
typedef void (*arp_leave_callback) (const arp_sid, void* data, void* context);
typedef void (*arp_spawn_callback) (const arp_tid, void** contextp);
typedef void (*arp_join_callback)  (const arp_tid, void** contextp);

typedef void (*arp_error_callback) (const arp_error_t, const char* msg, void* context);
typedef void (*arp_lock_callback)  (const void*, void*);
typedef void (*arp_unlock_callback)(const void*, void*);

/*
 * Functions.
 */

extern void  arp_start       (const arp_tid,       /* Maximum thread id */
                              const arp_sid,       /* Maximum section id */
                              const arp_mode,
                              arp_error_callback, 
                              void* error_context,
                              arp_lock_callback,
                              arp_unlock_callback,
                              void* lock_context);

extern void  arp_finish      ();

extern void arp_add_callbacks(const arp_data_type, 
                              arp_enter_callback, 
                              arp_leave_callback, 
                              arp_spawn_callback,
                              arp_join_callback, 
                              void* context);

extern void     arp_spawned       (const arp_tid, const arp_tid); 
extern void     arp_join          (const arp_tid);

extern void     arp_enter         (const arp_tid, const arp_sid, const arp_lidx);
extern void     arp_enter_kernel  (const arp_tid, const arp_sid, const arp_lidx);
//extern void     arp_enter_profiled(const arp_tid, const arp_sid, const arp_lidx);
extern void     arp_leave         (const arp_tid, const arp_sid); 
extern void     arp_leave_kernel  (const arp_tid, const arp_sid); 
//extern void     arp_leave_profiled(const arp_tid, const arp_sid); 


#ifdef __cplusplus
}
#endif

#endif


