#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void recorreDirectorio(char* path, off_t &st_size){
    DIR *dir = opendir(path);
    struct dirent *nextDirStruct;
     
    if(dir != NULL){
        nextDirStruct = readdir(dir);
        while(nextDirStruct != NULL){
            if(strcmp(nextDirStruct->d_name, ".") != 0 && strcmp(nextDirStruct->d_name, "..") != 0){
                char nextPath[sizeof(path) + sizeof(nextDirStruct->d_name) + 1]; // el tamaño es el tamaño del path inicial + tamaño del proximo path + tamaño del caracter "/"

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
                    
                    if(stat(nextPath, &statbuf) == -1){
                        perror("Error apertura fichero");
                    }
                    else{
                        mode = statbuf.st_mode;
                        
                        if(S_ISREG(mode))
                            printf("%s\n", nextDirStruct->d_name);
                        else if(S_ISLNK(mode))
                            printf("%s -> \n", nextDirStruct->d_name);
                        else
                            printf("%s*\n", nextDirStruct->d_name);

                        st_size += statbuf.st_size;
                    }
                }

                closedir(nextDir);

                recorreDirectorio(nextPath, st_size);
            }
           
            nextDirStruct = readdir(dir);
        }
    }

    closedir(dir);
}

// /home/richard/Documentos/vscode
int main(int argc, char **argv){
    off_t st_size = 0;

    recorreDirectorio(argv[1], st_size);

    printf("\nTamaño total: %ld bytes\n", st_size);

    return 1;
}