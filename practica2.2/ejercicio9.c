#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sysmacros.h>

#define st_atime st_atim.tv_sec

//pasar ruta por parámetro
int main(int argc, char **argv){

    struct stat statbuf;
    dev_t dev;
    mode_t mode;

    unsigned int major, minor;
    ino_t inode;
    struct timespec st_atim;
    time_t time; //se mostrara en segundos
    char *filetype;

    if(lstat(argv[1], &statbuf) == -1){
        perror("Error");
    }
    else{
        dev = statbuf.st_dev;
        mode = statbuf.st_mode;

        major = major(dev);
        minor = minor(dev);
        inode = statbuf.st_ino;
        st_atim = statbuf.st_atim;
        time = st_atim.tv_sec;

        if(S_ISREG(mode)) filetype = "reg";
        else if(S_ISLNK(mode)) filetype = "lnk";
        else if(S_ISDIR(mode)) filetype = "dir";

        printf("Major:%u Minor:%u i-node:%ld filetype:%s last_access:%lds\n", major, minor, inode, filetype, time);
    }

    return 1;
}

//st_mtime muestra cuando se modifico el fichero tras abrirlo
//st_ctime muestra cuando se han modificado los permisos del fichero