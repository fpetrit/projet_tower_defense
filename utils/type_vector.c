#include "type_vector.h"

extern type_vector types;

type_vector * type_vector_construct(void){
    type_vector * n = malloc(sizeof(type_vector));
    n->count = 0;
    n->arr = malloc( sizeof(type) * TYPE_VECTOR_INITIAL_LENGTH);
    n->length = TYPE_VECTOR_INITIAL_LENGTH;

    return n;
}

void type_vector_free(type_vector * t){
    free(t->arr);
    free(t);
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



