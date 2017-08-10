//==========================================================================
//	uc_pthread_spin_lock.h
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

#ifndef _UC_PTHREAD_SPIN_LOCK_H
#define _UC_PTHREAD_SPIN_LOCK_H

// FIXME It is not a C header

#ifdef __cplusplus
extern "C" {
#endif

typedef struct uc_spinlock {
	void *event;
	char lock;
	char init;
} uc_pthread_spinlock_t;

int uc_pthread_spin_lock(uc_pthread_spinlock_t *lock);
int uc_pthread_spin_trylock(uc_pthread_spinlock_t *lock);
int uc_pthread_spin_destroy(uc_pthread_spinlock_t *lock);
int uc_pthread_spin_init(uc_pthread_spinlock_t *lock, int pshared);
int uc_pthread_spin_unlock(uc_pthread_spinlock_t *lock);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
