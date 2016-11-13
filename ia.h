/**
 * \file ia.h
 * \brief Prototype et structure IA
 * \author PETIT Sylvain, FALLER Romain, FIRRINCIELI Arnaud
 * \version 1.1
 * \date Mars 2015
 *
 */

#ifndef IA_H_INCLUDED
#define IA_H_INCLUDED



/**
 * \struct coup
 * \brief Structure contenant les données d'un coup possible pour l'IA.
 * Cette structure définit la couleur et la taille de la pile du pion sélectionné et du pion cible
 */
typedef struct coup
{
    int couleur_pionSelectionne;
    int taille_pile_pionSelectionne;

    int couleur_pionCible;
    int taille_pile_pionCible;

} coup;


int eval(plateau **PlateauIA, const short int joueurActuel, int lvl); // Fonction d'évaluation

int MiniMax(plateau **PlateauIASuivant, int profondeur, int lvl, bool maximization);

int AlphaBeta(plateau **PlateauIASuivant, int profondeur, int lvl, int alpha, int beta, bool maximization, bool modeDemo);

void IA_ReflechieEtJoue(plateau **Plateau, int profondeur, int lvl, int *yOrigine, int *xOrigine, int *yDestination, int *xDestination, bool modeDemo);

void simuleCoup(plateau **PlateauIASuivant, coup *Coup, int i,int j,int y,int x);
void annuleCoup(plateau **PlateauIASuivant, coup *Coup, int i, int j, int y, int x);

#endif // IA_H_INCLUDED
