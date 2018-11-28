#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "site.h"

/* Temps en Heure */
#define MAX_TIME 504
#define BREAK_TIME 6

/* Algorithme du "plus proche voisin" 
    Prend en Parametre la latitude (double LAT) et la longitude (double LONG) de la position de depart
    Retourne Une liste doublement chainée (LDC) avec toute les positions intermédiaires
*/
LDC* Algo_Plus_Proche_Voisin(double LAT, double LONG, Site* s_tab, Site** s_mat, int n);


/* Algorithme du "champs des possibles" initiale
    Prend en Parametre la latitude (double LAT) et la longitude (double LONG) de la position d'origine ainsi que le tableau des Sites (Site* s_tab)
    Retourne Une liste doublement chainée (LDC) avec toute les positions possibles
*/
LDC* Algo_Champ_des_Possibles_init(double LATo, double LONGo, Site** s_tab, int n);

LDC* Algo_Champ_des_Possibles(double LATo, double LONGo, LDC* ldc, double* s_tab, double distance_restant);

#endif
