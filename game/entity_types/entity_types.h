#ifndef ENTITY_TYPES
#define ENTITY_TYPES

typedef struct {
    
    int id;
    char name[30];
    char abbr;
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

#endif