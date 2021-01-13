#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(){
    sigset_t blk_set, pending_set;

    if(sigemptyset(&blk_set) == -1)
        perror("empty set");
    if(sigaddset(&blk_set, SIGINT) == -1)
        perror("blk SIGINT");
    if(sigaddset(&blk_set, SIGTSTP) == -1)
        perror("blk SIGTSTP");

    printf("Sleep...\n");
    sigprocmask(SIG_BLOCK, &blk_set, NULL);
    
    char *sleep_secs = getenv("SLEEP_SECS");
    int secs = atoi(sleep_secs);
    
    sleep(secs);

    if(sigpending(&pending_set) == -1){
        perror("sigpending");
    };

    if(sigismember(&pending_set, SIGINT)){
        printf("\nSe ha recibido la señal SIGINT\n");
    }
    if(sigismember(&pending_set, SIGTSTP)){
        printf("\nSe ha recibido la señal SIGTSTP\n");
        sigdelset(&blk_set, SIGTSTP);
    }

    sigprocmask(SIG_UNBLOCK, &blk_set, NULL);

    return 0;
}