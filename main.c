/*! \mainpage Documentation Projet Algorithmique CIR 2: AVALAM
 *
 * \section Programme
 * Voici le fichier qui éxécute le programme principal en appelant les différentes fonctions élaborées dans les autres fichiers.c : main.c (avec defs.h).
 *
 * \section Aspect
 *Ce module nous a permis d’élaborer la gestion des menus. Il permet de charger et d’afficher les textures correspondantes aux
 *menus et aux règles, ainsi qu’à faire la transition entre les différents menus: menu.c (avec  menu.h).
 *
 * \section Moteurs
 * \subsection step1 Moteur graphique:
 * Ce module regroupe l’ensemble des fonctions permettant l’affichage du jeu, c’est à dire,
 * le fond, le plateau ainsi que les pions et les boutons (gère la transition des bouttons): avalam.c (avec avalam.h).
 * \subsection step2 Moteur de jeu:
 * Ce module regroupe l’ensemble des fonctions permettant le bon fonctionnement du jeu et assure
 * la partie technique du programme (gestion des évènement, algorithmes du jeu...): moteur.c (avec moteur.h).
 *
 * \section IA
 * Ce module regroupera l’ensemble des fonctions concernant l’intelligence artificielle,
 * soit elle sera appelée dans le mode ‘solo’ du jeu: ia.c (avec ia.h).
 *
 *
 * \section Autres
 * Plusieurs dossiers avec les images (dossier fonds, dossier jeu, dossier boutons...) mais également un dossier avec les différentes polices utilisées (en .ttf).
 */



#include "defs.h"

/**
* Appelle les fonctions de nettoyage. Doit être exécuté tout à la fin du programme
*/
void cleanup()
{
    SDL_DestroyTexture(Btn_menu.regles);
    SDL_DestroyTexture(Btn_menu.retour);
    SDL_DestroyTexture(Btn_menu.quitter);
    SDL_DestroyTexture(Btn_menu.jouer);
    SDL_DestroyTexture(Btn_menu.nouvellepartie);
    SDL_DestroyTexture(Btn_menu.jcj);
    SDL_DestroyTexture(Btn_menu.jcia);

    // Libération renderer et window
     SDL_DestroyRenderer(pRenderer);
     SDL_DestroyWindow(pWindow);

    //On quitte SDL_TTF 2
    TTF_Quit();
    //On quitte la SDL
    SDL_Quit();
}


int main(int argc, char** argv)
{
    /* Initialisation simple */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0 )
    {
        printf("Échec de l'initialisation de la SDL (%s)\n",SDL_GetError()); // Ecriture dans stdout, fichier de la SDL permettant de logger les erreurs par exemple
        return -1;
    }

    if (TTF_Init() != 0)
    {
        printf("ERREUR TTF_Init");
        SDL_Quit();
        return -1;
    }


    // Déclaration variables
    SDL_Event event; // Evenements
    SDL_Texture *fondMenu = NULL;
    SDL_Texture *logoMenu = NULL;
    SDL_Rect logoMenu_r;
    bool continuer= true;
    int menuActuel = MENU_PRINCIPAL;

    /* Création de la fenêtre */
    SDL_CreateWindowAndRenderer(LARGEUR_FENETRE, HAUTEUR_FENETRE,
                                SDL_WINDOW_SHOWN|SDL_WINDOWPOS_CENTERED,
                                &pWindow, &pRenderer);

    SDL_SetWindowTitle(pWindow, "Avalam");


    // Chargement textures boutons et initialisation
    initButtons();


    // Chargement et initialisation du fond et du logo
    fondMenu = IMG_LoadTexture(pRenderer, "fond/menu.jpg");
    logoMenu = IMG_LoadTexture(pRenderer, "fond/logo.png");

    SDL_QueryTexture(logoMenu, NULL, NULL, &logoMenu_r.w, &logoMenu_r.h);

    logoMenu_r.x = LARGEUR_FENETRE/2 - logoMenu_r.w/2;
    logoMenu_r.y = 0;


    if(!pWindow || !pRenderer)
    {
        fprintf(stderr,"Erreur de création de la fenêtre: %s\n",SDL_GetError());
        return -1;
    }

    /* Fond noir */
    SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);

    // Boucle princicpale
    while(continuer)
    {
        if(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_WINDOWEVENT: // Événement de la fenêtre
                if (event.window.event == SDL_WINDOWEVENT_CLOSE) // Fermeture de la fenêtre
                {
                    continuer = false;
                }
                break;
            case SDL_KEYUP: // Événement de relâchement d'une touche clavier
                if (event.key.keysym.sym == SDLK_ESCAPE) // C'est la touche Échap
                {
                    if(menuActuel != MENU_PRINCIPAL)
                    {
                        if(menuActuel == MENU_JCIA)
                            menuActuel = MENU_NOUVELLEPARTIE;
                        else if(menuActuel == MENU_CHARGER)
                            menuActuel = MENU_JOUER;
                        else if(menuActuel == MENU_JOUER)
                            menuActuel = MENU_PRINCIPAL;
                        else if(menuActuel == MENU_NOUVELLEPARTIE)
                            menuActuel = MENU_PRINCIPAL;
                    }

                    else // si on est dans le menu principal
                        continuer = false;
                }
                if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    if(menuActuel == MENU_REGLES)
                        menuActuel = REGLES_P2;
                    else if(menuActuel == REGLES_P2)
                        menuActuel = REGLES_P3;
                    else if(menuActuel == REGLES_P3)
                        menuActuel = REGLES_P4;
                }
                if (event.key.keysym.sym == SDLK_LEFT)
                {
                    if(menuActuel == REGLES_P2)
                        menuActuel = MENU_REGLES;
                    else if(menuActuel == REGLES_P3)
                        menuActuel = REGLES_P2;
                    else if(menuActuel == REGLES_P4)
                        menuActuel = REGLES_P3;
                }
                break;

            case SDL_MOUSEMOTION:
                break;

            case SDL_MOUSEBUTTONUP:
                break;
            case SDL_MOUSEBUTTONDOWN:
#ifdef DEBUG
                fprintf(stdout, "\tposition : %d;%d\n",event.button.x,event.button.y);
#endif
                // modifie le menu actuel en fonction des clics sur les boutons
                gestionEventsBouton(&menuActuel, &continuer, event.button.x, event.button.y);

                break;

            default:
                break;
            }
        }

        SDL_RenderClear(pRenderer); // Efface l'écran

        // Affichage
        SDL_RenderCopy(pRenderer, fondMenu, NULL, NULL); // Fond
        SDL_RenderCopy(pRenderer, logoMenu, NULL, &logoMenu_r); // Logo "bannière"

        // Dessine les boutons en fonction du menu actuel
        drawBoutons(menuActuel);

        // Met à jour l'affichage
        SDL_RenderPresent(pRenderer);

    }

    // Fonction de nettoyage

    SDL_DestroyTexture(fondMenu);
    SDL_DestroyTexture(logoMenu);

    cleanup();

    return 0;
}
