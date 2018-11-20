#include <stdlib.h>
#include "site.h"
#include "haversine.h"

int convKmCm(double d){
    double nombre = d * 100;
    int dm = (int) nombre;
    if( (nombre - dm) > .5) {
        dm ++;
    }
    return dm;
}

int** calculToutesDistances(List *l, double lon, double lat, int *taille){
    int longueur = List_taille(l);
    int **dist = (int **) malloc(sizeof(int) * (longueur + 1));
    int i = 0; // index de l'iteration mere
    int j = 0; // index de la sous iteration
    double distance; // distance en km entre deux lieux
    
    for(Elem courrant1 = l->premier; courrant1 != NULL; courrant1 = courrant1->suivant){
        dist[i] = (int *) malloc(sizeof(int) * (longueur + 1));
        for(Elem courrant2 = courant1->suivant; courrant2 != NULL; courrant2 = courrant2->suivant){
            distance = calculDistance((courant1->s)->LAT, (courant1->s)->LONG,(courant2->s)->LAT, (courant2->s)->LONG);//distance entre lieux
            dist[i][j] = convKmCm(distance);// conversion en cm
            j++;
        }
        distance = calculDistance((courant1->s)->LAT, (courant1->s)->LONG, lon, lat);//distance entre un lieu et le depart
        dist[i][j+1] = convKmCm(distance);// conversion en cm
        i++;
    }
    *taille = longueur + 1;
    return dist;
}

void afficheTab(int **tab, int taille){
    for(int i = 0; i < taille; i++) {
        printf("|");
        for(int j = 0; j < taille; j++){
            printf("%5d|", tab[i][j]);
        }
        printf("\n");
    }
}

void freeTab(int **tab, int taille){
    for(int i = 0; i < taille; i++){
        free(tab[i]);
    }
    free(tab);
}
