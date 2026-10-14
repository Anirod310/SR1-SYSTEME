#define _POSIX_C_SOURCE 202405L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>

#define N 3

void parcourir_fichiers(char* rep, int* tube){

    DIR* dir = opendir(rep);
    if(dir==NULL){
        perror(rep);
        return;
    }

    struct stat Infos;
    struct dirent* Elem;
    char chemin[1024];

    while((Elem = readdir(dir))!=NULL){
        if (strcmp(Elem->d_name, ".") != 0 && strcmp(Elem->d_name, "..") != 0){
            snprintf(chemin, sizeof(chemin), "%s/%s", rep, Elem->d_name);
            if((lstat(chemin, &Infos))==-1){
                perror(chemin);
            }
            if(S_ISREG(Infos.st_mode)){
                if((write(tube[1], chemin, sizeof(chemin)))==-1){
                    perror("Erreur d'écriture");
                }
            }
            if(S_ISDIR(Infos.st_mode)){
                parcourir_fichiers(chemin, tube);
            }
        }
    }

    closedir(dir);
}


int main(int argc, char* argv[]){
    if(argc != 3){
        fprintf(stderr,"Usage : %s magic_number designation_rep\n", argv[0]);
        exit(1);
    }

    int pere_fils_fich[2];
    if(pipe(pere_fils_fich)==-1){
        perror("Erreur creation tube");
        exit(2);
    }

    int fils_fich_fils_rep[2];
    if(pipe(fils_fich_fils_rep)==-1){
        perror("Erreur creation tube");
        exit(2);
    }

    for(int i=0; i<N; i++){//Fils traitant les fichiers
        int pid = fork();
        if(pid==-1){
            perror("Erreur création fork");
            exit(3);
        }

        if(pid==0){
            close(pere_fils_fich[1]);
            close(fils_fich_fils_rep[0]);
            
            char buffer_chemin[1024];

            while(read(pere_fils_fich[0], buffer_chemin[1024], sizeof(buffer_chemin))>0){
                int fd = open(buffer_chemin, O_RDONLY);
                if (fd == -1) {
                    perror(buffer_chemin);
                }

                ssize_t taille_magic = strlen(argv[1]);
                char buffer[taille_magic+1];
                ssize_t octets_lus = read(fd, buffer, sizeof(buffer));

                if(octets_lus == taille_magic){
                    buffer[octets_lus] == '\0';
                    if (strncmp(buffer, argv[1], taille_magic) == 0){
                        struct stat Infos;
                        if((lstat(buffer_chemin, &Infos))==-1){
                            perror(buffer_chemin);
                            //....
                        }
                    }
                }

            }

            exit(0);
        }

    }

    int pid = fork();
    if(pid==-1){
        perror("Erreur création fork");
        exit(3);
    }

    if(pid==0){//Fils traitant les réponses recues de ses freres 
        close(pere_fils_fich[0]);
        close(pere_fils_fich[1]);
        close(fils_fich_fils_rep[1]);
        //...
        exit(0);
    }

    close(pere_fils_fich[0]);
    close(fils_fich_fils_rep[0]);
    close(fils_fich_fils_rep[1]);

    parcourir_fichiers(argv[2], pere_fils_fich);

    close(pere_fils_fich[1]);

    while(wait(NULL)>0);

    exit(0);
    
}