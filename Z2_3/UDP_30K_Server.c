#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h> 

#define BUFFER_SIZE 30000
#define ATTEMPTS 100

char buf[BUFFER_SIZE];

int main(int argc, char **argv)
{
    struct sockaddr_in myaddr, endpoint;
    int sdsocket, recieved, addrlen;

    if (argc < 2) {
        printf("podaj numer portu jako parametr\n");
        return 1;
    }

    sdsocket = socket(AF_INET, SOCK_DGRAM, 0);
    addrlen = sizeof(struct sockaddr_in);
        
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(atoi(argv[1]));
    myaddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sdsocket,(struct sockaddr*) &myaddr, addrlen) < 0) {
        printf("bind() nie powiodl sie\n");
        return 1;
    }
    
    long long totalRecieved = 0;
    while (totalRecieved < BUFFER_SIZE * ATTEMPTS) {
        recieved=0;
        while (recieved < BUFFER_SIZE){
            int temp = 0;
            temp = recvfrom(sdsocket,
                                buf+recieved,
                                BUFFER_SIZE-recieved,
                                0,
                                (struct sockaddr*) &endpoint,
                                &addrlen);
            if(temp==-1){
                printf("An error has occured in recieving data.\n");
                exit(EXIT_FAILURE);
            }
            recieved += temp;
            totalRecieved += recieved;

        }
        if(sendto(sdsocket, buf, BUFFER_SIZE, 0, (struct sockaddr*) &endpoint, addrlen)==-1){
            printf("An error has occurred with sending data.\n");
            exit(EXIT_FAILURE);
        }
        printf("Sent packets\n");
    }
       
    return 0;
}