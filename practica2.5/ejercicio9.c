#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>

void handler(int signal){
    pid_t pid = wait(NULL);
    printf("Hijo termino\n");
}

int main(int argc, char **argv){

    if(argc != 3){
        printf("Se necesitan 2 argumentos: [dir][port]\n");
        return -1;
    }

    struct addrinfo *res, hints;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if(getaddrinfo(argv[1], argv[2], &hints, &res) != 0){
        perror("Error obteniendo informacion");
        return -1;
    }

    int tcp_sd = socket(res->ai_family, res->ai_socktype, 0); 

    if(tcp_sd == -1){
        perror("Error creacion de socket");
        return -1;
    }

    if(bind(tcp_sd, res->ai_addr, res->ai_addrlen) == -1){
        perror("Error bind");
        return -1;
    }
    
    freeaddrinfo(res);

    if(listen(tcp_sd, 5) == -1){
        perror("Error listen");
        return -1;
    }

    char buff[256];
    ssize_t c;
    
    signal(SIGCHLD, handler);

    while(1){

        char host[NI_MAXHOST];
        char port[NI_MAXSERV];
        struct sockaddr_storage addr;
        socklen_t addrlen = sizeof(addr);

        int cli_sd = accept(tcp_sd, (struct sockaddr *) &addr, &addrlen);
        if(cli_sd == -1){
            perror("Error aceptando");
            return -1;
        }

        pid_t pid;
        pid = fork();

        switch(pid){
            case -1:
                perror("Error fork");
                return -1;
            break;

            case 0:
            if(getnameinfo((struct sockaddr *) &addr, addrlen, host, NI_MAXHOST, port, NI_MAXSERV, 0) != 0){
                perror("Error nameinfo");
                return -1;
            } 

            printf("Conexion desde %s:%s", host, port);

            while(c = recv(cli_sd, buff, 256, 0)){
                buff[c] = '\0';
                send(cli_sd, buff, c, 0);
            }
            break;

            default:
                pid = wait(NULL);
                return 0;
            break;
        }
    }

    return 0;
}