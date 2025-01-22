
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "../preprocessor_macros_constants.h"



void affiche_jeu(void){  //ancienne version, fonctionne pas pour l'instant 
    Tourelle* t=game.tourelles;
    Etudiant* e=game.etudiants;
    CLEAR;
    printf("Tour %d\n",game.tour);
    for (int i=1;i<=ROWS;i++){
        for (int j=COLUMNS-1;j<=0;j--){
            if (t!=NULL && t->ligne==i && t->position==j){
                if (j==1) printf("%d |  %c ",i,t->type);
                else printf(" %c ",t->type);
                t=t->next;
                continue;
            }
            if (e!=NULL && e->ligne==i && e->position==j && e->tour<=game.tour){
                if (j==1) printf("%d |  %2d%c ",i,e->pointsDeVie,e->type);
                printf("%2d%c ",e->pointsDeVie,e->type);
                e=e->next;
                continue;
            }
            if (j==1) printf("%d |  . ",i);
            else printf("  . ");
        }
        printf("\n");
    }
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