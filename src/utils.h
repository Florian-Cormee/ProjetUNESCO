#ifndef UTILS_H
#define UTILS_H

#include "site.h"
/**
 * Mode deboggage actif
 */
#define DEBUG (0)

#define TRUE (1)
#define FALSE (0)
#define ERROR (-1)

/**
 * Vitesse de l'helicopter en km/h.
 */
#define VITESSE (80)
/**
 * Duree maximale du voyage en h.
 */
#define MAX_TIME (504)
/**
 * Duree d'une pause en h.
 */
#define BREAK_TIME (6)
/**
 * Distance perdu au cours de la pause en km.
 */
#define BREAK_DIST (VITESSE * BREAK_TIME)

/**
 * Calcule (en km) toutes les distances entres tous les sites et le point de depart.
 * @param ldc Liste des sites.
 * @param homeLat Latitude du point de depart.
 * @param lon Longitude du point de depart.
 * @param tabDistLength taille de la matricre retournée.
 * @return matrice des distances des sites en eux et avec le point de depart.
 */
double **calculToutesDistances(LDC *ldc, double homeLat, double lon, int *tabDistLength); /* Modifier indice des sites */

/**
* Affiche dans la console une matrice d'entier.
* @param tabDist matrice carre d'entiers.
* @param tabDistLength taille de la matrice.
*/

void afficheTab(double **tabDist, int tabDistLength);

/**
 * Libere une matrice carree de la memoire.
 * @param tabDist une matrice carree.
 * @param tabDistLength taille de la matrice.
 */
void freeTab(double **tabDist, int tabDistLength);

/**
 * Calcule le score de l'itineraire fourni.
 * @param ldc un itineraire.
 * @param printDetails (booleen) si TRUE, le detail du score est affiche.
 * @return le score pour cet itineraire.
 */
int score(LDC *ldc, int printDetails);

/**
 * Calcul le nombre de difference entre les sites culturels et naturels.
 * @param ldc un itineraire.
 * @return ecart part rapport a l'equilibre
 */
int difference(LDC *ldc);

/**
 * Affiche l'itineraire dans la console.
 * @param ldc un itineraire.
 * @param homeLat La latitude du point de départ.
 * @param homeLon La longitude du point de départ.
 * @param tabDist Matrice carree des distances entre sites.
 * @param tabSize Taille de la matrice des distances.
 */
void printPath(LDC *ldc, double homeLat, double homeLon, double **tabDist, int tabSize);

/**
 * Exporte l'itineraire sous la forme d'un CSV nomme : Tour.txt
 * @param ldc L'itineraire a exporter.
 * @param homeLat La latitude du point de départ.
 * @param homeLon La longitude du point de départ.
 * @return TRUE en cas de reussite, sinon FALSE.
 */
int pathToFile(LDC *ldc, double homeLat, double homeLon);

/**
 * Exporte en fichier puis affiche l'itineraire en lancant UnescoMap.jar.
 * @param ldc L'itineraire a afficher.
 * @param homeLat La latitude du point de départ.
 * @param homeLon La longitude du point de départ.
 * @return ERROR si ldc est NULL sinon le retour d'UnescoMap.jar.
 */
int showMap(LDC *ldc, double homeLat, double homeLon);

#endif
