/**
 * @file main.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include "preprocessor_macros_constants.h"
#include "game/game.h"
#include "game/entity_types/entity_type_vector.h"

Jeu game;
Entity_type_vector tourelle_types;
Entity_type_vector etudiant_types;

// fonction wrapper basée sur une boucle permettant la navigation dans le menu
// cette fonction évoluera avec la complexité/les graphismes du menu
// elle est responsable d'ouvir le fichier de partie séléctionné et d'appeler la fonction game_init
void menu(){

    bool quit = false;
    char response;
    FILE * level;

    // 8 caractères, il en reste 20 pour les noms de niveau, null byte géré
    char filename[28];
    strcpy(filename, "levels/");
    char level_name[21];

    while ( ! quit ){

        // macro évaluée par le preprocesseur à réutiliser
        CLEAR

        response = 0;

        printf("Quel niveau souhaitez vous charger ?\nNom du niveau : ");
        scanf("%s", level_name);

        if ( strcat(filename, level_name) ){
            printf("%s\n", filename);
            level = fopen(filename, "r");
            if (level){
                game_init(level);
                fclose(level);
                quit = true;
            }
            else {
                perror("Erreur lors du chargement du niveau");
                sleep(2);
            }
        }

        while ( ! quit && response != 'o' && response != 'n'){
            CLEAR
            printf("Continuer ? [o/n]\n");
            scanf("%c", &response);
        }

        quit = response == 'n' || quit;

        strcpy(filename, "levels/");
    }

}

int main(void){

    menu();

    for (int i = 0; i < tourelle_types.count; i++ ){
        printf("%s\n", tourelle_types.arr[i].type.t_type.name);
    }

    game_end();
    return 0;
}