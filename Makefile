CC=gcc
CFLAGS=-I.

tcp_server: tcp_server.c mpv_func.c mpv_func.h
	$(CC) -o tcp_server tcp_server.c mpv_func.c -I -Wall.