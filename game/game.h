/**
 * @file game.h 
 * @brief Header for the game main structures and their associated functions.
 */


#ifndef GAME_H
#define GAME_H



#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>



typedef enum {
    EQ_POS = 1 << 0,
    L_POS =  1 << 1,
    G_POS =  1 << 2
} POS_FLAGS;



/**
 * @brief Structure reprensenting the player's means of defense.
 * 
 * This structure is a node of a simply linked list.
 * The linking is done in creation order.
 */
typedef struct tourelle {
    char type;
    int prix;
    
    int pointsDeVie;
    int ligne;
    int position;
    

    /// The linking pointer.
    struct tourelle* next;

    struct tourelle * next_line;
    struct tourelle * prev_line;
} Tourelle ;



Tourelle * tourelle_create(int type, int ligne, int position);

void tourelle_line_prepend(Tourelle * prepended, Tourelle * t);

void tourelle_line_append(Tourelle * appended, Tourelle * t);

Tourelle * tourelle_add(int type, int ligne, int position, bool * error);

/// Free the Tourelle dynamically allocated memory and manage the linking.
void tourelle_delete(Tourelle * t);



/**
 * @brief Structure representing the enemies in the game.
 * 
 * This struct is a node for a simply linked list.
 * The simple linking is done in order of appearance in the level text file.
 * Another double linking is done by line.
 */
typedef struct etudiant {
    char type;
    int pointsDeVie;
    int ligne;
    int position;
    int vitesse;

    /// Used to determine when to display the enemy.
    int tour;

    struct etudiant* next;
    struct etudiant* next_line;
    struct etudiant* prev_line;
} Etudiant;



Etudiant * etudiant_create(char abbr, int ligne, int position, int tour);

/** 
 * @brief Prepend an Etudiant to another in the doubly linked list of Etudiant.
 * @param prepended Its next_line member will point to e.
 * @param e Its prev_line pointer will point to prepended.
 * @warning Does not verify the line_no and position coherence.
 */
void etudiant_line_prepend(Etudiant * prepended, Etudiant * e);

/** 
 * @brief Append an Etudiant to another in the doubly linked list of Etudiant.
 * @param appended Its prev_line member will point to e.
 * @param e Its next_line pointer will point to appended.
 * @warning Does not verify the line_no and position coherence.
 */
void etudiant_line_append(Etudiant * appended, Etudiant * e);

/// Free the Etudiant dynamically allocated memory and manage the simple linking and double linking by line.
void etudiant_delete(Etudiant * e);

void etudiant_insert(Etudiant * e);

void etudiant_append(Etudiant * appended, Etudiant * e);

/// @brief Holds the game main objects (2 linked lists, initial money amount, current round no.).
typedef struct {
    Tourelle* tourelles;
    Etudiant* etudiants;
    int etudiant_last_tour;
    int score;
    int cagnotte;
    int tour;
    bool finished;
    bool won;
} Jeu;



Tourelle * tourelle_get_nearest_line(int line, int position, POS_FLAGS * flags);
Etudiant * etudiant_get_nearest_line(int line, int position, POS_FLAGS * flags);


/// @brief Global variable defined in @file ../main.c "main.c" holding the game main objects.
extern Jeu game;


/**
 * @brief Initialize the game by reading the level text file.
 * 
 * This function is called by @ref menu "menu()" in @file ../main.c "main.c"
 * It reads the file level text file and fills the \ref game "game" external globally defined variable in @file ../main.c "main.c".
 * It also fills two global variable arrays defined in @file ../main.c "main.c" containing information about each Etudiant/Tourelle type. 
 */
void game_init(FILE * level);

void start_game();

void end_game(void);

#endif 