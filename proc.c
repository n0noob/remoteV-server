/*Few generic functions are defined here*/
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>

#include "proc.h"

_PROC_STATE check_instance(pid_t pid, char *test_line){
	DIR *dir;
	FILE *fp;
	char buff[1024];

	if(pid <= 1){
		#ifdef DBUG
		fprintf(stderr, "PID is not valid! Hence, mpv not running!\n");
		#endif
		return notrunning;
	}

	if (kill(pid, 0) < 0){
		if(errno == ESRCH){
			/* no such process with the given pid is running */
	    	fprintf(stderr, "Process with given PID does not exists!\n");
	    	return notrunning;	
		}
		else{
			/* some other error... use perror("...") or strerror(errno) to report */
			perror("Error: ");
	    	return error;
		}
	}
	else{
		/*Process is running*/
		if(!(dir = opendir("/proc/"))){
			perror("Error: ");
			fprintf(stderr, "Error in opening /proc directory!\n");
			return error;
		}
		closedir(dir);
		#ifdef DBUG
		printf("Path of cmdline : /proc/%d/cmdline\n", pid);
		#endif

		snprintf(buff, sizeof(buff), "/proc/%d/cmdline", pid);
		fp = fopen(buff, "r");
		if(fp){
			if(fgets(buff, sizeof(buff), fp) != NULL){
				#ifdef DBUG
				printf("CMDLINE : \n");
				puts(buff);
				#endif
				if(strcmp(buff, test_line) == 0){
					#ifdef DBUG
					printf("\nMPV seem to be running\n");
					#endif
					fclose(fp);
					return running;
				}
			}
		}
		fclose(fp);
		return notrunning;
	}
}