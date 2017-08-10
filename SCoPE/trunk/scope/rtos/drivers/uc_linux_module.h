//==========================================================================
//	uc_linux_module.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mar feb 12 2008
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

#ifndef _UC_LINUX_MODULE_H_
#define _UC_LINUX_MODULE_H_

#include "uc_linux_miscdevice.h"

// #include "rtos/kernel/uc_rtos_class.h"
// extern __thread UC_rtos_class *qt_parent_rtos;
// extern __thread UC_process_class *qt_parent_process;

class uc_module_init_class{
public:
	uc_module_init_class(int (*init)(void), const char *file, const char *name=" ", const char *version=" ", const char *descr=" ", const char *copyr=" ") {
		m_init = init;
		m_file = new char[strlen(file) + 1];
		strcpy(m_file, file);
		m_name = new char[strlen(name) + 1];
		strcpy(m_name, name);
		m_version = new char[strlen(version) + 1];
		strcpy(m_version, version);
		m_descr = new char[strlen(descr) + 1];
		strcpy(m_descr, descr);
		m_copyr = new char[strlen(copyr) + 1];
		strcpy(m_copyr, copyr);
	}

	~uc_module_init_class() {
		delete [] m_file;
		delete [] m_name;
		delete [] m_version;
		delete [] m_descr;
		delete [] m_copyr;
	}

	int (*m_init)(void);
	char *m_file;
	char *m_name;
	char *m_version;
	char *m_descr;
	char *m_copyr;
};

// class uc_module_exit_class{
// public:
// 	uc_module_exit_class(void (*exit)(void), char *file) {
// 		m_exit = exit;
// 		m_file = file;
// 	}
//
// 	void (*m_exit)(void);
// 	char *m_file;
// };

#define module_init(init) \
		uc_module_init_class kernel_module_init_##init() {	\
			return uc_module_init_class(init,__FILE__,DRV_NAME,DRV_VERSION,DRV_DESCRIPTION,DRV_COPYRIGHT);	\
		}

#define module_exit(exit)
//uc_module_exit_class uc_module_exit_##exit(exit,__FILE__)

class uc_linux_module{
public:
	char *m_file;
	char *m_name;
	char *m_version;
	char *m_descr;
	char *m_copyr;
	char *m_module_file;
	int  (*m_init)(void);
	void  (*m_exit)(void);
//	struct miscdevice misc;
//	int active;

	uc_linux_module(char* file){
		m_module_file = new char[strlen(file) + 1];
		strcpy(m_module_file, file);
		m_init = NULL;
		m_exit = NULL;
//		active=0;
	}

	uc_linux_module(){
		m_module_file = new char[strlen("") + 1];
		strcpy(m_module_file, "");
//		active=0;
	}

	void init(int (*init)(void),char *name,char *version,char *descr,char *copyr){
		m_name = new char[strlen(name) + 1];
		strcpy(m_name, name);
		m_version = new char[strlen(version) + 1];
		strcpy(m_version, version);
		m_descr = new char[strlen(descr) + 1];
		strcpy(m_descr, descr);
		m_copyr = new char[strlen(copyr) + 1];
		strcpy(m_copyr, copyr);

		m_init = init;
	}

	void set_exit(void (*exit)(void)){
		m_exit = exit;
	}
};

#endif
