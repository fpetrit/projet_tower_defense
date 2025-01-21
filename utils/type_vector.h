#ifndef TYPE_VECTOR_H
#define TYPE_VECTOR_H

#include "../game/game.h"

#define TYPE_VECTOR_INITIAL_LENGTH 20
#define NEW_LENGTH(N) 2*N+1

typedef struct {
    int count;
    int length;
    type * arr;
} type_vector;

type_vector * type_vector_construct(void);

void type_vector_free(type_vector * t);

void * type_vector_append(type_vector * tv, type t);

type * get_type(int id);

#endif