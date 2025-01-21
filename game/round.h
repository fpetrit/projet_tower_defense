#ifndef ROUND_H
#define ROUND_H

#include <stdio.h>
#include <stdlib.h>

#include "game.h"

enum damage_type {
    basic,
    zone,
};

enum movement_type {
    basic,
};

/**
 * @brief Applies the damages that a Tourelle can inflict to Etudiant s during a round.
 * 
 * The behaviour of the function (the zone in which Etudiant s receive damages, how much, etc...)
 * is personalized thanks to the Tourelle.type.
 */
void inflict_damage(Tourelle * t);

/**
 * @brief Applies the damages that an Etudiant can inflict to Tourelle s during a round.
 * 
 * The behaviour of the function (the zone in which Tourelle s receive damages, how much, etc...)
 * is personalized thanks to the Etudiant.type.
 */
void inflict_damage(Etudiant * e);

#endif ROUND_H