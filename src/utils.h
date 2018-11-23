#ifndef UTILS_H
#define UTILS_H

#include "site.h"

#define TRUE (1)
#define FALSE (0)
#define ERROR (-1)

/* 
Converti la distance en m en cm.

Entree:
    d   Distance (en m) a convertir
    
Sortie:
    Distance arondie en cm
*/
long convKmCm(double d);

long calculDistanceCm(double lat1, double lon1, double lat2, double lon2);

/*
Calcule (en cm) toutes les distances entres tous les sites et le point de depart

Entree:
    l   Liste des sites
    lon Longitude du point de depart
    lat lattiude du point de depart
    
Sortie:
    Tableau des distances des sites en eux et avec le point de depart
*/
long **calculToutesDistances(LDC *ldc, double lon, double lat, int *taille);

/*
Affiche dans la console une matrice d'entier

Entree:
    tab     matrice carre d'entiers
    taille  taille de la matrice
*/
void afficheTab(long **tab, int taille);

void freeTab(long **tab, int taille);

long dist(long **tabDist, Site *site1, Site *site2);

int score(LDC *ldc, int printDetails);

#endif
