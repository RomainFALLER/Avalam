/**
 * \file defs.h
 * \brief Fichier permettant l'incorporation de contenus de bibliothèques (SDL2,SDL2_ttf, SDL2_image) ainsi qu'aux autres fichiers .h du programme.
 * \author PETIT Sylvain, FALLER Romain, FIRRINCIELI Arnaud
 * \version 3.0
 * \date Janvier 2015
 *
 * Ce fichier permet d'incorporer au programme principal les différentes librairies utilisées telles que SDL2, SDL2_ttf, SDL2_image. defs.h permet également la définition de structures (pour le jeu) et de constantes telles que la largeur ou la longueur de la fenetre.
 * Il permet d'incorporer par la meme occasion les différents fichiers .h (fichiers de prototypes) du programme (menu.h, moteur.h ...).
 *
 */

#ifndef DEF_H_INCLUDED
#define DEF_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Variables globales
SDL_Window* pWindow;
SDL_Renderer *pRenderer;

/**
 * \struct images_jeu
 * \brief Structure correspondante aux objets de jeu.
 * Cette structure permet de de finir l’ensemble des textures (et leur position) correspondantes à l’affichage du plateau ainsi
 * qu’aux pions (et la surbrillance), soit l’affichage de la partie (fond, pion rouge, pion jaune, surbrillance).
 */typedef struct images_jeu
{
    SDL_Texture *fond;
    SDL_Texture *plateauJeu;

    SDL_Texture *pion_rouge;
    SDL_Texture *pion_jaune;
    SDL_Texture *surbrillance;
    SDL_Texture *surbrillanceCoupsPossibles;

    SDL_Surface *surfacePile;
    SDL_Texture *texturePile;

    SDL_Rect rect;
    SDL_Rect pile_r;
    SDL_Rect rectPlateauJeu;

    bool surbrillanceActive;

} images_jeu;



/**
 * \struct Struct_btn_jeu
 * \brief Structure classique correspondants aux boutons (lorsque la partie est en pause).
 * Cette structure permet de définir l’ensemble des textures (et leur position) correspondantes aux boutons présents
 * sur l’affichage de la partie qui permettront soit de sauvegarder la partie, soit de faire retour, ou encore de revoir les règles.
 */
typedef struct Struct_btn_jeu
{
    SDL_Texture *sauvegarder;
    SDL_Rect sauvegarder_r;
    SDL_Texture *charger;
    SDL_Rect charger_r;
    SDL_Texture *quitter;
    SDL_Rect quitter_r;
    SDL_Texture *reprendre;
    SDL_Rect reprendre_r;
    SDL_Texture *fondp;
    SDL_Rect fondp_r;

    SDL_Texture *save1;
    SDL_Rect save1_r;

    SDL_Texture *save2;
    SDL_Rect save2_r;

    SDL_Texture *save3;
    SDL_Rect save3_r;

    SDL_Texture *save4;
    SDL_Rect save4_r;

    SDL_Texture *retour;
    SDL_Rect retour_r;


} Struct_btn_jeu;

#include "menu.h"
#include "moteur.h"
#include "avalam.h"
#include "ia.h"


// Defines
#define DEBUG 1 // Mode debug

#define LARGEUR_FENETRE 1000
#define HAUTEUR_FENETRE 600



#endif
