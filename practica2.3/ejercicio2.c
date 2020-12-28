#include <stdio.h>
#include <sched.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv){
    char* policy_string[7] = {"SCHED_NORMAL", "SCHED_FIFO", "SCHED_RR", "SCHED_BATCH", "","SCHED_IDLE", "SCHED_DEADLINE"};

    pid_t pid = getpid();
    int policy_int = sched_getscheduler(pid);
    struct sched_param param;

    if(policy_int == -1) perror("Error");
    else{
        printf("Politica: %s\n", policy_string[policy_int]);

        if(sched_getparam(pid, &param) == -1){
            perror("Error");
        }
        else{
            printf("Prioridad: %d\n", param.sched_priority);

            int max = sched_get_priority_max(policy_int);
            int min = sched_get_priority_min(policy_int);

            if(max == -1 || min == -1){
                perror("Error");
            }
            else{
                printf("Minimo: %d Maximo: %d\n", min, max);
            }
        }
    }

    return 1;
}