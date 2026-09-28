#define _POSIX_C_SOURCE 202405L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void afficher_info_processus(){
    printf("pid : %d\npid du pere : %d\npid du groupe : %d\nlogin utilisateur : %s\nid proprietaire : %d\nid groupe : %d\n", getpid(), getppid(), getpgrp(), getlogin(), getuid(), getgid());   
}

void afficher_info_processus_v2(){
    pid_t pid = fork();
    if(pid==-1){
        perror("fork");
        exit(1);
    }
    if(pid==0){
        printf("Info processus fils : \n");
        afficher_info_processus();
        int code_retour_fils = 0;
        printf("[Fils] J'ai terminé, mon code de retour : %d\n", code_retour_fils);
        exit(code_retour_fils);
    }
    printf("Info processus père\n");

    afficher_info_processus();

    int circonstance;
    pid_t pid_fils = wait(&circonstance);
        
    printf("[Père] : Je termine, mon fils %d a renvoyé le code %d.\n", pid_fils, WEXITSTATUS(circonstance));
}
int main(int argc, char* argv[]){
    afficher_info_processus_v2();
    exit(0);
}