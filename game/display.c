
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdbool.h>
#include <unistd.h>

#include "game.h"
#include "../preprocessor_macros_constants.h"
#include "entity_types/entity_type_vector.h"
extern Entity_type_vector tourelle_types;
extern Entity_type_vector etudiant_types;

void display_game(void){
    char L[ROWS][COLUMNS+5][5];
    Tourelle* t=game.tourelles;
    Etudiant* e=game.etudiants;
    CLEAR
    printf("Tour %d\n",game.tour);
    for (int i=0; i<ROWS;i++){ //creates an empty board in a 2D list of strings
        for (int j=0; j<COLUMNS+1;j++){
            if (j==0){
                sprintf(L[i][0],"%d|  ",i+1);
                L[i][1][0]=' ';
                L[i][1][1]=' ';
                L[i][1][2]='.';
                L[i][1][3]=' ';
                L[i][1][4]='\0';
            }
            else{
                L[i][j+1][0]=' ';
                L[i][j+1][1]=' ';
                L[i][j+1][2]='.';
                L[i][j+1][3]=' ';
                L[i][j+1][4]='\0';
            }
        }
    }
    while (e!=NULL){ // Puts Etudiants in the right spots of the list
        if (e->tour<=game.tour){
        sprintf(L[e->ligne-1][COLUMNS+1 - e->position]," %2d%c",e->pointsDeVie,entity_type_get_type_by_id(&etudiant_types, e->type)->type.e_type.abbr);
        }
        e=e->next;
    }
    while (t!=NULL){ // Puts Tourelles in the right spots of the list
        sprintf(L[t->ligne-1][COLUMNS+1 - t->position]," %2d%c",t->pointsDeVie,entity_type_get_type_by_id(&tourelle_types, t->type)->type.t_type.abbr);
        t=t->next;
    }
    for (int k=0;k<=ROWS-1;k++){ //prints the list
        for (int l=0;l<=COLUMNS;l++){
            printf("%s", L[k][l]);
        }
        printf("\n");
    }
    printf("\n");
}


void display_wave(void){
    char L[ROWS][game.etudiant_last_tour+5][5];;
    Etudiant* e=game.etudiants;
    CLEAR
    printf("Vague d'ennemis\n");
    for (int i=0; i<ROWS;i++){
        for (int j=0; j<game.etudiant_last_tour+1;j++){
            if (j==0){
                sprintf(L[i][0],"%d|  ",i+1);
                L[i][1][0]=' ';
                L[i][1][1]=' ';
                L[i][1][2]='.';
                L[i][1][3]=' ';
                L[i][1][4]='\0';
            }
            else{
                L[i][j+1][0]=' ';
                L[i][j+1][1]=' ';
                L[i][j+1][2]='.';
                L[i][j+1][3]=' ';
                L[i][j+1][4]='\0';
            }
        }
    }
    while (e!=NULL){
        sprintf(L[e->ligne-1][e->tour]," %2d%c",e->pointsDeVie,entity_type_get_type_by_id(&etudiant_types, e->type)->type.e_type.abbr);
        e=e->next;
    }
    for (int k=0;k<=ROWS-1;k++){
        for (int l=0;l<=game.etudiant_last_tour;l++){
            printf("%s", L[k][l]);
        }
        printf("\n");
    }
    printf("\n");
}



void create_save(char nom[28]){
    FILE* f=fopen(nom,"w");
    fprintf(f,"%d\n%d\n%d\n",game.cagnotte,game.score,game.tour);
    Tourelle* t=game.tourelles;
    Etudiant* e=game.etudiants;
    while (t!=NULL){
        fprintf(f,"%d %d %d %d\n",t->type,t->ligne,t->position,t->pointsDeVie);
        t=t->next;
    }
    fprintf(f,"\n"); //2nd line skip to differentiate between Tourelles and Etudiants
    while (e!=NULL){
        fprintf(f,"%d %d %d %d %c\n",e->ligne,e->position,e->tour,e->pointsDeVie,entity_type_get_type_by_id(&etudiant_types, e->type)->type.e_type.abbr);
        e=e->next;
    }
    fclose(f);
}


int load_save(char nom[28]){
    game.etudiants = NULL;
    game.tourelles = NULL;
    game.finished = false;
    game.etudiant_last_tour = 0;
    game.won = false;
    FILE* f=fopen(nom,"r");
    char t;
    int l,p,pv;
    Tourelle* T=NULL;
    bool error=false,error_1=false;
    init_types();
    char str[2];
    int round_no, line_no;
    char abbr ;
    if (!f){
        return 0;
    }
    fscanf(f,"%d\n%d\n%d",&game.cagnotte,&game.score,&game.tour);
    fscanf(f,"%c",&t);
    fscanf(f,"%c",&t);
    while(t!='\n'){ //tests if we've arrived to the line skip 
        fscanf(f," %d %d %d",&l,&p,&pv);
        T=tourelle_add(t-48,l,p,&error_1);
        printf("%c %d %d %d\n\n",t,l,p,pv); 
        T->pointsDeVie=pv;
        fscanf(f,"%c",&t);
        fscanf(f,"%c",&t);
    }
    Etudiant * current_last_etudiant_on_line[ROWS];
    memset(current_last_etudiant_on_line, 0, ROWS * sizeof(Etudiant *));

    Etudiant * prev_e = NULL;
    Etudiant * e;

    // create the first etudiant, initialize prev_e
    if (! feof(f) ){
        error = EOF == fscanf(f, " %d %d %d %d %c", &line_no, &p, &round_no, &pv ,&abbr);
        printf(" %d %d %d %d %c\n", line_no, p, round_no, pv ,abbr);
        // to chain Etudiants
        prev_e = etudiant_create(abbr , line_no, p, round_no);
        error = error || prev_e == NULL;

        if (!error) {
            etudiant_insert(prev_e);
            current_last_etudiant_on_line[prev_e->ligne - 1] = prev_e;
            prev_e->next_line = NULL;
            prev_e->pointsDeVie=pv;
        }
    
    }

    // CHAINING
    while ( ! feof(f) && ! error){

        // first simple chaining
        error = EOF == fscanf(f, " %d %d %d %d %c", &line_no, &p, &round_no, &pv ,&abbr);
        printf(" %d %d %d %d %c\n", line_no, p, round_no, pv ,abbr);
        if ( ! error )
            e = etudiant_create(abbr , line_no, p, round_no);
        error = error || e == NULL || prev_e->tour > round_no;
        
        if ( ! error ) {

            etudiant_append(e, prev_e);
            prev_e = e;
            e->pointsDeVie=pv;
            // if it is the first etudiant created for this line, he does not have next etudiant
            if (current_last_etudiant_on_line[prev_e->ligne - 1] == NULL){
                current_last_etudiant_on_line[prev_e->ligne - 1] = prev_e;
                prev_e->next_line = NULL;
            } else {
                current_last_etudiant_on_line[prev_e->ligne - 1]->prev_line = prev_e;
                prev_e->next_line = current_last_etudiant_on_line[prev_e->ligne - 1];
                current_last_etudiant_on_line[prev_e->ligne - 1] = prev_e;
            }

            if (prev_e->tour > game.etudiant_last_tour)
                game.etudiant_last_tour = prev_e->tour;

        } else if (prev_e != NULL) {
            fprintf(stderr, "Error: incorrect level text file format.\nFirst fields of each row must be ordered in ascending order.");
        } else {
            fprintf(stderr, "Error: something went wrong with malloc.");
        }

        fgets(str, 2, f);
    }

    // the last etudiant on each line has no previous etudiant on the same line
    for (int i = 0; i < ROWS; i++){
        // some lines may be empty
        if (current_last_etudiant_on_line[i])
            current_last_etudiant_on_line[i]->prev_line = NULL;
    }
    fclose(f);
    return 1;
}

int save_s(char *nom){
    FILE *f=fopen("scores.txt","r");
    int i,p=-1;
    char lines[10][64];
    int s[10];
    fscanf(f,"%d\n",&i);
    for (int j=0;j<i;j++){ //puts already save scores in a list 
        if (!feof(f)){
            fscanf(f,"%d %s\n",&s[j],lines[j]);
        }
    }
    fclose(f);
    for (int k=0;k<i;k++){ // finds the position of the new score in the list
        if (s[k]<=game.score){
            p=k;
            break;
        }
    }
    if (i<10){ //if we dont have 10 scores saved, we just add the new one at the right position 
        i++;
        
        if (p==-1){
            p=i-1;
        }
        FILE* f1=fopen("scores.txt","w");
        fprintf(f1,"%d\n",i);
        for (int j=0;j<p;j++){
            
            fprintf(f1,"%d %s\n",s[j],lines[j]);
        }
        fprintf(f1,"%d %s\n",game.score,nom);
        for (int k=p+1;k<i;k++){
            fprintf(f1,"%d %s\n",s[k-1],lines[k-1]);
        }
        fclose(f1);
    }

    else{ // else we get rid of the lowest one
        if (p==-1){ // case where the 10 scores already registred are better than the new one
            printf("le score n'est pas dans les 10 meilleurs\n");
            return 0;
        }
        FILE* f2=fopen("scores.txt","w");
        fprintf(f2,"%d\n",i);
        for (int j=0;j<p;j++){
            fprintf(f2,"%d %s\n",s[j],lines[j]);
        }
        fprintf(f2,"%d %s\n",game.score,nom);
        for (int k=p+1;k<10;k++){
            fprintf(f2,"%d %s\n",s[k-1],lines[k-1]);
        }    
        fclose(f2);
    }

    printf("Le score a bien ete enregistre\n");
    return 1;
}

void display_s(void){
    FILE* f=fopen("scores.txt","r");
    int i,j;
    char s[28];
    fscanf(f,"%d\n",&i);
    if (i==0) printf("Pas de scores sauvegardes");
    else{
        printf("Meilleurs scores :\n");
        for (int k=0;k<i;k++){
            fscanf(f,"%d %s\n",&j,s);
            printf("Joueur : %s     Score : %d\n",s,j);
        }
    }
    fclose(f);
}


void defeat(void){
    printf( "DDDDDDDDDDDDD      EEEEEEEEEEEEEEEEEEEEEEFFFFFFFFFFFFFFFFFFFFFFEEEEEEEEEEEEEEEEEEEEEE               AAA         TTTTTTTTTTTTTTTTTTTTTTT\n"
            "D::::::::::::DDD   E::::::::::::::::::::EF::::::::::::::::::::FE::::::::::::::::::::E              A:::A        T:::::::::::::::::::::T\n"
            "D:::::::::::::::DD E::::::::::::::::::::EF::::::::::::::::::::FE::::::::::::::::::::E             A:::::A       T:::::::::::::::::::::T\n"
            "DDD:::::DDDDD:::::DEE::::::EEEEEEEEE::::EFF::::::FFFFFFFFF::::FEE::::::EEEEEEEEE::::E            A:::::::A      T:::::TT:::::::TT:::::T\n"
            "  D:::::D    D:::::D E:::::E       EEEEEE  F:::::F       FFFFFF  E:::::E       EEEEEE           A:::::::::A     TTTTTT  T:::::T  TTTTTT\n"
            "  D:::::D     D:::::DE:::::E               F:::::F               E:::::E                       A:::::A:::::A            T:::::T\n"        
            "  D:::::D     D:::::DE::::::EEEEEEEEEE     F::::::FFFFFFFFFF     E::::::EEEEEEEEEE            A:::::A A:::::A           T:::::T\n"        
            "  D:::::D     D:::::DE:::::::::::::::E     F:::::::::::::::F     E:::::::::::::::E           A:::::A   A:::::A          T:::::T\n"        
            "  D:::::D     D:::::DE:::::::::::::::E     F:::::::::::::::F     E:::::::::::::::E          A:::::A     A:::::A         T:::::T\n"        
            "  D:::::D     D:::::DE::::::EEEEEEEEEE     F::::::FFFFFFFFFF     E::::::EEEEEEEEEE         A:::::AAAAAAAAA:::::A        T:::::T\n"        
            "  D:::::D     D:::::DE:::::E               F:::::F               E:::::E                  A:::::::::::::::::::::A       T:::::T\n"        
            "  D:::::D    D:::::D E:::::E       EEEEEE  F:::::F               E:::::E       EEEEEE    A:::::AAAAAAAAAAAAA:::::A      T:::::T\n"        
            "DDD:::::DDDDD:::::DEE::::::EEEEEEEE:::::EFF:::::::FF           EE::::::EEEEEEEE:::::E   A:::::A             A:::::A   TT:::::::TT\n"      
            "D:::::::::::::::DD E::::::::::::::::::::EF::::::::FF           E::::::::::::::::::::E  A:::::A               A:::::A  T:::::::::T\n"      
            "D::::::::::::DDD   E::::::::::::::::::::EF::::::::FF           E::::::::::::::::::::E A:::::A                 A:::::A T:::::::::T\n"      
            "DDDDDDDDDDDDD      EEEEEEEEEEEEEEEEEEEEEEFFFFFFFFFFF           EEEEEEEEEEEEEEEEEEEEEEAAAAAAA                   AAAAAAATTTTTTTTTTT\n");
    sleep(5);
}

void victory(void){
    printf( "VVVVVVVV           VVVVVVVVIIIIIIIIII      CCCCCCCCCCCCCTTTTTTTTTTTTTTTTTTTTTTT     OOOOOOOOO     RRRRRRRRRRRRRRRRR   YYYYYYY       YYYYYYY\n"
            "V::::::V           V::::::VI::::::::I   CCC::::::::::::CT:::::::::::::::::::::T   OO:::::::::OO   R::::::::::::::::R  Y:::::Y       Y:::::Y\n"
            "V::::::V           V::::::VI::::::::I CC:::::::::::::::CT:::::::::::::::::::::T OO:::::::::::::OO R::::::RRRRRR:::::R Y:::::Y       Y:::::Y\n"
            "V::::::V           V::::::VII::::::IIC:::::CCCCCCCC::::CT:::::TT:::::::TT:::::TO:::::::OOO:::::::ORR:::::R     R:::::RY::::::Y     Y::::::Y\n"
            " V:::::V           V:::::V   I::::I C:::::C       CCCCCCTTTTTT  T:::::T  TTTTTTO::::::O   O::::::O  R::::R     R:::::RYYY:::::Y   Y:::::YYY\n"
            "  V:::::V         V:::::V    I::::IC:::::C                      T:::::T        O:::::O     O:::::O  R::::R     R:::::R   Y:::::Y Y:::::Y\n"        
            "   V:::::V       V:::::V     I::::IC:::::C                      T:::::T        O:::::O     O:::::O  R::::RRRRRR:::::R     Y:::::Y:::::Y\n"        
            "    V:::::V     V:::::V      I::::IC:::::C                      T:::::T        O:::::O     O:::::O  R:::::::::::::RR       Y:::::::::Y\n"        
            "     V:::::V   V:::::V       I::::IC:::::C                      T:::::T        O:::::O     O:::::O  R::::RRRRRR:::::R       Y:::::::Y\n"        
            "      V:::::V V:::::V        I::::IC:::::C                      T:::::T        O:::::O     O:::::O  R::::R     R:::::R       Y:::::Y\n"        
            "       V:::::V:::::V         I::::IC:::::C                      T:::::T        O:::::O     O:::::O  R::::R     R:::::R       Y:::::Y\n"        
            "        V:::::::::V          I::::I C:::::C       CCCCCC        T:::::T        O::::::O   O::::::O  R::::R     R:::::R       Y:::::Y\n"        
            "         V:::::::V         II::::::IIC:::::CCCCCCCC::::C      TT:::::::TT      O:::::::OOO:::::::ORR:::::R     R:::::R       Y:::::Y\n"      
            "          V:::::V          I::::::::I CC:::::::::::::::C      T:::::::::T       OO:::::::::::::OO R::::::R     R:::::R    YYYY:::::YYYY\n"      
            "           V:::V           I::::::::I   CCC::::::::::::C      T:::::::::T         OO:::::::::OO   R::::::R     R:::::R    Y:::::::::::Y\n"      
            "            VVV            IIIIIIIIII      CCCCCCCCCCCCC      TTTTTTTTTTT           OOOOOOOOO     RRRRRRRR     RRRRRRR    YYYYYYYYYYYYY\n");
    sleep(5);
}

// PROMPT SYSTEM ////////////////////////////////////////////////////

void help(void){
    printf("\nCOMMANDES :\n"
           "\t- st | stats tourelle    affiche les stats d'un type de tourelle\n"
           "\t- se | stats etudiant    affiche les stats d'un type d'etudiant\n"
           "\t- p  | place             initie le placement d'une tourelle\n"
           "\t- end                    termine la partie sur une defaite\n"
           "\t- save                   enregistre l'etat de la partie\n"
           "\t- <Entree>               passe au tour suivant\n\n");
}

void tourelle_stats(void){
    char c;
    int i=0;

    printf("\nAbbreviations existantes :\n");
    while (i<tourelle_types.count){
        printf("%c ",tourelle_types.arr[i].type.t_type.abbr);
        i++;
    }

    printf("\n\nDonnez l'abbreviation du type de la tourelle : ");
    scanf("%c",&c);

    // remove the '\n' from the buffer otherwise will not prompt again
    fgetc(stdin);

    Tagged_entity_type * type = entity_type_get_type_by_abbr(&tourelle_types, c, TOURELLE);
    Tourelle_type t_type;

    if (type){
        t_type = type->type.t_type;
        printf("\nType : %d --- %s\n%s\nForce : %d\nPV : %d\nPrix : %d\n\n",
        t_type.id, t_type.name, t_type.description, t_type.strength, t_type.pointsDeVie, t_type.prix);
    }

    else
        printf("Abbreviation non reconnue.\n\n");
}

void etudiant_stats(void){
    int i=0;
    char c;

    printf("\nAbbreviations existantes :\n");
    while (i<etudiant_types.count){
        printf("%c ", etudiant_types.arr[i].type.e_type.abbr);
        i++;
    }

    printf("\n\nDonnez l'abbreviation du type de l'etudiant : ");
    scanf("%c",&c);

    // remove the '\n' from the buffer otherwise will not prompt again
    fgetc(stdin);

    Tagged_entity_type * type = entity_type_get_type_by_abbr(&etudiant_types, c, ETUDIANT);
    Etudiant_type e_type;

    if (type){
        e_type = type->type.e_type;
        printf("\nType : %d --- %s\nForce : %d\nPV : %d\nVitesse : %d\n\n",
        e_type.id, e_type.name, e_type.strength, e_type.pointsDeVie, e_type.vitesse);
    }

    else
        printf("Abbreviation non reconnue.\n\n");
}

void place_tourelle(void){

    int line, position;
    char c;
    bool error;
    int i = 0;
    Tourelle_type t_type;
    
    printf("\nTourelles disponibles :\n");
    while (i < tourelle_types.count){
        
        t_type = entity_type_get_type_by_id(&tourelle_types, i)->type.t_type;

        printf("\n%c --- %s :\n%s\nDegats : %d\nPV : %d\nPrix : %d\n\n",
        t_type.abbr, t_type.name, t_type.description, t_type.strength, t_type.pointsDeVie, t_type.prix);
        
        i++;
    }

    printf("\nSaisissez l'abbreviation de la tourelle : ");
    scanf("%c",&c);

    // remove the '\n' from the buffer otherwise will not prompt again
    getc(stdin);

    Tagged_entity_type * type = entity_type_get_type_by_abbr(&tourelle_types, c, TOURELLE);
    
    if (type){
        t_type = type->type.t_type;

        if (game.cagnotte < t_type.prix){
            printf("\nPas assez d'argent.\n\n");
        }

        else {
            game.cagnotte -= t_type.prix;

            printf("Saisir la ligne de la tourelle : ");
            scanf("%d",&line);

            printf("Saisir la colonne de la tourelle : ");
            scanf("%d",&position);

            printf("\n");

            getc(stdin);

            if (0 < position && position <= COLUMNS && 0 < line && line <= ROWS) {

                Tourelle * new = tourelle_add(t_type.id, line, position, &error);

                if ( error )
                    printf("Une entitee occupe deja cette position !\n\n");
                else if (! new)
                    fprintf(stderr, "Error: PlaceTour malloc failed in tourelle_add\n\n");

                else {
                    display_game();
                }
            }

            else {
                printf("Position invalide.\n\n");
                fgetc(stdin);
            }   
        }
    }

    else {
        printf("Abbreviation non reconnue.\n\n");
    }


}

void end(void){
    game.finished = true;
    game.won = false;
}

void save(void){
    char filename[28];
    strcpy(filename, "saves/");
    char save_name[21];
    printf("nom de fichier de sauvegarde : ");
    scanf("%s",save_name); 
    fgetc(stdin);
    create_save(strcat(filename,save_name));
}

#define COMMANDS_NO 6

static const char * commands[] = {
    "help",
    "st",
    "se",
    "p",
    "end",
    "save",
};

const static void (* prompt_functions[]) (void) = {
    help,
    tourelle_stats,
    etudiant_stats,
    place_tourelle,
    end,
    save,
};

void prompt(void){

    char command[50];
    int len;

    bool quit = false;

    while ( ! quit ){

        printf("game > ");

        // 50 - 1 bytes are read max
        fgets(command, 50, stdin);
        len  = strlen(command);

        if (len > 1 && command[len - 1] == '\n'){
            command[len - 1] = '\0';
            len --;
        }

        else if (len == 1 && command[0] == '\n'){
            quit = true;
        }

        else {
            printf("Error in prompt\n");
            quit = true;
        }

        if (! quit ){

            int i = 0;
            while (i < COMMANDS_NO && strcmp(command, commands[i]) ){
                i++;
            }

            // i is the index in the commands array if found
            // else it is equal to COMMANDS_NO

            if (i == COMMANDS_NO)
                printf("Commande non reconnue.\n\n");
            
            else
                prompt_functions[i]();
        }
    }
}
