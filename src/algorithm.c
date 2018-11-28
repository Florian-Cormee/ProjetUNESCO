#include<stdio.h>
#include<stdlib.h>
#include "utils.h"
#include "algorithm.h"

/*LDC* Algo_Plus_Proche_Voisin(double LAT, double LONG, Site* s_tab, Site** s_mat, int n) {
    LDC* ldc_chemin = LDC_nouveau();
    LDC* ldc_portee = Algo_Champ_des_Possibles_init(LAT, LONG, s_tab, n);
    int dif = 0;
    double distance_restant = MAX_TIME*VITESSE, distance_min = 0.0;
    CelluleLDC* cell;
    do {
        cell = ldc_portee->premier;
        while(cell!=NULL) {
            // Choix du plus proche voisin
            cell=cell->suiv;
        }
        ldc_portee = Algo_Champ_des_Possibles(LAT, LONG, ldc_portee, , distance_restant);
    }while(ldc_portee->premier!=NULL);
    return ldc_chemin;
}*/

LDC* Algo_Champ_des_Possibles_init(double LATo, double LONGo, Site** s_tab, int n) {
    LDC* ldc = LDC_nouveau();
    long R0 = convKmCm((double)(MAX_TIME-BREAK_TIME)*(VITESSE/2)); /*A modifier Dr moins Dp*/
    for(int i=0; i<n; i++)
    {
        if(calculDistanceCm(LATo, LONGo, s_tab[i]->lat, s_tab[i]->lon)<=R0)
        {
            LDC_ajoute_fin(ldc, *(s_tab+i));
        }
    }
    return ldc;
}

LDC* Algo_Champ_des_Possibles(double LATo, double LONGo, LDC* ldc, double* s_tab, double distance_restant) {
    LDC* ldc_cp = LDC_nouveau();
    CelluleLDC* cell = ldc->premier;
    while(cell!=NULL)
    {
        if(s_tab[(cell->s)->n]!=0 && s_tab[(cell->s)->n]+calculDistanceCm(LATo, LONGo, (cell->s)->lat, (cell->s)->lon)<=distance_restant-BREAK_DIST)
        {
            LDC_ajoute_fin(ldc_cp, cell->s);
        }
        cell=cell->suiv;
    }
    return ldc;
}


