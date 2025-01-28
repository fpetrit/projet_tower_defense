#ifndef ROUND_H
#define ROUND_H

#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "entity_types/entity_types.h"
#include "entity_types/entity_type_vector.h"

typedef union {
    Tourelle * tourelle;
    Etudiant * etudiant;
} Entity;

typedef struct {
    Entity_type_tag tag;
    Entity entity;
} Tagged_entity;

typedef enum {
    DEAD,
    DAMAGE_INFLICTED
} LOG_TYPE;
 
void inflict_damage(Tagged_entity * entity);

void move(Tagged_entity * entity);

void update_round(void);

// void log(LOG_TYPE log_t, Entity * a, Entity * b);

#endif