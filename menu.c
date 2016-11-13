/**
 * \file menu.c
 * \brief Fichier contenant l'ensemble des fonctions des menus.
 * \author PETIT Sylvain, FALLER Romain, FIRRINCIELI Arnaud
 * \version 1.0
 * \date Janvier 2015
 *
 * Ce fichier contient l'ensemble des fonctions qui affichent et gèrent les différents menus du jeu (ainsi que l'affichage et les transitions des règles du jeu).
 *
 */


#include "defs.h"

/**
 * \fn initButtons()
 * \brief Fonction d'initialisation des textures. Elle permet de charger l'ensemble des textures (images, fond, boutons) des menus et règles (grâce à la fonction IMG_LoadTexture).
 * SDL_QueryTexture() récupère la largeur et la hauteur des textures dans le rect correspondant (obligatoire sinon la texture ne s'affiche pas).
 * On positionne ensuite ces texture grace aux rect correspondantes (on définit la position en x et y).
 *
 * \param Rien
 * \return Rien
 */
void initButtons()
{
    Btn_menu.jouer = IMG_LoadTexture(pRenderer, "boutons/jouer.png");
    Btn_menu.quitter = IMG_LoadTexture(pRenderer, "boutons/quitter.png");
    Btn_menu.regles = IMG_LoadTexture(pRenderer, "boutons/regles.png");

    Btn_menu.nouvellepartie = IMG_LoadTexture(pRenderer, "boutons/nouvelle_partie.png");
    Btn_menu.demo = IMG_LoadTexture(pRenderer, "boutons/demo.png");
    Btn_menu.retour = IMG_LoadTexture(pRenderer, "boutons/retour.png");

    Btn_menu.jcj = IMG_LoadTexture(pRenderer, "boutons/jcj.png");
    Btn_menu.jcia = IMG_LoadTexture(pRenderer, "boutons/jcia.png");

    Btn_menu.facile = IMG_LoadTexture(pRenderer, "boutons/facile.png");
    Btn_menu.moyen = IMG_LoadTexture(pRenderer, "boutons/moyen.png");
    Btn_menu.difficile = IMG_LoadTexture(pRenderer, "boutons/difficile.png");


    Btn_regles.suivant = IMG_LoadTexture(pRenderer, "boutons/suivant.png");
    Btn_regles.precedent = IMG_LoadTexture(pRenderer, "boutons/precedent.png");
    Btn_regles.retour = IMG_LoadTexture(pRenderer, "boutons/retour.png");

    Btn_regles.fondRegles1 = IMG_LoadTexture(pRenderer, "fond/fondregles/p1regles.png");
    Btn_regles.fondRegles2 = IMG_LoadTexture(pRenderer, "fond/fondregles/p2regles.png");
    Btn_regles.fondRegles3 = IMG_LoadTexture(pRenderer, "fond/fondregles/p3regles.png");
    Btn_regles.fondRegles4 = IMG_LoadTexture(pRenderer, "fond/fondregles/p4regles.png");

    // Récupère la largeur et la hauteur des textures dans le rect correspondant (obligatoire sinon la texture ne s'affiche pas)
    SDL_QueryTexture(Btn_menu.jouer, NULL, NULL, &Btn_menu.jouer_r.w, &Btn_menu.jouer_r.h );
    SDL_QueryTexture(Btn_menu.quitter, NULL, NULL, &Btn_menu.quitter_r.w, &Btn_menu.quitter_r.h );
    SDL_QueryTexture(Btn_menu.regles, NULL, NULL, &Btn_menu.regles_r.w, &Btn_menu.regles_r.h );

    SDL_QueryTexture(Btn_menu.nouvellepartie, NULL, NULL, &Btn_menu.nouvellepartie_r.w, &Btn_menu.nouvellepartie_r.h );
    SDL_QueryTexture(Btn_menu.demo, NULL, NULL, &Btn_menu.demo_r.w, &Btn_menu.demo_r.h );

    SDL_QueryTexture(Btn_menu.jcj, NULL, NULL, &Btn_menu.jcj_r.w, &Btn_menu.jcj_r.h );
    SDL_QueryTexture(Btn_menu.jcia, NULL, NULL, &Btn_menu.jcia_r.w, &Btn_menu.jcia_r.h );

    SDL_QueryTexture(Btn_menu.facile, NULL, NULL, &Btn_menu.facile_r.w, &Btn_menu.facile_r.h );
    SDL_QueryTexture(Btn_menu.moyen, NULL, NULL, &Btn_menu.moyen_r.w, &Btn_menu.moyen_r.h );
    SDL_QueryTexture(Btn_menu.difficile, NULL, NULL, &Btn_menu.difficile_r.w, &Btn_menu.difficile_r.h );

    SDL_QueryTexture(Btn_menu.retour, NULL, NULL, &Btn_menu.retour_r.w, &Btn_menu.retour_r.h );

    SDL_QueryTexture(Btn_regles.suivant, NULL, NULL, &Btn_regles.suivant_r.w, &Btn_regles.suivant_r.h );
    SDL_QueryTexture(Btn_regles.precedent, NULL, NULL, &Btn_regles.precedent_r.w, &Btn_regles.precedent_r.h );
    SDL_QueryTexture(Btn_regles.retour, NULL, NULL, &Btn_regles.retour_r.w, &Btn_regles.retour_r.h );



    // Positions des boutons

    Btn_menu.jouer_r.x = LARGEUR_FENETRE/2 - Btn_menu.jouer_r.w/2; // Centrage au milieu de la fenêtre
    Btn_menu.jouer_r.y = 280;

    Btn_menu.regles_r.x = LARGEUR_FENETRE/2 - Btn_menu.regles_r.w/2;
    Btn_menu.regles_r.y = 350;

    Btn_menu.quitter_r.x = LARGEUR_FENETRE/2 - Btn_menu.quitter_r.w/2;
    Btn_menu.quitter_r.y = 420;



    Btn_menu.nouvellepartie_r.x = LARGEUR_FENETRE/2 - Btn_menu.nouvellepartie_r.w/2; // Centrage au milieu de la fenêtre
    Btn_menu.nouvellepartie_r.y = 280;

    Btn_menu.demo_r.x = LARGEUR_FENETRE/2 - Btn_menu.demo_r.w/2;
    Btn_menu.demo_r.y = 350;

    Btn_menu.jcj_r.x = LARGEUR_FENETRE/2 - Btn_menu.jcj_r.w/2; // Centrage au milieu de la fenêtre
    Btn_menu.jcj_r.y = 280;

    Btn_menu.jcia_r.x = LARGEUR_FENETRE/2 - Btn_menu.jcia_r.w/2;
    Btn_menu.jcia_r.y = 350;


    Btn_menu.facile_r.x = LARGEUR_FENETRE/2 - Btn_menu.facile_r.w/2;
    Btn_menu.facile_r.y = 280;

    Btn_menu.moyen_r.x = LARGEUR_FENETRE/2 - Btn_menu.moyen_r.w/2;
    Btn_menu.moyen_r.y = 350;

    Btn_menu.difficile_r.x = LARGEUR_FENETRE/2 - Btn_menu.difficile_r.w/2;
    Btn_menu.difficile_r.y = 420;



    Btn_menu.retour_r.x = LARGEUR_FENETRE/2 - Btn_menu.retour_r.w/2;
    Btn_menu.retour_r.y = 420;

    Btn_regles.suivant_r.x = LARGEUR_FENETRE - 80;
    Btn_regles.suivant_r.y = 539;

    Btn_regles.precedent_r.x = LARGEUR_FENETRE - 160;
    Btn_regles.precedent_r.y = 539;

    Btn_regles.retour_r.x = 20;
    Btn_regles.retour_r.y = 535;
}


/**
 * \fn drawBoutons(int menuActuel)
 * \brief Fonction qui permet d'afficher l'ensemble des textures (chargées dans initButtons()). Les différentes textures sont affichées en fonction
 * des différents menus (on utilise l'énumération pour définir dans quel menu on se trouve). L'affichage des texture se fait grâce à la fonction SDL_RenderCopy().
 * \param menuActuel un entier (menu dans lequel on se trouve ==> on utilise l'énumération).
 * \return Rien
 */
void drawBoutons(int menuActuel)
{

    if(menuActuel == MENU_PRINCIPAL)
    {
        SDL_RenderCopy(pRenderer, Btn_menu.jouer, NULL, &Btn_menu.jouer_r);
        SDL_RenderCopy(pRenderer, Btn_menu.regles, NULL, &Btn_menu.regles_r);
        SDL_RenderCopy(pRenderer, Btn_menu.quitter, NULL, &Btn_menu.quitter_r);
    }

    else if(menuActuel == MENU_JOUER)
    {
        SDL_RenderCopy(pRenderer, Btn_menu.nouvellepartie, NULL, &Btn_menu.nouvellepartie_r);
        SDL_RenderCopy(pRenderer, Btn_menu.demo, NULL, &Btn_menu.demo_r);
        SDL_RenderCopy(pRenderer, Btn_menu.retour, NULL, &Btn_menu.retour_r);
    }

    else if(menuActuel == MENU_NOUVELLEPARTIE)
    {
        SDL_RenderCopy(pRenderer, Btn_menu.jcj, NULL, &Btn_menu.jcj_r);
        SDL_RenderCopy(pRenderer, Btn_menu.jcia, NULL, &Btn_menu.jcia_r);
        SDL_RenderCopy(pRenderer, Btn_menu.retour, NULL, &Btn_menu.retour_r);
    }
    else if (menuActuel == MENU_REGLES)
    {
        SDL_RenderCopy(pRenderer, Btn_regles.fondRegles1, NULL, NULL); // Fond
        SDL_RenderCopy(pRenderer, Btn_regles.suivant, NULL, &Btn_regles.suivant_r);
        SDL_RenderCopy(pRenderer, Btn_regles.retour, NULL, &Btn_regles.retour_r);
    }
    else if (menuActuel == REGLES_P2)
    {
        SDL_RenderCopy(pRenderer, Btn_regles.fondRegles2, NULL, NULL); // Fond
        SDL_RenderCopy(pRenderer, Btn_regles.suivant, NULL, &Btn_regles.suivant_r);
        SDL_RenderCopy(pRenderer, Btn_regles.precedent, NULL, &Btn_regles.precedent_r);
    }
    else if (menuActuel == REGLES_P3)
    {
        SDL_RenderCopy(pRenderer, Btn_regles.fondRegles3, NULL, NULL); // Fond
        SDL_RenderCopy(pRenderer, Btn_regles.suivant, NULL, &Btn_regles.suivant_r);
        SDL_RenderCopy(pRenderer, Btn_regles.precedent, NULL, &Btn_regles.precedent_r);
    }
    else if (menuActuel == REGLES_P4)
    {
        SDL_RenderCopy(pRenderer, Btn_regles.fondRegles4, NULL, NULL); // Fond
        SDL_RenderCopy(pRenderer, Btn_regles.precedent, NULL, &Btn_regles.precedent_r);
        SDL_RenderCopy(pRenderer, Btn_regles.retour, NULL, &Btn_regles.retour_r);
    }
    else if(menuActuel == MENU_JCIA)
    {
        SDL_RenderCopy(pRenderer, Btn_menu.facile, NULL, &Btn_menu.facile_r);
        SDL_RenderCopy(pRenderer, Btn_menu.moyen, NULL, &Btn_menu.moyen_r);
        SDL_RenderCopy(pRenderer, Btn_menu.difficile, NULL, &Btn_menu.difficile_r);
    }
}



/**
 * \fn gestionEventsBouton(int *menuActuel, bool *continuer, int x, int y)
 * \brief Cette fonction permet de faire la transition entre les
 * différents menus, soit lorsque l’utilisateur clique sur un bouton, la fonction permet de définir dans quel menu on se trouve selon les
 * les différentes actions possibles (boutons...).
 *
 * \param Pointeur d'entier pour stocker le menu dans lequel on est et l'on passe,
 * \param booleen pour savoir si on ferme la fenetre ou non.
 * \param x et y des entiers pour les coordonnées du clic.
 * \return Rien
 */
void gestionEventsBouton(int *menuActuel, bool *continuer, int x, int y)
{
    if(*menuActuel == MENU_PRINCIPAL)
    {
        if(sourisSur(Btn_menu.jouer_r, x, y))
            *menuActuel = MENU_JOUER;

        if(sourisSur(Btn_menu.regles_r, x, y))
            *menuActuel = MENU_REGLES;

        if(sourisSur(Btn_menu.quitter_r, x, y))
            *continuer = false;
    }

    else if(*menuActuel == MENU_JOUER)
    {
        if(sourisSur(Btn_menu.nouvellepartie_r, x, y))
            *menuActuel = MENU_NOUVELLEPARTIE;

        if(sourisSur(Btn_menu.demo_r, x, y))
            avalam(DEMO, 2);

        if(sourisSur(Btn_menu.retour_r, x, y))
            *menuActuel = MENU_PRINCIPAL; // Retour au menu d'avant

    }

    else if(*menuActuel == MENU_NOUVELLEPARTIE)
    {
        // AVALAM JOUEUR CONTRE JOUEUR
        if(sourisSur(Btn_menu.jcj_r, x, y))
        {
            avalam(JCJ, AUCUNE);
        }
        // JOUEUR CONTRE IA
        if(sourisSur(Btn_menu.jcia_r, x, y))
        {
            *menuActuel = MENU_JCIA;
        }


        if(sourisSur(Btn_menu.retour_r, x, y))
            *menuActuel = MENU_JOUER; // Retour au menu d'avant
    }
    else if (*menuActuel == MENU_JCIA)
    {
        if(sourisSur(Btn_menu.facile_r, x, y))
            avalam(JCIA,1);
        if(sourisSur(Btn_menu.moyen_r, x, y))
            avalam(JCIA,2);
        if(sourisSur(Btn_menu.difficile_r, x, y))
            avalam(JCIA,3);
    }
    else if (*menuActuel == MENU_REGLES)
    {
        if(sourisSur(Btn_regles.retour_r,x,y))
            *menuActuel = MENU_PRINCIPAL;
        if(sourisSur(Btn_regles.suivant_r,x,y))
            *menuActuel = REGLES_P2;
    }
    else if(*menuActuel == REGLES_P2)
    {
        if(sourisSur(Btn_regles.suivant_r,x,y))
            *menuActuel = REGLES_P3;
        if(sourisSur(Btn_regles.precedent_r,x,y))
            *menuActuel = MENU_REGLES;
    }
    else if(*menuActuel == REGLES_P3)
    {
        if(sourisSur(Btn_regles.suivant_r,x,y))
            *menuActuel = REGLES_P4;
        if(sourisSur(Btn_regles.precedent_r,x,y))
            *menuActuel = REGLES_P2;
    }
    else if(*menuActuel == REGLES_P4)
    {
        if(sourisSur(Btn_regles.precedent_r,x,y))
            *menuActuel = REGLES_P3;
        if(sourisSur(Btn_regles.retour_r,x,y))
            *menuActuel = MENU_PRINCIPAL;
    }

}

