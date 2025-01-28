#ifndef PROMPT_H
#define PROMPT_H

const char * prompt_commands[] = {
    "help",
    "stats",
    "quit",
    "place"
}

void help(char * args);
void stats(char * args);
void quit(char * args);
void place(char * args); 

void (* command_functions[]) (char *) = {
    help,
    stats,
    quit,
    place
}


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