//==========================================================================
//	uc_pthread_spin_lock.cpp
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

#include "rtos/api/posix/uc_posix.h"
#include "rtos/api/posix/uc_pthread_spin_lock.h"
#include <errno.h>

int UC_posix_class::uc_pthread_spin_lock(uc_pthread_spinlock_t *lock) {
	// FIXME
	while (lock->lock) {
		wait(*(sc_event*)lock->event);
	}
	lock->lock = 1;
	return 0;
}

int UC_posix_class::uc_pthread_spin_trylock(uc_pthread_spinlock_t *lock) {
	if (lock->lock) {
		return EBUSY;
	}
	lock->lock = 1;
	return 0;
}

int UC_posix_class::uc_pthread_spin_destroy(uc_pthread_spinlock_t *lock) {
	if (lock->lock) {
		return EBUSY;
	}
	lock->init = 0;
	return 0;
}

int UC_posix_class::uc_pthread_spin_init(uc_pthread_spinlock_t *lock, int pshared) {
	lock->init = 1;
	lock->lock = 0;
	lock->event = new sc_event();
	return 0;
}

int UC_posix_class::uc_pthread_spin_unlock(uc_pthread_spinlock_t *lock) {
	lock->lock = 0;
	(*(sc_event*)lock->event).notify();
	return 0;
}
