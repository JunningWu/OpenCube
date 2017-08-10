//==========================================================================
//	uc_unistd.h
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

#ifndef _UC_UNISTD_H
#define _UC_UNISTD_H

#undef STDIN_FILENO
#undef STDOUT_FILENO
#undef STDERR_FILENO

#define STDIN_FILENO   0
#define STDOUT_FILENO  1
#define STDERR_FILENO  2

#include <unistd.h>
#include "uc_time.h"

#include <sys/mman.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#undef _POSIX_SPORADIC_SERVER
#undef _POSIX_THREAD_SPORADIC_SERVER

#define I_O_ACCESS_MAX  SC_INF_TIME

uc_pid_t uc_getpid();
uc_gid_t uc_getgid();

uc_pid_t uc_getppid();
uc_pid_t uc_getpgid(uc_pid_t pid);
int uc_setpgid(uc_pid_t pid, uc_pid_t pgid);
uc_pid_t uc_getpgrp();
int uc_setpgrp();

int uc_setuid(uc_uid_t uid);
int uc_getuid();
uc_gid_t uc_getegid () ;  //effective group ID
int uc_setegid (uc_gid_t ee) ;  //effective group ID
uc_uid_t uc_geteuid () ;  //effective user ID
int uc_seteuid (uc_uid_t ee) ;  //effective user ID

long uc_gethostid(void); // IP of affinity ID ?

// int uc_open(const char *camino, int flags);
// int uc_open(const char *camino, int flags, mode_t modo);
int uc_open(const char *camino, int flags, ...);
int uc_creat(const char *camino, mode_t modo);

int uc_pipe(int filedes[2]);
int uc_close(int fd);

int uc_write(int fd, const void *buf, size_t len);
int uc_read(int fd, void *buf, size_t len);
int uc_pread(int fd, void *buf, size_t count, off_t offset);
int uc_pwrite(int fd, const void *buf, size_t count, off_t offset);

int uc_nice(int inc);
int uc_usleep(unsigned long int useconds);
unsigned int uc_sleep(unsigned int seconds);
unsigned int uc_alarm(unsigned int seconds);
int uc_pause(void);
void _uc_exit(int status);

#undef _POSIX_VERSION
#define   _POSIX_VERSION  199506L

#include "sys_uc_select.h"

/****************************************************************************

FILE - I/O NOT MODIFIED
// Test for access to NAME using the real UID and real GID.
extern int access (const char *__name, int __type) ;
int chdir(const char *path) ;
int fchdir(int fd) ;
int rmdir(const char *pathname) ;
char *getcwd(char *buf, size_t size) ;

int open(const char* pathname,int flags, ...) ;
int open64(const char* pathname,int flags, ...) ;
int creat(const char* pathname,mode_t mode) ;
int creat64(const char* pathname,mode_t mode) ;
int close(int fd) ;
off_t lseek(int fildes, off_t offset, int whence) ;
// warning: linux specific:
int llseek(int fildes, unsigned long hi, unsigned long lo, loff_t* result,int whence) ;


int select(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) ;

int fsync(int fd) ;
int fdatasync(int fd) ;
int sync(void) ;

int dup (int oldfd) ;
int dup2 (int oldfd,int newfd) ;

int unlink(const char *pathname) ;
int readlink(const char *path, char *buf, size_t bufsiz) ;
int symlink(const char *oldpath, const char *newpath) ;
int link(const char *oldpath, const char *newpath) ;

int isatty(int desc) ;
char *ttyname (int desc) ;

char *crypt(const char *key, const char *salt) ;
void encrypt(char block[64], int edflag) ;
void setkey(const char *key) ;


NET - NOT MODIFIED

int gethostname(char *name, size_t len) ;
int sethostname(const char *name, size_t len) ;


Process Execution - NOT MODIFIED

int execve(const char *filename, char *const argv [], char *const envp[]) ;
int execlp(const char *file, const char *arg, ...) ;
int execv(const char *path, char *const argv[]) ;
int execvp(const char *file, char *const argv[]) ;
int execl(const char *path, ...) ;
int execle(const char *path, ...) ;

USER - NOT MODIFIED

int chown(const char *path, uid_t owner, gid_t group) ;
int fchown(int fd, uid_t owner, gid_t group) ;
int lchown(const char *path, uid_t owner, gid_t group) ;
extern char* getlogin(void) ;
// warning: the diet libc getlogin() simply returns getenv("LOGNAME")
int chroot(const char *path) ;
pid_t tcgetpgrp(int fd) ;
int tcsetpgrp(int fd, pid_t pgrpid) ;


size_t getpagesize(void)  __attribute__((__const__));

long sysconf(int name) ;

extern int daemon(int nochdir,int noclose) ;

int getopt(int argc, char *const argv[], const char *options);
****************************************************************************/

int uc_shm_open(const char *name, int oflag, mode_t mode);
int uc_ftruncate(int fildes, off_t length);
int uc_shm_unlink(const char *name);
void *uc_mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off);
int uc_munmap(void *addr, size_t len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
