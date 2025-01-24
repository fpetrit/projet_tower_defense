#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "entity_types.h"
#include "entity_type_vector.h"
#include "../../preprocessor_macros_constants.h"

static int e_id = 0;
static int t_id = 0;

extern Entity_type_vector tourelle_types;
extern Entity_type_vector etudiant_types;
 
static inline void fill_etudiant_block(FILE * file, Etudiant_type * e_type){

    int length = 0;
    char str[LINE_MAX_CHAR_NO];

    memset(e_type, 0, sizeof(Etudiant_type));

    do {
        fgets(str, LINE_MAX_CHAR_NO, file);
    } while ( !feof(file) && ( *str == '\n' || *str == '/') ) ;
    
    length = strlen(str);
    if ( length > 0) {
        
        if (str[length - 1] == '\n'){
            str[length - 1] = '\0';
            length--;
        }

        if (length > 0) {

            e_type->id = e_id;
            e_id++;
        
            strcpy(e_type->name, str);

            fscanf(file, "%c", &e_type->abbr);

            fscanf(file, " %d", &e_type->pointsDeVie);

            fscanf(file, " %d", &e_type->damage_type);
    
            fscanf(file, " %d", &e_type->move_type);
        }

    }
}


static inline void fill_tourelle_block(FILE * file, Tourelle_type * t_type){

    int length = 0;
    char str[LINE_MAX_CHAR_NO];

    memset(t_type, 0, sizeof(Tourelle_type));

    do {
        fgets(str, LINE_MAX_CHAR_NO, file);
    } while ( !feof(file) && ( *str == '\n' || *str == '/') ) ;
    
    length = strlen(str);
    if ( length > 0) {
        
        if (str[length - 1] == '\n'){
            str[length - 1] = '\0';
            length--;
        }

        if (length > 0) {

            t_type->id = t_id;
            t_id++;
        
            strcpy(t_type->name, str);

            fscanf(file, "%c", &t_type->abbr);

            fscanf(file, " %d", &t_type->prix);

            fscanf(file, " %d", &t_type->pointsDeVie);

            fscanf(file, " %d", &t_type->damage_type);
    
            fscanf(file, " %d", &t_type->move_type);
        }

    }

}


void init_types(void){

    // 1) TOURELLE TYPES

    // this allocates memory and set the count & the size
    entity_type_vector_init(&tourelle_types);
    entity_type_vector_init(&etudiant_types);

    // buffer
    Tagged_entity_type tagged_type;
    union Entity_type * type = &tagged_type.type;


    FILE * tourelles_file = fopen("game/tourelles.txt", "r");
    FILE * etudiants_file = fopen("game/vilains.txt", "r");

    tagged_type.tag = ETUDIANT;
    while ( ! feof(etudiants_file) ) {
        fill_etudiant_block(etudiants_file, &type->e_type);
        entity_type_vector_append(&etudiant_types, tagged_type);
    }

    tagged_type.tag = TOURELLE;
    while ( ! feof(tourelles_file) ) {
        fill_tourelle_block(tourelles_file, &type->t_type);
        entity_type_vector_append(&tourelle_types, tagged_type);
    }

    fclose(tourelles_file);
    fclose(etudiants_file);


    // 2) ETUDIANT TYPES

}

