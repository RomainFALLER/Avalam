/**
 * \file moteur.h
 * \brief Fichier permettant la d�finition des structures et des prototype.
 * \author PETIT Sylvain, FALLER Romain, FIRRINCIELI Arnaud
 * \version 2.0
 * \date Janvier-F�vrier 2015
 *
 * Ce fichier permet de d�finir les structures (ou �num�rations) utilis�es dans moteur.c ainsi que les prototypes des fonctions utilis�es dans moteur.c.
 *
 */

#ifndef MOTEUR_H_INCLUDED
#define MOTEUR_H_INCLUDED


#define TAILLE_PLATEAU          9


/**
 * \struct jeu
 * \brief Structure classique correspondante au moteur du jeu.
 * Cette structure permet de d�finir l�ensemble des variables n�cessaires
 * au bon fonctionnement du moteur de jeu (joueur actuel, joueur gagnant, en train de jouer...), elle d�finit �galement les variables n�c�ssaire au bon comptage du score.
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
 * \brief Tableau de structure corrrespondante � la gestin du plateau (niveau moteur).
 * Cette structure permet de d�finir l�ensemble des variables qui nous
 * serviront � caract�riser le plateau lors de la partie (couleur, taille de la pile, surbrillance).
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
void initJeu(jeu *Jeu); /*Initialise la structure jeu au d�but de la partie (pas de joueur gagnant etc...). */

bool sourisSur(SDL_Rect obj, int x, int y); /* Permet de detecter si l'utilisateur survole un objet.*/
void gestionSurbrillanceSurvol(SDL_Event event, images_jeu *ImagesJeu, plateau **Plateau, plateau **PlateauSuivant); /* Gere la surbrilance d'un pion lors sur survole � a souris. */

void copiePlateau(plateau **Plateau, plateau **PlateauSuivant); /* Permet de copier le plateau suivant dans le plateau actuel. */
void afficheTableauDebug(plateau **PlateauSuivant); /* fonction de DEBUG => Apercu de tout le tableau. */

void selectionPion(SDL_Event event, images_jeu *ImagesJeu, plateau **Plateau, plateau **PlateauSuivant, jeu *Jeu); /* Permet de s�l�ctionner un pion (pour ensuite le d�placer). */
void deplacerPions(SDL_Event event, images_jeu *ImagesJeu, plateau **Plateau, plateau **PlateauSuivant, jeu *Jeu); /* Permet de d�placer un pion. */

bool estDansPlateau(int x, int y); /* Permet de d�terminer si on se trouve dans la zone du plateau de jeu. */
void initCoupPossibles(plateau **PlateauSuivant); /* Initialise les coup possibles (soit 0 coups impossibles au d�but de la partie).*/
bool calculCoupsPossibles(int x, int y, plateau **Plateau, plateau **PlateauSuivant); /*Permet de calculer � chaque tour les coups possible par rapport � un pion s�l�ctionn�. */
int lisCouleur(int i, int j, plateau **Plateau); /* Renvoie la couleur du pion selectionn� (ou de la pile selectionn�e). */


bool cherchePionSelectionne(int *k, int *l, plateau **Plateau); /* Permet de detecter si un pion est s�lectionn� et de le mettre en m�moire dans un pointeur. */
bool taillePileOK(int i, int j, int k, int l, plateau **Plateau); /*V�rifie si le d�placement entre 2 piles est possible ou non.*/


bool pionIsole(int x, int y, plateau **Plateau); /* Permet de savoir si le pion/la pile est isol� ou non (ou si la pile d�passe 5).*/
bool partieTerminee(plateau **Plateau, jeu *Jeu); /* Permet de dire si le tour est termin� ou non. */


void CalculScore(); /* Permet de calculer le score en temps direct. */

/**
 * \enum couleur
 * \brief Constantes repr�sentants les diff�rents "types" de pions du plateau.
 *
 * Cette �num�ration permet d�attribuer un entier pour les diff�rents types de pions.
 */
enum couleur {VIDE, ROUGE, JAUNE, SELECTIONNE};



#endif // MOTEUR_H_INCLUDED
