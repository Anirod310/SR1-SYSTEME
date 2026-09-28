#ifndef DECOUPE_H
#define DECOUPE_H

#define NBMOTSMAX 20

/* Construction d'un tableau de pointeurs vers le début des mots d'une chaîne
 * de caractères en vue de l'utilisation de la fonction execvp.
 * Retourne le nombre de mots trouvés.
 */
int Decoupe(char Chaine[], char *pMots[]);

#endif
