#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char **argv){
    char buffer[256];
    fd_set *set;

    FD_ZERO(set);

    int fd1 = open("pipe", O_RDONLY | O_NONBLOCK);
    if(fd1 == -1){
        perror("error de apertura de pipe");
        return -1;
    }

    int fd2 = open("pipe2", O_RDONLY | O_NONBLOCK);
    if(fd2 == -1){
        perror("error de apertura de pipe2");
        return -1;
    }

    FD_SET(fd1, set);
    FD_SET(fd2, set);

    int nfds = fd1 < fd2 ? fd2 + 1 : fd1 + 1;

    int selection = select(nfds, set, NULL, NULL, NULL);

    if(selection > 0){
        
        int pipe;
        if(FD_ISSET(fd1, set)){
            pipe = fd1;
        }
        else{
            pipe = fd2;
        }

        ssize_t size = read(pipe, buffer, strlen(buffer));
        if(size == -1){
            perror("Error de lectura en buffer");
            return -1;
        }

        printf("In: %s", buffer);

        if(pipe == fd1){
            close(fd1);
            fd1 = open("pipe", O_RDONLY | O_NONBLOCK);
            if(fd1 == -1){
                perror("error apertura de pipe");
                return -1;
            }
        }
        else if(pipe == fd2){
            close(fd2);
            fd2 = open("pipe2", O_RDONLY | O_NONBLOCK);
            if(fd2 == -1){
                perror("Error apertura de pipe2");
                return -1;
            }
        }
    }

    close(fd1);
    close(fd2);

    return 0;
}