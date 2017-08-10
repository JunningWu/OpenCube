//==========================================================================
//	uc_pthread_rwlock.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mié ene 9 2008
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

#ifndef PTHREAD_RWLOCK_H
#define PTHREAD_RWLOCK_H

// #include "uc_sleep_channel.h"
// #include "uc_thread_class.h"

// class UC_thread_class;
// class UC_sleep_channel;
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int pshared;
}uc_pthread_rwlockattr_t;

typedef struct uc__rwlock_t{
	int init;
	int id;
/*	uc__rwlock_t() {
		init = 0;
	}*/
}uc_pthread_rwlock_t;

int uc_pthread_rwlockattr_init(uc_pthread_rwlockattr_t *attr);
int uc_pthread_rwlockattr_destroy(uc_pthread_rwlockattr_t *attr);
int uc_pthread_rwlockattr_getpshared(const uc_pthread_rwlockattr_t *attr, int *num);
int uc_pthread_rwlockattr_setpshared(uc_pthread_rwlockattr_t *attr, int num);
int uc_pthread_rwlock_init(uc_pthread_rwlock_t *, const uc_pthread_rwlockattr_t *);
int uc_pthread_rwlock_destroy(uc_pthread_rwlock_t *);

int uc_pthread_rwlock_rdlock(uc_pthread_rwlock_t *);
int uc_pthread_rwlock_tryrdlock(uc_pthread_rwlock_t *);
int uc_pthread_rwlock_timedrdlock(uc_pthread_rwlock_t *, const struct timespec *);

int uc_pthread_rwlock_wrlock(uc_pthread_rwlock_t *);
int uc_pthread_rwlock_trywrlock(uc_pthread_rwlock_t *);
int uc_pthread_rwlock_timedwrlock(uc_pthread_rwlock_t *, const struct timespec *);
int uc_pthread_rwlock_unlock(uc_pthread_rwlock_t *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
