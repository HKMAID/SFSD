#ifndef PERSONNE_H
#define PERSONNE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct personne{
    char nom[20];
    char prenom[20];
    int age;
    int tranche;
    char resultat;
    struct personne* suiv;
}personne;

personne* creerPersonne(char* nom,char* prenom,int age,char resultat){
    personne* Personne=(personne*)malloc(sizeof(personne));
    if(Personne==NULL){
        printf("Erreur : Allocation de la memoire echouee.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(Personne->nom,nom);
    strcpy(Personne->prenom,prenom);
    Personne->age=age;
    Personne->tranche=age/10;
    Personne->resultat=resultat;
    Personne->suiv=NULL;
    return Personne;
}

void ajouterPersonne(personne** tete,personne* p){
    p->suiv=*tete;
    *tete=p;
}

void genererDonnees(personne** tete,int n){
    for(int i=0;i<n;i++){
        char nom[20],prenom[20];
        genererChaine(nom,20);
        genererChaine(prenom,20);
        int age=rand() % 100+1;
        char resultat=(rand() % 2 == 0) ?'D':'R';
        personne* p=creerPersonne(nom,prenom,age,resultat);
        ajouterPersonne(tete,p);
    }
}

void trierParNom(personne* debut,personne* fin){
    for(personne* i=debut;i!=fin;i= i->suiv){
        for(personne* j=i->suiv;j!=fin->suiv;j=j->suiv){
            if(strcmp(i->nom,j->nom)>0){
                personne temp=*i;
                *i=*j;
                *j=temp;
                personne* tmp=i->suiv;
                i->suiv=j->suiv;
                j->suiv=tmp;
            }
        }
    }
}

personne* trierParAgeEtNom(personne* tete){
    if(!tete || !tete->suiv){
        return tete; 
    }
    personne* triee=NULL;
    while(tete){
        personne* actuel=tete;
        tete=tete->suiv;
        if(!triee || actuel->tranche<triee->tranche){
            actuel->suiv=triee;
            triee=actuel;
        } else{
            personne* temp=triee;
            while(temp->suiv && temp->suiv->tranche<=actuel->tranche){
                temp=temp->suiv;
            }
            actuel->suiv=temp->suiv;
            temp->suiv=actuel;
        }
    }
    personne* current=triee;
    while(current){
        personne* DebutTranche=current;
        while(current->suiv && current->suiv->tranche==DebutTranche->tranche){
            current=current->suiv;
        }
        personne* FinTranche=current;
        trierParNom(DebutTranche,FinTranche);
        current=FinTranche->suiv;
    }
    return triee;
}

void afficherListe(personne* tete){
    while(tete){
        printf("Nom: %s, Prenom: %s, Age: %d, Resultat: %c\n",tete->nom,tete->prenom,tete->age,tete->resultat);
        tete=tete->suiv;
    }
}

void libererListe(personne* tete){
    while(tete){
        personne* tmp=tete;
        tete=tete->suiv;
        free(tmp);
    }
}

#endif

