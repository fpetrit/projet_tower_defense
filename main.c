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
int menu(bool *end){

    bool quit = false, save=false;
    char response,response_1=0;
    FILE * level;
    char save_name[21];
    char filename[28];
    //cas save : 
    while (response != 'o' && response != 'n'){
        CLEAR
        printf("charger une sauvegarde ? [o/n]\n");
        scanf("%c", &response);
    }
    if (response=='o'){
        strcpy(filename, "saves/");
        
        while (!save){
        save=false;
        response_1=0;   
            printf("Quelle sauvegarde souhaitez vous charger ?\nNom du niveau : ");
            scanf("%s", save_name);
            if ( strcat(filename, save_name) ){
                printf("%s\n", filename);
                if (load_save(filename)){
                    save = true;
                    quit=true;
                }
                else {
                    perror("Erreur lors du chargement de la sauvegarde");
                    sleep(2);
                }
            }
            while ( ! save && response_1 != 'o' && response_1 != 'n'){
                CLEAR
                printf("Continuer ? [o/n]\n");
                scanf("%c", &response_1);
            }
            save = response_1 == 'n' || save;
            strcpy(filename, "saves/");
        }
        if (response_1=='n') *end=true;
        return 0;
    }  
    // 8 chararcers ("levels" + null byte), 20 available for level filename
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
    if (response=='n') *end=true;
    return 1;
}




/**
 * @brief Contains the main loop of the game in which are called @ref next_round and @ref prompt .
 */
int main(void){
    bool end=false;
    // display best scores for 5 secs
    display_s();
    sleep(2);
    CLEAR
    // open level file & initialize all the data
    int i=menu(&end);
    if (end==true) return 0;
    if (i==1){
        display_wave();
        char str[2];
        str[0] = '\0';

        printf("\n\nEntree pour commencer a jouer ...");
        getc(stdin);
        scanf("%c", str);
    }
    if (i==0){
        char str[2];
        str[0] = '\0';
        CLEAR
        printf("\n\nEntree pour commencer a jouer ...");
        getc(stdin);
        scanf("%c", str);
    }
    display_game();
    prompt();
    do {
        
        next_round();
        if (! game.finished){
            prompt();
        }
    } while (! game.finished );
    
    if (game.won){
        char response=0;
        while (response != 'o' && response != 'n'){
            printf("\n\nEnregistrer le score ? [o/n]\n");
            scanf("%c", &response);
        }
        if (response=='o'){
            char nom[64];
            printf("Nom du joueur : ");
            scanf("%s",nom);
            CLEAR
            save_s(nom);
        }
    }
    end_game();
    
    return 0;
}
