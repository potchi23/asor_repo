#include <stdio.h>
#include <sys/time.h>

int main(int argc, char *argv[]){
    struct timeval tv;
    long int ini;
    long int end;
    long int resul;
    
    gettimeofday(&tv, NULL);
    ini = tv.tv_usec;

    for(int i = 0; i < 1000000; i++){
    }

    gettimeofday(&tv, NULL);
    end = tv.tv_usec;

    resul = (end - ini);

    printf("Ha tardado: %ld microsegundos\n", resul);

    return 1;
}