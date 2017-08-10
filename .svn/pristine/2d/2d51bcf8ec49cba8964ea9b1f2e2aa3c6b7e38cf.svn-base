//==========================================================================
//	uc_sleep_channel.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mié ene 16 2008
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

#include "rtos/api/posix/uc_sleep_channel.h"

UC_sleep_channel::UC_sleep_channel() {
	type_of_channel = 0;
	counter = 0;
	max_entries = 0;
}

UC_sleep_channel::UC_sleep_channel(int i) {
	type_of_channel = i;
	counter = 0;
	max_entries = 0;
}

UC_sleep_channel::UC_sleep_channel(int i, int b) {
	type_of_channel = i;
	counter = b;
	max_entries = 0;
}

int UC_sleep_channel::unblock(UC_thread_class *thread){
	vector<UC_thread_class *>::iterator it_thread;
	for (it_thread = blocked_threads.begin(); it_thread != blocked_threads.end(); it_thread++) {
		if (*it_thread == thread) {
			blocked_threads.erase(it_thread);
			break;
		}
	}
	counter=0;
	return 0;
}

void UC_sleep_channel::destroy_thread(UC_thread_class *thread){
	vector<UC_thread_class *>::iterator it_thread;
	for (it_thread = blocked_threads.begin(); it_thread != blocked_threads.end(); it_thread++) {
		if (*it_thread == thread) {
			blocked_threads.erase(it_thread);
			break;
		}
	}
}
