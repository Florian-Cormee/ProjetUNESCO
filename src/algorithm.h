#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "site.h"

/* Algorithme du "plus proche voisin" 
    Prend en Parametre la latitude (double LAT) et la longitude (double LONG) de la position de depart
    Retourne Une liste doublement chainée (LDC) avec toute les positions intermédiaires
*/
LDC *Algo_Plus_Proche_Voisin(LDC *l, double homeLat, double homeLong, long **mat_dist, int n);


/* Algorithme du "champs des possibles" initiale
    Prend en Parametre la latitude (double LAT) et la longitude (double LONG) de la position d'origine ainsi que le tableau des Sites (Site* s_tab)
    Retourne Une liste doublement chainée (LDC) avec toute les positions possibles
*/
LDC *Algo_Champ_des_Possibles_init(double homeLat, double homeLong, Site **s_tab, int n);

LDC *Algo_Champ_des_Possibles(LDC *l, long **tab_dist, int ind, long distance_restant, int n);

LDC *Algo_itineraire(LDC **ldc, long **tabDist, int tabDistLength);

long Algo_score(long **tabDist, int tabDistLength, int difference, Site *potentialSite, long portee, Site *currentSite);

long Algo_2opt(LDC *itineraire, long **tabDist, int tabDistLength);

#endif
