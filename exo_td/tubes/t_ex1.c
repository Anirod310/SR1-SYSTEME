#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/*
    ETAPE 1 : SCHEMA 
---------------------------
clavier -> processus pere -> close(tube[0]) -> caractères -> processus fils -> close(tube[1]) -> stdout_fileno
*/

int main(){
    char msg[512];
    printf("Entrez votre message : ");
    scanf("%s", msg);

    int tube[2];

    char car;

    if(pipe(tube)==-1){
        perror("echec creation tube\n");
        exit(1);
    }

    int pid = fork();

    if(pid==-1){
        perror("fork\n");
        exit(2);
    }

    if(pid>0){
        printf("Père démarre : \n");
        close(tube[0]);
        write(tube[1], (void *)msg, sizeof(char)*(strlen(msg) + 1));
        close(tube[1]);
        printf("Père se termine\n");
        exit(0);
    }
    close(tube[1]);
    printf("Le fils recoit \n");
    while(read(tube[0], (void *)&car, sizeof(char))){
        printf("%c", car);
    }
    
    close(tube[0]);
    printf("\n");
    printf("Fin du travail du fils \n");
    exit(0);

}