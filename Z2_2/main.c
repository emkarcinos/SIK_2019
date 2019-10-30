#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "arpa/inet.h"
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> 

char* convertIndex(char* index){
    unsigned char* finalBytes=malloc(11*sizeof(char));

    memcpy(finalBytes, index, 6);
    
    finalBytes[6]='\n';

    unsigned int indexNumber = htonl(atoi(index));
    memcpy(finalBytes+7, &indexNumber, 4);
    return finalBytes;
}

void sendMessage(char* message){
    struct sockaddr_in adr;
    int gniazdo, r;
    unsigned int port;
    char abcd[512];    

    printf("Podaj adres IP odbiorcy: ");
    scanf("%s", abcd);
    printf("Podaj numer portu odbiorcy: ");
    scanf("%u", &port);
    gniazdo = socket(AF_INET, SOCK_DGRAM, 0);
    adr.sin_family = AF_INET;
    adr.sin_port = htons(port);
    adr.sin_addr.s_addr = inet_addr(abcd);
    printf("Wysyłam wiadomość...");
           
    r = sendto(gniazdo,
               message,
               11,
               0,
               (struct sockaddr*) &adr,
               sizeof(adr));
    if (r != 11) printf("sendto() nie powiodl sie\n");
    else printf("Wiadomosc wyslana.\n");
    close(gniazdo);
    return 0;
}
int main(int argc, char** argv){
    if(argv[1]==NULL || strlen(argv[1])!=6){
        printf("Niepoprawny numer indeksu. Numer powinien składać się z 6 cyfr.");
        return 0;
    }
    printf("Wprowadzony numer indeksu: %s\n", argv[1]);
    unsigned char* out=convertIndex(argv[1]);
    printf("Indeks po konwersji:\n");
    printf("%x %x %x %x %x %x %x %x %x %x %x\n", out[0], out[1], out[2], out[3], out[4], out[5],
                                                    out[6], out[7], out[8], out[9], out[10]);
    sendMessage(out);
}