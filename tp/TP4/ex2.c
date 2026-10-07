#define _POSIX_C_SOURCE 202405L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define N 10
#define MAX_BASSIN 5

int main(void){
   


    int Remplir[2];
    int Etat[2];
    if(pipe(Remplir)==-1 || pipe(Etat)==-1){
        perror("Echec création tube");
        exit(4);
    }

    int pid = fork();
    if(pid==-1){
        perror("Echec création fork");
        exit(2);
    }

    if(pid==0){
        close(Etat[0]);
        close(Remplir[1]);

        int bassin = 0;
        int var;

        while((read(Remplir[0], &var, sizeof(var)))>0){
            if(var>=0){
                bassin += var;

                if(bassin>MAX_BASSIN){
                    printf("[Fils] Debordement (%d) ! Alerte\n", bassin);
                    if((write(Etat[1], &bassin, sizeof(bassin)))==-1){
                        perror("Erreur d'écriture");

                        close(Remplir[0]);
                        close(Etat[1]);

                        exit(3);
                    }
                }
                else{
                    printf("[Fils] Remplissage (+%d) -> Bassin = %d\n", var, bassin);
                }
            }
            else{
                bassin = 0;
                printf("[Fils] Vidange effectuée -> Bassin = 0\n");
            }
        }

        close(Etat[1]);
        close(Remplir[0]);

        exit(1);
    }

    if(pid>0){
        close(Etat[1]);
        close(Remplir[0]);

        int f_flags; /* mode non bloquant pour le tube Etat dans le Pere */
        f_flags = fcntl(Etat[0], F_GETFL); /* Recuperation des flags */
        f_flags |= O_NONBLOCK; /* Positionnement du flag de non blocage */
        fcntl(Etat[0], F_SETFL, f_flags); /* Mis a jour des flags */

        int un = 1;
        int moins_un = -1;

        for(int i=0; i<N; i++){
            if((write(Remplir[1], &un, sizeof(un)))==-1){
                perror("Erreur d'écriture");

                close(Remplir[1]);
                close(Etat[0]);

                exit(3);
            }

            sleep(1);

            int nb;
            if((read(Etat[0], &nb, sizeof(nb)))>0){
                if((write(Remplir[1], &moins_un, sizeof(moins_un)))==-1){
                    perror("Erreur d'écriture");

                    close(Remplir[1]);
                    close(Etat[0]);

                    exit(3);
                }
            }
        }

        close(Remplir[1]);
        close(Etat[0]);

        int status;
        wait(&status);

        printf("[Père] Mon fils %d est terminé avec le code %d\n", pid, WEXITSTATUS(status));
    }


    exit(0);
}

