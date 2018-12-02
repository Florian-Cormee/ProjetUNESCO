#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "utils.h"
#include "algorithm.h"

LDC* Algo_Plus_Proche_Voisin(LDC* l,double homeLat,double homeLong,long** mat_dist,int n) {
    long distance_restant = MAX_TIME*VITESSE;
    long distance_min = distance_restant;
    long d=0;
    int difference=0;
    int indice = n-1;
    int indice_min = indice;
    int dc;
    int dn;
    LDC* ldc_chemin = LDC_nouveau();
    CelluleLDC* cell;
    Site* s;
    while(l->premier!=NULL) {
        cell = l->premier;
        while(cell!=NULL) {
            d = mat_dist[indice][cell->s->n];
            dc = strcmp(cell->s->categorie,"Cultural");
            dn = strcmp(cell->s->categorie,"Natural");
            if(((difference>-1 && dc!=0)||(difference<1 && dn!=0))&& d<distance_min && d!=0) {
                indice_min = LDC_trouve(l, cell->s);
                distance_min = d;
            }
            cell=cell->suiv;
        }
        s = LDC_get(l, indice_min);
        printf("coucou\n");
        if(!strcmp(s->categorie,"Cultural")) {
            printf("plus\n");
            difference++;
        }
        if(!strcmp(s->categorie,"Natural")) {
            difference--;
            printf("moins\n");
        }
        printf("test2\n");
        LDC_ajoute_fin(ldc_chemin, s);
        indice = s->n;
        printf("%d\n",LDC_taille(l));
        LDC_rm(l, s);
        distance_restant-=(distance_min+BREAK_DIST);
        l = Algo_Champ_des_Possibles(l, mat_dist, indice, distance_restant, n);
    }
    return ldc_chemin;
}

LDC* Algo_Champ_des_Possibles_init(double homeLat, double homeLong, Site** s_tab, int n) {
    LDC* ldc = LDC_nouveau();
    long R0 = convKmCm((double)(MAX_TIME-BREAK_TIME)*(VITESSE/2)); /*A modifier Dr moins Dp*/
    for(int i=0; i<n; i++)
    {
        if(calculDistanceCm(homeLat, homeLong, s_tab[i]->lat, s_tab[i]->lon)<=R0)
        {
            LDC_ajoute_fin(ldc, *(s_tab+i));
        }
    }
    return ldc;
}

LDC* Algo_Champ_des_Possibles(LDC* l, long** tab_dist, int ind,long distance_restant, int n) {
    LDC* ldc_cp = LDC_nouveau();
    CelluleLDC* cell = l->premier;
    while(cell!=NULL)
    {
        if(tab_dist[ind][(cell->s)->n]!=0 && tab_dist[ind][(cell->s)->n]+tab_dist[(cell->s)->n][n-1]<=distance_restant-BREAK_DIST)
        {
            LDC_ajoute_fin(ldc_cp, cell->s);
        }
        cell=cell->suiv;
    }
    return ldc_cp;
}


