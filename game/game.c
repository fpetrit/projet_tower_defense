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
        new->next_line = NULL;
        new->prev_line = NULL;

        // fill type dependent members
        Tourelle_type t_type = ttype_res->type.t_type;

        new->pointsDeVie = t_type.pointsDeVie;
        new->prix = t_type.prix;
    }

    return new;
}



inline void tourelle_insert(Tourelle * t){
    t->next = game.tourelles;
    game.etudiants = t;
}



inline void tourelle_append(Tourelle * appended, Tourelle * t){
    appended->next = t->next;
    t->next = appended;
}



void tourelle_line_prepend(Tourelle * prepended, Tourelle * t){
    prepended->prev_line = t->prev_line;
    prepended->next_line = t;

    if (prepended->prev_line)
        prepended->prev_line->next_line = prepended;
    
    t->prev_line = prepended;
}



void tourelle_line_append(Tourelle * appended, Tourelle * t){
    appended->prev_line = t;
    appended->next_line = t->next_line;

    if (appended->next_line)
        appended->next_line->prev_line = appended;

    t->next_line = appended;
}



Tourelle * tourelle_get_nearest_line(int line, int position, POS_FLAGS * flags) {

    *flags = 0;
    
    Tourelle * node = game.tourelles;

    if (node) {

        // get a tourelle on the same line
        while (node->ligne != line) { node = node->next; }

        if (node){

            // node is: 
            // on the right
            //or or directly to the left
            // or at the same pos
            while (node->prev_line && node->position > position) { node = node->prev_line; }

            // node is: directly on the left
            // or directly on the right
            // or at the same pos
            while (node->next_line && node->position < position) { node = node->next_line ; }

            if (node->position == position)  
                *flags = EQ_POS;
            else if (node->position < position)
                *flags = G_POS;
            else
                *flags = L_POS;
        }
    }

    return node;
}



Tourelle * tourelle_add(int type, int ligne, int position, bool * error){

    POS_FLAGS flags = 0;
    *error = false;
    
    Tourelle * new = tourelle_create(type, ligne, position);
    Tourelle * node = game.tourelles;

    if (new){
        
        if (node){
            while (node->next) { node = node->next; }
            tourelle_append(new, node);

            node = tourelle_get_nearest_line(new->ligne, new->position, &flags);

            if (node){
                // error if there is a tourelle at the same position
                *error = flags & EQ_POS;

                if (!*error){

                    if (flags & G_POS)
                        tourelle_line_append(new, node);
                    else
                        tourelle_line_prepend(new, node);

                } else {
                    free(new);
                    new = NULL;
                }
            }
        }

        // else do nothing the prev_line & next_line pointers are NULL
        // there is no tourelle on the same line

    } else {
        tourelle_insert(new);
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

    // double linking by line

    if (t->prev_line)
        t->prev_line->next_line = t->next_line;

    if (t->next_line)
        t->next_line->prev_line = t->prev_line;

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



inline void etudiant_insert(Etudiant * e){
    e->next = game.etudiants;
    game.etudiants = e;
}



inline void etudiant_append(Etudiant * appended, Etudiant * e){
    appended->next = e->next;
    e->next = appended;
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



Etudiant * etudiant_get_nearest_line(int line, int position, POS_FLAGS * flags){

    *flags = 0;
    
    Etudiant * node = game.etudiants;

    if (node) {

        // get a tourelle on the same line
        while (node->ligne != line) { node = node->next; }

        if (node){

            // node is: 
            // on the right
            //or directly to the left
            // or at the same pos
            while (node->prev_line && node->position > position) { node = node->prev_line; }

            // node is: directly on the left
            // or directly on the right
            // or at the same pos
            while (node->next_line && node->position < position) { node = node->next_line ; }

            if (node->position == position)  
                *flags = EQ_POS;
            else if (node->position < position)
                *flags = G_POS;
            else
                *flags = L_POS;
        }
    }

    return node;
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
    Etudiant * e;

    // fill cagnotte
    fscanf(level, "%d", &game.cagnotte);

    // prev_e "initialization"
    if (! feof(level) ){
        fscanf(level, " %d %d %c", &round_no, &line_no, &type);
        // to chain Etudiants
        prev_e = etudiant_create(type, line_no, 0, round_no);
        etudiant_insert(prev_e);
        error = prev_e == NULL;
    }

    // CHAINING
    while ( ! feof(level) && ! error){

        // first simple chaining
        fscanf(level, " %d %d %c", &round_no, &line_no, &type);

        e = etudiant_create(type, line_no, 0, round_no);
        etudiant_append(e, prev_e);
        prev_e = e;

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


void end_game(void){
    entity_type_vector_free(&tourelle_types);
    entity_type_vector_free(&etudiant_types);
}