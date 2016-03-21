#include <stdio.h>
#include <sys/types.h>

#ifndef TCP_SERVER_H
#define TCP_SERVER_H

typedef struct command{
	char flag[6];
	int index;
}COMMAND;

typedef struct mpv_ins{
    pid_t mpv_pid;
    int instance_count;
}MPV_INSTANCE;

#endif