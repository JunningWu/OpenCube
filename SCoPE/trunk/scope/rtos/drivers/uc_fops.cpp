//==========================================================================
//	uc_fops.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: lun feb 11 2008
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

#include <errno.h>
#include "rtos/drivers/uc_fops.h"
#include "rtos/kernel/uc_thread_class.h"
#include "rtos/drivers/uc_io.h"
#include "rtos/kernel/uc_rtos_class.h"

extern __thread UC_rtos_class *qt_parent_rtos;

int uc__llseek(int fd, unsigned long offset_high,  unsigned  long offset_low, loff_t * result, unsigned int whence) {
	vector<struct uc_device_t *>::iterator it_device;
	for (it_device = qt_parent_rtos->m_dev_list.begin(); it_device != qt_parent_rtos->m_dev_list.end(); it_device++) {
		if ((*it_device)->fd == fd) {
			int major = (*it_device)->major;
			if (major == 10) {	// miscdevice
				vector<struct uc_driver_t *>::iterator it_driver;
				for (it_driver = qt_parent_rtos->m_miscdriver_list.begin();
								 it_driver != qt_parent_rtos->m_miscdriver_list.end(); it_driver++)
				{
					if ((*it_driver)->minor == (*it_device)->minor) {
						*result = (*it_driver)->fops->uc__llseek(&(*it_device)->dev_file, ((loff_t) offset_high << 32) | offset_low, whence);
						return 0;
					}
				}
			} else {
				*result = qt_parent_rtos->m_driver_list[major].fops->uc__llseek(&(*it_device)->dev_file, ((loff_t) offset_high << 32) | offset_low, whence);
				return 0;
			}
		}
	}

	return -EBADF;
}

int uc_ioctl(int fd, int request, char *arg) {
	vector<struct uc_device_t *>::iterator it_device;
	for (it_device = qt_parent_rtos->m_dev_list.begin(); it_device != qt_parent_rtos->m_dev_list.end(); it_device++) {
		if ((*it_device)->fd == fd) {
			int major = (*it_device)->major;
			if (major == 10) {	// miscdevice
				vector<struct uc_driver_t *>::iterator it_driver;
				for (it_driver = qt_parent_rtos->m_miscdriver_list.begin();
								 it_driver != qt_parent_rtos->m_miscdriver_list.end(); it_driver++)
				{
					if ((*it_driver)->minor == (*it_device)->minor) {
						return (*it_driver)->fops->uc_ioctl(&(*it_device)->i_node, &(*it_device)->dev_file, (unsigned int)request, (unsigned long)arg);
					}
				}
			} else {
				return qt_parent_rtos->m_driver_list[major].fops->uc_ioctl(&(*it_device)->i_node, &(*it_device)->dev_file, (unsigned int)request, (unsigned long)arg);
			}
		}
	}

	return -EBADF;
}
