CC=gcc
CFLAGS= -c -g -Wall -I.
#DEPS= mpv_func.h
_SERVER=tcp_server
_CLIENT=tcp_client
_S_OBJ=tcp_server.o mpv_func.o proc.o
_C_OBJ=tcp_client.o

all: tcp_server tcp_client

$(_SERVER): $(_S_OBJ)
	$(CC) -o $(_SERVER) -g $(_S_OBJ)

$(_CLIENT): $(_C_OBJ)
	$(CC) -o $(_CLIENT) $(_C_OBJ)

#%.o: %.c $(DEPS)
%.o: %.c
	$(CC) $(CFLAGS) $?


clean:
	-rm *o $(_SERVER) $(_CLIENT) 
#	-rm *o *.gch $(_SERVER) $(_CLIENT) 
