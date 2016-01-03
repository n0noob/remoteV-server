#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFLEN 20  //Max length of buffer
#define OP_PORT 2189   //The port on which to send data
#define P_BROADCAST "255.255.255.255"

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(void) {
        
    /* Our process ID and Session ID */
    pid_t pid, sid;
    
    enum states{
        SEARCH,
        PROMPT,
        CONNECTED
    }STATE;
    STATE = SEARCH;

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then
    we can exit the parent process. */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    /* Change the file mode mask */
    umask(0);
    
    /* Open any logs here */

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        /* Log the failure */
        exit(EXIT_FAILURE);
    }
        
        
    /* Change the current working directory */
    if ((chdir("/")) < 0) {
        /* Log the failure */
        exit(EXIT_FAILURE);
    }
        
    /* Close out the standard file descriptors */
    close(STDIN_FILENO);
    //close(STDOUT_FILENO);
    //close(STDERR_FILENO);
        
    /* Daemon-specific initialization goes here */
    

    //CLIENT CODE START
    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];
 
    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
 
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(OP_PORT);
     
    if (inet_aton(P_BROADCAST, &si_other.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
    //CLIENT CODE END


    /* The Big Loop */
    while (1) {
        /* Do some task here ... */
        
        switch(STATE){
            case SEARCH:
                if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
                    die("sendto()");
                break;
            case PROMPT:
                break;
            case CONNECTED:
                break;
            default:
                exit(EXIT_FAILURE);
        } 

        sleep(3); /* wait 3 seconds */
    }
    exit(EXIT_SUCCESS);
}
