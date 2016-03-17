hellomake: tcp_server.c mpv_func.c mpv_func.h mpv_server.h
	gcc -o tcp_server tcp_server.c mpv_func.c -I -Wall.