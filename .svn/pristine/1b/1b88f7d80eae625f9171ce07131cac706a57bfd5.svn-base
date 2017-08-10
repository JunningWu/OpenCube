//==========================================================================
//	uc_pthread_mutex_class.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mar ene 8 2008
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

#ifndef UC_PTHREAD_MUTEX_CLASS_H
#define UC_PTHREAD_MUTEX_CLASS_H

#include "systemc.h"
#include "rtos/api/posix/uc_sleep_channel.h"
#include "rtos/kernel/uc_process_class.h"

class uc_pthread_mutex_class : public UC_sleep_channel {
public:
	uc_pthread_mutexattr_t *mutexattr;
	UC_process_class *parent_process;
	int mutex_id;

	uc_pthread_mutex_class();
	explicit uc_pthread_mutex_class( const char* name_ );
	uc_pthread_mutex_class(  int type,  int protocol,  int pshared);
// 	virtual ~uc_pthread_mutex_class();

	// blocks until mutex could be locked
	virtual int lock();

	// returns -1 if mutex could not be locked
	virtual int timedlock(const struct timespec *abs_timeout);

	// returns -1 if mutex could not be locked
	virtual int trylock();

	// returns -1 if mutex was not locked by caller
	virtual int unlock();

	int empty();
	static const char* const kind_string;

	virtual const char* kind() const
	{ return "uc_pthread_mutex_t"; }

protected:

// support methods

	bool in_use() const
	{ return ( m_owner != 0 ); }

	UC_thread_class *m_owner;
	sc_event      m_free;

	void update_mutex_priority(UC_thread_class *attr);
	void restore_process_priority();

public:
	int num_locks;
	int sc_lock();
	int sc_lock(const struct timespec *abs_timeout);
	int sc_trylock();
	int unblock(); // in UC_sleep_channel

	UC_thread_class *get_owner();
};

#endif
