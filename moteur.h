/**
 * \file moteur.h
 * \brief Fichier permettant la définition des structures et des prototype.
 * \author PETIT Sylvain, FALLER Romain, FIRRINCIELI Arnaud
 * \version 2.0
 * \date Janvier-Février 2015
 *
 * Ce fichier permet de définir les structures (ou énumérations) utilisées dans moteur.c ainsi que les prototypes des fonctions utilisées dans moteur.c.
 *
 */

#ifndef MOTEUR_H_INCLUDED
#define MOTEUR_H_INCLUDED


#define TAILLE_PLATEAU          9


/**
 * \struct jeu
 * \brief Structure classique correspondante au moteur du jeu.
 * Cette structure permet de définir l’ensemble des variables nécessaires
 * au bon fonctionnement du moteur de jeu (joueur actuel, joueur gagnant, en train de jouer...), elle définit également les variables nécéssaire au bon comptage du score.
 */
 typedef struct jeu
{
    short unsigned int nbCoupsPossibles; // maximum de 8 coups possibles
    bool enTrainDeDeplacer;
    short unsigned int mode_jeu;

    int joueurActuel;
    int scoreJoueurRouge; // J1
    int scoreJoueurJaune; // J2
    int joueurGagnant;

} jeu;

/**
 * \struct plateau
 * \brief Tableau de structure corrrespondante à la gestin du plateau (niveau moteur).
 * Cette structure permet de définir l’ensemble des variables qui nous
 * serviront à caractériser le plateau lors de la partie (couleur, taille de la pile, surbrillance).
 */
 typedef struct plateau
{
    int coupPossible;
    int couleur;
    int taillePile;
    int type;
    int pointGagne;

    bool surbrillance;
    bool estSelectionne;

} plateau;


void initPlateau(plateau **Plateau, plateau **PlateauSuivant); /* initialise le plateau (selon les cases vides...).*/
void initJeu(jeu *Jeu); /*Initialise la structure jeu au début de la partie (pas de joueur gagnant etc...). */

bool sourisSur(SDL_Rect obj, int x, int y); /* Permet de detecter si l'utilisateur survole un objet.*/
void gestionSurbrillanceSurvol(SDL_Event event, images_jeu *ImagesJeu, plateau **Plateau, plateau **PlateauSuivant); /* Gere la surbrilance d'un pion lors sur survole à a souris. */

void copiePlateau(plateau **Plateau, plateau **PlateauSuivant); /* Permet de copier le plateau suivant dans le plateau actuel. */
void afficheTableauDebug(plateau **PlateauSuivant); /* fonction de DEBUG => Apercu de tout le tableau. */

void selectionPion(SDL_Event event, images_jeu *ImagesJeu, plateau **Plateau, plateau **PlateauSuivant, jeu *Jeu); /* Permet de séléctionner un pion (pour ensuite le déplacer). */
void deplacerPions(SDL_Event event, images_jeu *ImagesJeu, plateau **Plateau, plateau **PlateauSuivant, jeu *Jeu); /* Permet de déplacer un pion. */

bool estDansPlateau(int x, int y); /* Permet de déterminer si on se trouve dans la zone du plateau de jeu. */
void initCoupPossibles(plateau **PlateauSuivant); /* Initialise les coup possibles (soit 0 coups impossibles au début de la partie).*/
bool calculCoupsPossibles(int x, int y, plateau **Plateau, plateau **PlateauSuivant); /*Permet de calculer à chaque tour les coups possible par rapport à un pion séléctionné. */
int lisCouleur(int i, int j, plateau **Plateau); /* Renvoie la couleur du pion selectionné (ou de la pile selectionnée). */


bool cherchePionSelectionne(int *k, int *l, plateau **Plateau); /* Permet de detecter si un pion est sélectionné et de le mettre en mémoire dans un pointeur. */
bool taillePileOK(int i, int j, int k, int l, plateau **Plateau); /*Vérifie si le déplacement entre 2 piles est possible ou non.*/


bool pionIsole(int x, int y, plateau **Plateau); /* Permet de savoir si le pion/la pile est isolé ou non (ou si la pile dépasse 5).*/
bool partieTerminee(plateau **Plateau, jeu *Jeu); /* Permet de dire si le tour est terminé ou non. */


void CalculScore(); /* Permet de calculer le score en temps direct. */

/**
 * \enum couleur
 * \brief Constantes représentants les différents "types" de pions du plateau.
 *
 * Cette énumération permet d’attribuer un entier pour les différents types de pions.
 */
enum couleur {VIDE, ROUGE, JAUNE, SELECTIONNE};



#endif // MOTEUR_H_INCLUDED
