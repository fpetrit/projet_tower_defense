#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../preprocessor_macros_constants.h"
#include "game.h"
#include "round.h"
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



void tourelle_insert(Tourelle * t){
    t->next = game.tourelles;
    game.tourelles = t;
}



void tourelle_append(Tourelle * appended, Tourelle * t){
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

    Tourelle * new;
    Tourelle * node = game.tourelles;

    POS_FLAGS flags_e = 0;
    POS_FLAGS flags_t = 0;
    *error = false;

    Tourelle * node_t = tourelle_get_nearest_line(ligne, position, &flags_t);
    etudiant_get_nearest_line(ligne, position, &flags_e);

    *error = ( (flags_e | flags_t) & EQ_POS );
    
    // there is no tourelle/etudiant at this position (true / false)
    // new <--- tourelle_create
    // else
    // new <--- NULL
    new = ! *error ? tourelle_create(type, ligne, position) : NULL;

    // check at the same time if dynamic allocation failed in tourelle_create
    if (new){
        
        if (node){

            // get the last tourelle in the simple linking and append new to it
            while (node->next) { node = node->next; }
            tourelle_append(new, node);

            if (flags_t & G_POS)
                tourelle_line_append(new, node_t);
            // else --> L_POS is set
            else
                tourelle_line_prepend(new, node_t);
        }

        // this is the first tourelle
        else
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



void etudiant_insert(Etudiant * e){
    e->next = game.etudiants;
    game.etudiants = e;
}



void etudiant_append(Etudiant * appended, Etudiant * e){
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
    Etudiant * last_correct = NULL;

    if (node) {

        // get a tourelle on the same line
        while (node && (node->ligne != line || node->tour > game.tour ) ) { node = node->next; }

        if (node){

            // node is: 
            // on the right
            // or directly to the left
            // or at the same pos
            while (node->prev_line && node->position > position) {
                if (node->tour <= game.tour) last_correct = node;
                node = node->prev_line;
            }

            if (node->tour > game.tour)
                node = last_correct;

            // else the node is correct

            // node is: directly on the left
            // or directly on the right
            // or at the same pos
            while (node->next_line && node->position < position) {
                if (node->tour <= game.tour ) last_correct = node;
                node = node->next_line ;
            }

            if (node->tour > game.tour)
                node = last_correct;

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
    game.finished = false;
    game.won = false;

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

    // save game if not finished

    // save game stats if finished



    // free the type vectors memory
    entity_type_vector_free(&tourelle_types);
    entity_type_vector_free(&etudiant_types);

    // free the linked lists nodes memory

    // tourelles
    Tourelle * t = game.tourelles;
    Tourelle * prev_t;
    while (t){
        prev_t = t;
        t = t->next;
        free(prev_t);
    }

    // etudiants
    Etudiant * e = game.etudiants;
    Etudiant * prev_e;
    while (e){
        prev_e = e;
        e = e->next;
        free(prev_e);
    }
}



void update_tourelles(void){

    Tagged_entity entity;
    Tourelle * t = &entity.entity.tourelle;
    entity.tag = TOURELLE;

    while (t){
        // tourelles cannot move for now
        // move(&entity);
        inflict_damage(&entity);
        t = t->next;
    }
}



void update_etudiants(void){

    Tagged_entity entity;
    Etudiant * e = &entity.entity.etudiant;
    entity.tag = ETUDIANT;

    while (e){
        move(&entity);
        inflict_damage(&entity);
        e = e->next;
    }
}



static inline int count_etudiants(void){
    int count = 0;
    Etudiant * e = game.etudiants;
    while ( e ) { count++; e = e->next; } 
    return count;
}



void update_round(void){

    // the etudiants of the current round no. ust appear if their position is free (line i, position 0)
    // else their round no. is incrementedm

    POS_FLAGS flags_e = 0;
    POS_FLAGS flags_t = 0;

    Etudiant * e;
    Tourelle * t;

    Tourelle_type * ttype;
    Etudiant_type * etype;

    e = game.etudiants;
    while (e) {

        if (e->tour == game.tour + 1) {
            
            // these funcitons reset flags_e and flags_t, no need to set it to 0 here
            // only takes in account the visible entities
            etudiant_get_nearest_line(e->ligne, e->position, &flags_e);
            tourelle_get_nearest_line(e->ligne, e->position, &flags_t);

            if ( (flags_t | flags_e) & EQ_POS )
                e->tour ++;

            // else, the etudiant will be taken in account by the game since e->tour == game.tour after incrementing game.tour
        }
        
        e = e->next;
    }

    // increment game.tour AFTER THE NEAREST ENTITIES SEARCH
    // to ignore during the search the etudiants that must appear at this current round
    game.tour++;
    
    // tourelles are updated (move & inflict damages)
    update_tourelles();

    // etudiants are updated (move & inflict damages)
    update_etudiants();

    // we must collect the dead entities
    // delete them from the linking
    // print the events

    // tourelles
    t = game.tourelles;
    while (t) {

        ttype = &entity_type_get_type_by_id(&tourelle_types, t->type)->type.t_type;

        if (t->pointsDeVie <= 0) {
            
            printf("La tourelle '%s', position (%d, %d) a ete detruite !\n",
            ttype->name, t->ligne, t->position);

            tourelle_delete(t);
        }
    }

    // if one etudiant has reached the last position, the game is lost
    // etudiants
    e = game.etudiants;
    while (e) {

        etype = &entity_type_get_type_by_id(&etudiant_types, e->type)->type.e_type;

        if (e->pointsDeVie <= 0) {
            
            printf("L'étudiant '%s', position (%d, %d) a ete elemine !\n",
            etype->name, e->ligne, e->position);

            etudiant_delete(e);
        }

        // not dead and has reached his line last position
        else if (e->position == ROWS - 1){

            printf("Game over !\n\
                    l'etudiant '%s' a atteint la dernière position sur la ligne %d.\n",
                    etype->name, e->ligne);

            game.finished = true;
            game.won = false;
        }

        e = e->next;
    }

    // if not already lost & there is no more enemy in the linked list: win 
    if ( ! game.finished ) {
        int etudiant_no = count_etudiants();
        if (etudiant_no == 0){
            game.won = true;
            game.finished = true;
            printf("Vous avez gagne !\n Tous les ennemis sont morts.\n");
        }
    }
}

