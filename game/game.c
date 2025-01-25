#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../preprocessor_macros_constants.h"
#include "game.h"
#include "entity_types/entity_types.h"
#include "entity_types/entity_type_vector.h"


Tourelle * tourelle_create(int type, int ligne, int position){
    Tourelle * new = NULL;

    Tagged_entity_type * ttype_res = entity_type_get_type_by_id(&tourelle_types, type);

    if (ttype_res)
        new = malloc(sizeof(Tourelle));

    else
        // if null
        fprintf(stderr, "Error: tourelle_create failed to retrieve the Tourelle type described by the '%d' type id.\n\
        Maybe the Tourelle type description is missing in tourelles.txt.\n", type);

    if (new){

        // fill non type dependent members
        new->type = type;
        new->ligne = ligne;
        new->position = position;

        new->next = NULL;

        // fill type dependent members
        Tourelle_type t_type = ttype_res->type.t_type;

        new->pointsDeVie = t_type.pointsDeVie;
        new->prix = t_type.prix;
    }

    return new;
}



Tourelle * tourelle_insert(int type, int ligne, int position){
    Tourelle * new = tourelle_create(type, ligne, position);

    if (new){
        // may be NULL;
        new->next = game.tourelles;
        game.etudiants = new;
    }

    return new;
}



Tourelle * tourelle_append(Tourelle * t, int type, int ligne, int position){
    Tourelle * new = tourelle_create(type, ligne, position);

    if (new){
        // may be NULL
        new->next = t->next;
        t->next = new;
    }

    return new;
}



void tourelle_delete(Tourelle * t){
    
    // simple linking by creation time
    Tourelle * node = game.tourelles;

    // we compare the pointers rather than the struct values for each field
    // two different Etudiants may have the same struct members values

    if ( node != t){
        while ( node && node->next != t){
            node = node->next;
        }
    }

    if ( ! node )
        fprintf(stderr, "Error: Etudiant not found in the linked list during deletion.\n");
    else {
        node->next = t->next;
    }

    free(t);
}



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

        new->prev_line = NULL;
        new->next_line = NULL;
        new->next = NULL;

        // fill type dependent members
        Etudiant_type e_type = ttype_res->type.e_type;

        new->type = e_type.id;
        new->pointsDeVie = e_type.pointsDeVie;
    }

    return new;
}


Etudiant * etudiant_insert(char abbr, int ligne, int position, int tour){

    Etudiant * new = etudiant_create(abbr, ligne, position, tour);

    if (new){
        new->next = game.etudiants;
        game.etudiants = new;
    }

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



void etudiant_line_prepend(Etudiant * prepended, Etudiant * e){
    prepended->prev_line = e->prev_line;
    prepended->next_line = e;

    if (prepended->prev_line)
        prepended->prev_line->next_line = prepended;

    e->prev_line = prepended;    
}

void etudiant_line_append(Etudiant * appended, Etudiant * e){
    appended->prev_line = e;
    appended->next_line = e->next_line;

    if (appended->next_line)
        appended->next_line->prev_line = appended;

    e->next_line = appended;
}



void etudiant_delete(Etudiant * e){

    // double linking by line

    if (e->next_line)
        e->next_line->prev_line = e->prev_line;

    if (e->prev_line)
        e->prev_line->next_line = e->next_line;

    // simple linking by creation time
    Etudiant * node = game.etudiants;

    // we compare the pointers rather than the struct values for each field
    // two different Etudiants may have the same struct members values

    if ( node != e){
        while ( node && node->next != e){
            node = node->next;
        }
    }

    if ( ! node )
        fprintf(stderr, "Error: Etudiant not found in the linked list during deletion.\n");
    else {
        node->next = e->next;
    }

    free(e);
}



void game_init(FILE * level){

    game.etudiants = NULL;
    game.tourelles = NULL;

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