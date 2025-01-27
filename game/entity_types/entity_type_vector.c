#include "entity_type_vector.h"
#include "entity_types.h"

void entity_type_vector_init(Entity_type_vector * t){
    t->count = 0;
    t->arr = malloc( sizeof(Tagged_entity_type) * ENTITY_TYPE_INITIAL_LENGTH);
    t->length = ENTITY_TYPE_INITIAL_LENGTH;
}

void entity_type_vector_free(Entity_type_vector * t){
    free(t->arr);
}

void entity_type_vector_append(Entity_type_vector * tv, Tagged_entity_type t){
    if (tv->count == tv->length){
        tv->arr = realloc(tv->arr, NEW_LENGTH(tv->length) * sizeof(Tagged_entity_type));
        tv->length = NEW_LENGTH(tv->length);
    }

    tv->arr[tv->count] = t;
    tv->count++;
}



Tagged_entity_type * entity_type_get_type_by_id(Entity_type_vector * vect, int id){
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