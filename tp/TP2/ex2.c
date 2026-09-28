#define _POSIX_C_SOURCE 202405L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

void creer_fich_binaire(char* nom_fich, int n, int entier){
    int fd = open(nom_fich, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if(fd==-1){
        perror(nom_fich);
        exit(1);
    }

    if(lseek(fd, n * sizeof(int), SEEK_SET)==-1){
        perror(nom_fich);
        close(fd);
        exit(2);
    }

    if(write(fd, &entier, sizeof(entier))!=sizeof(int)){
        perror("Erreur écriture");
        close(fd);
        exit(3);
    }

    close(fd);
}

int lire_nieme(char* nom_fich, int n){
    int fd = open(nom_fich, O_RDONLY);
    if(fd==-1){
        perror(nom_fich);
        exit(1);
    }

    if(lseek(fd, n*sizeof(int), SEEK_SET)==-1){
        perror(nom_fich);
        close(fd);
        exit(2);
    }

    int valeur;
    if(read(fd, &valeur, sizeof(int))!=sizeof(int)){
        perror("Erreur lecture");
        close(fd);
        exit(4);
    }

    close(fd);

    return valeur;
}

int main(int argc, char* argv[]){

    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Usage lecture  : %s n fichier\n", argv[0]);
        fprintf(stderr, "Usage écriture : %s n entier fichier\n", argv[0]);
        exit(5);
    }

    char* endptr;
    errno = 0;
    long val_n = strtol(argv[1], &endptr, 10);
    
    if(errno==ERANGE || endptr == argv[1] || *endptr != 0 || val_n < 0){
        fprintf(stderr, "Erreur, la valeur %s est invalide (entier >= 0 attendu)\n", argv[1]);
        exit(6);
    }

    int n = (int)val_n;

    if (argc == 3) {
        int valeur = lire_nieme(argv[2], n);
        printf("L'entier à la position %d est : %d\n", n, valeur);
    } 
    else if (argc == 4) {
        int valeur = atoi(argv[2]);
        creer_fich_binaire(argv[3], n, valeur);
        printf("Entier %d écrit à la position %d dans '%s'.\n", valeur, n, argv[3]);
    }

    exit(0);
}