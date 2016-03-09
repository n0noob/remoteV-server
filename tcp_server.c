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
#include "tcp_server.h"

#define TEST_FILE "./mediafile.list"
#define MPV_SOCKET_FILE "/tmp/mpv_socket"
#define D_BUG


COMMAND cmd[] = {{"$LIST", 1}, {"$PLYG", 2}, {"$STOP", 3}, {"$PAUS", 4}};

const int cmd_count = 4;
const int cmd_len = 5;

typedef struct mpv_ins{
    pid_t mpv_pid;
    int instance_count;
}MPV_INSTANCE;


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

int compare_string(char * a, char * b)
{
    int i;
    if(*b != '$')
        return 1;
    for(i = 0; i < cmd_len; i++)
    {
        if(*b == NULL)
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
    int i;
    for(i = 0; i < cmd_count; i++)
    {
        if(!compare_string(a, cmd[i].flag))
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

int play(char *file)
{
    //echo '{ "command": ["loadfile", "/home/anoop/a.mp4"] }' | socat - /tmp/mpv_socket 
}

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0, index;
    struct sockaddr_in serv_addr; 
    
    //mpv's only instance
    static MPV_INSTANCE mpv_i = {NULL, 0};

    FILE *fp;
    char *file_path = NULL;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;    
    char rcvBuff[2048];
    //char ***ptr = &dummy_data;

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

                while ((read = getline(&line, &len, fp)) != -1) 
                {
                    write(connfd, line, strlen(line));
                    write(connfd, "$EOL", 4);
                }

                //strcpy(rcvBuff, "Hello from the server!\n");
                //write(connfd, rcvBuff, strlen(rcvBuff)); 

                fclose(fp);
                break;
                
            case 2:
                file_path = extract_path(rcvBuff, cmd_len);
                
                if(access(file_path, F_OK|R_OK) == -1){
                    printf("File is invalid!");
                    break;
                }

                mpv_setup(&mpv_i);

                //play(file_path);
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