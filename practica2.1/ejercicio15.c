#include <stdio.h>
#include <time.h>
#include <locale.h>

int main(int argc, char *argv[]){
    time_t t;
    char *s;

    setlocale(LC_TIME, "");
    time(&t);
    struct tm *timeStruct = localtime(&t);

    strftime(s, 100, "%A, %d de %B de %Y, %H:%M", timeStruct);

    printf("Fecha: %s\n", s);

    return 1;
}