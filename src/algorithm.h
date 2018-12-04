#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "site.h"

/**
 * Calcule (en km) toutes les distances entres tous les sites et le point de depart.
 * @param ldc Liste des sites.
 * @param homeLat Latitude du point de depart.
 * @param lon Longitude du point de depart.
 * @param tabDistLength taille de la matricre retournée.
 * @return matrice des distances des sites en eux et avec le point de depart.
 */
LDC *Algo_Champ_des_Possibles_init(double homeLat, double homeLong, Site **s_tab, int n);

LDC *Algo_Champ_des_Possibles(LDC *l, double **tab_dist, int ind_d, int ind_a, double distance_restant, int n);

LDC *Algo_itineraire(LDC **ldc, double **tabDist, int tabDistLength);

double Algo_score(double **tabDist, int tabDistLength, int difference, Site *potentialSite, double portee, Site *currentSite);

double Algo_score2(LDC *itineraire, double **tabDist, int tabDistLength, int difference, Site *potentialSite, double portee, Site *currentSite);

LDC *Algo_2optSwap(LDC *itineraire, int i, int k); //Pour inversion

double Algo_2opt2(LDC **itineraire, double **tabDist, int tabDistLength); //Par inversion

double Algo_2opt(LDC *itineraire, double **tabDist, int tabDistLength); //Par echange

LDC *Algo_Rand(LDC *ldc, double **tabDist, int tabDistLength);

LDC *Algo_Best_Rand(LDC **ldc, double **tabDist, int tabDistLength);

double Algo_length(LDC *itineraire, double **tabDist, int tabDistLength);

#endif


