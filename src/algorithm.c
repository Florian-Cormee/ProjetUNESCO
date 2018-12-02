#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "utils.h"
#include "algorithm.h"
#include "limits.h"

LDC *Algo_Plus_Proche_Voisin(LDC *l, double homeLat, double homeLong, long **mat_dist, int n) {
    if (l == NULL || mat_dist == NULL) {
        printf("ERREUR - Algo_Plus_Proche_Voisin\n\tl = %p et mat_dist = %p\n", l, mat_dist);
        return NULL;
    }
    long distance_restant = MAX_TIME * VITESSE;
    long distance_min;
    long d; // distance a l'origine
    int difference = 0;// difference du nombre de site culturel et naturel visites
    int indice = n - 1;// indice de la distance au point de depart
    int indice_min;// indice du site le plus proche
    int i = 0; // indice de la cellule courante
    LDC *ldc_chemin = LDC_nouveau();// itineraire
    CelluleLDC *cell = NULL; // cellule courante
    Site *s = NULL; // site a visiter

    /* Recherche du site le plus proche */
    while (l->premier != NULL) { // tant qu'il y a des sites visitables
        distance_min = distance_restant;
        indice_min = -1;
        i = 0;
        cell = l->premier;
        while (cell != NULL) { // iteration sur la liste des sites
            d = mat_dist[indice][cell->s->n];
            if (d < distance_min) {
                //&& (difference  == 0 || (difference < -1 && strcmp(cell->s->categorie, "Cultural") == 0)||(difference > 1 && strcmp(cell->s->categorie, "Natural") == 0) || strcmp(cell->s->categorie, "Mixed") == 0)) {
                /*Si on est a plus proche sans et que le type du site permette de rester a l'egalite a +-1 pret*/
                indice_min = i;
                distance_min = d;
            }
            cell = cell->suiv;
            i++;
        }
        if (indice_min == -1) {
            return ldc_chemin;
        }

        s = LDC_get(l, indice_min);

        if (s == NULL) {
            printf("/!\\ s est NULL pour indice_min = %d\n", indice_min);
        } else {
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
        }
        l = Algo_Champ_des_Possibles(l, mat_dist, indice, n-1, distance_restant, n);// recalcul des sites visitable possibles
    }
    return ldc_chemin;
}

LDC *Algo_Champ_des_Possibles_init(double homeLat, double homeLong, Site **s_tab, int n) {
    if (s_tab == NULL) {
        printf("ERREUR - Algo_Champ_des_Possibles_init\n\ts_tab = %p\n", s_tab);
        return NULL;
    }
    LDC *ldc = LDC_nouveau();// liste des sites a porte
    long R0 = convKmCm((double) (MAX_TIME - BREAK_TIME) * (VITESSE / 2)); // porte initiale
    for (int i = 0; i < n; i++) {
        if (s_tab[i] != NULL && calculDistanceCm(homeLat, homeLong, s_tab[i]->lat, s_tab[i]->lon) <= R0) {
            LDC_ajoute_fin(ldc, s_tab[i]);
        }
    }
    return ldc;
}

LDC *Algo_Champ_des_Possibles(LDC *l, long **tab_dist, int ind_d, int ind_a, long distance_restant, int n) {
    if (l == NULL || tab_dist == NULL) {
        printf("ERREUR - Algo_Champ_des_Possibles\n\tl = %p et tab_dist = %p\n", l, tab_dist);
        return NULL;
    }
    LDC* ldc_cp = LDC_nouveau();// liste des sites a porte
    Site *s = NULL; // valeur de cell

    for (CelluleLDC *cell = l->premier; cell != NULL; cell = cell->suiv) {
        s = cell->s;
        if (s != NULL && (ind_d != s->n) &&
            (tab_dist[ind_d][s->n] + tab_dist[s->n][ind_a] + BREAK_DIST <= distance_restant)) {
            LDC_ajoute_fin(ldc_cp, s);
        }
    }
    return ldc_cp;
}

long Algo_score(long **tabDist, int tabDistLength, int difference, Site *potentialSite, long portee, Site *currentSite) {
    if (tabDist == NULL || potentialSite == NULL) { return LONG_MAX; }
    char *cat = potentialSite->categorie;

    if (difference > 1 && strcmp(cat, "Cultural") == 0) {
        return LONG_MAX;
    } else if (difference < -1 && strcmp(cat, "Natural") == 0) {
        return LONG_MAX;
    }
    if (currentSite != NULL) {
        /*if (tabDist[currentSite->n][potentialSite->n] + tabDist[potentialSite->n][tabDistLength - 1] + 2 * BREAK_DIST >
            portee) {
            return LONG_MAX;
        } else {*/
        return tabDist[potentialSite->n][currentSite->n];
        //}
    } else {
        return tabDist[potentialSite->n][tabDistLength - 1];
    }
}

LDC *Algo_itineraire(LDC **sitesVisitables, long **tabDist, int tabDistLength) {
/* Verification de la liste ainsi que du tableau des distances */
    if (sitesVisitables == NULL || *sitesVisitables == NULL) {
        printf("ldc est NULL");
        return NULL;
    }
    if (tabDist == NULL) {
        printf("tabDist est NULL");
        return NULL;
    }
/* Initialisation variable */
    LDC *itineraire = LDC_nouveau();
    LDC *temp = NULL;
    Site *siteMin = NULL; // site possedant le score minimum
    long scoreMin = 0; // score minimum atteint
    long score = 0;
    long portee = MAX_TIME * VITESSE * 100;
    long length = 0;
    int difference = 0;
/* Debut de l'Algorithme */
    while (!LDC_empty(*sitesVisitables)) {
        siteMin = NULL;
        for (CelluleLDC *cell = (*sitesVisitables)->premier; cell != NULL; cell = cell->suiv) {
            score = Algo_score(tabDist, tabDistLength, difference, cell->s, portee,
                               (itineraire->dernier != NULL ? itineraire->dernier->s : NULL));
            if (siteMin == NULL || scoreMin > score) {
                siteMin = cell->s;
                scoreMin = score;
            }
        }
        if (siteMin == NULL) {
            return itineraire;
        }
        if (LDC_empty(itineraire)) {
            portee -= tabDist[siteMin->n][tabDistLength - 1];
        } else {
            portee -= tabDist[siteMin->n][itineraire->dernier->s->n];
        }
        portee -= BREAK_DIST;
        LDC_ajoute_fin(itineraire, siteMin);
        if (strcmp(siteMin->categorie, "Cultural") == 0) {
            difference++;
        } else if (strcmp(siteMin->categorie, "Natural") == 0) {
            difference--;
        }
        length = Algo_2opt(itineraire, tabDist, tabDistLength);
        if (length != ERROR) {
            portee = MAX_TIME * VITESSE * 100 - length;
        }
        temp = (*sitesVisitables);
        *sitesVisitables = Algo_Champ_des_Possibles(*sitesVisitables, tabDist, siteMin->n, tabDistLength-1, portee, tabDistLength);
        LDC_free(&temp, 0);
    }
    return itineraire;
}

long Algo_2opt(LDC *itineraire, long **tabDist, int tabDistLength) {
    int upgrade = TRUE;
    Site *site1 = NULL;
    Site *suivSite1 = NULL;
    Site *site2 = NULL;
    Site *suivSite2 = NULL;
    long length = 0;
    if (itineraire == NULL || tabDist == NULL) {
        return ERROR;
    }
    while (upgrade) {
        upgrade = FALSE;
        for (CelluleLDC *cell1 = itineraire->premier; cell1 != NULL; cell1 = cell1->suiv) {
            site1 = cell1->s;
            for (CelluleLDC *cell2 = itineraire->premier; cell2 != NULL; cell2 = cell2->suiv) {
                site2 = cell2->s;
                if (site1->n - 1 <= site2->n && site2->n <= site1->n + 1) {
                    continue;
                }
                if (cell1->suiv != NULL && cell2->suiv != NULL && cell1->suiv->s != NULL && cell2->suiv->s != NULL &&
                    tabDist[site1->n][cell1->suiv->s->n] + tabDist[site2->n][cell2->suiv->s->n] >
                    tabDist[site1->n][site2->n] + tabDist[cell1->suiv->s->n][cell2->suiv->s->n]) {
                    //printf("\tOptimisation!\n");
                    suivSite1 = cell1->suiv->s;
                    suivSite2 = cell2->suiv->s;
                    cell1->suiv->s = site2;
                    cell2->s = suivSite1;
                    cell2->suiv->s = suivSite2;
                    upgrade = TRUE;
                }
            }
        }
    }
    for (CelluleLDC *cell = itineraire->premier; cell != NULL; cell = cell->suiv) {
        if (cell->prec == NULL) {
            length += tabDist[cell->s->n][tabDistLength - 1];
        } else {
            length += tabDist[cell->s->n][cell->prec->s->n];
        }
        length += BREAK_DIST;
    }
    return length;
}

LDC *Algo_Rand(LDC *ldc, long **tabDist, int tabDistLength) {
    LDC *ldc_rand;
    LDC *ldc_cp;
    int taille;
    int indice_prev;
    long distance_restant;
    Site *s;
    do {
    taille = LDC_taille(ldc);
    ldc_cp = ldc;
    ldc_rand = LDC_nouveau();
    distance_restant = MAX_TIME*VITESSE*100;
    indice_prev = tabDistLength-1;
        while(taille>0)
        {
            s = LDC_get(ldc_cp, rand()%taille);
            LDC_ajoute_fin(ldc_rand, s);
            LDC_rm(ldc_cp, s);
            distance_restant-= tabDist[s->n][indice_prev]+BREAK_DIST;
            indice_prev = s->n;
            ldc_cp = Algo_Champ_des_Possibles(ldc_cp, tabDist, indice_prev, tabDistLength-1, distance_restant, tabDistLength);
            taille = LDC_taille(ldc_cp);
        }
    }while(fabs(difference(ldc_rand))>1);
    LDC_free(&ldc_cp, 0);
    return ldc_rand;
}

LDC *Algo_Best_Rand(LDC **ldc, long **tabDist, int tabDistLength) {
    LDC* ldc_rand_min = Algo_Rand(*ldc, tabDist, tabDistLength);
    ldc_rand_min = Algo_itineraire(&ldc_rand_min, tabDist, tabDistLength);
    Algo_2opt(ldc_rand_min, tabDist, tabDistLength);
    LDC* ldc_rand;
    clock_t t1;
    t1 = clock();
    do {
        ldc_rand = Algo_Rand(*ldc, tabDist, tabDistLength);
        ldc_rand = Algo_itineraire(&ldc_rand, tabDist, tabDistLength);
        Algo_2opt(ldc_rand, tabDist, tabDistLength);
        if(score(ldc_rand_min, 0)<score( ldc_rand, 0))
        {
            ldc_rand_min = ldc_rand;
        }
    }while((float)(clock()-t1)<30000.0);
    return ldc_rand_min;
}
