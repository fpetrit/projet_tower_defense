#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../preprocessor_macros_constants.h"
#include "game.h"
#include "entity_types/entity_types.h"
#include "entity_types/entity_type_vector.h"


Etudiant * etudiant_create(char abbr, int ligne, int position, int tour){

    Etudiant * new = NULL;

    Tagged_entity_type * ttype_res = entity_type_get_type_by_abbr(&etudiant_types, abbr, ETUDIANT);

    if (ttype_res)
        new = malloc(sizeof(Etudiant));
    else
        // if null
        fprintf(stderr, "Error: etudiant_create failed to retrieve the Etudiant type described by '%c'.\n\
        Maybe the Etudiant type description is missing in vilains.txt.\n", abbr);

    if (new){

        // fill non type dependent members
        new->ligne = ligne;
        new->position = position;
        new->tour = tour;

        // fill type dependent members
        Etudiant_type e_type = ttype_res->type.e_type;

        new->type = e_type.id;
        new->pointsDeVie = e_type.pointsDeVie;
    }

    return new;
}


Etudiant * etudiant_insert(char abbr, int ligne, int position, int tour){

    Etudiant * new = etudiant_create(abbr, ligne, position, tour);

    if (new && game.etudiants){
        new->next = game.etudiants;
    }

    if (new)
        game.etudiants = new;

    return new;
}


Etudiant * etudiant_append(Etudiant * e, char abbr, int ligne, int position, int tour){
    
    Etudiant * new = etudiant_create(abbr, ligne, position, tour);

    if (new) {

    // may be null
    new->next = e->next;

    e->next = new;
    }

    return new;
}



void game_init(FILE * level){

    // init & fill the Entity_type_vector tourelle_types & etudiant_types global variable defined in main.c
    init_types();

    int round_no, line_no;
    char type;
    bool error = false;

    char str[2];

    Etudiant * current_last_etudiant_on_line[ROWS];
    memset(current_last_etudiant_on_line, 0, ROWS * sizeof(Etudiant *));

    Etudiant * prev_e = NULL;

    // fill cagnotte
    fscanf(level, "%d", &game.cagnotte);

    // prev_e "initialization"
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
}


void game_end(void){
    entity_type_vector_free(&tourelle_types);
    entity_type_vector_free(&etudiant_types);
}