#define _POSIX_C_SOURCE 202405L
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>

void Affiche_inode(struct stat* Infos){

    if (S_ISREG(Infos->st_mode)) {
        printf("fichier ordinaire ");
    } else if (S_ISDIR(Infos->st_mode)) {
        printf("repertoire ");
    } else if (S_ISLNK(Infos->st_mode)) {
        printf("lien symbolique ");
    } else if (S_ISCHR(Infos->st_mode)) {
        printf("periph caractere ");
    } else if (S_ISBLK(Infos->st_mode)) {
        printf("periph bloc ");
    } else if (S_ISFIFO(Infos->st_mode)) {
        printf("tube ");
    } else if (S_ISSOCK(Infos->st_mode)) {
        printf("socket ");
    } else {
        printf("inconnu ");
    }

    struct passwd *pw = getpwuid(Infos->st_uid);
    char* nom_proprio = pw->pw_name;

    printf("%ld octets ", Infos->st_size);
    printf("%s ", nom_proprio);
    printf("%s", ctime(&Infos->st_mtime));

}

int main(int argc, char* argv[]){
    if(argc < 2){
        fprintf(stderr,"Usage : %s nom_fichier1 nom_fichier2\n", argv[0]);
        exit(1);
    }

    struct stat Infos;
    
    if(lstat(argv[1], &Infos)==-1){
    perror(argv[1]);
    exit(3);
    }

    if(argc==2 && S_ISDIR(Infos.st_mode)){
        printf("%s ", argv[1]);
        Affiche_inode(&Infos);
        
        DIR *dir = opendir(argv[1]);
        if((dir) == NULL){
            perror(argv[1]);
            exit(2);
        }

        struct dirent* Element;
        char chemin[512];

        while((Element = readdir(dir)) != NULL){

            if (strcmp(Element->d_name, ".") == 0 || strcmp(Element->d_name, "..") == 0) {
                continue;
            }
            
            snprintf(chemin, sizeof(chemin), "%s/%s", argv[1], Element->d_name);
            if((lstat(chemin, &Infos)) ==-1){
                perror(chemin);
                exit(3);
            }

            printf("%s ", chemin);
            Affiche_inode(&Infos);
        }

        closedir(dir);
    }
    else{
        for(int i=1; i<argc; i++){

            if(lstat(argv[i], &Infos)==-1){
            perror(argv[i]);
            exit(3);
            }

            printf("%s ",argv[i]);
            Affiche_inode(&Infos);
        }
    }

    exit(0);
}