#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>

#define BUFFER_SIZE 10000
#define ATTEMPTS 100

#define FILENAME "log.txt"

char buf[BUFFER_SIZE];

int main(int argc, char **argv)
{
    struct sockaddr_in endpoint;
    int sdsocket, addrlen, i, received;
    struct hostent *he;
    struct timeval time_b, time_e;

    if (argc<3) {
        printf("Podaj nazwe hosta i numer portu jako parametry\n");
        return 1;
    }

    he = gethostbyname(argv[1]);
    if (he == NULL) {
        printf("Nieznany host: %s\n",argv[1]);
        return 0;
    }

    endpoint.sin_family = AF_INET;
    endpoint.sin_port = htons(atoi(argv[2]));
    endpoint.sin_addr = *(struct in_addr*) he->h_addr_list[0];
    addrlen = sizeof(struct sockaddr_in);

    gettimeofday(&time_b, NULL);
    for (i=0; i<ATTEMPTS; i++) {
        if ((sdsocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("socket() %d nie powiodl sie\n", i);
            return 1;
        }
        if (connect(sdsocket,(struct sockaddr*) &endpoint, addrlen) < 0) {
            printf("connect() %d nie powiodl sie\n", i);
            return 0;
        }
        if(send(sdsocket, buf, BUFFER_SIZE, 0)==-1){
            printf("An error has occurred with sending the data.\n");
            exit(EXIT_FAILURE);
        }
        received = 0;
        while (received < BUFFER_SIZE)
        {
            received += recv(sdsocket, 
                             buf+received, 
                             BUFFER_SIZE-received, 
                             0);
            if(received==-1){
                printf("An error has occured in recieving data.\n");
                exit(EXIT_FAILURE);
            }
        }
        close(sdsocket);
    }
    gettimeofday(&time_e, NULL);

    /* Zapis rezultatu do pliku */
    FILE *log=fopen(FILENAME, "a");
    if(log==NULL) {
        perror("Błąd pliku wyjściowego.");
    } else {
            fprintf(log, "Średnia wydajność protokołu TCP przy bloku 10 000 bajtów:\n");
            fprintf(log, "czas: %.6f s\n",
                                    (((double) (time_e.tv_sec - time_b.tv_sec) * 1000000) +
                                    ((double) (time_e.tv_usec - time_b.tv_usec)))
                                    / (1000000.0 * ATTEMPTS));
    }
    return 0;
}