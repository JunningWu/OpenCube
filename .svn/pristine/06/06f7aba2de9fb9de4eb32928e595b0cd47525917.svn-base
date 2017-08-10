//==========================================================================
//	uc_file.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mié ene 23 2008
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

#include "rtos/drivers/uc_file.h"
#include "rtos/kernel/uc_thread_class.h"

uc_file::uc_file()
{
	buffer = new char[100];
	type = 0;
//	f_count = 0;
	fd = -1;
}

uc_file::~uc_file()
{
	delete [] buffer;
	wait_write.clear();
	wait_read.clear();
}

void uc_file::awake_wait_read(){
	while (wait_read.size() != 0) {
		wait_read[0]->unblock();
	}
}

void uc_file::awake_wait_write(){
	while (wait_write.size() != 0) {
		wait_write[0]->unblock();
	}
}

void uc_file::add_wait_read(UC_thread_class* thread){
	thread->block(&wait_read);
}

void uc_file::add_wait_write(UC_thread_class* thread){
	thread->block(&wait_write);
}
