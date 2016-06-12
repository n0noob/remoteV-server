#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mpv_func.h"
#include "proc.h"

#define MPV_SOCKET_FILE "/tmp/mpv_socket"
#define MPV_PATH "/usr/bin/mpv"

#ifndef DBUG
    #define DBUG
#endif


int touch(char *fpath){

    if(access(fpath, F_OK|R_OK) != -1){
        return 0;
    }

    FILE *fp; 
    if((fp = fopen(fpath, "w")) == NULL){
        perror("Error: ");
        exit(EXIT_FAILURE);
    }
    fclose(fp);
    return 0;
}

int mpv_setup(MPV_INSTANCE * instance)
{
    pid_t pid;
	int r;

    if(instance->instance_count > 0){
        return -1;
    }

	fflush(stdout);
    if((pid = fork()) < 0){
        printf("Error in forking mpv!\n");
        perror("Error: ");
        exit(2);
    }
    else if(pid == 0){          //for child process
		printf("Control reached child\n");        
		touch(MPV_SOCKET_FILE);
        r = execl("/usr/bin/mpv", "/usr/bin/mpv", "--profile=pseudo-gui", "--input-unix-socket="MPV_SOCKET_FILE, NULL);
		perror("Error: ");
		if(r != 0)
			printf("Exec did not execute properly!\n");
        exit(EXIT_SUCCESS); 
    }
    else{
        instance->mpv_pid = pid;
        //Still need to check this code
        instance->instance_count = 1;
        return 0;
    }
}

int check_mpv_instance(MPV_INSTANCE *instance){
    if(check_instance(instance->mpv_pid, MPV_PATH) == running){
        #ifdef DBUG
        printf("MPV is running!\n");
        #endif
    }
    else{
        #ifdef DBUG
        fprintf(stderr, "MPV is not running!\n");
        #endif
        instance->instance_count = 0;
    }
    return 0;
}

int mpv_play(char *file)
{
    char command[2048];
    memset(command, 0, sizeof(command));
    snprintf(command, 2048, "echo \'{ \"command\": [\"loadfile\", \"%s\"] }\' | socat - /tmp/mpv_socket", file);

    system(command);
    return 0;
}

int mpv_stop(MPV_INSTANCE * instance)
{
    char command[2048];

    if(instance->instance_count == 0){
        printf("Error : Player not running\n");
        return -1;
    }

    memset(command, 0, sizeof(command));
    snprintf(command, 2048, "echo \'{ \"command\": [\"stop\"] }\' | socat - /tmp/mpv_socket");

    system(command);
    instance->instance_count = 0;
    instance->mpv_pid = 0;

    return 0;
}

int mpv_pause(MPV_INSTANCE * instance)
{
    char command[2048];

    if(instance->instance_count == 0){
        printf("Error : Player not running\n");
        return -1;
    }

    memset(command, 0, sizeof(command));
    snprintf(command, 2048, "echo \'{ \"command\": [\"cycle\", \"pause\"] }\' | socat - /tmp/mpv_socket");

    system(command);

    return 0;
}

int mpv_seek(MPV_INSTANCE * instance, int seconds)
{
    char command[2048];

    if(instance->instance_count == 0){
        printf("Error : Player not running\n");
        return -1;
    }

    memset(command, 0, sizeof(command));
    snprintf(command, 2048, "echo \'{ \"command\": [\"seek\", \"%d\"] }\' | socat - /tmp/mpv_socket", seconds);

    system(command);

    return 0;
}
