#define _POSIX_C_SOURCE 202405L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "est_premier.c"

#define N 4

int main(int argc, char* argv[]){
    if(argc < 2){
        fprintf(stderr,"Usage : %s entier1 [entiers2] [...]", argv[0]);
        exit(1);
    }

    int tube[2];
    if(pipe(tube)==-1){
        perror("Erreur creation tube\n");
        exit(2);
    }

    for(int j = 0; j<N; j++){
        int pid = fork();
        if(pid==-1){
            perror("echec creation fork");
            exit(3);
        }

        if(pid==0){
            close(tube[1]);
            unsigned long long int entier;
            int nb_prems=0;

            while(read(tube[0], &entier, sizeof(entier))>0){
                if(est_premier(entier)){
                    printf("[Fils %d] %llu est premier\n", getpid(), entier);
                    nb_prems ++;
                }
                else{
                    printf("[Fils %d] %llu\n", getpid(), entier);
                }
            }

            close(tube[0]);
            exit(nb_prems);

        }
    }

    close(tube[0]);

    for(int i=1; i<argc; i++){
        char* fin;
        unsigned long long int val = strtoull(argv[i], &fin, 10);
        
        if(*fin != '\0' || fin == argv[i]) {
            fprintf(stderr, "L'argument '%s' n'est pas un entier valide.\n", argv[i]);
        }else{
            write(tube[1], &val, sizeof(val));
        }

    }

    close(tube[1]);

    int total_prems = 0;
    int status;

    for(int k = 0; k < N; k++) {
        wait(&status);
        total_prems += WEXITSTATUS(status);
    }

    printf("Il y a %d nombres premiers\n", total_prems);
    
    exit(0);
}