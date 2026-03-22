#include "personne.h"
#include "noeud.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct pays{
    char nom[25];
    int ND;
    int NR;
}pays; 

void genererChaine(char* ch,int longeur){
    const char* alphabet="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for(int i=0;i<longeur-1;i++){
        ch[i]=alphabet[rand() % 26];
    }
    ch[longeur-1]='\0';
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

int lire(char *F,pays p[],int *cmpt){
    FILE *f=fopen(F,"r");
    if(f==NULL){
        perror("Erreur lors de l'ouverture du fichier.\n");
        return -1;
    }
    while(fscanf(f,"%s %d %d",p[*cmpt].nom,&p[*cmpt].ND,&p[*cmpt].NR)==3){
        (*cmpt)++;
        if(*cmpt>=100){
            printf("Attention : Le tableau de pays est plein (100 pays maximum).\n");
            break;
        }
    }

    fclose(f);
    return 0;
}

void calculerStats(noeud* n,int minAge,int maxAge,char resultat,int* total,int* cmpt){
    if(n){
        if(n->age>=minAge && n->age<=maxAge){
            *total += n->occurrence;
            if(n->resultat==resultat){
                *cmpt+=n->occurrence;
            }
        }
        calculerStats(n->FG,minAge,maxAge,resultat,total,cmpt);
        calculerStats(n->FD,minAge,maxAge,resultat,total,cmpt);
    }
}

double calculerPCT(noeud* racine,int minAge,int maxAge,char resultat){
    int cmpt=0;
    int total=0;
    calculerStats(racine,minAge,maxAge,resultat,&total,&cmpt);
    if(total==0) return 0.0;
    return (cmpt/(double)total)*100.0;
}

void interface(double pourcentageD,double pourcentageR,int minAge,int maxAge){
    printf("\n");
    printf("===========================================\n");
    printf("Intervalle d'age : [%d-%d]\n",minAge,maxAge);
    printf("-------------------------------------------\n");
    printf("Pourcentage des 'D' : %.2f %%\n",pourcentageD);
    printf("Pourcentage des 'R' : %.2f %%\n",pourcentageR);
    printf("===========================================\n");
    printf("\n");
}

void ecrireArbre(noeud* racine,FILE* f){
    if(racine==NULL){
        return;
    }
    fprintf(f,"Age: %d, Occurrence: %d, Resultat: %c\n",racine->age,racine->occurrence,racine->resultat);
    ecrireArbre(racine->FG,f);
    ecrireArbre(racine->FD,f);
}
void sauvegarderArbre(noeud* racine,char* F){
    FILE* f=fopen(F,"w"); 
    if (f==NULL){
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }
    ecrireArbre(racine,f);
    fclose(f);
    printf("Arbre sauvegarde dans le fichier %s.\n", F);
}

int main(){
    srand(time(NULL));

    pays Pays[100];
    int cmpt = 0;
    if (lire("stats.txt",Pays,&cmpt) == -1){
        return 1;
    }

    printf("\nDonnees des pays lues depuis le fichier :\n");
    for (int i=0;i<cmpt;i++){
        printf("Pays: %s, ND: %d, NR: %d\n",Pays[i].nom,Pays[i].ND,Pays[i].NR);
    }

    int n;
    printf("\nEntrez le nombre de pays que vous voulez traiter : ");
    scanf("%d",&n);
    personne* T[n];
    for (int i=0;i<n;i++){
        T[i]=NULL;
        int nbPersonnes=rand() % 10 + 1;
        genererDonnees(&T[i],nbPersonnes);
    }

    printf("\nListe des pays avant tri :\n");
    for (int i=0;i<n;i++){
        printf("\nPays %d :\n",i+1);
        afficherListe(T[i]);
    }

    printf("\nTri par tranche d'age et nom :\n");
    for (int i=0;i<n;i++){
        T[i]=trierParAgeEtNom(T[i]);
        printf("\nPays %d apres tri :\n",i+1);
        afficherListe(T[i]);
    }

    noeud* A=tabEnArbre(T,n);
    printf("\nABR :\n");
    afficherArbre(A);

    printf("\nPourcentage de 'D' tq l'age est dans l'intervalle [50,60] : %.2f%%\n",calculerPCT(A,50,60,'D'));
    printf("Pourcentage de 'D' pour l'age > 70 : %.2f%%\n",calculerPCT(A,71,100,'D'));
    printf("Pourcentage de 'R' pour l'age < 30 : %.2f%%\n",calculerPCT(A,1,29,'R'));

    interface(calculerPCT(A,50,60,'D'),calculerPCT(A,50,60,'R'),50,60);
    interface(calculerPCT(A,71,100,'D'),calculerPCT(A,71,100,'R'),71,100);
    interface(calculerPCT(A,1,30,'D'),calculerPCT(A,1,30,'R'),1,30);

    sauvegarderArbre(A,"arbre.txt");

    libererArbre(A);
    for(int i=0;i<n;i++){
        libererListe(T[i]);
    }

    return 0;
}