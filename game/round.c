#include "round.h"
#include "../preprocessor_macros_constants.h"
#include "entity_types/entity_types.h"
#include "entity_types/entity_type_vector.h"

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

    if 
    ( 
        // no need to verify e->next_line->tour:
        // if this function is called, e->tour <= game.tour hence e->next_line->tour <= game.tour because it has a HIGHER POSITION  than e
          !  (  // NOT
        (  e->next_line && abs(e->next_line->position - e->position) == 1) ||
        (  t && (flags & G_POS) && abs(t->position - e->position ) == 1 ) 
             )
    ) 
    { e->position ++; }

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

void inflict_damage(Tagged_entity * t_e){

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

void move(Tagged_entity * t_e){

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



// void log(LOG_TYPE log_t, Tourelle * t, Etudiant * e){

//     char str[LOG_MAX_CHAR_NO];

//     Tourelle_type * t_type = entity_type_get_type_by_id(&tourelle_types, t->type)->type.t_type;
//     Etudiant_type * e_type = entity_type_get_type_by_id(&etudiant_types, e->type)->type.e_type;

//     switch (log_t)
//     {
//     case DEAD:
//         dead = 
//         sprintf(str, "La ");
//         break;

//     case DAMAGE_INFLICTED:

//         break;
    
//     default:
//         break;
//     }
// }
