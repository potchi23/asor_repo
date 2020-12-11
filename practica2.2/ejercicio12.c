#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv){
    int fd = open(argv[1], O_CREAT | O_APPEND | O_RDWR, 0666);
    if(fd == -1){
        perror("Error apertura de fichero");
    }
    else{
        dup2(fd, 1);
        printf("Hola\n");
    }

    close(fd);

    return 1;
}