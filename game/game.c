#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../preprocessor_macros_constants.h"
#include "game.h"
#include "round.h"
#include "entity_types/entity_types.h"
#include "entity_types/entity_type_vector.h"
#include "display.h"

#define max(a, b)   (a <= b) b : a

extern Log_storage logs;

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
        new->round_no = game.tour;

        new->effect = 0;
        memset(&new->effect_remaining_time, 0, EFFECT_NO);
        memset(&new->effect_values, 0, EFFECT_NO);

        new->next = NULL;
        new->next_line = NULL;
        new->prev_line = NULL;

        // fill type dependent members
        Tourelle_type t_type = ttype_res->type.t_type;

        new->pointsDeVie = t_type.pointsDeVie;
        new->prix = t_type.prix;
        new->strength = t_type.strength;
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
        while (node && node->ligne != line) { node = node->next; }

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

            else if ( flags_t & L_POS )
                tourelle_line_prepend(new, node_t);
            
            // else: EQ_POS is not set, hence no tourelle on the same line and node_t is NULL
        }

        // this is the first tourelle
        else
            tourelle_insert(new);
    } 
    
    return new;
}



void tourelle_delete(Tourelle * t){

    // THIS FUNCTION ASSUMES THAT game.tourelles != NULL AND t IS IN THE LINKED LIST
    
    // simple linking by creation time
    Tourelle * node = game.tourelles;

    // we compare the pointers rather than the struct values for each field
    // two different Etudiants may have the same struct members values

    if ( game.tourelles == t)
        game.tourelles = t->next;

    else {
        while ( node->next != t) { node = node->next; }
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

    if (new && ttype_res){

        // fill non type dependent members
        new->ligne = ligne;
        new->position = position;
        new->tour = tour;

        new->effect = 0;
        memset(&new->effect_remaining_time, 0, EFFECT_NO);
        memset(&new->effect_values, 0, EFFECT_NO);

        new->prev_line = NULL;
        new->next_line = NULL;
        new->next = NULL;

        // fill type dependent members
        Etudiant_type e_type = ttype_res->type.e_type;

        new->type = e_type.id;
        new->pointsDeVie = e_type.pointsDeVie;
        new->vitesse = e_type.vitesse;
        new->strength = e_type.strength;
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

        // get an etudiant on the same line
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

    // simple linking by creation time
    Etudiant * node = game.etudiants;

    // we compare the pointers rather than the struct values for each field
    // two different Etudiants may have the same struct members values

    if (game.etudiants == e)
        game.etudiants = e->next;

    else {
        while (node->next != e) { node = node->next; }
        node->next = e->next;
    }

    // double linking by line

    if (e->next_line)
        e->next_line->prev_line = e->prev_line;

    if (e->prev_line)
        e->prev_line->next_line = e->next_line;

    free(e);
}



void game_init(FILE * level){

    game.etudiants = NULL;
    game.tourelles = NULL;
    game.finished = false;
    game.won = false;
    game.etudiant_last_tour = 0;
    game.score = 0;

    // init & fill the Entity_type_vector tourelle_types & etudiant_types global variable defined in main.c
    init_types();

    int round_no, line_no;
    char abbr ;
    bool error = false;

    char str[2];

    Etudiant * current_last_etudiant_on_line[ROWS];
    memset(current_last_etudiant_on_line, 0, ROWS * sizeof(Etudiant *));

    Etudiant * prev_e = NULL;
    Etudiant * e;

    // fill cagnotte
    fscanf(level, "%d", &game.cagnotte);

    // create the first etudiant, initialize prev_e
    if (! feof(level) ){
        fscanf(level, " %d %d %c", &round_no, &line_no, &abbr );
        // to chain Etudiants
        prev_e = etudiant_create(abbr , line_no, 0, round_no);
        error = prev_e == NULL;

        if (!error) {
            etudiant_insert(prev_e);
            current_last_etudiant_on_line[prev_e->ligne - 1] = prev_e;
            prev_e->next_line = NULL;

            if (prev_e->tour > game.etudiant_last_tour)
                game.etudiant_last_tour = prev_e->tour;
        }
    
    }

    // CHAINING
    while ( ! feof(level) && ! error){

        // first simple chaining
        fscanf(level, " %d %d %c", &round_no, &line_no, &abbr );

        e = etudiant_create(abbr , line_no, 0, round_no);
        
        // erreur si les champs "tour" des lignes ne sont dans l'ordre croissant
        // nécessaire pour l'algo de chaînage double par ligne
        // erreur également si malloc n'a pas fonctionné
        error = e == NULL || prev_e->tour > round_no;
        
        if ( ! error ) {

            etudiant_append(e, prev_e);
            prev_e = e;

            // if it is the first etudiant created for this line, he does not have next etudiant
            if (current_last_etudiant_on_line[prev_e->ligne - 1] == NULL){
                current_last_etudiant_on_line[prev_e->ligne - 1] = prev_e;
                prev_e->next_line = NULL;
            } else {
                current_last_etudiant_on_line[prev_e->ligne - 1]->prev_line = prev_e;
                prev_e->next_line = current_last_etudiant_on_line[prev_e->ligne - 1];
                current_last_etudiant_on_line[prev_e->ligne - 1] = prev_e;
            }

            if (prev_e->tour > game.etudiant_last_tour)
                game.etudiant_last_tour = prev_e->tour;

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

    Tagged_entity_p entity;
    entity.tag = TOURELLE;

    Tourelle * t = game.tourelles;
    
    while (t){
        entity.entity.tourelle = t;

        // tourelles cannot move for now
        // move(&entity);
        inflict_damage(&entity);

        t = t->next;
    }
}



void update_etudiants(void){

    Tagged_entity_p entity;
    entity.tag = ETUDIANT;

    Etudiant * e = game.etudiants;

    while (e){
        entity.entity.etudiant = e;

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



void next_round(void){

    POS_FLAGS flags_e = 0;
    POS_FLAGS flags_t = 0;

    Etudiant * e;
    Tourelle * t;

    Tagged_entity tmp_t_entity;
    Etudiant * tmp_e;
    Tourelle * tmp_t;
    Etudiant_type e_type;
    Tourelle_type t_type;

    
    logs.count = 0;
    logs.length = LOGS_MAX_NO;

    int score;
    bool stop;

    // the etudiants of the current round no. ust appear if their position is free (line i, position 0)
    // else their round no. is incremented

    // take the opportunity to apply per round effect

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

    manage_per_round_effects();
    
    // tourelles are updated (move & inflict damages)
    update_tourelles();

    // etudiants are updated (move & inflict damages)
    update_etudiants();

    // we must collect the dead entities
    // delete them from the linking
    // print the events

    // tourelles
    tmp_t_entity.tag = TOURELLE;
    t = game.tourelles;
    while (t) {

        if (t->pointsDeVie <= 0) {
            
            // tmp_t_entity has type Tagged_entity (NOT Tagged_entity_p) that store the entity data itself and not pointers to it
            // because the tourelle memory is not longer available
            tmp_t_entity.entity.tourelle = *t;

            tmp_t = t;
            t = t->next;

            t_type = entity_type_get_type_by_id(&tourelle_types, tmp_t->type)->type.t_type;
            score = tourelle_get_score(t_type, game.tour);

            tourelle_delete(tmp_t);
            game.score += score;

            save_log(DEAD_TOURELLE, tmp_t_entity, score);

        }
        
        else
            t = t->next;
    }

    // if one etudiant has reached the last position, the game is lost
    // if one etudiant is dead we compute the score increment
    // etudiants
    tmp_t_entity.tag = ETUDIANT;
    stop = false;
    e = game.etudiants;
    while (! stop && e) {

        if (e->pointsDeVie <= 0) {
            
            tmp_t_entity.entity.etudiant = *e;

            tmp_e = e;
            e = e->next;

            e_type = entity_type_get_type_by_id(&etudiant_types, tmp_e->type)->type.e_type;
            score = etudiant_get_score(e_type, game.tour);

            game.score += score;
            etudiant_delete(tmp_e);

            save_log(DEAD_ETUDIANT, tmp_t_entity, score);
        }

        // not dead and has reached his line last position
        else if (e->position == COLUMNS){

            stop = true;

            tmp_t_entity.entity.etudiant = *e;
            save_log(ETUDIANT_WIN, tmp_t_entity, 0);

            game.finished = true;
            game.won = false;
        }

        else
            e = e->next;
    }

    memset(&tmp_t_entity, 0, sizeof(Tagged_entity));
    // if not already lost & there is no more enemy in the linked list: win 
    if ( ! game.finished ) {
        int etudiant_no = count_etudiants();
        if (etudiant_no == 0){
            game.won = true;
            game.finished = true;
            save_log(PLAYER_WIN, tmp_t_entity, 0);
        }
    }


    // update the temporary effects (decrement remaining times and reset characteristics)
    manage_effects();


    // print the game then the messages

    affiche_jeu();

    display_logs();

    printf("Score : %d\n\n", game.score);


}

