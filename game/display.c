
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "../preprocessor_macros_constants.h"



int affiche_jeu(void){  //pas encore possible de la tester correctement, il faut attendre d'avoir un sytème de gestion des tours qui fonctionne 
    Tourelle* t=game.tourelles;
    Etudiant* e=game.etudiants;
    Etudiant* f=game.etudiants;
    CLEAR;
    printf("Tour %d\n",game.tour);
    for (int i=1;i<=ROWS;i++){
        for (int j=COLUMNS-1;j<=0;j--){
            while (e!=NULL && e->ligne!=i){
                e=e->next;
            }
            if (t!=NULL && t->ligne==i && t->position==j){
                if (j==1) printf("%d |  %c ",i,t->type);
                else printf(" %c ",t->type);
                t=t->next;
                continue;
            }
            if (e!=NULL && e->ligne==i && e->position==j && e->tour<=game.tour){
                if (j==1) printf("%d |  %2d%c ",i,e->pointsDeVie,e->type);
                printf("%2d%c ",e->pointsDeVie,e->type);
                e=e->next_line;
                continue;
            }
            if (j==1) printf("%d |  . ",i);
            else printf("  . ");
        }
        printf("\n");
        e=f;
    }
    char p[256];
    printf("Entrez P si vous voulez faire pause sinon appuyez sur Entrée : ");
    scanf("%s",&p);
    if (p=="") return 1;
    return 0;
}


void affiche_vague(void){ //affiche la vague avant le début des tours 
    CLEAR;
    Etudiant* e=game.etudiants;
    Etudiant* f=game.etudiants;
    for (int i=1;i<=ROWS;i++){
        int c=1;
        while (f!=NULL){
            if (f->ligne==i){
                if (f->tour==c){
                    printf("%2d%c ",f->pointsDeVie,f->type);
                    f=f->next_line;
                    c++;
                }
                else {
                printf("  . ");
                c++;
                }
            }
            else {
                if (f->tour>c){
                    printf("  . ");
                    c++;
                }
                f=f->next;
            }
        }
        f=e;
        printf("\n");
    }
}

void help(void){
    printf("LISTE DES COMMANDES POSSIBLES :\n   -StatTour : affiche les stats d'une tour (de gauche à droite et de haut en bas)\n   -StatEtu : idem pour les étudiants\n   -end : termine la partie sur une défaite\n   -PlaceTour : Initie le placement d'une tour\n   -next : passe au tour suivant\n");
}


void interInstru(int* gamestate,char instru[256]){ //pas terminé on ajoutera des instructions possibles au fil du temps 
    if (!strcmp(instru,"help")){
        help();
        printf("Que voulez vous faire ? ");
        scanf("%s",&instru);
        interInstru(gamestate,instru);
    }
    if (!strcmp(instru,"StatTour")){
        char c;
        printf("\nDonnez le type de la tourelle");
        scanf("%c",&c);
        //fonction affichage tour j'attends de voir comment tu veux les intégrer au code avant de le faire
        printf("Que voulez vous faire ? ");
        scanf("%s",&instru);
        interInstru(gamestate,instru);
    }
    if (!strcmp(instru,"StatEtu")){
        char c;
        printf("\nDonnez le type de l'étudiant");
        scanf("%c",&c);
        //idem
        printf("Que voulez vous faire ? ");
        scanf("%s",&instru);
        interInstru(gamestate,instru);
    }
    if (!strcmp(instru,"end")){
        gamestate=0;
    }
    if (!strcmp(instru,"PlaceTour")){
        int i,k;
        char t;
        printf("Saisir le type de la tourelle : ");
        scanf("%c",&t);
        printf("Saisir la ligne de la tourelle : ");
        scanf("%d",&i);
        printf("Saisir la colonne de la tourelle : ");
        scanf("%d",&k);
        //fonction pour ajouter une tour à la liste
        printf("Que voulez vous faire ? ");
        scanf("%s",&instru);
        interInstru(gamestate,instru);
    }
    printf("instruction non reconnue");
    printf("Que voulez vous faire ? ");
    scanf("%s",&instru);
    interInstru(gamestate,instru);
}