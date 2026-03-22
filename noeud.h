#ifndef NOEUD_H
#define NOEUD_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct noeud{
    int age;
    int occurrence;
    char resultat;
    struct noeud* FG;
    struct noeud* FD;
}noeud;

noeud* creerNoeud(int age,char resultat){
    noeud* Noeud=(noeud*) malloc(sizeof(noeud));
    Noeud->age=age;
    Noeud->occurrence=1;
    Noeud->resultat=resultat;
    Noeud->FG=NULL;
    Noeud->FD=NULL;
    return Noeud;
}

noeud* insererNoeud(noeud* racine,int age,char resultat){
    if(!racine){
        return creerNoeud(age,resultat);
    }
    if(age<racine->age || (age==racine->age && resultat<racine->resultat)){
        racine->FG=insererNoeud(racine->FG,age,resultat);
    } else if(age>racine->age || (age==racine->age && resultat>racine->resultat)){
        racine->FD=insererNoeud(racine->FD, age,resultat);
    } else{
        racine->occurrence++;
    }
    return racine;
}

void afficherArbre(noeud* racine){
    if(racine){
        afficherArbre(racine->FG);
        printf("Age: %d, Resultat: %c, Occurrence: %d\n",racine->age,racine->resultat,racine->occurrence);
        afficherArbre(racine->FD);
    }
}

void libererArbre(noeud* racine){
    if(racine){
        libererArbre(racine->FG);
        libererArbre(racine->FD);
        free(racine);
    }
}

noeud* tabEnArbre(personne* T[],int taille){
    noeud* racine=NULL;
    for (int i=0;i<taille;i++) {
        personne* courant = T[i];
        while (courant) {
            racine=insererNoeud(racine,courant->age,courant->resultat);
            courant=courant->suiv;
        }
    }
    return racine;
}

#endif