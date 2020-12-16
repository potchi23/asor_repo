#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>

int recorreDirectorio(char* path){
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
                    printf("%s\n", nextDirStruct->d_name);
                }

                closedir(nextDir);

                recorreDirectorio(nextPath);
            }
           
            nextDirStruct = readdir(dir);
        }
    }

    closedir(dir);
}

// /home/richard/Documentos/vscode
int main(int argc, char **argv){

    recorreDirectorio(argv[1]);

    return 1;
}