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
    e_type->id = -1;

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

            fscanf(file, " %d", &e_type->vitesse);

            // strength
            fscanf(file, " %d", &e_type->strength);

            fscanf(file, " %d", &e_type->damage_type);
    
            fscanf(file, " %d", &e_type->move_type);
        }

    }
}


static inline void fill_tourelle_block(FILE * file, Tourelle_type * t_type){

    int length = 0;
    char str[LINE_MAX_CHAR_NO];

    memset(t_type, 0, sizeof(Tourelle_type));
    t_type->id = -1;

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

            // unique type id to refer the type
            t_type->id = t_id;
            t_id++;
        
            // name
            strcpy(t_type->name, str);

            // abbreviation
            fscanf(file, "%c", &t_type->abbr);

            // '\n'
            fgetc(file);

            // a description
            if (getc(file) == '[') {
                fscanf(file, " %[^]]", t_type->description);

                // ']'
                fgetc(file);
            }

            else
                *t_type->description = '\0';

            

            // price
            fscanf(file, " %d", &t_type->prix);


            // HPs
            fscanf(file, " %d", &t_type->pointsDeVie);

            // strength
            fscanf(file, " %d", &t_type->strength);

            // an integer damage type 
            fscanf(file, " %d", &t_type->damage_type);
    
            // an integer move type
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


    /**
     * La structure de ce code peut être améliorée si on a un grand nombre d'entités.
     * Une boucle basée sur le nombre de fichier "type d'entité" à aller scanner.
     * 
     */

    tagged_type.tag = ETUDIANT;
    while ( ! feof(etudiants_file) ) {
        fill_etudiant_block(etudiants_file, &type->e_type);
        if (type->e_type.id != -1)
            entity_type_vector_append(&etudiant_types, tagged_type);
    }

    tagged_type.tag = TOURELLE;
    while ( ! feof(tourelles_file) ) {
        fill_tourelle_block(tourelles_file, &type->t_type);
        if (type->e_type.id != -1)
            entity_type_vector_append(&tourelle_types, tagged_type);
    }

    fclose(tourelles_file);
    fclose(etudiants_file);


    // 2) ETUDIANT TYPES

}

