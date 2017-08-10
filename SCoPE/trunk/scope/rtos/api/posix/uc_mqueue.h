//==========================================================================
//	uc_mqueue.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mar ene 15 2008
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

#ifndef _UC_MQUEUE_H
#define _UC_MQUEUE_H

#include "rtos/api/posix/uc_time.h"
#include "rtos/api/posix/uc_signal.h"
// #include "uc_process_class.h"
// #include "uc_sleep_channel.h"

// class UC_process_class;
// class UC_sleep_channel;

#ifdef __cplusplus
extern "C" {
#endif

#undef  MQ_PRIO_MAX
#define MQ_PRIO_MAX 100
struct uc_mqd_params{
	pid_t pid;
	int read;
	int write;
	int unblock;
} ;

typedef int uc_mqd_t;

struct uc_mq_attr{
	long mq_flags;// Message queue flags;
	long mq_maxmsg;// Maximum number of messages;
	long mq_msgsize;// Maximum message size;
	long mq_curmsgs;// Number of messages currently queued;
};

struct mq_mess_element{
	int priority;
	int length;
	char *buffer;
};

uc_mqd_t uc_mq_open(const char *, int, ...);
int uc_mq_close(uc_mqd_t);
int uc_mq_notify(uc_mqd_t, const struct uc_sigevent *);
int uc_mq_getattr(uc_mqd_t, struct uc_mq_attr *);
int uc_mq_setattr(uc_mqd_t, const struct uc_mq_attr *, struct uc_mq_attr *);
ssize_t uc_mq_receive(uc_mqd_t, char *, size_t, void *);
ssize_t uc_mq_timedreceive(uc_mqd_t, char *, size_t,void *, const struct timespec *);
int uc_mq_send(uc_mqd_t, const char *, size_t, int );
int uc_mq_timedsend(uc_mqd_t, const char *, size_t, int , const struct timespec *);
int uc_mq_unlink(const char *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
