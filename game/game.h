/**
 * @file game.h 
 * @brief Header for the game main structures and their associated functions.
 */


#ifndef GAME_H
#define GAME_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


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
} Tourelle ;


/**
 * @brief Dynamically allocates memory for a new Tourelle and links it with the one passed as an argument.
 * 
 * The initial value of pointsDeVie and prix are determined by the type.
 * @param t This Tourelle "next" member will point to the new one.
 * @return True if successful, else false.
 */
bool tourelle_append(Tourelle * t, int type, int ligne, int position);


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


/**
 * @brief Dynamically allocates memory for a new Etudiant and set it as the first node of the linked list.
 */
Etudiant * edtudiant_insert(int type, int ligne, int position, int tour);


/**
 * @brief Dynamically allocates memory for a new Etudiant and links it with the one passed as an argument.
 * 
 * This function only manage the simple linking of the Etudiant node in order of apparition in the file.
 * The double linking by line is done etudiant_line_prepend and etudiant_line_append.
 * The initial value of pointsDeVie and prix are determined by the type.
 * 
 * @param e This Etudiant "next" member will points to the new one.
 * @return A pointer on the created Etudiant, NULL if the dynamic allocation failed.
 */
Etudiant * etudiant_append(Etudiant * e, int type, int ligne, int position, int tour);


/** 
 * @brief Prepend an Etudiant to another in the doubly linked list of Etudiant.
 * @param prepended Its next_line member will point to e.
 * @param e Its prev_line pointer will point to prepended.
 */
void etudiant_line_prepend(Etudiant * prepended, Etudiant * e);


/** 
 * @brief Append an Etudiant to another in the doubly linked list of Etudiant.
 * @param appended Its prev_line member will point to e.
 * @param e Its next_line pointer will point to appended.
 */
void etudiant_line_append(Etudiant * appended, Etudiant * e);


/// Free the Etudiant dynamically allocated memory and manage the simple linking and double linking by line.
void etudiant_delete(Etudiant * e);


/// @brief Holds the game main objects (2 linked lists, initial money amount, current round no.).
typedef struct {
    Tourelle* tourelles;
    Etudiant* etudiants;
    int cagnotte;
    int tour;
} Jeu;


typedef struct {

} etudiant_type;

typedef struct {

} tourelle_type;


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
void affiche_jeu(void);
void affiche_vague(void);
#endif 