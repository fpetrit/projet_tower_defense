#include "type_vector.h"

extern type_vector types;

void type_vector_construct(type_vector * t){
    t->count = 0;
    t->arr = malloc( sizeof(type) * TYPE_VECTOR_INITIAL_LENGTH);
    t->length = TYPE_VECTOR_INITIAL_LENGTH;
}

void type_vector_free(type_vector * t){
    free(t->arr);
}

void type_vector_append(type_vector * tv, type t){
    if (tv->count == tv->length){
        tv->arr = realloc(tv->arr, NEW_LENGTH(tv->length));
        tv->length = NEW_LENGTH(tv->length);
    }

    tv->arr[tv->count] = t;
    tv->count++;
}

inline type * get_type(int id){
    return &types.arr[id];
}



