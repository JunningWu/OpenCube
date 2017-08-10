//==========================================================================
//	sys_uc_wait.h
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

#ifndef	SYS_UC_WAIT_H
#define	SYS_UC_WAIT_H

#include <sys/wait.h>
#include "rtos/api/posix/uc_signal.h"
// #include "uc_resource.h"

#ifdef __cplusplus
extern "C" {
#endif

#if !defined _STDLIB_H || !defined __USE_XOPEN
/* This will define the `W*' macros for the flag
   bits to `waitpid', `wait3', and `wait4'.  */
# ifdef	__USE_BSD
#  if defined __GNUC__ && !defined __cplusplus
/*#   define __WAIT_INT(status) \
  (__extension__ (((union { __typeof(status) __in; int __i; }) \
                   { .__in = (status) }).__i))
#  else*/
#   undef  __WAIT_INT
#   define __WAIT_INT(status)	(*(__const int *) &(status))

#  endif /*defined __GNUC__ && !defined __cplusplus*/
#  endif /*__USE_BSD*/
#  endif /* !defined _STDLIB_H || !defined __USE_XOPEN*/


/*
#define WEXITSTATUS(d)	(d&0xff)//Return exit status.
#define WIFCONTINUED(d)	(d&0x80000000)//True if child has been continued.
#define WIFEXITED(d)	(d&0x40000000)//True if child exited normally.
#define WIFSIGNALED(d)	(d&0x20000000)//True if child exited due to uncaught signal.
#define WIFSTOPPED(d)	(d&0x10000000)//True if child is currently stopped.
#define WSTOPSIG(d)	(d&0x8000000)//Return signal number that caused process to stop.
#define WTERMSIG(d)	(d&0x4000000)//Return signal number that caused process to terminate.
*/
/* Everything extant so far uses these same bits.  */

#undef __WEXITSTATUS

/* If WIFEXITED(STATUS), the low-order 8 bits of the status.  */
#define	__WEXITSTATUS(status)	(((status) & 0xff00) >> 8)

#undef __WTERMSIG
/* If WIFSIGNALED(STATUS), the terminating signal.  */
#define	__WTERMSIG(status)	((status) & 0x7f)

#undef __WSTOPSIG
/* If WIFSTOPPED(STATUS), the signal that stopped the child.  */
#define	__WSTOPSIG(status)	__WEXITSTATUS(status)

#undef __WIFEXITED
/* Nonzero if STATUS indicates normal termination.  */
#define	__WIFEXITED(status)	(__WTERMSIG(status) == 0)

#undef __WIFSIGNALED
/* Nonzero if STATUS indicates termination by a signal.  */
#ifdef	__GNUC__
# define __WIFSIGNALED(status) \
  (__extension__ ({ int __status = (status);				      \
		    !__WIFSTOPPED(__status) && !__WIFEXITED(__status); }))
#else	/* Not GCC.  */
# define __WIFSIGNALED(status)	(!__WIFSTOPPED(status) && !__WIFEXITED(status))
#endif	/* GCC.  */

#undef __WIFSTOPPED
/* Nonzero if STATUS indicates the child is stopped.  */
#define	__WIFSTOPPED(status)	(((status) & 0xff) == 0x7f)

#undef __WCOREDUMP
/* Nonzero if STATUS indicates the child dumped core.  */
#define	__WCOREDUMP(status)	((status) & __WCOREFLAG)

#undef __W_EXITCODE
#undef __W_STOPCODE
#undef  __WCOREFLAG
/* Macros for constructing status values.  */
#define	__W_EXITCODE(ret, sig)	((ret) << 8 | (sig))
#define	__W_STOPCODE(sig)	((sig) << 8 | 0x7f)
#define	__WCOREFLAG		0x80


//options argument to waitid( ):

#undef WEXITED
#undef WSTOPPED
#undef WCONTINUED
#undef WNOHANG
#undef WNOWAIT
#undef WUNTRACED

#define WEXITED		1//Wait for processes that have exited.
#define WSTOPPED	2//Status is returned for any child that has stopped upon receipt of a signal.
#define WCONTINUED	4//Status is returned for any child that was stopped and has been continued.
#define WNOHANG		8//Return immediately if there are no children to wait for.
#define WNOWAIT		16//Keep the process whose status is returned in infop in a waitable state.
#define WUNTRACED       32
/*
enum idtype_t {P_ALL,P_PID,P_PGID};
*/

uc_pid_t uc_wait(int *ret_val);
uc_pid_t uc_waitpid (uc_pid_t __pid, int *__stat_loc, int __options);
// int waitid (idtype_t __idtype, id_t __id, siginfo_t *__infop,
//		   int __options) ;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* sys/wait.h  */
