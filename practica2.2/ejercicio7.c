#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

//pasar por parametro el nombre del fichero o ruta
int main(int argc, char** argv){
    umask(00027);
    
    if(open(argv[1], O_CREAT, 00645) == -1)
        perror("Error");

    return 1;
}