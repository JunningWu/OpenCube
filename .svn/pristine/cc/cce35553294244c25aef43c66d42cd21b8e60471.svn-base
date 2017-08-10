#include "stdio.h"
#include "stdlib.h"

struct linux_process_stats{
	int pid;
	char comm[50] ;
	char state ;
	int ppid ;
	int pgrp ;
	int session ;
	int tty_nr ;
	int tpgid ;
	unsigned long flags ; 
	unsigned long minflt ;
	unsigned long cminflt ; 
	unsigned long majflt ; 
	unsigned long cmajflt ;
	unsigned long utime ; 
	unsigned long stime ;
	long cutime ; 
	long cstime ; 
	long priority ; 
	long nice ;
	long num_threads ;
	long itrealvalue ; 
	unsigned long starttime ; 
	unsigned long vsize ; 
	long rss ; 
	unsigned long rlim ;
	unsigned long startcode ; 
	unsigned long endcode ; 
	unsigned long startstack ; 
	unsigned long kstkesp ; 
	unsigned long kstkeip ; 
	unsigned long signal ; 
	unsigned long blocked ; 
	unsigned long sigignore ; 
	unsigned long sigcatch ; 
	unsigned long wchan ; 
	unsigned long nswap ; 
	unsigned long cnswap ; 
	int exit_signal ;
	int processor ;
	unsigned long rt_priority ; 
	unsigned long policy;
};

linux_process_stats * linux_read_stats(){

	FILE *fstats=fopen("/proc/self/stat","r");
	if(fstats == NULL){
		perror("Error opening stat file:");
		exit(-1);
	}
	linux_process_stats *stats = (linux_process_stats*)malloc(sizeof(linux_process_stats));

	fscanf(fstats,"%d %s %c %d %d "
	"%d %d %d %lu %lu "
	"%lu %lu %lu %lu %lu"
	"%ld %ld %ld %ld %ld"
	"%ld %lu %lu %ld %lu"
	"%lu %lu %lu %lu %lu "
	"%lu %lu %lu %lu %lu "
	"%lu %lu %d %d %lu"
	"%lu", &stats->pid , stats->comm , &stats-> state , &stats-> ppid , &stats->pgrp , &stats->session , &stats->tty_nr , &stats->tpgid , &stats->flags , &stats-> minflt , &stats->cminflt , &stats-> majflt , &stats-> cmajflt , &stats-> utime , &stats-> stime , &stats->cutime , &stats-> cstime , &stats-> priority , &stats-> nice , &stats->num_threads , &stats->itrealvalue , &stats-> starttime , &stats-> vsize , &stats-> rss , &stats-> rlim , &stats->startcode , &stats-> endcode , &stats-> startstack , &stats-> kstkesp , &stats-> kstkeip , &stats-> signal , &stats-> blocked , &stats-> sigignore , &stats-> sigcatch , &stats-> wchan , &stats-> nswap , &stats-> cnswap , &stats-> exit_signal , &stats->processor , &stats->rt_priority , &stats-> policy );

	printf("pid %d comm %s state %c ppid %d pgrp %d \n"
	"session %d tty_nr %d tpgid %d flags %lu minflt %lu \n"
	"cminflt %lu majflt %lu cmajflt %lu utime %lu stime %lu \n"
	"cutime %ld cstime %ld priority %ld nice %ld num_threads %ld \n"
	"itrealvalue %ld starttime %lu vsize %lu rss %ld rlim %lu \n"
	"startcode 0x%x endcode 0x%x startstack 0x%x kstkesp 0x%x kstkeip 0x%x \n"
	"signal %lu blocked %lu sigignore %lu sigcatch %lu wchan %lu \n"
	"nswap %lu cnswap %lu exit_signal %d processor %d rt_priority %lu \n"
	"policy %lu \n", stats->pid , stats->comm , stats-> state , stats-> ppid , stats->pgrp , stats->session , stats->tty_nr , stats->tpgid , stats->flags , stats-> minflt , stats->cminflt , stats-> majflt , stats-> cmajflt , stats-> utime , stats-> stime , stats->cutime , stats-> cstime , stats-> priority , stats-> nice , stats->num_threads , stats->itrealvalue , stats-> starttime , stats-> vsize , stats-> rss , stats-> rlim , stats->startcode , stats-> endcode , stats-> startstack , stats-> kstkesp , stats-> kstkeip , stats-> signal , stats-> blocked , stats-> sigignore , stats-> sigcatch , stats-> wchan , stats-> nswap , stats-> cnswap , stats-> exit_signal , stats->processor , stats->rt_priority , stats-> policy );

	char line[200];
	int index = 0;
	char c;

	FILE *maps = fopen("/proc/self/maps","r");
	if(fstats == NULL){
		perror("Error opening maps file:");
		exit(-1);
	}
	while((c=fgetc(maps))!=EOF){
		if(c!='\n'){
			line[index++] = c;
		}
		else{
			line[index] = '\0';
			index = 0;
			printf("%s\n",line);
		}
	}
	line[index] = '\0';
	printf("%s\n",line);

	return stats;
}

// int main(){
// 	linux_read_stats();
// 	return 0;
// }
