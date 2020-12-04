#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

const char * const sys_errlist[];
int sys_nerr;
int errno;

int main(int argc, char *argv[]){

    if(setuid(0) == -1){
        perror("Error en cadena");
        printf("Error en numerico: %d\n", errno);
    }

    return 1;
}