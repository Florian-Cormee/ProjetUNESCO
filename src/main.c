#include<stdio.h>
#include<stdlib.h>

#include "algorithm.h"
#include "site.h"
#include "utils.h"

int main(int argc, char* argv[]) {
/* Initialisation */
    int size = 0, indice = 0;
    if(argc != 3){return 1;} // Verifie si les parametres sont bien places
    double homeLat = atof(argv[1]);
    double homeLong = atof(argv[2]);
    Site** s = Site_tab_init(&size); // Tableau des sites
    LDC* l_s = Algo_Champ_des_Possibles_init(homeLat, homeLong, s, size); // Liste des sites accessibles
    long** mat_dist = calculToutesDistances(l_s, homeLat, homeLong, &size); // Matrice des distances
/* Debut du programme */
    printf("Welcome to the Unesco travel challenge!\nChoose the algorithm : ");
    scanf("%d", &indice);
    if(indice==1) {
        l_s = Algo_Plus_Proche_Voisin(l_s, homeLat, homeLong, mat_dist, size);
    }
    printPath(l_s, homeLat, homeLong, mat_dist, size);
    showMap(l_s, homeLat, homeLong);
    LDC_free(&l_s, TRUE);
    Site_tab_supprime(s, size);
    return 0;
}
