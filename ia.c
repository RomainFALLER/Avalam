/**
 * \file ia.c
 * \brief Contient les fonctions de l'intelligence artificielle
 * \author PETIT Sylvain, FALLER Romain, FIRRINCIELI Arnaud
 * \date Janvier-F�vrier 2015
 */

#include "defs.h"


/**
* \fn int eval(plateau **PlateauIA, const short int joueurActuel)
* \brief Evalue le plateau en fonction du joueur actuel
* \param **PlateauIA plateau de jeu temporaire
* \param joueurActuel le joueur actuel (JAUNE ou ROUGE)
* \param lvl difficult�e actuelle
* \return valeur du jeu (nombre entier compris entre -10000 et +10000) favorisant le joueur actuel
*/
int eval(plateau **PlateauIA, const short int joueurActuel, int lvl)
{
    short int ADVERSAIRE, MOI = joueurActuel;
    if(MOI == JAUNE)
        ADVERSAIRE = ROUGE;
    else
        ADVERSAIRE = JAUNE;

    bool partieTermine = true;
    int poidsIA = 0, scoreMoi = 0, scoreAdversaire = 0;
    int i,j,k,l;
    int nbPionsJaunes = 0, nbPionsRouges = 0;
    int poidsVoisins = 0;
    int poidsEspacesVidesBordDuPlateau = 0;

    // COEFFS
    const int tauxPionIsole = 12; // Les pions isol�s c'est le top

    int coeffNombrePionsPossede; // Permet de rendre l'IA plus forte en d�but de partie
    int tauxVoisins;
    int taux5Pions;
    if(lvl == 3)
    {
        coeffNombrePionsPossede = 10; // Assez important
        tauxVoisins = 8;
        taux5Pions = 4;
    }
    else if(lvl == 2)
    {
        coeffNombrePionsPossede = 1;
        tauxVoisins = 2;
        taux5Pions = 12;
    }
    else
    {
        coeffNombrePionsPossede = 0;
        tauxVoisins = 1;
        taux5Pions = 2;
    }

    const int tauxPionSurBordDuPlateau = 4; // Favorise les pions � proximit� des espaces vides
    const int taux5PionsVoisins = 3; // Favorise les piles pleines voisines

    for(i=0 ; i < TAILLE_PLATEAU ; i++)
    {
        for(j = 0 ; j < TAILLE_PLATEAU ; j++)
        {
/* DEBUT Cas des tours "verrouill�es" */
            if(pionIsole(i,j, PlateauIA) && PlateauIA[i][j].taillePile < 5)
            {
                if(PlateauIA[i][j].couleur == MOI)
                {
                    poidsIA += tauxPionIsole;
                }
                else if(PlateauIA[i][j].couleur == ADVERSAIRE)
                {
                    poidsIA -= tauxPionIsole;
                }
            }
            // Si la tour est "verouill�"
            if(PlateauIA[i][j].taillePile == 5)
            {
                if(PlateauIA[i][j].couleur == MOI)
                {
                    poidsIA += taux5Pions;
                }
                else if(PlateauIA[i][j].couleur == ADVERSAIRE)
                {
                    poidsIA -= taux5Pions;
                }
            }
/* FIN Cas des tours "verrouill�es" */

            // Compte le nombre de pions
            if(PlateauIA[i][j].couleur == JAUNE)
                nbPionsJaunes++;
            else if(PlateauIA[i][j].couleur == ROUGE)
                nbPionsRouges++;

/* DEBUT Prise en charge des voisins */
            // Parcours les voisins
            poidsVoisins = 0;
            for(k = i - 1 ; k <= i + 1 ; k++)
            {
                for(l = j - 1 ; l <= j + 1 ; l++)
                {
                    if((lisCouleur(k,l, PlateauIA) != -1) && (i != k || j != l) && PlateauIA[k][l].couleur != VIDE)
                    {
                        // Peut faire une tour pleine avec ses pions
                        if(PlateauIA[i][j].taillePile + PlateauIA[k][l].taillePile == 5 && PlateauIA[i][j].couleur == MOI)
                        {
                            if(PlateauIA[k][l].couleur == MOI)
                            {
                                poidsIA += taux5PionsVoisins;
                            }

                            // Voisin ennemi
                            else if(PlateauIA[k][l].couleur == ADVERSAIRE)
                            {
                                poidsIA -= taux5PionsVoisins; // Ce plateau sera tr�s p�nalisant
                            }

                        }

                        // Voisin de m�me couleur => bon plan
                        if(PlateauIA[k][l].couleur == MOI && PlateauIA[i][j].couleur == MOI)
                        {
                            if(PlateauIA[i][j].taillePile == 5)
                            {
                                poidsVoisins--;
                            }
                            else
                            {
                                poidsVoisins++;
                            }
                        }


                        // Voisin adverse => pas bon
                        else if(PlateauIA[k][l].couleur == ADVERSAIRE && PlateauIA[i][j].couleur == MOI)
                        {
                            poidsVoisins--;
                        }

                    }
                    else if((lisCouleur(k,l, PlateauIA) == -1) && (i != k || j != l) ) // Signifie que le pion est sur le bord du plateau
                    {
                        if(PlateauIA[i][j].couleur == MOI)
                            poidsEspacesVidesBordDuPlateau++;
                        else if(PlateauIA[i][j].couleur == ADVERSAIRE)
                            poidsEspacesVidesBordDuPlateau++;
                    }
                }
            }
/* FIN Prise en charge des voisins */

            // Il y a plus de voisins amis que ennemi => bon coup
            if(poidsVoisins > 1)
            {
                poidsIA += tauxVoisins;
            }
            else
            {
                poidsIA -= (tauxVoisins);
            }

            // Il y a plus de pions amis sur le bord du plateau que de pions ennemis => bon coup
            if(poidsEspacesVidesBordDuPlateau > 1)
            {
                poidsIA += tauxPionSurBordDuPlateau;
            }
            else
            {
                poidsIA -= tauxPionSurBordDuPlateau;
            }



/* GESTION DU SCORE FINAL ICI */
            // Si un pion est isol� ou sa taille de pile est �gale � 5
            if(pionIsole(i,j, PlateauIA) || ((PlateauIA[i][j].taillePile == 5)))
            {
                if(PlateauIA[i][j].couleur == MOI)
                {
                    scoreMoi++;
                }
                else if(PlateauIA[i][j].couleur == ADVERSAIRE)
                {
                    scoreAdversaire++;
                }
            }
            else if(PlateauIA[i][j].couleur != VIDE)
            {
                // Si au moins un pion n'est pas isol�
                partieTermine = false; // La partie n'est pas finie
            }

        }
    }

/* Si la partie est termin�e (plus aucun coups possibles */
    if(partieTermine) // Plus aucun coup n'est possible
    {
        if(scoreAdversaire > scoreMoi)
        {
            return -10000 + scoreMoi; // J1
        }
        else if(scoreMoi > scoreAdversaire)
        {
            return 10000 - scoreAdversaire; // IA
        }
        else
        {
            return 0; // Egalit�
        }
    }

    if(joueurActuel == JAUNE)
        return poidsIA + (nbPionsJaunes - nbPionsRouges)*coeffNombrePionsPossede;
    else
        return poidsIA + (nbPionsRouges - nbPionsJaunes)*coeffNombrePionsPossede;
}


/**
* \fn int partieTermineeIA(plateau **PlateauIA)
* \brief Indique si la partie est termin�e dans le cadre d'une simulation
* \param **PlateauIA plateau de jeu temporaire
* \return 0 si la partie n'est pas termin�e
*/
int partieTermineeIA(plateau **PlateauIA)
{
    int i,j, scoreIA = 0, scoreJoueur = 0;
    // On suppose la partie termin�e
    bool partieTermine = true;

    // Parcourt le plateau
    for(i=0 ; i < TAILLE_PLATEAU ; i++)
    {
        for(j = 0 ; j < TAILLE_PLATEAU ; j++)
        {
            // Si un pion est isol� ou sa taille de pile est �gale � 5
            if(pionIsole(i,j, PlateauIA) || ((PlateauIA[i][j].taillePile == 5)))
            {
                if(PlateauIA[i][j].couleur == JAUNE)
                {
                    scoreIA++;
                }
                else if(PlateauIA[i][j].couleur == ROUGE)
                {
                    scoreJoueur++;
                }
            }
            else if(PlateauIA[i][j].couleur != VIDE)
            {
                // Si au moins un pion n'est pas isol�
                partieTermine = false; // La partie n'est pas finie
            }
        }
    }

    if(partieTermine) // Plus aucun coup n'est possible
    {
        return 1;
    }

    return 0; // On retourne 0 si la partie n'est pas termin�e
}


/**
* \fn int AlphaBeta(plateau **PlateauIASuivant, int profondeur, int alpha, int beta, bool maximization)
* \brief Equivalent de la fonction MiniMax mais avec �lagage Alpha-Beta
* \param **PlateauIA plateau de jeu temporaire
* \param profondeur
* \param alpha
* \param beta
* \param maximization
* \return best si aucun �lagage n'a �t� fait. Retourne alpha s'il y a �lagage alpha ou beta s'il y a �lagage beta.
*/

int AlphaBeta(plateau **PlateauIASuivant, int profondeur, int lvl, int alpha, int beta, bool maximization, bool modeDemo)
{
    /* l'�lagage alpha beta �vite d'�valuer des noeuds dont on est s�r que leur qualit� sera inf�rieure � un noeud d�j� �valu�
     * alpha < beta
     */

    // Si on est sur une feuille ou si la partie est termin�e on �value
    if(profondeur == 0 || partieTermineeIA(PlateauIASuivant)!=0)
    {
        if(maximization)
        {
            if(modeDemo) // L'IA joue contre elle m�me
                return eval(PlateauIASuivant, ROUGE, lvl);
            else // Mode JCIA
                return eval(PlateauIASuivant, JAUNE, lvl); // L'IA simule son d�placement
        }
        else
        {
            if(modeDemo) // L'IA joue contre elle m�me
                return eval(PlateauIASuivant, JAUNE, lvl); // L'IA simule son d�placement
            else // Mode JCIA
                return eval(PlateauIASuivant, ROUGE, lvl); // L'IA simule le d�placement du joueur
        }

    }

    int best; // Meilleur des coups temporaires
    if(maximization)
        best = alpha;
    else
        best = beta;

    int i,j,tmp;
    int x,y;

    coup Coup;

    for(i=0 ; i < TAILLE_PLATEAU ; i++)
    {
        for(j = 0 ; j < TAILLE_PLATEAU ; j++)
        {
            if(PlateauIASuivant[i][j].couleur != VIDE)
            {
                if(calculCoupsPossibles(i, j, PlateauIASuivant, PlateauIASuivant) != false)  // Au moins un coup est possible
                {
                    for(y = i-1; y <= i+1 ; y++)
                    {
                        for(x = j-1 ; x <= j+1 ; x++)
                        {
                            if((lisCouleur(y,x, PlateauIASuivant) != -1) && PlateauIASuivant[y][x].coupPossible == 1) // Si au moins un coup est possible
                            {
                                simuleCoup(PlateauIASuivant, &Coup, i, j, y, x);

                                /////////////////
                                if(maximization)
                                {
                                    tmp = AlphaBeta(PlateauIASuivant, profondeur-1, lvl, best, beta, false, modeDemo);
                                    if(tmp > best || ( (tmp == best) && (rand()%2 == 0) )) // Ajoute une l�g�re dimension al�atoire
                                    {
                                        best = tmp;
                                    }
                                    if(best >= beta) // On coupe la branche
                                    {
                                        annuleCoup(PlateauIASuivant, &Coup, i, j, y, x);
                                        initCoupPossibles(PlateauIASuivant);
                                        return beta;
                                    }

                                }
                                else // On minimise les d�gats
                                {
                                    tmp = AlphaBeta(PlateauIASuivant, profondeur-1, lvl, alpha, best, true, modeDemo);
                                    if(tmp < best || ( (tmp == best) && (rand()%2 == 0) )) // Ajoute une l�g�re dimension al�atoire
                                    {
                                        best = tmp;
                                    }
                                    if(best <= alpha)
                                    {
                                        annuleCoup(PlateauIASuivant, &Coup, i, j, y, x);
                                        initCoupPossibles(PlateauIASuivant);
                                        return alpha; // On coupe
                                    }


                                }
                                //////////////////

                                // Annule le coup simul� sur les 2 pions
                                annuleCoup(PlateauIASuivant, &Coup, i, j, y, x);

                            }
                        }
                    }
                }
                initCoupPossibles(PlateauIASuivant);
            }
        }
    }

    return best;
}



/**
* \fn int MiniMax(plateau **PlateauIASuivant, int profondeur, bool maximization)
* \brief Visite l'arbre de jeu pour faire remonter � la racine une valeur (appel�e valeur du jeu) qui est calcul�e r�cursivement
* \param **PlateauIA plateau de jeu temporaire
* \param profondeur
* \param maximization
* \return best
*/
int MiniMax(plateau **PlateauIASuivant, int profondeur, int lvl, bool maximization)
{
    // Si on est sur une feuille ou si la partie est termin�e on �value
    if(profondeur == 0 || partieTermineeIA(PlateauIASuivant) != 0)
    {
        if(maximization)
            return eval(PlateauIASuivant, lvl, JAUNE); // L'IA �value son d�placement
        else
            return eval(PlateauIASuivant, lvl, ROUGE); // L'IA �value le d�placement du joueur
    }

    int best; // Meilleur des coups temporaires
    if(maximization)
        best = -10000;
    else
        best = 10000;

    int i,j,tmp;
    int x,y;
    coup Coup; // COUCOU


    //copiePlateau(PlateauIA, PlateauIASuivant);
    for(i=0 ; i < TAILLE_PLATEAU ; i++)
    {
        for(j = 0 ; j < TAILLE_PLATEAU ; j++)
        {
            if(PlateauIASuivant[i][j].couleur != VIDE)
            {
                if(calculCoupsPossibles(i, j, PlateauIASuivant, PlateauIASuivant) != false) // Au moins un coup est possible
                {
                    //PlateauIASuivant[i][j].estSelectionne = true; // Pion actuel � d�placer
                    // On cherche tous les coups possibles (en x;y) sur le pion actuel en (i,j)
                    for(y = i-1; y <= i+1 ; y++)
                    {
                        for(x = j-1 ; x <= j+1 ; x++)
                        {
                            if((lisCouleur(y,x, PlateauIASuivant) != -1) && !(x==y && i==j) && PlateauIASuivant[y][x].coupPossible == 1) // Si au moins un coup est possible
                            {
                                simuleCoup(PlateauIASuivant, &Coup, i, j, y, x);

                                /////////////////
                                if(maximization)
                                {
                                    tmp = MiniMax(PlateauIASuivant, profondeur - 1, lvl, false);
                                    if(tmp > best || ( (tmp == best) && (rand()%2 == 0) )) // Ajoute dimension al�atoire || ( (tmp == best) && (rand()%2 == 0) )
                                    {
                                        best = tmp;
                                    }
                                }
                                else
                                {
                                    tmp = MiniMax(PlateauIASuivant, profondeur - 1, lvl, true);
                                    if(tmp < best || ( (tmp == best) && (rand()%2 == 0) ))
                                    {
                                        best = tmp;
                                    }
                                }


                                // Annule le coup simul� sur les 2 pions
                                annuleCoup(PlateauIASuivant, &Coup, i, j, y, x);


                            }
                        }
                    }
                }
                initCoupPossibles(PlateauIASuivant);
            }
        }
    }

    return best;
}


/**
* \fn void IA_ReflechieEtJoue(plateau **Plateau, int profondeur)
* \brief Simule les d�placements et appelle soit MiniMax soit alphaBeta
* \param **Plateau Plateau de jeu r�el
* \param profondeur d�sir�e d'exploration de l'arbre
* \param modeDemo vaut vrai si le mode d�mo est activ�
* \return void
*/
void IA_ReflechieEtJoue(plateau **Plateau, int profondeur, int lvl, int *yOrigine, int *xOrigine, int *yDestination, int *xDestination, bool modeDemo)
{
    int i,j,x,y;
    int max = -10000, tmp=0, max_y=0, max_x=0, max_k=0, max_l=0;
    int maxTaille_pile_pionCibleFinal, maxCouleurPionCibleFinal;
    // Tableaux pour simuler les d�placements de l'IA
    plateau **PlateauIASuivant;

    coup Coup; // Structure pour stocker le coup actuel

    // Allocation 1�re dimension
    PlateauIASuivant = malloc(sizeof(plateau*) * TAILLE_PLATEAU);
    for(i=0 ; i < TAILLE_PLATEAU ; i++) // Allocation 2�me dimension
    {
        PlateauIASuivant[i] = malloc(sizeof(plateau) * TAILLE_PLATEAU);
    }

    if(PlateauIASuivant == NULL)
    {
        printf("\n[!] erreur malloc IA");
        exit(0);
    }
    if(lvl == 2)
    {
        profondeur = 3;
    }


    // Initialise le plateau de l'IA
    copiePlateau(PlateauIASuivant, Plateau);
    srand (time(NULL));

    for(i = 0; i < TAILLE_PLATEAU; i++)
    {
        for(j = 0; j < TAILLE_PLATEAU; j++)
        {
            if(PlateauIASuivant[i][j].couleur != VIDE)
            {
                if(calculCoupsPossibles(i, j, PlateauIASuivant, PlateauIASuivant) != false) // Au moins un coup est possible
                {
                    // On cherche tous les coups possibles (en x;y) sur les voisins
                    for(y = i-1; y <= i+1 ; y++)
                    {
                        for(x = j-1 ; x <= j+1 ; x++)
                        {
                            if((lisCouleur(y,x, PlateauIASuivant) != -1) && !(x == y && i == j) && PlateauIASuivant[y][x].coupPossible == 1) // Si au moins un coup est possible
                            {
                                // Simulation du coup
                                simuleCoup(PlateauIASuivant, &Coup, i, j, y, x);

                                // Calcul du plateau � la profondeur inf�rieure
                                tmp = AlphaBeta(PlateauIASuivant, profondeur - 1, lvl, -10000, 10000, true, modeDemo);

                                // Si le coup actuel est meilleur que le pr�c�dent, on  m�morise toutes ses caract�ristiques
                                if(tmp >= max)
                                {
                                    max = tmp;

                                    max_y = y;
                                    max_x = x;

                                    max_k = i;
                                    max_l = j;

                                    maxTaille_pile_pionCibleFinal = Coup.taille_pile_pionCible + Coup.taille_pile_pionSelectionne;
                                    maxCouleurPionCibleFinal = Coup.couleur_pionSelectionne;
                                }

                                annuleCoup(PlateauIASuivant, &Coup, i, j, y, x); // Annule le coup simul� pr�c�demment
                            }
                        }
                    }
                }
                initCoupPossibles(PlateauIASuivant);
            }
        }
    }

    printf("\n [IA] Poids du coup max final=%d ", max);
    printf("\n [IA] Deplace de %d;%d vers %d;%d ", max_l,max_k,max_x,max_y);

    // L'IA d�place finalement le pion apr�s m�re r�flexion sur le plateau de jeu

    // Affectation pion(s) cible
    Plateau[max_y][max_x].couleur = maxCouleurPionCibleFinal;
    Plateau[max_y][max_x].taillePile = maxTaille_pile_pionCibleFinal;

    // Affectation pion(s) � d�placer
    Plateau[max_k][max_l].taillePile = 0;
    Plateau[max_k][max_l].couleur = VIDE;

    /* Mise � jour coordon�es pour l'animation */
    *xOrigine = max_l;
    *yOrigine = max_k;
    *xDestination = max_x;
    *yDestination =  max_y;

    for(i=0 ; i < TAILLE_PLATEAU ; i++) // D�sallocation 2�me dimension
    {
        free(PlateauIASuivant[i]);
    }
    free(PlateauIASuivant);
}


/**
* \fn void simuleCoup(plateau **PlateauIASuivant, coup *Coup, int i, int j, int y, int x)
* \brief Simule un coup
* \param **PlateauIASuivant Plateau IA temporaire
* \param *Coup coup actuel
* \param i ordonn�e pions � d�placer
* \param j abscisse pions � d�placer
* \param x ordonn�e pions cible
* \param y abscisse pions cible
* \return void
*/
void simuleCoup(plateau **PlateauIASuivant, coup *Coup, int i, int j, int y, int x)
{
    Coup->couleur_pionSelectionne = PlateauIASuivant[i][j].couleur;
    Coup->taille_pile_pionSelectionne = PlateauIASuivant[i][j].taillePile;

    Coup->taille_pile_pionCible = PlateauIASuivant[y][x].taillePile;
    Coup->couleur_pionCible = PlateauIASuivant[y][x].couleur;

    PlateauIASuivant[y][x].couleur = PlateauIASuivant[i][j].couleur;
    PlateauIASuivant[y][x].taillePile += PlateauIASuivant[i][j].taillePile;

    PlateauIASuivant[i][j].taillePile = 0;
    PlateauIASuivant[i][j].couleur = VIDE;
}

/**
* \fn void annuleCoup(plateau **PlateauIASuivant, coup *Coup, int i, int j, int y, int x)
* \brief Annule un coup simul�
* \param **PlateauIASuivant Plateau IA temporaire
* \param *Coup coup actuel
* \param i ordonn�e pions � d�placer
* \param j abscisse pions � d�placer
* \param x ordonn�e pions cible
* \param y abscisse pions cible
* \return void
*/
void annuleCoup(plateau **PlateauIASuivant, coup *Coup, int i, int j, int y, int x)
{
    // Annule le coup simul� sur les 2 pions
    PlateauIASuivant[i][j].couleur = Coup->couleur_pionSelectionne;
    PlateauIASuivant[i][j].taillePile = Coup->taille_pile_pionSelectionne;

    PlateauIASuivant[y][x].couleur = Coup->couleur_pionCible;
    PlateauIASuivant[y][x].taillePile = Coup->taille_pile_pionCible;

}



