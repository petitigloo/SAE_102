#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define LIGNE 6
#define COLONNE 7
#define TAB_LIGNE 42
#define TAB_BORNE 7
#define TAB_INDICE 2

typedef int t_grille[LIGNE][COLONNE];
typedef int t_indice[TAB_LIGNE][TAB_INDICE]; 
typedef int t_borne[TAB_BORNE][TAB_INDICE]; 

const int VIDE = 0;
const int JOUEUR = 1;
const int ADVERSAIRE = 2;
const int VIDETAB = -1; 

int main(int argc, char** argv);
void chargerGrille(char** argv, t_grille grille);


    /*  Stratégie */

void inittabIndice(t_indice tabIndice);
int chercherLigne(t_grille laGrille, int col);
bool estVainqueur(t_grille laGrille, int lig, int col);
int compteTaille(t_indice tab);
bool estPresent(t_indice tab, int lig, int col, int nbCaseInd);
void troisPions(t_grille grille, t_indice tabInterditBloque, t_indice tabInterditGagne, int pionStrat, int *tailleBloquer, int *tailleGagner, bool *dejaJoue, int *colonneJoue);
void compteCoupsJoues(t_grille laGrille, int pionStrat, int *nbrcoups); 
void jeuColonneCentrale(t_grille grille, t_indice tabInterditBloque, t_indice tabInterditGagne, int pionStrat, int tailleBloquer, int tailleGagner, int *colonneJoue, bool *dejaJoue); 
void jeuPair(t_grille laGrille, t_indice tabInterditBloque, t_indice tabInterditGagne, int pionStrat, int tailleBloquer, int tailleGagner, int *colonneJoue, bool *dejaJoue); 
int joueStratGagner(t_grille laGrille); 



int main(int argc, char** argv) 
{
    t_grille grille;

    chargerGrille(argv, grille);

    return joueStratGagner(grille);
}

void chargerGrille(char** argv, t_grille grille) 
{
    for(int i = 0; i < LIGNE; i++)
        for(int j = 0; j < COLONNE; j++)
            grille[i][j] = atoi(argv[i * COLONNE + j + 1]);
}


    /*  Stratégie */

void inittabIndice(t_indice tabIndice)
{
    for (int i = 0 ; i < TAB_LIGNE ; i++)
    {
        for (int j = 0 ; j < TAB_INDICE ; j++)
        {
            tabIndice[i][j] = VIDETAB; 
        }
    }
}

int chercherLigne(t_grille laGrille, int col)
{
    int ligne = -1;

    while (ligne < LIGNE-1 && laGrille[ligne+1][col] == VIDE)
    {
        ligne++;
    }
    
    return ligne;
}

int compteTaille(t_indice tab)
{
    int taille; 
    bool sortie;
    taille = 0; 
    sortie = false;

    for (int i = 0 ; i < TAB_LIGNE && sortie == false ; i++)
    {
        if (tab[i][0] != VIDETAB)
        {
            taille++; 
        }
        else 
        {
            sortie = true; 
        }
    }

    return taille; 
}

bool estVainqueur(t_grille laGrille, int lig, int col)
{
    char lePion = laGrille[lig][col];
    int cpt,i,j;

    i = lig;
    cpt = 0;
    while (i < LIGNE && laGrille[i][col] == lePion)
    {
        cpt++;
        i++;
    }
    if (cpt >= 4)
    {
        return true;
    }

    j = col;
    cpt = 0;
    while (j>=0 && laGrille[lig][j]==lePion)
    {
        cpt++;
        j--;
    }
    j = col+1;
    while (j < COLONNE && laGrille[lig][j] == lePion)
    {
        cpt++;
        j++;
    }
    if (cpt>=4 )
    {
        return true;
    }

    i = lig;
    j = col;
    cpt = 0;
    while (j >= 0 && i >= 0 && laGrille[i][j] == lePion)
    {
        cpt++;
        i--;
        j--;
    }
    i = lig+1;
    j = col+1;
    while (i < LIGNE && j < COLONNE && laGrille[i][j] == lePion)
    {
        cpt++;
        i++;
        j++;
    }
    if (cpt >=4)
    {
        return true;
    }

    i = lig;
    j = col;
    cpt = 0;
    while (j < COLONNE && i >= 0 && laGrille[i][j] == lePion)
    {
        cpt++;
        i--;
        j++;
    }
    i = lig+1;
    j = col-1;
    while (i < LIGNE && j >= 0 && laGrille[i][j] == lePion)
    {
        cpt++;
        i++;
        j--;
    }
    if (cpt >= 4)
    {
        return true;
    }
    return false;
}

bool estPresent(t_indice tab, int lig, int col, int nbCaseInd)
{
    bool estPresent; 
    estPresent = false; 

    for (int i = 0 ; (i < nbCaseInd) && (estPresent != true) ; i++)
    {
        if ((tab[i][0] == lig) && (tab[i][1] == col))
        {
            estPresent = true; 
        }
    }

    return estPresent; 
}

void troisPions(t_grille laGrille, t_indice tabInterditBloque, t_indice tabInterditGagne, int pionStrat, int *tailleBloquer, int *tailleGagner, bool *dejaJoue, int *colonneJoue)
{
    int lePion; 
    bool possibleGagne; 
    // Les boucles for sont dedoublees pour prioriser le fait de gagner

    if (pionStrat == JOUEUR)
    {
        lePion = ADVERSAIRE; 
    }
    else
    {
        lePion = JOUEUR; 
    }


    for (int i = 0 ; i < LIGNE && *dejaJoue == false ; i++)
    {
        for (int j = 0 ; j < COLONNE && *dejaJoue == false ; j++)
        {
            // Permet a la strategie de gagner
            if (laGrille[i][j] == VIDE)
            {
                laGrille[i][j] = pionStrat; 
                possibleGagne = estVainqueur(laGrille, i, j); 
                laGrille[i][j] = VIDE; 

                if (possibleGagne == true)
                {
                    if ((i < LIGNE-1  && laGrille[i+1][j] != VIDE) || i == LIGNE-1)
                    {
                        *colonneJoue = j; 
                        *dejaJoue = true;
                    }
                    else if (i > LIGNE-1 && laGrille[i+1][j] == VIDE && estPresent(tabInterditGagne, i+1, j, *tailleGagner) == false)
                    {
                        tabInterditGagne[*tailleGagner][0] = i+1;
                        tabInterditGagne[*tailleGagner][1] = j;
                        (*tailleGagner)++; 
                    }
                }
            }
        }
    }
    for (int i = 0 ; i < LIGNE && *dejaJoue == false ; i++)
    {
        for (int j = 0 ; j < COLONNE && *dejaJoue == false ; j++)
        {
            // Permet a la strategie de bloquer
            if (laGrille[i][j] == VIDE)
            {
                laGrille[i][j] = lePion; 
                possibleGagne = estVainqueur(laGrille, i, j); 
                laGrille[i][j] = VIDE; 

                if (possibleGagne == true)
                {
                    if ((i < LIGNE-1 && laGrille[i+1][j] != VIDE) || i == LIGNE-1)
                    {
                        *colonneJoue = j; 
                        *dejaJoue = true;
                    }
                    else if (i > LIGNE-1 && laGrille[i+1][j] == VIDE && estPresent(tabInterditBloque, i+1, j, *tailleBloquer) == false)                    {
                        tabInterditBloque[*tailleBloquer][0] = i+1;
                        tabInterditBloque[*tailleBloquer][1] = j;
                        (*tailleBloquer)++; 
                    }
                }
            }
        }
    }    
}

void compteCoupsJoues(t_grille laGrille, int pionStrat, int *nbrcoups)
{
    *nbrcoups = 1;
    for (int i = 0 ; i < LIGNE ; i++)
    {
        for (int j = 0 ; j < COLONNE ; j++)
        {
            if (laGrille[i][j] == pionStrat)
            {
                *nbrcoups = *nbrcoups + 1;
            }
        }
    }
}

void jeuColonneCentrale(t_grille laGrille, t_indice tabInterditBloque, t_indice tabInterditGagne, int pionStrat, int tailleBloquer, int tailleGagner, int *colonneJoue, bool *dejaJoue)
{
    int ligne; 
    ligne = chercherLigne(laGrille, 3);
    if (ligne != -1 && estPresent(tabInterditBloque, ligne, 3, tailleBloquer) == false && estPresent(tabInterditGagne, ligne, 3, tailleGagner) == false)
    {
        *colonneJoue = 3;
        *dejaJoue = true; 
    }
}

void jeuPair(t_grille laGrille, t_indice tabInterditBloque, t_indice tabInterditGagne, int pionStrat, int tailleBloquer, int tailleGagner, int *colonneJoue, bool *dejaJoue)
{
    int i; 
    int lig;
    bool esttrouvedroite, esttrouvegauche;
    int lignedroite, lignegauche;
    int colonnedroite, colonnegauche;

    esttrouvedroite = false;
    esttrouvegauche = false;

    i = 4; // n'etudie pas la colonne centrale
    while ((i < COLONNE) && (esttrouvedroite == false))
    {
        lignedroite = chercherLigne(laGrille, i); 
        if (lignedroite != -1)
        {
            if ((estPresent(tabInterditBloque, lignedroite, i, tailleBloquer) == false) &&
                (estPresent(tabInterditGagne,  lignedroite, i, tailleGagner) == false))
            {
                colonnedroite = i;
                esttrouvedroite = true;
            }
            else
            {
                lignedroite = -1; 
            }
        }
        i++; 
    }

    i = 2; // n'etudie pas la colonne centrale
    while ((i >= 0) && (esttrouvegauche == false))
    {
        lignegauche = chercherLigne(laGrille, i); 
        if (lignegauche != -1)
        {
            if ((estPresent(tabInterditBloque, lignegauche, i, tailleBloquer) == false) &&
                (estPresent(tabInterditGagne,  lignegauche, i, tailleGagner) == false))
            {
                colonnegauche = i;
                esttrouvegauche = true;
            }
            else
            {
                lignedroite = -1; 
            }
        }
        i--; 
    }
    
    // si une des deux colonnes voisines de celle centrales est valide
    if (esttrouvedroite == true || esttrouvegauche == true)
    {
        if ((lignedroite > lignegauche))
        {
            // c'est la droite qu'il faut choisir
            *colonneJoue = colonnedroite;
            *dejaJoue = true;
        }
        else
        {
            // c'est la gauche qu'il faut choisir
            *colonneJoue = colonnegauche; 
            *dejaJoue = true;
        }
    }

    else if (*dejaJoue == false)
    {
        // deux boucle for distincte pour prioriser le faite de ne pas gagner plutot que de laisser l'utilisateur gagner

        for (int i = 0; (i < COLONNE)&&(*dejaJoue == false); i++)
        {
            lig = chercherLigne(laGrille, i);
            if ((lig != -1)&&((estPresent(tabInterditGagne, lig, i, tailleGagner)) == true)&&((estPresent(tabInterditBloque, lig, i, tailleBloquer)) == false))
            {
                *colonneJoue = i;
                *dejaJoue = true;
            }
        }

        for (int i = 0; (i < COLONNE)&&(*dejaJoue == false); i++)
        {
            lig = chercherLigne(laGrille, i);
            if (lig != -1)
            {
                *colonneJoue = i;
                *dejaJoue = true;
            }
        }
    }
}



int joueStratGagner(t_grille laGrille)
{
    int colonneJoue; 
    t_indice tabInterditBloque; 
    t_indice tabInterditGagne;
    int  tailleBloquer; 
    int  tailleGagner; 
    bool dejaJoue; 
    int  nbrcoups;

    dejaJoue = false; 

    inittabIndice(tabInterditBloque); 
    inittabIndice(tabInterditGagne);  

    tailleBloquer = 0; 
    tailleGagner  = 0; 

    troisPions(laGrille, tabInterditBloque, tabInterditGagne, JOUEUR, &tailleBloquer, &tailleGagner, &dejaJoue, &colonneJoue);

    tailleBloquer = compteTaille(tabInterditBloque); 
    tailleGagner  = compteTaille(tabInterditGagne);

    if (dejaJoue == false)
    {
        compteCoupsJoues(laGrille, JOUEUR, &nbrcoups);

        // Jeu en impair : dans la colonne du milieu
        if (nbrcoups%2 != 0)
        {
            jeuColonneCentrale(laGrille, tabInterditBloque, tabInterditGagne, JOUEUR, tailleBloquer, tailleGagner, &colonneJoue, &dejaJoue); 
        }
        
        // Jeu en pair : dans la colonne voisine la plus vide de celle centrale ou si la colonne du milieu est pleine
        if (dejaJoue == false)
        {
            jeuPair(laGrille, tabInterditBloque, tabInterditGagne, JOUEUR, tailleBloquer, tailleGagner, &colonneJoue, &dejaJoue);
        }
    }    

    return colonneJoue;     
}
