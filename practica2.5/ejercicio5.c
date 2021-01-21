#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char **argv){
    pid_t *pid_arr;
    int n = 0;

    if(argc != 3){
        perror("Se necesitan 2 argumentos: [dir][port]");
        return -1;
    }

    struct addrinfo *res;
    if(getaddrinfo(argv[1], argv[2], NULL, &res) != 0){
        perror("Error obteniendo direccion");
        return -1;
    }

    int udp_sd = socket(res->ai_family, SOCK_DGRAM, 0);

    if(udp_sd == -1){
        perror("Error creacion de socket UDP");
        return -1;
    }
    
    if(bind(udp_sd, res->ai_addr, res->ai_addrlen) == -1){
        perror("Error bind");
        return -1;
    }

    char command;
    struct sockaddr_storage storage;
    socklen_t storage_len = sizeof(storage);

    while(command != 'q'){
        pid_t pid = fork();
        int size;

        switch(pid){
            case -1:
                perror("Error fork");
                return -1;
                break;
            case 0:
                size = recvfrom(udp_sd, &command, sizeof(char), 0, (struct sockaddr *) &storage, &storage_len);

                if(size == -1){
                    perror("Error leyendo");
                    return -1;
                }

                char host[NI_MAXHOST];
                char port[NI_MAXSERV];

                if(getnameinfo((struct sockaddr *) &storage, storage_len, host, NI_MAXHOST, port, NI_MAXSERV, 0) != 0){
                    perror("Error obteniendo direccion");
                    return -1;
                }

                printf("%d bytes de %s:%s PID:%d\n", size, host, port, getpid());

                setlocale(LC_TIME, "");
                time_t t;
                struct tm *local;
                char buffer[256];

                time(&t);
                struct tm *lt = localtime(&t);

                if(command == 't'){
                    strftime(buffer, 256, "%H:%M:%S", lt);
                    int size = sendto(udp_sd, buffer, strlen(buffer), 0, (struct sockaddr *) &storage, storage_len);
                    if(size == -1){
                        perror("Error enviando hora");
                        return -1;
                    }
                }
                else if (command == 'd'){
                    strftime(buffer, 256, "%d/%m/%Y", lt);
                    int size = sendto(udp_sd, buffer, strlen(buffer), 0, (struct sockaddr *) &storage, storage_len);
                    if(size == -1){
                        perror("Error enviando fecha");
                        return -1;
                    }
                }
                else if(command == 'q'){
                    printf("Salir\n");
                }
                else{
                    printf("Comando no valido\n");
                }

                break;

            default:
                wait(NULL);
            break;
        }
    }
    return 0;
}