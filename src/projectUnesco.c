#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "algorithm.h"
#include "utils.h"
#include "site.h"
#include "algorithm.h"

#define TEST_MODE 0
#if !TEST_MODE

int main(int argc, char *argv[]) {
    srand(time(NULL));
    double homeLat = 0;
    double homeLon = 0;
    int indice = 0;
    Site **siteTab = NULL;
    int siteTabLength = 0;
    double **distTab = NULL;
    int distTabLength = 0;
    LDC *siteLDC = NULL;
    LDC *itineraire = NULL;

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

    siteLDC = Algo_Champ_des_Possibles_init(homeLat, homeLon, siteTab, siteTabLength);

    printf("Calcul des distances entre sites..");
    siteLDC = Algo_Champ_des_Possibles_init(homeLat, homeLon,siteTab, siteTabLength);
    distTab = calculToutesDistances(siteLDC, homeLat, homeLon, &distTabLength);
    printf("Fini..\n");


    printf("Welcome to the Unesco travel challenge!\nChoose the algorithm : ");
    scanf("%d", &indice);
    if (indice == 1) {
    itineraire = Algo_itineraire(&siteLDC, distTab, distTabLength);
    }
    if (indice == 2) {
    itineraire = Algo_Parcours(&siteLDC, distTab, distTabLength);
    }
    if (indice == 3) {
    itineraire = Algo_Recuit_Simule(&siteLDC, distTab, distTabLength);
    }
    printPath(itineraire, homeLat, homeLon, distTab, distTabLength);
    printf("Score total : %4d pts !\n", score(itineraire, 1));

    printf("Writing path to file\n");
    showMap(itineraire, homeLat, homeLon);

    printf("Liberation de la memoire..");
    LDC_free(&siteLDC, 0);
    LDC_free(&itineraire, 0);
    freeTab(distTab, distTabLength);
    Site_tab_supprime(siteTab, siteTabLength);
    printf("Fini\nFin du programme.\n");

    return 0;
}

#endif
