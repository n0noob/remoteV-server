/*All the mpv related functions are stored in this file*/
#ifndef MPV_FUNC_H
#define MPV_FUNC_H

#include "tcp_server.h"

int touch(char *fpath);

int mpv_setup(MPV_INSTANCE * instance);

int mpv_play(char *file);
int mpv_stop(MPV_INSTANCE * instance);
int mpv_pause(MPV_INSTANCE * instance);
int mpv_seek(MPV_INSTANCE * instance, int seconds);

#endif