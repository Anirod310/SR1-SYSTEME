#define _POSIX_C_SOURCE 202405L
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>


void parcourir_fichiers_vides(const char* rep, int *compteur){
    
    DIR* dir = opendir(rep);
    if(dir==NULL){
        perror(rep);
        return;
    }

    struct dirent* Element;
    struct stat Infos;
    char chemin[512];

    while((Element=readdir(dir))!=NULL){
        if (strcmp(Element->d_name, ".") == 0 || strcmp(Element->d_name, "..") == 0) {
            continue;
        }

        snprintf(chemin, sizeof(chemin), "%s/%s", rep, Element->d_name);

        if (lstat(chemin, &Infos) == -1) {
            perror(chemin);
            return;
        }

        if(S_ISREG(Infos.st_mode)){
            if(Infos.st_size == 0){
                printf("%s\n", chemin);
                (*compteur)++;
            }
        }
        else if(S_ISDIR(Infos.st_mode)){
            parcourir_fichiers_vides(chemin, compteur);
        }

    }

    closedir(dir);
}


int main(int argc, char* argv[]){
    if(argc!=2){
        fprintf(stderr,"Usage : %s repertoire\n", argv[0]);
        exit(1);
    }

    int nb_fichiers_vides = 0;

    parcourir_fichiers_vides(argv[1], &nb_fichiers_vides);

    printf("%d fichier(s) vides trouvé(s) ! \n", nb_fichiers_vides);

    exit(0);

}