//==========================================================================
//	uc_fops.h
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


#ifndef _UC_FOPS_H
#define _UC_FOPS_H

// #include <linux/types.h>		/* for "__kernel_caddr_t" et al	*/
#include <vector>
#include <cstddef> /* add by Junning Wu */
/*#include "rtos/api/posix/uc_pthread_spin_lock.h"
#include "rtos/api/posix/sys_uc_types.h"
#include "rtos/drivers/uc_file.h"*/
#include "../api/posix/uc_pthread_spin_lock.h"
#include "../api/posix/sys_uc_types.h"
#include "uc_file.h"

using namespace std;

// class UC_thread_class;

#ifdef __cplusplus
extern "C" {
#endif

typedef int atomic_t;
struct wait_queue_head_t;
int uc__llseek(int fd, unsigned long offset_high, unsigned  long offset_low, loff_t * result, unsigned int whence);
int uc_ioctl(int fd, int request, char *arg);

#define __user

#define INODE_MODE_777 0777

#define O_ACCMODE          0003
#define O_RDONLY             00
#define O_WRONLY             01
#define O_RDWR               02
#define O_CREAT            0100 /* not fcntl */
#define O_EXCL             0200 /* not fcntl */
#define O_NOCTTY           0400 /* not fcntl */
#define O_TRUNC           01000 /* not fcntl */
#define O_APPEND          02000
#define O_NONBLOCK        04000
#define O_NDELAY        O_NONBLOCK
#define O_SYNC           010000
//#define O_DIRECTORY      040000 /* must be a directory */
//#define O_NOFOLLOW      0100000 /* don't follow links */
//#define O_DIRECT        0200000 /* direct disk access hint - currently ignored */
//#define O_LARGEFILE     0400000
#define O_NOATIME       01000000

#define F_DUPFD         0       /* dup */
#define F_GETFD         1       /* get close_on_exec */
#define F_SETFD         2       /* set/clear close_on_exec */
#define F_GETFL         3       /* get file->f_flags */
#define F_SETFL         4       /* set file->f_flags */
#define F_GETLK         5
#define F_SETLK         6
#define F_SETLKW        7

#define F_SETOWN        8       /*  for sockets. */
#define F_GETOWN        9       /*  for sockets. */
#define F_SETSIG        10      /*  for sockets. */
#define F_GETSIG        11      /*  for sockets. */

/* for F_[GET|SET]FL */
#define FD_CLOEXEC      1       /* actually anything with low bit set goes */

/* for posix fcntl() and lockf() */
#define F_RDLCK         0
#define F_WRLCK         1
#define F_UNLCK         2

/* for old implementation of bsd flock () */
#define F_EXLCK         4       /* or 3 */
#define F_SHLCK         8       /* or 4 */

/* for leases */
#define F_INPROGRESS    16

/* operations for bsd flock(), also used by the kernel implementation */
#define LOCK_SH         1       /* shared lock */
#define LOCK_EX         2       /* exclusive lock */
#define LOCK_NB         4       /* or'd with one of the above to prevent
                                   blocking */
#define LOCK_UN         8       /* remove lock */

#define LOCK_MAND       32      /* This is a mandatory flock */
#define LOCK_READ       64      /* ... Which allows concurrent read operations */
#define LOCK_WRITE      128     /* ... Which allows concurrent write operations */
#define LOCK_RW         192     /* ... Which allows concurrent read & write ops */


struct inode {
	dev_t		i_rdev;
	umode_t		i_mode;
	unsigned int	i_nlink;
	uid_t		i_uid;
	gid_t		i_gid;
	loff_t		i_size;
	__u32		i_generation;
	struct timespec	i_atime;
	struct timespec	i_mtime;
	struct timespec	i_ctime;
	//struct uc_semaphore i_sem;
};

struct file_operations {
	struct module *owner;
	loff_t (*uc__llseek) (struct uc_file *, loff_t, int);
	ssize_t (*uc_read) (struct uc_file *, char __user *, size_t, loff_t *);
	//ssize_t (*aio_read) (struct kiocb *, char __user *, size_t, loff_t);
	ssize_t (*uc_write) (struct uc_file *, const char __user *, size_t, loff_t *);
	//ssize_t (*aio_write) (struct kiocb *, const char __user *, size_t, loff_t);
	//int (*readdir) (struct file *, void *, filldir_t);
	unsigned int (*uc_poll) (struct uc_file *, struct poll_table_struct *);
	int (*uc_ioctl) (struct inode *, struct uc_file *, unsigned int, unsigned long);
	//long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
	//long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
	//int (*mmap) (struct file *, struct vm_area_struct *);
	int (*uc_open) (struct inode *, struct uc_file *);
	int (*uc_flush) (struct uc_file *);
	int (*uc_release) (struct inode *, struct uc_file *);
	//int (*fsync) (struct file *, struct dentry *, int datasync);
	//int (*aio_fsync) (struct kiocb *, int datasync);
	int (*uc_fasync) (int, struct uc_file *, int);
	int (*uc_lock) (struct uc_file *, int, struct file_lock *);
	//ssize_t (*readv) (struct file *, const struct iovec *, unsigned long, loff_t *);
	//ssize_t (*writev) (struct file *, const struct iovec *, unsigned long, loff_t *);
	//ssize_t (*sendfile) (struct file *, loff_t *, size_t, read_actor_t, void *);
	//ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
	//unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
	//int (*check_flags)(int);
	//int (*dir_notify)(struct file *filp, unsigned long arg);
	int (*uc_flock) (struct uc_file *, int, struct file_lock *);
};

struct sk_buff_head {
	/* These two members must be first. */
	struct sk_buff  *next;
	struct sk_buff  *prev;
	int           qlen;
	uc_spinlock      lock;
};

struct sk_buff {
	/* These two members must be first. */
	struct sk_buff		*next;
	struct sk_buff		*prev;
	struct net_device	*dev;
	struct net_device	*input_dev;
	unsigned int		len, data_len,truesize;
	unsigned char		*head;
	unsigned char 		*data;
	unsigned char 		*tail;
	unsigned char 		*end;
};

struct fasync_struct {
	int pid;
	struct fasync_struct  *fa_next;
};

/**
 * \brief Struct for devices
 *
 * A struct to identificate each device
 */
struct uc_device_t {
	int fd;			/// < fd file descriptor
	char *path;		/// < path file's name (/dev/mydev)
	int major;		/// < major device's major number
	int minor;		/// < minor device's minor number
	struct inode i_node;	/// < i_node inode structure needed by file_operation
	struct uc_file dev_file;	/// < dev_file represents the opened device

	/// Constructor
	uc_device_t() {
		fd = -1;
		path = NULL;
		major = -1;
		minor = -1;
	}

	/// Destructor
	~uc_device_t() {
		delete path;
	}
};
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
