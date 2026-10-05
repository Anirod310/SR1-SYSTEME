#define _POSIX_C_SOURCE 202405L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <wait.h>

int main(int argc, char* argv[]){
    if(argc!=3){
        fprintf(stderr,"Usage : %s HIERARCHIE UTILISATEUR\n", argv[0]);
        exit(1);
    }

    printf("ls -Ral %s | grep %s | wc -l\n", argv[1], argv[2]); // Voir ce qu'on fait

    int tubeLsGrep[2];
        if(pipe(tubeLsGrep) == -1){
            perror("creation du tube");
            exit(2);
    }

    int tubeGrepWc[2];
        if(pipe(tubeGrepWc) == -1){
            perror("creation du tube");
            exit(2);
    }

    int pid_fils_1 = fork();
    if(pid_fils_1==-1){
        perror("fork");
        exit(3);
    }

    if(pid_fils_1==0){
        close(tubeLsGrep[0]);
        close(tubeGrepWc[0]);
        close(tubeGrepWc[1]);

        if(dup2(tubeLsGrep[1], STDOUT_FILENO)==-1){
            perror("dup2");
            close(tubeLsGrep[1]);
            exit(4);
        }

        execlp("ls", "ls", "-Ral", argv[1], NULL);
        perror("ls");
        close(tubeLsGrep[1]);
        exit(4);
    }

    int pid_fils_2 = fork();
    if(pid_fils_2==-1){
        perror("fork");
        exit(3);
    }

    if(pid_fils_2==0){
        close(tubeGrepWc[0]);
        close(tubeLsGrep[1]);


        if(dup2(tubeLsGrep[0], STDIN_FILENO)==-1){
            perror("dup2");
            close(tubeLsGrep[0]);
            close(tubeGrepWc[1]);
            exit(4);
        }

        
        if(dup2(tubeGrepWc[1], STDOUT_FILENO)==-1){
            perror("dup2");
            close(tubeLsGrep[0]);
            close(tubeGrepWc[1]);
            exit(4);
        }

        execlp("grep", "grep", argv[2], NULL);
        perror("grep");
        close(tubeLsGrep[0]);
        close(tubeGrepWc[1]);
        exit(4);
    }

    close(tubeGrepWc[1]);
    close(tubeLsGrep[0]);
    close(tubeLsGrep[1]);

    if(dup2(tubeGrepWc[0], STDIN_FILENO)==-1){
        perror("dup2");
        close(tubeGrepWc[0]);
        exit(4);
    }

    execlp("wc", "wc", "-l", NULL);
    perror("wc");
    close(tubeGrepWc[0]);
    exit(4);



}