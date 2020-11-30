#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

const char * const sys_errlist[];
int sys_nerr;
int errno;

int main(){

    if(setuid(0) == -1){
        perror("Se ha producido un error");
        printf("Error: %s\n", strerror(errno));
    }

    return 1;
}