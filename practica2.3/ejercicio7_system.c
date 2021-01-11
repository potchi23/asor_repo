#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){

    if(system(argv[1]) == -1){ // en el argumento se introducen las comillas : "ps -el"
        perror("system error");
    }
    else{
        printf("El comando terminó de ejecutarse\n"); // Se imprime. System hace un fork del proceso actual
                                                      // para ejecutar el comando indicado.
    }

    return 0;
}

