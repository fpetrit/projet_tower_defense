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

Etudiant * etudiant_insert(int type, int ligne, int position, int tour){
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
        prev_e = etudiant_insert(type, line_no, 0, round_no);

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