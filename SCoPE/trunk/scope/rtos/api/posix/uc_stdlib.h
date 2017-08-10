//==========================================================================
//	uc_stdlib.h
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

#ifndef _UC_STDLIB_H
#define _UC_STDLIB_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif


#if defined __USE_XOPEN && !defined _SYS_WAIT_H
/* XPG requires a few symbols from <sys/wait.h> being defined.  */

# ifdef __USE_BSD

/* Lots of hair to allow traditional BSD use of `union wait'
   as well as POSIX.1 use of `int' for the status word.  */

#  if defined __GNUC__ && !defined __cplusplus
/*#   define __WAIT_INT(status)						      \
  (__extension__ ({ union { __typeof(status) __in; int __i; } __u;	      \
		    __u.__in = (status); __u.__i; }))
#  else*/
#   undef __WAIT_INT
#   define __WAIT_INT(status)	(*(int *) &(status))
#  endif /*defined __GNUC__ && !defined __cplusplus*/
#  endif /*__USE_BSD*/
#  endif /*defined __USE_XOPEN && !defined _SYS_WAIT_H*/

	int uc_mkstemp(char *temp);
	void uc_abort();
	int uc_system(const char* buf);
	void uc_exit(int status);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _UC_STDLIB_H */
