#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv){
    int pipefd_ph[2];
    int pipefd_hp[2];
    pid_t pid;
    char buff[240];
    char conf_char[1] = {'l'};
    char received[1];

    if(pipe(pipefd_ph) == -1){
        perror("Pipe error");
        return -1;
    }
    if(pipe(pipefd_hp) == -1){
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
            close(pipefd_ph[1]);
            close(pipefd_hp[0]);
            int i = 1;
            while(i <= 10){
        
                ssize_t size_msg = read(pipefd_ph[0], buff, sizeof(buff) - 1);
                if(size_msg == -1)
                    perror("error lectura de pipe p_h");

                buff[size_msg] = '\0';
                printf("%s", buff);
                sleep(1);
                //printf("holaaa");
                
                if(i == 10)
                    conf_char[0] = 'q';


                ssize_t size_l_char = write(pipefd_hp[1], conf_char, sizeof(conf_char));
                if(size_l_char == -1){
                    perror("Char buffer error");
                    return -1;
                }
                i++;
            }
            
            close(pipefd_ph[0]);
            close(pipefd_hp[1]);

            break;

        default:
            close(pipefd_ph[0]);
            close(pipefd_hp[1]);

            /*if(dup2(pipefd_ph[1], 0) == -1){
                perror("dup error");
                return -1;
            }
     
            if(dup2(1, pipefd_hp[0]) == -1){
                perror("dup error");
                return -1;
            }*/

            char* dummy;
            while(pipefd_hp[0] != 'q'){
                //scanf("%s", dummy);

                ssize_t size_r = read(0, received, sizeof(received));
                if(size_r == -1)
                    perror("error lectura entrada");
                received[size_r] = '\0';

                ssize_t size_w = write(pipefd_ph[1], received, sizeof(received));
                if(size_w == -1)
                    perror("error lectura de pipe de escritura");

                while(conf_char[0] != 'l' && conf_char[0] != 'q'){
                    size_r = read(pipefd_hp[0], conf_char, 1);
					if (size_r == -1) {
						perror("error");
						exit(-1);
					}
                }
            }

            close(pipefd_ph[1]);
            close(pipefd_hp[0]);

            break;
    }

    return 0;
}