#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>

int main(int argc, char *argv[]){
    struct passwd *pw;
    pw = getpwuid(getuid());
    printf("uid=%d euid=%d username=%s home=%s description=%s\n", getuid(), geteuid(), pw->pw_name, pw->pw_dir, pw->pw_gecos);

    return 1;
}