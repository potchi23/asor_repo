#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
using namespace std;

int main(int argc, char **argv){
	struct stat statbuf;
	mode_t mode;

	string filedir = argv[1];
	string hardstring = ".hard";
	string symstring = ".sym";

	char hard[filedir.length() + hardstring.length()];
	strcpy(hard, (filedir + hardstring).c_str());

	char sym[filedir.length() + symstring.length()];
	strcpy(sym, (filedir + symstring).c_str());

	if(stat(argv[1], &statbuf) == -1){
        perror("Error leyendo fichero");
    }

	else{
		mode = statbuf.st_mode;

		if(S_ISREG(mode)){
			if(link(argv[1], hard) == -1)
				perror("Error creando enlace rigido");
			if(symlink(argv[1], sym) == -1)
				perror("Error creando enlace simbolico");	
		}
	}
	
	return 1;
}
