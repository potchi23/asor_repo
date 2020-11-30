#include <stdio.h>
#include <unistd.h>

int main(){

    printf("Numero maximo de enlaces: %ld\n", pathconf("/",_PC_LINK_MAX));
    printf("Tamaño maximo de la ruta: %ld\n", pathconf("/",_PC_PATH_MAX));
    printf("Longitud maxima del nombre de un fichero en el directorio: %ld\n", pathconf("/",_PC_NAME_MAX));

    return 1;
}