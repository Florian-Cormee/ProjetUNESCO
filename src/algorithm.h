#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "site.h"

#define T0 1000
#define Tf 9

/* Algorithme du "plus proche voisin" 
    Prend en Parametre la latitude (double LAT) et la longitude (double LONG) de la position de depart
    Retourne Une liste doublement chainée (LDC) avec toute les positions intermédiaires
*/
LDC *Algo_Plus_Proche_Voisin(LDC *l, double homeLat, double homeLong, double **mat_dist, int n);


/* Algorithme du "champs des possibles" initiale
    Prend en Parametre la latitude (double LAT) et la longitude (double LONG) de la position d'origine ainsi que le tableau des Sites (Site* s_tab)
    Retourne Une liste doublement chainée (LDC) avec toute les positions possibles
*/
LDC *Algo_Champ_des_Possibles_init(double homeLat, double homeLong, Site **s_tab, int n);

LDC *Algo_Champ_des_Possibles(LDC *l, double **tab_dist, int ind_d, int ind_a, double distance_restant, int n);

LDC *Algo_itineraire(LDC **ldc, double **tabDist, int tabDistLength);

double
Algo_score(double **tabDist, int tabDistLength, int difference, Site *potentialSite, double portee, Site *currentSite);

double Algo_2opt(LDC *itineraire, double **tabDist, int tabDistLength);

LDC *Algo_Rand(LDC *ldc, double **tabDist, int tabDistLength);

LDC *Algo_Rand_Path(LDC *l, LDC *ldc, int n, int m, double **tabDist, int tabDistLength);

LDC *Algo_Best_Rand(LDC **ldc, double **tabDist, int tabDistLength);

LDC *Algo_Recuit_Simule(LDC **ldc, double **tabDist, int tabDistLength);

double Algo_energie(LDC *l, double **tabDist, int tabDistLength);

LDC *Algo_Metropolis(LDC *l_n, LDC *l_b, int T, double **tabDist, int tabDistLength);

/// TEST ///

Site *Proche(LDC *ldc, int indice, double **tabDist);

Site *Energie(LDC *ldc, int indice, double **tabDist, LDC *l);

Site *Point(LDC *ldc, int indice, double **tabDist, LDC *l);

LDC *Algo_Parcours(LDC **ldc, double **tabDist, int tabDistLength);
#endif


