#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int secs;
char *file;

void handler(int signal){
    if(signal == SIGUSR1){
        printf("El ejecutable no se autodestruye\n");
        exit(0);
    }
}

void loop_handler(int signal){

    printf("%d \n", secs);
    sleep(1);
    secs--;

    if(signal == SIGUSR1)
       raise(SIGINT);

    if(secs > 0)
        raise(SIGUSR2);
    else{
        unlink(file);
        printf("Borrado\n");
        exit(0);
    } 
}

int main(int argc, char **argv){
    if (argc != 2) {
        printf("No se ha introducido los segundos...\n");
        return -1;
    }
    
    file = argv[0];
    secs = atoi(argv[1]);
    sigset_t set;
    struct sigaction act1, act2;
    unsigned int secs = atoi(argv[1]);

    if(sigaction(SIGUSR1, NULL, &act1) == -1){
        perror("sigusr1");
        return -1;
    }
    act1.sa_handler = handler;
    if(sigaction(SIGUSR1, &act1, NULL) == -1){;
        perror("sigusr1");
        return -1;
    }

    if(sigaction(SIGUSR2, NULL, &act2) == -1){
        perror("sigusr2");
        return -1;
    }

    act2.sa_handler = loop_handler;
    
    if(sigaction(SIGUSR2, &act2, NULL) == -1){;
        perror("sigusr2");
        return -1;
    }

    printf("Este ejecutable se autodestruira en %d segundos...\n", secs);
    sigemptyset(&set);

    raise(SIGUSR2);
    sigsuspend(&set);

    return 0;
}