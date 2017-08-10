//==========================================================================
//
// Author(s): Hector Posadas
// Contributors:
//
// Modifications:
//
//==========================================================================
//
//  The following code is derived, directly or indirectly, from XML plug-in for SCoPE,
//  released June 30, 2008. Copyright (C) 2008 Univeristy of Cantabria(UC)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License (GPL) or GNU Lesser General Public License(LGPL) as published by
//  the Free Software Foundation; either version 2 of the License, or
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
//  For more information about SCoPE and the XML plug-in for SCoPE you can visit
//  http://www.teisa.unican.es/scope or write an e-mail to
//  the authors or a letter to SCoPE, GIM - TEISA, University
//  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
//
//==========================================================================

#ifndef _IO_ACCESS_H_
#define _IO_ACCESS_H_

#include <signal.h>
#include <ucontext.h>

/*	Function used to perform the initialization of the specific memory region.
	Set the bus manager and maps the speficied region into an empty file, to provoke bus errors when accesing
	*/

void uc_initialize_peripheral_access(void *addr, int size);

/*	Function used to perform the initilize the io access subsistem. It loads the signal handlers*/

void uc_initialize_io_access_subsystem();

class UC_IO_access_class{
public:
	UC_IO_access_class(){
		uc_initialize_io_access_subsystem();
	}
};

extern UC_IO_access_class uc_io_access_instance;

#endif
