#include <list>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "headerFiles/Models/Demineur.h"

using namespace std;

Demineur::Demineur()
{
}

        Demineur::Demineur(int lignes, int colonnes, int mines) : nbLignes(lignes), nbColonnes(colonnes), nbMines(mines), drapeau(0) , gameState(INIT) 
{   
    initialiserGrille();
        }


Demineur::Demineur(int lignes, int colonnes, int mines, int drapeau ) : nbLignes(lignes), nbColonnes(colonnes), nbMines(mines), drapeau(drapeau) , gameState(INIT)
{
    initialiserGrille();
}

Demineur::Demineur(const char *filename)
{
    if (importeGame(filename) == -1)
    {
        gameState = ERROR;
    }
}

int Demineur::importeGame(const char *filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        return -1;
    }

    file >> nbLignes >> nbColonnes >> nbMines >> drapeau;

    for (int i = 0; i < nbLignes; ++i)
    {
        for (int j = 0; j < nbColonnes; ++j)
        {
            file >> grille[i][j];
        }
    }

    for (int i = 0; i < nbLignes; ++i)
    {
        for (int j = 0; j < nbColonnes; ++j)
        {
            file >> grilleCachee[i][j];
        }
    }

    file.close();
    return 0;
}

void Demineur::sauvegarderGame(const char *filename)
{

    ofstream file(filename);
    if (!file.is_open())
    {
        return;
    }

    file << nbLignes << " " << nbColonnes << " " << nbMines << " " << drapeau << endl;

    for (int i = 0; i < nbLignes; ++i)
    {
        for (int j = 0; j < nbColonnes; ++j)
        {
            file << grille[i][j] << " ";
        }
        file << endl;
    }

    for (int i = 0; i < nbLignes; ++i)
    {
        for (int j = 0; j < nbColonnes; ++j)
        {
            file << grilleCachee[i][j] << " ";
        }
        file << endl;
    }

    file.close();
}

void Demineur::initialiserGrille()
{
    for (int i = 0; i < nbLignes; ++i)
    {
        for (int j = 0; j < nbColonnes; ++j)
        {
            grille[i][j] = 0;
            grilleCachee[i][j] = 0;
        }
    }

    // Placer les mines
    srand(time(nullptr));
    for (int i = 0; i < nbMines; ++i)
    {
        int x = rand() % nbLignes;
        int y = rand() % nbColonnes;
        if (grille[x][y] == -1)
        {
            --i;
        }
        else
        {
            grille[x][y] = -1;
        }
    }

    // Calculer les nombres de mines adjacentes
    for (int i = 0; i < nbLignes; ++i)
    {
        for (int j = 0; j < nbColonnes; ++j)
        {
            if (grille[i][j] == -1)
            {
                continue;
            }
            for (int k = -1; k <= 1; ++k)
            {
                for (int l = -1; l <= 1; ++l)
                {
                    int newX = i + k;
                    int newY = j + l;
                    if (newX >= 0 && newX < nbLignes && newY >= 0 && newY < nbColonnes)
                    {
                        if (grille[newX][newY] == -1)
                        {
                            ++grille[i][j];
                        }
                    }
                }
            }
        }
    }


}


    void Demineur::initialiserGrilleXY(int x, int y){
         for (int i = 0; i < nbLignes; ++i)
        {
        for (int j = 0; j < nbColonnes; ++j)
        {
            grille[i][j] = 0;
            grilleCachee[i][j] = 0;
        }
        }

        // Placer les mines sauf sur les cases 3 * 3 de centre x,y

        srand(time(nullptr));
        for (int i = 0; i < nbMines; ++i)
        {
            int k = rand() % nbLignes;
            int l = rand() % nbColonnes;
            if (grille[k][l] == -1 || (k >= y - 1 && k <= y + 1 && l >= x - 1 && l <= x + 1))
            {
                --i;
            }
            else
            {
                grille[k][l] = -1;
            }
        }

        // Calculer les nombres de mines adjacentes
        for (int i = 0; i < nbLignes; ++i)
        {
            for (int j = 0; j < nbColonnes; ++j)
            {
                if (grille[i][j] == -1)
                {
                    continue;
                }
                for (int k = -1; k <= 1; ++k)
                {
                    for (int l = -1; l <= 1; ++l)
                    {
                        int newX = i + k;
                        int newY = j + l;
                        if (newX >= 0 && newX < nbLignes && newY >= 0 && newY < nbColonnes)
                        {
                            if (grille[newX][newY] == -1)
                            {
                                ++grille[i][j];
                            }
                        }
                    }
                }
            }
        }




    }



bool Demineur::play(int x, int y)
{

    if (gameState == INIT)
    {
        while (grille[y][x] != 0)
        {
            initialiserGrilleXY(x, y);
        }
        drapeau = 0;
        gameState = EN_COURS;
    }

    
    if (x < 0 || x >= nbColonnes || y < 0 || y >= nbLignes)
    {
        return true;
    }

    if (grilleCachee[y][x] != 0)
    {
                return true;
    }

    if (grille[y][x] == -1)
    {
        gameState = PERDU;
        grilleCachee[y][x] = -1;
        return false;
    }

    list<pair<int, int>> toExplore;
    toExplore.push_back(make_pair(x, y));
    while (!toExplore.empty())
    {
        pair<int, int> current = toExplore.front();
        toExplore.pop_front();
        int i = current.first;
        int j = current.second;
        if (grilleCachee[j][i] == 1)
        {
            continue;
        }
        if(grilleCachee[j][i] == 2)
        {
            drapeau--;
           
        }
        grilleCachee[j][i] = 1;
        if (grille[j][i] == 0)
        {
            for (int k = -1; k <= 1; ++k)
            {
                for (int l = -1; l <= 1; ++l)
                {
                    int newX = i + k;
                    int newY = j + l;
                    if (newX >= 0 && newX < nbColonnes && newY >= 0 && newY < nbLignes)
                    {
                        toExplore.push_back(make_pair(newX, newY));
                    }
                }
            }
        }
    }

    int nbCasesDecouvertes = 0;
    for (int i = 0; i < nbLignes; ++i)
    {
        for (int j = 0; j < nbColonnes; ++j)
        {
            if (grilleCachee[i][j] == 1)
            {
                ++nbCasesDecouvertes;
            }
        }
    }

    if (nbCasesDecouvertes == nbLignes * nbColonnes - nbMines)
    {
        gameState = GAGNE;
        return false;
    }

    return true;
}

void Demineur::marquerCase(int x, int y)
{
    if (grilleCachee[y][x] == 0 && drapeau < nbMines)
    {
        grilleCachee[y][x] = 2;
        drapeau++;
    }
    else if (grilleCachee[y][x] == 2 && drapeau > 0)
    {
        grilleCachee[y][x] = 0;
        drapeau--;
    }

}

void Demineur::afficherGrille()
{
    for (int i = 0; i < nbLignes; ++i)
    {
        
        for (int j = 0; j < nbColonnes; ++j)
        {
            if (grilleCachee[i][j] == 1)
            {
                cout << grille[i][j] << " ";
            }
            else if (grilleCachee[i][j] == 2)
            {
                cout << "? ";
            }
            else
            {
                cout << "X ";
            }
        }
        cout << endl;
    }

    cout << endl;
}


void Demineur::resetGame()
{
    gameState = INIT;
    drapeau=0;
    initialiserGrille();
}


Demineur::Etat Demineur::getState()
{
    return gameState;
}


int *Demineur::getGrille()
{
    return &grille[0][0];
}

int Demineur::getNbLignes()
{
    return nbLignes;
}

int Demineur::getNbColonnes()
{
    return nbColonnes;
}

int Demineur::getNbMines()
{
    return nbMines;
}


int Demineur::getNbFlag()
{
    return drapeau;    
}

int Demineur::getCellValue(int x, int y)
{
    return grille[y][x];
}

int Demineur::getCellState(int x, int y)
{
    return grilleCachee[y][x];
}

void Demineur::showAll()
{
    for (int i = 0; i < nbLignes; ++i)
    {
        for (int j = 0; j < nbColonnes; ++j)
        {
            grilleCachee[i][j] = 1;
        }
    }
}

int *Demineur::getGrilleCachee()
{
    return &grilleCachee[0][0];
}

Demineur::~Demineur() {}
