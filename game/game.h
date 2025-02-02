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
 * @brief Flags variable type used by the function @ref etudiant_get_nearest_line and @ref tourelle_get_nearest_line .
 * A POS_FLAGS variable is passed by reference to these functions in which the flags are set.
 */
typedef enum {
    /// @brief Means that the passed position is equal to the returned @ref Tourelle or @ref Etudiant position.
    EQ_POS = 1 << 0,

    /// @brief Means that the passed position is strictly less than the returned @ref Tourelle or @ref Etudiant position.
    L_POS =  1 << 1,

    /// @brief Means that the passed position is strictly greater than the returned @ref Tourelle or @ref Etudiant position.
    G_POS =  1 << 2 
} POS_FLAGS;


/** @brief The number of values available for the @ref TEMP_EFFECT enum type.
 *  Used to initialize @ref Tourelle::effect_remaining_time and @ref Etudiant::effect_remaining_time and to manage these members in some functions.
 */ 
#define EFFECT_NO 4

/**
 * @brief Flags variable type used to know which temporary effects are applied to an @ref Etudiant or @ref Tourelle.
 */
typedef enum {
    
    /// @brief The speed of the entity is temporarily set to 0.
    FREEZE      = 1 << 0,

    /// @brief The entity recovers or loses HPs each round.
    HEALTH      = 1 << 1,

    /// @brief The strength of the entity is temporarily modified by a (possibly negative) increment value.
    STRENGTH    = 1 << 2,

    /// @brief The speed of the entity is temporarily modified by a (possibly negative) increment value.
    SPEED       = 1 << 3,

} TEMP_EFFECT;


/**
 * @brief Structure reprensenting the player's means of defense.
 * 
 * This structure is actually a node structure for a simply linked list and a doubly linked list of this type.
 * The simple linking is done according to the ascending Tourelle creation time order via @ref Tourelle::next .
 * The double linking is done according to the ascending Tourelle @ref Tourelle::position via @ref Tourelle::next_line and @ref Tourelle::prev_line.
 */
typedef struct tourelle {

    /// @brief Links the Tourelle with a @ref Tourelle_type, via @ref Tourelle_type::id.
    char type;

    /// @brief The player spend money to buy the Tourelle.
    int prix;
    
    /// @brief Health points.
    int pointsDeVie;

    /// @brief Tourelle's line on the game board.
    int ligne;

    /// @brief The position 0 is on the far right of the displayed game board.
    int position;

    /// @brief Used to compute how many damages can inflict the Tourelle when calling its attack function.
    int strength;

    /** @brief Used to store each applied effect remaining time in rounds number.
     * A mapping is done between the @ref TEMP_EFFECT enumerator values and their corresponding indices in this array by the @ref get_effect_index and @ref get_tmp_effect functions.
     */

    char effect_remaining_time[EFFECT_NO];

    /** @brief Used to store the values associated to each effect. Exemple: how many HPs the Entity recovers each round for the @ref TEMP_EFFECT::HEALTH effect.
     *  A mapping is done between the @ref TEMP_EFFECT enumerator values and their corresponding indices in this array by the @ref get_effect_index and @ref get_tmp_effect functions.
     */
    char effect_values[EFFECT_NO];

    /// @brief See @ref TMP_EFFECT .
    TEMP_EFFECT effect;
    
    /// @brief The round number during wich the Tourelle was created. Used to compute @ref Jeu::score decrements when a Tourelle is destroyed.
    int round_no;
    

    /// @brief A pointer to the subsequently created Tourelle, NULL when it does not exist one.
    struct tourelle* next;

    /// @brief A pointer to the previous Tourelle (inferior Tourelle::position, on the right on the game board) on the same @ref Tourelle::ligne "line".
    struct tourelle * next_line;

    /// @brief See @ref Tourelle::next_line .
    struct tourelle * prev_line;
} Tourelle ;


/// @brief Dynamically allocates memory for a new @ref Tourelle. Fill its member variables with initial values described by its @ref Tourelle::type "type".
/// @return A pointer to newly created Tourelle, or NULL if malloc failed.
/// @warning Do not use directly. Wrapper: @ref tourelle_add does additional checkings.
Tourelle * tourelle_create(int type, int ligne, int position);

/// @brief Includes prepended in the doubly linked list by line. Manage the @ref Tourelle::next_line and @ref Tourelle::prev_line members.
void tourelle_line_prepend(Tourelle * prepended, Tourelle * t);

/// @brief Same as @ref tourelle_create but append appended to t.
void tourelle_line_append(Tourelle * appended, Tourelle * t);

/// @brief Wrapper for the @ref tourelle_create function. Checks if the passed position is free. Manage both linkings.
/// @param error Reset to false and then to true if the passed position is already taken.
/// @return A pointer to the newly created @ref Tourelle if the position is free, else NULL.
Tourelle * tourelle_add(int type, int ligne, int position, bool * error);

/// Free the Tourelle dynamically allocated memory and manage both linking.
void tourelle_delete(Tourelle * t);



/**
 * @brief Structure representing the enemies.
 * 
 * For each member see the @ref Tourelle counterpart. 
 */
typedef struct etudiant {
    char type;
    int pointsDeVie;
    int ligne;
    int position;
    int vitesse;
    int strength;

    char effect_remaining_time[EFFECT_NO];
    char effect_values[EFFECT_NO];
    TEMP_EFFECT effect;

    /// @brief Ennemies are all specified in a text file loaded by the game. This variable indicates when (in round no) Etudiant should appear.
    int tour;

    struct etudiant* next;
    struct etudiant* next_line;
    struct etudiant* prev_line;

} Etudiant;


/// @brief See @ref tourelle_create.
/// @param abbr The abbreviation is passed rather than the id, because this is what is used in the txt level file.
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

/// @brief Set e as the head of the simply linked list.
void etudiant_insert(Etudiant * e);

/// @brief See @ref tourelle_append.
void etudiant_append(Etudiant * appended, Etudiant * e);

/// @brief Holds the game main objects (2 linked lists, the initial money amount, current round no...)
typedef struct {

    /// @brief @ref Tourelle simply linked list head.
    Tourelle* tourelles;

    /// @brief @ref Etudiant simply linked list head.
    Etudiant* etudiants;

    /// @brief The current maximum of the values contained in the @ref Etudiant::tour members of the created Etudiants.
    int etudiant_last_tour;

    /// @brief This value is updated each time a @ref Tourelle or @ref Etudiant is deleted.
    int score;
    int cagnotte;
    int tour;
    bool finished;
    bool won;
} Jeu;


/**
 * @brief Find the nearest @ref Tourelle from the passed position.
 * If there is a Tourelle on both sides & at the same distance, it returns the left one (greater position) as a priority.
 * @param The flags are reset and set accordingly to the description of @ref POS_FLAGS.
 * @warning Ignores the @ref Etudiant "Etudiants" on the game board.
 */
Tourelle * tourelle_get_nearest_line(int line, int position, POS_FLAGS * flags);

/**
 * @brief Find the nearest @ref Etudiant from the passed position.
 * If there is a Tourelle on both sides & at the same distance, it returns the left one (greater position) as a priority.
 * @param The flags are reset and set accordingly to the description of @ref POS_FLAGS.
 * @warning Ignores the @ref Tourelle "Tourelles" on the game board.
 */
Etudiant * etudiant_get_nearest_line(int line, int position, POS_FLAGS * flags);


/// Global variable defined in @ref main.c holding the game main objects.
extern Jeu game;


/**
 * @brief Initialize the game by reading the passed level text file.
 * 
 * This function is called by @ref menu in @ref main.c
 * It reads the file level text file and fills the \ref Jeu "game" external globally defined variable in @ref main.c, creating the described Etudiants.
 * It also fills two globally defined @ref Entity_type_vector variables in @ref main.c containing information about each type of @ref Tourelle or @ref Etudiant. 
 */
void game_init(FILE * level);


/** 
 * @brief The roud-wise game loop. Defines the order of the actions in the game.
*/
void next_round(void);

/**
 * @brief Responsible for freeing the heap memory before @ref main returns.
 */
void end_game(void);

#endif 