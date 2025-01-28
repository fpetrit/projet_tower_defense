#include <stdio.h>

#include "prompt.h"

void help(char * args){

    printf("COMMANDES :\n"
           "\t- 'stats <ligne, colonne>' :  affiche les informations de l'entite se trouvant à la position specifiee.\n"
           "\t- 'quit'  : termine la partie (echec).\n"
           "\t- 'place' : ouvre le menu permettant de placer une nouvelle tourelle.\n\n");
}




void stats(char * args);
void quit(char * args);
void place(char * args); 
