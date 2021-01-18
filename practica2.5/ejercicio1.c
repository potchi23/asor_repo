#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char **argv){

    if(argc != 2){
        perror("Solo puede haber 1 argumento");
        return -1;
    }

    struct addrinfo *res, *next;

    if(getaddrinfo(argv[1], NULL, NULL, &res) != 0){
        perror("Error obteniendo info");
    }

    next = res;
    while(next != NULL){
        char *host;

        if(getnameinfo(next->ai_addr, next->ai_addrlen, host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST) != 0){
            perror("Error obteniendo informacion");
            return -1;
        }

        printf("Host: %s\nFamilia: %i\nSocket: %i\n", host, next->ai_family, next->ai_socktype);
        printf("\n");
        next = next->ai_next;
    }
    
    return 0;
}