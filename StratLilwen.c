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
void initBorne(t_borne tabBorne); 
int chercherLigne(t_grille laGrille, int col);
bool estVainqueur(t_grille laGrille, int lig, int col);
int compteTaille(t_indice tab);
bool estPresent(t_indice tab, int lig, int col, int nbCaseInd);
void troisPions(t_grille grille, t_indice tabInterditBloque, t_indice tabInterditGagne, int pionStrat, int *tailleBloquer, int *tailleGagner, bool *dejaJoue, int *colonneJoue);
void deuxPions(t_grille grille, t_indice tabInterditBloque, t_indice tabInterditGagne, int pionStrat, int tailleBloquer, int tailleGagner, bool *dejaJoue, int *colonneJoue);
void verifDeuxPions(t_grille grille, t_borne tabBorne, int lig, int col);
void colonneValide(t_grille grille, t_indice tabInterditBloque, t_indice tabInterditGagne, int tailleBloquer, int tailleGagner, int pionStrat, int *colonneJoue, bool *dejaJoue); 
void jeuColonneCentrale(t_grille grille, t_indice tabInterditBloque, t_indice tabInterditGagne, int pionStrat, int tailleBloquer, int tailleGagner, int *colonneJoue, bool *dejaJoue); 
int joueStratBloquer(t_grille grille);



int main(int argc, char** argv) 
{
    t_grille grille;

    chargerGrille(argv, grille);

    return joueStratBloquer(grille);
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

void deuxPions(t_grille laGrille, t_indice tabInterditBloque, t_indice tabInterditGagne, int pionStrat, int tailleBloquer, int tailleGagner, bool *dejaJoue, int *colonneJoue)
{
    t_borne tabBorne;

    initBorne(tabBorne); 

    // Les boucles for sont dedoublees pour prioriser le fait de bloquer

    // Permet a la strategie de bloquer l'alignement de trois pions adverse
    for (int i = 0 ; i < LIGNE && *dejaJoue == false ; i++)
    {
        for (int j = 0 ; j < COLONNE && *dejaJoue == false ; j++)
        {
            if (laGrille[i][j] == ADVERSAIRE)
            {
                verifDeuxPions(laGrille, tabBorne, i, j);

                for (int i = 0 ; i < TAB_BORNE && *dejaJoue == false ; i++)
                {
                    if (tabBorne[i][0] != -1 && laGrille[tabBorne[i][0]][tabBorne[i][1]] == VIDE && ((tabBorne[i][0] < LIGNE-1  && laGrille[tabBorne[i][0]+1][tabBorne[i][1]] != VIDE) || tabBorne[i][0] == LIGNE-1))
                    {
                        if (estPresent(tabInterditBloque, tabBorne[i][0], tabBorne[i][1], tailleBloquer) == false && estPresent(tabInterditGagne, tabBorne[i][0], tabBorne[i][1], tailleGagner))
                        {
                            *colonneJoue = tabBorne[i][1]; 
                            *dejaJoue = true;
                        }
                    }
                }
            }
        }
    }

    // Permet a la strategie de d'alignement de trois de ces pions
    for (int i = 0 ; i < LIGNE && *dejaJoue == false ; i++)
    {
        for (int j = 0 ; j < COLONNE && *dejaJoue == false ; j++)
        {
            if (laGrille[i][j] == pionStrat)
            {
                verifDeuxPions(laGrille, tabBorne, i, j);

                for (int i = 0 ; i < 7 && *dejaJoue == false ; i++)
                {
                    if (tabBorne[i][0] != -1 && laGrille[tabBorne[i][0]][tabBorne[i][1]] == VIDE && ((tabBorne[i][0] < LIGNE-1  && laGrille[tabBorne[i][0]+1][tabBorne[i][1]] != VIDE) || tabBorne[i][0] == LIGNE-1))
                    {
                        if (estPresent(tabInterditBloque, tabBorne[i][0], tabBorne[i][1], tailleBloquer) == false && estPresent(tabInterditGagne, tabBorne[i][0], tabBorne[i][1], tailleGagner) == false)
                        {
                            *colonneJoue = tabBorne[i][1]; 
                            *dejaJoue = true;
                        }
                    }
                }
            }
        }
    }
}

void colonneValide(t_grille laGrille, t_indice tabInterditBloque, t_indice tabInterditGagne, int tailleBloquer, int tailleGagner, int pionStrat, int *colonneJoue, bool *dejaJoue)
{
    int meilleurColonne;
    bool trouver; 
    int lig; 

    trouver = false; 

    for (int i = 0 ; i < COLONNE && trouver == false ; i++)
    {
        lig = chercherLigne(laGrille, i); 

        if (lig != -1 && estPresent(tabInterditBloque, lig, i, tailleBloquer) == false && estPresent(tabInterditGagne, lig, i, tailleGagner) == false)
        {
            meilleurColonne = i; 
            trouver = true; 
        }
    }
    for (int i = 0 ; i < COLONNE && trouver == false ; i++)
    {
        if (lig != -1 && estPresent(tabInterditBloque, lig, i, tailleBloquer) == false && estPresent(tabInterditGagne, lig, i, tailleGagner) == true)
        {
            meilleurColonne = i;
            trouver = true; 
        }
    }
    for (int i = 0 ; i < COLONNE && trouver == false ; i++)
    {
        if (lig != -1)
        {
            meilleurColonne = i;
            trouver = true; 
        }
    }

    *colonneJoue = meilleurColonne; 
    *dejaJoue = true; 
}

void verifDeuxPions(t_grille laGrille, t_borne tabBorne, int lig, int col)
{
    char lePion = laGrille[lig][col];
    int cpt,i,j;


    // Ligne 
    if (col > 1 || col < COLONNE-1)
    {
        j = col;
        cpt = 0;

        while (j >= 0 && laGrille[lig][j] == lePion)
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

        if (cpt == 2)
        {
            if (laGrille[lig][col+1] == lePion)
            {
                if (col > 0)
                {
                    tabBorne[0][0] = lig; 
                    tabBorne[0][1] = col-1;
                }
                if (col < COLONNE-1)
                {
                    tabBorne[1][0] = lig; 
                    tabBorne[1][1] = col+2; 
                }
            }
            else 
            {
                if (col > 1)
                {
                    tabBorne[0][0] = lig; 
                    tabBorne[0][1] = col-2;
                }
                if (col < COLONNE)
                {
                    tabBorne[1][0] = lig; 
                    tabBorne[1][1] = col+1; 
                }
            }
        }
    }


    // Diagonale Haut-Gauche à Droite-Bas
    if ((col > 1 && lig > 1) || (col < COLONNE-1 && lig < LIGNE-1))
    {
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

        if (cpt == 2)
        {
            if (laGrille[lig+1][col+1] == lePion)
            {
                if (col > 0 && lig > 0)
                {
                    tabBorne[2][0] = lig-1; 
                    tabBorne[2][1] = col-1;
                }
                if (col < COLONNE-1 && lig < LIGNE-1)
                {
                    tabBorne[3][0] = lig+2; 
                    tabBorne[3][1] = col+2; 
                }
            }
            else 
            {
                if (col > 1 && lig > 1)
                {
                    tabBorne[2][0] = lig-2; 
                    tabBorne[2][1] = col-2;
                }
                if (col < COLONNE && lig < LIGNE)
                {
                    tabBorne[3][0] = lig+1; 
                    tabBorne[3][1] = col+1; 
                }
            }
        }
    }


    // Diagonale Haut-Droite à Bas-Gauche
    if ((col > 1 && lig > 1) || (col < COLONNE-1 && lig < LIGNE-1))
    {
        i = lig;
        j = col;
        cpt = 0;

        while (j < COLONNE && i >= 0 && laGrille[i][j] == lePion){
            cpt++;
            i--;
            j++;
        }
        i = lig+1;
        j = col-1;
        
        while (i<LIGNE && j >= 0 && laGrille[i][j] == lePion){
            cpt++;
            i++;
            j--;
        }

        if (cpt == 2)
        {
            if (laGrille[lig+1][col-1] == lePion)
            {
                if (col > 0 && lig < LIGNE)
                {
                    tabBorne[4][0] = lig+1; 
                    tabBorne[4][1] = col-1;
                }
                if (col < COLONNE-1 && lig > 1)
                {
                    tabBorne[5][0] = lig-2; 
                    tabBorne[5][1] = col+2; 
                }
            }
            else
            {
                if (col > 1 && lig < LIGNE-1)
                {
                    tabBorne[4][0] = lig+2; 
                    tabBorne[4][1] = col-2;
                }
                if (col < COLONNE && lig > 0)
                {
                    tabBorne[5][0] = lig-1; 
                    tabBorne[5][1] = col+1; 
                }
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



int joueStratBloquer(t_grille laGrille)
{
    int colonneJoue; 
    t_indice tabInterditBloque; 
    t_indice tabInterditGagne;
    int tailleBloquer; 
    int tailleGagner; 
    bool dejaJoue; 

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
        deuxPions(laGrille, tabInterditBloque, tabInterditGagne, JOUEUR, tailleBloquer, tailleGagner, &dejaJoue, &colonneJoue); 

        if (dejaJoue == false)
        {
            jeuColonneCentrale(laGrille, tabInterditBloque, tabInterditGagne, JOUEUR, tailleBloquer, tailleGagner, &colonneJoue, &dejaJoue); 
            
            if (dejaJoue == false) 
            {
                colonneValide(laGrille, tabInterditBloque, tabInterditGagne, tailleBloquer, tailleGagner, JOUEUR, &colonneJoue, &dejaJoue);  
            }
        }
    }

    return colonneJoue; 
}