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

#define TEST_FILE "./mediafile.list"

int compare_string(char * a, char * b)
{
    int i;
    if(*b != '$')
        return 1;
    for(i = 0; i < 5; i++)
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

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0, count = 5;
    struct sockaddr_in serv_addr; 
    
    FILE *fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;    
    char sendBuff[8192];
    //char ***ptr = &dummy_data;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 

    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

        if(recv(connfd, sendBuff , 8192 , 0) < 0)
        {
            puts("recv failed");
            exit(EXIT_FAILURE);
        }
        if(compare_string("$LIST", sendBuff) == 0)
        {
            puts("$LIST");
        }
        puts("Invalid command recieved");

        if(count == 5)
        {
            fp = fopen(TEST_FILE, "r");
            if (fp == NULL)
                exit(EXIT_FAILURE);

            while ((read = getline(&line, &len, fp)) != -1) 
            {
                //printf("%s", line);
                write(connfd, line, strlen(line));
                write(connfd, "$EOL", 4);
            }

            //strcpy(sendBuff, "Hello from the server!\n");
            //write(connfd, sendBuff, strlen(sendBuff)); 

            fclose(fp);
            count = 0;
        }
        count ++;
        close(connfd);
        sleep(1);
    }
    if (line)
        free(line);
     return 0;
}