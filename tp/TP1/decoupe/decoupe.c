#include "decoupe.h"

#include <stdio.h>

int Decoupe(char Chaine[], char *pMots[])
{
  char *p;
  int NbMots=0;

  p=Chaine;	/* On commence par le début */
  /* Tant que la fin de la chaîne n'est pas atteinte et qu'on ne déborde pas */
  while ((*p)!='\0' && NbMots<NBMOTSMAX)
  {
    while ((*p)==' ' && (*p)!='\0') p++; /* Recherche du début du mot */
    if ((*p)=='\0') break;	/* Fin de chaîne atteinte */
    pMots[NbMots++]=p;		/* Rangement de l'adresse du 1er caractère du mot */
    while ((*p)!=' ' && (*p)!='\0') p++; /* Recherche de la fin du mot */
    if ((*p)=='\0') break;	/* Fin de chaîne atteinte */
    *p='\0';			/* Marquage de la fin du mot */
    p++;			/* Passage au caractère suivant */
  }
  pMots[NbMots]=NULL;		/* Dernière adresse */
  return NbMots;
}
