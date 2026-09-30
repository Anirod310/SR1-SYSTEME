#define _POSIX_C_SOURCE 202405L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char* argv[]){
    if(argc < 2){
        fprintf(stderr, "Usage sans redirection : %s fichier1 \nUsage avec redirection : %s fichier 1 \" > \" fich_destination\n", argv[0], argv[0]);
        exit(1);
    }

    int redirection = 0;
    int position_redirection;
    int fin_boucle = argc;

    for(int i=0; i<argc; i++){
        if(strcmp(argv[i], ">") == 0){
            if((i+1)<argc){
                redirection = 1;
                position_redirection = i+1;
                fin_boucle = i;
            }else{
                fprintf(stderr, "Il faut préciser le fichier de redirection ! : \" > \" fichier \n");
                exit(2);
            }
        }
    }

    if(redirection == 1){
        int d = open(argv[position_redirection],  O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
        if(d == -1){
            perror(argv[position_redirection]);
            exit(3);
        }

        dup2(d, STDOUT_FILENO);
        close(d);
    }

    for(int j=1; j<fin_boucle; j++){
        pid_t pid=fork();
        if(pid==-1){
            perror("erreur fork");
            exit(4);                
        }
        if(pid == 0){
            int fd = open(argv[j], O_RDONLY);
            if(fd==-1){
                perror(argv[j]);
                exit(5);
            }

            char buffer[512];
            ssize_t octets_lus;

            while((octets_lus = read(fd, buffer, sizeof(buffer))) > 0){
                write(STDOUT_FILENO, buffer, octets_lus);
            }

            close(fd);
            exit(EXIT_SUCCESS);
        }
        else{
            wait(NULL);
        }
    }


exit(0);
}