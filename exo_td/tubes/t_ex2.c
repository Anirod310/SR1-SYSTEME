#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

/*
    ETAPE 1 : SCHEMA 
---------------------------
clavier -> processus pere -> close(tube[0]) -> caractères -> processus Alpha -> close(tube[1]) -> stdout_fileno
                             close              -> chiffres -> processus 
            .....
*/

int main(){

    int tube_alpha[2];
    if(pipe(tube_alpha) == -1){
        perror("creation du tube");
        exit(1);
    }

    int tube_chiffres









    printf("Pere : Je recois un flot de caractères...\n");
    close(tube[])


}

