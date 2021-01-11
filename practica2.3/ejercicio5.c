#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(int argc, char **argv){
    pid_t pid = getpid();
    pid_t ppid = getppid();
    pid_t pgid = getpgid(pid);
    pid_t sid = getsid(pid);

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

    return 0;
}