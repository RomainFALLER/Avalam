/**
 * \file moteur.c
 * \brief Fichier contenant l'ensemble des fonctions nécéssaire au MOTEUR DU JEU, soit les fonctions permettant de faire fonctionner le jeu correctement et correspond à la partie technique.
 * \author PETIT Sylvain, FALLER Romain, FIRRINCIELI Arnaud
 * \version 2.0
 * \date Janvier-Mars 2015
 *
 * Ce module regroupe l’ensemble des fonctions permetttant le bon fonctionnement du jeu et assure la partie technique du programme (gestion des événement, algorithmes du jeu...).
 *
 */


#include "defs.h"

/**
* \fn bool sourisSur(SDL_Rect obj, int x, int y)
* \brief Cette fonction retourne vrai si les coordonnées de la souris (x,y) sont dans obj (exemple: survol ou clic d'un bouton).
* \param obj pour les coordonnées de l'objet (position de l'objet avec .x et .y).
* \param x pour l'abscisses souris.
* \param y pour l'ordonnée souris.
* \return true si la souris est sur un bouton (ou un pion).
*/
bool sourisSur(SDL_Rect obj, int x, int y)
{
    // Si la souris est sur un bouton ou un pion (ou si on clique dessus, cela dépend des paramètres passé x et y)
    if( ( x > obj.x ) && ( x < obj.x + obj.w ) && ( y > obj.y ) && ( y < obj.y + obj.h ) )
        return true; // On retourne vrai
    // Sinon
    else
        return false; // on est pas sur le bouton ou le pion
}

/**
* \fn bool cherchePionSelectionne(int *k, int *l, plateau **Plateau)
* \brief Cette fonction permet de chercher le pion sélectionné et place ses coordonnées en mémoire dans deux pointeurs d'entiers.
* \param k pour l'ordonnée pion à déplacer.
* \param l pour l'abscisse pion à déplacer.
* \return true si le pion sélectionné est trouvé, false sinon.
*/
bool cherchePionSelectionne(int *k, int *l, plateau **Plateau)
{
    int i,j;
    for(i=0; i<TAILLE_PLATEAU; i++)
    {
        for(j=0; j<TAILLE_PLATEAU; j++)
        {
            if(Plateau[i][j].estSelectionne == true)
            {
                *k=i;
                *l=j;
                return true;
            }

        }
    }

    return false;
}

/**
* \fn bool taillePileOK(int i, int j, int k, int l, plateau **Plateau)
* \brief Cette fonction vérifie si le déplacement est possible de k;l vers i;j (si la taille de la pile apres l'addition des deux piles ne sera pas > 5).
* \param i pour l'ordonnée du pion où l'on veut déplacer.
* \param j pour l'abscisse du pion où l'on veut déplacer.
* \param k pour l'ordonnée pion à déplacer.
* \param l pour l'abscisse pion à déplacer.
* \return true si le pion sélectionné est trouvé, false si le déplacement est impossible.
*/bool taillePileOK(int i, int j, int k, int l, plateau **Plateau)
{
    if((Plateau[i][j].taillePile + Plateau[k][l].taillePile) > 5)
    {
        return false; // Déplacement impossible
    }
    else
    {
        return true;  // Déplacement possible
    }
}


/**
* \fn deplacerPions(SDL_Event event, image_jeu *ImagesJeu, plateau **Plateau, plateau **PlateauSuivant, jeu *Jeu)
* \brief Cette fonction permet de déplacer un pion, elle appelle de nombreuses fonctions de moteurs.c.
* \param event car elle sera actionnée lors de la selection d'un pion.
* \param *ImagesJeu car elle utilise des variables de la structure images_jeu (pour les pions etc...).
* \param **Plateau qui définit le plateau actuel.
* \param **PlateauSuivant qui définit le plateau une fois le pions déplacé.
* \param *Jeu car elle utilise des variables de la structure jeu (enTrainDeDeplacer etc...). Ceci permet de mettre des booleens a true ce qui nous permettra ensuite de l'utiliser dans avalam.c pour l'affichage.
* \return Rien.
*/
void deplacerPions(SDL_Event event, images_jeu *ImagesJeu, plateau **Plateau, plateau **PlateauSuivant, jeu *Jeu)
{
    int i,j,w,h,k,l;

    // Récupère les coordonnées du pion sélectionné dans k;l
    if(!cherchePionSelectionne(&k, &l, Plateau))
        printf("[!] Erreur: impossible de trouver le pion sélectionné\n");
    else
    {
        for(i=0, w = OFFSET_DEBUTPLATEAU_Y; i < TAILLE_PLATEAU; i++, w += ESPACEMENT_Y)
        {
            ImagesJeu->rect.y = w; // Mise à jour position verticale

            for(j=0, h = OFFSET_DEBUTPLATEAU_X; j < TAILLE_PLATEAU; j++, h += ESPACEMENT_X)
            {
                ImagesJeu->rect.x = h; // Mise à jour position horizontale

                // Déplacement valide
                if(Plateau[i][j].couleur != VIDE && sourisSur(ImagesJeu->rect, event.button.x, event.button.y))
                {
                    // Si on se trouve sur un coup possible
                    if(Plateau[i][j].coupPossible == 1)
                    {
                        // Déplacemenet valide en cours
                        Jeu->enTrainDeDeplacer = true;

                        printf("[*] DEPLACEMENT DE [%d;%d] VERS [%d;%d]\n", l, k, j, i);

                        // Vérifie que on puisse ajouter les piles entre elles
                        if(taillePileOK(i,j,k,l, Plateau))
                        {
                            // La couleur du 'dessus' doit changer
                            PlateauSuivant[i][j].couleur = Plateau[k][l].couleur;
                            // Ajout des pions
                            PlateauSuivant[i][j].taillePile += Plateau[k][l].taillePile;
                            PlateauSuivant[k][l].taillePile = 0;

                            PlateauSuivant[k][l].couleur = VIDE;

                            PlateauSuivant[k][l].estSelectionne = false;
                            PlateauSuivant[i][j].surbrillance = false;

                            printf("[*] Déplacement valide effectué (taillePile=%d)\n", PlateauSuivant[i][j].taillePile);

                            if(Jeu->joueurActuel == J1)
                                Jeu->joueurActuel = J2;
                            else
                                Jeu->joueurActuel = J1;

                        }

                        else
                        {
                            printf("[!] Taille pile supérieure à 5 ! \n");
                            PlateauSuivant[k][l].estSelectionne = false;
                            PlateauSuivant[k][l].surbrillance = false;
                        }

                    }
                    // Coup impossible
                    else
                    {
                        printf("[!] Impossible de déplacer \n");

                        PlateauSuivant[k][l].estSelectionne = false;
                        PlateauSuivant[k][l].surbrillance = false;
                    }


                    Jeu->enTrainDeDeplacer = false;

                }
                else
                {
                    PlateauSuivant[k][l].estSelectionne = false;
                    PlateauSuivant[k][l].surbrillance = false;
                }

            }
        }

        // Déplacement terminé
        initCoupPossibles(PlateauSuivant);
    }
}




/**
* \fn selection(SDL_Event event, images_jeu *ImagesJeu, plateau **Plateau, plateau **PlateauSuivant, jeu *Jeu)
* \brief Cette fonction permet de sélectionner le pion à déplacer (un clic permettra de sélectionner un pion pour ensuite le déplacer).
* \param event car la fonction agira lors d'un clic sur le pion.
* \param *ImagesJeu pour la mise a jour verticale et horizontale lors du parcours du plateau.
* \param **Plateau pour verifier la couleur (soit si ce n'est pas une case vide).
* \param **PlateauSuivant pour mettre à jour les coups possibles etc...
* \param *Jeu pour mettre à true la variable enTrainDeDeplacer de la structure jeu.
*/
void selectionPion(SDL_Event event, images_jeu *ImagesJeu, plateau **Plateau, plateau **PlateauSuivant, jeu *Jeu)
{
    int i,j,w,h;

    if(estDansPlateau(event.button.x, event.button.y)) // Si on est dans le plateau on traite les pions
    {
        for(i=0, w = OFFSET_DEBUTPLATEAU_Y; i < TAILLE_PLATEAU; i++, w += ESPACEMENT_Y)
        {
            ImagesJeu->rect.y = w; // Mise à jour position verticale

            for(j=0, h = OFFSET_DEBUTPLATEAU_X; j < TAILLE_PLATEAU; j++, h += ESPACEMENT_X)
            {
                ImagesJeu->rect.x = h; // Mise à jour position horizontale

                // si il clique sur un pion valide
                if(Plateau[i][j].couleur != VIDE && sourisSur(ImagesJeu->rect, event.button.x, event.button.y))
                {
                    initCoupPossibles(PlateauSuivant);
                    calculCoupsPossibles(i,j, Plateau, PlateauSuivant); // Calcule les coups possibles à partir de i;j
                    PlateauSuivant[i][j].estSelectionne = true;
                    Jeu->enTrainDeDeplacer = true;
                }
                // Déselection si clic sur plateau
                else
                {
                    PlateauSuivant[i][j].estSelectionne = false;
                    PlateauSuivant[i][j].surbrillance = false;
                    //PlateauSuivant[i][j].taillePile = Plateau[i][j].taillePile;
                }
            }
        }
        //        afficheTableauDebug();
    }

}

/**
* \fn bool calculCoupsPossibles(int x, int y, plateau **Plateau, plateau **PlateauSuivant)
* \brief Cette fonction calcule tous les coups possibles à partir de x;y.
* \param x abscisse du Plateau
* \param y ordonnée de Plateau
* \param **Plateau
* \param **PlateauSuivant
* \return false si aucun coup n'est possible, true sinon.
*/
bool calculCoupsPossibles(int x, int y, plateau **Plateau, plateau **PlateauSuivant)
{
    int i,j;
    bool coupPossible = false;

    // Calcul de tous les coups possibles pour le pion sélectionné
    for(i = x-1 ; i <= x+1 ; i++)
    {
        for(j=y-1 ; j <= y+1 ; j++)
        {
            if(lisCouleur(i,j, Plateau) != -1)
            {
                if((Plateau[i][j].taillePile == 5) || (!taillePileOK(i,j,x,y, Plateau)))
                {
                    //printf("\tPile pleine => Déplacement IMPOSSIBLE en %d;%d\n", j,i);
                }

                else if(i != x || j != y) // On ne peut pas se déplacer sur soi-même
                {
                    PlateauSuivant[i][j].coupPossible = 1;
                    // printf("\tDéplacement POSSIBLE en %d;%d\n", j,i);
                    coupPossible = true;
                }
            }

            else
            {
                // printf("\tDéplacement IMPOSSIBLE en %d;%d\n", j,i);
            }
        }
    }

    return coupPossible;
}


/**
* \fn initCoupPossibles(plateau **PlateauSuivant)
* \brief Cette fonction initialise tous les coups possibles à 0, soit on initialise tous les coups à impossibles.
* \param **PlateauSuivant pour initialiser la variable coupPossible de la structure plateau.
* \return Rien.
*/
void initCoupPossibles(plateau **PlateauSuivant)
{
    int i,j;
    for(i = 0 ; i < TAILLE_PLATEAU ; i++)
    {
        for(j = 0 ; j < TAILLE_PLATEAU ; j++)
        {
            PlateauSuivant[i][j].coupPossible = 0;
        }
    }
}


/**
* \fn int lisCouleur(int i, int j, plateau **Plateau)
* \brief Cette fonction renvoie la couleur du pion en (x;y) si possible.
* \param i pour l'abscisse de Plateau.
* \param j pour l'ordonnée de Plateau.
* \param **Plateau pour vérifier la couleur soit que ce n'est pas une case sans couleur (vide).
* \return -1 si il y a dépassement de tableau, et la couleur du pion le plus au dessus de la pile sinon.
*/
int lisCouleur(int i, int j, plateau **Plateau)
{
    if(i >= 0 && i < TAILLE_PLATEAU && j>=0 && j < TAILLE_PLATEAU && Plateau[i][j].couleur != 0)
    {
        return Plateau[i][j].couleur;
    }


    else return -1;
}

/**
* \fn gestionSurbrillanceSurvol(SDL_Event event, images_jeu *ImagesJeu, plateau **Plateau, plateau **PlateauSuivant)
* \brief Cette fonction gère la surbrillance d'un pion lorsqu'on le survole à la souris (pas l'affichage mais detecte quand il y a surbrillance).
* \param event structure événement de la SDL car cela s'actionne lors du survol d'un pion.
* \param *ImagesJeu pour prendre la position des pions.
* \param **Plateau pour voir si on est sur un pion.
* \param **PlateauSuivant pour mettre la surbrillance à true.
* \return Rien.
*/
void gestionSurbrillanceSurvol(SDL_Event event, images_jeu *ImagesJeu, plateau **Plateau, plateau **PlateauSuivant)
{
    int i,j,w,h;
    for(i=0, w=OFFSET_DEBUTPLATEAU_Y; i<TAILLE_PLATEAU; i++, w += ESPACEMENT_Y)
    {
        ImagesJeu->rect.y = w;

        for(j=0, h=OFFSET_DEBUTPLATEAU_X; j<TAILLE_PLATEAU; j++, h += ESPACEMENT_X)
        {
            ImagesJeu->rect.x = h;

            if(Plateau[i][j].couleur != VIDE)
            {
                if(sourisSur(ImagesJeu->rect, event.motion.x, event.motion.y))
                {
                    PlateauSuivant[i][j].surbrillance = true; // on active la surbrillance
                }
                else
                {
                    PlateauSuivant[i][j].surbrillance = false;
                }

            }
            else
            {
                PlateauSuivant[i][j].surbrillance = false;
            }

        }
    }

}




/**
* \fn copiePlateau(plateau **Plateau, plateau **PlateauSuivant)
* \brief Cette fonction permet copier le plateau suivant dans le plateau actuel, soit on copie un nouveau tableau dans
* l’ancien (caractérisant le tour actuel et le tour suivant).
* \param **Plateau car c'est le tableau (plateau) dans lequel on veut copier un nouveau tableau. (plateau actuel).
* \param **PlateauSuivant car c'est le tableau qu'on copie dans l'ancien (plateau suivant qu'on met dans l'ancien).
* \return Rien.
*/
void copiePlateau(plateau **Plateau, plateau **PlateauSuivant)
{
    int i,j;
    // Recopie le nouveau tableau dans l'ancien
    for(i=0; i < TAILLE_PLATEAU; i++)
    {
        for(j=0; j < TAILLE_PLATEAU; j++)
        {
            Plateau[i][j].type = PlateauSuivant[i][j].type;
            Plateau[i][j].coupPossible = PlateauSuivant[i][j].coupPossible;
            Plateau[i][j].couleur = PlateauSuivant[i][j].couleur;
            Plateau[i][j].surbrillance = PlateauSuivant[i][j].surbrillance;
            Plateau[i][j].taillePile = PlateauSuivant[i][j].taillePile;
            Plateau[i][j].estSelectionne = PlateauSuivant[i][j].estSelectionne;
        }
    }

}

/**
* \fn afficheTableauDebug(plateau **PlateauSuivant)
* \brief Cette fonction permet d’afficher tout le tableau, elle nous permet de voir plus facilement où
* sont les erreurs pendant le développement du programme (bugs, coups possibles etc...).
* \param **PlateauSuivant car on veut voir les coups possibles.
* \return Rien.
*/
void afficheTableauDebug(plateau **PlateauSuivant)
{
    short int i,j;
    /////////////////////////////DEBUG
    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            //printf("%d ", Plateau[i][j].couleur);
        }

        //printf("\n");
    }
    printf("\n");
    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            printf("%d ", PlateauSuivant[i][j].coupPossible);
        }

        printf("\n");
    }
////////////////////////////
}

/**
* \fn initJeu(jeu *Jeu)
* \brief Cette fonction permet d'initialiser la structure jeu au début de la partie (en effet, aucun joueur gagnant au début etc...).
* \param *Jeu car c'est la structure initialisée.
* \return Rien.
*/void initJeu(jeu *Jeu)
{
    Jeu->joueurActuel = J1; // Le joueur 1 commencera la partie
    Jeu->joueurGagnant = 0; // Pas de joueur gagnant au début
    Jeu->scoreJoueurJaune = 0;
    Jeu->scoreJoueurRouge = 0;
    Jeu->enTrainDeDeplacer = false;
}


/**
* \fn initPlateau(plateau **Plateau, plateau **PlateauSuivant)
* \brief Cette fonction permet d’initialiser le tableau 2D caractérisant le plateau. Soit on met des 0 pour les cases
* où y il a un pion et des -1 la où il n’y a pas de pion (vu que le plateau n’est pas carré).
* \param **Plateau car on initialise la structure plateau.
* \param **PlateauSuivant pour copier plateau dans un autre plateau (plateauSuivant).
* \return Rien.
*/
void initPlateau(plateau **Plateau, plateau **PlateauSuivant)
{
    int i,j;

    // Forme du plateau
    int init[TAILLE_PLATEAU][TAILLE_PLATEAU] = {{-1,-1,-1,-1,0,0,-1,-1,-1},
        {-1,0,0,0,0,0,-1,-1,-1},
        {0,0,0,0,0,0,0,-1,-1},
        {0,0,0,0,0,0,0,-1,-1},
        {-1,0,0,0,-1,0,0,0,-1},
        {-1,-1,0,0,0,0,0,0,0},
        {-1,-1,0,0,0,0,0,0,0},
        {-1,-1,-1,0,0,0,0,0,-1},
        {-1,-1,-1,0,0,-1,-1,-1,-1}
    };

    // Remplis le tableau initial
    for(i=0; i < TAILLE_PLATEAU; i++)
    {
        for(j=0; j < TAILLE_PLATEAU; j++)
        {
             // Recopie la forme du plateau dans la structure
            Plateau[i][j].type = init[i][j];
            Plateau[i][j].estSelectionne = false;
            Plateau[i][j].surbrillance = false;
            Plateau[i][j].pointGagne = VIDE;
            Plateau[i][j].coupPossible = 0;

            if(Plateau[i][j].type == 0) // Si c'est l'emplacement d'un pion
            {
                Plateau[i][j].taillePile = 1;

                if(j % 2 == 0) // On place des pions rouges sur les emplacements pairs
                    Plateau[i][j].couleur = ROUGE; // On place un pion
                else
                    Plateau[i][j].couleur = JAUNE; // On place un pion en jaune
            }

            else
            {
                Plateau[i][j].couleur = VIDE;
                Plateau[i][j].taillePile = 0;
            }


            PlateauSuivant[i][j] = Plateau[i][j];

        }
    }

}


/**
* \fn bool partieTerminee(plateau **Plateau, jeu *Jeu)
* \brief Cette fonction permet de savoir si la partie est terminé ou non.
* \param **Plateau car on utilise la fonction pionIsole et on regarde la taille des piles donc on a besoin de Plateau.
* \param *Jeu pour attribuer les points (les scores qui se trouvent dans la structure jeu).
* \return true si la partie est terminée false si elle ne l'est pas.
*/
bool partieTerminee(plateau **Plateau, jeu *Jeu)
{
    int i,j;
    // On suppose la partie terminée
    bool partieTermine = true;

    // Parcourt le plateau
    for(i=0; i<TAILLE_PLATEAU; i++)
    {
        for(j=0; j<TAILLE_PLATEAU; j++)
        {
            // Si un pion est isolé ou sa taille de pile est égale à 5
            if(pionIsole(i,j, Plateau) || ((Plateau[i][j].taillePile == 5)))
            {
                if(Plateau[i][j].couleur == JAUNE && Plateau[i][j].pointGagne == VIDE)
                {
                    Jeu->scoreJoueurJaune++;
                    Plateau[i][j].pointGagne = JAUNE;
                }
                else if(Plateau[i][j].couleur == ROUGE && Plateau[i][j].pointGagne == VIDE)
                {
                    Jeu->scoreJoueurRouge++;
                    Plateau[i][j].pointGagne = ROUGE;
                }
            }
            else if(Plateau[i][j].couleur != VIDE)
            {
                // Si au moins un pion n'est pas isolé
                partieTermine = false;
            }
        }
    }

    return partieTermine;
}


/**
* \fn bool pionIsole(int x, int y, plateau **Plateau)
* \brief Cette fonction détermine si le pion est isolé ou non.
* \param x pour l'abscisse dans le plateau.
* \param y pour l'ordonnée dans le plateau.
* \param **Plateau car on utilise les fonctions lisCouleur et taillePileOK qui ont besoin de Plateau en parametre d'entrée.
* \return true si le pion est isolé, false s'il ne l'est pas.
*/
bool pionIsole(int x, int y, plateau **Plateau)
{
    int i,j;
    bool pionEstIsole = true; // On suppose le pion isolé

    for(i = x - 1 ; i <= x + 1 ; i++)
    {
        for(j = y - 1 ; j <= y + 1 ; j++)
        {
            if((lisCouleur(i,j, Plateau) != -1) && (i != x || j != y) && taillePileOK(i,j,x,y,Plateau))
            {
                pionEstIsole = false;
            }
        }
    }

    return pionEstIsole;
}


/**
* \fn bool estDansPlateau(int x, int y)
* \brief Cette fonction détermine si (x;y) est dans la zone du plateau de jeu.
* \param x pour l'abscisse en pixel.
* \param y pour l'ordonnée en pixel.
* \return true si x;y est un point du plateau de jeu, false sinon
*/
bool estDansPlateau(int x, int y)
{
    // Si les coordonnees sont valides
    if ((x >= OFFSET_DEBUTPLATEAU_X) && (x < OFFSET_DEBUTPLATEAU_X + (ESPACEMENT_X * TAILLE_PLATEAU))
            && (y >= OFFSET_DEBUTPLATEAU_Y) && (y < OFFSET_DEBUTPLATEAU_Y + (ESPACEMENT_Y * TAILLE_PLATEAU) ))
    {
        return true;
    }

    else return false;
}


