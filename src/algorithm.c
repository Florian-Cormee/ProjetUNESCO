#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "haversine.h"
#include "utils.h"
#include "algorithm.h"
#include "limits.h"

LDC *Algo_Champ_des_Possibles_init(double homeLat, double homeLong, Site **s_tab, int n) {
    if (s_tab == NULL) {
        printf("ERREUR - Algo_Champ_des_Possibles_init\n\ts_tab = %p\n", s_tab);
        return NULL;
    }
    LDC *ldc = LDC_nouveau();// liste des sites a porte
    double R0 = (double) (MAX_TIME - BREAK_TIME) * ((double) VITESSE / 2); // porte initiale
    for (int i = 0; i < n; i++) {
        if (s_tab[i] != NULL && calculDistance(homeLat, homeLong, s_tab[i]->lat, s_tab[i]->lon) <= R0) {
            LDC_ajoute_fin(ldc, s_tab[i]);
        }
    }
    return ldc;
}

LDC *Algo_Champ_des_Possibles(LDC *l, double **tab_dist, int ind_d, int ind_a, double distance_restant, int n) {
    if (l == NULL || tab_dist == NULL) {
        printf("ERREUR - Algo_Champ_des_Possibles\n\tl = %p et tab_dist = %p\n", l, tab_dist);
        return NULL;
    }
    LDC *ldc_cp = LDC_nouveau();// liste des sites a porte
    Site *s = NULL; // valeur de cell

    for (CelluleLDC *cell = l->premier; cell != NULL; cell = cell->suiv) {
        s = cell->s;
        if (s != NULL && (ind_d != s->n) && (tab_dist[ind_d][s->n] + tab_dist[s->n][ind_a] + 2 * BREAK_DIST <= distance_restant)) {
            LDC_ajoute_fin(ldc_cp, s);
        }
    }
    return ldc_cp;
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

    LDC *itineraire = LDC_nouveau(); // itineraire a calculer
    LDC *temp = NULL; // variable temporaire pour echanger des valeurs
    Site *siteMin = NULL; // site possedant le score minimum
    double scoreMin = 0; // score minimum atteint
    double score = 0; // score du site teste
    double portee = MAX_TIME * VITESSE; // porte restante
    double length = 0;// longueur de l'itineraire
    int difference = 0;// difference a l'equilibre des type de sites'

    while (!LDC_empty(*sitesVisitables)) {
        siteMin = NULL;// reinitialisation du site minimisant le score
        for (CelluleLDC *cell = (*sitesVisitables)->premier; cell != NULL; cell = cell->suiv) { // iteration sur les sites visitables
            score = Algo_score(itineraire, tabDist, tabDistLength, difference, cell->s, portee,// score du site
                               (itineraire->dernier != NULL ? itineraire->dernier->s : NULL));
            if (siteMin == NULL || scoreMin > score) {// si on a pas encore de siteMin ou le site a un score plus faible
                siteMin = cell->s;// nouveau site minimisant le score
                scoreMin = score;// nouveau score a battre
            }
        }
        if (siteMin == NULL) {// si on a pas de site minimisant le score
            return itineraire;
        }
        LDC_ajoute_fin(itineraire, siteMin);// on ajoute le site a l'itineraire
        if (strcmp(siteMin->categorie, "Cultural") == 0) {// si le site est culturel
            difference++;
        } else if (strcmp(siteMin->categorie, "Natural") == 0) { // si le site est naturel
            difference--;
        }
        length = Algo_2opt2(&itineraire, tabDist, tabDistLength);// optimisation du trajet
        portee = MAX_TIME * VITESSE - length;// calcul de la porte apres optimisation
        temp = (*sitesVisitables);// sauvegarde des sites precedemment visitables
        *sitesVisitables = Algo_Champ_des_Possibles(*sitesVisitables, tabDist, siteMin->n, tabDistLength - 1, portee, tabDistLength);// determination des sites visitables depuis le dernier site visite
        LDC_free(&temp, 0);// liberation de l'ancien champ des possibles
    }
    return itineraire;
}

double Algo_scorePlusProche(double **tabDist, int tabDistLength, int difference, Site *potentialSite, double portee, Site *currentSite) {
    if (tabDist == NULL || potentialSite == NULL) { return LONG_MAX; }
    char *cat = potentialSite->categorie;// categorie du site potentiel

    if (difference > 1 && strcmp(cat, "Cultural") == 0) {// si on a deja plus d'1 d'equart
        return LONG_MAX;
    } else if (difference < -1 && strcmp(cat, "Natural") == 0) {// si on a deja plus d'1 d'equart
        return LONG_MAX;
    }
    if (currentSite != NULL) {// si on n'est pas au point de depart
        return tabDist[potentialSite->n][currentSite->n];// distance entre les points
    } else {
        return tabDist[potentialSite->n][tabDistLength - 1];// distance a l'origine
    }
}

double Algo_score(LDC *itineraire, double **tabDist, int tabDistLength, int difference, Site *potentialSite, double portee, Site *currentSite) {
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
    if (potentialDifference < abs(difference)) {//si le site nous approche de l'equilibre
        potentialDifference = -potentialDifference;//minimisation du score
    }

    deltaScore = -score(itineraire, FALSE);
    LDC_ajoute_fin(itineraire, potentialSite);
    deltaScore += score(itineraire, FALSE);//score supplementaire si on choisi ce site
    LDC_rm(itineraire, potentialSite);

    if (currentSite != NULL) {// si on n'est pas au point de depart
        dist = tabDist[potentialSite->n][currentSite->n];// distance au point precedant
    } else {
        dist = tabDist[potentialSite->n][tabDistLength - 1];// distance a l'origine
    }
    return (1. / deltaScore) * pow(5, potentialDifference) * dist;
}

LDC *Algo_Rand(LDC *ldc, double **tabDist, int tabDistLength) {
    LDC *ldc_rand;
    LDC *ldc_cp;
    int taille;
    double i, t = 2, q = 1 / t;
    int indice_prev;
    long distance_restant;
    Site *s;
    do {
        indice_prev = tabDistLength - 1;
        i = 1;
        ldc_cp = ldc;
        ldc_cp = Algo_Champ_des_Possibles(ldc_cp, tabDist, indice_prev, tabDistLength - 1, distance_restant * (i / t), tabDistLength);
        taille = LDC_taille(ldc_cp);
        ldc_rand = LDC_nouveau();
        distance_restant = MAX_TIME * VITESSE;
        while (taille > 0 || i < t) {
            if (taille > 0) {
                printf("distance false : %lf\n", distance_restant * (i / t));
                printf("Taille : %d\n", taille);
                s = LDC_get(ldc_cp, rand() % taille);
                LDC_ajoute_fin(ldc_rand, s);
                LDC_rm(ldc_cp, s);
                distance_restant -= tabDist[s->n][indice_prev] + BREAK_DIST;
                indice_prev = s->n;
                if (i < t) { i += q; }
                ldc_cp = Algo_Champ_des_Possibles(ldc, tabDist, indice_prev, tabDistLength - 1, distance_restant * (i / t), tabDistLength);
                taille = LDC_taille(ldc_cp);
            } else {
                if (i < t) { i += q; }
            }
        }
        printf("Sortie\n");
    } while (!LDC_empty(ldc_rand) || fabs(difference(ldc_rand)) > 1);
    LDC_free(&ldc_cp, 0);
    return ldc_rand;
}

/*
LDC *Algo_Best_Rand(LDC **ldc, double **tabDist, int tabDistLength) {
    LDC* ldc_rand_min = Algo_Rand(*ldc, tabDist, tabDistLength);
    printf("coucou\n");
    ldc_rand_min = Algo_itineraire(&ldc_rand_min, tabDist, tabDistLength);
    Algo_2opt(ldc_rand_min, tabDist, tabDistLength);
    LDC* ldc_rand;
    clock_t t1;
    t1 = clock();
    do {
        ldc_rand = Algo_Rand(*ldc, tabDist, tabDistLength);
        Algo_2opt(ldc_rand, tabDist, tabDistLength);
        if(score(ldc_rand_min, 0)<score( ldc_rand, 0))
        {
            ldc_rand_min = ldc_rand;
        }
    }while((float)(clock()-t1)<30.0);
    printf("%lf\n", Algo_energie(ldc_rand_min, tabDist, tabDistLength));
    return ldc_rand_min;
}
*/
double Algo_longueur(LDC *itineraire, double **tabDist, int tabDistLength) {
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

double Algo_2opt2(LDC **itineraire, double **tabDist, int tabDistLength) {
    printf("2opt2\n");
    int upgrade = TRUE;
    double bestLength = Algo_longueur(*itineraire, tabDist, tabDistLength);
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
        for (CelluleLDC *cell1 = (*itineraire)->premier; !upgrade && cell1 != NULL && cell1->suiv != NULL; cell1 = cell1->suiv) {
            k = i + 1;
            printf("cell1 = %p, itienraire = %p\n", cell1, *itineraire);
            for (CelluleLDC *cell2 = cell1->suiv; !upgrade && cell2 != NULL && cell2->suiv != NULL; cell2 = cell2->suiv) {
                printf("\ti = %d, k=%d\n", i, k);
                newItineraire = Algo_2optSwap(*itineraire, i, k);
                length = Algo_longueur(newItineraire, tabDist, tabDistLength);
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