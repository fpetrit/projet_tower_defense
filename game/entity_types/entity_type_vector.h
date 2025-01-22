#ifndef ENTITY_TYPE_VECTOR_H
#define ENTITY_TYPE_VECTOR_H

#include "../game/game.h"

#define ENTITY_TYPE_INITIAL_LENGTH 20
#define NEW_LENGTH(N) 2*N+1

typedef struct {
    int count;
    int length;
    Tagged_entity_type * arr;
} Entity_type_vector;

void entity_type_vector_init(Entity_type_vector * t);

void entity_type_vector_free(Entity_type_vector * t);

void entity_type_vector_append(Entity_type_vector * tv, Tagged_entity_type * t);

inline Tagged_entity_type * entity_type_get_type_by_id(Entity_type_tag tag, int id);

inline Tagged_entity_type * entity_type_get_type_by_abbr(Entity_type_tag tag, int id);

extern Entity_type_vector tourelle_types;
extern Entity_type_vector etudiant_types;

#endif