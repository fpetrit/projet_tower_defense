#ifndef ROUND_H
#define ROUND_H

#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "entity_types/entity_types.h"
#include "entity_types/entity_type_vector.h"
#include "../preprocessor_macros_constants.h"
 
void inflict_damage(Tagged_entity_p * entity);

void move(Tagged_entity_p * entity);

void next_round(void);

int etudiant_get_score(Etudiant_type e_type, int round_no);
int tourelle_get_score(Tourelle_type t_type, int round_no);

typedef enum {
    DEAD_TOURELLE,
    DEAD_ETUDIANT,
    PLAYER_WIN,
    ETUDIANT_WIN
} LOG_TYPE;

typedef struct {
    LOG_TYPE type;
    Tagged_entity t_entity;
    int data;
} Log_infos ;

typedef struct {
    int count;
    int length;
    Log_infos arr[LOGS_MAX_NO];
} Log_storage ;

void save_log(LOG_TYPE log_type, Tagged_entity t_entity, int data, Log_storage * storage);
void display_logs(Log_storage * storage);



#endif