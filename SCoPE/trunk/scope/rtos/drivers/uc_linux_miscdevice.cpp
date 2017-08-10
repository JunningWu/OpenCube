//==========================================================================
//	uc_linux_miscdevice.cpp
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
#include <fcntl.h>
#include "rtos/drivers/uc_io.h"
#include "rtos/drivers/uc_linux_miscdevice.h"
#include "rtos/drivers/uc_fops.h"
#include "rtos/kernel/uc_rtos_class.h"

extern __thread UC_rtos_class *qt_parent_rtos;

int uc_misc_register(struct uc_miscdevice * misc){
	struct uc_device_t *device;
	struct uc_driver_t *driver;

	driver = new struct uc_driver_t;
	driver->name = (char*)malloc(strlen("miscdevice") + 1);
	strcpy(driver->name, "miscdevice");
	driver->fops = misc->fops;
	driver->major = MISC_DEVICE_MAJOR;
	driver->minor = misc->minor;
	qt_parent_rtos->m_miscdriver_list.push_back(driver);

	// search for a device with minor iqual to misc->minor
	// if not found, create it
	bool dev_found = false;
	for (int i = 0; i < qt_parent_rtos->m_dev_list.size(); i++) {
		device = qt_parent_rtos->m_dev_list[i];
		if (device->minor == misc->minor) {
			dev_found = true;
			break;
		}
	}

	if (!dev_found) {
		int mayor = 10;
		dev_t dev = (mayor<<8) | misc->minor;
		uc_mknod((char*)misc->name, S_IFCHR, dev);
	}

	return 0;
}

int uc_misc_deregister(struct uc_miscdevice * misc){
	struct uc_driver_t *driver;
	for (int i = 0; i < qt_parent_rtos->m_miscdriver_list.size(); i++) {
		driver = qt_parent_rtos->m_miscdriver_list[i];
		if (!strcmp(driver->name, misc->name)) {
			qt_parent_rtos->m_miscdriver_list.erase(qt_parent_rtos->m_miscdriver_list.begin() + i);
			delete driver;
			return 0;
		}
	}

	return -1;
}

net_device *__get_dev_by_name(const char *name){
	net_device* dev;
	for(int i = 0; i < qt_parent_rtos->m_netdevice_list.size(); i++) {
		dev = qt_parent_rtos->m_netdevice_list[i];
		if (strcmp(name, dev->name) == 0){
			return dev;
		}
	}
	return NULL;
}

void register_netdevice(struct net_device *dev){
	struct miscdevice* mod;
	qt_parent_rtos->m_netdevice_list.push_back(dev);

	init_waitqueue_head(&dev->read_wait);
	init_waitqueue_head(&dev->write_wait);
	init_waitqueue_head(&dev->exc_wait);
}
