/**
 * \file avalam.c
 * \brief Fichier contenant l'ensemble des fonctions nécéssaire au MOTEUR GRAPHIQUE, soit l'affichage des pions, du plateau, du score etc.
 * \author PETIT Sylvain, FALLER Romain, FIRRINCIELI Arnaud
 * \version 1.0
 * \date Janvier-Février 2015
 *
 * Ce module regroupe l’ensemble des fonctions permettant l’affichage du jeu (moteur graphique), c’est à dire, le fond, le plateau ainsi que les pions et les boutons (gère la transition des boutons).
 *
 */


#include "defs.h"


/**
 * \fn avalam(int mode_jeu, int mode_difficulte)
 * \brief Fonction principale du moteur graphique qui fait appel aux autres fonctions d'affichage et de moteur.c.
 *
 * \param mode_jeu un entier pour savoir le mode de jeu choisit.
 * \param mode_difficulté un entier pour savoir le niveau de difficulté de l'IA si le mode Solo est choisit.
 * \return Rien
 */
void avalam(int mode_jeu, int mode_difficulte)
{
    SDL_Event event;
    bool continuer = true, pause = false, partieTermine = false, sauvegarder = false, charger = false;
    int i;
    int yOrigine, xOrigine, yDestination, xDestination; // Sert à l'IA et l'animation
    TTF_Font *police = chargerPolice("polices/Freshman.ttf", TAILLE_POLICE);

    // Déclaration structures
    images_jeu ImagesJeu;
    Struct_btn_jeu Btn_jeu;
    UI Ui;
    score score;
    jeu Jeu;
    Jeu.mode_jeu = mode_jeu;

    // Tableaux 2D de structure
    plateau **Plateau;
    plateau **PlateauSuivant;

    // Allocation 1ère dimension
    Plateau = malloc(sizeof(plateau*) * TAILLE_PLATEAU);
    PlateauSuivant = malloc(sizeof(plateau*) * TAILLE_PLATEAU);
    for(i=0 ; i < TAILLE_PLATEAU ; i++) // Allocation 2ème dimension
    {
        Plateau[i] = malloc(sizeof(plateau) * TAILLE_PLATEAU);
        PlateauSuivant[i] = malloc(sizeof(plateau) * TAILLE_PLATEAU);
    }


    // Charge et initialise les images du jeu
    chargeInitImagesJeu(&ImagesJeu, &Ui);
    // Charge et initialise les buttons du jeu
    chargeInitButtonJeu(&Btn_jeu);
    // Charge et initialise les buttons du menu sauvegarder et charger
    chargeInitButtonSave(&Btn_jeu);
    // Initialisation du plateau de jeu
    initPlateau(Plateau, PlateauSuivant);
    // Initialise la structure Jeu
    initJeu(&Jeu);

    // Boucle princicpale
    while(continuer)
    {
        /* Début gestion des événements */
        if(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_WINDOWEVENT: // Événement de la fenêtre
                if (event.window.event == SDL_WINDOWEVENT_CLOSE) // Fermeture de la fenêtre
                {
                    continuer = false; // Retour au menu
                }
                break;

            case SDL_KEYUP: // Événement de relâchement d'une touche clavier
                if (event.key.keysym.sym == SDLK_ESCAPE) // C'est la touche Échap
                    if(!pause && !sauvegarder)
                        pause = !pause; // Menu pause activé
                if(sauvegarder)
                {
                    sauvegarder = !sauvegarder;
                    pause = !pause;
                }
                if(charger)
                {
                    charger = !charger;
                    pause = true;
                }
                break;
            case SDL_MOUSEMOTION:
                // Detection souris sur pion
                if(!pause && !sauvegarder && !charger)
                    gestionSurbrillanceSurvol(event, &ImagesJeu, Plateau, PlateauSuivant);
                break;

            case SDL_MOUSEBUTTONDOWN: // Clic souris
#ifdef DEBUG
                fprintf(stdout, "\tposition : %d;%d\n",event.button.x,event.button.y);
#endif
                if(!pause && !sauvegarder && !charger)
                {
                    printf("\t\t\tSCORE : jaune=%d  rouge=%d\n", Jeu.scoreJoueurJaune, Jeu.scoreJoueurRouge);
                    if(Jeu.mode_jeu == JCJ)
                        selectionPion(event, &ImagesJeu, Plateau, PlateauSuivant, &Jeu); // On ne peut sélectionner un pion que si le menu pause n'est pas actif
                    else if(Jeu.mode_jeu == JCIA)
                    {
                        if(Jeu.joueurActuel == J1)
                        {
                            selectionPion(event, &ImagesJeu, Plateau, PlateauSuivant, &Jeu); // On ne peut sélectionner un pion que si le menu pause n'est pas actif
                        }
                    }
                }
                else if(pause)// Si le menu pause est activé, on gère les événements correspondants
                    gestionEventsBoutonPause(&pause,&continuer,&sauvegarder,&charger,event.button.x,event.button.y, &Btn_jeu);
                else if(sauvegarder)
                    gestionEventsBoutonSauvegarde(&pause,&continuer,&sauvegarder,event.button.x,event.button.y, &Btn_jeu,Plateau,&Jeu);
                else if(charger)
                    gestionEventsBoutonCharge(&pause,&continuer,&charger,event.button.x,event.button.y, &Btn_jeu,Plateau, PlateauSuivant, &Jeu);

                break;

            case SDL_MOUSEBUTTONUP:
                if(!pause)
                {
                    if(Jeu.mode_jeu == JCJ) // Mode Joueur contre joueur
                        deplacerPions(event, &ImagesJeu, Plateau, PlateauSuivant, &Jeu);
                    else if(Jeu.mode_jeu == JCIA)
                    {
                        if(Jeu.joueurActuel == J1)
                        {
                            deplacerPions(event, &ImagesJeu, Plateau, PlateauSuivant, &Jeu);
                        }
                    }
                }
                break;

            default:
                break;
            }
        }
        /* Fin de la gestion des événements */

        // Copie la structure plateau
        copiePlateau(Plateau, PlateauSuivant);

        if(partieTerminee(PlateauSuivant, &Jeu))
        {
            partieTermine = true;
            afficheGagnant(&Jeu, &Ui, police);
        }

        SDL_RenderClear(pRenderer); // Efface l'écran
        // Affiche le fond
        SDL_RenderCopy(pRenderer, ImagesJeu.fond, NULL, NULL);

        // Affichage plateau et interface
        drawPlateau(police, &ImagesJeu, Plateau);
        drawPionDeplace(event, police, &ImagesJeu, Plateau, &Jeu);
        drawUI(police, partieTermine, &Ui, &Jeu);
        drawscorePanel(police,partieTermine,&Jeu,&score);

        // Affichage menus in-game
        if(pause)
            drawPause(&Btn_jeu);

        if(sauvegarder)
            drawSauvegarde(&Btn_jeu);

        if(charger)
            drawSauvegarde(&Btn_jeu);

        // Met à jour l'affichage
        SDL_RenderPresent(pRenderer);

        // L'IA joue si c'est à son tour
        if(((Jeu.mode_jeu == JCIA && Jeu.joueurActuel == J2) || Jeu.mode_jeu == DEMO) && !partieTermine && !pause && !sauvegarder && !charger) // Au tour de l'IA de jouer
        {
            if(Jeu.mode_jeu == DEMO && Jeu.joueurActuel == J1) // L'IA joue contre elle même
                IA_ReflechieEtJoue(PlateauSuivant, 1, mode_difficulte, &yOrigine, &xOrigine, &yDestination, &xDestination, true);
            else if(Jeu.joueurActuel == J2)
                IA_ReflechieEtJoue(PlateauSuivant, 1, mode_difficulte, &yOrigine, &xOrigine, &yDestination, &xDestination, false);

            // Anime le déplacement du pion
            animePionIA(&ImagesJeu, Plateau, &Jeu, &Ui, police, partieTermine, &score, yOrigine, xOrigine, yDestination, xDestination);

            if(Jeu.joueurActuel == J2)
                Jeu.joueurActuel = J1;
            else if(Jeu.joueurActuel == J1)
                Jeu.joueurActuel = J2;
        }

    }

    // libère les textures
    cleanupTexturesJeu(&ImagesJeu, &Btn_jeu, &Ui);

    // Libère plateaux
    for(i=0 ; i < TAILLE_PLATEAU ; i++)
    {
        free(Plateau[i]);
        free(PlateauSuivant[i]);
    }
    free(Plateau);
    free(PlateauSuivant);
}


/**
 * \fn afficheGagnant (jeu *Jeu, UI *Ui, TTF_Font *police)
 * \brief Fonction qui affiche le gagnant à la fin de la fin de la partie.
 *
 * \param *Jeu pointeur de structure jeu (pour comparer le score des joueur).
 * \param *Ui pointeur de structure Ui (la ou on va afficher le gagnant et positionner le texte).
 * \param *police pointeur de type TTF pour charger la police, la couleur.
 * \return Rien
 */
void afficheGagnant(jeu *Jeu, UI *Ui, TTF_Font *police)
{
    char joueurGagnant[50];
    if(Jeu->scoreJoueurJaune < Jeu->scoreJoueurRouge)
    {
        SDL_Color couleurPolice = {255,0,0};
        sprintf(joueurGagnant, "Le joueur rouge est gagnant");
        Ui->surfaceTour = TTF_RenderText_Blended(police, joueurGagnant, couleurPolice);
        Ui->zone_text.x = 673;
        Ui->zone_text.y = 70;
    }
    else if((Jeu->scoreJoueurJaune > Jeu->scoreJoueurRouge))
    {
        SDL_Color couleurPolice = {255,200,0};

        sprintf(joueurGagnant, "Le joueur jaune est gagnant");
        Ui->surfaceTour = TTF_RenderText_Blended(police, joueurGagnant, couleurPolice);
        Ui->zone_text.x = 673;
        Ui->zone_text.y = 70;
    }
    else
    {
        SDL_Color couleurPolice = {255,255,255};
        sprintf(joueurGagnant, "Egalite : aucun joueur gagnant");
        Ui->surfaceTour = TTF_RenderText_Blended(police, joueurGagnant, couleurPolice);
        Ui->zone_text.x = 675;
        Ui->zone_text.y = 70;
    }

}


/**
 * \fn TTF_Font *chargerPolice(char *chemin, int taillePolice)
 * \brief Fonction qui permet de charger la police (par rapport au chemin comme on a plusieurs police pour eviter de définir plusieurs variable de police).
 *
 * \param *chemin un pointeur de caractere (une chaine de caractere) pour le chemin de la police.
 * \param taillePolice un entier pour la taille de la police.
 * \return police de type TTF_Font.
 */
TTF_Font *chargerPolice(char *chemin, int taillePolice)
{
    TTF_Font *police = TTF_OpenFont(chemin, taillePolice);
    if(police == NULL)
    {
        printf("[!] Impossible de charger la police");
        cleanup();
        exit(0);
    }
    else
        return police;
}


/**
 * \fn drawPionDeplace(SDL_Event event, TTF_Font *police, images_jeu *ImagesJeu, plateau **Plateau, jeu *Jeu)
 * \brief Cette fonction permet d’afficher le pions lorsqu’il est déplacé (cette fonction permet donc d’afficher le « Drag and drop » des pions).
 * Elle vérifie si le pion est bien sélectionné et déplacé, ensuite elle affiche le pion aux coordonnées actuelles (en direct).
 *
 * \param event un évenement car elle sera actionnée lors du déplacement d'un pion.
 * \param *police pour la taille de la pille sur le pion déplacé.
 * \param *ImagesJeu pour afficher défi.
 * \param **Plateau pour déterminé si le pion est sélectionné et la couleur du pion séléctionné.
 * \param *Jeu pour déterminé si enTrainDeDeplacer est bien vrai.
 * \return Rien.
 */
void drawPionDeplace(SDL_Event event, TTF_Font *police, images_jeu *ImagesJeu, plateau **Plateau, jeu *Jeu)
{
    int i,j,w,h;
    SDL_Rect pionDeplace;
    pionDeplace.w = ImagesJeu->rect.w;
    pionDeplace.h = ImagesJeu->rect.h;

    // Affichage
    for(i=0, w = OFFSET_DEBUTPLATEAU_Y; i < TAILLE_PLATEAU; i++, w += ESPACEMENT_Y)
    {
        ImagesJeu->rect.y = w;

        for(j=0, h = OFFSET_DEBUTPLATEAU_X; j < TAILLE_PLATEAU; j++, h += ESPACEMENT_X)
        {
            ImagesJeu->rect.x = h;

            // Affiche le pion en train d'être déplacé
            if(Jeu->enTrainDeDeplacer && Plateau[i][j].estSelectionne)
            {
                pionDeplace.x = event.motion.x - pionDeplace.w / 2;
                pionDeplace.y = event.motion.y - pionDeplace.h / 2;
                pionDeplace.w = ImagesJeu->rect.w;
                pionDeplace.h = ImagesJeu->rect.h;

                if(Plateau[i][j].couleur == JAUNE) // S'il s'agit d'un pion jaune
                {
                    SDL_RenderCopy(pRenderer, ImagesJeu->pion_jaune, NULL, &pionDeplace); // On l'affiche
                }

                else if(Plateau[i][j].couleur == ROUGE) // S'il s'agit d'un pion rouge
                {
                    SDL_RenderCopy(pRenderer, ImagesJeu->pion_rouge, NULL, &pionDeplace); // On l'affiche
                }

                SDL_RenderCopy(pRenderer, ImagesJeu->surbrillance, NULL, &pionDeplace);

                drawTaillePileDeplace(police, ImagesJeu, Plateau, Jeu, pionDeplace, i,j);

            }
        }

    }

}

/**
 * \fn void drawTaillePileDeplace(TTF_Font *police, images_jeu *ImagesJeu, plateau **Plateau, jeu *Jeu, SDL_Rect pionDeplace, int i, int j)
 * \brief Cette fonction dessine la taille de la pile de pion sur un pion en déplacement
 * \param *police pour le chemin du fichier ou va sauvegardesauvegarder la partie.
 * \param *ImagesJeu pour récupérer la couleur des pions sur le plateau.
 * \param **Plateau pour récupérer le score (qui se trouve dans la structure )
 * \param *Jeu
 * \param pionDeplace coordonnées du pion à déplacer
 * \param i
 * \param j
 * \return void
 */
void drawTaillePileDeplace(TTF_Font *police, images_jeu *ImagesJeu, plateau **Plateau, jeu *Jeu, SDL_Rect pionDeplace, int i, int j)
{
    char taillePile[2];
    SDL_Color couleurPolice = {0,0,0};
    // Affichage taille de la pile en déplacant la pièce
    if(Plateau[i][j].taillePile > 1)
    {
        // Ecriture dans la chaine de caractère taillePile
        sprintf(taillePile, "%d", Plateau[i][j].taillePile);

        // Création surface à partir de taillePile
        if(Plateau[i][j].taillePile == 5) // Pile de 5 => affichage taille en gris (pile "verrouillée")
        {
            couleurPolice.r = 45;
            couleurPolice.g = 45;
            couleurPolice.b = 45;
        }
        else
        {
            couleurPolice.r = 0;
            couleurPolice.g = 0;
            couleurPolice.b = 0;
        }

        ImagesJeu->surfacePile = TTF_RenderText_Blended(police, taillePile, couleurPolice);
        // Conversion en texture
        ImagesJeu->texturePile = SDL_CreateTextureFromSurface(pRenderer, ImagesJeu->surfacePile);

        SDL_QueryTexture(ImagesJeu->texturePile, NULL, NULL, &ImagesJeu->pile_r.w, &ImagesJeu->pile_r.h);

        // Mise à jour coordonnées
        pionDeplace.x = pionDeplace.x + (pionDeplace.w / 2) - 5;
        pionDeplace.y = pionDeplace.y + (pionDeplace.h / 2) - 5;

        pionDeplace.w = ImagesJeu->pile_r.w;
        pionDeplace.h = ImagesJeu->pile_r.h;

        SDL_RenderCopy(pRenderer, ImagesJeu->texturePile, NULL, &pionDeplace);

        SDL_FreeSurface(ImagesJeu->surfacePile);
        SDL_DestroyTexture(ImagesJeu->texturePile);
    }
}


/**
 * \fn void animePionIA(images_jeu *ImagesJeu, plateau **Plateau, jeu *Jeu, UI *Ui, TTF_Font *police, bool partieTermine, score *score, int yOrigine, int xOrigine, int yDestination, int xDestination)
 * \brief Anime le déplacement d'un pion
 * \param *police pour le chemin du fichier ou va sauvegarder la partie.
 * \param *ImagesJeu pour récupérer la couleur des pions sur le plateau.
 * \param **Plateau pour récupérer le score (qui se trouve dans la structure )
 * \param *Jeu
 * \param *Ui
 * \param partieTermine
 * \param *score
 * \param yOrigine
 * \param xOrigine
 * \param yDestination
 * \param xDestination
 * \param pionDeplace coordonnées du pion à déplacer
 * \param i
 * \param j
 * \return void
 */
void animePionIA(images_jeu *ImagesJeu, plateau **Plateau, jeu *Jeu, UI *Ui, TTF_Font *police,
                 bool partieTermine, score *score, int yOrigine, int xOrigine, int yDestination, int xDestination)
{
    bool animationEnCours = true;
    int i,j,w,h;

    SDL_Rect anim, animFin;
    anim.w = ImagesJeu->rect.w;
    anim.h = ImagesJeu->rect.h;

    // On "verrouille" le pion
    Plateau[yOrigine][xOrigine].estSelectionne = true;

    // On trouve les coordonnées en pixel du pion à déplacer
    for(i=0, w=OFFSET_DEBUTPLATEAU_Y; i<TAILLE_PLATEAU; i++, w += ESPACEMENT_Y)
    {
        ImagesJeu->rect.y = w;

        for(j=0, h=OFFSET_DEBUTPLATEAU_X; j<TAILLE_PLATEAU; j++, h += ESPACEMENT_X)
        {
            ImagesJeu->rect.x = h;

            if(Plateau[i][j].estSelectionne) // On trouve les coordonnées graphiques du pion à animer
            {
                anim.x = ImagesJeu->rect.x;
                anim.y = ImagesJeu->rect.y;
            }
            if(i == yDestination && j == xDestination)
            {
                animFin.x = ImagesJeu->rect.x;
                animFin.y = ImagesJeu->rect.y;
            }
        }
    }


    // Début de l'animation
    while(animationEnCours)
    {
        SDL_RenderClear(pRenderer); // Efface l'écran
        // Affiche le fond
        SDL_RenderCopy(pRenderer, ImagesJeu->fond, NULL, NULL);
        // Affichage plateau et interface
        drawPlateau(police, ImagesJeu, Plateau);

        // Affichage de l'animation
        if(yDestination > yOrigine) // Déplacement vers le haut...
            anim.y++;
        else if(yDestination < yOrigine) // Déplacement vers le bas...
            anim.y--;

        if(xDestination > xOrigine) // à droite
            anim.x++;
        else if(xDestination < xOrigine) // à gauche
            anim.x--;

        for(i=0, w=OFFSET_DEBUTPLATEAU_Y; i<TAILLE_PLATEAU; i++, w += ESPACEMENT_Y)
        {
            ImagesJeu->rect.y = w;

            for(j=0, h=OFFSET_DEBUTPLATEAU_X; j<TAILLE_PLATEAU; j++, h += ESPACEMENT_X)
            {
                ImagesJeu->rect.x = h;

                if(Plateau[i][j].estSelectionne)
                {
                    if(Plateau[i][j].couleur == JAUNE) // S'il s'agit d'un pion jaune
                    {
                        SDL_RenderCopy(pRenderer, ImagesJeu->pion_jaune, NULL, &anim); // On l'affiche
                    }

                    else if(Plateau[i][j].couleur == ROUGE) // S'il s'agit d'un pion rouge
                    {
                        SDL_RenderCopy(pRenderer, ImagesJeu->pion_rouge, NULL, &anim); // On l'affiche
                    }

                    SDL_RenderCopy(pRenderer, ImagesJeu->surbrillance, NULL, &anim);

                    drawTaillePileDeplace(police, ImagesJeu, Plateau, Jeu, anim, i,j);
                }
            }
        }
        if(anim.x == animFin.x && anim.y == animFin.y)
        {
            animationEnCours = false;
        }


        drawUI(police, partieTermine, Ui, Jeu);
        drawscorePanel(police, partieTermine, Jeu, score);

        // Met à jour l'affichage
        SDL_RenderPresent(pRenderer);

        SDL_Delay(10);
    }

    Plateau[yOrigine][xOrigine].estSelectionne = false;
}


/**
 * \fn drawPlateau(TTF_Font *police, images_jeu *ImagesJeu, plateau **Plateau)
 * \brief Cette fonction permet l’affichage du plateau, soit l’affichage de l’ensembles des textures chargées dans chargeInitmagesJeu().
 *
 * \param *police pour la taille de la pille sur le pion déplacé.
 * \param *ImagesJeu pour afficher défi.
 * \param **Plateau pour déterminé si le pion est sélectionné et la couleur du pion séléctionné.
 * \return Rien.
 */void drawPlateau(TTF_Font *police, images_jeu *ImagesJeu, plateau **Plateau)
{
    int i,j,w,h;

    SDL_Color couleurPolice = {0,0,0};
    char taillePile[2];

    SDL_RenderCopy(pRenderer, ImagesJeu->plateauJeu, NULL, &ImagesJeu->rectPlateauJeu);

    // Affichage
    for(i=0, w=OFFSET_DEBUTPLATEAU_Y; i<TAILLE_PLATEAU; i++, w += ESPACEMENT_Y)
    {
        ImagesJeu->rect.y = w;

        for(j=0, h=OFFSET_DEBUTPLATEAU_X; j<TAILLE_PLATEAU; j++, h += ESPACEMENT_X)
        {
            ImagesJeu->rect.x = h;

            if(!Plateau[i][j].estSelectionne)
            {
                // Affiche plateau actuel
                if(Plateau[i][j].couleur == JAUNE && Plateau[i][j].taillePile > 0) // S'il s'agit d'un pion jaune
                {
                    SDL_RenderCopy(pRenderer, ImagesJeu->pion_jaune, NULL, &ImagesJeu->rect); // On l'affiche
                }

                else if(Plateau[i][j].couleur == ROUGE && Plateau[i][j].taillePile > 0) // S'il s'agit d'un pion rouge
                {
                    SDL_RenderCopy(pRenderer, ImagesJeu->pion_rouge, NULL, &ImagesJeu->rect); // On l'affiche
                }

            }

            if(Plateau[i][j].surbrillance || AFFICHE_GRILLE_DEBUG) // Gestion debug
            {
                SDL_RenderCopy(pRenderer, ImagesJeu->surbrillance, NULL, &ImagesJeu->rect);

            }

            if(Plateau[i][j].coupPossible == 1)
            {
                SDL_RenderCopy(pRenderer, ImagesJeu->surbrillanceCoupsPossibles, NULL, &ImagesJeu->rect);
            }

            // Affichage taille de la pile
            if(Plateau[i][j].taillePile > 1 && !Plateau[i][j].estSelectionne)
            {
                // Ecriture dans la chaine de caractère taillePile
                sprintf(taillePile, "%d", Plateau[i][j].taillePile);

                // Création surface à partir de taillePile
                if(Plateau[i][j].taillePile == 5) // Pile de 5 => affichage taille en gris (pile "verrouillée")
                {
                    couleurPolice.r = 45;
                    couleurPolice.g = 45;
                    couleurPolice.b = 45;
                }
                else
                {
                    couleurPolice.r = 0;
                    couleurPolice.g = 0;
                    couleurPolice.b = 0;
                }
                ImagesJeu->surfacePile = TTF_RenderText_Blended(police, taillePile, couleurPolice);
                // Conversion en texture
                ImagesJeu->texturePile = SDL_CreateTextureFromSurface(pRenderer, ImagesJeu->surfacePile);

                SDL_QueryTexture(ImagesJeu->texturePile, NULL, NULL, &ImagesJeu->pile_r.w, &ImagesJeu->pile_r.h);

                // Mise à jour coordonnées
                ImagesJeu->pile_r.x = ImagesJeu->rect.x + ImagesJeu->rect.w/2 - 5;
                ImagesJeu->pile_r.y = ImagesJeu->rect.y + ImagesJeu->rect.h/2 - 5;
                SDL_RenderCopy(pRenderer, ImagesJeu->texturePile, NULL, &ImagesJeu->pile_r);

                SDL_FreeSurface(ImagesJeu->surfacePile);
                SDL_DestroyTexture(ImagesJeu->texturePile);
            }


        }
    }
}


/**
 * \fn drawPause(Struct_btn_jeu *Btn_jeu)
 * \brief Cette fonction permet l’affichage du menu « pause » , soit l’affichage de certaines textures chargées dans chargeInitBuDonJeu()
 * (les boutons lorsque l’utilisateur fait pause).
 * \param *Btn_jeu car on affiche des texture de la structure Struct_btn_jeu.
 * \return Rien
 */
void drawPause(Struct_btn_jeu *Btn_jeu)
{
    SDL_RenderCopy(pRenderer, Btn_jeu->fondp, NULL, NULL);
    SDL_RenderCopy(pRenderer, Btn_jeu->quitter, NULL, &Btn_jeu->quitter_r);
    SDL_RenderCopy(pRenderer, Btn_jeu->sauvegarder, NULL, &Btn_jeu->sauvegarder_r);
    SDL_RenderCopy(pRenderer, Btn_jeu->reprendre, NULL, &Btn_jeu->reprendre_r);
    SDL_RenderCopy(pRenderer, Btn_jeu->charger, NULL, &Btn_jeu->charger_r);
}


/**
 * \fn drawSauvegarde(Struct_btn_jeu *Btn_jeu)
 * \brief Cette fonction permet l'affichage de l'ensemble des textures correspondantes au "menu" de sauvegarde.
 * \param *Btn_jeu car on affiche des texture de la structure Struct_btn_jeu.
 * \return Rien
 */
void drawSauvegarde(Struct_btn_jeu *Btn_jeu)
{
    SDL_RenderCopy(pRenderer, Btn_jeu->fondp, NULL, NULL);

    SDL_RenderCopy(pRenderer, Btn_jeu->retour, NULL, &Btn_jeu->retour_r);
    SDL_RenderCopy(pRenderer, Btn_jeu->save1, NULL, &Btn_jeu->save1_r);
    SDL_RenderCopy(pRenderer, Btn_jeu->save2, NULL, &Btn_jeu->save2_r);
    SDL_RenderCopy(pRenderer, Btn_jeu->save3, NULL, &Btn_jeu->save3_r);
    SDL_RenderCopy(pRenderer, Btn_jeu->save4, NULL, &Btn_jeu->save4_r);
}

/**
 * \fn drawscorePanel(TTF_Font *police2, bool partieTermine, jeu *Jeu, score *Score)
 * \brief Cette fonction permet l’affichage de l’ensembles des textures correspondantes au score en direct pour chaque joueur.
 * \param *police2 pour charler une police (ttf).
 * \param partieTermine pour savoir si la partie est terminée ou non.
 * \param *jeu car on va afficher le score des joueurs qui se trouve dans la structure jeu.
 * \param *Score car on va écrire dans zone_textScore qui se trouve dans la structure score.
 * \return Rien
 */
void drawscorePanel(TTF_Font *police2, bool partieTermine, jeu *jeu, score *Score)
{
    char ScoreJoueur[90];

    SDL_Color couleurPolice = {255,255,255};
    sprintf(ScoreJoueur, "%s %s %d %s %d %s", "Score:","Jaune",jeu->scoreJoueurJaune,"-",jeu->scoreJoueurRouge,"Rouge");

    Score->surfaceText = TTF_RenderText_Blended(police2, ScoreJoueur, couleurPolice);
    Score->textureText = SDL_CreateTextureFromSurface(pRenderer, Score->surfaceText);
    SDL_QueryTexture(Score->textureText, NULL, NULL, &Score->zone_textScore.w, &Score->zone_textScore.h);

    Score->zone_textScore.x = 690;
    Score->zone_textScore.y = 120;

    SDL_RenderCopy(pRenderer, Score->textureText, NULL, &Score->zone_textScore);
    SDL_FreeSurface(Score->surfaceText);
    SDL_DestroyTexture(Score->textureText);
}


/**
 * \fn drawUI(TTF_Font *police, bool partieTermine, UI *Ui, jeu *Jeu)
 * \brief Cette fonction permet l’affichage de l’ensembles des textures correspondantes à l’interface utilisateur,
 * soit le tour de chaque joueur.
 * \param *police pour charger une police (ttf).
 * \param partieTermine pour savoir si la partie est terminée ou non.
 * \param *Ui car le texte ecrit sera dans zone_text de la structure UI.
 * \return Rien
 */
void drawUI(TTF_Font *police, bool partieTermine, UI *Ui, jeu *Jeu)
{
    char tourJoueur[100];
    SDL_Color couleurPolice = {255,255,255};

    if(!partieTermine)
    {
        SDL_RenderCopy(pRenderer, Ui->tourJoueur, NULL, &Ui->tourJoueur_r);
        if(Jeu->mode_jeu == JCIA)
        {
            Ui->zone_text.x = 700;
            Ui->zone_text.y = 70;

            if(Jeu->joueurActuel == J1)
                sprintf(tourJoueur, "Au tour du joueur rouge");
            else
            {
                Ui->zone_text.x = 745;
                sprintf(tourJoueur, "Au tour de l'ia");
            }

        }
        else
        {
            sprintf(tourJoueur, "Au tour du joueur %d", Jeu->joueurActuel+1);
            Ui->zone_text.x = 715;
            Ui->zone_text.y = 70;
        }

        Ui->surfaceTour = TTF_RenderText_Blended(police, tourJoueur, couleurPolice);
    }
    else
    {
        SDL_RenderCopy(pRenderer, Ui->joueurGagnant, NULL, &Ui->joueurGagnant_r);
    }


    Ui->textureTour = SDL_CreateTextureFromSurface(pRenderer, Ui->surfaceTour);

    SDL_QueryTexture(Ui->textureTour, NULL, NULL, &Ui->zone_text.w, &Ui->zone_text.h);

    SDL_RenderCopy(pRenderer, Ui->textureTour, NULL, &Ui->zone_text);
    SDL_FreeSurface(Ui->surfaceTour);
    SDL_DestroyTexture(Ui->textureTour);

}



/**
 * \fn gestionEventsBoutonPause(bool *pause, bool *continuer, int x, int y, Struct_btn_jeu *Btn_jeu)
 * \brief Cette fonction permet de gérer l’affichage et la transition entre la pause et le jeu.
 *  Soit elle permet de savoir si le jeu est en pause ou non (si l’utilisateur clique sur reprendre alors pause = false).
 * \param *ImagesJeu car on charge les textures dans structure images_jeu.
 * \param *Ui car on charge les textures dans structure UI.
 * \return Rien
 */
void gestionEventsBoutonPause(bool *pause, bool *continuer,bool *sauvegarder,bool *charger, int x, int y, Struct_btn_jeu *Btn_jeu)
{
    if(*pause)
    {
        if(sourisSur(Btn_jeu->quitter_r,x,y))
            *continuer = false;
        if(sourisSur(Btn_jeu->reprendre_r,x,y))
            *pause = false;
        if(sourisSur(Btn_jeu->sauvegarder_r,x,y))
        {
            *pause = false;
            *sauvegarder = true;
        }

        if(sourisSur(Btn_jeu->charger_r,x,y))
        {
            *pause = false;
            *charger = true;
        }

    }
}


/**
 * \fn gestionEventsBoutonSauvegarde(bool *pause, bool *continuer, bool *sauvegarder, int x, int y, Struct_btn_jeu *Btn_jeu, plateau **Plateau, jeu *Jeu)
 * \brief Cette fonction permet de gérer l’affichage et la transition entre le "menu" de sauvegarde et le jeu.
 * \param *pause pour la mettre a true lorsque le joueur reprend.
 * \param *continuer pour la mettre a false soit revenir au dernier menu.
 * \param *sauvegarder pour assurer la transition, soit true pour afficher le menu sauvegarde et  false pour le quitter.
 * \param x  pour l'abscisse de la souris (dans la fonction sourisSur()).
 * \param y  pour l'ordonnée de la souris (dans la fonction sourisSur()).
 * \param *Btn_jeu car on a besoin de la position des boutons qui se trouve dans la structure Struct_btn_jeu.
 * \param **Plateau car on a besoin de la structure plateau pour le sauvegarder (fonction sauvegarder()).
 * \param *Jeu car on a besoin de la structure jeu pour le sauvegarder (fonction sauvegarder()).
 */
void gestionEventsBoutonSauvegarde(bool *pause,bool *continuer, bool *sauvegarder, int x, int y, Struct_btn_jeu *Btn_jeu, plateau **Plateau, jeu *Jeu)
{
    if(*sauvegarder)
    {
        if(sourisSur(Btn_jeu->reprendre_r,x,y))
        {
            *pause = true;
            *sauvegarder = false;
        }
        else if(sourisSur(Btn_jeu->save1_r,x,y))
        {
            if(sauvegarde("save/partie1.txt",Plateau,Jeu) == 1)
            {
                *sauvegarder = false;
                *continuer = false;
                printf("Partie 1 save !\n");
            }
            else if(sauvegarde("save/partie1.txt",Plateau,Jeu) == 0)
            {
                printf("Impossible de save");
            }

        }
        else if(sourisSur(Btn_jeu->save2_r,x,y))
        {
            if(sauvegarde("save/partie2.txt",Plateau,Jeu) == 1)
            {
                *sauvegarder = false;
                *continuer = false;
                printf("Partie 2 save !\n");
            }
            else if(sauvegarde("save/partie2.txt",Plateau,Jeu) == 0)
            {
                printf("Impossible de save");
            }
        }
        else if(sourisSur(Btn_jeu->save3_r,x,y))
        {
            if(sauvegarde("save/partie3.txt",Plateau,Jeu) == 1)
            {
                *sauvegarder = false;
                *continuer = false;
                printf("Partie 3 save !\n");
            }
            else if(sauvegarde("save/partie3.txt",Plateau,Jeu) == 0)
            {
                printf("Impossible de save");
            }
        }
        else if(sourisSur(Btn_jeu->save4_r,x,y))
        {
            if(sauvegarde("save/partie4.txt",Plateau,Jeu) == 1)
            {
                *sauvegarder = false;
                *continuer = false;
                printf("Partie 4 save !\n");
            }
            else if(sauvegarde("save/partie4.txt",Plateau,Jeu) == 0)
            {
                printf("Impossible de save");
            }
        }
    }
}

/**
 * \fn void gestionEventsBoutonCharge(bool *pause,bool *continuer, bool *charger, int x, int y, Struct_btn_jeu *Btn_jeu, plateau **Plateau, plateau **Pcharge, jeu *Jeu)
 * \brief Cette fonction permet de gérer les événements du menu charger une partie.
 * \param *pause pour la mettre a true lorsque le joueur reprend.
 * \param *continuer pour la mettre a false soit revenir au dernier menu.
 * \param *charger
 * \param x pour l'abscisse de la souris (dans la fonction sourisSur()).
 * \param y pour l'ordonnée de la souris (dans la fonction sourisSur()).
 * \param *Btn_jeu car on a besoin de la position des boutons qui se trouve dans la structure Struct_btn_jeu.
 * \param **Plateau car on a besoin de la structure plateau pour la fonction charger
 * \param **PCharge
 * \param *Jeu car on a besoin de la structure jeu pour la fonction charger
 */
void gestionEventsBoutonCharge(bool *pause, bool *continuer, bool *charger, int x, int y, Struct_btn_jeu *Btn_jeu, plateau **Plateau, plateau **Pcharge, jeu *Jeu)
{
    if(*charger)
    {
        if(sourisSur(Btn_jeu->reprendre_r,x,y))
        {
            *pause = true;
            *charger = false;
        }

        if(sourisSur(Btn_jeu->save1_r,x,y))
        {
            if(charge("save/partie1.txt",Pcharge,Jeu) == 1)
            {
                *charger = false;
                Plateau = Pcharge;
                printf("Partie 1 chargé !\n");
            }
            else if(charge("save/partie1.txt",Plateau,Jeu) == 0)
            {
                printf("Impossible de charger");
            }

        }

        if(sourisSur(Btn_jeu->save2_r,x,y))
        {
            if(charge("save/partie2.txt",Pcharge,Jeu) == 1)
            {
                *charger = false;
                Plateau = Pcharge;
                printf("Partie 2 chargé !\n");
            }
            else if(charge("save/partie2.txt",Plateau,Jeu) == 0)
            {
                printf("Impossible de charger");
            }
        }

        if(sourisSur(Btn_jeu->save3_r,x,y))
        {
            if(charge("save/partie3.txt",Pcharge,Jeu) == 1)
            {
                *charger = false;
                Plateau = Pcharge;
                printf("Partie 3 chargé !\n");
            }
            else if(charge("save/partie3.txt",Plateau,Jeu) == 0)
            {
                printf("Impossible de charger");
            }
        }

        if(sourisSur(Btn_jeu->save4_r,x,y))
        {
            if(charge("save/partie4.txt",Pcharge,Jeu) == 1)
            {
                *charger = false;
                Plateau = Pcharge;
                printf("Partie 4 chargé !\n");
            }
            else if(charge("save/partie4.txt",Plateau,Jeu) == 0)
            {
                printf("Impossible de charger");
            }
        }
    }
}


/**
 * \fn chargeInitImagesJeu(images_jeu *ImagesJeu, UI *Ui)
 * \brief Cette fonction permet de charger l’ensemble des textures (ainsi que leurs positions) à afficher correspondantes aux images du jeu (pions, interface d’utilisateur...).
 * \param *ImagesJeu car on charge les textures dans structure images_jeu.
 * \param *Ui car on charge les textures dans structure UI.
 * \return Rien
 */
void chargeInitImagesJeu(images_jeu *ImagesJeu, UI *Ui)
{
    ImagesJeu->fond = IMG_LoadTexture(pRenderer, "fond/menu.jpg");
    ImagesJeu->pion_jaune = IMG_LoadTexture(pRenderer, "jeu/pion_jaune.png");
    ImagesJeu->pion_rouge = IMG_LoadTexture(pRenderer, "jeu/pion_rouge.png");
    ImagesJeu->plateauJeu = IMG_LoadTexture(pRenderer, "jeu/plateau.png");

    ImagesJeu->rectPlateauJeu.x = OFFSET_DEBUTPLATEAU_X-15;
    ImagesJeu->rectPlateauJeu.y = OFFSET_DEBUTPLATEAU_Y-15;

    SDL_QueryTexture(ImagesJeu->plateauJeu, NULL, NULL, &ImagesJeu->rectPlateauJeu.w, &ImagesJeu->rectPlateauJeu.h);

    ImagesJeu->surbrillance = IMG_LoadTexture(pRenderer, "jeu/surbrillance.png");
    ImagesJeu->surbrillanceCoupsPossibles = IMG_LoadTexture(pRenderer, "jeu/surbrillance2.png");

    // Récupère la largeur et la hauteur des textures dans le rect correspondant (obligatoire sinon la texture ne s'affiche pas)
    // Ici le pion rouge a la même dimension que le pion jaune
    SDL_QueryTexture(ImagesJeu->pion_jaune, NULL, NULL, &ImagesJeu->rect.w, &ImagesJeu->rect.h);

    ImagesJeu->surbrillanceActive = 0;

    // Position initiale
    ImagesJeu->rect.x = OFFSET_DEBUTPLATEAU_X;
    ImagesJeu->rect.y = OFFSET_DEBUTPLATEAU_Y;


    // Chargement de l'Ui
    Ui->tourJoueur = IMG_LoadTexture(pRenderer, "jeu/gagne.png");
    Ui->joueurGagnant = IMG_LoadTexture(pRenderer, "jeu/gagne.png");

    SDL_QueryTexture(Ui->tourJoueur, NULL, NULL, &Ui->tourJoueur_r.w, &Ui->tourJoueur_r.h);
    SDL_QueryTexture(Ui->joueurGagnant, NULL, NULL, &Ui->joueurGagnant_r.w, &Ui->joueurGagnant_r.h);

    Ui->tourJoueur_r.x = 650;
    Ui->tourJoueur_r.y = 50;
    Ui->joueurGagnant_r.x = 650;
    Ui->joueurGagnant_r.y = 50;
}


/**
 * \fn chargeInitButtonJeu(Struct_btn_jeu *Btn_jeu)
 * \brief Cette fonction permet de charger l’ensemble des textures (ainsi que leurs positions) à afficher correspondantes aux boutons notamment lorsque le jeu sera sur
«pause » (reprendre, quitter...).
 * \param *Btn_jeu car on charge les textures dans la structure Struct_btn_jeu.
 * \return Rien
 */
void chargeInitButtonJeu(Struct_btn_jeu *Btn_jeu)
{
    //Chargement menu pause
    Btn_jeu->quitter = IMG_LoadTexture(pRenderer, "boutons/quitter.png");
    Btn_jeu->sauvegarder = IMG_LoadTexture(pRenderer, "boutons/sauvegarder.png");
    Btn_jeu->reprendre = IMG_LoadTexture(pRenderer, "boutons/reprendre.png");
    Btn_jeu->fondp = IMG_LoadTexture(pRenderer, "fond/pause.png");
    Btn_jeu->charger = IMG_LoadTexture(pRenderer, "boutons/charger.png");

    SDL_QueryTexture(Btn_jeu->fondp, NULL, NULL, &Btn_jeu->fondp_r.w, &Btn_jeu->fondp_r.h);
    SDL_QueryTexture(Btn_jeu->reprendre, NULL, NULL, &Btn_jeu->reprendre_r.w, &Btn_jeu->reprendre_r.h);
    SDL_QueryTexture(Btn_jeu->quitter, NULL, NULL, &Btn_jeu->quitter_r.w, &Btn_jeu->quitter_r.h);
    SDL_QueryTexture(Btn_jeu->sauvegarder, NULL, NULL, &Btn_jeu->sauvegarder_r.w, &Btn_jeu->sauvegarder_r.h);
    SDL_QueryTexture(Btn_jeu->charger, NULL, NULL, &Btn_jeu->charger_r.w, &Btn_jeu->charger_r.h);

    Btn_jeu->reprendre_r.x = LARGEUR_FENETRE/2 - Btn_jeu->reprendre_r.w/2;
    Btn_jeu->reprendre_r.y = 180;
    Btn_jeu->sauvegarder_r.x = LARGEUR_FENETRE/2 - Btn_jeu->sauvegarder_r.w/2;
    Btn_jeu->sauvegarder_r.y = 250;
    Btn_jeu->charger_r.x = LARGEUR_FENETRE/2 - Btn_jeu->charger_r.w/2;
    Btn_jeu->charger_r.y = 320;
    Btn_jeu->quitter_r.x = LARGEUR_FENETRE/2 - Btn_jeu->quitter_r.w/2;
    Btn_jeu->quitter_r.y = 390;
}

/**
 * \fn void chargeInitButtonSave(Struct_btn_jeu *Btn_jeu)
 * \brief Cette fonction permet de charger et d'initialiser les boutons sauvegarder
 * \param *Btn_jeu boutons jeu
 * \return Rien
 */
void chargeInitButtonSave(Struct_btn_jeu *Btn_jeu)
{
    Btn_jeu->save1 = IMG_LoadTexture(pRenderer, "boutons/partie1.png");
    Btn_jeu->save2 = IMG_LoadTexture(pRenderer, "boutons/partie2.png");
    Btn_jeu->save3 = IMG_LoadTexture(pRenderer, "boutons/partie3.png");
    Btn_jeu->save4 = IMG_LoadTexture(pRenderer, "boutons/partie4.png");
    Btn_jeu->retour = IMG_LoadTexture(pRenderer, "boutons/retour.png");

    SDL_QueryTexture(Btn_jeu->save1, NULL, NULL, &Btn_jeu->save1_r.w, &Btn_jeu->save1_r.h);
    SDL_QueryTexture(Btn_jeu->save2, NULL, NULL, &Btn_jeu->save2_r.w, &Btn_jeu->save2_r.h);
    SDL_QueryTexture(Btn_jeu->save3, NULL, NULL, &Btn_jeu->save3_r.w, &Btn_jeu->save3_r.h);
    SDL_QueryTexture(Btn_jeu->save4, NULL, NULL, &Btn_jeu->save4_r.w, &Btn_jeu->save4_r.h);
    SDL_QueryTexture(Btn_jeu->retour, NULL, NULL, &Btn_jeu->retour_r.w, &Btn_jeu->retour_r.h);

    Btn_jeu->retour_r.x = LARGEUR_FENETRE/2 - Btn_jeu->retour_r.w/2;
    Btn_jeu->retour_r.y = 180;

    Btn_jeu->save1_r.x = LARGEUR_FENETRE/2 - Btn_jeu->save1_r.w/2;
    Btn_jeu->save1_r.y = 250;

    Btn_jeu->save2_r.x = LARGEUR_FENETRE/2 - Btn_jeu->save2_r.w/2;
    Btn_jeu->save2_r.y = 320;

    Btn_jeu->save3_r.x = LARGEUR_FENETRE/2 - Btn_jeu->save3_r.w/2;
    Btn_jeu->save3_r.y = 390;

    Btn_jeu->save4_r.x = LARGEUR_FENETRE/2 - Btn_jeu->save4_r.w/2;
    Btn_jeu->save4_r.y = 460;
}


/**
 * \fn int sauvegarde(char *chemin, plateau **Plateau, jeu *Jeu)
 * \brief Cette fonction permet de sauvegarder la partie dans un fichier texte.
 * \param *chemin pour le chemin du fichier ou va sauvegarder la partie.
 * \param **Plateau pour enregistrer la couleur des pions sur le plateau.
 * \param *Jeu pour enregistrer le score (qui se trouve dans la structure )
 * \return Rien
 */
int sauvegarde(char *chemin, plateau **Plateau, jeu *Jeu)
{
    FILE *fichier = NULL;

    int x,y;
    fichier = fopen(chemin,"w");

    if( fichier != NULL)
    {

        //début plateau couleur
        for(x=0; x<TAILLE_PLATEAU; x++)
        {
            for(y=0; y<TAILLE_PLATEAU; y++)
            {

                fprintf(fichier,"%d ",Plateau[x][y].couleur);
            }
            fprintf(fichier,"\n");
        }

        fprintf(fichier,"\n");
        //fin plateau couleur

        //début plateau taille pile
        for(x=0; x<TAILLE_PLATEAU; x++)
        {
            for(y=0; y<TAILLE_PLATEAU; y++)
            {

                fprintf(fichier,"%d ",Plateau[x][y].taillePile);
            }
            fprintf(fichier,"\n");
        }
        //fin plateau taille pile


        fprintf(fichier,"\n");

        fprintf(fichier,"%d",Jeu->joueurActuel);

        fclose(fichier);

        return 1;
    }
    else
    {
        printf("impossible d'ouvrir le fichier\n");
    }

    return 0;
}

/**
 * \fn int charger(char *chemin, plateau **Plateau, jeu *Jeu)
 * \brief Cette fonction permet de charger la partie à partir d'un fichier texte.
 * \param *chemin pour le chemin du fichier ou va sauvegarder la partie.
 * \param **Plateau pour récupérer la couleur des pions sur le plateau.
 * \param *Jeu pour récupérer le score (qui se trouve dans la structure )
 * \return Rien
 */
int charge(char *chemin, plateau **Plateau, jeu *Jeu)
{
    FILE *fichier = NULL;
    //int
    int x,y;
    fichier = fopen(chemin,"r+"); // r pour lire
    if( fichier != NULL)
    {
        // fseek( fichier, 13, SEEK_SET );

        for(x=0; x<9; x++)
        {
            for(y=0; y<9; y++)
            {
                fscanf(fichier,"%d ", &Plateau[x][y].couleur);  // on lit la couleur des pions dans le fichier pour chaque case du plateau

            }
        }
        printf("\n");
        //fin plateau couleur
        //fseek( fichier, 13, SEEK_SET );
        //début plateau taille pile
        for(x=0; x<9; x++)
        {
            for(y=0; y<9; y++)
            {
                fscanf(fichier,"%d ", &Plateau[x][y].taillePile); // on lit la taille de la pile dans le fichier pour chaque case du plateau.
            }
        }

        //tour joueur
        fscanf(fichier,"%d",&Jeu->joueurActuel);
        fclose(fichier); //Fermeture du fichier.
        return 1;
    }
    else
    {
        printf("impossible d'ouvrir le fichier\n");
    }
    return 0;
}

/**
 * \fn cleanupTexturesJeu(images_jeu *ImagesJeu, Struct_btn_jeu *Btn_jeu, UI *Ui)
 * \brief Cette fonction permet de détruire l’ensemble des textures.
 *
 * \param *ImagesJeu car on va détruires des textures de cette structure.
 * \param *Btn_jeu car on va détruires des textures de cette structure.
 * \param *Ui car on va détruires des textures de cette structure.
 * \return Rien
 */
void cleanupTexturesJeu(images_jeu *ImagesJeu, Struct_btn_jeu *Btn_jeu, UI *Ui)
{
    SDL_DestroyTexture(ImagesJeu->fond);
    SDL_DestroyTexture(ImagesJeu->plateauJeu);
    SDL_DestroyTexture(ImagesJeu->pion_jaune);
    SDL_DestroyTexture(ImagesJeu->pion_rouge);
    SDL_DestroyTexture(ImagesJeu->surbrillance);
    SDL_DestroyTexture(ImagesJeu->surbrillanceCoupsPossibles);

    SDL_DestroyTexture(Btn_jeu->fondp);
    SDL_DestroyTexture(Btn_jeu->quitter);
    SDL_DestroyTexture(Btn_jeu->reprendre);
    SDL_DestroyTexture(Btn_jeu->sauvegarder);
    SDL_DestroyTexture(Btn_jeu->charger);

    SDL_DestroyTexture(Btn_jeu->save1);
    SDL_DestroyTexture(Btn_jeu->save2);
    SDL_DestroyTexture(Btn_jeu->save3);
    SDL_DestroyTexture(Btn_jeu->save4);

    SDL_DestroyTexture(Ui->joueurGagnant);
    SDL_DestroyTexture(Ui->tourJoueur);

}


