#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <math.h>
#include "haversine.h"
#include "utils.h"
#include "algorithm.h"
#include "limits.h"

LDC *Algo_Plus_Proche_Voisin(LDC *l, double homeLat, double homeLong, double **mat_dist, int n) {
    if (l == NULL || mat_dist == NULL) {
        printf("ERREUR - Algo_Plus_Proche_Voisin\n\tl = %p et mat_dist = %p\n", l, mat_dist);
        return NULL;
    }
    double distance_restant = MAX_TIME * VITESSE;
    double distance_min;
    double d; // distance a l'origine
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
        l = Algo_Champ_des_Possibles(l, mat_dist, indice, distance_restant, n);// recalcul des sites visitable possibles
    }
    return ldc_chemin;
}

LDC *Algo_Champ_des_Possibles_init(double homeLat, double homeLong, Site **s_tab, int n) {
    if (s_tab == NULL) {
        printf("ERREUR - Algo_Champ_des_Possibles_init\n\ts_tab = %p\n", s_tab);
        return NULL;
    }
    LDC *ldc = LDC_nouveau();// liste des sites a porte
    double R0 = (double) (MAX_TIME - BREAK_TIME) * ((double) VITESSE / 2); /*A modifier Dr moins Dp*/
    // porte initiale

    for (int i = 0; i < n; i++) {
        if (s_tab[i] != NULL && calculDistance(homeLat, homeLong, s_tab[i]->lat, s_tab[i]->lon) <= R0) {
            LDC_ajoute_fin(ldc, s_tab[i]);
        }
    }
    return ldc;
}

LDC *Algo_Champ_des_Possibles(LDC *l, double **tab_dist, int ind, double distance_restant, int n) {
    if (l == NULL || tab_dist == NULL) {
        printf("ERREUR - Algo_Champ_des_Possibles\n\tl = %p et tab_dist = %p\n", l, tab_dist);
        return NULL;
    }
    LDC *ldc_cp = LDC_nouveau();// liste des sites a porte
    Site *s = NULL; // valeur de cell

    for (CelluleLDC *cell = l->premier; cell != NULL; cell = cell->suiv) {
        s = cell->s;
        if (s != NULL && (ind != s->n) &&
            (tab_dist[ind][s->n] + tab_dist[s->n][n - 1] + (2 * BREAK_DIST) <= distance_restant)) {
            LDC_ajoute_fin(ldc_cp, s);
        }

    }

    return ldc_cp;
}

double
Algo_score(double **tabDist, int tabDistLength, int difference, Site *potentialSite, double portee, Site *currentSite) {
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

double
Algo_score2(LDC *itineraire, double **tabDist, int tabDistLength, int difference, Site *potentialSite, double portee,
            Site *currentSite) {
    if (tabDist == NULL || potentialSite == NULL) { return LONG_MAX; }
    char *cat = potentialSite->categorie;// categorie de potentialSite.
    int potentialDifference = 0;// ecart à l'equilibre des types de site apporte par potentialSite.
    int deltaScore = 1;// score apporte par le potentialSite.
    double dist;// distance du potentialSite au currentSite.

    if (strcmp(cat, "Cultural") == 0) {
        potentialDifference = abs(difference + 1);
    } else if (strcmp(cat, "Natural") == 0) {
        potentialDifference = abs(difference - 1);
    }
    if (potentialDifference < 2) {
        potentialDifference = -potentialDifference;
    }

    deltaScore = -score(itineraire, FALSE);
    LDC_ajoute_fin(itineraire, potentialSite);
    deltaScore += score(itineraire, FALSE);
    LDC_rm(itineraire, potentialSite);

    if (currentSite != NULL) {
        dist = tabDist[potentialSite->n][currentSite->n];
    } else {
        dist = tabDist[potentialSite->n][tabDistLength - 1];
    }
    //printf("deltaDiff = %d\n", potentialDifference);
    return (1. / deltaScore) * pow(5, potentialDifference) * dist;
}

LDC *Algo_itineraire(LDC **sitesVisitables, double **tabDist, int tabDistLength) {
    if (sitesVisitables == NULL || *sitesVisitables == NULL) {
        printf("ldc est NULL");
        return NULL;
    }
    if (tabDist == NULL) {
        printf("tabDist est NULL");
        return NULL;
    }
    LDC *itineraire = LDC_nouveau();
    LDC *temp = NULL;
    Site *siteMin = NULL; // site possedant le score minimum
    double scoreMin = 0; // score minimum atteint
    double score = 0;
    double portee = MAX_TIME * VITESSE;
    double length = 0;
    int difference = 0;

    while (!LDC_empty(*sitesVisitables)) {
        siteMin = NULL;
        for (CelluleLDC *cell = (*sitesVisitables)->premier; cell != NULL; cell = cell->suiv) {
            score = Algo_score2(itineraire, tabDist, tabDistLength, difference, cell->s, portee,
                                (itineraire->dernier != NULL ? itineraire->dernier->s : NULL));
            if (siteMin == NULL || scoreMin > score) {
                siteMin = cell->s;
                scoreMin = score;
            }
        }
        if (siteMin == NULL) {
            return itineraire;
        }
        //printf("\n\tscore = %lf\n\n", scoreMin);
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
        /*length = Algo_2opt2(&itineraire, tabDist, tabDistLength);
        //if (length != ERROR) {
            portee = MAX_TIME * VITESSE - length;
        //}*/
        temp = (*sitesVisitables);
        *sitesVisitables = Algo_Champ_des_Possibles(*sitesVisitables, tabDist, siteMin->n, portee, tabDistLength);
        LDC_free(&temp, 0);
    }
    //    Algo_2opt(itineraire, tabDist, tabDistLength);
    //Algo_2opt2(&itineraire, tabDist, tabDistLength);
    return itineraire;
}

double Algo_2opt(LDC *itineraire, double **tabDist, int tabDistLength) {
    int upgrade = TRUE;
    Site *site1 = NULL;
    Site *suivSite1 = NULL;
    Site *site2 = NULL;
    Site *suivSite2 = NULL;
    double length = 0;
    Site *home = Site_construire(tabDistLength - 1, NULL, 0, 0, NULL, NULL, FALSE);

    if (itineraire == NULL || tabDist == NULL) {
        return ERROR;
    }

    LDC_ajoute_fin(itineraire, home);

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
    LDC_rm(itineraire, home);
    Site_supprime(home);


    for (CelluleLDC *cell = itineraire->premier; cell != NULL; cell = cell->suiv) {
        if (cell->prec == NULL) {
            length += tabDist[cell->s->n][tabDistLength - 1];
        } else {
            length += tabDist[cell->s->n][cell->prec->s->n];
        }
        length += BREAK_DIST;
        if (cell->suiv == NULL) {
            itineraire->dernier = cell;
        }
    }
    return length;
}

LDC *Algo_2optSwap(LDC *itineraire, int i, int k) {
    if (i >= 0 && k > i && k < LDC_taille(itineraire)) {
        LDC *newItineraire = LDC_nouveau();
        int index = 0;
        CelluleLDC *cell1 = itineraire->premier, *cell2 = NULL;

        while (cell1 != NULL && index < i) {
            index++;
            LDC_ajoute_fin(newItineraire, cell1->s);
            cell1 = cell1->suiv;
        }
        cell2 = cell1;
        if (cell2 != NULL) {
            for (int l = i; l < k && cell2 != NULL; l++) {
                cell2 = cell2->suiv;
            }
            if (cell2 != NULL) {
                cell1 = cell2->suiv;
                for (int l = k; l >= i && cell2 != NULL; l--) {
                    LDC_ajoute_fin(newItineraire, cell2->s);
                    cell2 = cell2->prec;
                }
                while (cell1 != NULL) {
                    LDC_ajoute_fin(newItineraire, cell1->s);
                    cell1 = cell1->suiv;
                }
                return newItineraire;
            }
        }
    }
    //LDC_free(&newItineraire, FALSE);
    return itineraire;

}

double Algo_length(LDC *itineraire, double **tabDist, int tabDistLength) {
    double length = 0;
    for (CelluleLDC *cell = itineraire->premier; cell != NULL; cell = cell->suiv) {
        if (cell->prec == NULL) {
            length += tabDist[cell->s->n][tabDistLength - 1];
        } else {
            length += tabDist[cell->s->n][cell->prec->s->n];
        }
        length += BREAK_DIST;
        if (cell->suiv == NULL) {
            itineraire->dernier = cell;
        }
    }
    return length;
}

double Algo_2opt2(LDC **itineraire, double **tabDist, int tabDistLength) {
    printf("2opt\n");
    int upgrade = TRUE;
    double bestLength = Algo_length(*itineraire, tabDist, tabDistLength);
    double length = 0;
    int i = 0;
    int k = 0;
    LDC *newItineraire;
    if ((*itineraire) == NULL || tabDist == NULL) {
        return ERROR;
    }

    while (upgrade) {
        upgrade = FALSE;
        printf("\tUpgrading..\n");
        i = 0;
        for (CelluleLDC *cell1 = (*itineraire)->premier;
             !upgrade && cell1 != NULL && cell1->suiv != NULL; cell1 = cell1->suiv) {
            k = i + 1;
            printf("cell1 = %p, itienraire = %p\n", cell1, *itineraire);
            for (CelluleLDC *cell2 = cell1->suiv;
                 !upgrade && cell2 != NULL && cell2->suiv != NULL; cell2 = cell2->suiv) {
                printf("\ti = %d, k=%d\n", i, k);
                newItineraire = Algo_2optSwap(*itineraire, i, k);
                length = Algo_length(newItineraire, tabDist, tabDistLength);
                printf("\tlength = %lf \t bestLenght = %lf\n", length, bestLength);
                if (length < bestLength) {
                    LDC_free(itineraire, FALSE);
                    *itineraire = newItineraire;
                    bestLength = length;
                    upgrade = TRUE;
                    printf("bcl 2 = %d, bcl 1 = %d\n", cell2 != NULL && !upgrade, cell1->suiv != NULL && !upgrade);
                    printf("\tOptimisation!\n");
                } else {
                    printf("\t pas opti\n");
                    LDC_free(&newItineraire, FALSE);
                }
                k++;
            }
            i++;
        }
    }
    return bestLength;
}