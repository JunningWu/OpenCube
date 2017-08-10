//==========================================================================
//	uc_enums.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mié oct 31 2007
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

#ifndef UC_ENUMS_H
#define UC_ENUMS_H

/**
 * \enum UC_process_states
 * \brief Indicates the state of a thread
 */
enum UC_process_states
{
	ZOMBIE = 0,			///< Process is dead but mantains basic information
	BLOCKED = 1,		///< Process blocked or asleep in a system call.
	WAITING = 2,		///< Waiting for running in user mode
	CREATED = 3,		///< Process created
	READY = 4,			///< Process not blocked, ready for super user mode
	USER = 5,			///< Running user code
	SUPER_USER = 6		///< Running a system call or interruption
};

#endif
