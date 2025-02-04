#ifndef DISPLAY_H
#define DISPLAY_H

// display a round
void display_game(void);

//display a wave before the game starts
void display_wave(void);

//display list of commands
void help(void);

//interprets user commands
void prompt(void);

//creates a save file
void create_save(char nom[28]);

//loads a save file
int load_save(char nom[28]);

//save a new highscore
int save_s(char *nom);

//display highscores
void display_s(void);

//victory screen
void defeat(void);

//defeat screen
void victory(void);

#endif