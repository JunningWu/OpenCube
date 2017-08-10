//==========================================================================
//	uc_sleep_channel.h
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

#ifndef _UC_SLEEP_CHANNEL_H
#define _UC_SLEEP_CHANNEL_H

#include <vector>
using namespace std;

// #include "uc_thread_class.h"
class UC_thread_class;

class UC_sleep_channel {
public:

	UC_sleep_channel();
	UC_sleep_channel(int i);
	UC_sleep_channel(int i, int b);
	int unblock(UC_thread_class *thread);
	void destroy_thread(UC_thread_class *thread);

	int type_of_channel;
		// Type 0: Waiting for processor or segment time (Autoblock)
		// Type 1: Sleep
		// Type 2: Mutex
		// Type 3: Thread Semaphore
		// Type 4: Conditional Variable
		// Type 5: Spin
		// Type 6: rwlock
		// Type 7: Message queue
		// Type 8: IPC Semaphore
		// Type 9: IPC Message queue
		// Type 10: signal
		// Type 11: I/O

	int counter;
	int channel_priority;
	vector<UC_thread_class *> blocked_threads;

	int max_entries;	// The maximum number of elements in the channel during the simulation
						// Used by child classes
};

#endif
