//==========================================================================
//	uc_file.h
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

#ifndef UC_FILE_H
#define UC_FILE_H

#include <vector>
using namespace std;
#include "../api/posix/sys_uc_types.h"

class UC_thread_class;

class uc_file{
public:
    uc_file();
    ~uc_file();

	void awake_wait_read();
	void awake_wait_write();
	void add_wait_read(UC_thread_class* thread);
	void add_wait_write(UC_thread_class* thread);

 	struct file_operations  *f_op; //This field points to the methods to use on this file
	atomic_t                f_count; //The number of references to this file. One for each different user-process file descriptor, plus one for each internal usage
	unsigned int            f_flags; //This field stores the flags for this file such as access type (read/write), nonblocking, appendonly etc.
	mode_t                  f_mode; //f_mode stores the read and write access as two separate bits
        //int                     f_error;
	loff_t                  f_pos; //This records the current file position which will be the address used for the next read request, and for the next write request if the file does NOT have the O_APPEND flag

	//        unsigned int            f_uid, f_gid; //
//        size_t                  f_maxcount;

	void                    *private_data; //This is used by many device drivers, and even a few file-systems, to store extra per-open-file information (such as credentials in coda)


	/////////////////////////////////////////////////////////////////////////
	//  Non-linux components
	/////////////////////////////////////////////////////////////////////////

	int fd;
	int type; //1:pipe, 2:socket
	uc_file *couple;
	int is_closed;
	char *name;

	char *buffer;// for sockets is not char*; represents all socket info
	int size;

	int (*read_func)(int s, void *dataptr, int size);
	int (*write_func)(int s, void *dataptr, int size);
	int (*close_func)(int s);

private:
	vector<UC_thread_class*> wait_read;
	vector<UC_thread_class*> wait_write;

};

#endif
