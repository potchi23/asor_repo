#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]){
    time_t t;

    time(&t);

    struct tm *timeStruct = localtime(&t);

    printf("%d\n", timeStruct->tm_year + 1900);


    return 1;
}