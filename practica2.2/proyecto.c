#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void extended_ls(char* path, off_t *st_size){
    DIR *dir = opendir(path);
    struct dirent *nextDirStruct;
     
    if(dir != NULL){
        nextDirStruct = readdir(dir);
        while(nextDirStruct != NULL){
            if(strcmp(nextDirStruct->d_name, ".") != 0 && strcmp(nextDirStruct->d_name, "..") != 0){

                /* El tamaño de nextPath es el tamaño del path inicial + tamaño del proximo path + tamaño del caracter "/". */
                char nextPath[sizeof(path) + sizeof(nextDirStruct->d_name) + 1]; 

                strcpy(nextPath, path);
                strcat(nextPath, "/");
                strcat(nextPath, nextDirStruct->d_name);

                DIR *nextDir = opendir(nextPath);

                if(nextDir != NULL){
                    printf("%s/\n", nextDirStruct->d_name);
                }
                else{
                    struct stat statbuf;
                    mode_t mode;
                    
                    if(lstat(nextPath, &statbuf) == -1){
                        perror("Error apertura fichero");
                    }
                    else{
                        mode = statbuf.st_mode;
                        
                        if(S_ISREG(mode)){
                            if(mode & S_IXUSR){
                                printf("%s*\n", nextDirStruct->d_name);
                            }
                            else{
                                printf("%s\n", nextDirStruct->d_name);
                            }
                        }
                        else if(S_ISLNK(mode)){
                            char *buf;
                            ssize_t size = readlink(nextPath, buf, sizeof(buf) - 1);
                            if(size == -1) perror("Error leyendo enlace simbólico");
                            else{
                                buf[size] = '\0';
                                printf("%s -> %s\n", nextDirStruct->d_name, buf);
                            }
                        }

                        *st_size += statbuf.st_size;
                    }
                }

                closedir(nextDir);

                extended_ls(nextPath, st_size);
            }
           
            nextDirStruct = readdir(dir);
        }
    }

    closedir(dir);
}

int main(int argc, char **argv){
    
    if(argc == 2){
        off_t st_size = 0;

        extended_ls(argv[1], &st_size);
        
        printf("\nTamaño total: %ld kB\n", st_size/1000);
    }
    else{
        printf("Número de argumentos permitidos: 1\n");
    }

    return 1;
}