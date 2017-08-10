#ifndef _UC_SELECT_H
#define _UC_SELECT_H

#include <sys/select.h>
#include "sys_uc_time.h"
#include "sys_uc_types.h"

//typedef long int __fd_mask;

#undef __NFDBITS
#undef __FDS_BITS
#undef __FDELT
#undef __FDMASK
#undef __FD_ZERO
#undef __FD_SET
#undef __FD_CLR
#undef __FD_ISSET

#define __NFDBITS  (8 * sizeof (__fd_mask))

# define __FDS_BITS(set) ((set)->fds_bits)
#define     __FDELT(d)      ((d) / __NFDBITS)
#define    __FDMASK(d)     ((__fd_mask) 1 << ((d) % __NFDBITS))

# define __FD_ZERO(set)  \
  do {									      \
    unsigned int __i;							      \
    fd_set *__arr = (set);						      \
    for (__i = 0; __i < sizeof (fd_set) / sizeof (__fd_mask); ++__i)	      \
      __FDS_BITS (__arr)[__i] = 0;					      \
  } while (0)
# define __FD_SET(d, set)    (__FDS_BITS (set)[__FDELT (d)] |= __FDMASK (d))
# define __FD_CLR(d, set)    (__FDS_BITS (set)[__FDELT (d)] &= ~__FDMASK (d))
# define __FD_ISSET(d, set)  (__FDS_BITS (set)[__FDELT (d)] & __FDMASK (d))

int uc_select(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) ;

#endif
