//==========================================================================
//
// Author(s): Hector Posadas
// Contributors:
//
// Modifications:
//
//==========================================================================
//
//  The following code is derived, directly or indirectly, from XML plug-in for SCoPE,
//  released June 30, 2008. Copyright (C) 2008 Univeristy of Cantabria(UC)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License (GPL) or GNU Lesser General Public License(LGPL) as published by
//  the Free Software Foundation; either version 2 of the License, or
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
//  For more information about SCoPE and the XML plug-in for SCoPE you can visit
//  http://www.teisa.unican.es/scope or write an e-mail to
//  the authors or a letter to SCoPE, GIM - TEISA, University
//  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
//
//==========================================================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sched.h>
#include <unistd.h>
#include <utime.h>

#include <asm/sigcontext.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>

#define _UC_SIGNAL_H

#include "io_access.h"
#include "disasm.h"
#include "rtos/low_level/uc_ll_bus_io.h"

class UC_thread_class ;
class UC_rtos_class ;
UC_thread_class* uc_get_current_thread();

struct uc_io_access_info{
	int m_bus_address;		// Address that provokes the bus error;
	char m_backup[100];		// Buffer for the original code replaced by the temporal code 	
	int m_assembler_addr;		// Address for the code insertion
	UC_thread_class *m_thread;	// Current thread pointer
	int m_read;			// 1 = read access, 0 = write access
};

struct uc_mapped_io_info{
	unsigned int m_addr;
	unsigned int m_size;
	int m_file1;
	int m_file2;
};
std::vector<struct uc_mapped_io_info*> uc_mapped_io_list;
std::vector<struct uc_io_access_info*> uc_io_accesses_list;

/*
	search the address in the peripherals list and return the info of the peripheral in uc_mapped_io_list
	Return: the pointer to the info struct, 0 otherwise
*/
struct uc_mapped_io_info* get_io_mapped(unsigned int addr){
	unsigned int size;
	for(int i=0; i<uc_mapped_io_list.size(); i++){
		size = uc_mapped_io_list[i]->m_size;
		if(addr >= uc_mapped_io_list[i]->m_addr && addr < uc_mapped_io_list[i]->m_addr+size){
			return uc_mapped_io_list[i];
		}
	}

	return NULL;
}
/*
	Extract and return the struct with the access info corresponding to the current thread
*/
struct uc_io_access_info* extract_current_io_access(){
	unsigned int size;
	struct uc_io_access_info* info;
	UC_thread_class *thread = uc_get_current_thread();
	for(int i=0; i<uc_io_accesses_list.size(); i++){
		if(thread == uc_io_accesses_list[i]->m_thread){
			info = uc_io_accesses_list[i];
			uc_io_accesses_list.erase(uc_io_accesses_list.begin()+i);
			return info;
		}
	}

	return NULL;
}

/*	Function called to inform the HW about an i/o access
	*/
void inform_hw_access(int bus_address){
	printf("Bus access performed at address 0x%.8x\n",bus_address);
}

/*	Function used to print the indicated context
	*/

void print_sigcontext(ucontext_t *data){
	
	struct sigcontext *sc;
	ucontext_t *ucp;
	ucp = (ucontext_t *)data;
	sc = (struct sigcontext *)((&(ucp->uc_mcontext))->gregs);
	
	printf("\tgs \t0x%.4x \t\t__gsh \t0x%.4x\t", sc->gs, sc->__gsh);	
	printf("\tfs \t0x%.4x \t\t__fsh \t0x%.4x\n", sc->fs, sc->__fsh);	
	printf("\tes \t0x%.4x \t\t__esh \t0x%.4x\t", sc->es, sc->__esh);	
	printf("\tds \t0x%.4x \t\t__dsh \t0x%.4x\n", sc->ds, sc->__dsh);	
	printf("\tcs \t0x%.4x \t\t__csh \t0x%.4x\t", sc->cs, sc->__csh);	
	printf("\tss \t0x%.4x \t\t__ssh \t0x%.4x\n", sc->ss, sc->__ssh);	
	
	printf("\tedi \t0x%.8x \tesi \t0x%.8x", sc->edi, sc->esi);	
	printf("\tebp \t0x%.8x \tesp \t0x%.8x\n", sc->ebp, sc->esp);	
	printf("\teax \t0x%.8x \tebx \t0x%.8x", sc->eax, sc->ebx);	
	printf("\tecx \t0x%.8x \tedx \t0x%.8x\n", sc->ecx, sc->edx);	
	
	printf("\ttrapno \t0x%.8x \terr \t0x%.8x ", sc->trapno, sc->err);	
	printf("\teip \t0x%.8x \teflags\t0x%.8x\n", sc->eip, sc->eflags);	

	
	printf("\tesps \t0x%.8x \tfpstate\t0x%.8x ", sc->esp_at_signal, sc->fpstate);	
	printf("\toldmask\t0x%.8x \tcr2 \t0x%.8x\n", sc->oldmask, sc->cr2);	

}

extern char injectionStart;
extern char injectionEnd;

ucontext_t uc_auxiliar_ucp;
volatile long uc_auxiliar_eax;
	
/*	Funtion called from the inserted code to manage the I/O
	*/
void returningFunction(){	
	int old_p;
	ucontext_t uc_ucp;
	long uc_eax;
	struct uc_io_access_info* access_info = extract_current_io_access();

	int bus_address = access_info->m_bus_address;
	
	struct uc_mapped_io_info* io_info = get_io_mapped(bus_address);

	uc_ucp = uc_auxiliar_ucp;
	uc_eax = uc_auxiliar_eax;

	/* close the file to flush the data to the real file*/
	//sync();

	/* perform the write access in the platform model*/
	if(!access_info->m_read){
		uc_iowrite32(*(int*)access_info->m_bus_address , (int*)access_info->m_bus_address);
	}

	/*Change the file where the I/O address is mapped 
	  The new file has 0 size to proboke a bus error in the next access
	*/
	
	int ret = munmap((int*)(bus_address&0xffff0000), 1024);
	if(ret != 0 ){
		perror("munmap failed(addr)");
	}
	void *ret_p = mmap((int*)(bus_address&0xffff0000), 1024, PROT_READ/*|PROT_WRITE*/, MAP_FIXED|MAP_SHARED, io_info->m_file2, 0);
	if(ret_p == 0 || ret_p !=(int*)(bus_address&0xffff0000) ){
		perror("mmap failed(addr)");
	}
	
	/*Restore the original source code*/
	
	memcpy((char*)access_info->m_assembler_addr,access_info->m_backup,&injectionEnd - &injectionStart);
	
	
	/*Restore the context to return to the original istruction before the code insertion and continue the program normally*/
	
	old_p = ((struct sigcontext *)((&(uc_ucp.uc_mcontext))->gregs))->eip;
	((struct sigcontext *)((&(uc_ucp.uc_mcontext))->gregs))->eip = access_info->m_assembler_addr;
	((struct sigcontext *)((&(uc_ucp.uc_mcontext))->gregs))->eax = uc_eax;

	delete access_info;
	setcontext(&uc_ucp);
	
}

int (*callGetContext)(ucontext_t *ucp) = &getcontext;
void (*callreturningFunction)() = &returningFunction;

/* 	Function not used directly.
	The code between the asm flags is the code inserted to model the I/O
	*/

void dummyTextContext( void )
{
/*	sometimes gcc is just too smart.. in an ideal world
	we would write smthng like (*(&exit))( 42 ); and get
	an indirect call (via absolute address). anyway,
	we don't get that so we have to make one pointer for
	every type of function we want to call */

	
	asm volatile( "injectionStart:" );

/*	do what you like here but keep it relocatable
	fire up objdump, gdb or whatever to check what you get */

	// !! FIXME: Function call includes three assembler instructions before calls.
	// context can be wrong!!
	asm volatile("movl %eax,uc_auxiliar_eax" );
//	asm volatile("movl %ebp,uc_auxiliar_ebp" );
//	asm volatile("movl uc_auxiliar_stack,%ebp" );
	
	(*callGetContext)( &uc_auxiliar_ucp );
	(*callreturningFunction)( );
						 
	
	asm volatile( "injectionEnd:" );
}

/* 	Function used to obtain the size of the indicated instruction.
			 Receives the address of the instruction.
			 Returns the size.
			 Is platform-dependent.
	*/
/*
#include "dis-asm.h"

enum bfd_architecture bfd_get_arch (bfd *abfd){
	return bfd_arch_i386;
}
		
int get_instruction_size(int eip){
	int i;
	struct disassemble_info info;

	info.arch = bfd_arch_i386;
	info.target = eip;
	info.mach == bfd_mach_i386_i386;
	disassemble_init_for_target (&info);
	i = print_insn_i386(eip, &info);	
	return i;
}
*/

int get_instruction_size(int dir){
	unsigned char str[1024];
	memcpy(str,(void*)dir,20);
	t_disasm da;
	int ret;
	ret = Disasm(str, 20, 0x400000, &da, DISASM_SIZE);
	return ret;
}


/* 	Function used to manage a bus error.
			 Bus error is expected when writing in a region mmaped over a file of 0 size
	Operations:
			 1. Modifies the program code to insert the i/o management
			 2. Re-mmaps the region to the real (non zero-sized) file
	*/
	
void manejador_bus_error(int sig, siginfo_t* info, void* data){
	struct sigcontext *sc;
	ucontext_t *ucp;
	int bus_address;
	struct uc_io_access_info* access_info = new struct uc_io_access_info;
	struct uc_mapped_io_info* io_info;

	bus_address = (int)info->si_addr;
	access_info->m_bus_address = bus_address;
	access_info->m_thread = uc_get_current_thread();
	// Read implies a SIGBUS, write implies a SIGSEGV
	access_info->m_read = (info->si_signo == SIGBUS);
	uc_io_accesses_list.push_back(access_info);

	io_info = get_io_mapped(bus_address); 
	if (io_info == NULL){
		printf("segmentation fault detected: invalid addr 0x%x\n",bus_address);
		raise(SIGINT);
		sched_yield();
		exit(-1);
	}

	ucp = (ucontext_t *)data;

	// Get the address of the instruction that provokes the error and the next instruction address
	
	sc = (struct sigcontext *)((&(ucp->uc_mcontext))->gregs);
	access_info->m_assembler_addr = ((int)sc->eip)+get_instruction_size(sc->eip);
	
	// Un-protect and modify the original source code to insert the i/o management code
	
	unsigned page = (unsigned)access_info->m_assembler_addr & ~( getpagesize() - 1 );

	/* Unprotects two page sizes to make sure the memcpy will not enter a protected page
	/*	chmod u=rwx page ;-) */
	if( mprotect( (char*)page, 2*getpagesize(), PROT_READ | PROT_WRITE | PROT_EXEC ) )
	{
		perror( "mprotect failed" );
		exit( -1 );
	}
	
	memcpy(access_info->m_backup,(char*)access_info->m_assembler_addr,&injectionEnd - &injectionStart);
	memcpy((char*)access_info->m_assembler_addr,&injectionStart,&injectionEnd - &injectionStart);
	
	// Mmap ping-pong. 
	// Map the region to the real file to get the i/o data and solve the bus error
	
	int ret = munmap((int*)(bus_address&0xffff0000), getpagesize());
	if(ret != 0 ){
		perror("munmap failed(addr)");
	}
	
	void *ret_p = mmap((int*)(bus_address&0xffff0000), getpagesize(), PROT_READ|PROT_WRITE, MAP_FIXED|MAP_SHARED, io_info->m_file1, 0);
	if(ret_p == 0 || ret_p !=(int*)(bus_address&0xffff0000) ){
		perror("mmap failed(addr)");
	}
	if(access_info->m_read){
		*(int*)access_info->m_bus_address = uc_ioread32((int*)access_info->m_bus_address);
	}
}
	
/* 	Function used to perform the initialization of the specific memory region.
			 Set the bus manager and maps the speficied region into an empty file, to provoke bus errors when accesing
	*/

void uc_initialize_peripheral_access(void *addr, int size){

	unsigned int size_page = getpagesize();
	unsigned int addr_i = (int)addr;
	unsigned int offset = addr_i % size_page;
	unsigned int addr_base = addr_i - offset;
	unsigned int total_size = size + offset;
	char name[100];
	struct uc_mapped_io_info *info = new struct uc_mapped_io_info;

	/* Creates the memory region with a zero sized file
	   All accesses will produce a bus error
	*/

	for(int i=0;i<uc_mapped_io_list.size();i++){
		if(addr_base == uc_mapped_io_list[i]->m_addr){
			return;
		}
	}

	sprintf(name,"io_map_%x.tmp",addr);
	info->m_file1 = open(name,O_CREAT|O_RDWR,0666);
	
	int ret2 = ftruncate(info->m_file1,size_page);
	if(ret2 != 0){
		perror("Warning: direct I/O initialization failed(size)");
	}
	
	sprintf(name,"io_map_empty_%x.tmp",addr);
	info->m_file2 = open(name,O_CREAT|O_RDWR,0666);

	ret2 = ftruncate(info->m_file2,0);
	if(ret2 != 0){
		perror("Warning: direct I/O  initialization failed(size)");
	}

	void *ret = mmap((int*)addr_base, size_page, PROT_READ/*|PROT_WRITE*/, MAP_FIXED|MAP_SHARED, info->m_file2, 0);
		if(ret == 0 || ret !=addr ){
		perror("Warning: direct I/O  initialization failed(addr)");
	}

	info->m_addr = (int)addr_base;
	info->m_size = size;
	uc_mapped_io_list.push_back(info);

//	printf("peripheral initialized at addr %x\n",addr);
}

/*
	Function used to perform the initilize the io access subsistem. It loads the signal handlers
	*/
void uc_initialize_io_access_subsystem(){
	/* Assign the bus error and segmentation fault managers*/
	
	struct sigaction action1; 
	action1.sa_sigaction=manejador_bus_error;
	action1.sa_flags = SA_SIGINFO;
	sigaction(SIGBUS, &action1,NULL);
	sigaction(SIGSEGV, &action1,NULL);
		
}


UC_IO_access_class uc_io_access_instance;
