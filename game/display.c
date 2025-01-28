
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "../preprocessor_macros_constants.h"
#include "entity_types/entity_type_vector.h"
extern Entity_type_vector tourelle_types;
extern Entity_type_vector etudiant_types;

int affiche_jeu(void){
    char L[ROWS+1][COLUMNS+5][5];
    Tourelle* t=game.tourelles;
    Etudiant* e=game.etudiants;
    CLEAR
    printf("Tour %d\n",game.tour);
    for (int i=0; i<ROWS;i++){
        for (int j=0; j<COLUMNS;j++){
            if (j==0){
                sprintf(L[i][0],"%d|  ",i+1);
                L[i][1][0]=' ';
                L[i][1][1]=' ';
                L[i][1][2]='.';
                L[i][1][3]=' ';
                L[i][1][4]='\0';
            }
            else{
                L[i][j+1][0]=' ';
                L[i][j+1][1]=' ';
                L[i][j+1][2]='.';
                L[i][j+1][3]=' ';
                L[i][j+1][4]='\0';
            }
        }
    }
    while (e!=NULL){
        if (e->tour<=game.tour){
        sprintf(L[e->ligne-1][COLUMNS - e->position]," %2d%c ",e->pointsDeVie,entity_type_get_type_by_id(&etudiant_types, e->type)->type.e_type.abbr);
        }
        e=e->next;
    }
    while (t!=NULL){
        sprintf(L[t->ligne-1][COLUMNS - t->position],"  %c ",entity_type_get_type_by_id(&tourelle_types, t->type)->type.t_type.abbr);
        t=t->next;
    }
    for (int k=0;k<=ROWS-2;k++){
        for (int l=0;l<=COLUMNS-1;l++){
            printf(L[k][l]);
        }
        printf("\n");
    }
    printf("\n");
    char p[256];
    printf("Entrez P si vous voulez faire pause sinon appuyez sur Entree : ");
    scanf("%s",p);
    if ( ! strcmp(p, "P") ) return 1;
    return 0;
}


void affiche_vague(void){ //affiche la vague avant le début des tours 
    CLEAR
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


int interInstru(void){ //pas terminé on ajoutera des instructions possibles au fil du temps 
    char instru[256];
    printf("Que voulez vous faire ? ");
    scanf("%s", instru);
    if (!strcmp(instru,"help")){
        help();
    }
    else{
        if (!strcmp(instru,"StatTour")){
            char c;
            printf("\nDonnez le type de la tourelle");
            scanf("%c",&c);
            //fonction affichage tour j'attends de voir comment tu veux les intégrer au code avant de le faire
        }
        else{
            if (!strcmp(instru,"StatEtu")){
                char c;
                printf("\nDonnez le type de l'étudiant");
                scanf("%c",&c);
                //idem
            }
            else{
                if (!strcmp(instru,"end")){
                    game.finished=1;
                    return 0;
                }
                else{
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
                    }
                    else{
                        if (!strcmp(instru,"continue")){
                            return 1;
                        }
                        else{
                            printf("Instruction non reconnue.\n");
                        }
                    }
                }
            }
        }
    }
    interInstru();
}
