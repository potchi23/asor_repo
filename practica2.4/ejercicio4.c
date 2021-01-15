#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO "/tmp/fifo"

// El pipe se encuentra en /tmp/fifo

int main(int argc, char **argv){

    if(argc != 2){
        printf("Error: Solo se puede introducir 1 argumento\n");
        return -1;
    }

    if(mkfifo(FIFO, 0666) == -1 ){
        perror("Error creacion pipe");
        return -1;
    }

    int fd = open(FIFO, O_WRONLY);
    if(fd == -1){
        perror("Error abriendo fichero");
        return -1;
    }
    
    ssize_t size = write(fd, argv[1], strlen(argv[1]));
    if(size == -1){
        perror("Error de escritura en pipe");
        return -1;
    }

    close(fd);
    unlink(FIFO);

    return 0;
}