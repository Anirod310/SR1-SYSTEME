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

typedef struct{
    char chemin[1024];
    ino_t inode;
    off_t taille;

} ReponseFichier;

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
                continue;
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
    if(argc != 3 && argc != 5){
        fprintf(stderr,"Usage : %s [-v cmd] magic_number designation_rep\n", argv[0]);
        exit(1);
    }

    char magic_number[1024];
    char designation[1024];

    if(argc == 3){
        snprintf(magic_number, sizeof(magic_number), "%s", argv[1]);
        snprintf(designation, sizeof(designation), "%s", argv[2]);
    }else
    {
        snprintf(magic_number, sizeof(magic_number), "%s", argv[3]);
        snprintf(designation, sizeof(designation), "%s", argv[4]);
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
            
            char chemin[1024];//désignation du fichier

            while(read(pere_fils_fich[0], chemin, sizeof(chemin))>0){
                int fd = open(chemin, O_RDONLY);
                if (fd == -1) {
                    perror(chemin);
                    continue;
                }
                
                ssize_t taille_magic = strlen(magic_number);
                char buffer[taille_magic+1];
                ssize_t octets_lus = read(fd, buffer, taille_magic);//Pas sizeof(buffer) parce que buffer vaut taille_magic+1

                if(octets_lus == taille_magic){
                    if (memcmp(buffer, magic_number, taille_magic) == 0){
                        struct stat Infos;
                        if((lstat(chemin, &Infos))==-1){
                            perror(chemin);
                        }
                        else{
                            ReponseFichier rep;
                            snprintf(rep.chemin, sizeof(rep.chemin), "%s", chemin);
                            rep.inode = Infos.st_ino;
                            rep.taille = Infos.st_size;

                            write(fils_fich_fils_rep[1], &rep, sizeof(ReponseFichier));
                        }
                    }
                }

                close(fd);
            }
            
            close(pere_fils_fich[0]);
            close(fils_fich_fils_rep[1]);
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
        
        ReponseFichier rep;

        while(read(fils_fich_fils_rep[0], &rep, sizeof(ReponseFichier))>0){
            printf("Fichier trouvé : %s | Inode : %lu | Taille : %ld octets\n", rep.chemin, (unsigned long)rep.inode, (long)rep.taille);
            
            if(argc==5){
                int pid = fork();
                if(pid == -1){
                    perror("Erreur Fork");
                    close(fils_fich_fils_rep[0]);
                    exit(3);
                }
                if(pid==0){//Fils visuRéponse
                    close(fils_fich_fils_rep[0]);
                    execlp(argv[2], argv[2], rep.chemin, NULL);
                    perror(argv[2]);
                    close(fils_fich_fils_rep[0]);
                    exit(4);
                }
                else{
                    wait(NULL);
                }
            }
        }


        close(fils_fich_fils_rep[0]);
        exit(0);
    }

    close(pere_fils_fich[0]);
    close(fils_fich_fils_rep[0]);
    close(fils_fich_fils_rep[1]);

    parcourir_fichiers(designation, pere_fils_fich);

    close(pere_fils_fich[1]);

    while(wait(NULL)>0);

    exit(0);
    
}