#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv){

    char *cmd = argv[1];
    char *arg[argc - 1];

    for(int i = 0; i < argc; i++){ 
        arg[i] = argv[i + 1];
    }

    if(execvp(cmd, arg) == -1){ // en el argumento no se introducen las comillas : ps -el
        perror("execvp error");
    }
    else{
        printf("El comando terminó de ejecutarse\n"); // No se imprime. Execvp reemplaza el proceso 
                                                      // actual por el nuevo proceso creado al ejecutar el comando.
    }

    return 0;
}

