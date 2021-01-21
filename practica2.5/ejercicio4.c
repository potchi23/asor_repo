#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/select.h>

int main(int argc, char **argv){

    if(argc != 3){
        perror("Se necesitan 2 argumentos: [dir][port]\n");
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

    char command[256];
    struct sockaddr_storage storage;
    socklen_t storage_len = sizeof(storage);

    fd_set set;
    FD_ZERO(&set);
    FD_SET(0, &set);
    
    int selection = select(1, &set, NULL, NULL, NULL);

    do{      

        if(selection == -1)
            perror("Error select");
        else if(selection == 1){
            ssize_t size = read(0, command, strlen(command));
            if(size == -1){
                perror("Error lectura de teclado");
                return -1;
            }
            command[size] = '\0';
            printf("Lectura por teclado\n");
        }
        else{
        int size = recvfrom(udp_sd, &command, sizeof(command), 0, (struct sockaddr *) &storage, &storage_len);
        if(size == -1){
            perror("Error leyendo por red");
            return -1;
        }
        command[size - 1] = '\0';
        char host[NI_MAXHOST];
        char port[NI_MAXSERV];
        if(getnameinfo((struct sockaddr *) &storage, storage_len, host, NI_MAXHOST, port, NI_MAXSERV, 0) != 0){
            perror("Error obteniendo direccion");
            return -1;
        }
        printf("%d bytes de %s:%s\n", size, host, command);
        }


        setlocale(LC_TIME, "");
        time_t t;
        struct tm *local;
        char buffer[256];

        time(&t);
        struct tm *lt = localtime(&t);

        if(command[0] == 't'){
            strftime(buffer, 256, "%H:%M:%S", lt);
            int size = sendto(udp_sd, buffer, strlen(buffer), 0, (struct sockaddr *) &storage, storage_len);
            if(size == -1){
                perror("Error enviando hora");
                return -1;
            }
        }
        else if (command[0] == 'd'){
            strftime(buffer, 256, "%d/%m/%Y", lt);
            int size = sendto(udp_sd, buffer, strlen(buffer), 0, (struct sockaddr *) &storage, storage_len);
            if(size == -1){
                perror("Error enviando fecha");
                return -1;
            }
        }
        else if(command[0] == 'q'){
            printf("Salir\n");
        }
        else{
            printf("Comando no valido\n");
        }

    }while(command[0] != 'q');

    return 0;
}