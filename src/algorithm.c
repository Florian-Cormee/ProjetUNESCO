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
    Algo_2opt(itineraire, tabDist, tabDistLength);
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

double Algo_2opt(LDC *itineraire, double **tabDist, int tabDistLength) {
    printf("2opt!\n");
    int upgrade = TRUE;// une amelioration a t elle ete trouve
    Site *site1 = NULL; // site courant
    Site *suivSite1 = NULL;// site suivant le courant
    Site *site2 = NULL;// site a comparer
    Site *suivSite2 = NULL;// site suivant site2
    double minLength = Algo_longueur(itineraire, tabDist, tabDistLength);// longueur minimal optenue
    double length = 0;// longueur du chemin
    Site *home = Site_construire(tabDistLength - 1, NULL, 0, 0, NULL, NULL, FALSE);// site pour representer le point de depart

    if (itineraire == NULL || tabDist == NULL) {
        return ERROR;
    }
    LDC_ajoute_fin(itineraire, home);// on ajoute la maison pour "clore" le tour
    while (upgrade) {
        upgrade = FALSE;// on s'assure de ne pas boucler a l'infini
        for (CelluleLDC *cell1 = itineraire->premier; !upgrade && cell1 != NULL; cell1 = cell1->suiv) {
            site1 = cell1->s;
            for (CelluleLDC *cell2 = itineraire->premier; !upgrade && cell2 != NULL; cell2 = cell2->suiv) {
                site2 = cell2->s;
                if (site1->n - 1 <= site2->n && site2->n <= site1->n + 1) {//si site2 est adjacent a site1
                    continue;
                }
                if (cell1->suiv != NULL && cell2->suiv != NULL && cell1->suiv->s != NULL && cell2->suiv->s != NULL) {// si tous les sites sont definis
                    suivSite1 = cell1->suiv->s;
                    suivSite2 = cell2->suiv->s;
                    // echange des segments
                    cell1->suiv->s = site2;
                    cell2->s = suivSite1;
                    cell2->suiv->s = suivSite2;
                    length = Algo_longueur(itineraire, tabDist, tabDistLength);//longueur du nouveau chemin
                    if (minLength > length) {//si on a optimise
                        upgrade = TRUE;
                        minLength = length;
                        printf("optimisation!\n");
                    } else {
                        // on annule les changements
                        cell2->s = site2;
                        cell2->suiv->s = suivSite2;
                        cell1->suiv->s = suivSite1;
                    }
                }
            }
        }
    }
    LDC_rm(itineraire, home);//on retire le site representant le point de depart
    Site_supprime(home);//on libere le point de depart
    return minLength;
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