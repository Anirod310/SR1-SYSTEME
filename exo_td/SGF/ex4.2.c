#define _POSIX_C_SOURCE 202405L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

long TailleRepCour(void){

    DIR* dir = opendir(".");
    if(dir==NULL){
        perror(".");
        return(-1);
    }

    struct stat Infos;
    struct dirent* Element;
    long taille=0;

    while((Element=readdir(dir))!=NULL){
        if(lstat(Element->d_name, &Infos) == -1){
            perror("lstat");
            return(-1);
        }
        if(S_ISREG(Infos.st_mode)){
            taille += (long)Infos.st_size;
        }
    }

    closedir(dir);

    return taille;

}

int main(){

    long taille = TailleRepCour();

    printf("Nb total d'octets des fich ordinaires dans le rep courant : %ld\n", taille);

    exit(0);

}