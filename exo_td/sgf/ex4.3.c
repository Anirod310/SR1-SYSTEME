#define _POSIX_C_SOURCE 202405L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void affiche_designation_fich_ordinaire(char rep[]){
    DIR *dir = opendir(rep);

    if((dir) == NULL){
        perror(rep);
        return;
    }

    struct dirent *Element;
    struct stat Infos;
    char chemin[512];


    while((Element = readdir(dir)) != NULL){
        if (strcmp(Element->d_name, ".") && strcmp(Element->d_name, "..")) {
            snprintf(chemin, sizeof(chemin), "%s/%s", rep, Element->d_name);
        }

        if(lstat(chemin, &Infos)==-1){
            perror(chemin);
            continue;
        }

        if(S_ISREG(Infos.st_mode)){
            printf("%s\n", chemin);
        }else if(S_ISDIR(Infos.st_mode)){
            affiche_designation_fich_ordinaire(chemin);
        }
    }

    closedir(dir);
}

int main(int argc, char* argv[]){
    if(argc != 2){
        fprintf(stderr, "Usage : %s repertoire\n", argv[0]);
        exit(1);
    }

    affiche_designation_fich_ordinaire(argv[1]);

    exit(0);
}