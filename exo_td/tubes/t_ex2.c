#define _POSIX_C_SOURCE 202405L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <wait.h>
/*
    ETAPE 1 : SCHEMA 
---------------------------
clavier -> processus pere -> close(tube[0]) -> caractères -> processus Alpha -> close(tube[1]) -> stdout_fileno
                             close              -> chiffres -> processus 
            .....
*/

int main(void){

    int tube_alpha[2];
    if(pipe(tube_alpha) == -1){
        perror("creation du tube");
        exit(2);
    }

    int tube_chiffres[2];
    if(pipe(tube_chiffres) == -1){
        perror("creation du tube");
        exit(2);
    }

    int pid_fils_1 = fork();
    if(pid_fils_1==-1){
        perror("fork");
        exit(3);
    }

    if(pid_fils_1==0){//Premier fils
        close(tube_alpha[1]);
        close(tube_chiffres[0]);
        close(tube_chiffres[1]);

        int compteur[26] = {0};
        char c;

        while(read(tube_alpha[0], &c, sizeof(c))>0){
            char min_c = tolower(c);
            compteur[min_c-'a']++;
        }

        close(tube_alpha[0]);
        printf("\n[Fils Alpha] Stat Tube Alpha : \n");
        for(int i=0; i<26;i++){
            if(compteur[i] > 0){
                printf("Charactère %c trouvé %d fois\n", 'a'+ i, compteur[i]);
            }
        }

        exit(0);
    }

    int pid_fils_2 = fork();
    if(pid_fils_2==-1){
        perror("fork");
        exit(3);
    }

    if(pid_fils_2==0){//Second fils
        close(tube_chiffres[1]);
        close(tube_alpha[0]);
        close(tube_alpha[1]);

        int somme = 0;
        char c;

        while(read(tube_chiffres[0], &c, sizeof(c))>0){
            somme += (c-'0');
        }

        close(tube_chiffres[0]);

        printf("[Fils Chiffre] La somme totale est %d\n", somme);
    
        exit(0);
    }

    close(tube_alpha[0]);
    close(tube_chiffres[0]);

    char c;

    while(read(STDIN_FILENO, &c, sizeof(c))>0){
        if(isdigit(c)){
            write(tube_chiffres[1], &c, sizeof(c));
        }
        if(isalpha(c)){
            write(tube_alpha[1], &c, sizeof(c));
        }
    }

    close(tube_alpha[1]);
    close(tube_chiffres[1]);

    wait(NULL);
    wait(NULL);

    exit(0);

}  

