#ifndef GAME_H
#define GAME_H

#include <stdlib.h>
#include <stdio.h>

typedef struct tourelle {
    int type;
    int pointsDeVie;
    int ligne;
    int position;
    int prix;
    struct tourelle* next;
} Tourelle ;


typedef struct etudiant {
    int type;
    int pointsDeVie;
    int ligne;
    int position;
    int vitesse;
    int tour;
    struct etudiant* next;
    struct etudiant* next_line;
    struct etudiant* prev_line;
} Etudiant;


typedef struct {
    Tourelle* tourelles;
    Etudiant* etudiants;
    int cagnotte;
    int tour;
} Jeu;


void game_init(FILE * level);

#endif