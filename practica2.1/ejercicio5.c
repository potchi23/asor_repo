#include <stdio.h>
#include <sys/utsname.h>

int main(){
    struct utsname unameData;

    if(uname(&unameData) == -1) perror("Error");

    printf("%s\n", unameData.sysname);
    printf("%s\n", unameData.nodename);
    printf("%s\n", unameData.release);
    printf("%s\n", unameData.version);
    printf("%s\n", unameData.machine);

    return 1;
}