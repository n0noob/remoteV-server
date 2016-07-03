#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h> 

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

typedef enum player_state{
	PLAYING, PAUSED, IDLE, ERROR
}_PLAYER_STATE;

typedef struct player_details{
	_PLAYER_STATE p_state;
	char * file;
	int volume;
	bool full_screen;
}PLAYER_DETAILS;

/*
STATE
{
	"PLAYER OPENED" : "TRUE/FALSE",
	"PLAYER DETAILS" : {
		"PLAYER STATE", "PLAYING/PAUSED/OPENED(or stopped)/NOT OPENED",
		"FILE", "/home/anoop/a.mp4",
		"VOLUME", 90,
		"FULL SCREEN", "Y/N"
	}
}
*/

typedef struct server_state{
	bool player_opened;
	PLAYER_DETAILS details;
}SERVER_STATE;

#endif