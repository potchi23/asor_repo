#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

int main(int argc, char **argv){

    if(argc != 4){
        printf("Se necesitan 3 argumentos [dir][port][command]");
        return -1;
    }

    struct addrinfo hints;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

    struct addrinfo *res;
    if(getaddrinfo(argv[1], argv[2], &hints, &res) == -1){
        perror("Error obteniendo info");
        return -1;
    }

    int udp_socket = socket(res->ai_family, SOCK_DGRAM, res->ai_protocol);
    if(udp_socket == -1){
        perror("Error socket");
        return -1;
    }
    
    struct sockaddr_storage dest_addr;
    socklen_t addrlen = sizeof(dest_addr);

    char *buf_send = argv[3];

    ssize_t send_size = sendto(udp_socket, buf_send, strlen(buf_send), 0, res->ai_addr, res->ai_addrlen);
    if(send_size == -1){
        perror("Error enviando mensaje");
        return -1;
    }

    char buf_recv[256];
    ssize_t recv_size = recvfrom(udp_socket, buf_recv, 256, 0, res->ai_addr, &res->ai_addrlen);
    if(recv_size == -1){
        perror("Error recibiendo mensaje");
        return -1;
    }

    buf_recv[recv_size] = '\0';

    printf("%s\n", buf_recv);

    return 0;
}