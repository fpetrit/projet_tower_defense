#ifndef ENTITY_TYPES_H
#define ENTITY_TYPES_H

#include "../game.h"
#include "../../preprocessor_macros_constants.h"

typedef struct {
    
    int id;
    char name[30];
    char abbr;
    char description[TYPE_DESCRIPTONS_MAX_CHAR_NO + 1];
    int prix;
    int pointsDeVie;
    int damage_type;
    int move_type;

} Tourelle_type;

typedef struct {

    int id;
    char name[30];
    char abbr;
    int pointsDeVie;
    int vitesse;
    int damage_type;
    int move_type;

} Etudiant_type;

typedef enum {
    TOURELLE,
    ETUDIANT,
} Entity_type_tag ;

union Entity_type {
    Tourelle_type t_type;
    Etudiant_type e_type;
};

typedef struct {
    Entity_type_tag tag;
    union Entity_type type;
} Tagged_entity_type;

typedef union {
    Tourelle * tourelle;
    Etudiant * etudiant;
} Entity_p;

typedef union {
    Tourelle tourelle;
    Etudiant etudiant;
} Entity;

typedef struct {
    Entity_type_tag tag;
    Entity_p entity;
} Tagged_entity_p;

typedef struct {
    Entity_type_tag tag;
    Entity entity;
} Tagged_entity;

void init_types(void);

#endif