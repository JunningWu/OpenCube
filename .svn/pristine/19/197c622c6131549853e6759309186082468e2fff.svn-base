//==========================================================================
//	uc_linux_if.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mar feb 12 2008
//	Description:
//==========================================================================
//  The following code is derived, directly or indirectly, from SCoPE,
//  released June 30, 2008. Copyright (C) 2006 Design of Systems on Silicon (DS2)
//  The Initial Developer of the Original Code is the University of Cantabria
//  for Design of Systems on Silicon (DS2)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License (GPL) or GNU Lesser General Public License(LGPL) as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License (GPL) or GNU Lesser General Public License(LGPL) for more details.
//
//  You should have received a copy of the GNU General Public License (GPL) or GNU Lesser General Public License(LGPL)
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//  For more information about SCOPE you can visit
//  http://www.teisa.unican.es/scope or write an e-mail to
//  scope@teisa.unican.es or a letter to SCoPE, GIM - TEISA, University
//  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
//==========================================================================

#ifndef _UC_LINUX_IF_H_
#define _UC_LINUX_IF_H_

#include <sys/poll.h>

#include "rtos/drivers/uc_linux_miscdevice.h"
/*
#include "if.h"
#include "list.h"
*/

#define ARPHRD_ETHER    1
#define ETH_ALEN        6               /* Octets in one ethernet addr */
#define ETH_HLEN        14              /* Total octets in header.     */
#define ETH_ZLEN        60              /* Min. octets in frame without FCS */
#define ETH_DATA_LEN    1500            /* Max. octets in payload      */
#define ETH_FRAME_LEN   1514            /* Max. octets in frame without FCS */
#define ETH_P_IP        0x0800          /* Internet Protocol packet     */
/*
#define POLLIN          0x0001
#define POLLPRI         0x0002
#define POLLOUT         0x0004
#define POLLERR         0x0008
#define POLLHUP         0x0010
#define POLLNVAL        0x0020
*/

#undef POLL_IN
#undef POLL_PRI
#undef POLL_OUT
#undef POLL_ERR
#undef POLL_HUP
#undef POLL_NVAL
#undef POLL_MSG

#define __SI_POLL       (2 << 16)
#define POLL_IN         (__SI_POLL|1)   /* data input available */
#define POLL_OUT        (__SI_POLL|2)   /* output buffers available */
#define POLL_MSG        (__SI_POLL|3)   /* input message available */
#define POLL_ERR        (__SI_POLL|4)   /* i/o error */
#define POLL_PRI        (__SI_POLL|5)   /* high priority input available */
#define POLL_HUP        (__SI_POLL|6)   /* device disconnected */
#define NSIGPOLL        6

/* The rest seem to be more-or-less nonstandard. Check them! */
/*
#define POLLRDNORM      0x0040
#define POLLRDBAND      0x0080
#define POLLWRNORM      0x0100
#define POLLWRBAND      0x0200
#define POLLMSG         0x0400
*/
#define POLLREMOVE      0x1000
#define POLLRDHUP       0x2000

#include "rtos/api/posix/uc_pthread.h"

#define _IOC_NRBITS     8
#define _IOC_TYPEBITS   8
#define _IOC_SIZEBITS   14
#define _IOC_DIRBITS    2

#define _IOC_NRMASK     ((1 << _IOC_NRBITS)-1)
#define _IOC_TYPEMASK   ((1 << _IOC_TYPEBITS)-1)
#define _IOC_SIZEMASK   ((1 << _IOC_SIZEBITS)-1)
#define _IOC_DIRMASK    ((1 << _IOC_DIRBITS)-1)

#define _IOC_NRSHIFT    0
#define _IOC_TYPESHIFT  (_IOC_NRSHIFT+_IOC_NRBITS)
#define _IOC_SIZESHIFT  (_IOC_TYPESHIFT+_IOC_TYPEBITS)
#define _IOC_DIRSHIFT   (_IOC_SIZESHIFT+_IOC_SIZEBITS)

/* used to decode ioctl numbers.. */

#define _IOC_DIR(nr)            (((nr) >> _IOC_DIRSHIFT) & _IOC_DIRMASK)
#define _IOC_TYPE(nr)           (((nr) >> _IOC_TYPESHIFT) & _IOC_TYPEMASK)
#define _IOC_NR(nr)             (((nr) >> _IOC_NRSHIFT) & _IOC_NRMASK)
#define _IOC_SIZE(nr)           (((nr) >> _IOC_SIZESHIFT) & _IOC_SIZEMASK)

#endif
