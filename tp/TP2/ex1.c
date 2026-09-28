#define _POSIX_C_SOURCE 202405L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

void ecrire_dans_stdout(char nom_fich[]){

    char buffer[512];
    ssize_t nb_lus;

    int fd = open(nom_fich, O_RDONLY);

    if(fd==-1){
        perror(nom_fich);
        exit(2);
    }

    while((nb_lus=read(fd, buffer, sizeof(buffer)))>0){
       ssize_t nb_ecrits = write(STDERR_FILENO, buffer, nb_lus);

       if(nb_ecrits != nb_lus){
        perror("Erreur d'écriture sur stdout");
        close(fd);
        exit(3);
       }
    }

    if(nb_lus == -1){
        perror("Erreur de lecture");
        close(fd);
        exit(4);
    }

    if(close(fd) == -1){
        perror("Erreur de fermeture");
        exit(5);
    }
}

int main(int argc, char* argv[]){
    if(argc != 2){
        fprintf(stderr,"Usage : %s nom_fich\n", argv[0]);
        exit(1);
    }

    ecrire_dans_stdout(argv[1]);

    exit(0);
}