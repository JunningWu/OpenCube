//==========================================================================
//	uc_semaphore.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: jue ene 17 2008
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

#ifndef _UC_SEMAPHORE_H
#define _UC_SEMAPHORE_H

#include "rtos/api/posix/uc_time.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SEM_FAILED NULL
#undef SEM_VALUE_MAX
#define SEM_VALUE_MAX 0x7fffffff

typedef int uc_sem_t;

int uc_sem_close(uc_sem_t *);
int uc_sem_destroy(uc_sem_t *);
int uc_sem_getvalue(uc_sem_t *, int *);
int uc_sem_init(uc_sem_t *, int, int);
uc_sem_t *uc_sem_open(const char *, int, ...);
int uc_sem_post(uc_sem_t *);
int uc_sem_timedwait(uc_sem_t *, const struct timespec *);
int uc_sem_trywait(uc_sem_t *);
int uc_sem_unlink(const char *);
int uc_sem_wait(uc_sem_t *);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
