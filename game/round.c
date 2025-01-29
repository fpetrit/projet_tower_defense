#include <limits.h>

#include "round.h"
#include "../preprocessor_macros_constants.h"
#include "entity_types/entity_types.h"
#include "entity_types/entity_type_vector.h"
#include "game.h"

#define min(a, b)   ((a <= b) ? a : b)

extern Entity_type_vector tourelle_types;
extern Entity_type_vector etudiant_types;


// FUNCTIONS TO INFLICT DAMAGE

// read function "tourelle damage no 0"
void t_d_0(Tourelle * t){

    POS_FLAGS flags = 0;
    
    // find the nearest etudiant on the same line
    // if there is an etudiant to the left, it will be returned as a priority
    Etudiant * e = etudiant_get_nearest_line(t->ligne, t->position, &flags);

    if (e)
        e->pointsDeVie -- ;
}



void e_d_0(Etudiant * e){

    POS_FLAGS flags = 0;

    Tourelle * t = tourelle_get_nearest_line(e->ligne, e->position, &flags);

    if (t && abs(e->position - t->position) == 1)
        t->pointsDeVie --;
        
    // else no tourelle on the same line, or the nearest tourelle is not right next to the etudiant
    // do nothing
}



// FUNCTIONS TO MOVE

// read function "tourelle move no 0"
void t_m_0(Tourelle * t);

void e_m_0(Etudiant * e){

    POS_FLAGS flags = 0;
    Tourelle * t = tourelle_get_nearest_line(e->ligne, e->position, &flags);

    int d1, d2;

    d1 = (e->next_line) ? abs(e->next_line->position - e->position) : INT_MAX;
    d2 = (t && (flags & L_POS)) ? abs(t->position - e->position ) : INT_MAX;

    d1 = min(d1, d2);

    e->position += min(d1 - 1, e->vitesse);

    e->position = min(e->position, COLUMNS);
}



// GLOBAL ARRAYS TO STORE THE ATTACK AND MOVE FUNCTIONS
// the damage type of an entity i is implemented by the function at index i of the corresponding function array 

const static void (* etudiant_inflict_damage[]) (Etudiant *) = {
    e_d_0,
};

const static void (* tourelle_inflict_damage[]) (Tourelle *) = {
    t_d_0,
};

const static void (* tourelle_move[]) (Tourelle *) = {
    // t_m_0,
};

const static void (* etudiant_move[]) (Etudiant *) = {
    e_m_0
};



// WRAPPER FUNCTIONS: AN ENTITY CAN MOVE AND INFLICT DAMAGES

void inflict_damage(Tagged_entity_p * t_e){

    int d_index;

    switch (t_e->tag){

        case ETUDIANT:
            if (t_e->entity.etudiant->tour <= game.tour) {
                d_index = entity_type_get_type_by_id(&etudiant_types, t_e->entity.etudiant->type)->type.e_type.damage_type;
                etudiant_inflict_damage[d_index](t_e->entity.etudiant);
            }
            break;

        case TOURELLE:
            d_index = entity_type_get_type_by_id(&tourelle_types, t_e->entity.tourelle->type)->type.t_type.damage_type;
            tourelle_inflict_damage[d_index](t_e->entity.tourelle);
            break;

        default:
            break;
    }
}

void move(Tagged_entity_p * t_e){

    int m_index;

    switch (t_e->tag)
    {
    case ETUDIANT:
        if (t_e->entity.etudiant->tour <= game.tour) {
            m_index = entity_type_get_type_by_id(&etudiant_types, t_e->entity.etudiant->type)->type.e_type.move_type;
            etudiant_move[m_index](t_e->entity.etudiant);
        }
        break;
    
    case TOURELLE:
        m_index = entity_type_get_type_by_id(&etudiant_types, t_e->entity.tourelle->type)->type.t_type.move_type;
        tourelle_move[m_index](t_e->entity.tourelle);
        break;
    
    default:
        break;
    }
}



void save_log(LOG_TYPE log_type, Tagged_entity t_entity, Log_storage * storage){

    Log_infos infos;
    infos.t_entity = t_entity;
    infos.type = log_type;

    if (storage->count < storage->length) {
        storage->arr[storage->count] = infos;
        storage->count++; 
    }
    else
        fprintf(stderr, "Error: too many game logs in the Log_storage struct");
}



static const char * log_format_strings[] = {
    "La tourelle '%s', ligne %d position %d, a ete detruite !\n\n",
    "L'etudiant '%s',  ligne %d position %d, a ete elemine !\n\n",
    "Tous les etudiants ont ete elemines ! Vous avez gagne !\n\n",
    "L'etudiant '%s', a atteint sa derniere position sur la ligne %d !\nVous avez perdu !\n",
};



void display_logs(Log_storage * storage){

    int i = 0;
    Tagged_entity t_entity;
    union Entity_type entity_type;


    while (i < storage->count){

        const char * format_string = log_format_strings[storage->arr[i].type];

        t_entity = storage->arr[i].t_entity;

        if ( t_entity.tag == ETUDIANT )
            entity_type = entity_type_get_type_by_id(&etudiant_types, t_entity.entity.etudiant.type)->type;
        else if ( t_entity.tag == TOURELLE)
            entity_type = entity_type_get_type_by_id(&tourelle_types, t_entity.entity.tourelle.type)->type;

        switch (storage->arr[i].type)
        {
        case DEAD_TOURELLE:
            printf(format_string, entity_type.t_type.name, t_entity.entity.tourelle.ligne, t_entity.entity.tourelle.position);
            break;

        case DEAD_ETUDIANT:
            printf(format_string, entity_type.e_type.name, t_entity.entity.etudiant.ligne, t_entity.entity.etudiant.position);
            break;

        case PLAYER_WIN:
            printf(format_string);
            break;

        case ETUDIANT_WIN:
            printf(format_string, entity_type.e_type.name, t_entity.entity.etudiant.ligne);
            break;
        
        default:
            break;
        }

        i++;

    }
}