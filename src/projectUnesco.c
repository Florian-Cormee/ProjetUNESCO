#include <stdio.h>
#include "algorithm.h"
#include "utils.h"
#include "site.h"

#define TEST_MODE 0

#if !TEST_MODE

int main(int argc, char *argv[]) {
    double homeLat = 0;
    double homeLon = 0;
    int indice = 0;
    Site **siteTab = NULL;
    int siteTabLength = 0;
    long **distTab = NULL;
    int distTabLength = 0;
    LDC *siteLDC = NULL;

    if (argc != 3) {
        printf("Il ne faut que les coordonnées du point de départ.\nUsage :\t./unesco <latitude> <longitude>\n");
        return 1;
    }

    sscanf(argv[1], "%lf", &homeLat);
    sscanf(argv[2], "%lf", &homeLon);

    printf("Point de départ : (%lf, %lf)\n", homeLat, homeLon);

    printf("Chargement des sites..");
    siteTab = Site_tab_init(&siteTabLength);
    printf("%d sites chargees..\n", siteTabLength);

    printf("Calcul des distances entre sites..");
    siteLDC = Algo_Champ_des_Possibles_init(homeLat, homeLon,siteTab, siteTabLength);
    distTab = calculToutesDistances(siteLDC, homeLat, homeLon, &distTabLength);
    printf("Fini..\n");
    printf("Welcome to the Unesco travel challenge!\nChoose the algorithm : ");
    scanf("%d", &indice);
    if(indice==1) {
        siteLDC = Algo_Plus_Proche_Voisin(siteLDC, homeLat, homeLon, distTab, distTabLength);
    }

    printPath(siteLDC, homeLat, homeLon, distTab, distTabLength);
    printf("Score total : %4d pts !\n", score(siteLDC, 1));

    showMap(siteLDC, homeLat, homeLon);

    printf("Liberation de la memoire..");
    Site_tab_supprime(siteTab, siteTabLength);
    freeTab(distTab, distTabLength);
    LDC_free(&siteLDC, 1);
    printf("Fini\nFin du programme.\n");

    return 0;
}

#endif
