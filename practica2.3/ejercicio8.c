#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute(pid_t fork_pid, char *cmd, int argc, char **argv){
    pid_t sid;

    if(fork_pid == 0){
        sid = setsid();
    }

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
}

int main(int argc, char **argv){
    pid_t fork_pid = fork();

    switch(fork_pid){
        case -1:
            perror("fork");
            exit(1);
        case 0:
            sleep(2);
            printf("Hijo\n");
            execute(fork_pid, argv[1], argc, argv);
            break;
        default:
            sleep(5);
            printf("Padre\n");
            execute(fork_pid, argv[1], argc, argv);
            break;
    }

    return 0;
}