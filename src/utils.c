#include <stdlib.h>
#include <stdio.h>
#include "site.h"
#include "haversine.h"
void afficheTab(int **tab, int taille);
int convKmCm(double d){
    double nombre = d * 100;
    int dm = (int) nombre;
    if( (nombre - dm) > .5) {
        dm ++;
    }
    return dm;
}

int calculDistanceCm(double lat1, double lon1, double lat2, double lon2){
    return convKmCm(calculDistance(lat1,lon1,lat2,lon2));
}

int** calculToutesDistances(List *l, double lon, double lat, int *taille){
    int longueur = List_taille(l); // longueur de la List l
    int **dist = (int **) malloc(sizeof(int *) * (longueur + 1));// matrice carree diagonale des distances entre sites et origine
    int i = 0; // index de l'iteration mere
    int j = 0; // index de la sous iteration
    int distance; // distance en cm entre deux lieux (sites ou origine)
    
    *taille = longueur + 1; // on indique la taille du tableau renvoye
    for (int i0 = 0; i0 < *taille; ++i0) {
        dist[i0] = (int *) malloc(sizeof(int) * (*taille));//on creer les lignes
        for (int j0 = 0; j0 < longueur+1; ++j0) { dist[i0][j0] = -1; }//on l'initialise avec une valeur par defaut de -1
    }

    for(Elem *courant1 = l->premier; courant1 != NULL; courant1 = courant1->suivant){//iteration sur la liste
        j=0;
        for(Elem *courant2 = l->premier; courant2 != NULL; courant2 = courant2->suivant){
            if(dist[i][j] == -1){//on a pas calcule la distance
                if(courant1 == courant2) {//on veut connaitre la distance entre deux points identiques
                    distance = 0;
                } else {
                    distance = calculDistanceCm((courant1->s)->LAT, (courant1->s)->LONG,(courant2->s)->LAT, (courant2->s)->LONG);//distance entre lieux
                }
                dist[i][j] = distance;
                dist[j][i] = distance;//comme la distance est commutative on evite de recalculer la meme distance
            }
            j++;
        }
        distance = calculDistanceCm((courant1->s)->LAT, (courant1->s)->LONG, lat, lon);//distance entre un lieu et le depart
        dist[i][j] = distance;// conversion en cm
        dist[j][i] = distance;//comme la distance est commutative on evite de recalculer la meme distance
        i++;
    }
    dist[(*taille)-1][(*taille)-1] = 0;

    return dist;
}

void afficheTab(int **tab, int taille){
    for(int i = 0; i < taille; i++) {
        printf("|");
        for(int j = 0; j < taille; j++){
            printf("%10d|", tab[i][j]);
        }
        printf("\n");
    }
}

void freeTab(int **tab, int taille){
    for(int i = 0; i < taille; i++){
        free(*(tab+i));
    }
    free(tab);
}
