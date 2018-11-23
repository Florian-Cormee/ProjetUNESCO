#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "site.h"
#include "haversine.h"

long convKmCm(double d) {
    double nombre = d * 100;
    int dm = (int) nombre;
    if ((nombre - dm) > .5) {
        dm++;
    }
    return dm;
}

long calculDistanceCm(double lat1, double lon1, double lat2, double lon2) {
    return convKmCm(calculDistance(lat1, lon1, lat2, lon2));
}

long **calculToutesDistances(LDC *l, double lon, double lat, int *taille) {
    int longueur = LDC_taille(l); // longueur de la List l
    long **dist = (long **) malloc(
            sizeof(long *) * (longueur + 1));// matrice carree diagonale des distances entre sites et origine
    int i = 0; // index de l'iteration mere
    int j = 0; // index de la sous iteration
    long distance; // distance en cm entre deux lieux (sites ou origine)

    *taille = longueur + 1; // on indique la taille du tableau renvoye
    for (int i0 = 0; i0 < *taille; ++i0) {
        dist[i0] = (long *) malloc(sizeof(long) * (*taille));//on creer les lignes
        for (int j0 = 0;
             j0 < longueur + 1; ++j0) { dist[i0][j0] = -1; }//on l'initialise avec une valeur par defaut de -1
    }

    for (CelluleLDC *courant1 = l->premier; courant1 != NULL; courant1 = courant1->suiv) {//iteration sur la liste
        j = 0;
        for (CelluleLDC *courant2 = l->premier; courant2 != NULL; courant2 = courant2->suiv) {
            if (dist[i][j] == -1) {//on a pas calcule la distance
                if (courant1 == courant2) {//on veut connaitre la distance entre deux points identiques
                    distance = 0;
                } else {
                    distance = calculDistanceCm((courant1->s)->lat, (courant1->s)->lon, (courant2->s)->lat,
                                                (courant2->s)->lon);//distance entre lieux
                }
                dist[i][j] = distance;
                dist[j][i] = distance;//comme la distance est commutative on evite de recalculer la meme distance
            }
            j++;
        }
        distance = calculDistanceCm((courant1->s)->lat, (courant1->s)->lon, lat,
                                    lon);//distance entre un lieu et le depart
        dist[i][j] = distance;// conversion en cm
        dist[j][i] = distance;//comme la distance est commutative on evite de recalculer la meme distance
        i++;
    }
    dist[(*taille) - 1][(*taille) - 1] = 0;

    return dist;
}

void afficheTab(long **tab, int taille) {
    for (int i = 0; i < taille; i++) {
        printf("|");
        for (int j = 0; j < taille; j++) {
            printf("%10ld|", tab[i][j]);
        }
        printf("\n");
    }
}

void freeTab(long **tab, int taille) {
    for (int i = 0; i < taille; i++) {
        free(*(tab + i));
    }
    free(tab);
}

long dist(long **tabDist, Site *site1, Site *site2) {
    return tabDist[site1->n][site2->n];
}

int score(LDC *ldc, int printDetails) {
    int taille = LDC_taille(ldc);//taille & nombre de destinations
    char *pays[taille];//tableau des pays visites
    int nbPays = 0; // nombre de pays visites differents
    int nbEndangered = 0; // nombre de sites en peril visites
    Site *site = NULL; // site courant
    char *paysSite = NULL; // pays associe au site

    for (int j = 0; j < taille; ++j) {
        pays[j] = NULL;
    }

    for (CelluleLDC *cell = ldc->premier; cell != NULL; cell = cell->suiv) {
        site = cell->s;
        if (site != NULL) {
            paysSite = site->pays;
            int i;
            for (i = 0; i < taille && pays[i] != NULL; ++i) {
                if (!strcmp(pays[i], paysSite)) {//paysSite est present dans tab
                    break;
                }
            }
            if (pays[i] == NULL) {
                // paysSite est absent (on ne verifi pas i car il y a au plus "taille" pays visite differents)
                pays[i] = paysSite;
            }
            if (site->enDanger) {//le site est en peril
                nbEndangered++;
            }
        }
    }

    for (nbPays = 0; nbPays < taille && pays[nbPays] != NULL; nbPays++) {}

    if (printDetails) {
        printf("Evaluation:\n\t%3d destinations x 1 point\n\t%3d pays x 2 points\n\t%3d endangered x 3 points\n",
               taille, nbPays, nbEndangered);
    }
    return taille + 2 * nbPays + 3 * nbEndangered;
}