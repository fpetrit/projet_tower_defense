#include "entity_type_vector.h"
#include "entity_types.h"

void entity_type_vector_vector_init(Entity_type_vector * t){
    t->count = 0;
    t->arr = malloc( sizeof(Tagged_entity_type) * ENTITY_TYPE_INITIAL_LENGTH);
    t->length = ENTITY_TYPE_INITIAL_LENGTH;
}

void entity_type_vector_vector_free(Entity_type_vector * t){
    free(t->arr);
}

void entity_type_vector_vector_append(Entity_type_vector * tv, Tagged_entity_type t){
    if (tv->count == tv->length){
        tv->arr = realloc(tv->arr, NEW_LENGTH(tv->length) * sizeof(Tagged_entity_type));
        tv->length = NEW_LENGTH(tv->length);
    }

    tv->arr[tv->count] = t;
    tv->count++;
}

inline Tagged_entity_type * entity_type_get_type_by_id(Entity_type_tag tag, int id){

    switch (tag)
    {
    case ETUDIANT:
        return &etudiant_types.arr[id];
        break;

    case TOURELLE:
        return &tourelle_types.arr[id];
        break;
    
    default:
        break;
    }
}

inline Tagged_entity_type * entity_type_get_type_by_abbr(Entity_type_tag tag, char abbr){

    Tagged_entity_type * res = NULL;
    Entity_type_vector * vect;
    bool found = false;

    switch (tag)
    {
    case ETUDIANT:
        vect = &etudiant_types;
        break;

    case TOURELLE:
        vect = &etudiant_types;
        break;

    default:
        fprintf(stderr, "Error: this entity tag is not supported, entity does not exist.\n");
        break;
    }

    int i = 0;
    while ( i < vect->count && ! found ){
        found = vect->arr[i].type.abbr == abbr;
        i++;
    }

    if (found)
        res = vect->arr[i-1];

    return res;
}



