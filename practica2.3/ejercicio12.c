#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int n_SIGINT = 0;
int n_SIGTSTP = 0;

void handler(int signal){
    if(signal == SIGINT) n_SIGINT++;
    if(signal == SIGTSTP) n_SIGTSTP++;
}

int main(){
    sigset_t blk_set;
    struct sigaction act;

    sigaction(SIGINT, NULL, &act); 
    act.sa_handler = handler;
    sigaction(SIGINT, &act, NULL); 
    
    sigaction(SIGTSTP, NULL, &act); 
    act.sa_handler = handler;
    sigaction(SIGTSTP, &act, NULL);

    /*if(sigemptyset(&blk_set) == -1)
        perror("empty set");
    if(sigaddset(&blk_set, SIGINT) == -1)
        perror("blk SIGINT");
    if(sigaddset(&blk_set, SIGTSTP) == -1)
        perror("blk SIGTSTP");
    */

    sigemptyset(&blk_set);
    while(n_SIGINT + n_SIGTSTP < 10){
        sigsuspend(&blk_set);
    }

    printf("\nSIGINT: %d\n", n_SIGINT);
    printf("SIGTSTP: %d\n", n_SIGTSTP);

    return 0;
}