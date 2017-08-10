
#include "sys_uc_select.h"

/*
 * This file contains the procedures for the handling of select and poll
 *
 * Based on Lunux file select.c
 *
 * select.c: Created for Linux based loosely upon Mathius Lattner's minix
 * patches by Peter MacDonald. Heavily edited by Linus.
 *
 */

#include "rtos/drivers/uc_fops.h"
#include "rtos/drivers/uc_poll.h"
#include <errno.h>
#include "uc_posix.h"
#include "linux_uc_smp.h"

#include "rtos/kernel/uc_rtos_class.h"

extern __thread UC_rtos_class *qt_parent_rtos;


#define FDS_IN(fds, n)          (fds->in + n)
#define FDS_OUT(fds, n)         (fds->out + n)
#define FDS_EX(fds, n)          (fds->ex + n)

#define BITS(fds, n)    (*FDS_IN(fds, n)|*FDS_OUT(fds, n)|*FDS_EX(fds, n))
 
#undef __NFDBITS
#define __NFDBITS       (8 * sizeof(unsigned long))

#undef __FD_SETSIZE
#define __FD_SETSIZE    1024

#undef __FDSET_LONGS
#define __FDSET_LONGS   (__FD_SETSIZE/__NFDBITS)


#define FDS_IN(fds, n)          (fds->in + n)
#define FDS_OUT(fds, n)          (fds->out + n)
#define FDS_EX(fds, n)          (fds->ex + n)

#define BITS(fds, n)     (*FDS_IN(fds, n)|*FDS_OUT(fds, n)|*FDS_EX(fds, n))

#define BIT(i)          (1UL << ((i)&(__NFDBITS-1)))
#define MEM(i,m)     ((m)+(unsigned)(i)/__NFDBITS)
#define ISSET(i,m)     (((i)&*(m)) != 0)
#define SET(i,m)     (*(m) |= (i))

#define POLLIN_SET (POLLRDNORM | POLLRDBAND | POLLIN | POLLHUP | POLLERR)
#define POLLOUT_SET (POLLWRBAND | POLLWRNORM | POLLOUT | POLLERR)
#define POLLEX_SET (POLLPRI)
#define DEFAULT_POLLMASK (POLLIN | POLLOUT | POLLRDNORM | POLLWRNORM)

int file_poll(int fd, poll_table *pt){
	struct uc_device_t *elem;

	for (int i = 0; i < qt_parent_rtos->m_dev_list.size(); i++) {
		elem = qt_parent_rtos->m_dev_list[i];
		if (elem->fd == fd) {
			return elem->dev_file.f_op->uc_poll(&elem->dev_file,pt);

		}
	}
	return 0;
}

int UC_posix_class::uc_select(int n, fd_set *inp, fd_set *outp,
                       fd_set *exp, struct timeval *tvp)
{
    struct poll_wqueues table;	
    poll_table *wait;
    int retval, i, j, id_1, id_2, id_3;
    int desp, fd, mask, mask_aux;
    long __timeout=0;

    poll_initwait(&table);
    wait = &table.pt;
	
    if(tvp!=NULL){
	__timeout=tvp->tv_sec*1000000;
	__timeout+=tvp->tv_usec/1000;
    }

    if (!__timeout)
         wait = NULL;
    retval = 0;

    for (;;)  {

	set_current_state(TASK_INTERRUPTIBLE);

	for(i=0;i<__FDSET_LONGS;i++){
		id_1=0;
		id_2=0;
		id_3=0;
		if(inp!=NULL)  id_1=inp->fds_bits[i];
		if(outp!=NULL) id_2=outp->fds_bits[i];
		if(exp!=NULL)  id_3=exp->fds_bits[i];
		if(id_1 || id_2 || id_3 ){
			for(int j=0;j<32;j++){
				desp = 1<<j;
				mask_aux=0;
				if(id_3 & desp) mask_aux= POLLIN | POLLOUT;
				if(id_2 & desp) mask_aux|= POLLIN;
				if(id_1 & desp) mask_aux|= POLLOUT;

				if(mask_aux==0) continue;

				fd=i*32+j;
				if(fd>=n) continue;

				mask = file_poll(fd, retval ? NULL : wait);
		
               			if (mask & mask_aux) {
               			        retval++;
		               	}
			}
		}
	}
        if (retval || !__timeout )
              break;
        if(table.error) {
              retval = table.error;
              break;
        }
        if (__timeout < 0) {
              /* Wait indefinitely */
             __timeout = 0x7fffffff;
        } 
        __timeout = schedule_timeout(__timeout);
    }
    set_current_state(TASK_RUNNING);

    poll_freewait(&table);

    return retval;
}
