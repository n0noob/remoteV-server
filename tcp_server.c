#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <limits.h>
#include <signal.h>

#include "tcp_server.h"
#include "mpv_func.h"

#define TEST_FILE "./mediafile.list"
#define D_BUG


COMMAND cmd[] = {{"$LIST", 1}, {"$PLAY", 2}, {"$STOP", 3}, {"$PAUS", 4}, {"$FFWD", 5}, {"$RWND", 6}};

const int cmd_count = 6;
const int cmd_len = 5;
int connfd = 0;
char * line = NULL;


int compare_string(char * a, char * b)
{
    int i;    
	if(*b != '$')
        return 1;
    for(i = 0; i < cmd_len; i++)
    {
        if(*b == '\0')
            return 1;
        if(*b != *a)
            return 1;
        b++;
        a++;
    }
    return 0;
}

int get_index(char * a)
{
    int i, x;
    for(i = 0; i < cmd_count; i++)
    {
		x = compare_string(a, cmd[i].flag);
		//printf("String compare ka result: %d\n", x);
        if(!x)
            return (cmd[i].index);
    }
    return -1;
}


char * extract_path(char *buffer, int val)
{
    int count = 0, path_length;
    int i;
    char * temp = buffer;
    while(*buffer != 0)
    {
        buffer++;
        count++;
    }
    path_length = count - cmd_len;
    if(path_length < 5)
    {
        printf("\nPath too short");
        exit(EXIT_FAILURE);
    }

    char *x = NULL;
    x = (char *)malloc((path_length * sizeof(char)) + sizeof(char));
    x[path_length] = '\0';

    for(i = 0; i < path_length; i++)
    {
        x[i] = temp[i + cmd_len];
    }

    return x;
}


void sigint_handler(int signum)
{
    close(connfd);
    if (line)
        free(line);
}


int main(int argc, char *argv[])
{
    int listenfd = 0, index;
    struct sockaddr_in serv_addr; 
    
    //mpv's only instance
    static MPV_INSTANCE mpv_i = {0, 0};

    FILE *fp;
    char *file_path = NULL;
    //char * line = NULL;
    size_t len = 0;
    ssize_t read;    
    char rcvBuff[2048];
    //char ***ptr = &dummy_data;

    //signal(SIGINT, sigint_handler);
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 

    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        memset(rcvBuff, 0, sizeof(rcvBuff)); 

        if(recv(connfd, rcvBuff, 2048, 0) < 0)
        {
            puts("recv failed");
            perror("Error: ");
            exit(EXIT_FAILURE);
        }
        
        printf("Recieved : #%s#\n", rcvBuff);
        index = get_index(rcvBuff);

        //FSM
        switch(index)
        {
            case 1:
                fp = fopen(TEST_FILE, "r");
                if (fp == NULL){
                    perror("Error: ");
                    exit(EXIT_FAILURE);
                }

				write(connfd, "5\n", 2);

                while ((read = getline(&line, &len, fp)) != -1) 
                {
					//format_line(line, len);
                    write(connfd, line, strlen(line));
                    //write(connfd, "\n", 1);
                }

                //strcpy(rcvBuff, "Hello from the server!\n");
                //write(connfd, rcvBuff, strlen(rcvBuff)); 

                fclose(fp);
                break;
                
            case 2:
                file_path = extract_path(rcvBuff, cmd_len);
                //printf("File path : %s\n", file_path);
                if(access(file_path, F_OK|R_OK) == -1){
                    printf("File is invalid!");
                    break;
                }
                printf("Before mpv setup\n");
                mpv_setup(&mpv_i);
                printf("After mpv setup\n");
                sleep(1);
                printf("Plaing mpv file\n");
                mpv_play(file_path);
                break;

            case 3:
                mpv_stop(&mpv_i);
                break;

            case 4:
                mpv_pause(&mpv_i);
                break;

            case 5:
                mpv_seek(&mpv_i, 5);
                break;

            case 6:
                mpv_seek(&mpv_i, -5);
                break;

            default:
                printf("Default case!");
                break;    
        }
        close(connfd);
    }
    if (line)
        free(line);
    return 0;
}
