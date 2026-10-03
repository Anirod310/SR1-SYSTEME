#define _POSIX_C_SOURCE 202405L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 4

int main(void){
    int tube[2];
 
    if(pipe(tube)==-1){
        perror("echec creation tube");
        exit(2);
    }

    int pid = fork();

    if(pid==-1){
        perror("echec creation fork");
        exit(3);
    }

    if(pid>0){

        close(tube[0]);

        for(int i=1; i<=N; i++ ){
            if((write(tube[1], &i, sizeof(i))) == -1){
                perror("Erreur ecriture");
                exit(4);
            }
            sleep(1);   
        }

        close(tube[1]);

        int status;

        wait(&status);

        printf("[Père] : Mon fils %d est terminé avec le code %d\n", pid, WEXITSTATUS(status));

        exit(0);
    }

    if(pid==0){

        close(tube[1]);
        
        int val;

        while(read(tube[0], &val, sizeof(val))>0){
            printf("[Fils] %d\n", val);
        }

        exit(1);
    }

}

