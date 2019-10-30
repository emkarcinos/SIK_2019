#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "arpa/inet.h"
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> 

void connectToHost(int *sock){
    unsigned int port;
    char ip[512];    
    struct sockaddr_in adr;

    printf("Podaj adres IP odbiorcy: ");
    scanf("%s", ip);
    printf("Podaj numer portu odbiorcy: ");
    scanf("%u", &port);
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    if(*sock < 0){
        printf("socket() nie powiódł się.\n");
        exit(1);
    }

    adr.sin_family = AF_INET;
    adr.sin_port = htons(port);
    adr.sin_addr.s_addr = inet_addr(ip);

    fflush(stdin);
    if (connect(*sock,(struct sockaddr*) &adr, sizeof(struct sockaddr_in)) < 0) {
            printf("connect() nie powiodl sie\n");
            exit(1);
        }
    else printf("Połączenie nawiązane.\n");
}

void sendMessage(int socket, const char* message){
    printf("Wysyłam wiadomość...");    
    int r = send(socket, message, 8, 0);
    if (r != 8) printf("sendto() nie powiodl sie\n");
    else printf("Wiadomosc wyslana.\n");
    return 0;
}

long getMessage(int socket){
    long recieved;
    if(recv(socket, &recieved, 8, 0) != 8) {
        printf("recv() nie powiódł się.\n");
        return NULL;
    }
    printf("odebrano\n");
    return recieved;
}

int main(int argc, char** argv){
    if(argc!=2 || strlen(argv[1])!=6){
        printf("Niepoprawny numer indeksu. Numer powinien składać się z 6 cyfr.");
        return 0;
    }
    
    int socket;
    connectToHost(&socket);
    char* firstSend=malloc(sizeof(char)*8);
    long x1 = htonl(atol(argv[1]));
    memcpy(firstSend, &x1, 8);

    sendMessage(socket, firstSend);

    long answer = ntohl(getMessage(socket));
    answer++;

    char* finalSend=malloc(sizeof(char)*8);
    long x2=htonl(answer);
    memcpy(finalSend, &x2, 8);
    sendMessage(socket, finalSend);

    close(socket);
}