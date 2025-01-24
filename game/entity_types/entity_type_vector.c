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