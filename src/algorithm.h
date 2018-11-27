#ifndef ALGORITHM_H
#define ALGORITHM_H

/* Temps en Heure */
#define MAX_TIME 504
#define BREAK_TIME 6
/* Vitesse en Cm/Heure */
#define VITESSE 8000000

/* Algorithme du "plus proche voisin" 
    Prend en Parametre la latitude (double LAT) et la longitude (double LONG) de la position de depart
    Retourne Une liste doublement chainée (LDC) avec toute les positions intermédiaires
*/
LDC* Algo_ppv(double LAT, double LONG);


/* Algorithme du "champs des possibles" initiale
    Prend en Parametre la latitude (double LAT) et la longitude (double LONG) de la position d'origine ainsi que le tableau des Sites (Site* s_tab)
    Retourne Une liste doublement chainée (LDC) avec toute les positions possibles
*/
LDC* Algo_cdpi(double LATo, double LONGo, Site** s_tab,int n);
#endif
