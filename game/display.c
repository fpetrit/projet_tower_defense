
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "../preprocessor_macros_constants.h"
#include "entity_types/entity_type_vector.h"
extern Entity_type_vector tourelle_types;
extern Entity_type_vector etudiant_types;

int affiche_jeu(void){
    char L[ROWS][COLUMNS+5][5];
    Tourelle* t=game.tourelles;
    Etudiant* e=game.etudiants;
    CLEAR
    printf("Tour %d\n",game.tour);
    for (int i=0; i<ROWS;i++){
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
    while (e!=NULL){
        if (e->tour<=game.tour){
        sprintf(L[e->ligne-1][COLUMNS+1 - e->position]," %2d%c",e->pointsDeVie,entity_type_get_type_by_id(&etudiant_types, e->type)->type.e_type.abbr);
        }
        e=e->next;
    }
    while (t!=NULL){
        sprintf(L[t->ligne-1][COLUMNS+1 - t->position]," %2d%c",t->pointsDeVie,entity_type_get_type_by_id(&tourelle_types, t->type)->type.t_type.abbr);
        t=t->next;
    }
    for (int k=0;k<=ROWS-1;k++){
        for (int l=0;l<=COLUMNS;l++){
            printf(L[k][l]);
        }
        printf("\n");
    }
    printf("\n");
}


void affiche_vague(void){ //affiche la vague avant le début des tours 
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
        sprintf(L[e->ligne-1][game.etudiant_last_tour+1 - e->tour]," %2d%c",e->pointsDeVie,entity_type_get_type_by_id(&etudiant_types, e->type)->type.e_type.abbr);
        e=e->next;
    }
    for (int k=0;k<=ROWS-1;k++){
        for (int l=0;l<=game.etudiant_last_tour;l++){
            printf(L[k][l]);
        }
        printf("\n");
    }
    printf("\n");
}

void help(void){
    printf("\nCOMMANDES :\n"
           "\t- StatTour    affiche les stats d'une tour <ligne> <position>\n"
           "\t- StatEtu     idem pour les étudiants\n"
           "\t- end         termine la partie sur une defaite\n"
           "\t- PlaceTour   initie le placement d'une tour\n"
           "\t- <Entree>    passe au tour suivant\n\n");
}

void creer_save(char nom[28]){
    FILE* f=fopen(nom,"w");
    fprintf(f,"%d\n%d\n%d\n",game.cagnotte,game.score,game.tour);
    Tourelle* t=game.tourelles;
    Etudiant* e=game.etudiants;
    while (t!=NULL){
        fprintf(f,"%d %d %d %d\n",t->type,t->ligne,t->position,t->pointsDeVie);
        t=t->next;
    }
    fprintf(f,"\n"); //2e saut de ligne pour différencier tourelles et ennemis
    while (e!=NULL){
        fprintf(f,"%d %d %d %d %d\n",e->type,e->ligne,e->position,e->tour,e->pointsDeVie);
        e=e->next;
    }
    fclose(f);
}


int charge_save(char nom[64]){
    game.etudiants = NULL;
    game.tourelles = NULL;
    game.finished = false;
    game.etudiant_last_tour = 0;
    game.won = false;
    FILE* f=fopen(nom,"r");
    char t;
    int l,p,pv;
    Tourelle* T=NULL;
    bool error=false;
    init_types();
    char str[2];
    int round_no, line_no;
    char abbr ;
    if (!f){
        printf("La sauvegarde %s n'existe pas",nom);
        return 0;
    }
    fscanf(f,"%d\n%d\n%d\n",&game.cagnotte,&game.score,&game.tour);
    fscanf(f,"%c",&t);
    while(t!='\n'){
        fscanf(f," %d %d %d\n",&l,&p,&pv);
        T=tourelle_add(t-48,l,p,0);
        T->pointsDeVie=pv;
        fscanf(f,"%c",&t);
    }
    Etudiant * current_last_etudiant_on_line[ROWS];
    memset(current_last_etudiant_on_line, 0, ROWS * sizeof(Etudiant *));

    Etudiant * prev_e = NULL;
    Etudiant * e;
    if (! feof(f) ){
        fscanf(f, "%c %d %d %d %d",&abbr, &line_no, &p, &round_no, &pv );
        // to chain Etudiants
        if(round_no>game.etudiant_last_tour) game.etudiant_last_tour=round_no;
        prev_e = etudiant_create(abbr , line_no, p, round_no);
        error = prev_e == NULL;

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
        fscanf(f, "%c %d %d %d %d",&abbr, &line_no, &p, &round_no, &pv );
        if(round_no>game.etudiant_last_tour) game.etudiant_last_tour=round_no;
        e = etudiant_create(abbr , line_no, p, round_no);
        
        // erreur si les champs "tour" des lignes ne sont dans l'ordre croissant
        // nécessaire pour l'algo de chaînage double par ligne
        // erreur également si malloc n'a pas fonctionné
        error = e == NULL || prev_e->tour > round_no;
        
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

        } else if (prev_e != NULL) {
            fprintf(stderr, "Error: incorrect level text file format.\nFirst fields of each row must be ordered in ascending order.");
        } else {
            fprintf(stderr, "Error: something went wrong with malloc.");
        }

        fgets(str, 2, f);
    }

    // the last etudiant on each line has no previous etudiant on the same line
    for (int i = 0; i < ROWS; i++){
        current_last_etudiant_on_line[i]->prev_line = NULL;
    }
}

int prompt(void){ 
    char instru[256];
    int len;
    printf("\ngame > ");
    fgets(instru, 256, stdin);
    len = strlen(instru);

    if ( len > 1)
        instru[len - 1] = '\0';


    if (!strcmp(instru,"help")){
        help();
    }
    else if (!strcmp(instru,"StatTour")){
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

        Tourelle_type t_type = entity_type_get_type_by_abbr(&tourelle_types, c, TOURELLE)->type.t_type;
        printf("\nType : %d --- %s\n%s\nForce : %d\nPV : %d\nPrix : %d\n\n",
        t_type.id, t_type.name, t_type.description, t_type.strength, t_type.pointsDeVie, t_type.prix);
    }

    else if (!strcmp(instru,"StatEtu")){
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

        Etudiant_type e_type = entity_type_get_type_by_abbr(&etudiant_types, c, ETUDIANT)->type.e_type;
        printf("\nType : %d --- %s\nForce : %d\nPV : %d\nVitesse : %d\n\n",
        e_type.id, e_type.name, e_type.strength, e_type.pointsDeVie, e_type.vitesse);
    }

    else if (!strcmp(instru,"end")){
        game.finished=1;
        return 0;
    }
    else if (!strcmp(instru,"PlaceTour")){
        int line, position, type;
        bool error;
        int i=0;
        printf("\nTourelles disponibles :\n");
        while (i<tourelle_types.count){
            printf("\nType %d --- %s :\n%s\nDegats : %d, PV : %d, prix : %d\n",tourelle_types.arr[i].type.t_type.id, tourelle_types.arr[i].type.t_type.name, tourelle_types.arr[i].type.t_type.description, tourelle_types.arr[i].type.t_type.damage_type, tourelle_types.arr[i].type.t_type.pointsDeVie, tourelle_types.arr[i].type.t_type.prix);
            i++;
        }
        printf("\nSaisir le type de la tourelle : ");
        scanf("%d",&type);
        if (game.cagnotte<entity_type_get_type_by_id(&tourelle_types, type)->type.t_type.prix){
            printf("\nPas assez d'argent.");
        }
        else{
            game.cagnotte-=entity_type_get_type_by_id(&tourelle_types, type)->type.t_type.prix;
            printf("Saisir la ligne de la tourelle : ");
            scanf("%d",&line);
            printf("Saisir la colonne de la tourelle : ");
            scanf("%d",&position);
            Tourelle * new = tourelle_add(type, line, position, &error);
            if ( error )
                printf("Une entitee occupe deja cette position !\n");
            else if (! new)
                fprintf(stderr, "Error: PlaceTour malloc failed in tourelle_add");
        }
    }
    else if (!strcmp(instru,"save")){
        char filename[28];
        strcpy(filename, "saves/");
        char save_name[21];
        printf("nom de fichier de sauvegarde : ");
        scanf("%s",save_name); 
        creer_save(strcat(filename,save_name));
    }
    else if (!strcmp(instru,"\n")){
        return 1;
    }
    else{
        printf("Instruction non reconnue.\n");
    }
    prompt();
}
