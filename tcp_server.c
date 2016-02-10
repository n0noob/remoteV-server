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


int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    char sendBuff[1024];
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

        //Fill the buffer
        //fill_data(sendBuff, ptr);
        //

        strcpy(sendBuff, "Hello from the server!\n");

        write(connfd, sendBuff, strlen(sendBuff)); 

        close(connfd);
        sleep(1);
     }
}


/*No need for this code
char *dummy_data[] =   {"/home/anoop/a.mp4", 
                        "/home/anoop/bc.mkv", 
                        NULL};

int fill_data(char *buffer, char ***ptr){
    while (**ptr){
        snprintf(buffer, 1024*sizeof(char), **ptr);
        snprintf(buffer, 1024*sizeof(buffer), "\n");
        (*ptr)++;
    }
    return 0;
}
*/