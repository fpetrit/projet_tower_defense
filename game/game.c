#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "../preprocessor_macros_constants.h"

// bool tourelle_append(Tourelle * t, int type, int ligne, int position){
    
//     Tourelle * new = malloc(sizeof(Tourelle));

//     if (new){
//         new->type = type;
//         new->ligne = ligne;
//         new->position = position;
//     }

//     return (bool) new;
// }

Etudiant * edtudiant_insert(int type, int ligne, int position, int tour){
    Etudiant * new = malloc(sizeof(Etudiant));
    if (new){
        new->type = type;
        new->ligne = ligne;
        new->position = position;
        new->tour = tour;
    }

    // set it as the first node in the linked list
    game.etudiants = new;

    return new;
}


// A MODIFIER, VERSION PROVISOIRE, pour tester le chainage
Etudiant * etudiant_append(Etudiant * e, int type, int ligne, int position, int tour){
    Etudiant * new = malloc(sizeof(Etudiant));

    if (new){
        new->type = type;
        new->ligne = ligne;
        new->position = position;
        new->tour = tour;
    }

    e->next = new;

    return new;
}



void game_init(FILE * level){

    int round_no, line_no;
    char type;
    bool error = false;

    char str[2];

    Etudiant * current_last_etudiant_on_line[ROWS];
    memset(current_last_etudiant_on_line, 0, ROWS * sizeof(Etudiant *));

    Etudiant * prev_e = NULL;

    // fill cagnotte
    fscanf(level, "%d", &game.cagnotte);

    if (! feof(level) ){

        fscanf(level, " %d %d %c", &round_no, &line_no, &type);

        // to chain Etudiants
        prev_e = edtudiant_insert(type, line_no, 0, round_no);

        error = prev_e == NULL;
    }

    // CHAINING
    while ( ! feof(level) && ! error){

        // first simple chaining
        fscanf(level, " %d %d %c", &round_no, &line_no, &type);

        prev_e = etudiant_append(prev_e, type, line_no, 0, round_no);

        // erreur si les champs "tour" des lignes ne sont dans l'ordre croissant
        // nécessaire pour l'algo de chaînage double par ligne
        // erreur également si malloc n'a pas fonctionné
        error = prev_e == NULL || prev_e->tour > round_no;
        
        if ( ! error ) {

            // if it is the first etudiant created for this line, he does not have next etudiant
            if (current_last_etudiant_on_line[prev_e->ligne - 1] == NULL){
                current_last_etudiant_on_line[prev_e->ligne - 1] = prev_e;
                prev_e->next_line = NULL;
            } else {
                current_last_etudiant_on_line[prev_e->ligne - 1]->next_line = prev_e;
                prev_e->prev_line = current_last_etudiant_on_line[prev_e->ligne - 1];
                current_last_etudiant_on_line[prev_e->ligne - 1] = prev_e;
            }

        } else if (prev_e != NULL) {
            fprintf(stderr, "Error: incorrect level text file format.\nFirst fields of each row must be ordered in ascending order.");
        } else {
            fprintf(stderr, "Error: something went wrong with malloc.");
        }

        fgets(str, 2, level);
    }

    // the last etudiant on each line has no previous etudiant on the same line
    for (int i = 0; i < ROWS; i++){
        current_last_etudiant_on_line[i]->prev_line = NULL;
    }

    
    // todo: filling of the Etudiant & Tourelle types global arrays


}


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