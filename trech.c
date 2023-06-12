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
void chargergrille(char** argv, t_grille t_grille);


    /*  Stratégie */

void inittabIndice(t_indice tabIndice);
bool estPresent(t_indice tab, int lig, int col, int nbCaseInd);
void troisPions(t_grille laGrille, t_indice tabInterditBloque, t_indice tabInterditGagne, int pionStrat, int *tailleBloquer, int *tailleGagner, bool *dejaJoue, int *colonneJoue);
bool alignementTrois(t_grille laGrille, int pionStrat, int lig, int col);
void deuxPions(t_grille laGrille, t_indice tabInterditBloque, t_indice tabInterditGagne, int pionStrat, int tailleBloquer, int tailleGagner, bool *dejaJoue, int *colonneJoue);
bool alignementDeux(t_grille laGrille, int pionStrat, int lig, int col);
void deuxPions(t_grille laGrille, t_indice tabInterditBloque, t_indice tabInterditGagne, int pionStrat, int tailleBloquer, int tailleGagner, bool *dejaJoue, int *colonneJoue);
void verifDeuxPions(t_grille laGrille, t_borne tabBorne, int lig, int col);
int compteCoupsJoues(t_grille laGrille, int pionStrat); 
void jeuPair(t_grille laGrille, t_indice tabInterditBloque, t_indice tabInterditGagne, int tailleBloquer, int tailleGagner, int pionStrat, int *colonneJoue); 
int joueTrech(t_grille laGrille);



int main(int argc, char** argv) 
{
    t_grille grille;

    chargergrille(argv, grille);

    return joueTrech(grille);
}

void chargergrille(char** argv, t_grille grille) 
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

void initBorne(t_borne tabBorne)
{
    for (int i = 0 ; i < TAB_BORNE ; i++)
    {
        for (int j = 0 ; j < TAB_INDICE ; j++)
        {
            tabBorne[i][j] = VIDETAB; 
        }
    }
}

int chercherLigne(t_grille laGrille, int col)
{
    int ligne = -1;

    while (ligne < LIGNE-1 && laGrille[ligne+1][col]==VIDE)
    {
        ligne++;
    }
    
    return ligne;
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
    bool possibleGagne; 

    // Les boucles for sont dedoublees pour prioriser le fait de gagner
    for (int i = 0 ; i < LIGNE && *dejaJoue == false ; i++)
    {
        for (int j = 0 ; j < COLONNE && *dejaJoue == false ; j++)
        {
            // Permet a la strategie de gagner
            if (laGrille[i][j] == VIDE)
            {
                laGrille[i][j] = pionStrat; 
                possibleGagne = alignementTrois(laGrille, pionStrat, i, j); 
                laGrille[i][j] = VIDE; 

                if (possibleGagne == true)
                {
                    if ((i < LIGNE-1  && laGrille[i+1][j] != VIDE) || i == LIGNE-1)
                    {
                        *colonneJoue = j; 
                        *dejaJoue = true;
                    }
                    else if (i < LIGNE-1 && laGrille[i+1][j] == VIDE && estPresent(tabInterditGagne, i+1, j, *tailleGagner) == false)
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
                laGrille[i][j] = ADVERSAIRE; 
                possibleGagne = alignementTrois(laGrille, ADVERSAIRE, i, j); 
                laGrille[i][j] = VIDE; 

                if (possibleGagne == true)
                {
                    if ((i < LIGNE-1 && laGrille[i+1][j] != VIDE) || i == LIGNE-1)
                    {
                        *colonneJoue = j; 
                        *dejaJoue = true;
                    }
                    else                    
                    {
                        if (i < LIGNE-1 && laGrille[i+1][j] == VIDE && estPresent(tabInterditBloque, i+1, j, *tailleBloquer) == false) 
                        {
                            tabInterditBloque[*tailleBloquer][0] = i+1;
                            tabInterditBloque[*tailleBloquer][1] = j;
                            (*tailleBloquer)++; 
                        }
                    }
                }
            }
        }
    }    
}

bool alignementTrois(t_grille laGrille, int pionStrat, int lig, int col)
{
    int cpt,i,j;

    // Colonne 
    i = lig;
    cpt = 0;
    while (i < LIGNE && laGrille[i][col] == pionStrat && cpt < 4)
    {
        cpt++;
        i++;
    }
    if (cpt>=4){
        return true;
    }

    // Ligne 
    j = col;
    cpt = 0;
        // Droite
    while (j>=0 && laGrille[lig][j]==pionStrat  && cpt < 4){
        cpt++;
        j--;
    }
    j = col+1;
        // Gauche
    while (j<COLONNE && laGrille[lig][j]==pionStrat && cpt < 4){
        cpt++;
        j++;
    }
    if (cpt>=4 ){
        return true;
    }

    // Nord Ouest et au Sud Est
    i = lig;
    j = col;
    cpt = 0;
    // Nord Ouest
    while (j>=0 && i>=0 && laGrille[i][j]==pionStrat && cpt < 4){
        cpt++;
        i--;
        j--;
    }
    i = lig+1;
    j = col+1;
    //  Sud Est
    while (i<LIGNE && j<COLONNE && laGrille[i][j]==pionStrat && cpt < 4){
        cpt++;
        i++;
        j++;
    }
    if (cpt>=4 ){
        return true;
    }

    // Nord Est et au Sud Ouest
    i = lig;
    j = col;
    cpt = 0;
    // Nord Est
    while (j<COLONNE && i>=0 && laGrille[i][j]==pionStrat && cpt < 4){
        cpt++;
        i--;
        j++;
    }
    i = lig+1;
    j = col-1;
    // Sud Ouest
    while (i<LIGNE && j>=0 && laGrille[i][j]==pionStrat && cpt < 4){
        cpt++;
        i++;
        j--;
    }
    if (cpt>=4 ){
        return true;
    }
    return false;
}

void deuxPions(t_grille laGrille, t_indice tabInterditBloque, t_indice tabInterditGagne, int pionStrat, int tailleBloquer, int tailleGagner, bool *dejaJoue, int *colonneJoue)
{
    bool possibleGagne; 
    
    // Les boucles for sont dedoublees pour prioriser le fait de gagner
    for (int i = 0 ; i < LIGNE && *dejaJoue == false ; i++)
    {
        for (int j = 0 ; j < COLONNE && *dejaJoue == false ; j++)
        {
            // Permet a la strategie de bloquer l'alignement de trois pions adverse
            if (laGrille[i][j] == VIDE)
            {
                laGrille[i][j] = ADVERSAIRE; 
                possibleGagne = alignementDeux(laGrille, ADVERSAIRE, i, j); 
                laGrille[i][j] = VIDE; 

                if (possibleGagne == true)
                {
                    if (((i < LIGNE-1  && laGrille[i+1][j] != VIDE) || i == LIGNE-1) && estPresent(tabInterditBloque, i, j, tailleBloquer) == false)
                    {
                        *colonneJoue = j; 
                        *dejaJoue = true;
                    }
                }
            }
        }
    }
    for (int i = 0 ; i < LIGNE && *dejaJoue == false ; i++)
    {
        for (int j = 0 ; j < COLONNE && *dejaJoue == false ; j++)
        {
            // Permet a la strategie d'aligner trois pions
            if (laGrille[i][j] == VIDE)
            {
                laGrille[i][j] = pionStrat; 
                possibleGagne = alignementDeux(laGrille, pionStrat, i, j); 
                laGrille[i][j] = VIDE; 

                if (possibleGagne == true)
                {
                    if (((i < LIGNE-1 && laGrille[i+1][j] != VIDE) || i == LIGNE-1) && estPresent(tabInterditGagne, i, j, tailleGagner) == false)
                    {
                        *colonneJoue = j; 
                        *dejaJoue = true;
                    }
                }
            }
        }
    } 
}

bool alignementDeux(t_grille laGrille, int pionStrat, int lig, int col)
{
    int cpt,i,j;

    // Colonne
    i = lig;
    cpt = 0;
    while (i<LIGNE && laGrille[i][col]==pionStrat && cpt < 3)
    {
        cpt++;
        i++;
    }
    if (cpt == 3){
        return true;
    }

    // Ligne
    j = col;
    cpt = 0;
    // Droite
    while (j>=0 && laGrille[lig][j]==pionStrat && cpt < 3){
        cpt++;
        j--;
    }
    j = col+1;
    // Ouest
    while (j<COLONNE && laGrille[lig][j]==pionStrat && cpt < 3){
        cpt++;
        j++;
    }
    if (cpt == 3){
        return true;
    }
    
    // Nord Ouest et au Sud Est
    i = lig;
    j = col;
    cpt = 0;
    // Nord Ouest
    while (j>=0 && i>=0 && laGrille[i][j]==pionStrat && cpt < 3){
        cpt++;
        i--;
        j--;
    }
    i = lig+1;
    j = col+1;
    // Sud Est
    while (i<LIGNE && j<COLONNE && laGrille[i][j]==pionStrat && cpt < 3){
        cpt++;
        i++;
        j++;
    }
    if (cpt == 3){
        return true;
    }

    // Sud Ouest
    i = lig;
    j = col;
    cpt = 0;
    // Nord Est
    while (j<COLONNE && i>=0 && laGrille[i][j]==pionStrat && cpt < 3){
        cpt++;
        i--;
        j++;
    }
    i = lig+1;
    j = col-1;
    // Sud Ouest
    while (i<LIGNE && j>=0 && laGrille[i][j]==pionStrat && cpt < 3){
        cpt++;
        i++;
        j--;
    }
    if (cpt == 3){
        return true;
    }
    return false;
}

int compteCoupsJoues(t_grille laGrille, int pionStrat)
{
    int nbrcoups = 0; 

    for (int i = 0 ; i < LIGNE ; i++)
    {
        for (int j = 0 ; j < COLONNE ; j++)
        {
            if (laGrille[i][j] == pionStrat)
            {
                nbrcoups++;
            }
        }
    }

    return nbrcoups; 
}

void jeuPair(t_grille laGrille, t_indice tabInterditBloque, t_indice tabInterditGagne, int tailleBloquer, int tailleGagner, int pionStrat, int *colonneJoue)
{
    bool dejaJoue;
    bool esttrouvedroite, esttrouvegauche;
    int lignedroite, lignegauche;
    int colonnedroite, colonnegauche;
    int lig;

    dejaJoue = false; 

    esttrouvedroite = false;
    esttrouvegauche = false;

    // n'etudie pas la colonne centrale -- Vers la droite
    for (int i = 4 ; i < COLONNE && esttrouvedroite == false ; i++)
    {
        lignedroite = chercherLigne(laGrille, i); 

        if (lignedroite != -1 && estPresent(tabInterditBloque, lignedroite, i, tailleBloquer) == false && estPresent(tabInterditGagne, lignedroite, i, tailleGagner) == false)
        {
            colonnedroite = i; 
            esttrouvedroite = true; 
        }
        else 
        {
            lignedroite = -1; 
        }
    }

    // n'etudie pas la colonne centrale -- Vers la gauche
    for (int i = 2 ; i >= 0 && esttrouvegauche == false ; i--)
    {
        lignegauche = chercherLigne(laGrille, i); 

        if (lignegauche != -1 && estPresent(tabInterditBloque, lignegauche, i, tailleBloquer) == false && estPresent(tabInterditGagne, lignegauche, i, tailleGagner) == false)
        {
            colonnegauche = i; 
            esttrouvedroite = true; 
        }
        else 
        {
            lignegauche = -1; 
        }
    }
    

    // si deux colonne est valide
    // si esttrouverX = false => ligneX = -1
    //    donc esttrouverX est forcement < esttrouverY
    if (esttrouvedroite == true || esttrouvegauche == true)
    {
        if (lignedroite > lignegauche)
        {
            *colonneJoue = colonnedroite;
            dejaJoue = true; 
        }
        else
        {
            *colonneJoue = colonnegauche;
            dejaJoue = true; 
        }
        
    }


    // Cas où aucune des colonnes à côté de celle centrale n'est valide
    if (dejaJoue == false)
    {
        lig = chercherLigne(laGrille, 3);
        if (lig != -1 && estPresent(tabInterditBloque, lig, 3, tailleBloquer) == false && estPresent(tabInterditGagne, lig, 3, tailleGagner) == false)
        {
            *colonneJoue = 3;
            dejaJoue = true;
        }
    }


    // Cas où aucune des colonnes n'est valide
    if (dejaJoue == false)
    {
        // deux boucle for distincte pour prioriser le faite de ne pas gagner plutot que de laisser l'utilisateur gagner
        for (int i = 0 ; i < COLONNE && dejaJoue == false ; i++)
        {
            lig = chercherLigne(laGrille, i);
            if (lig != -1 && estPresent(tabInterditBloque, lig, i, tailleBloquer) == false)
            { 
                *colonneJoue = i;
                dejaJoue = true;
            }
        }

        for (int i = 0 ; i < COLONNE && dejaJoue == false ; i++)
        {
            lig = chercherLigne(laGrille, i);
            if (lig != -1)
            {
                *colonneJoue = i;
                dejaJoue = true;
            }
        }
    }
}


int joueTrech(t_grille laGrille)
{
    t_indice tabInterditBloque; 
    t_indice tabInterditGagne; 
    int colonneJoue; 
    int tailleBloquer; 
    int tailleGagner; 
    bool dejaJoue; 
    int  nbrcoups;
    int pionStrat; 
    int ligne; 

    pionStrat = JOUEUR; 

    inittabIndice(tabInterditBloque); 
    inittabIndice(tabInterditGagne);  

    dejaJoue = false; 

    tailleBloquer = 0; 
    tailleGagner = 0;

    troisPions(laGrille, tabInterditBloque, tabInterditGagne, pionStrat, &tailleBloquer, &tailleGagner, &dejaJoue, &colonneJoue);

    if (dejaJoue == false)
    {
        deuxPions(laGrille, tabInterditBloque, tabInterditGagne, pionStrat, tailleBloquer, tailleGagner, &dejaJoue, &colonneJoue); 

        if (dejaJoue == false)
        {
            nbrcoups = compteCoupsJoues(laGrille, pionStrat);

            // Jeu en impair : dans la colonne du milieu
            ligne = chercherLigne(laGrille, 3);
            if (nbrcoups%2 == 0 && ligne != -1 && estPresent(tabInterditBloque, ligne, 3, tailleBloquer) == false && estPresent(tabInterditGagne, ligne, 3, tailleGagner) == false)
            {
                colonneJoue = 3;
                dejaJoue = true; 
            }

            // Jeu en pair : dans la colonne voisine la plus vide de celle centrale ou si la colonne du milieu est pleine
            if (dejaJoue == false)
            {
                jeuPair(laGrille, tabInterditBloque, tabInterditGagne, tailleBloquer, tailleGagner, pionStrat, &colonneJoue);   
            }
        }
    }

    return colonneJoue; 
}
