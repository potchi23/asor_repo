#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]){
    printf("uid=%d euid=%d\n", getuid(), geteuid());

    return 1;
}