#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void print_att(pid_t fork_pid){
    pid_t pid = getpid();
    pid_t ppid = getppid();
    pid_t pgid = getpgid(pid);
    pid_t sid = getsid(pid);

    if(fork_pid == 0){
        if(chdir("/tmp") == -1){
            perror("new cwd error");
        }
        else{
            sid = setsid();
        }
    }

    printf("PID %d\n", pid);
    printf("PPID %d\n", ppid);

    if(pgid == -1) {
        perror("PGPID error");
    }
    else{
        printf("PGPID %d\n", pgid);
    }
    
    if(sid == -1){
        perror("SID error\n");
    }
    else{
        printf("SID %d\n", sid);
    }
    struct rlimit rlim;

    if(getrlimit(RLIMIT_NOFILE, &rlim) == -1){
        perror("Get limit error");
    }
    else{
        printf("MAX NOFILE %ld\n", rlim.rlim_max);
    }

    char buffer[4096];
    if(getcwd(buffer, sizeof(buffer)) == NULL){
        perror("CWD error");
    }
    else{
        printf("cwd %s\n", buffer);
    }
}

int main(int argc, char **argv){
    pid_t fork_pid = fork();

    switch(fork_pid){
        case -1:
            perror("fork");
            exit(1);
        case 0:
            printf("Hijo\n");
            print_att(fork_pid);
            break;
        default:
            sleep(5);
            printf("Padre\n");
            print_att(fork_pid);
            break;
    }

    return 0;
}

/*
    Si termina primero el padre, se termina la ejecucion en la shell, pero posteriormente se ejecuta el hijo.
    Si termina primero el hijo, el comportamiento es similar a si hubieramos hecho un wait en el proceso padre, en el que
    el padre espera a que termine el hijo antes de ejecutarse.
*/