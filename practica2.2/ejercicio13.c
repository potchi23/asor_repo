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
        dup2(fd, 2);
        perror("Mensaje por salida de error");
    }

    close(fd);

    return 1;
}

// Hay diferencias si las redirecciones se hacen en distinto orden
// cuando los descriptores son distintos

/*  No es lo mismo ya que ls > dirlist 2>&1 imprime todo lo que salga de ls
    (incluido errores) al fichero dirlist, y despues, los errores que se produzcan
    en ese acceso a dirlist, se imprimen por la salida estándar.

    ls 2>&1 > dirlist imprime la salida de error por pantalla, pero no se escribe 
    nada en dirlist
*/