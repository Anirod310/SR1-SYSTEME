#define _POSIX_C_SOURCE 202405L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "decoupe/decoupe.c"

int main(int argc, char* argv[]){
    if(argc<2){
        printf("Usage : %s commande_1 commande_2 commande_x", argv[0]);
        exit(1);
    }

    for(int i=1; i<argc; i++){
        pid_t pid = fork();
        if(pid==-1){
            perror("fork");
            exit(2);
        }
        if(pid==0){
            printf("[%d] je lance %s : \n", getpid(), argv[i]);
            char *pMots[NBMOTSMAX+1];
            
            Decoupe(argv[i], pMots);
            execvp(pMots[0], pMots);

            perror(argv[i]);
            exit(3);
        }
        /*else{
            printf("[%d] J'ai délégué %s à %d. J'attend sa fin...\n", getpid(), argv[i], pid);
            wait(NULL);
            printf("[%d] %d terminé\n", getpid(), pid);

        }*/
       else{
        printf("[%d] J'ai délégué %s à %d.\n", getpid(), argv[i], pid);
       }
    }

    for(int i=1; i<argc; i++){
        pid_t fils_termine = wait(NULL);
        if (fils_termine != -1) {
            printf("[%d] %d terminé.\n", getpid(), fils_termine);
        }
    }

    printf("[%d] J'ai fini\n", getpid());

}