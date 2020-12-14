#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <locale.h>

static struct flock lock;
static char* lock_type[3] = {"READ LOCKED", "WRITE LOCKED", "UNLOCKED"};

int main(int argc, char **argv){    
    int fd = open(argv[1], O_RDONLY, 0444);

    if(fcntl(fd, F_GETLK, &lock) == -1)
        perror("Error consulta cerrojo");
    printf("Estado:%s ", lock_type[lock.l_type]);

    if(lock.l_type == 2){
        time_t t;
        char *s;
        setlocale(LC_TIME, "");
        time(&t);
        struct tm *timeStruct = localtime(&t);
        strftime(s, 100, "%H:%M:%S", timeStruct);
        printf("Hora %s\n", s);


        lock.l_type = F_RDLCK;
        if(fcntl(fd, F_SETLK, &lock) == -1)
            perror("Error cerrojo");
        printf("Estado:%s\n", lock_type[lock.l_type]);

        sleep(30);

        lock.l_type = F_UNLCK;
        if(fcntl(fd, F_SETLK, &lock) == -1)
            perror("Error cerrojo");
        printf("Estado:%s\n", lock_type[lock.l_type]);
    }

    close(fd);
    return 1;
}