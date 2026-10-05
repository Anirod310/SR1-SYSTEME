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

int main(int argc, char* argv[]){
    if(argc!=2){
        fprintf(stderr,"Usage : %s designation_fich\n", argv[0]);
        exit(1);
    }

    struct stat Infos;

    if(lstat(argv[1], &Infos)==-1){
        perror(argv[1]);
        exit(2);
    }

    printf("Fichier %s : \ntaille : %ld octets\ndate derniere modifs données: %s", argv[1], (long)Infos.st_size, ctime(&Infos.st_mtime));

    exit(0);
}  