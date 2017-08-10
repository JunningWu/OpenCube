//==========================================================================
//	sys_uc_wait.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: vie abr 4 2008
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
#include "rtos/api/posix/sys_uc_wait.h"

/**
 * \brief Wait for a child matching PID to die
 */
uc_pid_t UC_posix_class::uc_wait(int *ret_val) {
	uc_pthread_testcancel();
// 	int ret = uc_wait(ret_val);
	uc_pthread_testcancel();
// 	return ret;
	return 0;
}

/**
 * \brief Wait for a child matching PID to die
 *
 * If PID is greater than 0, match any process whose process ID is PID.
 * If PID is (pid_t) -1, match any process.
 * If PID is (pid_t) 0, match any process with the same process group as the current process.
 * If PID is less than -1, match any process whose process group is the absolute value of PID.
 * If the WNOHANG bit is set in OPTIONS, and that child is not already dead, return (pid_t) 0.
 * If successful, return PID and store the dead child's status in STAT_LOC.
 * Return (pid_t) -1 for errors.
 * If the WUNTRACED bit is set in OPTIONS, return status for stopped children; otherwise don't.
 */
uc_pid_t UC_posix_class::uc_waitpid (uc_pid_t pid, int *stat_loc, int options) {
	uc_pthread_testcancel();
// 	int ret = uc_waitpid(pid, stat_loc,  options);
	uc_pthread_testcancel();
// 	return ret;
	return 0;
}

/**
 * \brief Wait for a childing matching IDTYPE and ID to change the status and place appropriate information in *INFOP.
 * If IDTYPE is P_PID, match any process whose process ID is ID.
 * If IDTYPE is P_PGID, match any process whose process group is ID.
 * If IDTYPE is P_ALL, match any process.
 * If the WNOHANG bit is set in OPTIONS, and that child is not already dead,
 * clear *INFOP and return 0.
 * If successful, store exit code and status in *INFOP.
 */
// int waitid (idtype_t __idtype, id_t __id, siginfo_t *__infop,
//		   int __options) ;
