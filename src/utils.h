#ifndef UTILS_H
#define UTILS_H

#include "site.h"

#define TRUE 1
#define FALSE 0
#define ERROR -1

/* 
Converti la distance en m en cm.

Entree:
    d   Distance (en m) a convertir
    
Sortie:
    Distance arondie en cm
*/
int convKmCm(double d);

/*
Calcule (en cm) toutes les distances entres tous les sites et le point de depart

Entree:
    l   Liste des sites
    lon Longitude du point de depart
    lat lattiude du point de depart
    
Sortie:
    Tableau des distances des sites en eux et avec le point de depart
*/
int** calculToutesDistances(List *l, double lon, double lat, int *taille);

/*
Affiche dans la console une matrice d'entier

Entree:
    tab     matrice carre d'entiers
    taille  taille de la matrice
*/
void afficheTab(int **tab, int taille);

void freeTab(int **tab, int taille);

#endif
