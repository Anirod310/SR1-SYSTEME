#define _POSIX_C_SOURCE 202405L
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

// ------ Ex 3.4 : Fonction dédiée ------
int lire_nieme_int(int fd, int n) {
    if (lseek(fd, n * sizeof(int), SEEK_SET) == -1) {
        perror("lseek ex3.4");
        return -1;
    }

    int nb;
    if (read(fd, &nb, sizeof(int)) != sizeof(int)) {
        fprintf(stderr, "Erreur : impossible de lire l'entier à l'index %d (hors limites ou EOF)\n", n);
        return -1;
    }

    return nb;
}

/*
---------POUR FAIRE L'EXO--------
*/

int main(int argc, char* argv[]){
    if(argc != 1){
        fprintf(stderr,"Usage : %s\n", argv[0]);
        exit(1);
    }

    int fd = open("test_ex3.4.bin", O_RDWR | O_CREAT | O_TRUNC, 0644);
    int nombres[] = {10, 20, 30, 40, 50, 60};
    write(fd, nombres, sizeof(nombres));

    /*
    -----------------------------
    --------EXO------------------
    */

    //------Ex 3.2-------
    off_t position;
    if((position=lseek(fd, 0, SEEK_CUR))==-1){
        perror("test_ex3.4.bin");
        close(fd);
        exit(3);
    }

    printf("Position courante dans le fichier : %ld\n", (long)position);

    if((position=lseek(fd, position, SEEK_SET))==-1){
    perror("test_ex3.4.bin");
    close(fd);
    exit(3);
    }
    //------Ex 3.3-------
    off_t taille;
    if((taille=lseek(fd, 0, SEEK_END))==-1){
    perror("test_ex3.4.bin");
    close(fd);
    exit(3);
    }

    printf("Taille en octets de test_ex3.4.bin : %ld\n", (long)taille);
    //-------Ex 3.4-------

    int n = 4;
    int resultat = lire_nieme_int(fd, n);
    if (resultat != -1) {
        printf("Entier à l'index %d : %d\n", n, resultat);
    }
    close(fd);

    exit(0);

}