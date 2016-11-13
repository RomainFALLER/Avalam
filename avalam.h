/**
 * \file avalam.h
 * \brief Fichier permettant la définition des structures et des prototype.
 * \author PETIT Sylvain, FALLER Romain, FIRRINCIELI Arnaud
 * \version 3.0
 * \date Janvier 2015
 *
 * Ce fichier permet de définir les structures (ou énumérations) utilisées dans avalam.c ainsi que les prototypes des fonctions utilisées dans avalam.c.
 *
 */

#ifndef AVALAM_H_INCLUDED
#define AVALAM_H_INCLUDED


#define AFFICHE_GRILLE_DEBUG    0 /* Constante de DEBUG --> Mettre à 1 pour afficher toute la grille. */


#define TAILLE_POLICE           18 /* Constante pour la taille de la police. */

#define LARGEUR_PLATEAU         800 /* Constante pour la largeur du plateau de jeu. */
#define HAUTEUR_PLATEAU         600 /* Constante pour la hauteur du plateau de jeu. */

// Offset en pixels d'affichage du tableau
#define OFFSET_DEBUTPLATEAU_X   100 /* Constante définissant l'abscisse d'ou commence l'affichage du plateau. */
#define OFFSET_DEBUTPLATEAU_Y   50  /* Constante définissant l'ordonnée d'ou commence l'affichage du plateau. */


// Espacement en pixels entre les pions
#define ESPACEMENT_X            55 /* Constante définissant l'espacement en x entre les pions. */
#define ESPACEMENT_Y            55 /* Constante définissant l'espacement en y entre les pions. */

/**
 * \struct score
 * \brief Structure correspondants aux objets pour l'affichage du score.
 * Cette structure permet de définir l’ensemble des textures (et leur position) correspondantes à l’affichage
 * du score en direct.
 */
typedef struct score
{
   SDL_Surface *surfaceText;
   SDL_Texture *textureText;
   SDL_Rect zone_textScore;
} score;


/**
 * \struct UI
 * \brief Structure correspondants aux objets pour l'interface des utilisateurs.
 * Cette structure permet de définir l’ensemble des textures (et leur position) correspondantes à l’affichage
 * de l’interface des utilisateurs, c’est à dire, les panneaux indiquant le tour.
 */
typedef struct UI
{
    SDL_Surface *surfaceTour;
    SDL_Texture *textureTour;
    SDL_Texture *tourJoueur;
    SDL_Texture *joueurGagnant;

    SDL_Rect tourJoueur_r;
    SDL_Rect zone_text;
    SDL_Rect joueurGagnant_r;

}UI;


/**
 * \enum joueurs
 * \brief Constantes représentants les différents joueurs.
 *
 * Cette énumération permet d’attribuer un entier pour chaque joueur (joueur 1 et joueur 2 ou IA).
 */
enum joueurs {J1, J2, IA};

/**
 * \enum mode_jeu
 * \brief Constantes représentants les différents modes de jeu (Solo ou multi).
 *
 * Cette énumeération permet d’attribuer un entier pour chaque mode de jeu (Joueur contre joueur ou joueur contre IA ou IA contre IA).
 */
enum mode_jeu{JCJ, JCIA, DEMO};

/**
 * \enum difficulte
 * \brief Constantes représentants les différents niveaux de difficultés.
 *
 * Cette énumération permet d’attribuer un entier pour chaque niveau de difficulté différent (facile, moyen, difficile).
 */
enum difficulte{AUCUNE,FACILE,MOYEN,DIFFICILE};

TTF_Font *chargerPolice(char *chemin, int taillePolice); /* Permet de charger les polices. */


void drawPlateau(TTF_Font *police, images_jeu *ImagesJeu, plateau **Plateau); /* Permet d'afficher le plateau (pions etc...). */
void drawPionDeplace(SDL_Event event, TTF_Font *police, images_jeu *ImagesJeu, plateau **Plateau, jeu *Jeu); /* Permet d'afficher le pion déplacé. */
void drawPause(Struct_btn_jeu *Btn_jeu); /* Permet d'afficher les boutons etc... lors de la pause. */
void drawUI(TTF_Font *police, bool partieTermine, UI *Ui, jeu *Jeu); /* Permet d'afficher l'interface utilisateur. */
void drawSauvegarde(Struct_btn_jeu *Btn_jeu); /* Permet d'afficher le menu sauvegarde. */
void drawscorePanel(TTF_Font *police, bool partieTermine, jeu *Jeu, score *Score); /* Permet d'afficher le score en live. */
void animePionIA(images_jeu *ImagesJeu, plateau **Plateau, jeu *Jeu, UI *Ui, TTF_Font *police, bool partieTermine, score *score, int yOrigine, int xOrigine, int yDestination, int xDestination);
void drawTaillePileDeplace(TTF_Font *police, images_jeu *ImagesJeu, plateau **Plateau, jeu *Jeu, SDL_Rect pionDeplace, int i, int j); /* Affiche la taille de la pile lors du déplacement */

void chargeInitImagesJeu(images_jeu *ImagesJeu, UI *Ui); /* Permet de charger l'ensemble des textures du plateau. */
void chargeInitButtonJeu(Struct_btn_jeu *Btn_jeu);       /* Permet de charger l'ensemble des boutons lors du jeu en pause. */
void chargeInitButtonSave(Struct_btn_jeu *Btn_jeu);

void avalam(int mode_jeu, int mode_difficulte); /* C'est la fonction principale qui appelle l'ensemble des fonctions des autres fonctions. */
void cleanupTexturesJeu(images_jeu *ImagesJeu, Struct_btn_jeu *Btn_jeu, UI *Ui); /* Permet de détruire les textures. */
void afficheGagnant(jeu *Jeu, UI *Ui, TTF_Font *police); /* Permet d'afficher le gagnant. */
void gestionEventsBoutonSauvegarde(bool *pause,bool *continuer, bool *sauvegarder, int x, int y, Struct_btn_jeu *Btn_jeu, plateau **Plateau,jeu *Jeu); /* Permet de gérer la transition selon les boutons dans la pause. */
int sauvegarde(char *chemin, plateau **Plateau, jeu *Jeu); /* Permet de sauvegarder une partie dans un fichier .txt */

void gestionEventsBoutonPause(bool *pause, bool *continuer,bool *sauvegarder,bool *charger, int x, int y, Struct_btn_jeu *Btn_jeu);
void gestionEventsBoutonSauvegarde(bool *pause,bool *continuer, bool *sauvegarder, int x, int y, Struct_btn_jeu *Btn_jeu, plateau **Plateau,jeu *Jeu);
void gestionEventsBoutonCharge(bool *pause,bool *continuer, bool *charger, int x, int y, Struct_btn_jeu *Btn_jeu, plateau **Plateau, plateau **Pcharge, jeu *Jeu);

int charge(char *chemin, plateau **Plateau, jeu *Jeu);



#endif // AVALAM_H_INCLUDED
