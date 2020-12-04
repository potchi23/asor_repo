#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

const char * const sys_errlist[];
int sys_nerr;
int errno;

int main(int argc, char *argv[]){

    for(int i = 0; i <= 255; i++){
        printf("Error #%d: %s\n", i, strerror(i));
    }

    return 1;
}