#include<stdio.h>
#include<stdlib.h>
#include "utils.h"
#include "algorithm.h"

LDC* Algo_Plus_Proche_Voisin(LDC* l, double homeLat, double homeLong, long** mat_dist, int n) {
    if (l == NULL || mat_dist == NULL) {
        printf("ERREUR - Algo_Plus_Proche_Voisin\n\tl = %p et mat_dist = %p\n", l, mat_dist);
        return NULL;
    }
    long distance_restant = MAX_TIME * VITESSE;
    long distance_min = distance_restant;
    long d = 0; // distance a l'origine
    int difference = 0;// difference du nombre de site culturel et naturel visites
    int indice = n - 1;// indice de la distance au point de depart
    int indice_min = 0;// indice du site le plus proche
    int i = 0; // indice de la cellule courante
    LDC* ldc_chemin = LDC_nouveau();// itineraire
    CelluleLDC* cell = NULL; // cellule courante
    Site* s = NULL; // site a visiter
    
    /* Recherche du site le plus proche */
    while (l->premier != NULL) { // tant qu'il y a des sites visitables
        cell = l->premier;
        while (cell != NULL) { // iteration sur la liste des sites
            d = mat_dist[indice][cell->s->n];
            if(d < distance_min && (difference  == 0 || (difference < -1 && strcmp(cell->s->categorie, "Cultural") == 0)||(difference > 1 && strcmp(cell->s->categorie, "Natural") == 0) || strcmp(cell->s->categorie, "Mixed") == 0)) {
            /*Si on est a plus proche sans et que le type du site permette de rester a l'egalite a +-1 pret*/
                indice_min = i;
                distance_min = d;
            }
            cell = cell->suiv;
            i++;
        }
        
        s = LDC_get(l, indice_min);
        if(s == NULL) {
            printf("/!\\ s est NULL pour indice_min = %d\n", indice_min);
        }
        if (strcmp(s->categorie, "Cultural") != 0) {
            printf("plus\n");
            difference++;
        } else if (strcmp(s->categorie, "Natural") != 0) {
            difference--;
            printf("moins\n");
        }
        
        LDC_ajoute_fin(ldc_chemin, s);// ajout du site a l'itineraire
        indice = s->n;// mise a jour de la localisation
        LDC_rm(l, s);// on retire le site de la liste des destinations possibles
        distance_restant -= distance_min + BREAK_DIST;// calcul de la nouvelle distance restante
        l = Algo_Champ_des_Possibles(l, mat_dist, indice, distance_restant, n);// recalcul des sites visitable possibles
    }
    return ldc_chemin;
}

LDC* Algo_Champ_des_Possibles_init(double homeLat, double homeLong, Site** s_tab, int n) {
    if (s_tab == NULL) {
        printf("ERREUR - Algo_Champ_des_Possibles_init\n\ts_tab = %p\n", s_tab);
        return NULL;
    }
    LDC* ldc = LDC_nouveau();// liste des sites a porte
    long R0 = convKmCm((double) (MAX_TIME-BREAK_TIME)*(VITESSE/2)); /*A modifier Dr moins Dp*/
    // porte initiale
    
    for(int i = 0; i < n; i++) {
        if (s_tab[i] != NULL && calculDistanceCm(homeLat, homeLong, s_tab[i]->lat, s_tab[i]->lon) <= R0) {
            LDC_ajoute_fin(ldc, s_tab[i]);
        }
    }
    return ldc;
}

LDC* Algo_Champ_des_Possibles(LDC* l, long **tab_dist, int ind, long distance_restant, int n) {
    if (l != NULL || tab_dist != NULL) {
        printf("ERREUR - Algo_Champ_des_Possibles\n\tl = %p et tab_dist = %p\n", l, tab_dist);
        return NULL;
    }
    LDC* ldc_cp = LDC_nouveau();// liste des sites a porte
    CelluleLDC* cell = l->premier;// cellule courante
    Site *s = NULL; // valeur de cell
    
    while(cell != NULL) {
        s = cell->s;
        if(s != NULL && (ind != s->n) && (tab_dist[ind][s->n] + tab_dist[s->n][n-1] <= distance_restant - BREAK_DIST)) {
            LDC_ajoute_fin(ldc_cp, s);
        }
        cell = cell->suiv;
    }
    
    return ldc_cp;
}


