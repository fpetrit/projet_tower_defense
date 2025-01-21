#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "preprocessor_macros_constants.h"

// bool tourelle_append(Tourelle * t, int type, int ligne, int position){
    
//     Tourelle * new = malloc(sizeof(Tourelle));

//     if (new){
//         new->type = type;
//         new->ligne = ligne;
//         new->position = position;
//     }

//     return (bool) new;
// }


void game_init(FILE * level){

    int round_no, line_no;
    char type;
    bool error = false;

    Etudiant * current_last_etudiant_on_line[ROWS];
    memset(current_last_etudiant_on_line, 0, ROWS * sizeof(Etudiant *));

    // fill cagnotte
    fscanf(level, "%d", &game.cagnotte);

    // to chain Etudiants
    Etudiant * prev_e = game.etudiants;

    // CHAINING
    while ( ! feof(level) && ! error ){
        // first simple chaining
        fscanf(level, " %d %d %c", &round_no, &line_no, &type);

        // erreur si les champs "tour" des lignes ne sont dans l'ordre croissant
        // nécessaire pour l'algo de chaînage double par ligne
        error = prev_e == NULL || prev_e->tour > round_no;

        if ( ! error) {
            etudiant_append(prev_e, type, line_no, 0, round_no);
            prev_e = prev_e->next;

            // if it is the first etudiant created for this line, he does not have next etudiant
            if (current_last_etudiant_on_line[prev_e->ligne] == NULL){
                current_last_etudiant_on_line[prev_e->ligne] = prev_e;
                prev_e->next_line = NULL;
            } else {
                current_last_etudiant_on_line[prev_e->ligne]->next_line = prev_e;
                prev_e->prev_line = current_last_etudiant_on_line[prev_e->ligne];
                current_last_etudiant_on_line[prev_e->ligne] = prev_e;
            }

            // the last etudiant on each line has no previous etudiant on the same line
            for (int i = 0; i < ROWS; i++){
                current_last_etudiant_on_line[i]->prev_line = NULL;
            }
        } else
            fprintf(stderr, "Error: incorrect level text file format.\nFirst fields of each row must be ordered in ascending order.");
    }

    
    // todo: filling of the Etudiant & Tourelle types global arrays


}