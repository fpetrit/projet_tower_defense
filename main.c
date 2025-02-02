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
#include "game/display.h"
#include "game/round.h"

Jeu game;
Entity_type_vector tourelle_types;
Entity_type_vector etudiant_types;
Log_storage logs;

/**
 * @brief Prompt the user for a level filename, open it and initialize a game with it by calling @ref game_init
 */
void menu(){

    bool quit = false;
    char response;
    FILE * level;

    // 8 chararcers ("levels" + null byte), 20 available for level filename
    char filename[28];
    strcpy(filename, "levels/");
    char level_name[21];

    while ( ! quit ){

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



/**
 * @brief Contains the main loop of the game in which are called @ref next_round and @ref prompt .
 */
int main(void){

    // open level file & initialize all the data
    menu();
    
    affiche_vague();
    char str[2];
    str[0] = '\0';

    printf("\n\nEntree pour commencer à jouer ...");
    getc(stdin);
    scanf("%c", str);

    affiche_jeu();
    prompt();

    do {
        
        next_round();
        if (! game.finished)
            prompt();

    } while (! game.finished );
    

    end_game();
    
    return 0;
}
