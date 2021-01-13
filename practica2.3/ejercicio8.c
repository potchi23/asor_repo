#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define DAEMON_OUT "/tmp/daemon.out"
#define DAEMON_ERR "/tmp/daemon.err"
#define STANDARD_IN "/dev/null"

void execute(pid_t fork_pid, char *cmd, int argc, char **argv){
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

    if(argc < 2){
        perror("Se necesita al menos un argumento");
        return -1;
    }

    switch(fork_pid){
        case -1:
            perror("fork");
            exit(-1);
        case 0:
            if(setsid() == -1){
                perror("sesion");
                return -1;
            }
            else{
                printf("Hijo\n");
                int standard_in = open(STANDARD_IN, O_WRONLY | O_CREAT);
                int daemon_out = open(DAEMON_OUT, O_WRONLY | O_CREAT);
                int daemon_err = open(DAEMON_ERR, O_WRONLY | O_CREAT);
                
                if(standard_in == -1 || daemon_out == -1 || daemon_err == -1){
                    perror("fichero");
                    close(standard_in);
                    close(daemon_out);
                    close(daemon_err);

                    return -1;
                }

                int dup_in = dup2(standard_in, 0);
                int dup_out = dup2(daemon_out, 1);
                int dup_err = dup2(daemon_err, 2);

                if(dup_in == -1 || dup_out == -1 || dup_err == -1){
                    perror("dup");
                    return -1;
                }

                execute(fork_pid, argv[1], argc, argv);
                close(standard_in);
                close(daemon_out);
                close(daemon_err);
            }
            break;
        default:
            printf("Padre\n");
            break;
    }

    return 0;
}