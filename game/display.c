
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "../preprocessor_macros_constants.h"
#include "entity_types/entity_type_vector.h"
extern Entity_type_vector tourelle_types;
extern Entity_type_vector etudiant_types;

int affiche_jeu(void){
    char L[ROWS][COLUMNS+5][5];
    Tourelle* t=game.tourelles;
    Etudiant* e=game.etudiants;
    CLEAR
    printf("Tour %d\n",game.tour);
    for (int i=0; i<ROWS;i++){
        for (int j=0; j<COLUMNS+1;j++){
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
        sprintf(L[e->ligne-1][COLUMNS+1 - e->position]," %2d%c",e->pointsDeVie,entity_type_get_type_by_id(&etudiant_types, e->type)->type.e_type.abbr);
        }
        e=e->next;
    }
    while (t!=NULL){
        sprintf(L[t->ligne-1][COLUMNS+1 - t->position],"  %c ",entity_type_get_type_by_id(&tourelle_types, t->type)->type.t_type.abbr);
        t=t->next;
    }
    for (int k=0;k<=ROWS-1;k++){
        for (int l=0;l<=COLUMNS;l++){
            printf(L[k][l]);
        }
        printf("\n");
    }
    printf("\n");
}


void affiche_vague(void){ //affiche la vague avant le début des tours 
    char L[ROWS][COLUMNS+5][5];;
    Etudiant* e=game.etudiants;
    CLEAR
    printf("Vague d'ennemis");
    for (int i=0; i<ROWS;i++){
        for (int j=0; j<COLUMNS+1;j++){
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
        sprintf(L[e->ligne-1][COLUMNS+1 - e->tour]," %2d%c",e->pointsDeVie,entity_type_get_type_by_id(&etudiant_types, e->type)->type.e_type.abbr);
        e=e->next;
    }
    for (int k=0;k<=ROWS-1;k++){
        for (int l=0;l<=COLUMNS;l++){
            printf(L[k][l]);
        }
        printf("\n");
    }
    printf("\n");
}

void help(void){
    printf("LISTE DES COMMANDES POSSIBLES :\n   -StatTour : affiche les stats d'une tour (de gauche à droite et de haut en bas)\n   -StatEtu : idem pour les étudiants\n   -end : termine la partie sur une défaite\n   -PlaceTour : Initie le placement d'une tour\n   -next : passe au tour suivant\n");
}


int interInstru(void){ //pas terminé on ajoutera des instructions possibles au fil du temps 
    char instru[256];
    int len;
    printf("game > ");
    fgets(instru, 256, stdin);
    len = strlen(instru);

    if ( len > 1)
        instru[len - 1] = '\0';


    if (!strcmp(instru,"help")){
        help();
    }
    else if (!strcmp(instru,"StatTour")){
        char c;
        printf("\nDonnez le type de la tourelle");
        scanf("%c",&c);
        //fonction affichage tour j'attends de voir comment tu veux les intégrer au code avant de le faire
    }
    else if (!strcmp(instru,"StatEtu")){
        char c;
        printf("\nDonnez le type de l'étudiant");
        scanf("%c",&c);
        //idem
    }
    else if (!strcmp(instru,"end")){
        game.finished=1;
        return 0;
    }
    else if (!strcmp(instru,"PlaceTour")){
        int line, position, type;
        bool error;
        printf("Saisir le type de la tourelle :\n");
        scanf("%d",&type);
        printf("Saisir la ligne de la tourelle :\n");
        scanf("%d",&line);
        printf("Saisir la colonne de la tourelle :\n");
        scanf("%d",&position);
        Tourelle * new = tourelle_add(type, line, position, &error);
        if ( error )
            printf("Une entitee occupe deja cette position !\n");
        else if (! new)
            fprintf(stderr, "Error: PlaceTour malloc failed in tourelle_add");
        }
    else if (!strcmp(instru,"\n")){
        return 1;
    }
    else{
        printf("Instruction non reconnue.\n");
    }
    interInstru();
}
