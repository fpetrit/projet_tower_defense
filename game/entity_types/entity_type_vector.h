#ifndef ENTITY_TYPE_VECTOR_H
#define ENTITY_TYPE_VECTOR_H

#include "../game.h"
#include "entity_types.h"
#include "entity_type_vector.h"

#define ENTITY_TYPE_INITIAL_LENGTH 20
#define NEW_LENGTH(N) 2*N+1

typedef struct {
    int count;
    int length;
    Tagged_entity_type * arr;
} Entity_type_vector;

void entity_type_vector_init(Entity_type_vector * t);

void entity_type_vector_free(Entity_type_vector * t);

void entity_type_vector_append(Entity_type_vector * tv, Tagged_entity_type t);


inline Tagged_entity_type * entity_type_get_type_by_id(Entity_type_vector * vect, int id){
    return &vect->arr[id];
}

Tagged_entity_type * entity_type_get_type_by_abbr(Entity_type_vector * vect, char abbr, Entity_type_tag tag){

    Tagged_entity_type * res = NULL;
    bool found = false;

    int i = 0;
    while ( i < vect->count && ! found ){

        switch (tag)
        {
        case ETUDIANT:
            found = vect->arr[i].type.e_type.abbr == abbr;
            break;

        case TOURELLE:
            found = vect->arr[i].type.t_type.abbr == abbr;
            break;
        
        default:
            break;
        }

        i++;
    }

    if (found)
        res = &vect->arr[i-1];

    return res;
}

extern Entity_type_vector tourelle_types;
extern Entity_type_vector etudiant_types;

#endif