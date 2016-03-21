CC=gcc
CFLAGS= -c -Wall -I.
DEPS= mpv_func.h
MAIN_EXE=tcp_server
OBJ=tcp_server.o mpv_func.o

all: tcp_server

$(MAIN_EXE): $(OBJ)
	$(CC) -o $(MAIN_EXE) $(OBJ)

#%.o: %.c $(DEPS)
%.o: %.c
	$(CC) $(CFLAGS) $?


clean:
	-rm *o *.gch tcp_server 
