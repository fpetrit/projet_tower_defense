
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
        sprintf(L[t->ligne-1][COLUMNS+1 - t->position]," %2d%c",t->pointsDeVie,entity_type_get_type_by_id(&tourelle_types, t->type)->type.t_type.abbr);
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
    printf("Vague d'ennemis\n");
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
    printf("\nCOMMANDES :\n"
           "\t- StatTour    affiche les stats d'une tour <ligne> <position>\n"
           "\t- StatEtu     idem pour les étudiants\n"
           "\t- end         termine la partie sur une defaite\n"
           "\t- PlaceTour   initie le placement d'une tour\n"
           "\t- <Entree>    passe au tour suivant\n\n");
}

void creer_save(char nom[64]){
    FILE* f=fopen(nom,"w");
    fprintf(f,"%d\n%d\n%d\n",game.cagnotte,game.score,game.tour); // game.score à ajouter
    Tourelle* t=game.tourelles;
    Etudiant* e=game.etudiants;
    while (t!=NULL){
        fprintf(f,"%d %d %d %d\n",t->type,t->ligne,t->position,t->pointsDeVie);
        t=t->next;
    }
    fprintf(f,"\n"); //2e saut de ligne pour différencier tourelles et ennemis
    while (e!=NULL){
        fprintf(f,"%d %d %d %d\n",e->type,e->ligne,e->position,e->tour,e->pointsDeVie);
        e=e->next;
    }
    fclose(f);
}

int interInstru(void){ //pas terminé on ajoutera des instructions possibles au fil du temps 
    printf("Cagnotte : %d\n",game.cagnotte);
    char instru[256];
    int len;
    printf("\ngame > ");
    fgets(instru, 256, stdin);
    len = strlen(instru);

    if ( len > 1)
        instru[len - 1] = '\0';


    if (!strcmp(instru,"help")){
        help();
    }
    else if (!strcmp(instru,"StatTour")){
        char c;
        int i=0;
        printf("Types possibles:\n");
        while (i<tourelle_types.count){
            printf("%d\n",tourelle_types.arr[i].type.t_type.id);
            i++;
        }
        printf("\nDonnez le type de la tourelle : ");
        scanf("%c",&c);
        printf("Type : %d : %s : Degats : %d, PV : %d, prix : %d\n",tourelle_types.arr[i].type.t_type.id, tourelle_types.arr[i].type.t_type.name, tourelle_types.arr[i].type.t_type.damage_type, tourelle_types.arr[i].type.t_type.pointsDeVie, tourelle_types.arr[i].type.t_type.prix);
    }
    else if (!strcmp(instru,"StatEtu")){
        int i=0;
        char c;
        printf("Types possibles:\n");
        while (i<etudiant_types.count){
            printf("%d\n",etudiant_types.arr[i].type.e_type.id);
            i++;
        }
        printf("\nDonnez le type de l'étudiant : ");
        scanf("%c",&c);
        printf("Type : %d : %s : Degats : %d, PV : %d, vitesse : %d\n",etudiant_types.arr[i].type.e_type.id, etudiant_types.arr[i].type.e_type.name, etudiant_types.arr[i].type.e_type.damage_type, etudiant_types.arr[i].type.e_type.pointsDeVie, etudiant_types.arr[i].type.e_type.vitesse);
    }
    else if (!strcmp(instru,"end")){
        game.finished=1;
        return 0;
    }
    else if (!strcmp(instru,"PlaceTour")){
        int line, position, type;
        bool error;
        int i=0;
        printf("\nTourelles disponibles :\n");
        while (i<tourelle_types.count){
            printf("\nType %d --- %s :\n%s\nDegats : %d, PV : %d, prix : %d\n",tourelle_types.arr[i].type.t_type.id, tourelle_types.arr[i].type.t_type.name, tourelle_types.arr[i].type.t_type.description, tourelle_types.arr[i].type.t_type.damage_type, tourelle_types.arr[i].type.t_type.pointsDeVie, tourelle_types.arr[i].type.t_type.prix);
            i++;
        }
        printf("\nSaisir le type de la tourelle :\n");
        scanf("%d",&type);
        if (game.cagnotte<entity_type_get_type_by_id(&tourelle_types, type)->type.t_type.prix){
            printf("Pas assez d'argent");
        }
        else{
            game.cagnotte-=entity_type_get_type_by_id(&tourelle_types, type)->type.t_type.prix;
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
    }
    else if (!strcmp(instru,"\n")){
        return 1;
    }
    else{
        printf("Instruction non reconnue.\n");
    }
    interInstru();
}
