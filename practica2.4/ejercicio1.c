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

    char* arg1[2];
    char* arg2[2];

    for(int i = 0; i < argc - 1; i++){
        if(i < 2){
            arg1[i] = argv[i + 1];
        }
        else{
            arg2[i - 2] = argv[i + 1];
        }
    }

    arg1[2] = '\0';
    arg2[2] = '\0';

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
            close(pipefd[0]);
            
            if(execvp(arg1[0], arg1) == -1){
                perror("error de ejecucion cmd1");
                return -1;
            }

            break;
        default:
            close(pipefd[0]);
            if(dup2(pipefd[1], 1) == -1){
                perror("dup error");
                return -1;
            }
            close(pipefd[1]);
            
            if(execvp(arg2[0], arg2) == -1){
                perror("error de ejecucion cmd2");
                return -1;
            }

            //wait(NULL);
            break;
    }

    return 0;
}