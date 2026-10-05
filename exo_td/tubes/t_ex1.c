#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/*
    ETAPE 1 : SCHEMA 
---------------------------
clavier -> processus pere -> close(tube[0]) -> caractères -> processus fils -> close(tube[1]) -> stdout_fileno
*/

int main(void){
    int tube[2];
    if(pipe(tube)==-1){
        perror("echec creation tube\n");
        exit(1);
    }

    int pid = fork();
    if(pid==-1){
        perror("fork\n");
        exit(2);
    }

    if(pid==0){
        printf("Fils :\n");
        close(tube[1]);
        
        char c;

        while(read(tube[0], &c, sizeof(c))>0){
            write(STDOUT_FILENO, &c, sizeof(c));
        }

        close(tube[0]);
        printf("Fils fini\n");

        exit(0);
    }

    printf("Pere :\n");
    close(tube[0]);

    char c;

    while((read(STDIN_FILENO, &c, sizeof(c))>0)&&c!='f'){
        write(tube[1], &c, sizeof(c));
    }

    close(tube[1]);
    printf("Pere fini\n");

    exit(0);
}