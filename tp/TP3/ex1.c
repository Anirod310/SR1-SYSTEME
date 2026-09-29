#define _POSIX_C_SOURCE 202405L
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>

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
        printf("tube nommé ");
    } else if (S_ISSOCK(Infos->st_mode)) {
        printf("socket ");
    } else {
        printf("inconnu ");
    }

    printf("%ld octets ", Infos->st_size);
    printf("%s", ctime(&Infos->st_mtime));

}

int main(int argc, char* argv[]){
    if(argc != 2){
        fprintf(stderr,"Usage : %s nom_fichier\n", argv[0]);
        exit(1);
    }

    struct stat Infos;

    if(lstat(argv[1], &Infos)==-1){
    perror(argv[1]);
    exit(2);
    }

    printf("%s ",argv[1]);
    Affiche_inode(&Infos);

    exit(0);
}