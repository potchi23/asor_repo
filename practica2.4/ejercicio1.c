#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv){
    int pipefd[2];
    pid_t pid;

    if(argc != 5){
        printf("Numero de argumentos no validos\n");
        return -1;
    }

    if(pipe(pipefd) == -1){
        perror("Pipe error");
        return -1;
    }

    pid = fork();

    switch(pid){
        case -1:
            perror("Fork error");
            return -1;
            break;
            
        case 0:
            close(pipefd[1]);
            if(dup2(pipefd[0], 0) == -1){
                perror("dup error");
                return -1;
            }
            
            if(execlp(argv[3], argv[3], argv[4], NULL) == -1){
                perror("error de ejecucion cmd2");
                return -1;
            }

            break;

        default:
            close(pipefd[0]);
            if(dup2(pipefd[1], 1) == -1){
                perror("dup error");
                return -1;
            }
            
            if(execlp(argv[1], argv[1], argv[2], NULL) == -1){
                perror("error de ejecucion cmd1");
                return -1;
            }

            wait(NULL);
            break;
    }

    return 0;
}