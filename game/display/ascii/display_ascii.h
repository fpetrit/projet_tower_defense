#ifndef DISPLAY_ASCII_H
#define DISPLAY_ASCII_H

void display_ascii_invasion(void);

void display_ascii_current_game_state(void);

/**
 * Commands available in the game prompt:
 * 
 * help
 * stats <row> <column> 
 * quit (asks for save)
 * place (open the shop, display all the tourelle types, display game.cagnotte, highlight tourelle prices)
 * <ENTER> exit the prompt and start the next round
 * 
 */
void display_ascii_prompt(void);





#endif