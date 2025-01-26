#ifndef DISPLAY_H
#define DISPLAY_H

// affiche un tour de jeu 
void affiche_jeu(void);

//affiche un aperçu de la vague avant le début de la partie 
void affiche_vague(void);

//affiche l'aide aux commandes
void help(void);

//interprète les commandes utilisateur
void interInstru(int* gamestate,char instru[256]);

#endif