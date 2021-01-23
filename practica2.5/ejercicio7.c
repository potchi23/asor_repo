#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char **argv){

    if(argc != 3){
        printf("Se necesitan dos parametros: [host][port]\n");
        return -1;
    }

    struct addrinfo *res, hints;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct sockaddr_storage addr;
    socklen_t addrlen = sizeof(addr);

    if(getaddrinfo(argv[1], argv[2], &hints, &res) != 0){
        perror("Error obteniendo informacion");
        return -1;
    }

    int tcp_sd = socket(res->ai_family, res->ai_socktype, 0);
    connect(tcp_sd, (struct sockaddr *) &addr, addrlen);

    char buff_cli[257];
    char buff_serv[257];

    ssize_t c;
    while(c = read(0, buff_cli, 256)){
        buff_cli[c] = '\0';

        if(buff_cli[0] == 'q'){
            return 0;
        }

        if(send(tcp_sd, buff_cli, c, 0) == -1){
            perror("Error enviando mensaje");
            return -1;
        }

        c = recv(tcp_sd, buff_serv, 256, 0);
        buff_serv[c] = '\0';
        printf("%s\n", buff_serv);
    }

    return 0;
}