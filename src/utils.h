#ifndef UTILS_H
#define UTILS_H

#include "site.h"

#define DEBUG (0)

#define TRUE (1)
#define FALSE (0)
#define ERROR (-1)

#define VITESSE (80)
#define MAX_TIME (504)
#define BREAK_TIME (6)
#define BREAK_DIST (VITESSE * BREAK_TIME)

/**
 * Calcule (en km) toutes les distances entres tous les sites et le point de depart.
 *
 * @param ldc Liste des sites
 * @param lat Lattiude du point de depart
 * @param lon Longitude du point de depart
 * @param taille taille de la matricre retournée
 * @return matrice des distances des sites en eux et avec le point de depart
 */
double **calculToutesDistances(LDC *ldc, double lat, double lon, int *taille); /* Modifier indice des sites */

/**
* Affiche dans la console une matrice d'entier.
*
* @param tab matrice carre d'entiers
* @param taille taille de la matrice
*/
void afficheTab(double **tab, int taille);

void freeTab(double **tab, int taille);

int score(LDC *ldc, int printDetails);

void printPath(LDC *ldc, double homeLat, double homeLong, double **tabDist, int tabSize);

int pathToFile(LDC *ldc, double homeLat, double homeLong);

int showMap(LDC *ldc, double homeLat, double homeLong);

#endif
