#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mpv_func.h"

#define MPV_SOCKET_FILE "/tmp/mpv_socket"


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

    if(instance->instance_count > 0){
        return -1;
    }

    if((pid = fork()) < 0){
        printf("Error in forking mpv!");
        perror("Error: ");
        exit(2);
    }
    else if(pid == 0){          //for child process
        touch(MPV_SOCKET_FILE);
        execl("/bin/mpv", "/bin/mpv", "--profile=pseudo-gui", "--input-unix-socket="MPV_SOCKET_FILE, NULL);
        exit(EXIT_SUCCESS); 
    }
    else{
        instance->mpv_pid = pid;
        //Still need to check this code
        instance->instance_count = instance->instance_count + 1;
        return 0;
    }
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
        printf("Error : Player not running");
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
        printf("Error : Player not running");
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
        printf("Error : Player not running");
        return -1;
    }

    memset(command, 0, sizeof(command));
    snprintf(command, 2048, "echo \'{ \"command\": [\"seek\", \"%d\"] }\' | socat - /tmp/mpv_socket", seconds);

    system(command);

    return 0;
}