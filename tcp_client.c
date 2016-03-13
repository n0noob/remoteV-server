#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

#define ADDR_SERVER "127.0.0.1"

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[1024], ch;
    struct sockaddr_in serv_addr; 

    memset(recvBuff, 0, sizeof(recvBuff));
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); 

    if(inet_pton(AF_INET, ADDR_SERVER, &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    while (1)
    {
        printf("\n\n********************************************\n");
        printf("** This is client side of the application **\n");
        printf("********************************************\n");
        printf("\nPress n to exit");
        printf("\nOptions:\n");
        printf("\t1. Get list (LIST)\n");
        printf("\t2. Play file (PLAY)\n");
        printf("\t3. Stop playback (STOP)\n");
        printf("\t3. Pause playback (PAUS)\n");
        printf("\t9. Clear screen\n");
        ch = getchar();
        getchar();
        switch(ch)
        {
            case '1':
                if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                {
                    printf("\n Error : Could not create socket \n");
                    return 1;
                }

                if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
                {
                   printf("\n Error : Connect Failed \n");
                   return 1;
                }

                if( send(sockfd , "$LIST" , strlen("$LIST") , 0) < 0)
                {
                    puts("Send failed");
                    return 1;
                }
                puts("Data Sent\n");

                while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
                {
                    recvBuff[n] = 0;
                    if(fputs(recvBuff, stdout) == EOF)
                    {
                        printf("\n Error : Fputs error\n");
                    }
                }
                if(n < 0)
                {
                    printf("\n Read error \n");
                }

                close(sockfd);
                break;
                
            case '2':
                if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                {
                    printf("\n Error : Could not create socket \n");
                    return 1;
                }

                if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
                {
                   printf("\n Error : Connect Failed \n");
                   return 1;
                }

                if( send(sockfd , "$PLAY/home/anoop/Downloads/Dexter - Season 1 - BRRip - x264 - AC3 5.1 -={SPARROW}=-/Dexter S01 E06 - BRRip - x264 - AC3 5.1 -={SPARROW}=-.mkv" , strlen("$PLAY/home/anoop/Downloads/Dexter - Season 1 - BRRip - x264 - AC3 5.1 -={SPARROW}=-/Dexter S01 E06 - BRRip - x264 - AC3 5.1 -={SPARROW}=-.mkv") , 0) < 0)
                {
                    puts("Send failed");
                    return 1;
                }
                puts("Data Sent\n");
                break;
            case '3':
                if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                {
                    printf("\n Error : Could not create socket \n");
                    return 1;
                }

                if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
                {
                   printf("\n Error : Connect Failed \n");
                   return 1;
                }

                if( send(sockfd , "$STOP" , strlen("$STOP") , 0) < 0)
                {
                    puts("Send failed");
                    return 1;
                }
                puts("Data Sent\n");
                break;
            case '4':
                if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                {
                    printf("\n Error : Could not create socket \n");
                    return 1;
                }

                if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
                {
                   printf("\n Error : Connect Failed \n");
                   return 1;
                }

                if( send(sockfd , "$PAUS" , strlen("$PAUS") , 0) < 0)
                {
                    puts("Send failed");
                    return 1;
                }
                puts("Data Sent\n");
                break;
            case '9':
                printf("\033[H\033[J");
                //system("clear");
                break;
            case 'n':
                exit(EXIT_SUCCESS);
            default:
                printf("Invalid choice!\n");
                break;
        }
    }
    
    return 0;
}