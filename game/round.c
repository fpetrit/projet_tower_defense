#include <limits.h>
#include <math.h>
#include <string.h>

#include "round.h"
#include "../preprocessor_macros_constants.h"
#include "entity_types/entity_types.h"
#include "entity_types/entity_type_vector.h"
#include "game.h"

#define min(a, b)   ((a <= b) ? a : b)
#define max(a, b)   ((a <= b) ? b : a)

extern Entity_type_vector tourelle_types;
extern Entity_type_vector etudiant_types;

// temporary effects setters & helpers

int get_effect_index(TEMP_EFFECT effect){
    return (int) round( log2(effect) ) ;
}

TEMP_EFFECT get_tmp_effect_type(int index){
    return (int) round(pow(2, index));
}

void set_speed_effect(Etudiant * e, int value){

    e->effect |= SPEED;

    e->effect_remaining_time[get_effect_index(SPEED)] = 3;

    e->vitesse = max(0, e->vitesse + value);

}

// FUNCTIONS TO INFLICT DAMAGE ////////////////////////////////////

// TOURELLES

// read function "tourelle damage no 0"
// classic
void t_d_0(Tourelle * t){

    POS_FLAGS flags = 0;
    
    // find the nearest etudiant on the same line
    // if there is an etudiant to the left, it will be returned as a priority
    Etudiant * e = etudiant_get_nearest_line(t->ligne, t->position, &flags);

    if (e)
        e->pointsDeVie = max(0, e->pointsDeVie - t->strength) ;
}

// inflicts damages in depth (3 etudiants on the line on the same direction max)
void t_d_1(Tourelle * t){

    POS_FLAGS flags = 0;

    Etudiant * e = etudiant_get_nearest_line(t->ligne, t->position, &flags);

    int i = 0;
    while (i < 2 && e){
        e->pointsDeVie = max(0, e->pointsDeVie - t->strength);

        // etudiants are targeted on the right
        if (flags & G_POS)
            e = e->prev_line;

        // L_POS must be set
        // etudiants are targeted on the left
        else
            e = e->next_line;
    }
}

// bomb type, damages inflicted to all etudiants on the same column and line
void t_d_2(Tourelle * t){

    Etudiant * e = game.etudiants;
    while (e && e->ligne != t->ligne) { e = e->next; }

    if (e) {
        while (e->prev_line) { e = e->prev_line; }

        while (e){
            e->pointsDeVie = max(0, e->pointsDeVie - t->strength);
            e = e->next_line;
        }
    }

    // to find the etduiants on the same column no other choices: must browse all the linked list
    e = game.etudiants;
    while (e){
        if (e->position == t->position) e->pointsDeVie = max(0, e->pointsDeVie - t->strength);
        e = e->next;
    }
}

// classic + a speed malus temp effect (3 rounds remaining time)
void t_d_3(Tourelle * t){

    POS_FLAGS flags = 0;

    Etudiant * e = etudiant_get_nearest_line(t->ligne, t->position, &flags);

    if (e) {
        e->pointsDeVie = max(0, e->pointsDeVie - t->strength);
        set_speed_effect(e, -1);
    }

}

void t_d_4(Tourelle * t){

    Etudiant * e = game.etudiants;
    Etudiant * max_e = game.etudiants;
    Tourelle * tt = game.tourelles;

    // no seg fault is no etudiants bc e = max_e
    while (e){
        if (e->position > max_e->position)
            max_e = e;

        e = e->next;
    }

    e = game.etudiants;
    while (e){
        if (e->position == max_e->position)
            e->pointsDeVie = max(0, e->pointsDeVie - t->strength);
        e = e->next;
    }

    while (tt){
        if (tt->position == max_e->position)
            tt->pointsDeVie = max(0, tt->pointsDeVie - t->strength);
        tt = tt->next;
    }
}

// ETUDIANTS

// classic
void e_d_0(Etudiant * e){

    POS_FLAGS flags = 0;

    Tourelle * t = tourelle_get_nearest_line(e->ligne, e->position, &flags);

    if (t && abs(e->position - t->position) == 1)
        t->pointsDeVie --;
        
    // else no tourelle on the same line, or the nearest tourelle is not right next to the etudiant
    // do nothing
}

// do not inflict damages but heals 1 PV to the first ennemy on line
void e_d_1(Etudiant * e){

    // heals himself if he is the first on the line

    Etudiant * a = e;
    while (a->next_line){
        a = a->next_line;
    }

    // does not heal dead etudiants
    // health points limited to its initial value (type dependent)
    if (a->pointsDeVie > 0)
        a->pointsDeVie = min( entity_type_get_type_by_id(&etudiant_types, a->type)->type.e_type.pointsDeVie, a->pointsDeVie + 1  );
}



// FUNCTIONS TO MOVE //////////////////////////////////////////////

// TOURELLES, not implemented

// read function "tourelle move no 0"
void t_m_0(Tourelle * t);



// ETUDIANTS

// classic
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

// classic but the speed is updated to be the first etudiant on line speed's + 1
void  e_m_1(Etudiant * e){

    Etudiant * a = e;
    while (a->next_line){
        a = a->next_line;
    }

    e->vitesse = a->vitesse + 1;

    e_m_0(e);
}



// GLOBAL ARRAYS TO STORE THE ATTACK AND MOVE FUNCTIONS
// the damage type of an entity i is implemented by the function at index i of the corresponding function array 

const static void (* etudiant_inflict_damage[]) (Etudiant *) = {
    e_d_0,
    e_d_1,
};

const static void (* tourelle_inflict_damage[]) (Tourelle *) = {
    t_d_0,
    t_d_1,
    t_d_2,
    t_d_3,
    t_d_4,
};

const static void (* tourelle_move[]) (Tourelle *) = {
    // t_m_0,
};

const static void (* etudiant_move[]) (Etudiant *) = {
    e_m_0,
    e_m_1,
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

static Tagged_entity t_entity_p_to_t_entity(Tagged_entity_p t_e_p){
    Tagged_entity t_e;
    t_e.tag = t_e_p.tag;
    
    if (t_e.tag == TOURELLE)
        t_e.entity.tourelle = *t_e_p.entity.tourelle;

    else if (t_e.tag == ETUDIANT)
        t_e.entity.etudiant = *t_e_p.entity.etudiant;

    return t_e;
}



// TEMPORARY EFFECTS //////////////////////////////////////////////

// this function reset the temporary effects when their remaining time is 0
// decrement the effect remaining time
// save logs when reseting an effect
// apply the per round effect actions
static inline void manage_effects_on_entity(Tagged_entity_p entity, Tagged_entity_type * type, Log_storage * logs){

    TEMP_EFFECT * effects;
    TEMP_EFFECT effect;
    char * remaining_time;

    int normal_speed, normal_strength;
    int * speed, * strength;


    if ( entity.tag == TOURELLE ){
        Tourelle_type t_type = type->type.t_type;
        Tourelle * t = entity.entity.tourelle;
        effects = &t->effect;
        remaining_time = t->effect_remaining_time;
        normal_speed = 0;
        speed = NULL; 
        normal_strength = t_type.strength;
        strength = &t->strength;
    }

    else if ( entity.tag == ETUDIANT ) {
        Etudiant_type e_type = type->type.e_type;
        Etudiant * e = entity.entity.etudiant;
        effects = &e->effect;
        remaining_time = e->effect_remaining_time;
        normal_speed = e_type.vitesse;
        speed = &e->vitesse;
        normal_strength = e_type.strength;
        strength = &e->strength;
    }

    // decrement remaining times
    // remove the effect whose time is 0
    for (int i = 0; i < EFFECT_NO; i++){

        if ( remaining_time[i] == 1){

            // if remaining time is not 0, we have effect flag set in the effects flags (Tourelle.effect or Etudiant.effect)
            effect = get_tmp_effect_type(i);

            // toggle (to FALSE) the effect flag in TEMP_EFFECT * effects (proper entity effects)
            // bitwise xor ^ operator --> toggle
            *effects ^= effect;

            // SWITCH WITHOUT BREAKS
            switch (effect)
            {
            case SPEED:
                *speed = normal_speed;

            // HEALTH: NOTHING TO DO

            case STRENGTH:
                *strength = normal_strength;
            
            default:
                break;
            }
            
            save_log(ENTITY_EFFECT_REMOVE, t_entity_p_to_t_entity(entity), effect, logs);
        }

        // decrement the non zero remaining times
        if ( remaining_time[i] > 0 ) 
            remaining_time[i] --;
    }
}

void manage_effects(Log_storage * logs){

    Tagged_entity_p entity;
    Tagged_entity_type type;
    Etudiant_type * e_type = &type.type.e_type;
    Tourelle_type * t_type = &type.type.t_type;

    // TOURELLES
    entity.tag = TOURELLE;
    type.tag = TOURELLE;
    entity.entity.tourelle = game.tourelles;
    while (entity.entity.tourelle){
        *t_type = entity_type_get_type_by_id(&tourelle_types, entity.entity.tourelle->type)->type.t_type;
        manage_effects_on_entity(entity, &type, logs);
        entity.entity.tourelle = entity.entity.tourelle->next;
    }

    // ETUDIANTS
    entity.tag = ETUDIANT;
    type.tag = ETUDIANT;
    entity.entity.etudiant = game.etudiants;
    while (entity.entity.etudiant){
        *e_type = entity_type_get_type_by_id(&etudiant_types, entity.entity.etudiant->type)->type.e_type;
        manage_effects_on_entity(entity, &type, logs);
        entity.entity.tourelle = entity.entity.tourelle->next;
    }
}

void manage_per_round_effects(void){

    int initital_health;
    Etudiant * e = game.etudiants;
    Tourelle * t = game.tourelles;

    while (e){
        if ( e->effect & HEALTH ) {
            initital_health = entity_type_get_type_by_id(&etudiant_types, e->type)->type.e_type.pointsDeVie;
            e->pointsDeVie = min(initital_health, e->pointsDeVie + e->effect_values[get_effect_index(HEALTH)]);
        }
        e = e->next;
    }

    while (t){
        if ( t->effect & HEALTH) {
            initital_health = entity_type_get_type_by_id(&tourelle_types, t->type)->type.t_type.pointsDeVie;
            t->pointsDeVie = min(initital_health, t->pointsDeVie + t->effect_values[get_effect_index(HEALTH)]);
        }
        t = t->next;
    }

}



//  LOG SYSTEM FUNCTIONS: to keep track of the messages generated during the round resolution
//  Because we want to keep data of entities whose memory may be free() 

void save_log(LOG_TYPE log_type, Tagged_entity t_entity, int data, Log_storage * storage){

    Log_infos infos;
    infos.t_entity = t_entity;
    infos.type = log_type;
    infos.data = data;

    if (storage->count < storage->length) {
        storage->arr[storage->count] = infos;
        storage->count++; 
    }
    else
        fprintf(stderr, "Error: too many game logs in the Log_storage struct");
}

static char * effect_names[] = {
    "IMMOBILE",
    "REGENERATION",
    "FORCE",
    "VITESSE"
};

static char * log_format_strings[] = {
    "la tourelle '%s', ligne %d position %d, a ete detruite !\t%+d points !\n\n",
    "l'etudiant '%s',  ligne %d position %d, a ete elemine !\t%+d points !\n\n",
    "tous les etudiants ont ete elemines ! Vous avez gagne !\n\n",
    "l'etudiant '%s', a atteint sa derniere position sur la ligne %d !\n\nVous avez perdu !\n\n",
    "%s '%s', ligne %d, position %d, n'a plus l'effet '%s' !\n\n",
    "l'effet '%s' s'applique sur %s '%s', ligne %d, position %d (%+d) !\n\n",
};

void display_logs(Log_storage * storage){

    int i = 0;

    Tagged_entity t_entity;
    union Entity_type entity_type;
    int line, position;
    char * name;
    char * effect_values;
    char entity_name[12];

    int data;
    char * format_string;


    while (i < storage->count){

        format_string = log_format_strings[storage->arr[i].type];

        t_entity = storage->arr[i].t_entity;

        data = storage->arr[i].data;


        if ( t_entity.tag == ETUDIANT ) {
            entity_type = entity_type_get_type_by_id(&etudiant_types, t_entity.entity.etudiant.type)->type;
            line = t_entity.entity.etudiant.ligne;
            position = t_entity.entity.etudiant.position;
            name = entity_type.e_type.name;
            strcpy(entity_name, "l'etudiant");
            effect_values = t_entity.entity.etudiant.effect_values;
        }

        else if ( t_entity.tag == TOURELLE) {
            entity_type = entity_type_get_type_by_id(&tourelle_types, t_entity.entity.tourelle.type)->type;
            line = t_entity.entity.tourelle.ligne;
            position = t_entity.entity.tourelle.position;
            name = entity_type.t_type.name;
            strcpy(entity_name, "la tourelle");
            effect_values = t_entity.entity.tourelle.effect_values;

        }

        switch (storage->arr[i].type)
        {
        case DEAD_TOURELLE:
            printf(format_string, name, line, position, data);
            break;

        case DEAD_ETUDIANT:
            printf(format_string, name, line, position, data);
            break;

        case PLAYER_WIN:
            printf(format_string);
            break;

        case ETUDIANT_WIN:
            printf(format_string, name, line);
            break;

        case ENTITY_EFFECT_REMOVE:
            printf(format_string, entity_name, name, line, position, effect_names[get_effect_index(data)] );
            break;
        
        case ENTITY_EFFECT_APPLY:
            printf(format_string, effect_names[ get_effect_index(data) ], entity_name, name, line, position, effect_values[ get_effect_index(data)] );
            break;
        
        default:
            break;
        }

        i++;
    }
}



int etudiant_get_score(Etudiant_type e_type, int round_no){

    // moyenne pondérée 0.6 points de vie et 0.4 rapidité d'execution
    // tous les trois tours après l'apparition de l'ennemi, le bonus de rapidité d'exécution est divisé par 2
    // normalisation des ordres de grandeurs des pv
    double res = e_type.pointsDeVie * 0.6 * 100 + (1 / pow(2, (game.tour - round_no) / 3.0  )) * 0.4 * 100 ;

    // resultat arrondi à l'entier inférieur
    return round(res);
}

int tourelle_get_score(Tourelle_type t_type, int round_no){
    double res = -0.5 * ( t_type.pointsDeVie * 25 + (1 / pow(2, (game.tour - round_no) / 3.0  )) * 25 ) ;
    return round(res);
}
