#define _POSIX_C_SOURCE 202405L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){

    pid_t pid = fork();

    if(pid==-1){
        perror("fork");
        exit(1);
    }
    if(pid==0){
        execlp("ls", "ls", "-l", NULL);
        perror("ls");
        exit(2);
    }

    wait(NULL);
    execlp("date", "date", NULL);
    perror("date");
    exit(3);
}