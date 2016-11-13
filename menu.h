/**
 * \file menu.h
 * \brief Fichier permettant la définition des structures et des prototypes.
 * \author PETIT Sylvain, FALLER Romain, FIRRINCIELI Arnaud
 * \version 3.0
 * \date Janvier 2015
 *
 * Ce fichier permet de définir les structures (ou énumérations) utilisées dans menu.c ainsi que les prototypes des fonctions utilisées dans menu.c.
 *
 */


#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

/**
 * \struct Struct_Btn_menu
 * \brief Structure correspondante aux boutons des différents menus.
 * Cette structure permet de définir l’ensemble des textures (et leur position) correspondantes aux boutons des menus
 * principaux (Menu principal, Menu « Jouer ou charger», Menu « Choix du mode »).
 */
typedef struct Struct_Btn_menu
{
    SDL_Texture *jouer;
    SDL_Rect jouer_r;
    SDL_Texture *quitter;
    SDL_Rect quitter_r;
    SDL_Texture *regles;
    SDL_Rect regles_r;

    SDL_Texture *nouvellepartie;
    SDL_Rect nouvellepartie_r;

    SDL_Texture *demo;
    SDL_Rect demo_r;

    SDL_Texture *jcj;
    SDL_Rect jcj_r;
    SDL_Texture *jcia;
    SDL_Rect jcia_r;

    SDL_Texture *facile;
    SDL_Rect facile_r;
    SDL_Texture *moyen;
    SDL_Rect moyen_r;
    SDL_Texture *difficile;
    SDL_Rect difficile_r;


    SDL_Texture *retour;
    SDL_Rect retour_r;

} Struct_Btn_menu;



/**
 * \struct Struct_Btn_regles
 * \brief Structure correspondante aux boutons des règles.
 * Cette structure permet de définir l’ensemble des textures (et leur position) correspondantes
 * aux boutons permettant de parcourir les différents affichages des règles du jeu (suivant, précédent, retour).
 */
typedef struct Struct_Btn_regles
{
    SDL_Texture *suivant;
    SDL_Rect suivant_r;

    SDL_Texture *precedent;
    SDL_Rect precedent_r;

    SDL_Texture *retour;
    SDL_Rect retour_r;

    SDL_Texture *fondRegles1;
    SDL_Texture *fondRegles2;
    SDL_Texture *fondRegles3;
    SDL_Texture *fondRegles4;
} Struct_Btn_regles;

// Structures
Struct_Btn_regles Btn_regles;
Struct_Btn_menu Btn_menu;

void initButtons();
void drawBoutons(int menuActuel);
void gestionEventsBouton(int *menuActuel, bool *continuer, int x, int y);
void cleanup();


/**
 * \enum menu
 * \brief Constantes représentants les différents menus (ou pages pour les règles).
 *
 * Cette énumération permet d’attribuer un entier aux différents menus présents (différentes pages d’affichage des menus et règles...).
 */
enum menu {MENU_PRINCIPAL, MENU_JOUER, MENU_REGLES, MENU_NOUVELLEPARTIE, MENU_CHARGER, MENU_JCIA, JEU_JCJ,REGLES_P2,REGLES_P3,REGLES_P4};

#endif // MENU_H_INCLUDED
